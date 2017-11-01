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
 *  文 件 名: Task_0800.c
 *  功    能: 支付宝微信多处理
 *  编程人员: 
 *  开发时间: 2017-1-4
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
static int in_err_flow(char *flag_result, tbl_alipay_wechat_def *ptbl_alipay_wechat, ht_tbl_host_txn_def *pht_tbl_host_txn)
{
    ht_tbl_err_flow_def tbl_err_flow;
    int iRet = 0;
    
    memset(&tbl_err_flow, 0x00, sizeof(tbl_err_flow));
    memcpy(tbl_err_flow.inter_brh_code,  ext_inter_brh_code, sizeof(tbl_err_flow.inter_brh_code)-1);      
    memcpy(tbl_err_flow.sett_date	,    dbtbl_date_inf.stoday,  sizeof(tbl_err_flow.sett_date)-1);

    memcpy(tbl_err_flow.trans_date	,    ptbl_alipay_wechat->trans_date,  sizeof(tbl_err_flow.trans_date)-1);
    memcpy(tbl_err_flow.trans_time	,     ptbl_alipay_wechat->trans_time,  sizeof(tbl_err_flow.trans_time)-1);
    memcpy(tbl_err_flow.flag_result	,  flag_result,  BT_FLAG_RESULT_LEN);
    memcpy(tbl_err_flow.order_no	,     ptbl_alipay_wechat->order_no,  sizeof(tbl_err_flow.order_no)-1);
    memcpy(tbl_err_flow.plat_key	,     ptbl_alipay_wechat->plat_key,  sizeof(tbl_err_flow.plat_key)-1);
    memcpy(tbl_err_flow.batch_no	,    ptbl_alipay_wechat->batch_no,  sizeof(tbl_err_flow.batch_no)-1);
    
    if(strlen(pht_tbl_host_txn->bus_tp) == 0)
    {
        memcpy(tbl_err_flow.bus_tp	,      ptbl_alipay_wechat->bus_tp,  sizeof(tbl_err_flow.bus_tp)-1);
        memcpy(tbl_err_flow.trans_tp	,   ptbl_alipay_wechat->trans_tp,  sizeof(tbl_err_flow.trans_tp)-1);
        memcpy(tbl_err_flow.acct_type	,    ptbl_alipay_wechat->acct_type,  sizeof(tbl_err_flow.acct_type)-1);
    }
    else
    {
        memcpy(tbl_err_flow.bus_tp	,      pht_tbl_host_txn->bus_tp,  sizeof(tbl_err_flow.bus_tp)-1);
        memcpy(tbl_err_flow.trans_tp	,   pht_tbl_host_txn->trans_tp,  sizeof(tbl_err_flow.trans_tp)-1);
        memcpy(tbl_err_flow.acct_type	,    pht_tbl_host_txn->acct_type,  sizeof(tbl_err_flow.acct_type)-1);
    }
    memcpy(tbl_err_flow.trans_state	,    ptbl_alipay_wechat->trans_state,  sizeof(tbl_err_flow.trans_state)-1);
    memcpy(tbl_err_flow.pan	,   ptbl_alipay_wechat->pan,  sizeof(tbl_err_flow.pan)-1);
    memcpy(tbl_err_flow.ccy	,      ptbl_alipay_wechat->ccy,  sizeof(tbl_err_flow.ccy)-1);
    memcpy(tbl_err_flow.trans_at	,   ptbl_alipay_wechat->trans_at,  sizeof(tbl_err_flow.trans_at)-1);
    memcpy(tbl_err_flow.dest_pan	,     pht_tbl_host_txn->pan,  sizeof(tbl_err_flow.dest_pan)-1);
    memcpy(tbl_err_flow.dest_ccy	,      pht_tbl_host_txn->ccy,  sizeof(tbl_err_flow.dest_ccy)-1);
    memcpy(tbl_err_flow.dest_trans_at,   pht_tbl_host_txn->trans_at,  sizeof(tbl_err_flow.dest_trans_at)-1);
    iRet = DbsTblErrFlow(DBS_INSERT, &tbl_err_flow);
    if (iRet != 0 && iRet != DBS_KEYDUPLICATE)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblErrFlow DBS_INSERT error, %d.", iRet);
        return -1;
    }
    else if(iRet == DBS_KEYDUPLICATE)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "plat_key[%s]",ptbl_alipay_wechat->plat_key);
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblErrFlow DBS_KEYDUPLICATE");
    }     
    /*更新核心流水对账状态*/
    memcpy(pht_tbl_host_txn->flag_result, flag_result,  BT_FLAG_RESULT_LEN);
    iRet = DbsTblHostTxn(DBS_UPDATE, pht_tbl_host_txn, 0, 0);
    if (iRet && iRet != DBS_NOTFOUND)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblHostTxn DBS_UPDATE sqlcode[%d]", iRet);
        return -1;
    }

    /*更新支付宝流水对账状态*/
    memcpy(ptbl_alipay_wechat->flag_result, flag_result, BT_FLAG_RESULT_LEN);
    iRet = DbsTblAlipayWechatTxn(DBS_UPDATE, ptbl_alipay_wechat, 0, 0);
    if (iRet && iRet != DBS_NOTFOUND)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAlipayWechatTxn DBS_UPDATE sqlcode[%d]", iRet);
        return -1;
    }
    return 0;
}

