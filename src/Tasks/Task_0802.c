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
 *  文 件 名: Task_0802.c
 *  功    能: 兴业银行多或支付网关无处理
 *  编程人员: wyl
 *  开发时间: 2017-3-28
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
*函数名称∶ in_err_flow
*函数功能∶插入差错表
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
static int in_err_flow(char *flag_result, ht_tbl_cib_txn_def *pht_tbl_cib_txn, ht_tbl_host_txn_def *pht_tbl_host_txn)
{
    ht_tbl_err_flow_def tbl_err_flow;
    int iRet = 0;
    
    memset(&tbl_err_flow, 0x00, sizeof(tbl_err_flow));
    memcpy(tbl_err_flow.inter_brh_code,  ext_inter_brh_code, sizeof(tbl_err_flow.inter_brh_code)-1);      
    memcpy(tbl_err_flow.sett_date	,    dbtbl_date_inf.stoday,  sizeof(tbl_err_flow.sett_date)-1);
    memcpy(tbl_err_flow.trans_date	,    pht_tbl_cib_txn->transdate,  sizeof(tbl_err_flow.trans_date)-1);
    memcpy(tbl_err_flow.trans_time	,     pht_tbl_cib_txn->epaytime+BT_DATE_LEN,  BT_TIME_LEN);
    memcpy(tbl_err_flow.flag_result	,  flag_result,  BT_FLAG_RESULT_LEN);
    memcpy(tbl_err_flow.order_no	,     pht_tbl_cib_txn->order_no,  sizeof(tbl_err_flow.order_no)-1);
    memcpy(tbl_err_flow.plat_key	,     pht_tbl_cib_txn->plat_key,  sizeof(tbl_err_flow.plat_key)-1);
    memcpy(tbl_err_flow.batch_no	,    pht_tbl_cib_txn->batch_no,  sizeof(tbl_err_flow.batch_no)-1);
    
    if(strlen(pht_tbl_host_txn->bus_tp) == 0)
    {
        memcpy(tbl_err_flow.bus_tp	,      pht_tbl_cib_txn->bus_tp,  sizeof(tbl_err_flow.bus_tp)-1);
        memcpy(tbl_err_flow.trans_tp	,   pht_tbl_cib_txn->trans_tp,  sizeof(tbl_err_flow.trans_tp)-1);
        memcpy(tbl_err_flow.acct_type	,    pht_tbl_cib_txn->acct_type,  sizeof(tbl_err_flow.acct_type)-1);
    }
    else
    {
        memcpy(tbl_err_flow.bus_tp	,      pht_tbl_host_txn->bus_tp,  sizeof(tbl_err_flow.bus_tp)-1);
        memcpy(tbl_err_flow.trans_tp	,   pht_tbl_host_txn->trans_tp,  sizeof(tbl_err_flow.trans_tp)-1);
        memcpy(tbl_err_flow.acct_type	,    pht_tbl_host_txn->acct_type,  sizeof(tbl_err_flow.acct_type)-1);
    }
    
    RightTrim(pht_tbl_cib_txn->orderamt);
    RightTrim(pht_tbl_cib_txn->feeamt);
    if(strlen(pht_tbl_cib_txn->orderamt) != 0)
    {
        memcpy(tbl_err_flow.trans_fee,   pht_tbl_cib_txn->feeamt,  sizeof(tbl_err_flow.trans_fee)-1);
        sprintf(tbl_err_flow.debt_at,"%12.0lf", floor(atof(pht_tbl_cib_txn->orderamt) - atof(pht_tbl_cib_txn->feeamt) + 0.50001));
    }

    memcpy(tbl_err_flow.trans_state	,    pht_tbl_cib_txn->trans_state,  sizeof(tbl_err_flow.trans_state)-1);
    memcpy(tbl_err_flow.pan	,   pht_tbl_cib_txn->pan,  sizeof(tbl_err_flow.pan)-1);
    memcpy(tbl_err_flow.pan1	,   pht_tbl_host_txn->pan,  sizeof(tbl_err_flow.pan1)-1);
    memcpy(tbl_err_flow.ccy	,      pht_tbl_cib_txn->ccy,  sizeof(tbl_err_flow.ccy)-1);
    memcpy(tbl_err_flow.trans_at	,   pht_tbl_cib_txn->orderamt,  sizeof(tbl_err_flow.trans_at)-1);
    memcpy(tbl_err_flow.dest_pan	,     pht_tbl_host_txn->pan,  sizeof(tbl_err_flow.dest_pan)-1);
    memcpy(tbl_err_flow.dest_ccy	,      pht_tbl_host_txn->ccy,  sizeof(tbl_err_flow.dest_ccy)-1);
    memcpy(tbl_err_flow.dest_trans_at,   pht_tbl_host_txn->trans_at,  sizeof(tbl_err_flow.dest_trans_at)-1);   
    iRet = DbsTblErrFlow(DBS_INSERT, &tbl_err_flow);
    if (iRet != 0)
    {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblErrFlow DBS_INSERT error, %d.", iRet);
            return -1;
    }
    
    /*更新核心流水对账状态*/
    memcpy(pht_tbl_host_txn->flag_result, flag_result,  BT_FLAG_RESULT_LEN);
    iRet = DbsTblHostTxn(DBS_UPDATE, pht_tbl_host_txn, 0, 0);
    if (iRet && iRet != DBS_NOTFOUND)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblHostTxn DBS_UPDATE sqlcode[%d]", iRet);
        return -1;
    }

    /*更新兴业银行流水对账状态*/
    memcpy(pht_tbl_cib_txn->flag_result, flag_result, BT_FLAG_RESULT_LEN);
    iRet = DbsTblCIBTxn(DBS_UPDATE, pht_tbl_cib_txn, 0, 0);
    if (iRet && iRet != DBS_NOTFOUND)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblCIBTxn DBS_UPDATE sqlcode[%d]", iRet);
        return -1;
    }
    return 0;
}

