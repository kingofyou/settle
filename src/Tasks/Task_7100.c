/*****************************************************************************
 *  Copyright 2011, Shanghai Huateng Software Systems Co., Ltd.
 *  All right reserved.
 *
 *  THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF SHANGHAI HUATENG
 *  SOFTWARE SYSTEMS CO., LTD.  THE CONTENTS OF THIS FILE MAY NOT
 *  BE DISCLOSED TO THIRD PARTIES, COPIED OR DUPLICATED IN ANY FORM,
 *  IN WHOLE OR IN PART, WITHOUT THE PRIOR WRITTEN PERMISSION OF
 *  SHANGHAI HUATENG SOFTWARE SYSTEMS CO., LTD.
 *
 *  文 件 名: Task_7100.c
 *  功    能: 虚拟卡交易记录生成划账文件
 *  编程人员: bcw
 *  开发时间: 2017308
 *  备    注:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include <sys/socket.h>
#include "batch.h"
#include "Common.h"

extern  char    gLogFile[LOG_NAME_LEN_MAX];
extern  char    ext_child_date[8 + 1];
extern  char    ext_inter_brh_code[10 + 1];
extern  int     ext_inter_brh_sta;
extern          tbl_date_inf_def dbtbl_date_inf;

int VarRecdToPlat(char* strBuf, char cTab, tbl_sett_list_def* vtdbl_sett_list);

int iInsertTblFileInfo(tbl_file_info_def   dbtbl_file_info)
{
    int iRet = 0;
    HtLog(gLogFile,HT_LOG_MODE_DEBUG,__FILE__,__LINE__,
        "view the data dbtbl_file_info [%s][%s][%s][%s][%s][%s][%s]",dbtbl_file_info.sett_date,dbtbl_file_info.bank_code,dbtbl_file_info.file_type,dbtbl_file_info.file_flag,dbtbl_file_info.step_no,dbtbl_file_info.batch_no,dbtbl_file_info.file_name);
   
    iRet = DbsTblFileInfo(DBS_SELECT, &dbtbl_file_info);
    if(iRet == DBS_NOTFOUND)
    {
        iRet = DbsTblFileInfo(DBS_INSERT, &dbtbl_file_info);
        if(iRet)
        {
            HtLog(gLogFile,HT_LOG_MODE_ERROR,__FILE__,__LINE__,
                "DbsTblFileInfo DBS_INSERT error!:[%d]", iRet);
            return iRet;
        }
    }
    else if(iRet)
    {
        HtLog(gLogFile,HT_LOG_MODE_ERROR,__FILE__,__LINE__,
            "DbsTblFileInfo DBS_SELECT error!:[%d]", iRet);
        return iRet;
    }
    else
    {
        CommonRTrim(dbtbl_file_info.sett_date);
        CommonRTrim(dbtbl_file_info.file_name);
        iRet = DbsTblFileInfo(DBS_UPDATE, &dbtbl_file_info);
        if(iRet)
        {
            HtLog(gLogFile,HT_LOG_MODE_ERROR,__FILE__,__LINE__,
                      "DbsTblFileInfo DBS_UPDATE error!");
            return iRet;
        }
    }    
    return 0;
}

/*************************************************
*函数名称∶ Total_7100
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_7100()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_7100
*函数功能∶ 结算明细表虚拟卡记录生成划账文件
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_7100 ( int nBeginOffset, int nEndOffset )
{
    int     nReturnCode;
    char    sDateTime[14+1];
    char    stlm_dt[8+1];
    char    *pstr;
    char    sFileRecord[RECORD_LEN_MAX];
    FILE    *fp;
    char    sFileFullName[200];
    char    sTmpFileName[100];
    int		nTotalCount = 0;
    
    tbl_sett_list_def   dbtbl_sett_list;
    tbl_file_info_def   dbtbl_file_info;

    /* 初始文件信息 */
    memset(&dbtbl_file_info, 0x00, sizeof(dbtbl_file_info));
    memset(sDateTime, 0x00, sizeof(sDateTime));
    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
    
    getSysTime(sDateTime);
    memcpy(dbtbl_file_info.sett_date, stlm_dt, 8);
    memcpy(dbtbl_file_info.snd_time, sDateTime, 14);
    
    memset(sTmpFileName, 0, sizeof(sTmpFileName));
    strcpy(sTmpFileName, "HOST_YYYYMMDD_XX_ACCT.txt");
    pstr = strstr(sTmpFileName, "YYYYMMDD");
    if (pstr)
        memcpy(pstr, stlm_dt, 8);
    pstr = strstr(sTmpFileName, "XX");
    if (pstr)
        memcpy (pstr, "01", 2);
    memcpy(dbtbl_file_info.step_no, "04", 2);
    memcpy(dbtbl_file_info.batch_no, "01", 2);
    memcpy(dbtbl_file_info.file_type, "H", 1);
    memcpy(dbtbl_file_info.file_flag, "S", 1);
    memcpy(dbtbl_file_info.end_flag, "N", 1);
    memcpy(dbtbl_file_info.bank_code, "HOST", 4);
    
    memset(sFileFullName, 0, sizeof(sFileFullName));
    sprintf(sFileFullName, "%s/%8.8s/%s/%s/%s/%s%s/",
            getenv("SHARE_FILE_PATH"),
            dbtbl_file_info.sett_date,
            dbtbl_file_info.bank_code,
            dbtbl_file_info.file_type,
            dbtbl_file_info.file_flag,
            dbtbl_file_info.step_no,
            dbtbl_file_info.batch_no);
    nReturnCode = CheckDir(sFileFullName);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "CheckDir err[%d][%s][%s]",
               nReturnCode, strerror(errno), sFileFullName);
        return -1;
    }
    strcat(sFileFullName, sTmpFileName);
    memcpy(dbtbl_file_info.file_name, sFileFullName, strlen(sFileFullName));
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name : [%s]", dbtbl_file_info.file_name);
    
    fp = fopen(dbtbl_file_info.file_name,"w+");
    if (fp == NULL)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen [%s] error.", dbtbl_file_info.file_name);
        return -1;
    }

    memset(&dbtbl_sett_list, 0x00, sizeof(dbtbl_sett_list));
    memcpy(dbtbl_sett_list.sett_date, stlm_dt, 8);
    memcpy(dbtbl_sett_list.mcht_sett_acct_type, "3", 1);
    memcpy(dbtbl_sett_list.status, "2", 1);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date:[%s], mcht_sett_acct_type:[%s], status:[%s]\n", dbtbl_sett_list.sett_date, dbtbl_sett_list.mcht_sett_acct_type, dbtbl_sett_list.status);
    /* 游标，结算明细表 */
    DbsTblSettList(DBS_CURSOR3, &dbtbl_sett_list);
    nReturnCode = DbsTblSettList(DBS_OPEN3, &dbtbl_sett_list);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN3 ht_tbl_sett_list_cur3 error [%d]", nReturnCode);
        fclose(fp);
        return -1;
    }
    
    while(1)
    {
        /* fetch 每条未入账的记录 */
        memset(&dbtbl_sett_list, 0, sizeof(dbtbl_sett_list));
        nReturnCode = DbsTblSettList(DBS_FETCH3, &dbtbl_sett_list);
        if (nReturnCode == DBS_NOTFOUND)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_FETCH3 ht_tbl_sett_list_cur3 end.");
            DbsTblSettList(DBS_CLOSE3, &dbtbl_sett_list);
            break;
        }
        else if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_FETCH3 ht_tbl_sett_list_cur3 error [%d]", nReturnCode);
            DbsTblSettList(DBS_CLOSE3, &dbtbl_sett_list);
            return -1;
        }
        nTotalCount++;
        /* 将记录信息写入文件中 */
        memset(sFileRecord, 0x00, sizeof(sFileRecord));
        VarRecdToPlat(sFileRecord, '|', &dbtbl_sett_list);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "Record:[%s]", sFileRecord);
        nReturnCode = fputs(sFileRecord, fp);
        if(nReturnCode>=0 && nReturnCode != EOF)
        {
	        memcpy(&dbtbl_sett_list.status, "6", 1);
	        RightTrim(dbtbl_sett_list.seq_no);
	        /* 更新结算明细表记录 */
	        nReturnCode = DbsTblSettList(DBS_UPD_SEQ, &dbtbl_sett_list);
	        if (nReturnCode)
	        {
	            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPD_SEQ ht_tbl_acct_rslt error [%d]", nReturnCode);
	            DbsTblSettList(DBS_CLOSE3, &dbtbl_sett_list);
	            return -1;
	        }
	    }
    }
    DbsTblSettList(DBS_CLOSE3, &dbtbl_sett_list);
    fclose(fp);
    
    memcpy(dbtbl_file_info.status, "0", 1);        
    nReturnCode = iInsertTblFileInfo(dbtbl_file_info);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "iInsertTblFileInfo error [%d]", nReturnCode);
        return -1;
    }
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nTotalCount:[%d]", nTotalCount);
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_7100 Succ .");

    return 0;
}

