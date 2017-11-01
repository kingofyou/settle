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
 *  文 件 名: Task_0701.c
 *  功    能: 支付网关与核心对账
 *  编程人员: HELU
 *  开发时间: 2016-12-20
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
static int in_err_flow(char *flag_result, ht_tbl_host_txn_def *pht_tbl_host_txn, ht_tbl_gateway_txn_def *pht_tbl_gateway_txn)
{
    ht_tbl_err_flow_def tbl_err_flow;
    int iRet = 0;
    
    memset(&tbl_err_flow, 0x00, sizeof(tbl_err_flow));
    memcpy(tbl_err_flow.inter_brh_code,  ext_inter_brh_code, sizeof(tbl_err_flow.inter_brh_code)-1);      
    memcpy(tbl_err_flow.sett_date	,    dbtbl_date_inf.stoday,  sizeof(tbl_err_flow.sett_date)-1);
    memcpy(tbl_err_flow.trans_date	,    pht_tbl_gateway_txn->trans_date,  sizeof(tbl_err_flow.trans_date)-1);
    memcpy(tbl_err_flow.trans_time	,     pht_tbl_gateway_txn->trans_time,  sizeof(tbl_err_flow.trans_time)-1);
    memcpy(tbl_err_flow.flag_result	,  flag_result,  BT_FLAG_RESULT_LEN);
    memcpy(tbl_err_flow.order_no	,     pht_tbl_gateway_txn->order_no,  sizeof(tbl_err_flow.order_no)-1);
    memcpy(tbl_err_flow.plat_key	,     pht_tbl_gateway_txn->plat_key,  sizeof(tbl_err_flow.plat_key)-1);
    memcpy(tbl_err_flow.batch_no	,    pht_tbl_gateway_txn->batch_no,  sizeof(tbl_err_flow.batch_no)-1);
    memcpy(tbl_err_flow.bus_tp	,      pht_tbl_gateway_txn->bus_tp,  sizeof(tbl_err_flow.bus_tp)-1);
    memcpy(tbl_err_flow.trans_tp	,   pht_tbl_gateway_txn->trans_tp,  sizeof(tbl_err_flow.trans_tp)-1);
    memcpy(tbl_err_flow.acct_type	,    pht_tbl_gateway_txn->acct_type,  sizeof(tbl_err_flow.acct_type)-1);
    memcpy(tbl_err_flow.trans_state	,    pht_tbl_gateway_txn->trans_state,  sizeof(tbl_err_flow.trans_state)-1);
    memcpy(tbl_err_flow.pan	,   pht_tbl_gateway_txn->pan,  sizeof(tbl_err_flow.pan)-1);
    memcpy(tbl_err_flow.ccy	,      pht_tbl_gateway_txn->ccy,  sizeof(tbl_err_flow.ccy)-1);
    memcpy(tbl_err_flow.trans_at	,   pht_tbl_gateway_txn->trans_at,  sizeof(tbl_err_flow.trans_at)-1);
    memcpy(tbl_err_flow.dest_pan	,     pht_tbl_host_txn->pan,  sizeof(tbl_err_flow.dest_pan)-1);
    memcpy(tbl_err_flow.pan1	,     pht_tbl_gateway_txn->pan1,  sizeof(tbl_err_flow.pan1)-1);
    memcpy(tbl_err_flow.dest_ccy	,      pht_tbl_host_txn->ccy,  sizeof(tbl_err_flow.dest_ccy)-1);
    memcpy(tbl_err_flow.dest_trans_at,   pht_tbl_host_txn->trans_at,  sizeof(tbl_err_flow.dest_trans_at)-1);

    iRet = DbsTblErrFlow(DBS_INSERT, &tbl_err_flow);
    if (iRet != 0)
    {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblErrFlow DBS_INSERT error, %d.", iRet);
            return -1;
    }
/*更新网关流水对账状态*/
    memcpy(pht_tbl_gateway_txn->flag_result, flag_result, BT_FLAG_RESULT_LEN);
    iRet=DbsTblGatewayTxn(DBS_UPDATE, pht_tbl_gateway_txn, 0, 0);
    if (iRet)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblGatewayTxn DBS_UPDATE sqlcode[%d]", iRet);
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
    return 0;
}

