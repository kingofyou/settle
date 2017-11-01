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
 *  文 件 名: Task_4101.c
 *  功    能: 清算
 *  编程人员: HELU
 *  开发时间: 2016-12-23
 *  备    注:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include "batch.h"
#include "settle.h"

extern char	gLogFile[LOG_NAME_LEN_MAX];
extern  tbl_date_inf_def dbtbl_date_inf;
extern	char ext_inter_brh_code[10 + 1];
static double pSettleStr[MAX_CACL_ITEM];     /*参数结构体*/
static int c_d_flag = TXN_DEBIT;     /*参数结构体*/
static char up_mcht_no[15+1];

static int InitData(ht_tbl_dat_flow_def *tbl_dat_flow);
static int SaveData(ht_tbl_dat_flow_def *ptbl_dat_flow, ht_tbl_sett_flow_def  *ptbl_sett_flow);
static int CaclOpr(int stepnum, ht_tbl_disc_algo_def *ptbl_disc_algos);
static void  CaclStep(double *result, double *operand1, char operator1, double *operand2, char operator2, double *operand3);

static int ExecCaclFee(char *palgo_id)
{
    int         iRet=0, stepnum=0;
    ht_tbl_disc_algo_def tbl_disc_algos[MAX_DISC_ALGO];
    ht_tbl_disc_algo_def tbl_disc_algo;
    memset(&tbl_disc_algos, 0x00, sizeof(tbl_disc_algos));
    memset(&tbl_disc_algo, 0x00, sizeof(tbl_disc_algo));
    memcpy(tbl_disc_algo.disc_id, palgo_id, sizeof(tbl_disc_algo.disc_id)-1);
    /*算法步骤表(HT_TBL_DISC_ALGO）*/
    DbsTblDiscAlgo(DBS_CURSOR, &tbl_disc_algo, 0, 0);
    iRet = DbsTblDiscAlgo(DBS_OPEN, &tbl_disc_algo, 0, 0);
    if(iRet)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblDiscAlgo DBS_OPEN error, %d.", iRet);
        return -1;
    }

    while (1)
    {
        memset(&tbl_disc_algo, 0, sizeof(tbl_disc_algo));
        iRet = DbsTblDiscAlgo(DBS_FETCH, &tbl_disc_algo, 0, 0);
        if (iRet)
        {
            if (iRet == DBS_NOTFOUND)
            {
                break;
            }

            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblDiscAlgo fetch sqlcode[%d]", iRet);
            DbsTblDiscAlgo(DBS_CLOSE, &tbl_disc_algo, 0, 0);
            return -1;
        }

        memcpy(&tbl_disc_algos[stepnum++], &tbl_disc_algo, sizeof(tbl_disc_algo));
    }
    DbsTblDiscAlgo(DBS_CLOSE, &tbl_disc_algo, 0, 0);
    iRet = CaclOpr(stepnum, tbl_disc_algos);
    if(iRet)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "CaclOpr error, %d.", iRet);
        return -1;
    }
    return 0;
}

static int CaclOpr(int stepnum, ht_tbl_disc_algo_def *ptbl_disc_algos)
{
    double     *p_result;
    double     *p_operand1;
    double     *p_operand2;
    double     *p_operand3;
    double     *pAlgoId;
    int        i,iRet;
    char       strAlgoID[5];
    for(i=0; i<stepnum; i++)
    {
        if(ptbl_disc_algos[i].operand1<0)
        {
            p_operand1 = pSettleStr - (int)ptbl_disc_algos[i].operand1 - 1;
        }
        else
        {
            p_operand1 = &ptbl_disc_algos[i].operand1;
        }

        if (ptbl_disc_algos[i].operand2 < 0)
        {
            p_operand2 = pSettleStr - (int)ptbl_disc_algos[i].operand2 - 1;
        }
        else
        {
            p_operand2 = &ptbl_disc_algos[i].operand2;
        }

        if (ptbl_disc_algos[i].operand3 < 0)
        {
            p_operand3 = pSettleStr - (int)ptbl_disc_algos[i].operand3 - 1;
        }
        else
        {
            p_operand3 = &ptbl_disc_algos[i].operand3;
        }
        p_result = pSettleStr - (int)ptbl_disc_algos[i].oper_rslt - 1;
        if (ptbl_disc_algos[i].operator1[0] == '$')
        {
            break;
        }

        if (ptbl_disc_algos[i].operator1[0] == '#')
        {
            memset(strAlgoID, 0, sizeof(strAlgoID));
            if (ptbl_disc_algos[i].oper_rslt < 0)
            {
                pAlgoId = pSettleStr - (int)ptbl_disc_algos[i].oper_rslt - 1;
                sprintf(strAlgoID, "%d", (int)*pAlgoId);
            }
            else
            {
                sprintf(strAlgoID, "%d", (int)ptbl_disc_algos[i].oper_rslt);
            }
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "algoid:[%s]", strAlgoID);
            if (0 < atoi(strAlgoID))
            {
                iRet = ExecCaclFee(strAlgoID);

                if (iRet)
                {
                    return -1;
                }
                else
                {
                    continue;
                }
            }
            continue;
        }

        if (ptbl_disc_algos[i].operator1[0] == 'L' && ptbl_disc_algos[i].operator2[0] == 'L')
        {
            if ((*p_operand1 <= *p_operand2) && (*p_operand2 <= *p_operand3))
            {
                i += ptbl_disc_algos[i].oper_rslt - 1;
            }
            continue;
        }
        else
        {
            CaclStep(p_result,
                     p_operand1,
                     ptbl_disc_algos[i].operator1[0],
                     p_operand2,
                     ptbl_disc_algos[i].operator2[0],
                     p_operand3);
        }
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "步骤[%d], 结果[%lf]. 操作数1[%lf] 操作数2[%lf]操作数3[%lf]操作符1[%s],操作符2[%s]", i, *p_result, *p_operand1, *p_operand2, *p_operand3, ptbl_disc_algos[i].operator1, ptbl_disc_algos[i].operator2);
    }
    return 0;
}

static int CaclFee(ht_tbl_dat_flow_def *ptbl_dat_flow)
{
    int         iRet=0;
    ht_tbl_algo_ctl_def tbl_algo_ctl;
    ht_tbl_txn_algo_rule_def tbl_txn_algo_rule;
    memset(&tbl_algo_ctl, 0x00, sizeof(tbl_algo_ctl));
    memcpy(tbl_algo_ctl.bus_tp, ptbl_dat_flow->bus_tp, sizeof(tbl_algo_ctl.bus_tp)-1);
    memcpy(tbl_algo_ctl.txn_num, ptbl_dat_flow->trans_tp, sizeof(tbl_algo_ctl.bus_tp)-1);
    /*清算控制表（HT_TBL_ALGO_CTL）*/
    iRet = DbsTblAlgoCtl(DBS_SELECT, &tbl_algo_ctl, 0, 0);
    if(iRet)
    {
        HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "bus_tp[%s],trans_tp[%s].", tbl_algo_ctl.bus_tp, tbl_algo_ctl.txn_num);
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAlgoCtl DBS_SELECT error, %d.", iRet);
        return -1;
    }
    if(tbl_algo_ctl.stlm_flag[0]=='0') return NO_SETTLE;
    c_d_flag = atoi(tbl_algo_ctl.c_d_flag);

    memset(&tbl_txn_algo_rule, 0x00, sizeof(tbl_txn_algo_rule));
    memcpy(tbl_txn_algo_rule.bus_tp, ptbl_dat_flow->bus_tp, sizeof(tbl_txn_algo_rule.bus_tp)-1);
    memcpy(tbl_txn_algo_rule.txn_num, ptbl_dat_flow->trans_tp, sizeof(tbl_txn_algo_rule.bus_tp)-1);
    /*清算算法规则表(HT_TBL_TXN_ALGO_RULE）*/
    iRet = DbsTblTxnAlgoRule(DBS_SELECT, &tbl_txn_algo_rule, 0, 0);
    if(iRet)
    {
        HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "bus_tp[%s],trans_tp[%s].", tbl_txn_algo_rule.bus_tp, tbl_txn_algo_rule.txn_num);
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblTxnAlgoRule DBS_SELECT error, %d.", iRet);
        return -1;
    }
    
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "业务类型[%s]. 内部交易码[%s] 规则ID[%s]", tbl_txn_algo_rule.bus_tp, tbl_txn_algo_rule.txn_num, tbl_txn_algo_rule.algo_id);
    iRet = ExecCaclFee(tbl_txn_algo_rule.algo_id);
    if(iRet)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "ExecCaclFee error, %d.", iRet);
        return -1;
    }
    return 0;
}

