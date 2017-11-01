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
 *  文 件 名: Task_7402.c
 *  功    能: 处理银行归集响应文件
 *  编程人员: bcw
 *  开发时间: 2017-03-09
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

#define BANK_STEP_NO                                     "08"

int dealCibMer(char* file_name, cib_rsp_info_def* vtrsp_cib_info);

/*************************************************
*函数名称∶ Total_7402
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_7402()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_7402
*函数功能∶ 处理银行归集响应文件
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_7402 ( int nBeginOffset, int nEndOffset )
{
    int		nReturnCode;
    int	    nTotCount=0;
    int     nFileCount =0;
    char    *pstr;
    char	sFileRecord[RECORD_LEN_MAX];
    FILE   *fp;
    char    sDateTime[14+1];
    char    stlm_dt[8+1]; 
    char    sRcvFileName[200] = {0};
    tbl_file_info_def dbtbl_file_info;
    cib_rsp_info_def rsp_cib_info;
    tbl_mission_info_def dbtbl_miss_info;

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
    
    memset(sDateTime, 0, sizeof(sDateTime)-1);
	getSysTime(sDateTime);
	memset(&dbtbl_file_info, 0, sizeof(dbtbl_file_info)-1);
	memcpy(dbtbl_file_info.sett_date, stlm_dt, 8);
    memcpy(dbtbl_file_info.step_no, BANK_STEP_NO, 2);
    memcpy(dbtbl_file_info.batch_no, "02", 2);
	memcpy(dbtbl_file_info.file_type, "M", 1);
	memcpy(dbtbl_file_info.file_flag, "R", 1);
	memcpy(dbtbl_file_info.status, "4", 1);
	HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date:[%s], file_type:[%s], file_flag:[%s], status:[%s]", 
	                        dbtbl_file_info.sett_date, dbtbl_file_info.file_type, dbtbl_file_info.file_flag, dbtbl_file_info.status);
	
	/* 游标 文件信息表获取文件名和银行编号 */
	nReturnCode = DbsTblFileInfo(DBS_CURSOR, &dbtbl_file_info);
	
	nReturnCode = DbsTblFileInfo(DBS_OPEN, &dbtbl_file_info);
	if (nReturnCode)
	{
	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN tbl_file_info_cur error [%d]", nReturnCode);
	    return -1;
	}
	
	while(1)
	{
	    /* FETCH数据，获取文件名 */
	    memset(&dbtbl_file_info, 0, sizeof(dbtbl_file_info)-1);
	    nReturnCode = DbsTblFileInfo(DBS_FETCH, &dbtbl_file_info);
	    if (nReturnCode == DBS_NOTFOUND)
	    {
	        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_FETCH tbl_file_info_cur end.");
	        break;
	    }
	    else if (nReturnCode)
	    {
	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_FETCH tbl_file_info_cur error [%d].", nReturnCode);
	        DbsTblFileInfo(DBS_CLOSE, &dbtbl_file_info);
	        return -1;
	    }
	    nFileCount++;
	    RightTrim(dbtbl_file_info.file_name);
	    memset(sRcvFileName, 0, sizeof(sRcvFileName)-1);
	    memcpy(sRcvFileName, dbtbl_file_info.file_name, strlen(dbtbl_file_info.file_name));
	    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "RECEEVE FILE :[%s]\n", sRcvFileName);
	    
	    /* 打开文件 */
	    fp = fopen (sRcvFileName, "r");
	    if (!fp)
	    {
	        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen [%s] error.", sRcvFileName);
	        continue;
	    }
	    
	    nTotCount=0;
	    while(fgets(sFileRecord, RECORD_LEN_MAX, fp) != NULL)
	    {
	        /* 解析文件 */
	        memset(&rsp_cib_info, 0, sizeof(rsp_cib_info)-1);
	        GetInfoToBuf(sFileRecord, strlen(sFileRecord), '|', 1, rsp_cib_info.seq_no);
	        GetInfoToBuf(sFileRecord, strlen(sFileRecord), '|', 2, rsp_cib_info.mcht_no);
	        GetInfoToBuf(sFileRecord, strlen(sFileRecord), '|', 3, rsp_cib_info.tran_date);
	        GetInfoToBuf(sFileRecord, strlen(sFileRecord), '|', 4, rsp_cib_info.tran_time);
	        GetInfoToBuf(sFileRecord, strlen(sFileRecord), '|', 5, rsp_cib_info.pay_acct_no);
	        GetInfoToBuf(sFileRecord, strlen(sFileRecord), '|', 6, rsp_cib_info.pay_acct_name);
	        GetInfoToBuf(sFileRecord, strlen(sFileRecord), '|', 7, rsp_cib_info.acct_no);
	        GetInfoToBuf(sFileRecord, strlen(sFileRecord), '|', 8, rsp_cib_info.acct_name);
	        GetInfoToBuf(sFileRecord, strlen(sFileRecord), '|', 9, rsp_cib_info.acct_type);
	        GetInfoToBuf(sFileRecord, strlen(sFileRecord), '|', 10, rsp_cib_info.tran_amt);
	        GetInfoToBuf(sFileRecord, strlen(sFileRecord), '|', 11, rsp_cib_info.function);
	        GetInfoToBuf(sFileRecord, strlen(sFileRecord), '|', 12, rsp_cib_info.tran_fee);
	        GetInfoToBuf(sFileRecord, strlen(sFileRecord), '|', 13, rsp_cib_info.tran_status);
	        GetInfoToBuf(sFileRecord, strlen(sFileRecord), '|', 14, rsp_cib_info.reserve);
	        nTotCount++;
	        nReturnCode=dealCibMer(sRcvFileName, &rsp_cib_info);
	        if(nReturnCode)
	        {
	            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "dealCibRcv error.");
	            DbsTblFileInfo(DBS_CLOSE, &dbtbl_file_info);
	            fclose (fp);
            	return -1;
	        }
	    }
	    fclose (fp);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nTotCount:[%d]", nTotCount);

	    /* 文件状态更新 */
	    RightTrim(dbtbl_file_info.file_name);
	    RightTrim(dbtbl_file_info.bank_code);
	    RightTrim(dbtbl_file_info.file_flag);
	    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name:[%s],bank_cod:[%s],file_flag:[%s]\n",
	                                                            dbtbl_file_info.file_name,dbtbl_file_info.bank_code,dbtbl_file_info.file_flag);
	    memcpy(dbtbl_file_info.status, "5", 1);
	    nReturnCode = DbsTblFileInfo(DBS_UPDATE, &dbtbl_file_info);
	    if (nReturnCode)
	    {
	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPDATE ht_tbl_file_info error [%d].", nReturnCode);
	        DbsTblFileInfo(DBS_CLOSE, &dbtbl_file_info);
	        return -1;
	    }
	}
	DbsTblFileInfo(DBS_CLOSE, &dbtbl_file_info);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nFileCount:[%d]", nFileCount);
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_7402 Succ .");
	
	return 0;
}


