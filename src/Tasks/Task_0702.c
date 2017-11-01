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
 *  �� �� ��: Task_0702.c
 *  ��    ��: ��ҵ�����������׶���
 *  �����Ա: wyl
 *  ����ʱ��: 2017-3-28
 *  ��    ע:
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
*�������ơ� in_err_flow
*�������ܡò������
*���������
*���������
*�������ء� 1
***************************************************/
static int in_err_flow(char *flag_result, ht_tbl_cib_txn_def *pht_tbl_cib_txn, ht_tbl_host_txn_def *pht_tbl_host_txn, ht_tbl_gateway_txn_def *pht_tbl_gateway_txn, int flag)
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
    memcpy(tbl_err_flow.pan1	,   pht_tbl_gateway_txn->pan1,  sizeof(tbl_err_flow.pan1)-1);
    memcpy(tbl_err_flow.ccy	,      pht_tbl_gateway_txn->ccy,  sizeof(tbl_err_flow.ccy)-1);
    memcpy(tbl_err_flow.trans_at	,   pht_tbl_gateway_txn->trans_at,  sizeof(tbl_err_flow.trans_at)-1);
    if(flag == 1)
    {
        memcpy(tbl_err_flow.dest_pan	,     pht_tbl_cib_txn->pan,  sizeof(tbl_err_flow.dest_pan)-1);
        memcpy(tbl_err_flow.dest_ccy	,      pht_tbl_cib_txn->ccy,  sizeof(tbl_err_flow.dest_ccy)-1);
        memcpy(tbl_err_flow.dest_trans_at,   pht_tbl_cib_txn->orderamt,  sizeof(tbl_err_flow.dest_trans_at)-1);
    }
    else 
    {
        memcpy(tbl_err_flow.dest_pan	,     pht_tbl_host_txn->pan,  sizeof(tbl_err_flow.dest_pan)-1);
        memcpy(tbl_err_flow.dest_ccy	,      pht_tbl_host_txn->ccy,  sizeof(tbl_err_flow.dest_ccy)-1);
        memcpy(tbl_err_flow.dest_trans_at,   pht_tbl_host_txn->trans_at,  sizeof(tbl_err_flow.dest_trans_at)-1);
    }
    
    RightTrim(pht_tbl_cib_txn->orderamt);
    RightTrim(pht_tbl_cib_txn->feeamt);
    if(strlen(pht_tbl_cib_txn->orderamt) != 0)
    {
        memcpy(tbl_err_flow.trans_fee,   pht_tbl_cib_txn->feeamt,  sizeof(tbl_err_flow.trans_fee)-1);
        sprintf(tbl_err_flow.debt_at,"%12.0lf", floor(atof(pht_tbl_cib_txn->orderamt) - atof(pht_tbl_cib_txn->feeamt) + 0.50001));
    }        
    iRet = DbsTblErrFlow(DBS_INSERT, &tbl_err_flow);
    if (iRet != 0)
    {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblErrFlow DBS_INSERT error, %d.", iRet);
            return -1;
    }

	// ��������Ƶ������ٲ����¶�����ˮ��
    if(!memcmp("B", flag_result, 1)) return 0;

    /*����������ˮ����״̬*/
    memcpy(pht_tbl_gateway_txn->flag_result, flag_result,  BT_FLAG_RESULT_LEN);
    iRet=DbsTblGatewayTxn(DBS_UPDATE, pht_tbl_gateway_txn, 0, 0);
    if (iRet && iRet != DBS_NOTFOUND)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblGatewayTxn DBS_UPDATE sqlcode[%d]", iRet);
        return -1;
    }
    
    /*���º�����ˮ����״̬*/
    memcpy(pht_tbl_host_txn->flag_result, flag_result,  BT_FLAG_RESULT_LEN);
    iRet = DbsTblHostTxn(DBS_UPDATE, pht_tbl_host_txn, 0, 0);
    if (iRet && iRet != DBS_NOTFOUND)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblHostTxn DBS_UPDATE sqlcode[%d]", iRet);
        return -1;
    }

    /*������ҵ������ˮ����״̬*/
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
*�������ơ� in_cmp_flow
*�������ܡò�����˽����
*���������
*���������
*�������ء� 1
***************************************************/
static int in_cmp_flow(char *flag_result, ht_tbl_cib_txn_def *pht_tbl_cib_txn, ht_tbl_host_txn_def *pht_tbl_host_txn, ht_tbl_gateway_txn_def *pht_tbl_gateway_txn)
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
    memcpy(tbl_cmp_flow.pan	,     pht_tbl_gateway_txn->pan,  sizeof(tbl_cmp_flow.pan)-1);
    
    memcpy(tbl_cmp_flow.pan1	,     pht_tbl_host_txn->pan,  sizeof(tbl_cmp_flow.pan1)-1);
    
    memcpy(tbl_cmp_flow.acct_name	,     pht_tbl_gateway_txn->acct_name,  sizeof(tbl_cmp_flow.acct_name)-1);
    memcpy(tbl_cmp_flow.ccy	,      pht_tbl_gateway_txn->ccy,  sizeof(tbl_cmp_flow.ccy)-1);
    memcpy(tbl_cmp_flow.trans_at,pht_tbl_gateway_txn->trans_at,  sizeof(tbl_cmp_flow.trans_at)-1);
    memcpy(tbl_cmp_flow.trans_at2, pht_tbl_cib_txn->feeamt,  sizeof(tbl_cmp_flow.trans_at2)-1);
    memcpy(tbl_cmp_flow.trans_at3, pht_tbl_gateway_txn->trans_at3,  sizeof(tbl_cmp_flow.trans_at3)-1);
    memcpy(tbl_cmp_flow.trans_at4,pht_tbl_gateway_txn->trans_at4,  sizeof(tbl_cmp_flow.trans_at4)-1);
    memcpy(tbl_cmp_flow.trans_at5, pht_tbl_gateway_txn->trans_at5,  sizeof(tbl_cmp_flow.trans_at5)-1);
    memcpy(tbl_cmp_flow.ext_mcht_no, pht_tbl_gateway_txn->ext_mcht_no,  sizeof(tbl_cmp_flow.ext_mcht_no)-1);
    
    /*��ֵ�����̻�*/
    memcpy(tbl_cmp_flow.sett_tp1,	    BT_SETT_TP_SETT,  BT_SETT_TP_LEN);
    memcpy(tbl_cmp_flow.mcht_no1,	    pht_tbl_gateway_txn->mcht_no1,  sizeof(tbl_cmp_flow.mcht_no1)-1);
    
    /*��ֵ�����̻�*/
    memcpy(tbl_cmp_flow.sett_tp2,	    BT_SETT_TP_EXT, BT_SETT_TP_LEN);
    memcpy(tbl_cmp_flow.mcht_no2,       pht_tbl_gateway_txn->ext_mcht_no,  sizeof(tbl_cmp_flow.mcht_no2)-1);

    memcpy(tbl_cmp_flow.acct_name,	    pht_tbl_gateway_txn->acct_name,  sizeof(tbl_cmp_flow.acct_name)-1);
    CommonRTrim(tbl_cmp_flow.acct_name);

    strcpy(tbl_cmp_flow.sett_tp2,	" ");
    strcpy(tbl_cmp_flow.mcht_no2,	" ");
    strcpy(tbl_cmp_flow.sett_tp3,	" ");
    strcpy(tbl_cmp_flow.mcht_no3,	" ");
    strcpy(tbl_cmp_flow.sett_tp4,	" ");
    strcpy(tbl_cmp_flow.mcht_no4,	" ");
    strcpy(tbl_cmp_flow.sett_tp5,	" ");
    strcpy(tbl_cmp_flow.mcht_no5,	" ");
    strcpy(tbl_cmp_flow.sett_tp6,	" ");
    strcpy(tbl_cmp_flow.mcht_no6,	" ");
    strcpy(tbl_cmp_flow.sett_tp7,	" ");
    strcpy(tbl_cmp_flow.mcht_no7,	" ");
    strcpy(tbl_cmp_flow.sett_tp8,	" ");
    strcpy(tbl_cmp_flow.mcht_no8,	" ");
    strcpy(tbl_cmp_flow.sett_tp9,	" ");
    strcpy(tbl_cmp_flow.mcht_no9,	" "); 
    
    iRet = DbsTblCmpFlow(DBS_INSERT, &tbl_cmp_flow);
    if (iRet != 0)
    {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblCmpFlow DBS_INSERT error, %d.", iRet);
            return -1;
    }
    
    /*����������ˮ����״̬*/
    memcpy(pht_tbl_gateway_txn->flag_result, flag_result,  BT_FLAG_RESULT_LEN);
    iRet=DbsTblGatewayTxn(DBS_UPDATE, pht_tbl_gateway_txn, 0, 0);
    if (iRet)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblGatewayTxn DBS_UPDATE sqlcode[%d]", iRet);
        return -1;
    }
    
    /*���º�����ˮ����״̬*/
    memcpy(pht_tbl_host_txn->flag_result, flag_result,  BT_FLAG_RESULT_LEN);
    iRet = DbsTblHostTxn(DBS_UPDATE, pht_tbl_host_txn, 0, 0);
    if (iRet && iRet != DBS_NOTFOUND)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblHostTxn DBS_UPDATE sqlcode[%d]", iRet);
        return -1;
    }
    
    /*������ҵ������ˮ����״̬*/
    memcpy(pht_tbl_cib_txn->flag_result, flag_result,  BT_FLAG_RESULT_LEN);
    iRet = DbsTblCIBTxn(DBS_UPDATE, pht_tbl_cib_txn, 0, 0);
    if (iRet && iRet != DBS_NOTFOUND)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblCIBTxn DBS_UPDATE sqlcode[%d]", iRet);
        return -1;
    }
    return 0;
}

