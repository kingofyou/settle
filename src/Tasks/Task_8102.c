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
 *  文 件 名: Task_8102.c
 *  功    能: 银行卡交易记录(归集成功后的记录)生成核心记账文件
 *  编程人员: bcw
 *  开发时间: 2017-03-09
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

int MerRecdToPlat(char* strBuf, char cTab, tbl_sett_list_def* vtdbl_sett_list);
int iInsertTblFileInfo(tbl_file_info_def   dbtbl_file_info);
/*************************************************
*函数名称∶ Total_8102
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_8102()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_8102
*函数功能∶ 银行卡交易记录(归集成功后的记录)生成核心记账文件
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_8102 ( int nBeginOffset, int nEndOffset )
{
    int     nReturnCode;
    char    sDateTime[14+1];
    char    stlm_dt[8+1];        
    char    *pstr;
    char    sFileRecord[RECORD_LEN_MAX];
    FILE    *fp;
    char    sFileFullName[200];
    char    sTmpFileName[100];
    
    tbl_sett_list_def   dbtbl_sett_list;
    tbl_file_info_def   dbtbl_file_info;

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);

    /* 初始文件信息 */
    getSysTime(sDateTime);
    memset(&dbtbl_file_info, 0x00, sizeof(dbtbl_file_info)-1);
    memcpy(dbtbl_file_info.sett_date, stlm_dt, 8);
    memcpy(dbtbl_file_info.snd_time, sDateTime, 14);
    strcpy(sTmpFileName, "HOST_YYYYMMDD_XX_ACCT.txt");
    pstr = strstr(sTmpFileName, "YYYYMMDD");
    if (pstr)
        memcpy(pstr, stlm_dt, 8);
    pstr = strstr(sTmpFileName, "XX");
    if (pstr)
        memcpy (pstr, "04", 2);
    memcpy(dbtbl_file_info.step_no, "04", 2);
    memcpy(dbtbl_file_info.batch_no, "04", 2);
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

    memset(&dbtbl_sett_list, 0x00, sizeof(dbtbl_sett_list)-1);
    memcpy(dbtbl_sett_list.sett_date, stlm_dt, 8);
    memcpy(dbtbl_sett_list.mcht_sett_acct_type, "1", 1);
    memcpy(dbtbl_sett_list.status, "4", 1);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date:[%s], acct_type1:[%s], status:[%s]\n", dbtbl_sett_list.sett_date, dbtbl_sett_list.mcht_sett_acct_type, dbtbl_sett_list.status);
    /* 游标，结算明细表 */
    nReturnCode = DbsTblSettList(DBS_CURSOR_VAR, &dbtbl_sett_list);
    
    nReturnCode = DbsTblSettList(DBS_OPEN_VAR, &dbtbl_sett_list);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN_VAR tbl_sett_var_cur error [%d]", nReturnCode);
        fclose(fp);
        return -1;
    }
    
    while(1)
    {
        /* fetch 每条未入账的记录 */
        memset(&dbtbl_sett_list, 0, sizeof(dbtbl_sett_list));
        nReturnCode = DbsTblSettList(DBS_FETCH_VAR, &dbtbl_sett_list);
        if (nReturnCode == DBS_NOTFOUND)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_FETCH_VAR tbl_sett_var_cur end.");
            DbsTblSettList(DBS_CLOSE_VAR, &dbtbl_sett_list);

            break;
        }
        else if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_FETCH_VAR tbl_sett_var_cur error [%d]", nReturnCode);
            DbsTblSettList(DBS_CLOSE_VAR, &dbtbl_sett_list);
            return -1;
        }
        
        /* mark 渠道选择 */
        
        /* 将记录信息写入文件中 */
        memset(sFileRecord, 0x00, sizeof(sFileRecord)-1);
        MerRecdToPlat(sFileRecord, '|', &dbtbl_sett_list);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "Record:[%s]", sFileRecord);
        nReturnCode = fputs(sFileRecord, fp);
        if(nReturnCode>=0 && nReturnCode != EOF)
        {
	        memcpy(&dbtbl_sett_list.status, "6", 1);
	        /* 更新结算明细表记录 */
	        nReturnCode = DbsTblSettList(DBS_UPD_SEQ, &dbtbl_sett_list);
	        if (nReturnCode)
	        {
	            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPD_SEQ ht_tbl_acct_rslt error [%d]", nReturnCode);
	            DbsTblSettList(DBS_CLOSE_VAR, &dbtbl_sett_list);
	            return -1;
	        }
	    }
    }
    DbsTblSettList(DBS_CLOSE_VAR, &dbtbl_sett_list);
    fclose(fp);
    
    memcpy(dbtbl_file_info.status, "0", 1);
    
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name:[%s]\n", dbtbl_file_info.file_name);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date:[%s]\n", dbtbl_file_info.sett_date);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "snd_time :[%s]\n", dbtbl_file_info.snd_time); 
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "rcv_time :[%s]\n", dbtbl_file_info.rcv_time); 
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "status   :[%s]\n", dbtbl_file_info.status);   
    
    /* 插入文件信息表 */
    nReturnCode = iInsertTblFileInfo(dbtbl_file_info);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "iInsertTblFileInfo error [%d]", nReturnCode);
        return -1;
    }
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_8102 Succ .");

    return 0;
}