int VarRecdToPlat(char* strBuf, char cTab, tbl_sett_list_def* vtdbl_sett_list)
{
    int     nReturnCode;
    char    sRcdBuf[RECORD_LEN_MAX] = {0};
    char    tmpBuf[50] = {0};
    char    sDelim[1];
    char    sDateTime[14+1];
    
    memset(sDelim, 0, sizeof(sDelim));
    sDelim[0]=cTab;
    
    RightTrim(vtdbl_sett_list->seq_no);
    strncat(sRcdBuf, vtdbl_sett_list->seq_no, strlen(vtdbl_sett_list->seq_no));                         /*结算流水号*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    strncat(sRcdBuf, "TOPCPG", 6);
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    RightTrim(vtdbl_sett_list->chl_id);
    strncat(sRcdBuf, vtdbl_sett_list->chl_id, strlen(vtdbl_sett_list->chl_id));                         /* 渠道 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    getSysTime(sDateTime);
    strncat(sRcdBuf, sDateTime, 8);                                                                     /*交易日期*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    strncat(sRcdBuf, sDateTime+8, 6);                                                                   /*交易时间*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));

    RightTrim(vtdbl_sett_list->plat_sett_bank);
    strncat(sRcdBuf, vtdbl_sett_list->plat_sett_bank, strlen(vtdbl_sett_list->plat_sett_bank));                   /* 转出交易行号 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_sett_list->plat_sett_acct_type);
    strncat(sRcdBuf, "0022", 4);                   /* 转出账号类型 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_sett_list->plat_sett_acct);
    strncat(sRcdBuf, vtdbl_sett_list->plat_sett_acct, strlen(vtdbl_sett_list->plat_sett_acct));                       /* 转出账号 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_sett_list->plat_sett_acct_nm);
    strncat(sRcdBuf, vtdbl_sett_list->plat_sett_acct_nm, strlen(vtdbl_sett_list->plat_sett_acct_nm));                   /* 转出方户名 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    strncat(sRcdBuf, "156", 3);                                                                         /* 币种 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    sprintf(sRcdBuf+strlen(sRcdBuf), "%.2f", vtdbl_sett_list->amt);										/*交易金额*/
    strncat(sRcdBuf, sDelim, strlen(sDelim)); 

    RightTrim(vtdbl_sett_list->mcht_sett_bank);
    strncat(sRcdBuf, vtdbl_sett_list->mcht_sett_bank, strlen(vtdbl_sett_list->mcht_sett_bank));                 /* 转入交易行号 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_sett_list->mcht_sett_acct_type);
    strncat(sRcdBuf, "0031", 4);                 /* 转入账号类型 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_sett_list->mcht_sett_xingmin_acct);
    strncat(sRcdBuf, vtdbl_sett_list->mcht_sett_xingmin_acct, strlen(vtdbl_sett_list->mcht_sett_acct));                     /* 转入账号 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_sett_list->mcht_sett_acct_nm);
    strncat(sRcdBuf, vtdbl_sett_list->mcht_sett_acct_nm, strlen(vtdbl_sett_list->mcht_sett_acct_nm));                 /* 转入方户名 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    strncat(sRcdBuf, "02", 2);                                                                          /* 交易类型 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    strcat(sRcdBuf, "\n");
    memcpy(strBuf, sRcdBuf, strlen(sRcdBuf));
    
    return 0;
}


/****************************** end of Task_7100.c ******************************/