/*************************************************
*函数名称∶ Total_0802
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_0802()
{
    int		nTotalNum = 0;
    int     iRet = 0;
    ht_tbl_cib_txn_def ht_tbl_cib_txn;

    memset(&ht_tbl_cib_txn, 0x00, sizeof(ht_tbl_cib_txn));
    memcpy(ht_tbl_cib_txn.sett_date, dbtbl_date_inf.stoday, sizeof(ht_tbl_cib_txn.sett_date)-1);
    memcpy(ht_tbl_cib_txn.inter_brh_code, ext_inter_brh_code, sizeof(ht_tbl_cib_txn.inter_brh_code)-1);
    memcpy(ht_tbl_cib_txn.flag_result, BT_FLAG_RESULT_NULL, BT_FLAG_RESULT_LEN);
    iRet = DbsTblCIBTxn(DBS_SELECT1, &ht_tbl_cib_txn, 0, 0);
    if (iRet != 0)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblCIBTxn DBS_SELECT1 error, %d.", iRet);
        return -1;
    }
    nTotalNum = ht_tbl_cib_txn.seq_num;
    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "nTotalNum =[%d].", nTotalNum);

    return nTotalNum;
}

/*************************************************
*函数名称∶ Task_0802
*函数功能∶
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_0802 ( int nBeginOffset, int nEndOffset )
{
    int		iRet = 0, nTotCount=0;
    ht_tbl_cib_txn_def ht_tbl_cib_txn;
    ht_tbl_host_txn_def  ht_tbl_host_txn;
	ht_tbl_err_flow_def tbl_err_flow;

    memset(&ht_tbl_cib_txn, 0x00, sizeof(ht_tbl_cib_txn));
    memcpy(ht_tbl_cib_txn.sett_date, dbtbl_date_inf.stoday, sizeof(ht_tbl_cib_txn.sett_date)-1);
    memcpy(ht_tbl_cib_txn.inter_brh_code, ext_inter_brh_code, sizeof(ht_tbl_cib_txn.inter_brh_code)-1);
    /*兴业银行多*/
    memcpy(ht_tbl_cib_txn.flag_result, BT_FLAG_RESULT_NULL, BT_FLAG_RESULT_LEN);
    DbsTblCIBTxn(DBS_CURSOR1, &ht_tbl_cib_txn, nBeginOffset, nEndOffset);
    iRet = DbsTblCIBTxn(DBS_OPEN1, &ht_tbl_cib_txn, nBeginOffset, nEndOffset);
    if (iRet != 0)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblCIBTxn DBS_OPEN1 error, %d.", iRet);
        return -1;
    }

    while (1)
    {
        memset(&ht_tbl_cib_txn, 0, sizeof(ht_tbl_cib_txn));
        iRet = DbsTblCIBTxn(DBS_FETCH1, &ht_tbl_cib_txn, nBeginOffset, nEndOffset);
        if (iRet)
        {
            if (iRet == DBS_NOTFOUND)
            {
                break;
            }

            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblCIBTxn fetch1 sqlcode[%d]", iRet);
            DbsTblCIBTxn(DBS_CLOSE1, &ht_tbl_cib_txn, nBeginOffset, nEndOffset);
            return -1;
        }
        RightTrim(ht_tbl_cib_txn.inter_brh_code);
        RightTrim(ht_tbl_cib_txn.plat_key);
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "-----------------plat_key[%s]--------------------", ht_tbl_cib_txn.plat_key);
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "ht_tbl_host_txn.sett_date[%s]dbtbl_date_inf.stoday[%s]", ht_tbl_cib_txn.sett_date, dbtbl_date_inf.stoday);
        memset(&ht_tbl_host_txn, 0x00, sizeof(ht_tbl_host_txn));
        memcpy(ht_tbl_host_txn.inter_brh_code, ht_tbl_cib_txn.inter_brh_code, sizeof(ht_tbl_host_txn.inter_brh_code)-1);
        memcpy(ht_tbl_host_txn.plat_key, ht_tbl_cib_txn.plat_key, sizeof(ht_tbl_host_txn.plat_key)-1);
        RightTrim(ht_tbl_host_txn.plat_key);
        RightTrim(ht_tbl_host_txn.inter_brh_code);
        iRet = DbsTblHostTxn(DBS_SELECT3, &ht_tbl_host_txn, nBeginOffset, nEndOffset);
        if (iRet != 0)
        {
            if (iRet == DBS_NOTFOUND)
            {
                /*T+2对不平才登记对账不平表*/
                //if(memcmp(ht_tbl_cib_txn.sett_date, dbtbl_date_inf.stoday, BT_DATE_LEN) < 0)
                //{
                    /*兴业银行多*/
                    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "兴业银行多");
                    iRet = in_err_flow(BT_FLAG_RESULT_PART, &ht_tbl_cib_txn, &ht_tbl_host_txn);
                    if (iRet)
                    {
                        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_err_flow err[%d]", iRet);
                        DbsTblCIBTxn(DBS_CLOSE1, &ht_tbl_cib_txn, nBeginOffset, nEndOffset);
                        return -1;
                    }                   
                //}
                continue;
            }
            else
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblHostTxn DBS_SELECT3 error, %d.", iRet);
                return -1;
            }
        }
        RightTrim(ht_tbl_host_txn.inter_brh_code);
        RightTrim(ht_tbl_host_txn.plat_key);
        /*支付网关无*/
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "支付网关无");
        iRet = in_err_flow(BT_FLAG_RESULT_GATEWAYNULL, &ht_tbl_cib_txn, &ht_tbl_host_txn);
        if (iRet)
        {
			if(-1 == iRet) 
			{
                memset(&tbl_err_flow, 0, sizeof(tbl_err_flow));
				memcpy(tbl_err_flow.inter_brh_code, "100003", sizeof(tbl_err_flow.inter_brh_code)-1);
				memcpy(tbl_err_flow.sett_date, dbtbl_date_inf.syesterday, sizeof(tbl_err_flow.sett_date)-1);
				memcpy(tbl_err_flow.plat_key, ht_tbl_host_txn.plat_key, sizeof(tbl_err_flow.plat_key)-1);
				memcpy(tbl_err_flow.flag_result, BT_FLAG_RESULT_GATEWAYNULL, BT_FLAG_RESULT_LEN);
				iRet = DbsTblErrFlow(DBS_UPDATE, &tbl_err_flow);
				if (iRet)
                {
                      HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblErrFlow DBS_UPDATE sqlcode[%d]", iRet);
					  DbsTblCIBTxn(DBS_CLOSE1, &ht_tbl_cib_txn, nBeginOffset, nEndOffset);
                      return -1;
                } 
			}
			else
			{
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_err_flow err[%d]", iRet);
                DbsTblCIBTxn(DBS_CLOSE1, &ht_tbl_cib_txn, nBeginOffset, nEndOffset);
                return -1;
			}
        }
        
        nTotCount++;
    }
    DbsTblCIBTxn(DBS_CLOSE1, &ht_tbl_cib_txn, nBeginOffset, nEndOffset);
    
    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "total num [%d].", nTotCount);
    HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_0802 Succ .");
    return 0;
}
/****************************** end of Task_0802.c ******************************/

