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
 *  文 件 名: Task_6102.c
 *  功    能: 生成对账不平流水上送核心
 *  编程人员: HELU
 *  开发时间: 2016-12-27
 *  备    注:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include "batch.h"

extern char	gLogFile[LOG_NAME_LEN_MAX];
extern  tbl_date_inf_def dbtbl_date_inf;
extern	char ext_inter_brh_code[10 + 1];


typedef struct
{
	char trans_date	    [8+ 1];
	char trans_time	    [6+ 1];
	char key_ssh        [20+1];
	char plat_key	     [48+ 1];
	char batch_no	     [2+ 1];
	char order_no	     [32+ 1];
	char ext_mcht_no	 [15+ 1];
	char bus_tp	       [8+ 1];
	char trans_tp	     [4+ 1];
	char trans_state	 [1+ 1];
	char revsal_flag	 [1+ 1];
	char key_revsal	   [32+ 1];
	char cancel_flag	 [1+ 1];
	char key_cancel	   [32+ 1];
	char acct_type	   [2+ 1];
	char pan	         [20+ 1];
	char ccy	         [3+ 1];
	char trans_at	   [12+ 1];
	char trans_at1	   [12+ 1];
	char trans_at2	 [20+ 1];
	char trans_at3	 [20+ 1];
	char trans_at4	 [20+ 1];
	char trans_at5	 [20+ 1];
	char flag_result	 [1+ 1];
}ht_tbl_err_txn_def;

