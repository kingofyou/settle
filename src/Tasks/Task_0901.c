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
 *  文 件 名: Task_0901.c
 *  功    能: 核心多处理
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
static int in_err_flow(char *flag_result, ht_tbl_host_txn_def *pht_tbl_host_txn)
{
    ht_tbl_err_flow_def tbl_err_flow;
    int iRet = 0;
    
    memset(&tbl_err_flow, 0x00, sizeof(tbl_err_flow));
    memcpy(tbl_err_flow.inter_brh_code,  ext_inter_brh_code, sizeof(tbl_err_flow.inter_brh_code)-1);      
    memcpy(tbl_err_flow.sett_date	,    dbtbl_date_inf.stoday,  sizeof(tbl_err_flow.sett_date)-1);
    memcpy(tbl_err_flow.trans_date	,    pht_tbl_host_txn->trans_date,  sizeof(tbl_err_flow.trans_date)-1);
    memcpy(tbl_err_flow.trans_time	,     pht_tbl_host_txn->trans_time,  sizeof(tbl_err_flow.trans_time)-1);
    memcpy(tbl_err_flow.flag_result	,  flag_result,  BT_FLAG_RESULT_LEN);
    memcpy(tbl_err_flow.order_no	,     pht_tbl_host_txn->order_no,  sizeof(tbl_err_flow.order_no)-1);
    memcpy(tbl_err_flow.plat_key	,     pht_tbl_host_txn->plat_key,  sizeof(tbl_err_flow.plat_key)-1);
    memcpy(tbl_err_flow.batch_no	,    pht_tbl_host_txn->batch_no,  sizeof(tbl_err_flow.batch_no)-1);
    memcpy(tbl_err_flow.bus_tp	,      pht_tbl_host_txn->bus_tp,  sizeof(tbl_err_flow.bus_tp)-1);
    memcpy(tbl_err_flow.trans_tp	,   pht_tbl_host_txn->trans_tp,  sizeof(tbl_err_flow.trans_tp)-1);
    memcpy(tbl_err_flow.acct_type	,    pht_tbl_host_txn->acct_type,  sizeof(tbl_err_flow.acct_type)-1);
    memcpy(tbl_err_flow.trans_state	,    pht_tbl_host_txn->trans_state,  sizeof(tbl_err_flow.trans_state)-1);
    memcpy(tbl_err_flow.pan	,   pht_tbl_host_txn->pan,  sizeof(tbl_err_flow.pan)-1);
    memcpy(tbl_err_flow.ccy	,      pht_tbl_host_txn->ccy,  sizeof(tbl_err_flow.ccy)-1);
    memcpy(tbl_err_flow.trans_at	,   pht_tbl_host_txn->trans_at,  sizeof(tbl_err_flow.trans_at)-1);
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
    return 0;
}