/*************************************************
*函数名称∶ in_cmp_flow
*函数功能∶插入对账结果表
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
static int in_cmp_flow(char *flag_result, ht_tbl_host_txn_def *pht_tbl_host_txn, ht_tbl_gateway_txn_def *pht_tbl_gateway_txn)
{
    ht_tbl_cmp_flow_def tbl_cmp_flow;
    int iRet = 0;

    memset(&tbl_cmp_flow, 0x00, sizeof(tbl_cmp_flow));
    memcpy(tbl_cmp_flow.inter_brh_code,  ext_inter_brh_code, sizeof(tbl_cmp_flow.inter_brh_code)-1);      
    memcpy(tbl_cmp_flow.sett_date	,    dbtbl_date_inf.stoday,  sizeof(tbl_cmp_flow.sett_date)-1);    
    memcpy(tbl_cmp_flow.trans_date	,    pht_tbl_gateway_txn->trans_date,  sizeof(tbl_cmp_flow.trans_date)-1);
    memcpy(tbl_cmp_flow.trans_time	,     pht_tbl_gateway_txn->trans_time,  sizeof(tbl_cmp_flow.trans_time)-1);
    memcpy(tbl_cmp_flow.flag_result	,  flag_result,  BT_FLAG_RESULT_LEN);
    memcpy(tbl_cmp_flow.order_no	,     pht_tbl_gateway_txn->order_no,  sizeof(tbl_cmp_flow.order_no)-1);
    memcpy(tbl_cmp_flow.plat_key	,     pht_tbl_gateway_txn->plat_key,  sizeof(tbl_cmp_flow.plat_key)-1);
    memcpy(tbl_cmp_flow.batch_no	,    pht_tbl_gateway_txn->batch_no,  sizeof(tbl_cmp_flow.batch_no)-1);
    memcpy(tbl_cmp_flow.bus_tp	,      pht_tbl_gateway_txn->bus_tp,  sizeof(tbl_cmp_flow.bus_tp)-1);
    memcpy(tbl_cmp_flow.trans_tp	,   pht_tbl_gateway_txn->trans_tp,  sizeof(tbl_cmp_flow.trans_tp)-1);
    memcpy(tbl_cmp_flow.acct_type	,    pht_tbl_gateway_txn->acct_type,  sizeof(tbl_cmp_flow.acct_type)-1);
    memcpy(tbl_cmp_flow.trans_state	,    pht_tbl_gateway_txn->trans_state,  sizeof(tbl_cmp_flow.trans_state)-1);
    memcpy(tbl_cmp_flow.revsal_flag	,   pht_tbl_gateway_txn->revsal_flag,  sizeof(tbl_cmp_flow.revsal_flag)-1);
    memcpy(tbl_cmp_flow.key_revsal,      pht_tbl_gateway_txn->key_revsal,  sizeof(tbl_cmp_flow.key_revsal)-1);
     memcpy(tbl_cmp_flow.cancel_flag,   pht_tbl_gateway_txn->cancel_flag,  sizeof(tbl_cmp_flow.cancel_flag)-1);
    memcpy(tbl_cmp_flow.key_cancel,   pht_tbl_gateway_txn->key_cancel,  sizeof(tbl_cmp_flow.key_cancel)-1);
    memcpy(tbl_cmp_flow.acct_name	,     pht_tbl_gateway_txn->acct_name,  sizeof(tbl_cmp_flow.acct_name)-1);
    if(memcmp(pht_tbl_gateway_txn->trans_tp, BT_TRANS_TP_WITHDRAW, BT_TRANS_TP_LEN) == 0)
    {
        /*提现交易取支付网关送的卡号*/
        memcpy(tbl_cmp_flow.pan	,     pht_tbl_gateway_txn->pan,  sizeof(tbl_cmp_flow.pan)-1);
        memcpy(tbl_cmp_flow.pan1	, pht_tbl_host_txn->pan,  sizeof(tbl_cmp_flow.pan1)-1);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "pan:[%s]pan1[%s]...", tbl_cmp_flow.pan, tbl_cmp_flow.pan1);
    }
    else
    {
        memcpy(tbl_cmp_flow.pan	,     pht_tbl_host_txn->pan,  sizeof(tbl_cmp_flow.pan)-1);
        memcpy(tbl_cmp_flow.pan1	, pht_tbl_host_txn->pan,  sizeof(tbl_cmp_flow.pan1)-1);
    }
 
    memcpy(tbl_cmp_flow.ccy	,      pht_tbl_gateway_txn->ccy,  sizeof(tbl_cmp_flow.ccy)-1);
    memcpy(tbl_cmp_flow.trans_at,pht_tbl_gateway_txn->trans_at,  sizeof(tbl_cmp_flow.trans_at)-1);
    memcpy(tbl_cmp_flow.trans_at2,pht_tbl_gateway_txn->trans_at2,  sizeof(tbl_cmp_flow.trans_at2)-1);
    memcpy(tbl_cmp_flow.trans_at3, pht_tbl_gateway_txn->trans_at3,  sizeof(tbl_cmp_flow.trans_at3)-1);
    memcpy(tbl_cmp_flow.trans_at4,pht_tbl_gateway_txn->trans_at4,  sizeof(tbl_cmp_flow.trans_at4)-1);
    memcpy(tbl_cmp_flow.trans_at5, pht_tbl_gateway_txn->trans_at5,  sizeof(tbl_cmp_flow.trans_at5)-1);
    memcpy(tbl_cmp_flow.ext_mcht_no, pht_tbl_gateway_txn->ext_mcht_no,  sizeof(tbl_cmp_flow.ext_mcht_no)-1);
    
    /*赋值结算商户*/
    memcpy(tbl_cmp_flow.sett_tp1,	    BT_SETT_TP_SETT,  BT_SETT_TP_LEN);
    memcpy(tbl_cmp_flow.mcht_no1,	    pht_tbl_gateway_txn->sett_mcht_no,  sizeof(tbl_cmp_flow.mcht_no1)-1);
    memcpy(tbl_cmp_flow.acct_name,	    pht_tbl_gateway_txn->acct_name,  sizeof(tbl_cmp_flow.acct_name)-1);
    CommonRTrim(tbl_cmp_flow.acct_name);
    
    strcpy(tbl_cmp_flow.sett_tp2, " ");
    strcpy(tbl_cmp_flow.mcht_no2, " ");
    strcpy(tbl_cmp_flow.sett_tp3, " ");
    strcpy(tbl_cmp_flow.mcht_no3, " ");
    strcpy(tbl_cmp_flow.sett_tp4, " ");
    strcpy(tbl_cmp_flow.mcht_no4, " ");
    strcpy(tbl_cmp_flow.sett_tp5, " ");
    strcpy(tbl_cmp_flow.mcht_no5, " ");
    strcpy(tbl_cmp_flow.sett_tp6, " ");
    strcpy(tbl_cmp_flow.mcht_no6, " ");
    strcpy(tbl_cmp_flow.sett_tp7, " ");
    strcpy(tbl_cmp_flow.mcht_no7, " ");
    strcpy(tbl_cmp_flow.sett_tp8, " ");
    strcpy(tbl_cmp_flow.mcht_no8, " ");
    strcpy(tbl_cmp_flow.sett_tp9, " ");
    strcpy(tbl_cmp_flow.mcht_no9, " ");
    
    iRet = DbsTblCmpFlow(DBS_INSERT, &tbl_cmp_flow);
    if (iRet != 0)
    {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblCmpFlow DBS_INSERT error, %d.[%s]", iRet, tbl_cmp_flow.plat_key);
            return -1;
    }