/*************************************************
*函数名称∶TxnClear
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
static int TxnClear(ht_tbl_dat_flow_def *ptbl_dat_flow)
{
    int         iRet=0;
    ht_tbl_sett_flow_def  tbl_sett_flow;
    
    iRet = InitData(ptbl_dat_flow);
    if(iRet)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "InitData error, %d.", iRet);
        return -1;
    }

    iRet = CaclFee(ptbl_dat_flow);
    if(iRet)
    {
        if(iRet==NO_SETTLE) return 0;
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "CaclFee error, %d.", iRet);
        return -1;
    }
    
    memset(&tbl_sett_flow, 0x00, sizeof(tbl_sett_flow));
    iRet = SaveData(ptbl_dat_flow, &tbl_sett_flow);
    if(iRet)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "SaveData error, %d.", iRet);
        return -1;
    }

    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "tbl_sett_flow.plat_key[%s]",tbl_sett_flow.plat_key);
    iRet = DbsTblSettFlow(DBS_INSERT, &tbl_sett_flow, 0, 0);
    if(iRet)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblSettFlow DBS_INSERT error, %d.", iRet);
        return -1;
    }
    return 0;
}

/*************************************************
*函数名称∶ Total_4101
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_4101()
{
    int		nTotalNum = 0;
    int         iRet=0;
    ht_tbl_dat_flow_def tbl_dat_flow;

    memset(&tbl_dat_flow, 0x00, sizeof(tbl_dat_flow));
    memcpy(tbl_dat_flow.sett_date, dbtbl_date_inf.stoday, sizeof(tbl_dat_flow.sett_date)-1);
    memcpy(tbl_dat_flow.inter_brh_code, ext_inter_brh_code, sizeof(tbl_dat_flow.inter_brh_code)-1);
    iRet = DbsTblDatFlow(DBS_SELECT1, &tbl_dat_flow, 0, 0);
    if (iRet != 0)
    {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblDatFlow DBS_SELECT1 error, %d.", iRet);
            return -1;
    }
    nTotalNum = tbl_dat_flow.seq_num;
    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "nTotalNum =[%d].", nTotalNum);

    return nTotalNum;
}

/*************************************************
*函数名称∶ Task_4101
*函数功能∶
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_4101 ( int nBeginOffset, int nEndOffset )
{
    int	 iRet = 0, nTotCount=0;
    ht_tbl_dat_flow_def tbl_dat_flow;ht_tbl_sett_flow_def  tbl_sett_flow;
    
    memset(&tbl_dat_flow, 0x00, sizeof(tbl_dat_flow));
    memcpy(tbl_dat_flow.sett_date, dbtbl_date_inf.stoday, sizeof(tbl_dat_flow.sett_date)-1);
    memcpy(tbl_dat_flow.inter_brh_code, ext_inter_brh_code, sizeof(tbl_dat_flow.inter_brh_code)-1);
    memcpy(tbl_dat_flow.stlm_flag, "N", sizeof(tbl_dat_flow.stlm_flag)-1);
    DbsTblDatFlow(DBS_CURSOR, &tbl_dat_flow, nBeginOffset, nEndOffset);
    iRet = DbsTblDatFlow(DBS_OPEN, &tbl_dat_flow, nBeginOffset, nEndOffset);
    if (iRet != 0)
    {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblDatFlow DBS_OPEN error, %d.", iRet);
            return -1;
    }

    while (1)
    {
        memset(&tbl_dat_flow, 0, sizeof(tbl_dat_flow));
        iRet = DbsTblDatFlow(DBS_FETCH, &tbl_dat_flow, nBeginOffset, nEndOffset);
        if (iRet)
        {
            if (iRet == DBS_NOTFOUND)
            {
                break;
            }

            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblDatFlow fetch sqlcode[%d]", iRet);
            DbsTblDatFlow(DBS_CLOSE, &tbl_dat_flow, nBeginOffset, nEndOffset);
            return -1;
        }
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "-----------------plat_key[%s]---------", tbl_dat_flow.plat_key);

        nTotCount++;
        iRet = TxnClear(&tbl_dat_flow);
        if (iRet != 0)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "TxnClear error, %d.", iRet);
            return -1;
        }
        
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "DBS_UPDATE plat_key[%s]", tbl_dat_flow.plat_key);
        memcpy(tbl_dat_flow.stlm_flag	,    BT_DAT_STLM_FLAG_C,  BT_DAT_STLM_FLAG_LEN);
        iRet = DbsTblDatFlow(DBS_UPDATE, &tbl_dat_flow, nBeginOffset, nEndOffset);
        if (iRet)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblDatFlow DBS_UPDATE error. sqlcode[%d]", iRet);
            DbsTblDatFlow(DBS_CLOSE, &tbl_dat_flow, nBeginOffset, nEndOffset);
            return -1;
        }
    }
    DbsTblDatFlow(DBS_CLOSE, &tbl_dat_flow, nBeginOffset, nEndOffset);
    
    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "total num [%d].", nTotCount);
    HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_4101 Succ .");
    return 0;
}

/*************************************************
*函数名称∶InSettleStr
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
static int InSettleStr(ht_tbl_his_disc_algo_def *tbl_his_disc_algo, int base)
{
        //最低收费金额(分段)
        pSettleStr[base+1] = tbl_his_disc_algo->floor_amount;
        
        //最大收费金额(分段)
        pSettleStr[base+2] = tbl_his_disc_algo->upper_amount;

        if(memcmp(tbl_his_disc_algo->fee_flag, "1", 1)==0)
        {
            //固定值
            pSettleStr[base+3] = tbl_his_disc_algo->fee_value*100;
        }else{
            //按比例
            pSettleStr[base+4] = tbl_his_disc_algo->fee_value/100;
        }
        
        //最小手续费
        pSettleStr[base+5] = tbl_his_disc_algo->fee_min*100;
        
         //最高手续费
        pSettleStr[base+6] = tbl_his_disc_algo->fee_max*100;
        return 0;
}

/*************************************************
*函数名称∶InitData
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
static int InitData(ht_tbl_dat_flow_def *ptbl_dat_flow)
{
    int iRet;
    double trans_at2 = 0.0;
    ht_tbl_his_disc_algo_def tbl_his_disc_algo;
    ht_tbl_mcht_inf_def tbl_mcht_inf;
    ht_tbl_brh_inf_def tbl_brh_inf;
    ht_tbl_brh_fee_inf_def tbl_brh_fee_inf;
    tp_payway_ext_def tp_payway_ext;
    
    
    memset(pSettleStr, 0 , sizeof(pSettleStr));
    
    pSettleStr[TRANS_AMT] = atof(ptbl_dat_flow->trans_at);
    /*
    pSettleStr[MCHT1_SETT_FEE] = atof(ptbl_dat_flow->trans_at2);
    */
    memset(&tbl_mcht_inf, 0 , sizeof(tbl_mcht_inf));
    memcpy(tbl_mcht_inf.mcht_no, ptbl_dat_flow->mcht_no1, sizeof(tbl_mcht_inf.mcht_no)-1);
    CommonRTrim(tbl_mcht_inf.mcht_no);

    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_no[%s]", tbl_mcht_inf.mcht_no);        
    iRet = DbsTblMchtInf(DBS_SELECT, &tbl_mcht_inf, 0, 0);
    if (iRet != 0 && iRet!=DBS_NOTFOUND)
    {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "tbl_mcht_inf.mcht_no[%s].", tbl_mcht_inf.mcht_no);
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblMchtInf DBS_SELECT error, %d.", iRet);
            return -1;
    }
    
    memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
    memcpy(tbl_his_disc_algo.mcht_no, ptbl_dat_flow->mcht_no1, sizeof(tbl_his_disc_algo.mcht_no)-1);
    CommonRTrim(tbl_his_disc_algo.mcht_no);    
    memcpy(tbl_his_disc_algo.tran_code, ptbl_dat_flow->trans_tp, sizeof(tbl_his_disc_algo.tran_code)-1);
    memcpy(tbl_his_disc_algo.card_type, ptbl_dat_flow->acct_type, sizeof(tbl_his_disc_algo.card_type)-1);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_no[%s]. tbl_his_disc_algo.tran_code[%s] tbl_his_disc_algo.card_type[%s]", tbl_his_disc_algo.mcht_no, tbl_his_disc_algo.tran_code, tbl_his_disc_algo.card_type);
    iRet = DbsTblHisDiscAlgo(DBS_SELECT, &tbl_his_disc_algo);
    if (iRet != 0 && iRet!=DBS_NOTFOUND)
    {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_no[%s],tran_code[%s],card_type[%s].",  tbl_his_disc_algo.mcht_no,tbl_his_disc_algo.tran_code, tbl_his_disc_algo.card_type);
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblHisDiscAlgo DBS_SELECT error, %d.", iRet);
            return -1;
    }
    else if (iRet==DBS_NOTFOUND)
    {
        memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
        memcpy(tbl_his_disc_algo.mcht_no, ptbl_dat_flow->mcht_no1, sizeof(tbl_his_disc_algo.mcht_no)-1);
        CommonRTrim(tbl_his_disc_algo.mcht_no);    
        memcpy(tbl_his_disc_algo.tran_code, "*", 1);
        memcpy(tbl_his_disc_algo.card_type, ptbl_dat_flow->acct_type, sizeof(tbl_his_disc_algo.card_type)-1);
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_no[%s]. tbl_his_disc_algo.tran_code[%s] tbl_his_disc_algo.card_type[%s]", tbl_his_disc_algo.mcht_no, tbl_his_disc_algo.tran_code, tbl_his_disc_algo.card_type);
        iRet = DbsTblHisDiscAlgo(DBS_SELECT, &tbl_his_disc_algo);
        if (iRet != 0 && iRet!=DBS_NOTFOUND)
        {
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_no[%s],tran_code[%s],card_type[%s].",  tbl_his_disc_algo.mcht_no,tbl_his_disc_algo.tran_code, tbl_his_disc_algo.card_type);
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblHisDiscAlgo DBS_SELECT error, %d.", iRet);
                return -1;
        }
        else if (iRet==DBS_NOTFOUND)
        {
            tbl_his_disc_algo.fee_min=0.00;//最低手续费                
            tbl_his_disc_algo.fee_max=atof(ptbl_dat_flow->trans_at)/100; //最高手续费           
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_no[%s] tran_code[%s],费率表未设置费率,默认值.fee_min[%lf]fee_max[%lf]",  tbl_his_disc_algo.mcht_no,tbl_his_disc_algo.tran_code,tbl_his_disc_algo.fee_min,tbl_his_disc_algo.fee_max);           
        }        
    }

    /* 手续费抉择方选择begin*/
    /*支付机构信息表*/
    memset(&tp_payway_ext, 0x00, sizeof(tp_payway_ext));
    ChnIn2Out(tp_payway_ext.channel_code, ptbl_dat_flow->bus_tp);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "ptbl_dat_flow->bus_tp[%s].", ptbl_dat_flow->bus_tp);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "tp_payway_ext.channel_code: [%s].", tp_payway_ext.channel_code);    
    iRet = DbsTpPaywayExt(DBS_SELECT, &tp_payway_ext);
    if (iRet != 0)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTpPaywayExt DBS_SELECT error, %d.", iRet);
        return -1;
    }
    CommonRTrim(tp_payway_ext.channel_code);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "channel_code[%s]compt_fee_flag[%s].", tp_payway_ext.channel_code, tp_payway_ext.compt_fee_flag);
    /*  01 - 对账文件的手续费，02 -清算出来的手续费作为入账手续费  */
    if(memcmp(tp_payway_ext.compt_fee_flag, "01", 2)==0)
    {
        if(memcmp(ptbl_dat_flow->bus_tp, BT_BUS_TP_CIB5, BT_BUS_TP_LEN) == 0 ||
		   memcmp(ptbl_dat_flow->bus_tp, BT_BUS_TP_CIB6, BT_BUS_TP_LEN) == 0 ||
		   memcmp(ptbl_dat_flow->bus_tp, BT_BUS_FY_ALIPAY, BT_BUS_TP_LEN) == 0 ||
		   memcmp(ptbl_dat_flow->bus_tp, BT_BUS_FY_WECHAT, BT_BUS_TP_LEN) == 0)
        {
            /*快捷、网关支付渠道，取文件手续费*/
            pSettleStr[MCHT6_FILE_FLAG] = pSettleStr[TRANS_AMT] + 1.0;   /*用于公式：标识取文件手续费率:交易金额+1*/
            pSettleStr[MCHT6_FILE_FEE] = atof(ptbl_dat_flow->trans_at2);   /*对账文件的手续费*/
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "标识取文件手续费率:交易金额+1[%lf]对账文件的手续费[%lf]", pSettleStr[MCHT6_FILE_FLAG], pSettleStr[MCHT6_FILE_FEE]);
        }
        else if(memcmp(ptbl_dat_flow->bus_tp, BT_BUS_TP_ALIPAY, BT_BUS_TP_LEN) == 0 || memcmp(ptbl_dat_flow->bus_tp, BT_BUS_TP_WECHAT, BT_BUS_TP_LEN) == 0)
        {
            /*支付宝、微信支付渠道，取文件手续费率*/				
            strcpy(tbl_his_disc_algo.fee_flag, "2");        
            tbl_his_disc_algo.fee_value = atof(ptbl_dat_flow->trans_at4);   /*对账文件的手续费率*/
			if(atof(ptbl_dat_flow->trans_at2) <= 0) {
                tbl_his_disc_algo.fee_value = 0.00;
			}
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "对账文件的手续费率[%lf]", tbl_his_disc_algo.fee_value);
        }
    }
    /*手续费抉择方选择end */
    
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "up_mcht_no[%s]. ret_fee_flag[%s]", tbl_mcht_inf.up_mcht_no, tbl_mcht_inf.ret_fee_flag);
    if(memcmp(tbl_mcht_inf.ret_fee_flag, "n", 1) == 0 && memcmp(ptbl_dat_flow->trans_tp, BT_TRANS_TP_REFUND, BT_TRANS_TP_LEN) == 0)
    {
        /*商户信息表设置为不退手续费并且是退款交易 设置手续费率为0*/
        strcpy(tbl_his_disc_algo.fee_flag, "2"); 
        tbl_his_disc_algo.fee_value = 0; 
    }
	

	// 充值是否收手续费
	if(memcmp(tbl_mcht_inf.recharge_flag, "n", 1) == 0 && memcmp(ptbl_dat_flow->trans_tp, BT_TRANS_TP_CHARGE, BT_TRANS_TP_LEN) == 0)
    {
        strcpy(tbl_his_disc_algo.fee_flag, "2"); 
        tbl_his_disc_algo.fee_value = 0; 
    }

    
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_no[%s]. tran_code[%s] card_type[%s][%lf]", tbl_his_disc_algo.mcht_no, tbl_his_disc_algo.tran_code, tbl_his_disc_algo.card_type, tbl_his_disc_algo.fee_value);
    InSettleStr(&tbl_his_disc_algo, (int)MCHT1_BASE);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "结算商户, 最小值[%lf]. 最大值[%lf] 比例[%lf]", pSettleStr[MCHT1_FEE_MIN], pSettleStr[MCHT1_FEE_MAX], pSettleStr[MCHT1_PCE_AMT]);
 
    CommonRTrim(tbl_mcht_inf.spl_acct_no1);
    CommonRTrim(tbl_mcht_inf.spl_acct_no2);
    CommonRTrim(tbl_mcht_inf.spl_acct_no3);
    CommonRTrim(tbl_mcht_inf.spl_acct_no4);    
    CommonRTrim(tbl_mcht_inf.up_mcht_no); 
    
    memcpy(up_mcht_no, tbl_mcht_inf.up_mcht_no, sizeof(up_mcht_no) - 1);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "tbl_mcht_inf.up_mcht_no[%s]", up_mcht_no);        
    if(strlen(tbl_mcht_inf.spl_acct_no1) > 0)
    {
        if(memcmp(ptbl_dat_flow->trans_tp, BT_TRANS_TP_REFUND, BT_TRANS_TP_LEN) == 0)
        {
           if(memcmp(tbl_mcht_inf.ret_fee_flag, "n", 1) == 0)
           {
                memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
                strcpy(tbl_his_disc_algo.fee_flag, "2");        /*按比例*/
                tbl_his_disc_algo.fee_value = 0.0;
                InSettleStr(&tbl_his_disc_algo, (int)MCHT2_BASE);        
                memcpy(ptbl_dat_flow->sett_tp2, BT_SETT_TP_EXT, BT_SETT_TP_LEN);
                CommonRTrim(tbl_mcht_inf.spl_brh_role1);
                strcpy(ptbl_dat_flow->mcht_no2, tbl_mcht_inf.spl_brh_role1);        
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构2, 分润方1[%s].  手续费[%lf] 比例[%lf]", ptbl_dat_flow->mcht_no2, pSettleStr[MCHT2_SETT_FEE], pSettleStr[MCHT2_PCE_AMT]);
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构2[%s], spl_acct_ret_fee_flag1[%s]. ", ptbl_dat_flow->mcht_no2, tbl_mcht_inf.spl_acct_ret_fee_flag1);           
            }
            else if(memcmp(tbl_mcht_inf.ret_fee_flag, "n", 1) != 0)
            {
                memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
                strcpy(tbl_his_disc_algo.fee_flag, "2");        /*按比例*/
                tbl_his_disc_algo.fee_value = tbl_mcht_inf.spl_acct_rate1;
                InSettleStr(&tbl_his_disc_algo, (int)MCHT2_BASE);        
                memcpy(ptbl_dat_flow->sett_tp2, BT_SETT_TP_EXT, BT_SETT_TP_LEN);
                CommonRTrim(tbl_mcht_inf.spl_brh_role1);
                strcpy(ptbl_dat_flow->mcht_no2, tbl_mcht_inf.spl_brh_role1);        
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构2, 分润方1[%s].  手续费[%lf] 比例[%lf]", ptbl_dat_flow->mcht_no2, pSettleStr[MCHT2_SETT_FEE], pSettleStr[MCHT2_PCE_AMT]);
            }
        }
		else if(memcmp(ptbl_dat_flow->trans_tp, BT_TRANS_TP_CHARGE, BT_TRANS_TP_LEN) == 0)
		{
		   if(memcmp(tbl_mcht_inf.recharge_flag, "n", 1) == 0)
           {
                memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
                strcpy(tbl_his_disc_algo.fee_flag, "2");        /*按比例*/
                tbl_his_disc_algo.fee_value = 0.0;
                InSettleStr(&tbl_his_disc_algo, (int)MCHT2_BASE);        
                memcpy(ptbl_dat_flow->sett_tp2, BT_SETT_TP_EXT, BT_SETT_TP_LEN);
                CommonRTrim(tbl_mcht_inf.spl_brh_role1);
                strcpy(ptbl_dat_flow->mcht_no2, tbl_mcht_inf.spl_brh_role1);        
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构2, 分润方1[%s].  手续费[%lf] 比例[%lf]", ptbl_dat_flow->mcht_no2, pSettleStr[MCHT2_SETT_FEE], pSettleStr[MCHT2_PCE_AMT]);
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构2[%s], spl_acct_ret_fee_flag1[%s]. ", ptbl_dat_flow->mcht_no2, tbl_mcht_inf.spl_acct_ret_fee_flag1);           
		   }
		   else if(memcmp(tbl_mcht_inf.recharge_flag, "n", 1) != 0)
           {
                memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
                strcpy(tbl_his_disc_algo.fee_flag, "2");        /*按比例*/
                tbl_his_disc_algo.fee_value = tbl_mcht_inf.spl_acct_rate1;
                InSettleStr(&tbl_his_disc_algo, (int)MCHT2_BASE);        
                memcpy(ptbl_dat_flow->sett_tp2, BT_SETT_TP_EXT, BT_SETT_TP_LEN);
                CommonRTrim(tbl_mcht_inf.spl_brh_role1);
                strcpy(ptbl_dat_flow->mcht_no2, tbl_mcht_inf.spl_brh_role1);        
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构2, 分润方1[%s].  手续费[%lf] 比例[%lf]", ptbl_dat_flow->mcht_no2, pSettleStr[MCHT2_SETT_FEE], pSettleStr[MCHT2_PCE_AMT]);
           }
		}
		else {
            memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
            strcpy(tbl_his_disc_algo.fee_flag, "2");        /*按比例*/
            tbl_his_disc_algo.fee_value = tbl_mcht_inf.spl_acct_rate1;
            InSettleStr(&tbl_his_disc_algo, (int)MCHT2_BASE);        
            memcpy(ptbl_dat_flow->sett_tp2, BT_SETT_TP_EXT, BT_SETT_TP_LEN);
            CommonRTrim(tbl_mcht_inf.spl_brh_role1);
            strcpy(ptbl_dat_flow->mcht_no2, tbl_mcht_inf.spl_brh_role1);        
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构2, 分润方1[%s].  手续费[%lf] 比例[%lf]", ptbl_dat_flow->mcht_no2, pSettleStr[MCHT2_SETT_FEE], pSettleStr[MCHT2_PCE_AMT]);
		}
    }
    
    if(strlen(tbl_mcht_inf.spl_acct_no2) > 0)
    {
        if(memcmp(ptbl_dat_flow->trans_tp, BT_TRANS_TP_REFUND, BT_TRANS_TP_LEN) == 0)
        {
           if(memcmp(tbl_mcht_inf.ret_fee_flag, "n", 1) == 0)
           {
                memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
                strcpy(tbl_his_disc_algo.fee_flag, "2");        /*按比例*/
                tbl_his_disc_algo.fee_value = 0.0;
                InSettleStr(&tbl_his_disc_algo, (int)MCHT3_BASE);        
                memcpy(ptbl_dat_flow->sett_tp3, BT_SETT_TP_EXT, BT_SETT_TP_LEN);
                CommonRTrim(tbl_mcht_inf.spl_brh_role2);
                strcpy(ptbl_dat_flow->mcht_no3, tbl_mcht_inf.spl_brh_role2);        
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构3, 分润方2[%s].  手续费[%lf] 比例[%lf]", ptbl_dat_flow->mcht_no3, pSettleStr[MCHT3_SETT_FEE], pSettleStr[MCHT3_PCE_AMT]);
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构3[%s], spl_acct_ret_fee_flag2[%s]. ", ptbl_dat_flow->mcht_no3, tbl_mcht_inf.spl_acct_ret_fee_flag2);           
            }
            else if(memcmp(tbl_mcht_inf.ret_fee_flag, "n", 1) != 0)
            {
                memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
                strcpy(tbl_his_disc_algo.fee_flag, "2");        /*按比例*/
                tbl_his_disc_algo.fee_value = tbl_mcht_inf.spl_acct_rate2;
                InSettleStr(&tbl_his_disc_algo, (int)MCHT3_BASE);        
                memcpy(ptbl_dat_flow->sett_tp3, BT_SETT_TP_EXT, BT_SETT_TP_LEN);
                CommonRTrim(tbl_mcht_inf.spl_brh_role2);
                strcpy(ptbl_dat_flow->mcht_no3, tbl_mcht_inf.spl_brh_role2);        
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构3, 分润方2[%s].  手续费[%lf] 比例[%lf]", ptbl_dat_flow->mcht_no3, pSettleStr[MCHT3_SETT_FEE], pSettleStr[MCHT3_PCE_AMT]);
            }
        }
		else if(memcmp(ptbl_dat_flow->trans_tp, BT_TRANS_TP_CHARGE, BT_TRANS_TP_LEN) == 0)
		{
		   if(memcmp(tbl_mcht_inf.recharge_flag, "n", 1) == 0)
           {
                memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
                strcpy(tbl_his_disc_algo.fee_flag, "2");        /*按比例*/
                tbl_his_disc_algo.fee_value = 0.0;
                InSettleStr(&tbl_his_disc_algo, (int)MCHT3_BASE);        
                memcpy(ptbl_dat_flow->sett_tp3, BT_SETT_TP_EXT, BT_SETT_TP_LEN);
                CommonRTrim(tbl_mcht_inf.spl_brh_role2);
                strcpy(ptbl_dat_flow->mcht_no3, tbl_mcht_inf.spl_brh_role2);        
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构3, 分润方2[%s].  手续费[%lf] 比例[%lf]", ptbl_dat_flow->mcht_no3, pSettleStr[MCHT3_SETT_FEE], pSettleStr[MCHT3_PCE_AMT]);
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构3[%s], spl_acct_ret_fee_flag2[%s]. ", ptbl_dat_flow->mcht_no3, tbl_mcht_inf.spl_acct_ret_fee_flag2);           
		   }
		   else if(memcmp(tbl_mcht_inf.recharge_flag, "n", 1) != 0)
           {
                memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
                strcpy(tbl_his_disc_algo.fee_flag, "2");        /*按比例*/
                tbl_his_disc_algo.fee_value = tbl_mcht_inf.spl_acct_rate2;
                InSettleStr(&tbl_his_disc_algo, (int)MCHT3_BASE);        
                memcpy(ptbl_dat_flow->sett_tp3, BT_SETT_TP_EXT, BT_SETT_TP_LEN);
                CommonRTrim(tbl_mcht_inf.spl_brh_role2);
                strcpy(ptbl_dat_flow->mcht_no3, tbl_mcht_inf.spl_brh_role2);        
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构3, 分润方2[%s].  手续费[%lf] 比例[%lf]", ptbl_dat_flow->mcht_no3, pSettleStr[MCHT3_SETT_FEE], pSettleStr[MCHT3_PCE_AMT]);
           }
		}
		else {
            memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
            strcpy(tbl_his_disc_algo.fee_flag, "2");        /*按比例*/
            tbl_his_disc_algo.fee_value = tbl_mcht_inf.spl_acct_rate2;
            InSettleStr(&tbl_his_disc_algo, (int)MCHT3_BASE);        
            memcpy(ptbl_dat_flow->sett_tp3, BT_SETT_TP_EXT, BT_SETT_TP_LEN);
            CommonRTrim(tbl_mcht_inf.spl_brh_role2);
            strcpy(ptbl_dat_flow->mcht_no3, tbl_mcht_inf.spl_brh_role2);        
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构3, 分润方2[%s].  手续费[%lf] 比例[%lf]", ptbl_dat_flow->mcht_no3, pSettleStr[MCHT3_SETT_FEE], pSettleStr[MCHT3_PCE_AMT]);
		}
    }
    
    if(strlen(tbl_mcht_inf.spl_acct_no3) > 0)
    {
        if(memcmp(ptbl_dat_flow->trans_tp, BT_TRANS_TP_REFUND, BT_TRANS_TP_LEN) == 0)
        {
           if(memcmp(tbl_mcht_inf.ret_fee_flag, "n", 1) == 0)
           {
                memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
                strcpy(tbl_his_disc_algo.fee_flag, "2");        /*按比例*/
                tbl_his_disc_algo.fee_value = 0.0;
                InSettleStr(&tbl_his_disc_algo, (int)MCHT4_BASE);        
                memcpy(ptbl_dat_flow->sett_tp4, BT_SETT_TP_EXT, BT_SETT_TP_LEN);
                CommonRTrim(tbl_mcht_inf.spl_brh_role3);
                strcpy(ptbl_dat_flow->mcht_no4, tbl_mcht_inf.spl_brh_role3);        
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构4, 分润方3[%s].  手续费[%lf] 比例[%lf]", ptbl_dat_flow->mcht_no4, pSettleStr[MCHT4_SETT_FEE], pSettleStr[MCHT4_PCE_AMT]);
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构4[%s], spl_acct_ret_fee_flag3[%s]. ", ptbl_dat_flow->mcht_no4, tbl_mcht_inf.spl_acct_ret_fee_flag3);           
            }
            else if(memcmp(tbl_mcht_inf.ret_fee_flag, "n", 1) != 0)
            {
                memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
                strcpy(tbl_his_disc_algo.fee_flag, "2");        /*按比例*/
                tbl_his_disc_algo.fee_value = tbl_mcht_inf.spl_acct_rate3;
                InSettleStr(&tbl_his_disc_algo, (int)MCHT4_BASE);        
                memcpy(ptbl_dat_flow->sett_tp4, BT_SETT_TP_EXT, BT_SETT_TP_LEN);
                CommonRTrim(tbl_mcht_inf.spl_brh_role3);
                strcpy(ptbl_dat_flow->mcht_no4, tbl_mcht_inf.spl_brh_role3);        
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构4, 分润方3[%s].  手续费[%lf] 比例[%lf]", ptbl_dat_flow->mcht_no4, pSettleStr[MCHT4_SETT_FEE], pSettleStr[MCHT4_PCE_AMT]);
            }
        }
		else if(memcmp(ptbl_dat_flow->trans_tp, BT_TRANS_TP_CHARGE, BT_TRANS_TP_LEN) == 0)
		{
		   if(memcmp(tbl_mcht_inf.recharge_flag, "n", 1) == 0)
           {
                memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
                strcpy(tbl_his_disc_algo.fee_flag, "2");        /*按比例*/
                tbl_his_disc_algo.fee_value = 0.0;
                InSettleStr(&tbl_his_disc_algo, (int)MCHT4_BASE);        
                memcpy(ptbl_dat_flow->sett_tp4, BT_SETT_TP_EXT, BT_SETT_TP_LEN);
                CommonRTrim(tbl_mcht_inf.spl_brh_role3);
                strcpy(ptbl_dat_flow->mcht_no4, tbl_mcht_inf.spl_brh_role3);        
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构4, 分润方3[%s].  手续费[%lf] 比例[%lf]", ptbl_dat_flow->mcht_no4, pSettleStr[MCHT4_SETT_FEE], pSettleStr[MCHT4_PCE_AMT]);
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构4[%s], spl_acct_ret_fee_flag3[%s]. ", ptbl_dat_flow->mcht_no4, tbl_mcht_inf.spl_acct_ret_fee_flag3);           
		   }
		   else if(memcmp(tbl_mcht_inf.recharge_flag, "n", 1) != 0)
           {
                memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
                strcpy(tbl_his_disc_algo.fee_flag, "2");        /*按比例*/
                tbl_his_disc_algo.fee_value = tbl_mcht_inf.spl_acct_rate3;
                InSettleStr(&tbl_his_disc_algo, (int)MCHT4_BASE);        
                memcpy(ptbl_dat_flow->sett_tp4, BT_SETT_TP_EXT, BT_SETT_TP_LEN);
                CommonRTrim(tbl_mcht_inf.spl_brh_role3);
                strcpy(ptbl_dat_flow->mcht_no4, tbl_mcht_inf.spl_brh_role3);        
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构4, 分润方3[%s].  手续费[%lf] 比例[%lf]", ptbl_dat_flow->mcht_no4, pSettleStr[MCHT4_SETT_FEE], pSettleStr[MCHT4_PCE_AMT]);
           }
		}
		else {
            memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
            strcpy(tbl_his_disc_algo.fee_flag, "2");        /*按比例*/
            tbl_his_disc_algo.fee_value = tbl_mcht_inf.spl_acct_rate3;
            InSettleStr(&tbl_his_disc_algo, (int)MCHT4_BASE);        
            memcpy(ptbl_dat_flow->sett_tp4, BT_SETT_TP_EXT, BT_SETT_TP_LEN);
            CommonRTrim(tbl_mcht_inf.spl_brh_role3);
            strcpy(ptbl_dat_flow->mcht_no4, tbl_mcht_inf.spl_brh_role3);        
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构4, 分润方3[%s].  手续费[%lf] 比例[%lf]", ptbl_dat_flow->mcht_no4, pSettleStr[MCHT4_SETT_FEE], pSettleStr[MCHT4_PCE_AMT]);
		}
    }
    
    if(strlen(tbl_mcht_inf.spl_acct_no4) > 0)
    {
        if(memcmp(ptbl_dat_flow->trans_tp, BT_TRANS_TP_REFUND, BT_TRANS_TP_LEN) == 0)
        {
           if(memcmp(tbl_mcht_inf.ret_fee_flag, "n", 1) == 0)
           {
                memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
                strcpy(tbl_his_disc_algo.fee_flag, "2");        /*按比例*/
                tbl_his_disc_algo.fee_value = 0.0;
                InSettleStr(&tbl_his_disc_algo, (int)MCHT5_BASE);        
                memcpy(ptbl_dat_flow->sett_tp5, BT_SETT_TP_EXT, BT_SETT_TP_LEN);
                CommonRTrim(tbl_mcht_inf.spl_brh_role4);
                strcpy(ptbl_dat_flow->mcht_no5, tbl_mcht_inf.spl_brh_role4);        
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构5, 分润方4[%s].  手续费[%lf] 比例[%lf]", ptbl_dat_flow->mcht_no5, pSettleStr[MCHT5_SETT_FEE], pSettleStr[MCHT5_PCE_AMT]);
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构5[%s], spl_acct_ret_fee_flag4[%s]. ", ptbl_dat_flow->mcht_no5, tbl_mcht_inf.spl_acct_ret_fee_flag4);           
            }
            else if(memcmp(tbl_mcht_inf.ret_fee_flag, "n", 1) != 0)
            {
                memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
                strcpy(tbl_his_disc_algo.fee_flag, "2");        /*按比例*/
                tbl_his_disc_algo.fee_value = tbl_mcht_inf.spl_acct_rate4;
                InSettleStr(&tbl_his_disc_algo, (int)MCHT5_BASE);        
                memcpy(ptbl_dat_flow->sett_tp5, BT_SETT_TP_EXT, BT_SETT_TP_LEN);
                CommonRTrim(tbl_mcht_inf.spl_brh_role4);
                strcpy(ptbl_dat_flow->mcht_no5, tbl_mcht_inf.spl_brh_role4);        
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构5, 分润方4[%s].  手续费[%lf] 比例[%lf]", ptbl_dat_flow->mcht_no5, pSettleStr[MCHT5_SETT_FEE], pSettleStr[MCHT5_PCE_AMT]);
            }
        }
		else if(memcmp(ptbl_dat_flow->trans_tp, BT_TRANS_TP_CHARGE, BT_TRANS_TP_LEN) == 0)
		{
		   if(memcmp(tbl_mcht_inf.recharge_flag, "n", 1) == 0)
           {
                memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
                strcpy(tbl_his_disc_algo.fee_flag, "2");        /*按比例*/
                tbl_his_disc_algo.fee_value = 0.0;
                InSettleStr(&tbl_his_disc_algo, (int)MCHT5_BASE);        
                memcpy(ptbl_dat_flow->sett_tp5, BT_SETT_TP_EXT, BT_SETT_TP_LEN);
                CommonRTrim(tbl_mcht_inf.spl_brh_role4);
                strcpy(ptbl_dat_flow->mcht_no5, tbl_mcht_inf.spl_brh_role4);        
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构5, 分润方4[%s].  手续费[%lf] 比例[%lf]", ptbl_dat_flow->mcht_no5, pSettleStr[MCHT5_SETT_FEE], pSettleStr[MCHT5_PCE_AMT]);
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构5[%s], spl_acct_ret_fee_flag4[%s]. ", ptbl_dat_flow->mcht_no5, tbl_mcht_inf.spl_acct_ret_fee_flag4);           
		   }
		   else if(memcmp(tbl_mcht_inf.recharge_flag, "n", 1) != 0)
           {
                memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
                strcpy(tbl_his_disc_algo.fee_flag, "2");        /*按比例*/
                tbl_his_disc_algo.fee_value = tbl_mcht_inf.spl_acct_rate4;
                InSettleStr(&tbl_his_disc_algo, (int)MCHT5_BASE);        
                memcpy(ptbl_dat_flow->sett_tp5, BT_SETT_TP_EXT, BT_SETT_TP_LEN);
                CommonRTrim(tbl_mcht_inf.spl_brh_role4);
                strcpy(ptbl_dat_flow->mcht_no5, tbl_mcht_inf.spl_brh_role4);        
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构5, 分润方4[%s].  手续费[%lf] 比例[%lf]", ptbl_dat_flow->mcht_no5, pSettleStr[MCHT5_SETT_FEE], pSettleStr[MCHT5_PCE_AMT]);
           }
		}
		else {
            memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
            strcpy(tbl_his_disc_algo.fee_flag, "2");        /*按比例*/
            tbl_his_disc_algo.fee_value = tbl_mcht_inf.spl_acct_rate4;
            InSettleStr(&tbl_his_disc_algo, (int)MCHT5_BASE);        
            memcpy(ptbl_dat_flow->sett_tp5, BT_SETT_TP_EXT, BT_SETT_TP_LEN);
            CommonRTrim(tbl_mcht_inf.spl_brh_role4);
            strcpy(ptbl_dat_flow->mcht_no5, tbl_mcht_inf.spl_brh_role4);        
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构5, 分润方4[%s].  手续费[%lf] 比例[%lf]", ptbl_dat_flow->mcht_no5, pSettleStr[MCHT5_SETT_FEE], pSettleStr[MCHT5_PCE_AMT]);
		}
    }
    
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_no[%s];比例[%lf][%lf][%lf][%lf]", tbl_mcht_inf.mcht_no, pSettleStr[MCHT2_PCE_AMT], pSettleStr[MCHT3_PCE_AMT], pSettleStr[MCHT4_PCE_AMT], pSettleStr[MCHT5_PCE_AMT]);

    memcpy(ptbl_dat_flow->sett_tp6, BT_SETT_TP_BRH, BT_SETT_TP_LEN);
    memcpy(ptbl_dat_flow->mcht_no6, tp_payway_ext.external_org_no, 8);
    CommonRTrim(ptbl_dat_flow->mcht_no6);
	CommonRTrim(ptbl_dat_flow->ext_mcht_no);
    
    memset(&tbl_brh_fee_inf, 0, sizeof(tbl_brh_fee_inf));
    memcpy(tbl_brh_fee_inf.tran_code, ptbl_dat_flow->trans_tp, 4);
    memcpy(tbl_brh_fee_inf.card_type, ptbl_dat_flow->acct_type, 2);
    memcpy(tbl_brh_fee_inf.payway_code, tp_payway_ext.channel_code, sizeof(tbl_brh_fee_inf.payway_code)-1);
	memcpy(tbl_brh_fee_inf.ext_mch_no, ptbl_dat_flow->ext_mcht_no, sizeof(tbl_brh_fee_inf.ext_mch_no)-1);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "payway_code[%s]trans_code[%s];card_type[%s]", tbl_brh_fee_inf.payway_code, tbl_brh_fee_inf.tran_code, tbl_brh_fee_inf.card_type); 
    
    iRet = DbsTblBrhFeeInfTxn(DBS_SELECT, &tbl_brh_fee_inf, 0, 0);
    if (iRet != 0 && iRet!=DBS_NOTFOUND)
    {
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "tbl_mcht_inf.mcht_no[%s].", tbl_brh_fee_inf.payway_code);
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblBrhFeeInfTxn DBS_SELECT error, %d.", iRet);
        return -1;
    }
    else if (iRet==DBS_NOTFOUND)
    {
        memset(&tbl_brh_fee_inf, 0, sizeof(tbl_brh_fee_inf));
        memcpy(tbl_brh_fee_inf.tran_code, "*", 1);
        memcpy(tbl_brh_fee_inf.card_type, ptbl_dat_flow->acct_type, 2);
        memcpy(tbl_brh_fee_inf.payway_code, tp_payway_ext.channel_code, sizeof(tbl_brh_fee_inf.payway_code)-1);
		memcpy(tbl_brh_fee_inf.ext_mch_no, ptbl_dat_flow->ext_mcht_no, sizeof(tbl_brh_fee_inf.ext_mch_no)-1);
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "pay_brh[%s]trans_code[%s];card_type[%s][%s]", tbl_brh_fee_inf.payway_code, tbl_brh_fee_inf.tran_code, tbl_brh_fee_inf.card_type, tbl_brh_fee_inf.payway_code); 
        
        iRet = DbsTblBrhFeeInfTxn(DBS_SELECT, &tbl_brh_fee_inf, 0, 0);
        if (iRet != 0 && iRet!=DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "tbl_mcht_inf.mcht_no[%s].", tbl_brh_fee_inf.payway_code);
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblBrhFeeInfTxn DBS_SELECT error, %d.", iRet);
            return -1;
        }
    }
    
    CommonRTrim(tbl_brh_fee_inf.fee_flag);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "payway_code[%s];fee_value[%lf]fee_flag[%s]", tbl_brh_fee_inf.payway_code, tbl_brh_fee_inf.fee_value, tbl_brh_fee_inf.fee_flag); 
    strcpy(tbl_his_disc_algo.fee_flag, tbl_brh_fee_inf.fee_flag);        
    tbl_his_disc_algo.fee_value = tbl_brh_fee_inf.fee_value;
    tbl_his_disc_algo.fee_min = tbl_brh_fee_inf.fee_min;
    tbl_his_disc_algo.fee_max = tbl_brh_fee_inf.fee_max;
    InSettleStr(&tbl_his_disc_algo, (int)MCHT6_BASE);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "支付机构成本, 最小手续费[%lf]. 最高手续费[%lf] 比例[%lf]", pSettleStr[MCHT6_FEE_MIN], pSettleStr[MCHT6_FEE_MAX], pSettleStr[MCHT6_PCE_AMT]); 
  
    memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
    memcpy(tbl_his_disc_algo.tran_code, ptbl_dat_flow->trans_tp, sizeof(tbl_his_disc_algo.tran_code)-1);
    memcpy(tbl_his_disc_algo.mcht_no, ptbl_dat_flow->mcht_no7, sizeof(tbl_his_disc_algo.mcht_no)-1);
    memcpy(tbl_his_disc_algo.card_type, ptbl_dat_flow->acct_type, sizeof(ptbl_dat_flow->acct_type)-1);
    CommonRTrim(tbl_his_disc_algo.mcht_no);
    iRet = DbsTblHisDiscAlgo(DBS_SELECT, &tbl_his_disc_algo);
    if (iRet != 0 && iRet!=DBS_NOTFOUND)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblHisDiscAlgo DBS_SELECT error, %d.", iRet);
        return -1;
    }
    else if (iRet==DBS_NOTFOUND)
    {
        memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
        memcpy(tbl_his_disc_algo.mcht_no, ptbl_dat_flow->mcht_no1, sizeof(tbl_his_disc_algo.mcht_no)-1);
        CommonRTrim(tbl_his_disc_algo.mcht_no);    
        memcpy(tbl_his_disc_algo.tran_code, "*", 1);
        memcpy(tbl_his_disc_algo.card_type, ptbl_dat_flow->acct_type, sizeof(tbl_his_disc_algo.card_type)-1);
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_no[%s]. tbl_his_disc_algo.tran_code[%s] tbl_his_disc_algo.card_type[%s]", tbl_his_disc_algo.mcht_no, tbl_his_disc_algo.tran_code, tbl_his_disc_algo.card_type);
        iRet = DbsTblHisDiscAlgo(DBS_SELECT, &tbl_his_disc_algo);
        if (iRet != 0 && iRet!=DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_no[%s],tran_code[%s],card_type[%s].",  tbl_his_disc_algo.mcht_no,tbl_his_disc_algo.tran_code, tbl_his_disc_algo.card_type);
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblHisDiscAlgo DBS_SELECT error, %d.", iRet);
            return -1;
        }
    }
    InSettleStr(&tbl_his_disc_algo, (int)MCHT7_BASE);
    
    memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo)); 
    memcpy(tbl_his_disc_algo.tran_code, ptbl_dat_flow->trans_tp, sizeof(tbl_his_disc_algo.tran_code)-1);
    memcpy(tbl_his_disc_algo.mcht_no, ptbl_dat_flow->mcht_no8, sizeof(tbl_his_disc_algo.mcht_no)-1);
    memcpy(tbl_his_disc_algo.card_type, ptbl_dat_flow->acct_type, sizeof(ptbl_dat_flow->acct_type)-1);
    CommonRTrim(tbl_his_disc_algo.mcht_no);
    iRet = DbsTblHisDiscAlgo(DBS_SELECT, &tbl_his_disc_algo);
    if (iRet != 0 && iRet!=DBS_NOTFOUND)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblHisDiscAlgo DBS_SELECT error, %d.", iRet);
        return -1;
    }
    else if (iRet==DBS_NOTFOUND)
    {
        memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
        memcpy(tbl_his_disc_algo.mcht_no, ptbl_dat_flow->mcht_no1, sizeof(tbl_his_disc_algo.mcht_no)-1);
        CommonRTrim(tbl_his_disc_algo.mcht_no);    
        memcpy(tbl_his_disc_algo.tran_code, "*", 1);
        memcpy(tbl_his_disc_algo.card_type, ptbl_dat_flow->acct_type, sizeof(tbl_his_disc_algo.card_type)-1);
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_no[%s]. tbl_his_disc_algo.tran_code[%s] tbl_his_disc_algo.card_type[%s]", tbl_his_disc_algo.mcht_no, tbl_his_disc_algo.tran_code, tbl_his_disc_algo.card_type);
        iRet = DbsTblHisDiscAlgo(DBS_SELECT, &tbl_his_disc_algo);
        if (iRet != 0 && iRet!=DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_no[%s],tran_code[%s],card_type[%s].",  tbl_his_disc_algo.mcht_no,tbl_his_disc_algo.tran_code, tbl_his_disc_algo.card_type);
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblHisDiscAlgo DBS_SELECT error, %d.", iRet);
            return -1;
        }
    }
    InSettleStr(&tbl_his_disc_algo, (int)MCHT8_BASE);
    
    memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
    memcpy(tbl_his_disc_algo.tran_code, ptbl_dat_flow->trans_tp, sizeof(tbl_his_disc_algo.tran_code)-1);
    memcpy(tbl_his_disc_algo.mcht_no, ptbl_dat_flow->mcht_no9, sizeof(tbl_his_disc_algo.mcht_no)-1);
    memcpy(tbl_his_disc_algo.card_type, ptbl_dat_flow->acct_type, sizeof(ptbl_dat_flow->acct_type)-1);
    CommonRTrim(tbl_his_disc_algo.mcht_no);
    iRet = DbsTblHisDiscAlgo(DBS_SELECT, &tbl_his_disc_algo);
    if (iRet != 0 && iRet!=DBS_NOTFOUND)
    {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblHisDiscAlgo DBS_SELECT error, %d.", iRet);
            return -1;
    }
    else if (iRet==DBS_NOTFOUND)
    {
        memset(&tbl_his_disc_algo, 0x00, sizeof(tbl_his_disc_algo));
        memcpy(tbl_his_disc_algo.mcht_no, ptbl_dat_flow->mcht_no1, sizeof(tbl_his_disc_algo.mcht_no)-1);
        CommonRTrim(tbl_his_disc_algo.mcht_no);    
        memcpy(tbl_his_disc_algo.tran_code, "*", 1);
        memcpy(tbl_his_disc_algo.card_type, ptbl_dat_flow->acct_type, sizeof(tbl_his_disc_algo.card_type)-1);
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_no[%s]. tbl_his_disc_algo.tran_code[%s] tbl_his_disc_algo.card_type[%s]", tbl_his_disc_algo.mcht_no, tbl_his_disc_algo.tran_code, tbl_his_disc_algo.card_type);
        iRet = DbsTblHisDiscAlgo(DBS_SELECT, &tbl_his_disc_algo);
        if (iRet != 0 && iRet!=DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_no[%s],tran_code[%s],card_type[%s].",  tbl_his_disc_algo.mcht_no,tbl_his_disc_algo.tran_code, tbl_his_disc_algo.card_type);
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblHisDiscAlgo DBS_SELECT error, %d.", iRet);
            return -1;
        }
    }
    InSettleStr(&tbl_his_disc_algo, (int)MCHT9_BASE);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "合作机构2, 本金[%lf]. 手续费[%lf] 比例[%lf]", pSettleStr[MCHT2_SETT_AMT], pSettleStr[MCHT2_SETT_FEE], pSettleStr[MCHT2_PCE_AMT]);
    return 0;
}