/*************************************************
*函数名称∶ Total_0901
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_0901()
{
    int		nTotalNum = 0;
    int         iRet=0;
    ht_tbl_host_txn_def ht_tbl_host_txn;

    memset(&ht_tbl_host_txn, 0x00, sizeof(ht_tbl_host_txn));
    memcpy(ht_tbl_host_txn.sett_date, dbtbl_date_inf.stoday, sizeof(ht_tbl_host_txn.sett_date)-1);
    memcpy(ht_tbl_host_txn.inter_brh_code, ext_inter_brh_code, sizeof(ht_tbl_host_txn.inter_brh_code)-1);
    memcpy(ht_tbl_host_txn.flag_result, BT_FLAG_RESULT_NULL, BT_FLAG_RESULT_LEN);
    memcpy(ht_tbl_host_txn.trans_state, BT_TRANS_STATE_SUCC, BT_TRANS_STATE_LEN);
    memcpy(ht_tbl_host_txn.revsal_flag, BT_REVSAL_FLAG_N, BT_REVSAL_FLAG_LEN);
    memcpy(ht_tbl_host_txn.cancel_flag, BT_CANCEL_FLAG_N, BT_CANCEL_FLAG_LEN);
    iRet = DbsTblHostTxn(DBS_SELECT1, &ht_tbl_host_txn, 0, 0);
    if (iRet != 0)
    {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblHostTxn DBS_SELECT1 error, %d.", iRet);
            return -1;
    }
    nTotalNum = ht_tbl_host_txn.seq_num;
    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "nTotalNum =[%d].", nTotalNum);

    return nTotalNum;
}

int waitCheckEnd()
{
	int	nReturn;

    tbl_mission_inf_def      dbtbl_mission_inf;
    while(1)
    {
        /*等待支付宝渠道对账完成完成*/
        memset(&dbtbl_mission_inf, 0x00, sizeof(dbtbl_mission_inf));
        memcpy(dbtbl_mission_inf.inter_brh_code, BT_BRH_ID_ALIPAY, BT_BRH_ID_LEN);
        memcpy(dbtbl_mission_inf.settlmt_date, dbtbl_date_inf.stoday, BT_DATE_LEN);
        memcpy(dbtbl_mission_inf.mission_index, "0901", BT_MISSION_INDEX_LEN);
        nReturn = DbsTblmissionInf(DBS_SELECT4, &dbtbl_mission_inf);
        if (nReturn && nReturn != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblmissionInf error[%d]", nReturn);
            return -1;
        }
        else if (nReturn == DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "not found! settlmt_date[%s],task9031 excuting, please hold 30's.", dbtbl_mission_inf.settlmt_date);
            sleep(2);
            continue;
        }

        if (dbtbl_mission_inf.mission_status == BT_MISSION_STATUS_SUSS)
        {
            ;
        }
        else if (dbtbl_mission_inf.mission_status == BT_MISSION_STATUS_FAIL)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DbsTblmissionInf error, please check.");
            return -1;
        }
        else
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "settlmt_date[%s],task9031 excuting, please hold 30's.", dbtbl_mission_inf.settlmt_date);
            sleep(2);
            continue;
        }
        
        /*等待微信渠道对账完成*/
        memset(&dbtbl_mission_inf, 0x00, sizeof(dbtbl_mission_inf));
        memcpy(dbtbl_mission_inf.inter_brh_code, BT_BRH_ID_WECHAT, BT_BRH_ID_LEN);
        memcpy(dbtbl_mission_inf.settlmt_date, dbtbl_date_inf.stoday, BT_DATE_LEN);
        memcpy(dbtbl_mission_inf.mission_index, "0901", BT_MISSION_INDEX_LEN);
        nReturn = DbsTblmissionInf(DBS_SELECT4, &dbtbl_mission_inf);
        if (nReturn && nReturn != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblmissionInf error[%d]", nReturn);
            return -1;
        }
        else if (nReturn == DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "not found! settlmt_date[%s],task9031 excuting, please hold 30's.", dbtbl_mission_inf.settlmt_date);
            sleep(2);
            continue;
        }

        if (dbtbl_mission_inf.mission_status == BT_MISSION_STATUS_SUSS)
        {
            ;
        }
        else if (dbtbl_mission_inf.mission_status == BT_MISSION_STATUS_FAIL)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DbsTblmissionInf error, please check.");
            return -1;
        }
        else
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "settlmt_date[%s],task9031 excuting, please hold 30's.", dbtbl_mission_inf.settlmt_date);
            sleep(2);
            continue;
        }
        
        /*等待快捷渠道对账完成*/
        memset(&dbtbl_mission_inf, 0x00, sizeof(dbtbl_mission_inf));
        memcpy(dbtbl_mission_inf.inter_brh_code, BT_BRH_ID_QUICK, BT_BRH_ID_LEN);
        memcpy(dbtbl_mission_inf.settlmt_date, dbtbl_date_inf.stoday, BT_DATE_LEN);
        memcpy(dbtbl_mission_inf.mission_index, "0901", BT_MISSION_INDEX_LEN);
        nReturn = DbsTblmissionInf(DBS_SELECT4, &dbtbl_mission_inf);
        if (nReturn && nReturn != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblmissionInf error[%d]", nReturn);
            return -1;
        }
        else if (nReturn == DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "not found! settlmt_date[%s],task9031 excuting, please hold 30's.", dbtbl_mission_inf.settlmt_date);
            sleep(2);
            continue;
        }

        if (dbtbl_mission_inf.mission_status == BT_MISSION_STATUS_SUSS)
        {
            ;
        }
        else if (dbtbl_mission_inf.mission_status == BT_MISSION_STATUS_FAIL)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DbsTblmissionInf error, please check.");
            return -1;
        }
        else
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "settlmt_date[%s],task9031 excuting, please hold 30's.", dbtbl_mission_inf.settlmt_date);
            sleep(2);
            continue;
        }
        
        /*等待网银渠道对账完成*/
        memset(&dbtbl_mission_inf, 0x00, sizeof(dbtbl_mission_inf));
        memcpy(dbtbl_mission_inf.inter_brh_code, BT_BRH_ID_GATEWAY, BT_BRH_ID_LEN);
        memcpy(dbtbl_mission_inf.settlmt_date, dbtbl_date_inf.stoday, BT_DATE_LEN);
        memcpy(dbtbl_mission_inf.mission_index, "0901", BT_MISSION_INDEX_LEN);
        nReturn = DbsTblmissionInf(DBS_SELECT4, &dbtbl_mission_inf);
        if (nReturn && nReturn != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "select task status error[%d]", nReturn);
            return -1;
        }
        else if (nReturn == DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "not found! settlmt_date[%s],task9031 excuting, please hold 30's.", dbtbl_mission_inf.settlmt_date);
            sleep(2);
            continue;
        }

        if (dbtbl_mission_inf.mission_status == BT_MISSION_STATUS_SUSS)
        {
            break;
        }
        else if (dbtbl_mission_inf.mission_status == BT_MISSION_STATUS_FAIL)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "task excuted error, please check.");
            return -1;
        }
        else
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "settlmt_date[%s],task9031 excuting, please hold 30's.", dbtbl_mission_inf.settlmt_date);
            sleep(2);
            continue;
        }

		/*等待富友渠道对账完成*/
        memset(&dbtbl_mission_inf, 0x00, sizeof(dbtbl_mission_inf));
        memcpy(dbtbl_mission_inf.inter_brh_code, BT_BRH_ID_FUIOU, BT_BRH_ID_LEN);
        memcpy(dbtbl_mission_inf.settlmt_date, dbtbl_date_inf.stoday, BT_DATE_LEN);
        memcpy(dbtbl_mission_inf.mission_index, "0900", BT_MISSION_INDEX_LEN);
        nReturn = DbsTblmissionInf(DBS_SELECT4, &dbtbl_mission_inf);
        if (nReturn && nReturn != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblmissionInf error[%d]", nReturn);
            return -1;
        }
        else if (nReturn == DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "not found! settlmt_date[%s],task9031 excuting, please hold 30's.", dbtbl_mission_inf.settlmt_date);
            sleep(2);
            continue;
        }

        if (dbtbl_mission_inf.mission_status == BT_MISSION_STATUS_SUSS)
        {
            ;
        }
        else if (dbtbl_mission_inf.mission_status == BT_MISSION_STATUS_FAIL)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DbsTblmissionInf error, please check.");
            return -1;
        }
        else
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "settlmt_date[%s],task9031 excuting, please hold 30's.", dbtbl_mission_inf.settlmt_date);
            sleep(2);
            continue;
        }
    }
}

