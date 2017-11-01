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
 *  文 件 名: Task_7802.c
 *  功    能: 银行卡(归集成功后)交易记录生成银行划账文件
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

#define BANK_STEP_NO                                     "09"

int MerRcdToCIB(char* strBuf, char cTab, tbl_sett_list_def* vtdbl_sett_list);
static int SetChan(double amt, tbl_sett_chan_def* vtdbl_sett_chan);
int iInsertTblFileInfo(tbl_file_info_def   dbtbl_file_info);

/*************************************************
*函数名称∶ Total_7802
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_7802()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_7802
*函数功能∶ 银行卡(归集成功后)交易记录生成银行划账文件
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_7802 ( int nBeginOffset, int nEndOffset )
{
    int     nReturnCode;
    char    sDateTime[14+1];
    char    stlm_dt[8+1];    
    char    *pstr;
    char    sFileRecord[RECORD_LEN_MAX];
    char    sDelim[] = "|";
    FILE    *fp;
    char    tmpFile[30+1];
    
    tbl_sett_list_def   dbtbl_sett_list, dbtbl_sett_list_dc;
    tbl_file_info_def   dbtbl_file_info;
    tbl_sett_chan_def   dbtbl_sett_chan;
    
    char    sFileFullName[100];
    char    sTmpFileName[30];
    
    memset(tmpFile, 0, sizeof(tmpFile)-1);

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
   
    /* 游标，所有需划账的行号 */
    getSysTime(sDateTime);
    memset(&dbtbl_sett_list, 0, sizeof(dbtbl_sett_list)-1);
    memcpy(dbtbl_sett_list.sett_date, stlm_dt, 8);
    memcpy(dbtbl_sett_list.status, "2", 1);
    memcpy(dbtbl_sett_list.dc_flag, "C", 1);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date:[%s], status:[%s], dc_flag:[%s]", dbtbl_sett_list.sett_date, dbtbl_sett_list.status, dbtbl_sett_list.dc_flag);
    nReturnCode = DbsTblSettList(DBS_CURSOR_BNK, &dbtbl_sett_list);
    
    nReturnCode = DbsTblSettList(DBS_OPEN_BNK, &dbtbl_sett_list);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN_BNK tbl_sett_bnk_cur error, %d.", nReturnCode);
        return -1;
    }
    
    while(1)
    {
        /* fetch */
        memset(&dbtbl_sett_list, 0x00, sizeof(dbtbl_sett_list)-1);
        nReturnCode = DbsTblSettList(DBS_FETCH_BNK, &dbtbl_sett_list);
        if (nReturnCode == DBS_NOTFOUND)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_FETCH_BNK tbl_sett_bnk_cur end.");
            break;
        }
        else if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_FETCH_BNK tbl_sett_bnk_cur error [%d].", nReturnCode);
            DbsTblSettList(DBS_CLOSE_BNK, &dbtbl_sett_list);
            return -1;
        }
        RightTrim(dbtbl_sett_list.plat_sett_bank);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "bank_code:[%s].", dbtbl_sett_list.plat_sett_bank);
       
        /* 初始文件信息 */
        getSysTime(sDateTime);
        memset(&dbtbl_file_info, 0x00, sizeof(dbtbl_file_info)-1);
        memcpy(dbtbl_file_info.sett_date, stlm_dt, 8);
        memcpy(dbtbl_file_info.snd_time, sDateTime, 14);
        memset(sTmpFileName, 0, sizeof(sTmpFileName));
        memcpy(dbtbl_file_info.step_no, BANK_STEP_NO, 2);
        memcpy(dbtbl_file_info.batch_no, "02", 2);
        memcpy(dbtbl_file_info.file_type, "B", 1);
        memcpy(dbtbl_file_info.file_flag, "S", 1);
        memcpy(dbtbl_file_info.end_flag, "N", 1);
        
        sprintf(sTmpFileName, "BATCH_PAY_YYYYMMDD_%s%s.txt",dbtbl_file_info.step_no,dbtbl_file_info.batch_no);
        pstr = strstr(sTmpFileName, "YYYYMMDD");
        if (pstr)
            memcpy(pstr, stlm_dt, 8);
        memcpy(dbtbl_file_info.bank_code, dbtbl_sett_list.plat_sett_bank, strlen(dbtbl_sett_list.plat_sett_bank));
        
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
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name:[%s].", dbtbl_file_info.file_name);
        
        fp = fopen(dbtbl_file_info.file_name,"w+");
        if (!fp)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen %s error.", dbtbl_file_info.file_name);
            DbsTblSettList(DBS_CLOSE_BNK, &dbtbl_sett_list);
            return -1;
        }
        
        /* 游标，根据待划账的行号查询结算明细表 */
        memset(&dbtbl_sett_list_dc, 0x00, sizeof(dbtbl_sett_list_dc)-1);
        memcpy(dbtbl_sett_list_dc.sett_date, stlm_dt, 8);
        memcpy(dbtbl_sett_list_dc.plat_sett_bank, dbtbl_sett_list.plat_sett_bank, sizeof(dbtbl_sett_list_dc.plat_sett_bank)-1);
        memcpy(dbtbl_sett_list_dc.status, "2", 1);
        memcpy(dbtbl_sett_list_dc.dc_flag, "C", 1);
        memcpy(dbtbl_sett_list_dc.plat_sett_acct_type, "1", 1);
        
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date:[%s]", dbtbl_sett_list_dc.sett_date);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "bank_code:[%s]", dbtbl_sett_list_dc.plat_sett_bank); 
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "acct_type:[%s]", dbtbl_sett_list_dc.plat_sett_acct_type); 
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "dc_flag:[%s]", dbtbl_sett_list_dc.dc_flag);  
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "status:[%s]", dbtbl_sett_list_dc.status);   
        
        nReturnCode = DbsTblSettList(DBS_CURSOR_C, &dbtbl_sett_list_dc);
        
        nReturnCode = DbsTblSettList(DBS_OPEN_C, &dbtbl_sett_list_dc);
        if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN_C error, %d.", nReturnCode);
            fclose(fp);
            continue;
        }
        
        while(1)
        {
            /* fetch 每条待划账的记录 */
            memset(&dbtbl_sett_list_dc, 0x00, sizeof(dbtbl_sett_list_dc)-1);
            nReturnCode = DbsTblSettList(DBS_FETCH_C, &dbtbl_sett_list_dc);
            if (nReturnCode == DBS_NOTFOUND)
            {
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_FETCH_C end [%d].", nReturnCode);
                break;
            }
            else if (nReturnCode)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_FETCH_C error [%d].", nReturnCode);
                break;
            }
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "amt:[%.2f].", dbtbl_sett_list_dc.amt);
            /* mark 渠道信息选择 */
            memset(&dbtbl_sett_chan, 0, sizeof(dbtbl_sett_chan));
            nReturnCode = SetChan(dbtbl_sett_list_dc.amt, &dbtbl_sett_chan);
            if(nReturnCode)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "SetChan error [%d].", nReturnCode);
	            break;
            }
            
            /* 将记录信息写入文件中 */
            memset(sFileRecord, 0x00, sizeof(sFileRecord)-1);
            MerRcdToCIB(sFileRecord, '|', &dbtbl_sett_list_dc);
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "Record:[%s].", sFileRecord);
            nReturnCode=fputs(sFileRecord, fp);
            if(nReturnCode > 0 &&  nReturnCode != EOF)
            {
            	memcpy(dbtbl_sett_list_dc.status,"3",1);
            	memset(dbtbl_sett_list_dc.file_name, 0, sizeof(dbtbl_sett_list_dc.file_name)-1);
            	sprintf(dbtbl_sett_list_dc.file_name, "%s", dbtbl_file_info.file_name);
            	RightTrim(dbtbl_sett_chan.chnl_id);
            	memset(dbtbl_sett_list_dc.chl_id, 0, sizeof(dbtbl_sett_list_dc.chl_id));
            	memcpy(dbtbl_sett_list_dc.chl_id, dbtbl_sett_chan.chnl_id, strlen(dbtbl_sett_chan.chnl_id));
            	RightTrim(dbtbl_sett_chan.chnl_name);
            	memset(dbtbl_sett_list_dc.chnl_name, 0, sizeof(dbtbl_sett_list_dc.chnl_name));
            	memcpy(dbtbl_sett_list_dc.chnl_name, dbtbl_sett_chan.chnl_name, strlen(dbtbl_sett_chan.chnl_name));
            	HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "status:[%s], file_name:[%s]", dbtbl_sett_list_dc.status, dbtbl_sett_list_dc.file_name);
	            /* 更新结算明细表记录 */
	            nReturnCode = DbsTblSettList(DBS_UPD_FL, &dbtbl_sett_list_dc);
	            if (nReturnCode)
	            {
	                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_UPD_FL ht_tbl_sett_list error [%d].", nReturnCode);
	                break;
	            }
	        }
        }
        fclose(fp);
        DbsTblSettList(DBS_CLOSE_C, &dbtbl_sett_list_dc);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file:[%s] succ.", dbtbl_file_info.file_name);
        
        strcpy(dbtbl_file_info.status, "0");
        /* 插入文件信息表 */
        nReturnCode = iInsertTblFileInfo(dbtbl_file_info);
        if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "iInsertTblFileInfo error [%d].", nReturnCode);
            break;
        }
    }
    DbsTblSettList(DBS_CLOSE_BNK, &dbtbl_sett_list);
    
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_7802 Succ .");

    return 0;
}