/*更新网关流水对账状态*/
    memcpy(pht_tbl_gateway_txn->flag_result, flag_result,  BT_FLAG_RESULT_LEN);
    iRet=DbsTblGatewayTxn(DBS_UPDATE, pht_tbl_gateway_txn, 0, 0);
    if (iRet)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblGatewayTxn DBS_UPDATE sqlcode[%d][%s]", iRet, pht_tbl_gateway_txn->plat_key);
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
    return 0;
}

/*************************************************
*函数名称∶ Total_0701
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_0701()
{
    int		nTotalNum = 0;
    int         iRet=0;
    ht_tbl_gateway_txn_def tbl_gateway_txn;

    memset(&tbl_gateway_txn, 0x00, sizeof(tbl_gateway_txn));
    memcpy(tbl_gateway_txn.sett_date, dbtbl_date_inf.stoday, sizeof(tbl_gateway_txn.sett_date)-1);
    memcpy(tbl_gateway_txn.inter_brh_code, ext_inter_brh_code, sizeof(tbl_gateway_txn.inter_brh_code)-1);
    memcpy(tbl_gateway_txn.flag_result, BT_FLAG_RESULT_NULL, BT_FLAG_RESULT_LEN);
    memcpy(tbl_gateway_txn.trans_state, BT_TRANS_STATE_SUCC, BT_TRANS_STATE_LEN);
    memcpy(tbl_gateway_txn.revsal_flag, BT_REVSAL_FLAG_N, BT_REVSAL_FLAG_LEN);
    memcpy(tbl_gateway_txn.cancel_flag, BT_CANCEL_FLAG_N, BT_CANCEL_FLAG_LEN);
    iRet = DbsTblGatewayTxn(DBS_SELECT1, &tbl_gateway_txn, 0, 0);
    if (iRet != 0)
    {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblHostTxn DBS_SELECT1 error, %d.", iRet);
            return -1;
    }
    nTotalNum = tbl_gateway_txn.seq_num;
    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "nTotalNum =[%d].", nTotalNum);

    return nTotalNum;
}

/*************************************************
*函数名称∶ Task_0701
*函数功能∶
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_0701 ( int nBeginOffset, int nEndOffset )
{
    int		iRet = 0, nTotCount=0;
    int nEptFlag;
    int iPanTag = 1;
    int iAmtTag = 1;
    char sFl_Flag_Pan[1 + 1];
    char sFl_Flag_Amt[1 + 1];
    ht_tbl_host_txn_def ht_tbl_host_txn;
    ht_tbl_gateway_txn_def ht_tbl_gateway_txn;

    memset(sFl_Flag_Pan, 0, sizeof(sFl_Flag_Pan));
    memcpy(sFl_Flag_Pan, getenv("FL_FLAG_PAN"), 1);
    if(strlen(sFl_Flag_Pan) == 0)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "can't get FL_FLAG_PAN");
        return -1;
    }

    memset(sFl_Flag_Amt, 0, sizeof(sFl_Flag_Amt));
    strcpy(sFl_Flag_Amt, getenv("FL_FLAG_AMT"));
    if(strlen(sFl_Flag_Amt) == 0)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "can't get FL_FLAG_AMT");
        return -1;
    }

    memset(&ht_tbl_gateway_txn, 0x00, sizeof(ht_tbl_gateway_txn));
    memcpy(ht_tbl_gateway_txn.sett_date, dbtbl_date_inf.stoday, sizeof(ht_tbl_gateway_txn.sett_date)-1);
    memcpy(ht_tbl_gateway_txn.inter_brh_code, ext_inter_brh_code, sizeof(ht_tbl_gateway_txn.inter_brh_code)-1);
    memcpy(ht_tbl_gateway_txn.flag_result, BT_FLAG_RESULT_NULL, BT_FLAG_RESULT_LEN);
    memcpy(ht_tbl_gateway_txn.trans_state, BT_TRANS_STATE_SUCC, BT_TRANS_STATE_LEN);
    memcpy(ht_tbl_gateway_txn.revsal_flag, BT_REVSAL_FLAG_N, BT_REVSAL_FLAG_LEN);
    memcpy(ht_tbl_gateway_txn.cancel_flag, BT_CANCEL_FLAG_N, BT_CANCEL_FLAG_LEN);
    DbsTblGatewayTxn(DBS_CURSOR, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
    iRet = DbsTblGatewayTxn(DBS_OPEN, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
    if (iRet != 0)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblGatewayTxn DBS_OPEN error, %d.", iRet);
        return -1;
    }

    while (1)
    {
        iAmtTag = 1;
        iPanTag = 1;
        nEptFlag = 0;
        memset(&ht_tbl_gateway_txn, 0, sizeof(ht_tbl_gateway_txn));
        iRet = DbsTblGatewayTxn(DBS_FETCH, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
        if (iRet)
        {
            if (iRet == DBS_NOTFOUND)
            {
                break;
            }

            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblGatewayTxn fetch sqlcode[%d]", iRet);
            DbsTblGatewayTxn(DBS_CLOSE, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
            return -1;
        }
        RightTrim(ht_tbl_gateway_txn.inter_brh_code);
        RightTrim(ht_tbl_gateway_txn.plat_key);
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "-----------------plat_key[%s]trans_tp[%s]---------", ht_tbl_gateway_txn.plat_key, ht_tbl_gateway_txn.trans_tp);
        nTotCount++;
        
        memset(&ht_tbl_host_txn, 0x00, sizeof(ht_tbl_host_txn));
        memcpy(ht_tbl_host_txn.plat_key, ht_tbl_gateway_txn.plat_key, sizeof(ht_tbl_host_txn.plat_key)-1);
        memcpy(ht_tbl_host_txn.inter_brh_code, ht_tbl_gateway_txn.inter_brh_code, sizeof(ht_tbl_host_txn.inter_brh_code)-1);
        RightTrim(ht_tbl_host_txn.plat_key);
        RightTrim(ht_tbl_host_txn.inter_brh_code);
        iRet = DbsTblHostTxn(DBS_SELECT3, &ht_tbl_host_txn, 0, 0);
        if (iRet)
        {
            if (iRet == DBS_NOTFOUND)
            {
                /*单边账*/
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "网关有,单边账");
                iRet =  in_err_flow(BT_FLAG_RESULT_CORENULL, &ht_tbl_host_txn, &ht_tbl_gateway_txn);
                if (iRet)
                {
                    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_err_flow err sqlcode[%d]", iRet);
                    DbsTblGatewayTxn(DBS_CLOSE, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                    return -1;
                }
                continue;
            }

            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblHostTxn DBS_SELECT3 sqlcode[%d]", iRet);
            DbsTblGatewayTxn(DBS_CLOSE, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
            return -1;
        }
        RightTrim(ht_tbl_host_txn.plat_key);
        RightTrim(ht_tbl_host_txn.inter_brh_code);

        /*核对核心交易状态*/
        if(memcmp(ht_tbl_host_txn.trans_state, BT_TRANS_STATE_FAIL, BT_TRANS_STATE_LEN) == 0)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "支付网关交易成功，核心交易失败");
            iRet =  in_err_flow(BT_FLAG_RESULT_COREFAIL, &ht_tbl_host_txn, &ht_tbl_gateway_txn);
            if (iRet)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_err_flow err sqlcode[%d]", iRet);
                DbsTblGatewayTxn(DBS_CLOSE, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                return -1;
            }
            continue;            
        }

        /*核对核心冲正标志*/
        if(memcmp(ht_tbl_host_txn.revsal_flag, BT_REVSAL_FLAG_Y, BT_REVSAL_FLAG_LEN) == 0)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "支付网关交易成功，核心交易已被冲正");
            iRet =  in_err_flow(BT_FLAG_RESULT_COREFAIL, &ht_tbl_host_txn, &ht_tbl_gateway_txn);
            if (iRet)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_err_flow err sqlcode[%d]", iRet);
                DbsTblGatewayTxn(DBS_CLOSE, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                return -1;
            }
            continue;            
        }

        /*核对核心撤销标志*/
        if(memcmp(ht_tbl_host_txn.cancel_flag, BT_CANCEL_FLAG_Y, BT_CANCEL_FLAG_LEN) == 0)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "支付网关交易成功，核心交易已被撤销");
            iRet =  in_err_flow(BT_FLAG_RESULT_COREFAIL, &ht_tbl_host_txn, &ht_tbl_gateway_txn);
            if (iRet)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_err_flow err sqlcode[%d]", iRet);
                DbsTblGatewayTxn(DBS_CLOSE, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                return -1;
            }
            continue;            
        }
         
        /* 核对卡号 */
        /*网关送的是统一平台的虚拟卡号，与虚拟核心的卡号不一致,不核对卡号*/

        /* 核对金额 */
        if(sFl_Flag_Amt[0] == 'Y')
        {
             if(abs(atof(ht_tbl_gateway_txn.trans_at) -atof(ht_tbl_host_txn.trans_at))<=0.000001)
            {
                 iAmtTag = 1;
            }
            else
            {
                HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Amt not match: gateway[%13.2f], HOST[%13.2f]",
                           ht_tbl_gateway_txn.plat_key, atof(ht_tbl_gateway_txn.trans_at), atof(ht_tbl_host_txn.trans_at));
                iAmtTag = 0;
            }
        }
        /* 对平 */
        if (iAmtTag == 1 && iPanTag == 1)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "对平");
            iRet = in_cmp_flow(BT_FLAG_RESULT_CMP, &ht_tbl_host_txn, &ht_tbl_gateway_txn);
            if (iRet != 0)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_cmp_flow err sqlcode[%d]", iRet);
                DbsTblGatewayTxn(DBS_CLOSE, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                return -1;
            }
        }else{
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "要素不平");
            iRet =  in_err_flow(BT_FLAG_RESULT_NOTMATC, &ht_tbl_host_txn, &ht_tbl_gateway_txn);
            if (iRet)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_err_flow err sqlcode[%d]", iRet);
                DbsTblGatewayTxn(DBS_CLOSE, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                return -1;
            }
        }
    }
    DbsTblGatewayTxn(DBS_CLOSE, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
    
    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "total num [%d].", nTotCount);
    HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_0701 Succ .");
    return 0;
}
/****************************** end of Task_0701.c ******************************/