/* 归集响应处理 */
int dealCibMer(char* file_name, cib_rsp_info_def* vtrsp_cib_info)
{
	int nReturnCode;
	double miss_amt;
	double miss_fee;
	char    sDateTime[14+1];

	tbl_mission_info_def dbtbl_miss_info;
	tbl_postion_info_def dbtbl_post_info;
	tbl_sett_list_def   dbtbl_sett_list;
	
	memset(&dbtbl_miss_info, 0, sizeof(dbtbl_miss_info));
	memcpy(dbtbl_miss_info.seq_no, vtrsp_cib_info->seq_no, sizeof(dbtbl_miss_info.seq_no)-1);
	RightTrim(dbtbl_miss_info.seq_no);
	HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "seq_no:[%s]", dbtbl_miss_info.seq_no);
	HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "tran_status:[%s]", vtrsp_cib_info->tran_status);
	
	if(strncmp(vtrsp_cib_info->tran_status, "3", 1) == 0) /* 银行响应失败 */
	{
		/* 更新归集明细表该笔记录的归集状态为-归集失败 */
		memcpy(dbtbl_miss_info.status, "3", 1);
		nReturnCode = DbsTblMissInfo(DBS_UPDATE, &dbtbl_miss_info);
		if (nReturnCode)
		{
		    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPDATE ht_tbl_mission_info error [%d].", nReturnCode);
		    return -1;
		}
	}
	else if(strncmp(vtrsp_cib_info->tran_status, "2", 1) == 0)/* 银行响应成功 */
	{
		/* 更新归集明细表该笔记录的归集状态为-归集完成 */
		memcpy(dbtbl_miss_info.status, "2", 1);
		nReturnCode = DbsTblMissInfo(DBS_UPDATE, &dbtbl_miss_info);
		if (nReturnCode)
		{
		    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPDATE ht_tbl_mission_info error [%d].", nReturnCode);
		    return -1;
		}
		
		HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "tran_amt:[%s]", vtrsp_cib_info->tran_amt);
		HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "tran_fee:[%s]", vtrsp_cib_info->tran_fee);
		
		miss_amt = atof(vtrsp_cib_info->tran_amt);
		miss_fee = atof(vtrsp_cib_info->tran_fee);
		
		/* 更新转出结算行的头寸信息 */
		memset(&dbtbl_post_info, 0, sizeof(dbtbl_post_info)-1);
		RightTrim(vtrsp_cib_info->pay_acct_no);
		memcpy(dbtbl_post_info.acct_no, vtrsp_cib_info->pay_acct_no, strlen(vtrsp_cib_info->pay_acct_no));
		nReturnCode = DbsTblPostInfo(DBS_SEL_BNK, &dbtbl_post_info);
		if (nReturnCode)
		{
		    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_SELECT ht_tbl_postion_info error [%d].", nReturnCode);
		    return -1;
		}
		HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "pay bank_code:[%s]", dbtbl_post_info.bank_code);
		HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "pay miss_amt:[%.2lf], miss_fee:[%.2lf]", miss_amt, miss_fee);
		dbtbl_post_info.acct_bal -= miss_amt;
		RightTrim(dbtbl_post_info.bank_code);
		nReturnCode = DbsTblPostInfo(DBS_UPD_BAL, &dbtbl_post_info);
		if (nReturnCode)
		{
		    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPD_BAL ht_tbl_postion_info error [%d].", nReturnCode);
		    return -1;
		}
		
		/* 更新转入结算行的头寸信息 */
		memset(&dbtbl_post_info, 0, sizeof(dbtbl_post_info)-1);
		RightTrim(vtrsp_cib_info->acct_no);
		memcpy(dbtbl_post_info.acct_no, vtrsp_cib_info->acct_no, strlen(vtrsp_cib_info->acct_no));
		nReturnCode = DbsTblPostInfo(DBS_SEL_BNK, &dbtbl_post_info);
		if (nReturnCode)
		{
		    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_SELECT ht_tbl_postion_info error [%d].", nReturnCode);
		    return -1;
		}
		HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "miss bank_code:[%s]", dbtbl_post_info.bank_code);
		HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "miss amt:[%.2lf]", miss_amt);
		dbtbl_post_info.acct_bal += miss_amt;
		RightTrim(dbtbl_post_info.bank_code);
		nReturnCode = DbsTblPostInfo(DBS_UPD_BAL, &dbtbl_post_info);
		if (nReturnCode)
		{
		    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPD_BAL ht_tbl_postion_info error [%d].", nReturnCode);
		    return -1;
		}
		
	}
	
	return 0;
}

/****************************** end of Task_7402.c ******************************/