static int SaveData(ht_tbl_dat_flow_def *ptbl_dat_flow, ht_tbl_sett_flow_def  *ptbl_sett_flow)
{
    int txn_flag=1;
    int iRet = 0;
    
    memcpy(ptbl_sett_flow->inter_brh_code   , ptbl_dat_flow->inter_brh_code   , sizeof(ptbl_sett_flow->inter_brh_code   )-1);
    memcpy(ptbl_sett_flow->sett_date        , ptbl_dat_flow->sett_date        , sizeof(ptbl_sett_flow->sett_date        )-1);
    memcpy(ptbl_sett_flow->trans_date       , ptbl_dat_flow->trans_date       , sizeof(ptbl_sett_flow->trans_date       )-1);
    memcpy(ptbl_sett_flow->trans_time       , ptbl_dat_flow->trans_time       , sizeof(ptbl_sett_flow->trans_time       )-1);
    memcpy(ptbl_sett_flow->flag_result      , ptbl_dat_flow->flag_result      , sizeof(ptbl_sett_flow->flag_result      )-1);
    memcpy(ptbl_sett_flow->order_no         , ptbl_dat_flow->order_no         , sizeof(ptbl_sett_flow->order_no         )-1);
    memcpy(ptbl_sett_flow->plat_key         , ptbl_dat_flow->plat_key         , sizeof(ptbl_sett_flow->plat_key         )-1);
    memcpy(ptbl_sett_flow->batch_no         , ptbl_dat_flow->batch_no         , sizeof(ptbl_sett_flow->batch_no         )-1);
    memcpy(ptbl_sett_flow->bus_tp           , ptbl_dat_flow->bus_tp           , sizeof(ptbl_sett_flow->bus_tp           )-1);
    memcpy(ptbl_sett_flow->trans_tp         , ptbl_dat_flow->trans_tp         , sizeof(ptbl_sett_flow->trans_tp         )-1);
    memcpy(ptbl_sett_flow->acct_type        , ptbl_dat_flow->acct_type        , sizeof(ptbl_sett_flow->acct_type        )-1);
    memcpy(ptbl_sett_flow->trans_state      , ptbl_dat_flow->trans_state      , sizeof(ptbl_sett_flow->trans_state      )-1);
    memcpy(ptbl_sett_flow->revsal_flag      , ptbl_dat_flow->revsal_flag      , sizeof(ptbl_sett_flow->revsal_flag      )-1);
    memcpy(ptbl_sett_flow->key_revsal       , ptbl_dat_flow->key_revsal       , sizeof(ptbl_sett_flow->key_revsal       )-1);
    memcpy(ptbl_sett_flow->cancel_flag      , ptbl_dat_flow->cancel_flag      , sizeof(ptbl_sett_flow->cancel_flag      )-1);
    memcpy(ptbl_sett_flow->key_cancel       , ptbl_dat_flow->key_cancel       , sizeof(ptbl_sett_flow->key_cancel       )-1);
    memcpy(ptbl_sett_flow->pan              , ptbl_dat_flow->pan              , sizeof(ptbl_sett_flow->pan              )-1);
    memcpy(ptbl_sett_flow->pan1             , ptbl_dat_flow->pan1             , sizeof(ptbl_sett_flow->pan1              )-1);
    memcpy(ptbl_sett_flow->ccy              , ptbl_dat_flow->ccy              , sizeof(ptbl_sett_flow->ccy              )-1);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "c_d_flag,[%d]", c_d_flag);
    if(c_d_flag == TXN_DEBIT)
    {
        txn_flag*=-1;
    }
    ptbl_sett_flow->trans_at =   atof(ptbl_dat_flow->trans_at) * txn_flag;
    ptbl_sett_flow->trans_at2 = atof(ptbl_dat_flow->trans_at2);
    ptbl_sett_flow->trans_at3 = atof(ptbl_dat_flow->trans_at3) * txn_flag;
    ptbl_sett_flow->trans_at4 = atof(ptbl_dat_flow->trans_at4);
    ptbl_sett_flow->trans_at5 = atof(ptbl_dat_flow->trans_at5) * txn_flag;
    if(memcmp(ptbl_sett_flow->trans_tp, BT_TRANS_TP_WITHDRAW, BT_TRANS_TP_LEN) == 0)
    {
        ptbl_sett_flow->share_profit_plat = pSettleStr[PLAT_SHARE_ALLOT];
        ptbl_sett_flow->share_profit_mcht_no1= pSettleStr[MCHT1_SHARE_ALLOT];
		ptbl_sett_flow->share_profit_mcht_no2= pSettleStr[MCHT2_SHARE_ALLOT];
        ptbl_sett_flow->share_profit_mcht_no3= pSettleStr[MCHT3_SHARE_ALLOT];
        ptbl_sett_flow->share_profit_mcht_no4= pSettleStr[MCHT4_SHARE_ALLOT];
        ptbl_sett_flow->share_profit_mcht_no5= pSettleStr[MCHT5_SHARE_ALLOT];
        ptbl_sett_flow->share_profit_mcht_no6= pSettleStr[MCHT6_SHARE_ALLOT];
        ptbl_sett_flow->share_profit_mcht_no7= pSettleStr[MCHT7_SHARE_ALLOT];
        ptbl_sett_flow->share_profit_mcht_no8= pSettleStr[MCHT8_SHARE_ALLOT];
        ptbl_sett_flow->share_profit_mcht_no9= pSettleStr[MCHT9_SHARE_ALLOT];

    }
    else
    {
        ptbl_sett_flow->share_profit_plat = pSettleStr[PLAT_SHARE_ALLOT] * txn_flag;
        ptbl_sett_flow->share_profit_mcht_no1= pSettleStr[MCHT1_SHARE_ALLOT] * txn_flag;
		ptbl_sett_flow->share_profit_mcht_no2= pSettleStr[MCHT2_SHARE_ALLOT] * txn_flag;
        ptbl_sett_flow->share_profit_mcht_no3= pSettleStr[MCHT3_SHARE_ALLOT] * txn_flag;
        ptbl_sett_flow->share_profit_mcht_no4= pSettleStr[MCHT4_SHARE_ALLOT] * txn_flag;
        ptbl_sett_flow->share_profit_mcht_no5= pSettleStr[MCHT5_SHARE_ALLOT] * txn_flag;
        ptbl_sett_flow->share_profit_mcht_no6= pSettleStr[MCHT6_SHARE_ALLOT] * txn_flag;
        ptbl_sett_flow->share_profit_mcht_no7= pSettleStr[MCHT7_SHARE_ALLOT] * txn_flag;
        ptbl_sett_flow->share_profit_mcht_no8= pSettleStr[MCHT8_SHARE_ALLOT] * txn_flag;
        ptbl_sett_flow->share_profit_mcht_no9= pSettleStr[MCHT9_SHARE_ALLOT] * txn_flag;

    } 
   
	if(pSettleStr[MCHT1_SETT_FEE] - pSettleStr[MCHT6_SHARE_ALLOT] < 0) 
	{
        ptbl_sett_flow->share_profit_mcht_no2= 0;
        ptbl_sett_flow->share_profit_mcht_no3= 0;
        ptbl_sett_flow->share_profit_mcht_no4= 0;
        ptbl_sett_flow->share_profit_mcht_no5= 0;
        ptbl_sett_flow->share_profit_mcht_no7= 0;
        ptbl_sett_flow->share_profit_mcht_no8= 0;
        ptbl_sett_flow->share_profit_mcht_no9= 0;
		ptbl_sett_flow->share_profit_plat = pSettleStr[MCHT1_SETT_FEE] - pSettleStr[MCHT6_SHARE_ALLOT];
	}

    memcpy(ptbl_sett_flow->ext_mcht_no      , ptbl_dat_flow->ext_mcht_no      , sizeof(ptbl_sett_flow->ext_mcht_no      )-1);
    memcpy(ptbl_sett_flow->sett_tp1         , ptbl_dat_flow->sett_tp1         , sizeof(ptbl_sett_flow->sett_tp1         )-1);
    memcpy(ptbl_sett_flow->mcht_no1         , ptbl_dat_flow->mcht_no1         , sizeof(ptbl_sett_flow->mcht_no1         )-1);
    
    if(memcmp(ptbl_sett_flow->bus_tp+4, BT_SETT_TRAN_D, 4) == 0)
    {
        /*直联模式*/
        memcpy(ptbl_sett_flow->mcht_role1       , BT_MCHT_ROLE_SETT_D, BT_MCHT_ROLE_LEN);
        memcpy(ptbl_sett_flow->mcht_role2       , BT_MCHT_ROLE_PROF_BRH_D, BT_MCHT_ROLE_LEN);
        memcpy(ptbl_sett_flow->mcht_role3       , BT_MCHT_ROLE_PROF_BRH_D, BT_MCHT_ROLE_LEN);
        memcpy(ptbl_sett_flow->mcht_role4       , BT_MCHT_ROLE_PROF_BRH_D, BT_MCHT_ROLE_LEN);
        memcpy(ptbl_sett_flow->mcht_role5       , BT_MCHT_ROLE_PROF_BRH_D, BT_MCHT_ROLE_LEN);
        memcpy(ptbl_sett_flow->mcht_role6       , BT_MCHT_ROLE_PAY_BRH_D, BT_MCHT_ROLE_LEN);
    }
    else
    {
        /*二清模式*/
        memcpy(ptbl_sett_flow->mcht_role1       , BT_MCHT_ROLE_SETT_IND, BT_MCHT_ROLE_LEN);  
        memcpy(ptbl_sett_flow->mcht_role2       , BT_MCHT_ROLE_PROF_BRH_IND, BT_MCHT_ROLE_LEN);  
        memcpy(ptbl_sett_flow->mcht_role3       , BT_MCHT_ROLE_PROF_BRH_IND, BT_MCHT_ROLE_LEN);
        memcpy(ptbl_sett_flow->mcht_role4       , BT_MCHT_ROLE_PROF_BRH_IND, BT_MCHT_ROLE_LEN);
        memcpy(ptbl_sett_flow->mcht_role5       , BT_MCHT_ROLE_PROF_BRH_IND, BT_MCHT_ROLE_LEN);
        memcpy(ptbl_sett_flow->mcht_role6       , BT_MCHT_ROLE_PAY_BRH_IND, BT_MCHT_ROLE_LEN);
    }
    
    ptbl_sett_flow->mcht_rate1 = pSettleStr[MCHT1_PCE_AMT];
    if(c_d_flag != TXN_DEBIT)
    {
        ptbl_sett_flow->cret_fee_mcht_no1= pSettleStr[MCHT1_SETT_FEE];
        ptbl_sett_flow->debt_at_mcht_no1 = pSettleStr[MCHT1_SETT_AMT];
    }else{
        ptbl_sett_flow->debt_fee_mcht_no1 = pSettleStr[MCHT1_SETT_FEE];
        ptbl_sett_flow->cret_at_mcht_no1  = pSettleStr[MCHT1_SETT_AMT];
    }
    
    if(memcmp(ptbl_sett_flow->trans_tp, BT_TRANS_TP_WITHDRAW, BT_TRANS_TP_LEN) == 0)
    {
        ptbl_sett_flow->debt_fee_mcht_no1 = 0.00;
        ptbl_sett_flow->cret_fee_mcht_no1= pSettleStr[MCHT1_SETT_FEE];
    }
        
    memcpy(ptbl_sett_flow->sett_tp2         , ptbl_dat_flow->sett_tp2         , sizeof(ptbl_sett_flow->sett_tp2         )-1);
    memcpy(ptbl_sett_flow->mcht_no2         , ptbl_dat_flow->mcht_no2         , sizeof(ptbl_sett_flow->mcht_no2         )-1);
    
    
    ptbl_sett_flow->mcht_rate2 = pSettleStr[MCHT2_PCE_AMT];
    if(c_d_flag != TXN_DEBIT)
    {
        ptbl_sett_flow->debt_at_mcht_no2  =pSettleStr[MCHT2_SETT_AMT];
        ptbl_sett_flow->cret_fee_mcht_no2 =pSettleStr[MCHT2_SETT_FEE];
    }else{
        ptbl_sett_flow->cret_at_mcht_no2  =pSettleStr[MCHT2_SETT_AMT];
        ptbl_sett_flow->debt_fee_mcht_no2 =pSettleStr[MCHT2_SETT_FEE];
    }
    
    
    memcpy(ptbl_sett_flow->sett_tp3         , ptbl_dat_flow->sett_tp3         , sizeof(ptbl_sett_flow->sett_tp3         )-1);
    memcpy(ptbl_sett_flow->mcht_no3         , ptbl_dat_flow->mcht_no3         , sizeof(ptbl_sett_flow->mcht_no3         )-1);
    
    
    ptbl_sett_flow->mcht_rate3 = pSettleStr[MCHT3_PCE_AMT];
    if(c_d_flag != TXN_DEBIT)
    {
        ptbl_sett_flow->debt_at_mcht_no3  =pSettleStr[MCHT3_SETT_AMT];
        ptbl_sett_flow->cret_fee_mcht_no3 =pSettleStr[MCHT3_SETT_FEE];
    }else{
        ptbl_sett_flow->cret_at_mcht_no3  =pSettleStr[MCHT3_SETT_AMT];
        ptbl_sett_flow->debt_fee_mcht_no3 =pSettleStr[MCHT3_SETT_FEE];
    }
    
    memcpy(ptbl_sett_flow->sett_tp4         , ptbl_dat_flow->sett_tp4         , sizeof(ptbl_sett_flow->sett_tp4         )-1);
    memcpy(ptbl_sett_flow->mcht_no4         , ptbl_dat_flow->mcht_no4         , sizeof(ptbl_sett_flow->mcht_no4         )-1);
    
    
    ptbl_sett_flow->mcht_rate4 = pSettleStr[MCHT4_PCE_AMT];
    if(c_d_flag != TXN_DEBIT)
    {
         ptbl_sett_flow->debt_at_mcht_no4  =pSettleStr[MCHT4_SETT_AMT];
         ptbl_sett_flow->cret_fee_mcht_no4 =pSettleStr[MCHT4_SETT_FEE];
    }else{
        ptbl_sett_flow->cret_at_mcht_no4  =pSettleStr[MCHT4_SETT_AMT];
        ptbl_sett_flow->debt_fee_mcht_no4 =pSettleStr[MCHT4_SETT_FEE];
    }
    memcpy(ptbl_sett_flow->sett_tp5         , ptbl_dat_flow->sett_tp5         , sizeof(ptbl_sett_flow->sett_tp5         )-1);
    memcpy(ptbl_sett_flow->mcht_no5         , ptbl_dat_flow->mcht_no5         , sizeof(ptbl_sett_flow->mcht_no5         )-1);
    
    
    ptbl_sett_flow->mcht_rate5 = pSettleStr[MCHT5_PCE_AMT];
    if(c_d_flag != TXN_DEBIT)
    {
        ptbl_sett_flow->debt_at_mcht_no5  =pSettleStr[MCHT5_SETT_AMT];
        ptbl_sett_flow->cret_fee_mcht_no5 =pSettleStr[MCHT5_SETT_FEE];
    }else{
        ptbl_sett_flow->cret_at_mcht_no5  =pSettleStr[MCHT5_SETT_AMT];
        ptbl_sett_flow->debt_fee_mcht_no5 =pSettleStr[MCHT5_SETT_FEE];
    }
    memcpy(ptbl_sett_flow->sett_tp6         , ptbl_dat_flow->sett_tp6         , sizeof(ptbl_sett_flow->sett_tp6         )-1);
    memcpy(ptbl_sett_flow->mcht_no6         , ptbl_dat_flow->mcht_no6         , sizeof(ptbl_sett_flow->mcht_no6         )-1);    
    
    ptbl_sett_flow->mcht_rate6 = pSettleStr[MCHT6_PCE_AMT];
    if(c_d_flag != TXN_DEBIT)
    {
        ptbl_sett_flow->debt_at_mcht_no6  =pSettleStr[MCHT6_SETT_AMT];
        ptbl_sett_flow->cret_fee_mcht_no6 =pSettleStr[MCHT6_SETT_FEE];
    }else{
        ptbl_sett_flow->cret_at_mcht_no6  =pSettleStr[MCHT6_SETT_AMT];
        ptbl_sett_flow->debt_fee_mcht_no6 =pSettleStr[MCHT6_SETT_FEE];
    }
    memcpy(ptbl_sett_flow->sett_tp7         , ptbl_dat_flow->sett_tp7         , sizeof(ptbl_sett_flow->sett_tp7         )-1);
    memcpy(ptbl_sett_flow->mcht_no7         , ptbl_dat_flow->mcht_no7         , sizeof(ptbl_sett_flow->mcht_no7         )-1);
    memcpy(ptbl_sett_flow->mcht_role7       ,  " "     , 1);
    
    ptbl_sett_flow->mcht_rate7 = pSettleStr[MCHT7_PCE_AMT];    
    if(c_d_flag != TXN_DEBIT)
    {
        ptbl_sett_flow->debt_at_mcht_no7 =pSettleStr[MCHT7_SETT_AMT];
        ptbl_sett_flow->cret_fee_mcht_no7=pSettleStr[MCHT7_SETT_FEE];
    }else{
        ptbl_sett_flow->cret_at_mcht_no7 =pSettleStr[MCHT7_SETT_AMT];
        ptbl_sett_flow->debt_fee_mcht_no7=pSettleStr[MCHT7_SETT_FEE];
    }
    
    memcpy(ptbl_sett_flow->sett_tp8         , ptbl_dat_flow->sett_tp8         , sizeof(ptbl_sett_flow->sett_tp8         )-1);
    memcpy(ptbl_sett_flow->mcht_no8         , ptbl_dat_flow->mcht_no8         , sizeof(ptbl_sett_flow->mcht_no8         )-1);
    memcpy(ptbl_sett_flow->mcht_role8       ,  " "    , 1);
    
    ptbl_sett_flow->mcht_rate8 = pSettleStr[MCHT8_PCE_AMT];
    if(c_d_flag != TXN_DEBIT)
    {
        ptbl_sett_flow->debt_at_mcht_no8 =pSettleStr[MCHT8_SETT_AMT];
        ptbl_sett_flow->cret_fee_mcht_no8=pSettleStr[MCHT8_SETT_FEE];
    }else{
        ptbl_sett_flow->cret_at_mcht_no8 =pSettleStr[MCHT8_SETT_AMT];
        ptbl_sett_flow->debt_fee_mcht_no8=pSettleStr[MCHT8_SETT_FEE];
    }
    
    memcpy(ptbl_sett_flow->sett_tp9         , ptbl_dat_flow->sett_tp9         , sizeof(ptbl_sett_flow->sett_tp9         )-1);
    memcpy(ptbl_sett_flow->mcht_no9         , ptbl_dat_flow->mcht_no9         , sizeof(ptbl_sett_flow->mcht_no9         )-1);
    memcpy(ptbl_sett_flow->mcht_role9       ,  " "   , 1);
    
    ptbl_sett_flow->mcht_rate9 = pSettleStr[MCHT9_PCE_AMT];
    if(c_d_flag != TXN_DEBIT)
    {
        ptbl_sett_flow->debt_at_mcht_no9 =pSettleStr[MCHT9_SETT_AMT];
        ptbl_sett_flow->cret_fee_mcht_no9=pSettleStr[MCHT9_SETT_FEE];
    }else{
        ptbl_sett_flow->cret_at_mcht_no9 =pSettleStr[MCHT9_SETT_AMT];
        ptbl_sett_flow->debt_fee_mcht_no9=pSettleStr[MCHT9_SETT_FEE];
    }
    
    /*登记活跃商户表*/
    iRet = InsertActiveMcht(ptbl_dat_flow);
    if(iRet)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "InsertActiveMcht error, %d.", iRet);
        return -1;
    }
    
    memcpy(ptbl_sett_flow->up_mcht_no1,  up_mcht_no, sizeof(ptbl_sett_flow->up_mcht_no1)-1);
    memcpy(ptbl_sett_flow->acct_name,  ptbl_dat_flow->acct_name, sizeof(ptbl_sett_flow->acct_name)-1);
    CommonRTrim(ptbl_sett_flow->acct_name);
    memcpy(ptbl_sett_flow->acct_statu,  "N", 1);
    memcpy(ptbl_sett_flow->status,  "0", 1);
    memcpy(ptbl_sett_flow->remark,  "-", 1);

    return 0;
}