/*************************************************
*函数名称∶ Total_0800
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_0800()
{
    int		nTotalNum = 0;
    int     iRet = 0;
	tbl_alipay_wechat_def tbl_alipay_wechat;

    memset(&tbl_alipay_wechat, 0x00, sizeof(tbl_alipay_wechat));
    memcpy(tbl_alipay_wechat.sett_date, dbtbl_date_inf.stoday, sizeof(tbl_alipay_wechat.sett_date)-1);
    memcpy(tbl_alipay_wechat.inter_brh_code, ext_inter_brh_code, sizeof(tbl_alipay_wechat.inter_brh_code)-1);
    memcpy(tbl_alipay_wechat.flag_result, BT_FLAG_RESULT_NULL, BT_FLAG_RESULT_LEN);
    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "sett_date=[%s][%s][%s].", tbl_alipay_wechat.sett_date, tbl_alipay_wechat.inter_brh_code, tbl_alipay_wechat.flag_result);
    iRet = DbsTblAlipayWechatTxn(DBS_SELECT1, &tbl_alipay_wechat, 0, 0);
    if (iRet != 0)
    {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAlipayWechatTxn DBS_SELECT1 error, %d.", iRet);
            return -1;
    }
    nTotalNum = tbl_alipay_wechat.seq_num;
    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "nTotalNum =[%d].", nTotalNum);

    return nTotalNum;
}

/*************************************************
*函数名称∶ Task_0800
*函数功能∶
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_0800 ( int nBeginOffset, int nEndOffset )
{
    int		iRet = 0, nTotCount=0;
    tbl_alipay_wechat_def tbl_alipay_wechat;
    ht_tbl_host_txn_def  ht_tbl_host_txn;
	ht_tbl_err_flow_def tbl_err_flow;

    memset(&tbl_alipay_wechat, 0x00, sizeof(tbl_alipay_wechat));
    memcpy(tbl_alipay_wechat.sett_date, dbtbl_date_inf.stoday, sizeof(tbl_alipay_wechat.sett_date)-1);
    memcpy(tbl_alipay_wechat.inter_brh_code, ext_inter_brh_code, sizeof(tbl_alipay_wechat.inter_brh_code)-1);
    /*支付宝多*/
    memcpy(tbl_alipay_wechat.flag_result, BT_FLAG_RESULT_NULL, BT_FLAG_RESULT_LEN);
    DbsTblAlipayWechatTxn(DBS_CURSOR1, &tbl_alipay_wechat, nBeginOffset, nEndOffset);
    iRet = DbsTblAlipayWechatTxn(DBS_OPEN1, &tbl_alipay_wechat, nBeginOffset, nEndOffset);
    if (iRet != 0)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAlipayWechatTxn DBS_OPEN1 error, %d.", iRet);
        return -1;
    }

    while (1)
    {
        memset(&tbl_alipay_wechat, 0, sizeof(tbl_alipay_wechat));
        iRet = DbsTblAlipayWechatTxn(DBS_FETCH1, &tbl_alipay_wechat, nBeginOffset, nEndOffset);
        if (iRet)
        {
            if (iRet == DBS_NOTFOUND)
            {
                break;
            }

            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAlipayWechatTxn fetch1 sqlcode[%d]", iRet);
            DbsTblAlipayWechatTxn(DBS_CLOSE1, &tbl_alipay_wechat, nBeginOffset, nEndOffset);
            return -1;
        }
        RightTrim(tbl_alipay_wechat.plat_key);
        
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "-----------------plat_key[%s]--------------------", tbl_alipay_wechat.plat_key);
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "ht_tbl_host_txn.sett_date[%s]dbtbl_date_inf.stoday[%s]", tbl_alipay_wechat.sett_date, dbtbl_date_inf.stoday);
        memset(&ht_tbl_host_txn, 0x00, sizeof(ht_tbl_host_txn));
        memcpy(ht_tbl_host_txn.inter_brh_code, tbl_alipay_wechat.inter_brh_code, sizeof(ht_tbl_host_txn.inter_brh_code)-1);
        memcpy(ht_tbl_host_txn.plat_key, tbl_alipay_wechat.plat_key, sizeof(ht_tbl_host_txn.plat_key)-1);
        RightTrim(ht_tbl_host_txn.plat_key);
        RightTrim(ht_tbl_host_txn.inter_brh_code);
        iRet = DbsTblHostTxn(DBS_SELECT3, &ht_tbl_host_txn, nBeginOffset, nEndOffset);
        if (iRet != 0)
        {
            if (iRet == DBS_NOTFOUND)
            {
                /*T+2对不平才登记对账不平表*/
                /*支付宝多*/
                //if(memcmp(tbl_alipay_wechat.sett_date, dbtbl_date_inf.stoday, BT_DATE_LEN) < 0)
                //{
                    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "支付宝多");
                    iRet = in_err_flow(BT_FLAG_RESULT_PART, &tbl_alipay_wechat, &ht_tbl_host_txn);
                    if (iRet)
                    {
                        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_err_flow err[%d]", iRet);
                        DbsTblAlipayWechatTxn(DBS_CLOSE1, &tbl_alipay_wechat, nBeginOffset, nEndOffset);
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
        RightTrim(ht_tbl_host_txn.plat_key);
        RightTrim(ht_tbl_host_txn.inter_brh_code);
        /*支付网关无*/
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "支付网关无");
        iRet = in_err_flow(BT_FLAG_RESULT_GATEWAYNULL, &tbl_alipay_wechat, &ht_tbl_host_txn);
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
					  DbsTblAlipayWechatTxn(DBS_CLOSE1, &tbl_alipay_wechat, nBeginOffset, nEndOffset);
                      return -1;
                } 
			}
			else
			{
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_err_flow err[%d]", iRet);
                DbsTblAlipayWechatTxn(DBS_CLOSE1, &tbl_alipay_wechat, nBeginOffset, nEndOffset);
                return -1;
			}
        }
        
        nTotCount++;
    }
    DbsTblAlipayWechatTxn(DBS_CLOSE1, &tbl_alipay_wechat, nBeginOffset, nEndOffset);
    
    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "total num [%d].", nTotCount);
    HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_0800 Succ .");
    return 0;
}
/****************************** end of Task_0800.c ******************************/

