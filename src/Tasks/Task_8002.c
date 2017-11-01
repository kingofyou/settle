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
 *  文 件 名: Task_8002.c
 *  功    能: 处理银行批量代付响应文件
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

static int updRsltByBnk(int tran_flag, tbl_sett_list_def* vtdbtbl_sett_list);
static int dealCibRsp(cib_rsp_info_def* vtrsp_cib_info);

#define BANK_STEP_NO                                     "09"

/*************************************************
*函数名称∶ Total_8002
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_8002()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_8002
*函数功能∶ 处理银行批量代付响应文件
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_8002 ( int nBeginOffset, int nEndOffset )
{
    int		nReturnCode;
    int	        i  = 1, nTotCount=0;
    char    *pstr;
    char	sFileRecord[RECORD_LEN_MAX];
    FILE   *fp;
    char    sDateTime[14+1];
    char    stlm_dt[8+1];        
    char    sRcvFileName[100] = {0};
    tbl_file_info_def dbtbl_file_info;
    cib_rsp_info_def rsp_cib_info;
    tbl_mission_info_def dbtbl_miss_info;

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
    
    memset(sDateTime, 0, sizeof(sDateTime)-1);
	getSysTime(sDateTime);
	memset(&dbtbl_file_info, 0, sizeof(dbtbl_file_info));
	memcpy(dbtbl_file_info.sett_date, stlm_dt, 8);
    memcpy(dbtbl_file_info.step_no, BANK_STEP_NO, 2);
    memcpy(dbtbl_file_info.batch_no, "02", 2);
	memcpy(dbtbl_file_info.file_type, "B", 1);
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
	        
	        nReturnCode=dealCibRsp(&rsp_cib_info);
	        if(nReturnCode)
	        {
	            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "dealCibRcv error.");
	            DbsTblFileInfo(DBS_CLOSE, &dbtbl_file_info);
	            fclose (fp);
            	return -1;
	        }
	    }
	    fclose (fp);
	    
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
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_8002 Succ .");
	
	return 0;
}

/* 银行响应处理 */
int dealCibRsp(cib_rsp_info_def* vtrsp_cib_info)
{
	int nReturnCode;
	double tran_amt;
	double tran_fee;
	char    sDateTime[14+1];
	int succ_flag;

	tbl_postion_info_def dbtbl_post_info;
	tbl_sett_list_def   dbtbl_sett_list;
	
	memset(&dbtbl_sett_list, 0, sizeof(dbtbl_sett_list));
	memcpy(dbtbl_sett_list.seq_no, vtrsp_cib_info->seq_no, strlen(vtrsp_cib_info->seq_no));
	HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "seq_no:[%s]", dbtbl_sett_list.seq_no);
	HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "tran_status:[%s]", vtrsp_cib_info->tran_status);
	
	if(strncmp(vtrsp_cib_info->tran_status, "3", 1) == 0) /* 银行响应失败 */
	{
		/* 银行划付失败 */
		memcpy(dbtbl_sett_list.status, "5", 1);
		nReturnCode = DbsTblSettList(DBS_UPD_SEQ, &dbtbl_sett_list);
		if (nReturnCode)
		{
		    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPD_SEQ ht_tbl_sett_list error [%d].", nReturnCode);
		    return -1;
		}
		succ_flag = 0;
		
	    /* mark 更新入账结果表记录,挂账 */
	    nReturnCode = updRsltByBnk(succ_flag, &dbtbl_sett_list);
    	if (nReturnCode)
    	{
    	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "updRsltByKrnl error [%d].", nReturnCode);
    	    return -1;
    	}
	}
	else if(strncmp(vtrsp_cib_info->tran_status, "2", 1) == 0)/* 银行响应成功 */
	{
		tran_amt = atof(vtrsp_cib_info->tran_amt);
		tran_fee = atof(vtrsp_cib_info->tran_fee);
		HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "tran_amt:[%.lf]", tran_amt);
		HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "tran_fee:[%.lf]", tran_fee);
		
		/* 更新转出结算行的头寸信息 */
		memset(&dbtbl_post_info, 0, sizeof(dbtbl_post_info));
		RightTrim(vtrsp_cib_info->pay_acct_no);
		memcpy(dbtbl_post_info.acct_no, vtrsp_cib_info->pay_acct_no, strlen(vtrsp_cib_info->pay_acct_no));
		nReturnCode = DbsTblPostInfo(DBS_SEL_BNK, &dbtbl_post_info);
		if (nReturnCode)
		{
		    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_SELECT ht_tbl_postion_info error [%d].", nReturnCode);
		    return -1;
		}
		HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "pay bank_code:[%s]", dbtbl_post_info.bank_code);
		HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "pay tran_amt:[%.2lf], tran_fee:[%.2lf]", tran_amt, tran_fee);
		dbtbl_post_info.acct_bal -= tran_amt;
		RightTrim(dbtbl_post_info.bank_code);
		nReturnCode = DbsTblPostInfo(DBS_UPD_BAL, &dbtbl_post_info);
		if (nReturnCode)
		{
		    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPD_BAL ht_tbl_postion_info error [%d].", nReturnCode);
		    return -1;
		}
		
		/* 银行划付成功 */
		memcpy(dbtbl_sett_list.status, "4", 1);
		nReturnCode = DbsTblSettList(DBS_UPD_SEQ, &dbtbl_sett_list);
		if (nReturnCode)
		{
		    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPD_SEQ ht_tbl_sett_list error [%d].", nReturnCode);
		    return -1;
		}
	}
	
	
	return 0;
}