/************************************************************************************/
/*   函数名：                       CaclStep                                        */
/************************************************************************************/
/*   声明  ： void  CaclStep(double *result, double *operand1, char operator1,      */
/*                           double *operand2, char operator2, double *operand3)    */
/*   功能  ： 根据计算公式进行计算                                                  */
/*   参数  ：                                                                       */
/*     输入：                                                                       */
/*      operand1              - 操作数1                                             */
/*      operator1             - 操作符1                                             */
/*      operand2              - 操作数2                                             */
/*      operator2             - 操作符2                                             */
/*      operand3              - 操作数3                                             */
/*     输出：                                                                       */
/*       result               - 计算结果                                            */
/*                                                                                  */
/*  返回值 ：                                                                       */
/*         无                                                                       */
/************************************************************************************/
static void  CaclStep(double *result, double *operand1, char operator1, double *operand2, char operator2, double *operand3)
{
    switch (operator1)
    {
        case '*':
            *result = floor(*operand1 * *operand2 + 0.50001);
            break;
            
        case '/':
            *result = floor(*operand1 / *operand2 + 0.50001);
            break;
            
        case '+':
            *result = *operand1 + *operand2;
            break;
            
        case '-':
            *result = *operand1 - *operand2;
            break;
            
        case '>':
            if (*operand1 > *operand2)
            {
                *result = *operand2;
            }
            break;
            
        case '<':
            if (*operand1 < *operand2)
            {
                *result = *operand2;
            }
            break;
            
        case '=':
            *result = *operand1;
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "test[%c][%lf][%lf]", operator1, *result, *operand1);
            break;
            
        default:
            break;
    }
    switch (operator2)
    {
        case '*':
            *result = floor(*result * *operand3 + 0.50001);
            break;
            
        case '/':
            *result = floor(*result / *operand3 + 0.50001);
            break;
            
        case '+':
            *result = *result + *operand3;
            break;
            
        case '-':
            *result = *result - *operand3;
            break;
            
        case '>':
            if (*result > *operand3)
            {
                *result = *operand3;
            }
            break;
            
        case '<':
            if (*result < *operand3)
            {
                *result = *operand3;
            }
            break;
            
        case '=':
            *result = *operand3;
            break;
            
        default:
            break;
    }
}