/*************************************************
*函数名称∶ WriteFile
*函数功能∶
*输入参数∶ 生成流水文件
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
void WriteErrFile(ht_tbl_err_txn_def *pstbl_err_txn, FILE *fp)
{
    fprintf( fp, "%s%s", pstbl_err_txn->trans_date, "|");
    fprintf( fp, "%s%s", pstbl_err_txn->trans_time, "|");
    fprintf( fp, "%s%s", pstbl_err_txn->key_ssh, "|");
    fprintf( fp, "%s%s", pstbl_err_txn->plat_key, "|");
    fprintf( fp, "%s%s", pstbl_err_txn->batch_no, "|");
    fprintf( fp, "%s%s", pstbl_err_txn->order_no, "|");
    fprintf( fp, "%s%s", pstbl_err_txn->ext_mcht_no, "|");
    fprintf( fp, "%s%s", pstbl_err_txn->bus_tp, "|");
    fprintf( fp, "%s%s", pstbl_err_txn->trans_tp, "|");
    fprintf( fp, "%s%s", pstbl_err_txn->trans_state, "|");
    fprintf( fp, "%s%s", pstbl_err_txn->revsal_flag, "|");
    fprintf( fp, "%s%s", pstbl_err_txn->key_revsal, "|");
    fprintf( fp, "%s%s", pstbl_err_txn->cancel_flag, "|");
    fprintf( fp, "%s%s", pstbl_err_txn->key_cancel, "|");
    fprintf( fp, "%s%s", pstbl_err_txn->acct_type, "|");
    fprintf( fp, "%s%s", pstbl_err_txn->pan, "|");
    fprintf( fp, "%s%s", pstbl_err_txn->ccy, "|");
    fprintf( fp, "%s%s", pstbl_err_txn->trans_at, "|");
    fprintf( fp, "%s%s", pstbl_err_txn->trans_at1, "|");
    fprintf( fp, "%s%s", pstbl_err_txn->trans_at2, "|");
    fprintf( fp, "%s%s", pstbl_err_txn->trans_at3, "|");
    fprintf( fp, "%s%s", pstbl_err_txn->trans_at4, "|");
    fprintf( fp, "%s%s", pstbl_err_txn->trans_at5, "|");
    fprintf( fp, "%s%s", pstbl_err_txn->flag_result, "|");
    fprintf( fp, "\n");
    fflush(fp);

    return;
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

/*************************************************
*函数名称∶ Total_6102
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_6102()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_6102
*函数功能∶
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_6102 ( int nBeginOffset, int nEndOffset )
{
    int	    nReturnCode = 0;
    int     nTotCount=0;
    char    sDateTime[14+1];
    char    stlm_dt[8+1];
    char    *pstr;
    char    sFileRecord[RECORD_LEN_MAX];
    FILE    *fp;
    char    sFileFullName[100];
    char    sTmpFileName[30];
    
    tbl_file_info_def   dbtbl_file_info;
    ht_tbl_err_flow_def tbl_err_flow;
    ht_tbl_err_txn_def tbl_err_txn;

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
    
    /* 初始文件信息 */
    getSysTime(sDateTime);
    memset(&dbtbl_file_info, 0x00, sizeof(dbtbl_file_info));
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
    memcpy(dbtbl_file_info.step_no, "01", 2);
    memcpy(dbtbl_file_info.batch_no, "01", 2);
    memcpy(dbtbl_file_info.file_type, "H", 1);
    memcpy(dbtbl_file_info.file_flag, "S", 1);
    memcpy(dbtbl_file_info.end_flag, "Y", 1);
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
    

    memset(&tbl_err_flow, 0x00, sizeof(tbl_err_flow));
    memcpy(tbl_err_flow.sett_date, stlm_dt, sizeof(tbl_err_flow.sett_date)-1);
    DbsTblErrFlow(DBS_CURSOR, &tbl_err_flow, nBeginOffset, nEndOffset);
    nReturnCode = DbsTblErrFlow(DBS_OPEN, &tbl_err_flow, nBeginOffset, nEndOffset);
    if (nReturnCode != 0)
    {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblErrFlow DBS_OPEN error, %d.", nReturnCode);
            fclose (fp);
            return -1;
    }

    while (1)
    {
        memset(&tbl_err_flow, 0, sizeof(tbl_err_flow));
        nReturnCode = DbsTblErrFlow(DBS_FETCH, &tbl_err_flow, nBeginOffset, nEndOffset);
        if (nReturnCode && nReturnCode != DBS_FETCHNULL)
        {
            if (nReturnCode == DBS_NOTFOUND)
            {
                break;
            }

            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblDatFlow fetch sqlcode[%d]", nReturnCode);
            DbsTblErrFlow(DBS_CLOSE, &tbl_err_flow, nBeginOffset, nEndOffset);
            fclose (fp);
            return -1;
        }
        nTotCount++;
        memset(&tbl_err_txn,0x00,sizeof(tbl_err_txn));
        
     	memcpy(tbl_err_txn.trans_date,tbl_err_flow.trans_date,sizeof(tbl_err_txn.trans_date)-1);
     	memcpy(tbl_err_txn.trans_time,tbl_err_flow.trans_time,sizeof(tbl_err_txn.trans_time)-1);
     	//memcpy(tbl_err_txn.key_ssh,tbl_err_flow.key_ssh,sizeof(tbl_err_txn.key_ssh)-1);
     	memcpy(tbl_err_txn.plat_key,tbl_err_flow.plat_key,sizeof(tbl_err_txn.plat_key)-1);
     	memcpy(tbl_err_txn.batch_no,tbl_err_flow.batch_no,sizeof(tbl_err_txn.batch_no)-1);
     	memcpy(tbl_err_txn.order_no,tbl_err_flow.order_no,sizeof(tbl_err_txn.order_no)-1);
     	memcpy(tbl_err_txn.bus_tp,tbl_err_flow.bus_tp,sizeof(tbl_err_txn.bus_tp)-1);
     	memcpy(tbl_err_txn.trans_tp,tbl_err_flow.trans_tp,sizeof(tbl_err_txn.trans_tp)-1);
     	memcpy(tbl_err_txn.trans_state,tbl_err_flow.trans_state,sizeof(tbl_err_txn.trans_state)-1);
     	//memcpy(tbl_err_txn.revsal_flag,tbl_err_flow.revsal_flag,sizeof(tbl_err_txn.revsal_flag)-1);
     	//memcpy(tbl_err_txn.key_revsal,tbl_err_flow.key_revsal,sizeof(tbl_err_txn.key_revsal)-1);
     	//memcpy(tbl_err_txn.cancel_flag,tbl_err_flow.cancel_flag,sizeof(tbl_err_txn.cancel_flag)-1);
     	//memcpy(tbl_err_txn.key_cancel,tbl_err_flow.key_cancel,sizeof(tbl_err_txn.key_cancel)-1);
     	memcpy(tbl_err_txn.acct_type,tbl_err_flow.acct_type,sizeof(tbl_err_txn.acct_type)-1);
     	memcpy(tbl_err_txn.pan,tbl_err_flow.pan,sizeof(tbl_err_txn.pan)-1);
     	memcpy(tbl_err_txn.ccy,tbl_err_flow.ccy,sizeof(tbl_err_txn.ccy)-1);
     	sprintf(tbl_err_txn.trans_at,"%.2f",atof(tbl_err_flow.trans_at)/100);
     	//memcpy(tbl_err_txn.trans_at,tbl_err_flow.trans_at,sizeof(tbl_err_txn.trans_at)-1);
     	sprintf(tbl_err_txn.trans_at1,"%.2f",atof(tbl_err_flow.dest_trans_at)/100);
     	HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "trans_at:[%s][%d]\n", tbl_err_flow.trans_at,atoi(tbl_err_flow.trans_at));
     	//memcpy(tbl_err_txn.trans_at1,tbl_err_flow.dest_trans_at,sizeof(tbl_err_txn.trans_at1)-1);
     	//memcpy(tbl_err_txn.trans_at2,tbl_err_flow.trans_at2,sizeof(tbl_err_txn.trans_at2)-1);
     	//memcpy(tbl_err_txn.trans_at3,tbl_err_flow.trans_at3,sizeof(tbl_err_txn.trans_at3)-1);
     	//memcpy(tbl_err_txn.trans_at4,tbl_err_flow.trans_at4,sizeof(tbl_err_txn.trans_at4)-1);
     	//memcpy(tbl_err_txn.trans_at5,tbl_err_flow.trans_at5,sizeof(tbl_err_txn.trans_at5)-1);
     	memcpy(tbl_err_txn.flag_result,tbl_err_flow.flag_result,sizeof(tbl_err_txn.flag_result)-1);         
        CommonRTrim(tbl_err_txn.plat_key);
        CommonRTrim(tbl_err_txn.order_no);
        CommonRTrim(tbl_err_txn.pan);
        //CommonRTrim(tbl_err_txn.trans_at);
        WriteErrFile(&tbl_err_txn, fp);
        
    }
    DbsTblErrFlow(DBS_CLOSE, &tbl_err_flow, nBeginOffset, nEndOffset);
    fclose (fp);

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
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file [%s] Insert into ht_tbl_file_info success.", dbtbl_file_info.file_name);
    
    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "total num [%d].", nTotCount);
    HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_6102 Succ .");
    return 0;
}
/****************************** end of Task_6102.c ******************************/