/*************************************************
*函数名称∶ Task_0901
*函数功能∶
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_0901 ( int nBeginOffset, int nEndOffset )
{
    int		iRet = 0, nTotCount=0;
    ht_tbl_host_txn_def ht_tbl_host_txn;
	ht_tbl_err_flow_def tbl_err_flow;

	iRet =  waitCheckEnd();
	if(iRet) return -1;

    memset(&ht_tbl_host_txn, 0x00, sizeof(ht_tbl_host_txn));
    memcpy(ht_tbl_host_txn.sett_date, dbtbl_date_inf.stoday, sizeof(ht_tbl_host_txn.sett_date)-1);
    memcpy(ht_tbl_host_txn.inter_brh_code, ext_inter_brh_code, sizeof(ht_tbl_host_txn.inter_brh_code)-1);
    memcpy(ht_tbl_host_txn.flag_result, BT_FLAG_RESULT_NULL, BT_FLAG_RESULT_LEN);
    memcpy(ht_tbl_host_txn.trans_state, BT_TRANS_STATE_SUCC, BT_TRANS_STATE_LEN);
    memcpy(ht_tbl_host_txn.revsal_flag, BT_REVSAL_FLAG_N, BT_REVSAL_FLAG_LEN);
    memcpy(ht_tbl_host_txn.cancel_flag, BT_CANCEL_FLAG_N, BT_CANCEL_FLAG_LEN);
    DbsTblHostTxn(DBS_CURSOR1, &ht_tbl_host_txn, nBeginOffset, nEndOffset);
    iRet = DbsTblHostTxn(DBS_OPEN1, &ht_tbl_host_txn, nBeginOffset, nEndOffset);
    if (iRet != 0)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblHostTxn DBS_OPEN1 error, %d.", iRet);
        return -1;
    }

    while (1)
    {
        memset(&ht_tbl_host_txn, 0, sizeof(ht_tbl_host_txn));
        iRet = DbsTblHostTxn(DBS_FETCH1, &ht_tbl_host_txn, nBeginOffset, nEndOffset);
        if (iRet)
        {
            if (iRet == DBS_NOTFOUND)
            {
                break;
            }

            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblHostTxn fetch1 sqlcode[%d]", iRet);
            DbsTblHostTxn(DBS_CLOSE1, &ht_tbl_host_txn, nBeginOffset, nEndOffset);
            return -1;
        }
        RightTrim(ht_tbl_host_txn.inter_brh_code);
        RightTrim(ht_tbl_host_txn.plat_key);
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "-----------------plat_key[%s]--------------------", ht_tbl_host_txn.plat_key);
        nTotCount++;
        if(memcmp(ht_tbl_host_txn.trans_tp, BT_TRANS_TP_CHARGE, BT_TRANS_TP_LEN) == 0)
        {
            /*充值交易 T+2对不平才登记对账不平表*/
            
			if(memcmp(ht_tbl_host_txn.sett_date, dbtbl_date_inf.stoday, BT_DATE_LEN) == 0)
            {
                iRet = in_err_flow(BT_FLAG_RESULT_CORE_ORMORE, &ht_tbl_host_txn);
                if (iRet)
                {
                    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_err_flow err[%d]", iRet);
                    DbsTblHostTxn(DBS_CLOSE1, &ht_tbl_host_txn, nBeginOffset, nEndOffset);
                    return -1;
                }   
            }
			else if(memcmp(ht_tbl_host_txn.sett_date, dbtbl_date_inf.stoday, BT_DATE_LEN) < 0)
            {
				memset(&tbl_err_flow, 0, sizeof(tbl_err_flow));
				memcpy(tbl_err_flow.inter_brh_code, ext_inter_brh_code, sizeof(tbl_err_flow.inter_brh_code)-1);
				memcpy(tbl_err_flow.sett_date, dbtbl_date_inf.syesterday, sizeof(tbl_err_flow.sett_date)-1);
				memcpy(tbl_err_flow.plat_key, ht_tbl_host_txn.plat_key, sizeof(tbl_err_flow.plat_key)-1);
				memcpy(tbl_err_flow.flag_result, BT_FLAG_RESULT_CORE, BT_FLAG_RESULT_LEN);
				iRet = DbsTblErrFlow(DBS_UPDATE, &tbl_err_flow);
				if (iRet)
                {
                      HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblErrFlow DBS_UPDATE sqlcode[%d]", iRet);
					  DbsTblHostTxn(DBS_CLOSE1, &ht_tbl_host_txn, nBeginOffset, nEndOffset);
                      return -1;
                } 
            }
        }
        else
        {
            iRet = in_err_flow(BT_FLAG_RESULT_CORE, &ht_tbl_host_txn);
            if (iRet)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_err_flow err[%d]", iRet);
                DbsTblHostTxn(DBS_CLOSE1, &ht_tbl_host_txn, nBeginOffset, nEndOffset);
                return -1;
            }   
        }
    }
    DbsTblHostTxn(DBS_CLOSE1, &ht_tbl_host_txn, nBeginOffset, nEndOffset);
    
    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "total num [%d].", nTotCount);
    HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_0901 Succ .");
    return 0;
}
/****************************** end of Task_0901.c ******************************/