int MerRecdToPlat(char* strBuf, char cTab, tbl_sett_list_def* vtdbl_sett_list)
{
    int     nReturnCode;
    char    sRcdBuf[RECORD_LEN_MAX] = {0};
    char    tmpBuf[50] = {0};
    char    sDelim[1+1];
    char    sDateTime[14+1];
    
    memset(sDelim, 0, sizeof(sDelim));
    sDelim[0]=cTab;
    
    strncat(sRcdBuf, vtdbl_sett_list->seq_no, strlen(vtdbl_sett_list->seq_no));                         /*平台流水号*/
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
#if 0    
    if(!strcmp(vtdbl_sett_list->dc_flag, "C")){/*转出方有核心默认*/
        strncat(sRcdBuf, sDelim, strlen(sDelim));
        strncat(sRcdBuf, sDelim, strlen(sDelim));
        strncat(sRcdBuf, sDelim, strlen(sDelim));
        strncat(sRcdBuf, sDelim, strlen(sDelim));
    }else if(!strcmp(vtdbl_sett_list->dc_flag, "D")){
        RightTrim(vtdbl_sett_list->plat_sett_bank);
        strncat(sRcdBuf, vtdbl_sett_list->plat_sett_bank, strlen(vtdbl_sett_list->plat_sett_bank));               /* 转出交易行号 */
        strncat(sRcdBuf, sDelim, strlen(sDelim));
        
        RightTrim(vtdbl_sett_list->plat_sett_acct_type);
        strncat(sRcdBuf, vtdbl_sett_list->plat_sett_acct_type, strlen(vtdbl_sett_list->plat_sett_acct_type));               /* 转出账号类型 */
        strncat(sRcdBuf, sDelim, strlen(sDelim));
        
        RightTrim(vtdbl_sett_list->plat_sett_acct);
        strncat(sRcdBuf, vtdbl_sett_list->plat_sett_acct, strlen(vtdbl_sett_list->plat_sett_acct));                   /* 转出账号 */
        strncat(sRcdBuf, sDelim, strlen(sDelim));
        
        RightTrim(vtdbl_sett_list->plat_sett_acct_nm);
        strncat(sRcdBuf, vtdbl_sett_list->plat_sett_acct_nm, strlen(vtdbl_sett_list->plat_sett_acct_nm));               /* 转出方户名 */
        strncat(sRcdBuf, sDelim, strlen(sDelim));
    }
#endif
    RightTrim(vtdbl_sett_list->plat_sett_bank);
    strncat(sRcdBuf, vtdbl_sett_list->plat_sett_bank, strlen(vtdbl_sett_list->plat_sett_bank));                   /* 转出交易行号 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_sett_list->plat_sett_acct_type);
    strncat(sRcdBuf, vtdbl_sett_list->plat_sett_acct_type, strlen(vtdbl_sett_list->plat_sett_acct_type));                   /* 转出账号类型 */
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
    strncat(sRcdBuf, vtdbl_sett_list->mcht_sett_acct_type, strlen(vtdbl_sett_list->mcht_sett_acct_type));                 /* 转入账号类型 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_sett_list->mcht_sett_acct);
    strncat(sRcdBuf, vtdbl_sett_list->mcht_sett_acct, strlen(vtdbl_sett_list->mcht_sett_acct));                     /* 转入账号 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_sett_list->mcht_sett_acct_nm);
    strncat(sRcdBuf, vtdbl_sett_list->mcht_sett_acct_nm, strlen(vtdbl_sett_list->mcht_sett_acct_nm));                 /* 转入方户名 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    if(!strncmp(vtdbl_sett_list->acct_statu, "2", 1)){
        strncat(sRcdBuf, "04", 2);
    }else{
        strncat(sRcdBuf, "02", 2);
    }
    
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    strcat(sRcdBuf, "\n");
    memcpy(strBuf, sRcdBuf, strlen(sRcdBuf));
    
    return 0;
}

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
/****************************** end of Task_8102.c ******************************/