static update_account_tab(char *flag_result, ht_tbl_cib_txn_def *pht_tbl_cib_txn, ht_tbl_host_txn_def *pht_tbl_host_txn, ht_tbl_gateway_txn_def *pht_tbl_gateway_txn)
{
	int iRet = 0;
    /*����������ˮ����״̬*/
    memcpy(pht_tbl_gateway_txn->flag_result, flag_result,  BT_FLAG_RESULT_LEN);
    iRet=DbsTblGatewayTxn(DBS_UPDATE, pht_tbl_gateway_txn, 0, 0);
    if (iRet && iRet != DBS_NOTFOUND)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblGatewayTxn DBS_UPDATE sqlcode[%d]", iRet);
        return -1;
    }
    
    /*���º�����ˮ����״̬*/
    memcpy(pht_tbl_host_txn->flag_result, flag_result,  BT_FLAG_RESULT_LEN);
    iRet = DbsTblHostTxn(DBS_UPDATE, pht_tbl_host_txn, 0, 0);
    if (iRet && iRet != DBS_NOTFOUND)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblHostTxn DBS_UPDATE sqlcode[%d]", iRet);
        return -1;
    }

    /*������ҵ��ˮ����״̬*/
    memcpy(pht_tbl_cib_txn->flag_result, flag_result, BT_FLAG_RESULT_LEN);
    iRet = DbsTblCIBTxn(DBS_UPDATE, pht_tbl_cib_txn, 0, 0);
    if (iRet && iRet != DBS_NOTFOUND)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblCIBTxn DBS_UPDATE sqlcode[%d]", iRet);
        return -1;
    }

}

/*************************************************
*�������ơ� Total_0702
*�������ܡ�
*���������
*���������
*�������ء� 1
***************************************************/
int Total_0702()
{
    int		nTotalNum = 0;
    int         iRet=0;
    ht_tbl_gateway_txn_def ht_tbl_gateway_txn;

    memset(&ht_tbl_gateway_txn, 0x00, sizeof(ht_tbl_gateway_txn));
    memcpy(ht_tbl_gateway_txn.sett_date, dbtbl_date_inf.stoday, sizeof(ht_tbl_gateway_txn.sett_date)-1);
    memcpy(ht_tbl_gateway_txn.inter_brh_code, ext_inter_brh_code, sizeof(ht_tbl_gateway_txn.inter_brh_code)-1);
    memcpy(ht_tbl_gateway_txn.flag_result, BT_FLAG_RESULT_NULL, BT_FLAG_RESULT_LEN);
    memcpy(ht_tbl_gateway_txn.trans_state, BT_TRANS_STATE_SUCC, BT_TRANS_STATE_LEN);
    memcpy(ht_tbl_gateway_txn.revsal_flag, BT_REVSAL_FLAG_N, BT_REVSAL_FLAG_LEN);
    memcpy(ht_tbl_gateway_txn.cancel_flag, BT_CANCEL_FLAG_N, BT_CANCEL_FLAG_LEN);
    iRet = DbsTblGatewayTxn(DBS_SELECT1, &ht_tbl_gateway_txn, 0, 0);
    if (iRet != 0)
    {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblgatewayTxn DBS_SELECT1 error, %d.", iRet);
            return -1;
    }
    nTotalNum = ht_tbl_gateway_txn.seq_num;
    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "nTotalNum =[%d].", nTotalNum);

    return nTotalNum;
}