int MerRcdToCIB(char* strBuf, char cTab, tbl_sett_list_def* vtdbl_sett_list)
{
    int     nReturnCode;
    char    sRcdBuf[RECORD_LEN_MAX] = {0};
    char    tmpBuf[50] = {0};
    char    sDelim[1+1];
    char    sDateTime[14+1];
    
    memset(sDelim, 0, sizeof(sDelim));
    sDelim[0]=cTab;
    
    strncat(sRcdBuf, vtdbl_sett_list->seq_no, strlen(vtdbl_sett_list->seq_no));                         /*交易流水号*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    strncat(sRcdBuf, sDelim, strlen(sDelim));                                                           /*二级商户为空*/
    
    RightTrim(vtdbl_sett_list->mcht_sett_acct);
    strncat(sRcdBuf, vtdbl_sett_list->mcht_sett_acct, strlen(vtdbl_sett_list->mcht_sett_acct));                     /*商户银行卡号*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_sett_list->mcht_sett_acct_nm);
    strncat(sRcdBuf, vtdbl_sett_list->mcht_sett_acct_nm, strlen(vtdbl_sett_list->mcht_sett_acct_nm));                 /*商户账户名称*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));                                                           
    
    RightTrim(vtdbl_sett_list->mcht_sett_bank);
    strncat(sRcdBuf, vtdbl_sett_list->mcht_sett_bank, strlen(vtdbl_sett_list->mcht_sett_bank));                 /*银行代码*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_sett_list->mcht_sett_acct_type);
    strncat(sRcdBuf, vtdbl_sett_list->mcht_sett_acct_type, strlen(vtdbl_sett_list->mcht_sett_acct_type));                 /*商户账户类型*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));                                                           
    
    sprintf(sRcdBuf+strlen(sRcdBuf), "%.2lf", vtdbl_sett_list->amt);                                    /*金额*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    strncat(sRcdBuf, sDelim, strlen(sDelim));                                                           /*用途*/
    
    strcat(sRcdBuf, "\n");
    memcpy(strBuf, sRcdBuf, strlen(sRcdBuf));
    
    return 0;
}

