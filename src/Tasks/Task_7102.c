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
 *  文 件 名: Task_7102.c
 *  功    能: 结算明细表中待归集记录汇总并归集,归集过程计入归集明细表
 *  编程人员: bcw
 *  开发时间: 2017-03-08
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

int BnkRecdToPlat(char* strBuf, char cTab, tbl_sett_list_def* vtdbl_sett_list);

/*************************************************
*函数名称∶ Total_7102
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_7102()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_7102
*函数功能∶ 结算明细表中待归集记录汇总并归集,归集过程计入归集明细表
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_7102 ( int nBeginOffset, int nEndOffset )
{
	
	char    sDateTime[14+1];
    char    stlm_dt[8+1];    
	double  total_miss_amt;
	double  miss_amt;
	double  avbl_bal;
	int     nReturnCode;
	int     tmpCount;
	char    tmpBankCode[12+1] = {0};
	char    tmpAcctNo[22+1] = {0};
	char    tmpAcctName[70+1] = {0};
	int		nTotalNum = 0;
	int		nPostTrialNum = 0;

    tbl_sett_list_def		dbtbl_sett_list;
	sett_miss_info_def      sett_miss_info;
	tbl_postion_trial_def   dbtbl_post_trial;
	tbl_mission_info_def    dbtbl_miss_info;
	tbl_funds_shrt_def      dbtbl_fund_shrt;

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
	
	memset(sDateTime, 0, sizeof(sDateTime));
	getSysTime(sDateTime);
	/* 游标1从结算明细表中，按照对手结算行统计 待归集 记录，获取 结算行号、缺口资金、记录总笔数按照笔数从大到小排序 */
	memset(&sett_miss_info, 0, sizeof(sett_miss_info));
	memcpy(sett_miss_info.sett_date, stlm_dt, 8);
	memcpy(sett_miss_info.status, "1", 1);
    nReturnCode = DbsTblSettTotal(DBS_CURSOR, &sett_miss_info);    
    nReturnCode = DbsTblSettTotal(DBS_OPEN, &sett_miss_info);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN DbsTblSettTotal error [%d]", nReturnCode);
        return -1;
    }
    tmpCount = 0;
    
    while(1)
    {
        /* fetch1每笔记录 */
        memset(&sett_miss_info, 0x00, sizeof(sett_miss_info));
        nReturnCode = DbsTblSettTotal(DBS_FETCH, &sett_miss_info);
        if (nReturnCode == DBS_NOTFOUND)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_FETCH ht_tbl_postion_trial end.");
            break;
        }
        else if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_FETCH ht_tbl_postion_trial error [%d].", nReturnCode);
            DbsTblSettTotal(DBS_CLOSE, &sett_miss_info);
            return -1;
        }
        nTotalNum++;
        RightTrim(sett_miss_info.bank_no);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "  bank_no:[%s]", sett_miss_info.bank_no);      
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "  total_amt:[%.2f]", sett_miss_info.total_amt);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "  total_num:[%d]", sett_miss_info.total_num);  
	    
	    /* 从头寸试算表中，查询需归集银行头寸信息，计算缺口资金 */
	    memset(&dbtbl_post_trial, 0, sizeof(dbtbl_post_trial));
	    memcpy(dbtbl_post_trial.bank_code, sett_miss_info.bank_no, sizeof(dbtbl_post_trial.bank_code)-1);
	    memcpy(dbtbl_post_trial.sett_date, stlm_dt, sizeof(dbtbl_post_trial.sett_date)-1);
	    nReturnCode = DbsTblPostTrial(DBS_SELECT, &dbtbl_post_trial);
        if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_SELECT DbsTblPostTrial error [%d].", nReturnCode);
            DbsTblSettTotal(DBS_CLOSE, &sett_miss_info);
            return -1;
        }
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "  bal_trial:[%.2f]resv_bal:[%.2f]", dbtbl_post_trial.bal_trial,dbtbl_post_trial.resv_bal);
        total_miss_amt = sett_miss_info.total_amt - (dbtbl_post_trial.bal_trial - dbtbl_post_trial.resv_bal);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "  total miss amt:[%.2f]", total_miss_amt);
        
        memset(tmpBankCode, 0x00, sizeof(tmpBankCode));
        memset(tmpAcctNo, 0x00, sizeof(tmpAcctNo));
        memset(tmpAcctName, 0x00, sizeof(tmpAcctName));
	    RightTrim(dbtbl_post_trial.bank_code);	    
	    memcpy(tmpBankCode, dbtbl_post_trial.bank_code, sizeof(tmpBankCode)-1);
	    RightTrim(dbtbl_post_trial.acct_no);
	    memcpy(tmpAcctNo, dbtbl_post_trial.acct_no, sizeof(tmpAcctNo)-1);
	    RightTrim(dbtbl_post_trial.acct_name);
	    memcpy(tmpAcctName, dbtbl_post_trial.acct_name, sizeof(tmpAcctName)-1);
	    
	    /*插入资金缺口信息表*/
	    memset(&dbtbl_fund_shrt, 0, sizeof(dbtbl_fund_shrt));
	    memcpy(dbtbl_fund_shrt.bank_code, dbtbl_post_trial.bank_code, sizeof(dbtbl_fund_shrt.bank_code)-1);
	    memcpy(dbtbl_fund_shrt.acct_no, dbtbl_post_trial.acct_no, sizeof(dbtbl_fund_shrt.acct_no)-1);
	    memcpy(dbtbl_fund_shrt.acct_name, dbtbl_post_trial.acct_name, sizeof(dbtbl_fund_shrt.acct_name)-1);
	    dbtbl_fund_shrt.shrt_amt = total_miss_amt;
	    memcpy(dbtbl_fund_shrt.sett_date, stlm_dt, 8);
	    nReturnCode = DbsTblFundShrt(DBS_INSERT, &dbtbl_fund_shrt);
        if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_INSERT ht_tbl_funds_short error [%d]", nReturnCode);
            DbsTblSettTotal(DBS_CLOSE, &sett_miss_info);
            return -1;
        }
        
	    /* 从头寸试算表中，依次取利率低(其他规则)的银行作为归集银行 */
	    memcpy(dbtbl_post_trial.sett_date, stlm_dt, sizeof(dbtbl_post_trial.sett_date)-1);
	    nReturnCode = DbsTblPostTrial(DBS_CURSOR, &dbtbl_post_trial);       
        nReturnCode = DbsTblPostTrial(DBS_OPEN, &dbtbl_post_trial);
        if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN tbl_postion_trial_cur error [%d]", nReturnCode);
            DbsTblSettTotal(DBS_CLOSE, &sett_miss_info);
            return -1;
        }
        
        while(1)
        {
            /* fetch 每条未入账的记录 */
            memset(&dbtbl_post_trial, 0, sizeof(dbtbl_post_trial));
            nReturnCode = DbsTblPostTrial(DBS_FETCH, &dbtbl_post_trial);
            if (nReturnCode == DBS_NOTFOUND)
            {
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_FETCH tbl_postion_trial_cur end.nPostTrialNum=[%d]",nPostTrialNum);
                break;
            }
            else if (nReturnCode)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_FETCH tbl_postion_trial_cur error [%d]", nReturnCode);
                DbsTblSettTotal(DBS_CLOSE, &sett_miss_info);
                return -1;
            }
            
            if(total_miss_amt <= 0.000001)
            {
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "MISS BANK:[%d] succ.", sett_miss_info.bank_no);
                break;
            }
            nPostTrialNum++;
            avbl_bal = dbtbl_post_trial.bal_trial-dbtbl_post_trial.resv_bal;/* 可用余额 */
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "bal_trial:[%.lf]", dbtbl_post_trial.bal_trial);
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "resv_bal:[%.lf]", dbtbl_post_trial.resv_bal);
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "total_miss_amt:[%.lf]", total_miss_amt);
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "acct_no:[%s]", dbtbl_post_trial.acct_no);
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "avbl_bal:[%.lf]", avbl_bal);
            
            memset(&dbtbl_miss_info, 0, sizeof(dbtbl_miss_info));
            memcpy(dbtbl_miss_info.sett_date, stlm_dt, 8);
            memcpy(dbtbl_miss_info.trans_date, stlm_dt, 8);
    	    memcpy(dbtbl_miss_info.bank_code, tmpBankCode, sizeof(dbtbl_miss_info.bank_code)-1);
    	    memcpy(dbtbl_miss_info.acct_no, tmpAcctNo, sizeof(dbtbl_miss_info.acct_no)-1);
    	    memcpy(dbtbl_miss_info.acct_name, tmpAcctName, sizeof(dbtbl_miss_info.acct_name)-1);
    	    
            if(avbl_bal <= 0.000001)
            {
                continue;
            }
            else if(avbl_bal >= total_miss_amt)
            {
                dbtbl_miss_info.amt = total_miss_amt;
                dbtbl_post_trial.bal_trial -= total_miss_amt;
                total_miss_amt -= dbtbl_miss_info.amt;
            }
            else
            {
                total_miss_amt -= avbl_bal;
                dbtbl_miss_info.amt = avbl_bal;
                dbtbl_post_trial.bal_trial -= avbl_bal;
            }
            
            
    	    memcpy(dbtbl_miss_info.seq_no, stlm_dt, 8);
    	    sprintf(dbtbl_miss_info.seq_no+8, "%011d", tmpCount);
            RightTrim(dbtbl_post_trial.bank_code);
    	    memcpy(dbtbl_miss_info.bank_code1, dbtbl_post_trial.bank_code, sizeof(dbtbl_miss_info.bank_code1)-1);
    	    RightTrim(dbtbl_post_trial.acct_no);
    	    memcpy(dbtbl_miss_info.acct_no1, dbtbl_post_trial.acct_no, sizeof(dbtbl_miss_info.acct_no1)-1);
    	    RightTrim(dbtbl_post_trial.acct_name);
    	    memcpy(dbtbl_miss_info.acct_name1, dbtbl_post_trial.acct_name, sizeof(dbtbl_miss_info.acct_name1)-1);
    	    memcpy(dbtbl_miss_info.status, "0", 1);
    	    
    	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "MISS RECODE INFO:");
    	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==seq_no:[%s]", dbtbl_miss_info.seq_no);        
    	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==sett_date:[%s]", dbtbl_miss_info.sett_date);  
    	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==trans_date:[%s]", dbtbl_miss_info.trans_date);
    	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==bank_code:[%s]", dbtbl_miss_info.bank_code);  
    	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==acct_no:[%s]", dbtbl_miss_info.acct_no);      
    	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==acct_name:[%s]", dbtbl_miss_info.acct_name);  
    	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==bank_code1:[%s]", dbtbl_miss_info.bank_code1);
    	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==acct_no1:[%s]", dbtbl_miss_info.acct_no1);    
    	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==acct_name1:[%s]", dbtbl_miss_info.acct_name1);
    	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==amt:[%.2lf]", dbtbl_miss_info.amt);
    	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==status:[%s]", dbtbl_miss_info.status);        
    	    
    	    /* 填写归集信息到归集明细表 */
    	    nReturnCode = DbsTblMissInfo(DBS_INSERT, &dbtbl_miss_info);
            if (nReturnCode)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_INSERT error [%d]", nReturnCode);
                DbsTblSettTotal(DBS_CLOSE, &sett_miss_info);
                return -1;
            }
            
            /* 更新头寸试算表余额信息 */
            nReturnCode = DbsTblPostTrial(DBS_UPD_BAL, &dbtbl_post_trial);
            if (nReturnCode)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPD_BAL ht_tbl_postion_trial error [%d]", nReturnCode);
                DbsTblSettTotal(DBS_CLOSE, &sett_miss_info);
                return -1;
            }
            tmpCount++;
        }
        
        /* 更新结算明细表中该行需归集记录为归集中 */
        memset(&dbtbl_sett_list, 0, sizeof(dbtbl_sett_list));
        memcpy(dbtbl_sett_list.plat_sett_bank, sett_miss_info.bank_no, sizeof(dbtbl_sett_list.plat_sett_bank)-1);
        memcpy(dbtbl_sett_list.sett_date, stlm_dt, 8);
        memcpy(dbtbl_sett_list.status, "9", 1);
        nReturnCode = DbsTblSettList(DBS_UPDATE, &dbtbl_sett_list);
        if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPDATE ht_tbl_sett_list error [%d]", nReturnCode);
            DbsTblSettTotal(DBS_CLOSE, &sett_miss_info);
            return -1;
        }
    }
    DbsTblSettTotal(DBS_CLOSE, &sett_miss_info);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nTotalNum:[%d]", nTotalNum);
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_7102 Succ .");
	return 0;
}

/****************************** end of Task_7102.c ******************************/