/*************************************************
*�������ơ� Task_0702
*�������ܡ�
*��������� nBeginOffset��nEndOffset
*���������
*�������ء� 0  -- �ɹ�
            -1 -- ʧ��
***************************************************/
int Task_0702 ( int nBeginOffset, int nEndOffset )
{
    int	 iRet = 0, nTotCount=0;
    int iPanTag = 1;
    int iAmtTag = 1;
    int iCheckTag = 0;          //���˱�־��0-�������ˣ�1-��������
    char sFl_Flag_Amt[1 + 1];
    char sFl_Flag_Pan[1 + 1];
    ht_tbl_cib_txn_def ht_tbl_cib_txn;
    ht_tbl_gateway_txn_def ht_tbl_gateway_txn;
    ht_tbl_gateway_txn_def ht_tbl_gateway_txn_org;
    ht_tbl_host_txn_def ht_tbl_host_txn;
    ht_tbl_err_flow_def tbl_err_flow;
    
	memset(&tbl_err_flow, 0, sizeof(tbl_err_flow));
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
    
    /*�ѹ����˿�ף����浥��������ֹԭ֧������δ�����������Ȱ��˿�״�����*/
    DbsTblGatewayTxn(DBS_CURSOR2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
    iRet = DbsTblGatewayTxn(DBS_OPEN2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
    if (iRet != 0)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblGatewayTxn DBS_OPEN2 error, %d.", iRet);
        return -1;
    }

    while (1)
    {
		memset(&tbl_err_flow, 0, sizeof(tbl_err_flow));
        memset(&ht_tbl_gateway_txn, 0, sizeof(ht_tbl_gateway_txn));
        iRet = DbsTblGatewayTxn(DBS_FETCH2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
        if (iRet)
        {
            if (iRet == DBS_NOTFOUND)
            {
                break;
            }

            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblGatewayTxn fetch sqlcode[%d]", iRet);
            DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
            return -1;
        }
        RightTrim(ht_tbl_gateway_txn.plat_key);
        RightTrim(ht_tbl_gateway_txn.inter_brh_code);
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "-----------------plat_key[%s][%s]--------------------", ht_tbl_gateway_txn.plat_key, ht_tbl_gateway_txn.trans_tp);
        nTotCount++;
        
        if(memcmp(ht_tbl_gateway_txn.trans_tp, BT_TRANS_TP_CHARGE, BT_TRANS_TP_LEN) == 0)
        {
            /*��������*/
            iCheckTag = 1;
        }
        else
        {
            iCheckTag = 0;
        }

        memset(&ht_tbl_cib_txn, 0x00, sizeof(ht_tbl_cib_txn));
        memcpy(ht_tbl_cib_txn.inter_brh_code, ht_tbl_gateway_txn.inter_brh_code, sizeof(ht_tbl_cib_txn.inter_brh_code)-1);
        memcpy(ht_tbl_cib_txn.flag_result, BT_FLAG_RESULT_NULL, BT_FLAG_RESULT_LEN);

        memcpy(ht_tbl_cib_txn.plat_key, ht_tbl_gateway_txn.plat_key, sizeof(ht_tbl_cib_txn.plat_key)-1);
        
        /*���˿���ⶼ�������ļ���*/
        if(memcmp(ht_tbl_gateway_txn.bus_tp, BT_BUS_TP_CIB5, BT_BUS_TP_LEN) == 0)
        {
            /*���֧�����˽���*/
            memcpy(ht_tbl_cib_txn.trans_tp, BT_TRANS_TP_QUICK_IN, BT_TRANS_TP_LEN);
        }
        else
        {
            /*����֧�����˽���*/
            memcpy(ht_tbl_cib_txn.trans_tp, BT_TRANS_TP_GATEWAY_IN, BT_TRANS_TP_LEN);
        }

        RightTrim(ht_tbl_cib_txn.inter_brh_code);
        RightTrim(ht_tbl_cib_txn.plat_key);
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "ht_tbl_cib_txn.trans_tp[%s].ht_tbl_cib_txn.bus_tp[%s]", ht_tbl_cib_txn.trans_tp, ht_tbl_cib_txn.bus_tp);
        
        iRet = DbsTblCIBTxn(DBS_SELECT3, &ht_tbl_cib_txn, nBeginOffset, nEndOffset);
        if (iRet)
        {
            if (iRet == DBS_NOTFOUND)
            {
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "ht_tbl_gateway_txn.sett_date[%s]dbtbl_date_inf.stoday[%s]", ht_tbl_gateway_txn.sett_date, dbtbl_date_inf.stoday);
                if(iCheckTag == 0)
                {
                    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "֧�����ض࣬��T2���ˣ�T+1������˲�ƽ��״̬Ϊ���Ʋ�ƽ");
                    // ֧�����ض࣬��T2���ˣ�T+1������˲�ƽ��״̬Ϊ���Ʋ�ƽ
					if(!memcmp(ht_tbl_gateway_txn.sett_date, dbtbl_date_inf.stoday, BT_DATE_LEN))
                    {
						/*��������-������������*/
                        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "��������-������������");
                        memset(&ht_tbl_host_txn, 0, sizeof(ht_tbl_host_txn));
                        iRet = in_err_flow(BT_FLAG_RESULT_PART_ORLESS, &ht_tbl_cib_txn, &ht_tbl_host_txn, &ht_tbl_gateway_txn, 1);
                        if (iRet)
                        {
                            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_err_flow err sqlcode[%d]", iRet);
                            DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                            return -1;
                        }
					}

					/*��������-֧�����ض�*/
                    /*T+2�Բ�ƽ�ŵǼǶ��˲�ƽ��*/
                    if(memcmp(ht_tbl_gateway_txn.sett_date, dbtbl_date_inf.stoday, BT_DATE_LEN) < 0)
                    {
                        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "��������-֧�����ض�,��ҵ������");
						// ��������-�������٣����¶��˲�ƽ��HT_TBL_ERR_FLOW
						// ����HT_TBL_GATEWAY_TXN
						memcpy(tbl_err_flow.inter_brh_code, ext_inter_brh_code, sizeof(tbl_err_flow.inter_brh_code)-1);
						memcpy(tbl_err_flow.sett_date, dbtbl_date_inf.syesterday, sizeof(tbl_err_flow.sett_date)-1);
						memcpy(tbl_err_flow.plat_key, ht_tbl_gateway_txn.plat_key, sizeof(tbl_err_flow.plat_key)-1);
						memcpy(tbl_err_flow.flag_result, BT_FLAG_RESULT_PART_NULL, BT_FLAG_RESULT_LEN);
						iRet = DbsTblErrFlow(DBS_UPDATE, &tbl_err_flow);
						if (iRet)
                        {
                            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblErrFlow DBS_UPDATE sqlcode[%d]", iRet);
					        DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                            return -1;
                        }

						/*����������ˮ����״̬*/
                        memcpy(ht_tbl_gateway_txn.flag_result, BT_FLAG_RESULT_PART_NULL,  BT_FLAG_RESULT_LEN);
                        iRet=DbsTblGatewayTxn(DBS_UPDATE, &ht_tbl_gateway_txn, 0, 0);
                        if (iRet)
                        {
                            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblGatewayTxn DBS_UPDATE sqlcode[%d]", iRet);
						    DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                            return -1;
                        }
                    }
                    continue;
                }
                
                memset(&ht_tbl_host_txn, 0, sizeof(ht_tbl_host_txn));
                memcpy(ht_tbl_host_txn.plat_key, ht_tbl_gateway_txn.host_key, sizeof(ht_tbl_host_txn.plat_key)-1);
                RightTrim(ht_tbl_host_txn.plat_key);
                memcpy(ht_tbl_host_txn.inter_brh_code, ht_tbl_gateway_txn.inter_brh_code, sizeof(ht_tbl_host_txn.inter_brh_code)-1);
                iRet = DbsTblHostTxn(DBS_SELECT3, &ht_tbl_host_txn, 0, 0);
                if (iRet)
                {
                    if (iRet == DBS_NOTFOUND)
                    {
                        /*�������ˣ�֧�����ض�,��ҵ�����ޣ����������*/
                        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "֧�����ض�");
                        iRet =  in_err_flow(BT_FLAG_RESULT_GATEWAY, &ht_tbl_cib_txn, &ht_tbl_host_txn, &ht_tbl_gateway_txn, 1);
                        if (iRet)
                        {
                            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_err_flow err sqlcode[%d]", iRet);
                            DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                            return -1;
                        }
                        continue;
                    }
                    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblHostTxn DBS_SELECT3 err sqlcode[%d]", iRet);
                    DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                    return -1;
                }
                RightTrim(ht_tbl_host_txn.inter_brh_code);
                RightTrim(ht_tbl_host_txn.plat_key);
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "֧�����ض࣬��T2����, T+1������˲�ƽ��״̬Ϊ���Ʋ�ƽ");    
                // T+1������˲�ƽ��״̬Ϊ���Ʋ�ƽ
				if(!memcmp(ht_tbl_gateway_txn.sett_date, dbtbl_date_inf.stoday, BT_DATE_LEN))
                {
                    /*��������-������������*/
                    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "�������ˣ�������������");
                    iRet = in_err_flow(BT_FLAG_RESULT_PART_ORLESS, &ht_tbl_cib_txn, &ht_tbl_host_txn, &ht_tbl_gateway_txn, 2);
                    if (iRet)
                    {
                        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_err_flow err sqlcode[%d]", iRet);
                        DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                        return -1;
                    }
                }

                /*T+2�Բ�ƽ���¶��˲�ƽ��*/
                if(memcmp(ht_tbl_gateway_txn.sett_date, dbtbl_date_inf.stoday, BT_DATE_LEN) < 0)
                {
					// ��������-�������٣����¶��˲�ƽ��HT_TBL_ERR_FLOW
					// ����HT_TBL_GATEWAY_TXN��HT_TBL_HOST_TXN
                    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "��������-��ҵ��");
				    memcpy(tbl_err_flow.inter_brh_code, ext_inter_brh_code, sizeof(tbl_err_flow.inter_brh_code)-1);
					memcpy(tbl_err_flow.sett_date, dbtbl_date_inf.syesterday, sizeof(tbl_err_flow.sett_date)-1);
					memcpy(tbl_err_flow.plat_key, ht_tbl_gateway_txn.plat_key, sizeof(tbl_err_flow.plat_key)-1);
					memcpy(tbl_err_flow.flag_result, BT_FLAG_RESULT_PART_NULL, BT_FLAG_RESULT_LEN);
					iRet = DbsTblErrFlow(DBS_UPDATE, &tbl_err_flow);
					if (iRet)
                    {
                        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblErrFlow DBS_UPDATE sqlcode[%d]", iRet);
					    DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                        return -1;
                    }
                    /*����������ˮ����״̬*/
                    memcpy(ht_tbl_gateway_txn.flag_result, BT_FLAG_RESULT_PART_NULL,  BT_FLAG_RESULT_LEN);
                    iRet=DbsTblGatewayTxn(DBS_UPDATE, &ht_tbl_gateway_txn, 0, 0);
                    if (iRet && iRet != DBS_NOTFOUND)
                    {
                        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblGatewayTxn DBS_UPDATE sqlcode[%d]", iRet);
			            DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                        return -1;
                    }

					/*���º�����ˮ����״̬*/
                    memcpy(ht_tbl_host_txn.flag_result, BT_FLAG_RESULT_PART_NULL,  BT_FLAG_RESULT_LEN);
                    iRet = DbsTblHostTxn(DBS_UPDATE, &ht_tbl_host_txn, 0, 0);
                    if (iRet && iRet != DBS_NOTFOUND)
                    {
                        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblHostTxn DBS_UPDATE sqlcode[%d]", iRet);
					    DbsTblGatewayTxn(DBS_CLOSE, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                        return -1;
                    } 
                }
                continue;
            }

            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblGatewayTxn DBS_SELECT3 sqlcode[%d]", iRet);
            DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
            return -1;
        }
        RightTrim(ht_tbl_cib_txn.inter_brh_code);
        RightTrim(ht_tbl_cib_txn.plat_key);
        memcpy(ht_tbl_cib_txn.inter_brh_code, ht_tbl_gateway_txn.inter_brh_code, sizeof(ht_tbl_cib_txn.inter_brh_code)-1);

        /* �˶Խ�� */
        if(sFl_Flag_Amt[0] == 'Y')
        {
            if(abs(atof(ht_tbl_gateway_txn.trans_at) -atof(ht_tbl_cib_txn.orderamt))<=0.000001)
            {
                iAmtTag = 1;
            }
            else
            {
                HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Amt not match: gateway[%13.2f], Alipay[%13.2f]",
                                ht_tbl_gateway_txn.plat_key, atof(ht_tbl_gateway_txn.trans_at), atof(ht_tbl_cib_txn.orderamt));
                iAmtTag = 0;
            }
        }

        /* ��ƽ */
        if (iAmtTag == 1 && iPanTag == 1)
        {
            if(iCheckTag == 0)
            {
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "˫�����ˣ���ƽ");
				// �������Ƶ������ٵĲ�ƽ����Ϊȷ��
				memcpy(tbl_err_flow.inter_brh_code, ext_inter_brh_code, sizeof(tbl_err_flow.inter_brh_code)-1);
			    memcpy(tbl_err_flow.sett_date, dbtbl_date_inf.syesterday, sizeof(tbl_err_flow.sett_date)-1);
			    memcpy(tbl_err_flow.plat_key, ht_tbl_gateway_txn.plat_key, sizeof(tbl_err_flow.plat_key)-1);
				memcpy(tbl_err_flow.flag_result, BT_FLAG_RESULT_PART_NOTORLESS, BT_FLAG_RESULT_LEN);
				iRet = DbsTblErrFlow(DBS_UPDATE, &tbl_err_flow);
				if (iRet && iRet != DBS_NOTFOUND)
                {
                    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblErrFlow DBS_UPDATE sqlcode[%d]", iRet);
					DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                    return -1;
                }

                memset(&ht_tbl_host_txn, 0 , sizeof(ht_tbl_host_txn));
                iRet = in_cmp_flow(BT_FLAG_RESULT_CMP, &ht_tbl_cib_txn,  &ht_tbl_host_txn, &ht_tbl_gateway_txn);
                if (iRet != 0)
                {
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_cmp_flow err sqlcode[%d]", iRet);
                    DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                    return -1;
                }
                continue;
            }
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "��������֧�����غ���ҵ���ж�ƽ��������������Ķ���");
        }
        else
        {            
            if(iCheckTag == 0)
            {
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "��������Ҫ�ز�ƽ");
                memset(&ht_tbl_host_txn, 0, sizeof(ht_tbl_host_txn));
            }
            else
            {
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "��������Ҫ�ز�ƽ");
                memset(&ht_tbl_host_txn, 0, sizeof(ht_tbl_host_txn));
                memcpy(ht_tbl_host_txn.plat_key, ht_tbl_gateway_txn.plat_key, sizeof(ht_tbl_host_txn.plat_key)-1);
                RightTrim(ht_tbl_host_txn.plat_key);
            }    
			
			// �������Ƶ������ٵĲ�ƽ����״̬ΪBT_FLAG_RESULT_NOTMATC
			memcpy(tbl_err_flow.inter_brh_code, ext_inter_brh_code, sizeof(tbl_err_flow.inter_brh_code)-1);
			memcpy(tbl_err_flow.sett_date, dbtbl_date_inf.syesterday, sizeof(tbl_err_flow.sett_date)-1);
			memcpy(tbl_err_flow.plat_key, ht_tbl_gateway_txn.plat_key, sizeof(tbl_err_flow.plat_key)-1);
		    memcpy(tbl_err_flow.flag_result, BT_FLAG_RESULT_NOTMATC, BT_FLAG_RESULT_LEN);
			iRet = DbsTblErrFlow(DBS_UPDATE, &tbl_err_flow);
			if (iRet && iRet != DBS_NOTFOUND)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblErrFlow DBS_UPDATE sqlcode[%d]", iRet);
			    DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                return -1;
            }
			else if(!iRet) {
                // ���±�HT_TBL_HOST_TXN��HT_TBL_GATEWAY_TXN��HT_TBL_CIB_TXN
				iRet = update_account_tab(BT_FLAG_RESULT_NOTMATC, &ht_tbl_cib_txn, &ht_tbl_host_txn, &ht_tbl_gateway_txn);
				if(iRet)
                {
                    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "update HT_TBL_HOST_TXN��HT_TBL_GATEWAY_TXN��ht_tbl_cib_txn err sqlcode[%d]", iRet);
                    DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                    return -1;
                }
				
				continue;
			}

            iRet =  in_err_flow(BT_FLAG_RESULT_NOTMATC, &ht_tbl_cib_txn, &ht_tbl_host_txn, &ht_tbl_gateway_txn, 1);
            if (iRet)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_err_flow err sqlcode[%d]", iRet);
                DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                return -1;
            }
            continue;
        }

        memset(&ht_tbl_host_txn, 0x00, sizeof(ht_tbl_host_txn));
        memcpy(ht_tbl_host_txn.plat_key, ht_tbl_gateway_txn.plat_key, sizeof(ht_tbl_host_txn.plat_key)-1);
        memcpy(ht_tbl_host_txn.inter_brh_code, ht_tbl_gateway_txn.inter_brh_code, sizeof(ht_tbl_host_txn.inter_brh_code)-1);
        RightTrim(ht_tbl_host_txn.inter_brh_code);
        RightTrim(ht_tbl_host_txn.plat_key);
        iRet = DbsTblHostTxn(DBS_SELECT3, &ht_tbl_host_txn, 0, 0);
        if (iRet)
        {
            if (iRet == DBS_NOTFOUND)
            {
                /*��������,��������ޣ���ҵ�����У�֧��������*/
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "���������");
                iRet =  in_err_flow(BT_FLAG_RESULT_CORENULL, &ht_tbl_cib_txn, &ht_tbl_host_txn, &ht_tbl_gateway_txn, 2);
                if (iRet)
                {
                    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_err_flow1 err sqlcode[%d]", iRet);
                    DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                    return -1;
                }
                continue;
            }

            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblHostTxn DBS_SELECT3 sqlcode[%d]", iRet);
            DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
            return -1;
        }
        RightTrim(ht_tbl_host_txn.inter_brh_code);
        RightTrim(ht_tbl_host_txn.plat_key);
        /*�˶Ժ��Ľ���״̬*/
        if(memcmp(ht_tbl_host_txn.trans_state, BT_TRANS_STATE_FAIL, BT_TRANS_STATE_LEN) == 0)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "֧�����ؽ��׳ɹ������Ľ���ʧ��");
			// �������Ƶ������ٵĲ�ƽ����״̬ΪBT_FLAG_RESULT_COREFAIL
			memcpy(tbl_err_flow.inter_brh_code, ext_inter_brh_code, sizeof(tbl_err_flow.inter_brh_code)-1);
			memcpy(tbl_err_flow.sett_date, dbtbl_date_inf.syesterday, sizeof(tbl_err_flow.sett_date)-1);
			memcpy(tbl_err_flow.plat_key, ht_tbl_gateway_txn.plat_key, sizeof(tbl_err_flow.plat_key)-1);
		    memcpy(tbl_err_flow.flag_result, BT_FLAG_RESULT_COREFAIL, BT_FLAG_RESULT_LEN);
			iRet = DbsTblErrFlow(DBS_UPDATE, &tbl_err_flow);
			if (iRet && iRet != DBS_NOTFOUND)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblErrFlow DBS_UPDATE sqlcode[%d]", iRet);
			    DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                return -1;
            }
			else if(!iRet) {
                // ���±�HT_TBL_HOST_TXN��HT_TBL_GATEWAY_TXN��ht_tbl_cib_txn
				iRet = update_account_tab(BT_FLAG_RESULT_COREFAIL, &ht_tbl_cib_txn, &ht_tbl_host_txn, &ht_tbl_gateway_txn);
				if(iRet)
                {
                    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "update HT_TBL_HOST_TXN��HT_TBL_GATEWAY_TXN��ht_tbl_cib_txn err sqlcode[%d]", iRet);
                    DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                    return -1;
                }
				
				continue;
			}

            iRet =  in_err_flow(BT_FLAG_RESULT_COREFAIL, &ht_tbl_cib_txn, &ht_tbl_host_txn, &ht_tbl_gateway_txn, 1);
            if (iRet)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_err_flow err sqlcode[%d]", iRet);
                DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                return -1;
            }
            continue;            
        }

        /*�˶Ժ��ĳ�����־*/
        if(memcmp(ht_tbl_host_txn.revsal_flag, BT_REVSAL_FLAG_Y, BT_REVSAL_FLAG_LEN) == 0)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "֧�����ؽ��׳ɹ������Ľ����ѱ�����");
			// �������Ƶ������ٵĲ�ƽ����״̬ΪBT_FLAG_RESULT_COREFAIL
			memcpy(tbl_err_flow.inter_brh_code, ext_inter_brh_code, sizeof(tbl_err_flow.inter_brh_code)-1);
			memcpy(tbl_err_flow.sett_date, dbtbl_date_inf.syesterday, sizeof(tbl_err_flow.sett_date)-1);
			memcpy(tbl_err_flow.plat_key, ht_tbl_gateway_txn.plat_key, sizeof(tbl_err_flow.plat_key)-1);
		    memcpy(tbl_err_flow.flag_result, BT_FLAG_RESULT_COREFAIL, BT_FLAG_RESULT_LEN);
			iRet = DbsTblErrFlow(DBS_UPDATE, &tbl_err_flow);
			if (iRet && iRet != DBS_NOTFOUND)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblErrFlow DBS_UPDATE sqlcode[%d]", iRet);
			    DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                return -1;
            }
			else if(!iRet) {
                // ���±�HT_TBL_HOST_TXN��HT_TBL_GATEWAY_TXN��ht_tbl_cib_txn
				iRet = update_account_tab(BT_FLAG_RESULT_COREFAIL, &ht_tbl_cib_txn, &ht_tbl_host_txn, &ht_tbl_gateway_txn);
				if(iRet)
                {
                    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "update HT_TBL_HOST_TXN��HT_TBL_GATEWAY_TXN��ht_tbl_cib_txn err sqlcode[%d]", iRet);
                    DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                    return -1;
                }
				
				continue;
			}

            iRet =  in_err_flow(BT_FLAG_RESULT_COREFAIL, &ht_tbl_cib_txn, &ht_tbl_host_txn, &ht_tbl_gateway_txn, 1);
            if (iRet)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_err_flow err sqlcode[%d]", iRet);
                DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                return -1;
            }
            continue;            
        }

        /*�˶Ժ��ĳ�����־*/
        if(memcmp(ht_tbl_host_txn.cancel_flag, BT_CANCEL_FLAG_Y, BT_CANCEL_FLAG_LEN) == 0)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "֧�����ؽ��׳ɹ������Ľ����ѱ�����");
			// �������Ƶ������ٵĲ�ƽ����״̬ΪBT_FLAG_RESULT_COREFAIL
			memcpy(tbl_err_flow.inter_brh_code, ext_inter_brh_code, sizeof(tbl_err_flow.inter_brh_code)-1);
			memcpy(tbl_err_flow.sett_date, dbtbl_date_inf.syesterday, sizeof(tbl_err_flow.sett_date)-1);
			memcpy(tbl_err_flow.plat_key, ht_tbl_gateway_txn.plat_key, sizeof(tbl_err_flow.plat_key)-1);
		    memcpy(tbl_err_flow.flag_result, BT_FLAG_RESULT_COREFAIL, BT_FLAG_RESULT_LEN);
			iRet = DbsTblErrFlow(DBS_UPDATE, &tbl_err_flow);
			if (iRet && iRet != DBS_NOTFOUND)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblErrFlow DBS_UPDATE sqlcode[%d]", iRet);
			    DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                return -1;
            }
			else if(!iRet) {
                // ���±�HT_TBL_HOST_TXN��HT_TBL_GATEWAY_TXN��ht_tbl_cib_txn
				iRet = update_account_tab(BT_FLAG_RESULT_COREFAIL, &ht_tbl_cib_txn, &ht_tbl_host_txn, &ht_tbl_gateway_txn);
				if(iRet)
                {
                    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "update HT_TBL_HOST_TXN��HT_TBL_GATEWAY_TXN��ht_tbl_cib_txn err sqlcode[%d]", iRet);
                    DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                    return -1;
                }
				
				continue;
			}

            iRet =  in_err_flow(BT_FLAG_RESULT_COREFAIL, &ht_tbl_cib_txn, &ht_tbl_host_txn, &ht_tbl_gateway_txn, 1);
            if (iRet)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_err_flow err sqlcode[%d]", iRet);
                DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                return -1;
            }
            continue;            
        }

        /* �˶Կ��� */
        /*��ֵ������֧������û�������׸����ţ����˶�*/
        if (sFl_Flag_Pan[0] == 'Y' && memcmp(ht_tbl_gateway_txn.trans_tp, BT_TRANS_TP_CHARGE, BT_TRANS_TP_LEN) != 0 && memcmp(ht_tbl_gateway_txn.trans_tp, BT_TRANS_TP_WITHDRAW, BT_TRANS_TP_LEN) != 0)
        {
            iRet = CheckPanIfEqual(ht_tbl_gateway_txn.pan, ht_tbl_host_txn.pan);
            if (iRet)
            {
                HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Pan not match: gateway[%s], host[%s]",
                                                         ht_tbl_gateway_txn.pan, ht_tbl_host_txn.pan);
                iPanTag = 0;
            }
            else
            {
                iPanTag = 1;
            }
        }

        /* �˶Խ�� */
        if(sFl_Flag_Amt[0] == 'Y')
        {
            if(abs(atof(ht_tbl_gateway_txn.trans_at) -atof(ht_tbl_host_txn.trans_at))<=0.000001)
            {
                 iAmtTag = 1;
            }
            else
            {
                HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Amt not match: gateway[%13.2f], host[%13.2f]",
                           ht_tbl_gateway_txn.plat_key, atof(ht_tbl_gateway_txn.trans_at), atof(ht_tbl_host_txn.trans_at));
                iAmtTag = 0;
            }
        }

        /* ��ƽ */
        if (iAmtTag == 1 && iPanTag == 1)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "�������ˣ���ƽ");
			// �������Ƶ������ٵĲ�ƽ����Ϊȷ��
			memcpy(tbl_err_flow.inter_brh_code, ext_inter_brh_code, sizeof(tbl_err_flow.inter_brh_code)-1);
			memcpy(tbl_err_flow.sett_date, dbtbl_date_inf.syesterday, sizeof(tbl_err_flow.sett_date)-1);
			memcpy(tbl_err_flow.plat_key, ht_tbl_gateway_txn.plat_key, sizeof(tbl_err_flow.plat_key)-1);
		    memcpy(tbl_err_flow.flag_result, BT_FLAG_RESULT_PART_NOTORLESS, BT_FLAG_RESULT_LEN);
			iRet = DbsTblErrFlow(DBS_UPDATE, &tbl_err_flow);
			if (iRet && iRet != DBS_NOTFOUND)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblErrFlow DBS_UPDATE sqlcode[%d]", iRet);
			    DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                return -1;
            }

            iRet = in_cmp_flow(BT_FLAG_RESULT_CMP, &ht_tbl_cib_txn,  &ht_tbl_host_txn, &ht_tbl_gateway_txn);
            if (iRet != 0)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_cmp_flow err sqlcode[%d]", iRet);
                DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                return -1;
            }
        }
        else
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "�������ˣ�Ҫ�ز�ƽ");
			// �������Ƶ������ٵĲ�ƽ����״̬ΪBT_FLAG_RESULT_NOTMATC
			memcpy(tbl_err_flow.inter_brh_code, ext_inter_brh_code, sizeof(tbl_err_flow.inter_brh_code)-1);
			memcpy(tbl_err_flow.sett_date, dbtbl_date_inf.syesterday, sizeof(tbl_err_flow.sett_date)-1);
			memcpy(tbl_err_flow.plat_key, ht_tbl_gateway_txn.plat_key, sizeof(tbl_err_flow.plat_key)-1);
		    memcpy(tbl_err_flow.flag_result, BT_FLAG_RESULT_NOTMATC, BT_FLAG_RESULT_LEN);
			iRet = DbsTblErrFlow(DBS_UPDATE, &tbl_err_flow);
			if (iRet && iRet != DBS_NOTFOUND)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblErrFlow DBS_UPDATE sqlcode[%d]", iRet);
			    DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                return -1;
            }
			else if(!iRet) {
                // ���±�HT_TBL_HOST_TXN��HT_TBL_GATEWAY_TXN��ht_tbl_cib_txn
				iRet = update_account_tab(BT_FLAG_RESULT_NOTMATC, &ht_tbl_cib_txn, &ht_tbl_host_txn, &ht_tbl_gateway_txn);
				if(iRet)
                {
                    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "update HT_TBL_HOST_TXN��HT_TBL_GATEWAY_TXN��ht_tbl_cib_txn err sqlcode[%d]", iRet);
                    DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                    return -1;
                }
				
				continue;
			}

            iRet =  in_err_flow(BT_FLAG_RESULT_NOTMATC, &ht_tbl_cib_txn, &ht_tbl_host_txn, &ht_tbl_gateway_txn, 1);
            if (iRet)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_err_flow err sqlcode[%d]", iRet);
                DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                return -1;
            }
        }
    }
    DbsTblGatewayTxn(DBS_CLOSE2, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "nTotCount num [%d].", nTotCount);
    
    /*���������˿��----------------------------------------------------------------*/
    /*�����˿�ɹ�/ʧ�ܶ���Ϊ��������*/
    memset(&ht_tbl_gateway_txn, 0x00, sizeof(ht_tbl_gateway_txn));
    memcpy(ht_tbl_gateway_txn.sett_date, dbtbl_date_inf.stoday, sizeof(ht_tbl_gateway_txn.sett_date)-1);
    memcpy(ht_tbl_gateway_txn.inter_brh_code, ext_inter_brh_code, sizeof(ht_tbl_gateway_txn.inter_brh_code)-1);
    memcpy(ht_tbl_gateway_txn.flag_result, BT_FLAG_RESULT_NULL, BT_FLAG_RESULT_LEN); 
    memcpy(ht_tbl_gateway_txn.revsal_flag, BT_REVSAL_FLAG_N, BT_REVSAL_FLAG_LEN);
    memcpy(ht_tbl_gateway_txn.cancel_flag, BT_CANCEL_FLAG_N, BT_CANCEL_FLAG_LEN);   
    DbsTblGatewayTxn(DBS_CURSOR1, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
    iRet = DbsTblGatewayTxn(DBS_OPEN1, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
    if (iRet != 0)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblGatewayTxn DBS_OPEN error, %d.", iRet);
        return -1;
    }

    while (1)
    {
		memset(&tbl_err_flow, 0, sizeof(tbl_err_flow));
        memset(&ht_tbl_gateway_txn, 0, sizeof(ht_tbl_gateway_txn));
        iRet = DbsTblGatewayTxn(DBS_FETCH1, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
        if (iRet)
        {
            if (iRet == DBS_NOTFOUND)
            {
                break;
            }

            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblGatewayTxn fetch sqlcode[%d]", iRet);
            DbsTblGatewayTxn(DBS_CLOSE1, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
            return -1;
        }
        RightTrim(ht_tbl_gateway_txn.plat_key);
        RightTrim(ht_tbl_gateway_txn.inter_brh_code);
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "-----------------plat_key[%s][%s]--------------------", ht_tbl_gateway_txn.plat_key, ht_tbl_gateway_txn.trans_tp);
        nTotCount++;

        memset(&ht_tbl_cib_txn, 0x00, sizeof(ht_tbl_cib_txn));
        memcpy(ht_tbl_cib_txn.inter_brh_code, ht_tbl_gateway_txn.inter_brh_code, sizeof(ht_tbl_cib_txn.inter_brh_code)-1);
        memcpy(ht_tbl_cib_txn.flag_result, BT_FLAG_RESULT_NULL, BT_FLAG_RESULT_LEN);


        /*�˿�ף��ڲ�������+ԭ֧����ˮ�Ų�ѯ��ҵ������ˮ��*/
        memcpy(ht_tbl_cib_txn.plat_key, ht_tbl_gateway_txn.plat_key, sizeof(ht_tbl_cib_txn.plat_key)-1);
        
        /*Ŀǰֻ���˿���ڳ����ļ���*/
        if(memcmp(ht_tbl_gateway_txn.bus_tp, BT_BUS_TP_CIB5, BT_BUS_TP_LEN) == 0)
        {
            /*���֧�����˽���*/
            memcpy(ht_tbl_cib_txn.trans_tp, BT_TRANS_TP_QUICK_OUT, BT_TRANS_TP_LEN);
        }
        else
        {
            /*����֧�����˽���*/
            memcpy(ht_tbl_cib_txn.trans_tp, BT_TRANS_TP_GATEWAY_OUT, BT_TRANS_TP_LEN);
        }

        RightTrim(ht_tbl_cib_txn.inter_brh_code);
        RightTrim(ht_tbl_cib_txn.plat_key);
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "ht_tbl_cib_txn.trans_tp[%s].ht_tbl_cib_txn.bus_tp[%s]", ht_tbl_cib_txn.trans_tp, ht_tbl_cib_txn.bus_tp);
        
        iRet = DbsTblCIBTxn(DBS_SELECT3, &ht_tbl_cib_txn, nBeginOffset, nEndOffset);
        if (iRet)
        {
            if (iRet == DBS_NOTFOUND)
            {
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "ht_tbl_gateway_txn.sett_date[%s]dbtbl_date_inf.stoday[%s]", ht_tbl_gateway_txn.sett_date, dbtbl_date_inf.stoday);

                /*�ж��Ƿ��ǵ����˿��(��ҵ��������)*/
                memset(&ht_tbl_gateway_txn_org, 0, sizeof(ht_tbl_gateway_txn_org));
                memcpy(ht_tbl_gateway_txn_org.inter_brh_code, ht_tbl_gateway_txn.inter_brh_code, sizeof(ht_tbl_gateway_txn_org.inter_brh_code)-1);
                memcpy(ht_tbl_gateway_txn_org.plat_key, ht_tbl_gateway_txn.host_key, sizeof(ht_tbl_gateway_txn_org.plat_key)-1);
                RightTrim(ht_tbl_gateway_txn_org.inter_brh_code);
                RightTrim(ht_tbl_gateway_txn_org.plat_key);
                iRet = DbsTblGatewayTxn(DBS_SELECT, &ht_tbl_gateway_txn_org, nBeginOffset, nEndOffset);
                if (iRet && iRet != DBS_NOTFOUND)
                {
                    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblGatewayTxn DBS_SELECT sqlcode[%d]", iRet);
                    DbsTblGatewayTxn(DBS_CLOSE1, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                    return -1;
                }
                if(iRet == DBS_NOTFOUND)
                {
                    /*û���ҵ�ԭ֧�����ף���ҵ�����ޣ���T2����*/
                    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DbsTblGatewayTxn DBS_NOTFOUND sqlcode[%d][%s]", iRet, ht_tbl_gateway_txn_org.plat_key);
                }
                else
                {
                    
                    RightTrim(ht_tbl_gateway_txn_org.inter_brh_code);
                    RightTrim(ht_tbl_gateway_txn_org.plat_key);
                    /*�˿�ɹ�/ʧ��&&ԭ֧�����׳ɹ�&&ԭ֧������δ����/��������&&��ҵ������(����)&&��������ΪT+1*/
                    if((memcmp(ht_tbl_gateway_txn_org.flag_result, BT_FLAG_RESULT_NULL, BT_FLAG_RESULT_LEN) == 0 ||
                        memcmp(ht_tbl_gateway_txn_org.flag_result, BT_FLAG_RESULT_PART_NULL, BT_FLAG_RESULT_LEN) == 0) &&
                       memcmp(ht_tbl_gateway_txn_org.trans_state, BT_TRANS_STATE_SUCC, BT_TRANS_STATE_LEN) == 0  &&
                       memcmp(ht_tbl_gateway_txn_org.revsal_flag, BT_REVSAL_FLAG_N, BT_REVSAL_FLAG_LEN) == 0  &&
                       memcmp(ht_tbl_gateway_txn_org.cancel_flag, BT_CANCEL_FLAG_N, BT_CANCEL_FLAG_LEN) == 0 &&
                       memcmp(ht_tbl_gateway_txn_org.sett_date, dbtbl_date_inf.stoday, BT_DATE_LEN) == 0)
                    {
                        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "�˿�ɹ�&&ԭ֧�����׳ɹ�&&δ����&&��ҵ������(����)");
                        
                        /*�����˿�׵ı�־ΪT*/
                        memcpy(ht_tbl_gateway_txn.flag_result, BT_FLAG_RESULT_REFUND,  BT_FLAG_RESULT_LEN);
                        iRet=DbsTblGatewayTxn(DBS_UPDATE, &ht_tbl_gateway_txn, 0, 0);
                        if (iRet && iRet != DBS_NOTFOUND)
                        {
                            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblGatewayTxn DBS_UPDATE sqlcode[%d]", iRet);
                            return -1;
                        }
                        
                        /*����ԭ֧�����׵ı�־ΪT*/
                        memcpy(ht_tbl_gateway_txn_org.flag_result, BT_FLAG_RESULT_REFUND,  BT_FLAG_RESULT_LEN);
                        iRet=DbsTblGatewayTxn(DBS_UPDATE, &ht_tbl_gateway_txn_org, 0, 0);
                        if (iRet && iRet != DBS_NOTFOUND)
                        {
                            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblGatewayTxn DBS_UPDATE sqlcode[%d]", iRet);
                            return -1;
                        }
                        
                        /*ɾ��ԭ֧�����ײ�����ˮ*/
                        memset(&tbl_err_flow, 0, sizeof(tbl_err_flow));
                        memcpy(tbl_err_flow.inter_brh_code,  ext_inter_brh_code, sizeof(tbl_err_flow.inter_brh_code)-1);
                        memcpy(tbl_err_flow.plat_key	,     ht_tbl_gateway_txn_org.plat_key,  sizeof(tbl_err_flow.plat_key)-1);
                        RightTrim(tbl_err_flow.inter_brh_code);
                        RightTrim(tbl_err_flow.plat_key);
                        iRet=DbsTblErrFlow(DBS_DELETE, &tbl_err_flow, 0, 0);
                        if (iRet && iRet != DBS_NOTFOUND)
                        {
                            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblErrFlow DBS_DELETE sqlcode[%d]", iRet);
                            return -1;
                        }
                        
                        continue;
                    }
                    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "ԭ֧������״̬��Ϊ�������");
                }  

                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "֧�����ض࣬��T2����, T+1������˲�ƽ��״̬Ϊ���Ʋ�ƽ");
				// ֧�����ض࣬��T2����, T+1������˲�ƽ��״̬Ϊ���Ʋ�ƽ
				if(!memcmp(ht_tbl_gateway_txn.sett_date, dbtbl_date_inf.stoday, BT_DATE_LEN))
                {
                    /*��������-��ҵ������*/
                    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "��������-��ҵ������");
                    memset(&ht_tbl_host_txn, 0, sizeof(ht_tbl_host_txn));
                    iRet = in_err_flow(BT_FLAG_RESULT_PART_ORLESS, &ht_tbl_cib_txn, &ht_tbl_host_txn, &ht_tbl_gateway_txn, 1);
                    if (iRet)
                    {
                        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_err_flow err sqlcode[%d]", iRet);
                        DbsTblGatewayTxn(DBS_CLOSE1, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                        return -1;
                    }
                }
                /*��������-֧�����ض�*/
                /*T+2�Բ�ƽ���¶��˲�ƽ��*/
                if(memcmp(ht_tbl_gateway_txn.sett_date, dbtbl_date_inf.stoday, BT_DATE_LEN) < 0)
                {
                    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "��������-֧�����ض�,��ҵ������,ԭ֧��������");
					// ��������-����ͨ�٣����¶��˲�ƽ��HT_TBL_ERR_FLOW
					// ����HT_TBL_GATEWAY_TXN
					memcpy(tbl_err_flow.inter_brh_code, ext_inter_brh_code, sizeof(tbl_err_flow.inter_brh_code)-1);
					memcpy(tbl_err_flow.sett_date, dbtbl_date_inf.syesterday, sizeof(tbl_err_flow.sett_date)-1);
					memcpy(tbl_err_flow.plat_key, ht_tbl_gateway_txn.plat_key, sizeof(tbl_err_flow.plat_key)-1);
					memcpy(tbl_err_flow.flag_result, BT_FLAG_RESULT_GATEWAY, BT_FLAG_RESULT_LEN);
					iRet = DbsTblErrFlow(DBS_UPDATE, &tbl_err_flow);
					if (iRet)
                    {
                        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblErrFlow DBS_UPDATE sqlcode[%d]", iRet);
					    DbsTblGatewayTxn(DBS_CLOSE1, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                        return -1;
                    }
                    /*����������ˮ����״̬*/
                    memcpy(ht_tbl_gateway_txn.flag_result, BT_FLAG_RESULT_GATEWAY,  BT_FLAG_RESULT_LEN);
                    iRet=DbsTblGatewayTxn(DBS_UPDATE, &ht_tbl_gateway_txn, 0, 0);
                    if (iRet)
                    {
                        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblGatewayTxn DBS_UPDATE sqlcode[%d]", iRet);
					    DbsTblGatewayTxn(DBS_CLOSE1, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                        return -1;
                    }
                }
                continue;
            }

            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblGatewayTxn DBS_SELECT3 sqlcode[%d]", iRet);
            DbsTblGatewayTxn(DBS_CLOSE1, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
            return -1;
        }
        
        RightTrim(ht_tbl_cib_txn.inter_brh_code);
        RightTrim(ht_tbl_cib_txn.plat_key);
        memcpy(ht_tbl_cib_txn.inter_brh_code, ht_tbl_gateway_txn.inter_brh_code, sizeof(ht_tbl_cib_txn.inter_brh_code)-1);

        /*У�齻��״̬Ϊʧ��*/
        if(memcmp(ht_tbl_gateway_txn.trans_state, BT_TRANS_STATE_FAIL, BT_TRANS_STATE_LEN) == 0)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "����״̬ʧ��[%s]", ht_tbl_gateway_txn.trans_state);
			// �������Ƶ������ٵĲ�ƽ����Ϊȷ��
			memcpy(tbl_err_flow.inter_brh_code, ext_inter_brh_code, sizeof(tbl_err_flow.inter_brh_code)-1);
			memcpy(tbl_err_flow.sett_date, dbtbl_date_inf.syesterday, sizeof(tbl_err_flow.sett_date)-1);
			memcpy(tbl_err_flow.plat_key, ht_tbl_gateway_txn.plat_key, sizeof(tbl_err_flow.plat_key)-1);
		    memcpy(tbl_err_flow.flag_result, BT_FLAG_RESULT_GATEWAYFAIL, BT_FLAG_RESULT_LEN);
			iRet = DbsTblErrFlow(DBS_UPDATE, &tbl_err_flow);
			if (iRet && iRet != DBS_NOTFOUND)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblErrFlow DBS_UPDATE sqlcode[%d]", iRet);
			    DbsTblGatewayTxn(DBS_CLOSE1, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                return -1;
            }
			else if(!iRet) {
				// ���±�HT_TBL_HOST_TXN��HT_TBL_GATEWAY_TXN��ht_tbl_cib_txn
				iRet = update_account_tab(BT_FLAG_RESULT_GATEWAYFAIL, &ht_tbl_cib_txn, &ht_tbl_host_txn, &ht_tbl_gateway_txn);
				if(iRet)
                {
                    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "update HT_TBL_HOST_TXN��HT_TBL_GATEWAY_TXN��ht_tbl_cib_txn err sqlcode[%d]", iRet);
                    DbsTblGatewayTxn(DBS_CLOSE1, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                    return -1;
                }				
				continue;
			}

            memset(&ht_tbl_host_txn, 0, sizeof(ht_tbl_host_txn));
           
            iRet =  in_err_flow(BT_FLAG_RESULT_GATEWAYFAIL, &ht_tbl_cib_txn, &ht_tbl_host_txn, &ht_tbl_gateway_txn, 1);
            if (iRet)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_err_flow err sqlcode[%d]", iRet);
                DbsTblGatewayTxn(DBS_CLOSE1, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                return -1;
            }
            continue;
        }
        
        /* �˶Խ�� */
        if(sFl_Flag_Amt[0] == 'Y')
        {
            if(abs(atof(ht_tbl_gateway_txn.trans_at) -atof(ht_tbl_cib_txn.orderamt))<=0.000001)
            {
                iAmtTag = 1;
            }
            else
            {
                HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Amt not match: gateway[%13.2f], Alipay[%13.2f]",
                                ht_tbl_gateway_txn.plat_key, atof(ht_tbl_gateway_txn.trans_at), atof(ht_tbl_cib_txn.orderamt));
                iAmtTag = 0;
            }
        }

        /* ��ƽ */
        if (iAmtTag == 1 && iPanTag == 1)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "˫�����ˣ���ƽ");
			// �������Ƶ������ٵĲ�ƽ����Ϊȷ��
			memcpy(tbl_err_flow.inter_brh_code, ext_inter_brh_code, sizeof(tbl_err_flow.inter_brh_code)-1);
			memcpy(tbl_err_flow.sett_date, dbtbl_date_inf.syesterday, sizeof(tbl_err_flow.sett_date)-1);
			memcpy(tbl_err_flow.plat_key, ht_tbl_gateway_txn.plat_key, sizeof(tbl_err_flow.plat_key)-1);
		    memcpy(tbl_err_flow.flag_result, BT_FLAG_RESULT_PART_NOTORLESS, BT_FLAG_RESULT_LEN);
			iRet = DbsTblErrFlow(DBS_UPDATE, &tbl_err_flow);
			if (iRet && iRet != DBS_NOTFOUND)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblErrFlow DBS_UPDATE sqlcode[%d]", iRet);
		        DbsTblGatewayTxn(DBS_CLOSE1, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                return -1;
            }

            memset(&ht_tbl_host_txn, 0 , sizeof(ht_tbl_host_txn));
            iRet = in_cmp_flow(BT_FLAG_RESULT_CMP, &ht_tbl_cib_txn,  &ht_tbl_host_txn, &ht_tbl_gateway_txn);
            if (iRet != 0)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_cmp_flow err sqlcode[%d]", iRet);
                DbsTblGatewayTxn(DBS_CLOSE1, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                return -1;
            }
        }
        else
        {            

            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "��������Ҫ�ز�ƽ");
			// �������Ƶ������ٵĲ�ƽ����״̬ΪBT_FLAG_RESULT_NOTMATC
			memcpy(tbl_err_flow.inter_brh_code, ext_inter_brh_code, sizeof(tbl_err_flow.inter_brh_code)-1);
			memcpy(tbl_err_flow.sett_date, dbtbl_date_inf.syesterday, sizeof(tbl_err_flow.sett_date)-1);
			memcpy(tbl_err_flow.plat_key, ht_tbl_gateway_txn.plat_key, sizeof(tbl_err_flow.plat_key)-1);
		    memcpy(tbl_err_flow.flag_result, BT_FLAG_RESULT_NOTMATC, BT_FLAG_RESULT_LEN);
			iRet = DbsTblErrFlow(DBS_UPDATE, &tbl_err_flow);
			if (iRet && iRet != DBS_NOTFOUND)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblErrFlow DBS_UPDATE sqlcode[%d]", iRet);
			    DbsTblGatewayTxn(DBS_CLOSE1, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                return -1;
            }
			else if(!iRet) {
                // ���±�HT_TBL_HOST_TXN��HT_TBL_GATEWAY_TXN��ht_tbl_cib_txn
				iRet = update_account_tab(BT_FLAG_RESULT_NOTMATC, &ht_tbl_cib_txn, &ht_tbl_host_txn, &ht_tbl_gateway_txn);
				if(iRet)
                {
                    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "update HT_TBL_HOST_TXN��HT_TBL_GATEWAY_TXN��ht_tbl_cib_txn err sqlcode[%d]", iRet);
                    DbsTblGatewayTxn(DBS_CLOSE1, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                    return -1;
                }
				
				continue;
			}

            memset(&ht_tbl_host_txn, 0, sizeof(ht_tbl_host_txn));
           
            iRet =  in_err_flow(BT_FLAG_RESULT_NOTMATC, &ht_tbl_cib_txn, &ht_tbl_host_txn, &ht_tbl_gateway_txn, 1);
            if (iRet)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_err_flow err sqlcode[%d]", iRet);
                DbsTblGatewayTxn(DBS_CLOSE1, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);
                return -1;
            }
        }
    }
    DbsTblGatewayTxn(DBS_CLOSE1, &ht_tbl_gateway_txn, nBeginOffset, nEndOffset);

    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "total num [%d].", nTotCount);
    HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_0702 Succ .");
    return 0;
}
/****************************** end of Task_0702.c ******************************/