int SetChan(double amt, tbl_sett_chan_def* vtdbl_sett_chan)
{
    int     nReturnCode;
    tbl_sett_chan_def   dbtbl_sett_chan;
    
    memset(&dbtbl_sett_chan, 0, sizeof(dbtbl_sett_chan));
    RightTrim(vtdbl_sett_chan->status);
    memcpy(dbtbl_sett_chan.status, "Y", 1);
    
    nReturnCode = DbsTblSettChan(DBS_SELECT, amt, &dbtbl_sett_chan);
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_SELECT ht_tbl_sett_chan error, %d.", nReturnCode);
        return -1;
    }
      
    RightTrim(dbtbl_sett_chan.chnl_id);
    RightTrim(dbtbl_sett_chan.chnl_name);
    RightTrim(dbtbl_sett_chan.status);
    RightTrim(dbtbl_sett_chan.index_day);
    RightTrim(dbtbl_sett_chan.index_time);
    RightTrim(dbtbl_sett_chan.index_fee);
    RightTrim(dbtbl_sett_chan.period);
    RightTrim(dbtbl_sett_chan.shadom);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "chnl_id:[%s].", dbtbl_sett_chan.chnl_id);
    dbtbl_sett_chan.bal -= amt;
    nReturnCode = DbsTblSettChan(DBS_UPDATE, amt, &dbtbl_sett_chan);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_UPDATE ht_tbl_sett_chan error [%d].", nReturnCode);
        return -1;
    }
    memcpy(vtdbl_sett_chan, &dbtbl_sett_chan, sizeof(dbtbl_sett_chan)-1);
    
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
/****************************** end of Task_7802.c ******************************/


