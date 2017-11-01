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
 *  文 件 名: Task_0402.c
 *  功    能: 导入核心流水文件
 *  编程人员: HELU
 *  开发时间: 2016-12-15
 *  备    注:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include "batch.h"
#include "Common.h"

extern char	gLogFile[LOG_NAME_LEN_MAX];
extern  tbl_date_inf_def dbtbl_date_inf;
extern	char ext_inter_brh_code[10 + 1];

/*************************************************
*函数名称∶ Total_0402
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_0402()
{
    int		nTotalNum = 0;
    char      sHostTxnNum[2 + 1];
    char	sFileRecord[RECORD_LEN_MAX];
    char    *pstr;
    char	host_file_name[200] = {0};
    FILE *fp;
    stExtnFileRegDef StExtnFileReg;

     /* 初始文件信息 */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    memcpy(StExtnFileReg.host_name, "HOST", 9);
    memcpy(StExtnFileReg.src_file_name, "HOST_YYYYMMDD_XX_TXN.txt", 24);

    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);
    pstr = strstr (StExtnFileReg.src_file_name, "XX");
    if (pstr)
        memcpy (pstr, "01", 2);

    getExtnFileFullName(&StExtnFileReg, host_file_name);

    /* 取文件行数 */
    fp = fopen (host_file_name, "r");
    if (!fp)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen %s error.", host_file_name);
        return -1;
    }

    while(fgets( sFileRecord, RECORD_LEN_MAX, fp) != NULL)
    {
        if( strlen( sFileRecord) == 0)
            continue;
        nTotalNum++;
    }

    fclose (fp);

    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "nTotalNum =[%d].", nTotalNum);

    return nTotalNum;
}

/*************************************************
*函数名称∶ Task_0402
*函数功能∶
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_0402 ( int nBeginOffset, int nEndOffset )
{
    int		iRet = 0;
    int	    i  = 1, nTotCount=0;
    char    *pstr;
    char    sCupBrhId[8 + 1] = {0};
    char	host_file_name[200] = {0};
    char	sFileRecord[RECORD_LEN_MAX];
    char    sTmp[512] = {0};
    char    sDelim[] = "|";
    FILE   *fp;

    stExtnFileRegDef StExtnFileReg;
    ht_tbl_host_txn_def ht_tbl_host_txn;
    tbl_brh_map_def tbl_brh_map_inf;

    /* 初始文件信息 */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    memcpy(StExtnFileReg.host_name, "HOST", 4);
    memcpy(StExtnFileReg.src_file_name, "HOST_YYYYMMDD_XX_TXN.txt", 24);

    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);
    pstr = strstr (StExtnFileReg.src_file_name, "XX");
    if (pstr)
        memcpy (pstr, "01", 2);

    getExtnFileFullName(&StExtnFileReg, host_file_name);
    
    fp = fopen (host_file_name, "r");
    if (!fp)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen %s error", host_file_name);
        return -1;
    }

    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "nBeginOffset=[%d] nEndOffset=[%d].", nBeginOffset, nEndOffset);

    /* 置文件起始点 */
    while (i < nBeginOffset && fgets( sFileRecord, RECORD_LEN_MAX, fp) != NULL)
    {
        if (strlen(sFileRecord) == 0)
        {
            continue;
        }

        i++;
    }

    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "start point = [%d].", i);

    while (i < nEndOffset + 1)
    {
        memset(sFileRecord, 0x00, sizeof(sFileRecord));
        memset(&ht_tbl_host_txn, 0x00, sizeof(ht_tbl_host_txn));
        if (fgets(sFileRecord, RECORD_LEN_MAX, fp) == NULL)
        {
            break;
        }
        if(strlen(sFileRecord) < BT_MINLINE_LEN)
        {
            HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sFileRecord [%s].", sFileRecord);
            continue;    
        }
        i++;
        nTotCount++;
	
	    memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, ht_tbl_host_txn.trans_date, sDelim);
        getValueByDelim(sFileRecord, ht_tbl_host_txn.trans_time, sDelim);
        getValueByDelim(sFileRecord, sTmp, sDelim);
        getValueByDelim(sFileRecord, ht_tbl_host_txn.plat_key, sDelim);
        CommonRTrim(ht_tbl_host_txn.plat_key);
        getValueByDelim(sFileRecord, ht_tbl_host_txn.batch_no, sDelim);
        getValueByDelim(sFileRecord, ht_tbl_host_txn.order_no, sDelim);
        getValueByDelim(sFileRecord, ht_tbl_host_txn.ext_mcht_no, sDelim);

        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);
        CommonRTrim(sTmp);        
        strcpy(ht_tbl_host_txn.bus_tp, sTmp);
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);
        CommonRTrim(sTmp);        
        strcat(ht_tbl_host_txn.bus_tp, sTmp);   
        
        getValueByDelim(sFileRecord, ht_tbl_host_txn.trans_tp, sDelim);
        getValueByDelim(sFileRecord, ht_tbl_host_txn.trans_state, sDelim);
        
        getValueByDelim(sFileRecord, ht_tbl_host_txn.revsal_flag, sDelim);
        getValueByDelim(sFileRecord, ht_tbl_host_txn.key_revsal, sDelim);
        getValueByDelim(sFileRecord, ht_tbl_host_txn.cancel_flag, sDelim);
        getValueByDelim(sFileRecord, ht_tbl_host_txn.key_cancel, sDelim);
        getValueByDelim(sFileRecord, ht_tbl_host_txn.acct_type, sDelim);
        getValueByDelim(sFileRecord, ht_tbl_host_txn.pan, sDelim);
        CommonRTrim(ht_tbl_host_txn.pan);
        getValueByDelim(sFileRecord, ht_tbl_host_txn.ccy, sDelim);
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);
        CommonRTrim(sTmp);
        sprintf(ht_tbl_host_txn.trans_at, "%012.0lf", floor(atof(sTmp)*100+0.50001));
        
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);
        CommonRTrim(sTmp);
        sprintf(ht_tbl_host_txn.trans_at2, "%012.0lf", floor(atof(sTmp)*100+0.50001));
        
        memcpy(ht_tbl_host_txn.inter_brh_code, ext_inter_brh_code, sizeof(ht_tbl_host_txn.inter_brh_code)-1);
        
        memcpy(ht_tbl_host_txn.sett_date, dbtbl_date_inf.stoday, sizeof(ht_tbl_host_txn.sett_date)-1);
        memcpy(ht_tbl_host_txn.flag_result, BT_FLAG_RESULT_NULL, BT_FLAG_RESULT_LEN);
        
        iRet = DbsTblHostTxn(DBS_INSERT, &ht_tbl_host_txn, 0, 0);
        if (iRet != 0 && iRet != DBS_KEYDUPLICATE)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblHostTxn DBS_INSERT error, %d.", iRet);
            fclose (fp);
            return -1;
        }
        else if(iRet == DBS_KEYDUPLICATE)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblHostTxn DBS_KEYDUPLICATE");
        }
    }
    
    fclose (fp);
    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "total num [%d].", nTotCount);
    HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_0402 Succ .");
    return 0;
}
/****************************** end of Task_0402.c ******************************/