int updRsltByBnk(int tran_flag, tbl_sett_list_def* vtdbtbl_sett_list)
{
    int nReturnCode;
    tbl_sett_list_def dbtbl_sett_list;
    ht_tbl_acct_rslt_def   dbtbl_acct_rslt;
    int succ_flag = tran_flag;
    
    memset(&dbtbl_sett_list, 0, sizeof(dbtbl_sett_list));
    RightTrim(vtdbtbl_sett_list->seq_no);
    memcpy(dbtbl_sett_list.seq_no, vtdbtbl_sett_list->seq_no, sizeof(dbtbl_sett_list.seq_no)-1);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "seq_no:[%s]", dbtbl_sett_list.seq_no);
    nReturnCode = DbsTblSettList(DBS_SELECT, &dbtbl_sett_list);
	if (nReturnCode)
	{
	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_SELECT ht_tbl_sett_list error [%d].", nReturnCode);
	    return -1;
	}
	RightTrim(dbtbl_sett_list.mcht_rslt_no);
	RightTrim(dbtbl_sett_list.mcht_role);
	RightTrim(dbtbl_sett_list.dc_flag);
	RightTrim(dbtbl_sett_list.status);
	HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_rslt_no:[%s],mcht_role[%s],dc_flag:[%s],status:[%s]", 
	dbtbl_sett_list.mcht_rslt_no, 
	dbtbl_sett_list.mcht_role,
	dbtbl_sett_list.dc_flag, 
	dbtbl_sett_list.status);
	
	memset(&dbtbl_acct_rslt, 0x00, sizeof(dbtbl_acct_rslt));
	memcpy(dbtbl_acct_rslt.mcht_rslt_no, dbtbl_sett_list.mcht_rslt_no, sizeof(dbtbl_acct_rslt.mcht_rslt_no)-1);
	memcpy(dbtbl_acct_rslt.mcht_role, dbtbl_sett_list.mcht_role, sizeof(dbtbl_acct_rslt.mcht_role)-1);
	nReturnCode = DbsTblAcctRslt(DBS_SELECT2, &dbtbl_acct_rslt,0,0);
	if (nReturnCode)
	{
	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_SELECT_MD ht_tbl_acct_rslt error [%d].", nReturnCode);
	    return -1;
	}
	RightTrim(dbtbl_acct_rslt.mcht_rslt_no);
	RightTrim(dbtbl_acct_rslt.sett_md);
	HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_md:[%s]", dbtbl_acct_rslt.sett_md);
	if(!strncmp(dbtbl_acct_rslt.sett_md, "01", 2) && !succ_flag)/* 轧差 */
	    memcpy(dbtbl_acct_rslt.acct_statu, "1", 1);
	else if(!strncmp(dbtbl_acct_rslt.sett_md, "02", 2))/* 收支 */
	{
	    if(!succ_flag)
    	{
    	    if(!strncmp(dbtbl_acct_rslt.acct_statu, "9",1) || 
    	        !strncmp(dbtbl_acct_rslt.acct_statu, "5",1) || 
    	        !strncmp(dbtbl_acct_rslt.acct_statu, "6",1))
    	    {
    	        memcpy(dbtbl_acct_rslt.acct_statu, "1", 1);
    	        if(!strncmp(dbtbl_sett_list.dc_flag, "C", 1))
    	        {
    	            memcpy(dbtbl_acct_rslt.sz_status, "1", 1);   	            
    	        }
    	        else if(!strncmp(dbtbl_sett_list.dc_flag, "D", 1))
    	        {
    	            memcpy(dbtbl_acct_rslt.sz_status, "2", 1);    	            
    	        }
    	    }
    	    else if(!strncmp(dbtbl_acct_rslt.acct_statu, "1",1))
    	    {
    	        memcpy(dbtbl_acct_rslt.sz_status, "3", 1);
    	    }
    	}
	}	
	nReturnCode = DbsTblAcctRslt(DBS_UPDATE, &dbtbl_acct_rslt,0,0);
	if (nReturnCode)
	{
	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPDATE ht_tbl_acct_rslt error [%d].", nReturnCode);
	    return -1;
	}
    
    return 0;
}

/****************************** end of Task_8002.c ******************************/