int InsertActiveMcht(ht_tbl_dat_flow_def *ptbl_dat_flow)
{
    int iRet = 0;
    ht_tbl_active_mcht_inf_def tbl_active_mcht_inf;
    ht_tbl_mcht_inf_def tbl_mcht_inf;
    memset(&tbl_active_mcht_inf, 0, sizeof(tbl_active_mcht_inf)); 
    

    memcpy(tbl_active_mcht_inf.inter_brh_code, ptbl_dat_flow->inter_brh_code, sizeof(tbl_active_mcht_inf.inter_brh_code)-1);
    memcpy(tbl_active_mcht_inf.sett_date        , ptbl_dat_flow->sett_date        , sizeof(tbl_active_mcht_inf.sett_date        )-1);
    tbl_active_mcht_inf.trans_at = atof(ptbl_dat_flow->trans_at);
    tbl_active_mcht_inf.trans_num = 1;    

    CommonRTrim(ptbl_dat_flow->mcht_no1);      
    memcpy(tbl_active_mcht_inf.mcht_no         , ptbl_dat_flow->mcht_no1         , sizeof(tbl_active_mcht_inf.mcht_no         )-1);
    
    memset(&tbl_mcht_inf, 0x00, sizeof(tbl_mcht_inf));
    memcpy(tbl_mcht_inf.mcht_no, tbl_active_mcht_inf.mcht_no, sizeof(tbl_mcht_inf.mcht_no)-1);
    CommonRTrim(tbl_mcht_inf.mcht_no);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_no[%s]",tbl_mcht_inf.mcht_no);
    iRet = DbsTblMchtInf(DBS_SELECT, &tbl_mcht_inf, 0, 0);
    if (iRet != 0 && iRet != DBS_NOTFOUND)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblMchtInf DBS_SELECT error, %d,mcht_no[%s].", iRet, tbl_mcht_inf.mcht_no);
        return -1;
    }
    memcpy(tbl_active_mcht_inf.mcht_nm, tbl_mcht_inf.mcht_nm, sizeof(tbl_active_mcht_inf.mcht_nm)-1);
    CommonRTrim(tbl_active_mcht_inf.mcht_nm);
    /*登记商户活跃表*/
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "ptbl_dat_flow[%s].", ptbl_dat_flow->plat_key);
    iRet = DbsTblActiveMchtInfTxn(DBS_INSERT, &tbl_active_mcht_inf, 0, 0);
    if (iRet != 0)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblActiveMchtInfTxn DBS_INSERT error, %d,mcht_no[%s].", iRet, tbl_active_mcht_inf.mcht_no);
        return -1;
    }
    
    return 0;    
}
/****************************** end of Task_4101.c ******************************/

