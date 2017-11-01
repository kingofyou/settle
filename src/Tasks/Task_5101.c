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
 *  文 件 名: Task_5101.c
 *  功    能: 清分
 *  编程人员: HELU
 *  开发时间: 2016-12-23
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
char g_plat_mcht_no[32+1];

/*参数mcht_no 结算商户*/
static int in_sett_settmcht_rslt(ht_tbl_sett_flow_def *ptbl_sett_flow, int flag, char* mcht_no)
{
    int iRet;
    int iYear, iMonth, iDay, iWeek;
    
    char cYear[5];
    char cMonth[3];
    char cDay[3];
    
    ht_tbl_sett_rslt_def tbl_sett_rslt;
    ht_tbl_mcht_inf_def tbl_mcht_inf;
    ht_tbl_ext_mcht_inf_def  tbl_ext_mcht_inf;    
    tp_operater_acc_def tp_operater_acc_info;

    memset(&tbl_sett_rslt, 0x00, sizeof(tbl_sett_rslt));
    memcpy(tbl_sett_rslt.inter_brh_code, ptbl_sett_flow->inter_brh_code, sizeof(tbl_sett_rslt.inter_brh_code)-1);
    memcpy(tbl_sett_rslt.sett_date, ptbl_sett_flow->sett_date, sizeof(tbl_sett_rslt.sett_date)-1);
    memcpy(tbl_sett_rslt.batch_no, ptbl_sett_flow->batch_no, sizeof(tbl_sett_rslt.batch_no)-1);
    memcpy(tbl_sett_rslt.bus_tp, ptbl_sett_flow->bus_tp, sizeof(tbl_sett_rslt.bus_tp)-1);
    memcpy(tbl_sett_rslt.trans_tp, ptbl_sett_flow->trans_tp, sizeof(tbl_sett_rslt.trans_tp)-1);
    memcpy(tbl_sett_rslt.mcht_role, ptbl_sett_flow->mcht_role1, sizeof(tbl_sett_rslt.mcht_role)-1);
    memcpy(tbl_sett_rslt.mcht_no, ptbl_sett_flow->mcht_no1, sizeof(tbl_sett_rslt.mcht_no)-1);
    memcpy(tbl_sett_rslt.acct_type, ptbl_sett_flow->acct_type, sizeof(tbl_sett_rslt.acct_type)-1);

    memset(&tbl_mcht_inf, 0x00, sizeof(tbl_mcht_inf));

    memcpy(tbl_mcht_inf.mcht_no, mcht_no, sizeof(tbl_mcht_inf.mcht_no)-1);
    
    CommonRTrim(tbl_mcht_inf.mcht_no);
    if(strlen(tbl_mcht_inf.mcht_no)==0) return 0;
    iRet = DbsTblMchtInf(DBS_SELECT, &tbl_mcht_inf, 0, 0);
    if (iRet != 0)
    {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblMchtInf DBS_SELECT error, %d,mcht_no[%s].", iRet, tbl_mcht_inf.mcht_no);
            return -1;
    }    
    CommonRTrim(tbl_mcht_inf.businuss_brh_no);
    memset(g_plat_mcht_no, 0, sizeof(g_plat_mcht_no));
    strcpy(g_plat_mcht_no, tbl_mcht_inf.businuss_brh_no);
    
    CommonRTrim(tbl_mcht_inf.mcht_sett_bank);
    CommonRTrim(tbl_mcht_inf.mcht_sett_bank_nm);
    CommonRTrim(tbl_mcht_inf.mcht_sett_acct);
    CommonRTrim(tbl_mcht_inf.mcht_sett_acct_nm);

    CommonRTrim(tbl_mcht_inf.mcht_debit_bank);
    CommonRTrim(tbl_mcht_inf.mcht_debit_bank_nm);
    CommonRTrim(tbl_mcht_inf.mcht_debit_acct);
    CommonRTrim(tbl_mcht_inf.mcht_debit_acct_nm);

    memcpy(tbl_sett_rslt.sett_md, tbl_mcht_inf.sett_md, sizeof(tbl_sett_rslt.sett_md)-1);
    if(flag == 1)
    {
        memcpy(tbl_sett_rslt.mcht_sett_acct_type, tbl_mcht_inf.mcht_sett_acct_type, sizeof(tbl_sett_rslt.mcht_sett_acct_type)-1);
        memcpy(tbl_sett_rslt.mcht_sett_bank, tbl_mcht_inf.mcht_sett_bank, sizeof(tbl_sett_rslt.mcht_sett_bank)-1);
        memcpy(tbl_sett_rslt.mcht_sett_bank_nm, tbl_mcht_inf.mcht_sett_bank_nm, sizeof(tbl_sett_rslt.mcht_sett_bank_nm)-1);
        memcpy(tbl_sett_rslt.mcht_sett_acct, tbl_mcht_inf.mcht_sett_acct, sizeof(tbl_sett_rslt.mcht_sett_acct)-1);
        memcpy(tbl_sett_rslt.mcht_sett_acct_nm, tbl_mcht_inf.mcht_sett_acct_nm, sizeof(tbl_sett_rslt.mcht_sett_acct_nm)-1);
        memcpy(tbl_sett_rslt.mcht_sett_xingmin_acct, tbl_mcht_inf.xingmin_pay_acct, sizeof(tbl_sett_rslt.mcht_sett_xingmin_acct)-1);

        memcpy(tbl_sett_rslt.mcht_debit_acct_type, tbl_mcht_inf.mcht_debit_acct_type, sizeof(tbl_sett_rslt.mcht_debit_acct_type)-1);
        memcpy(tbl_sett_rslt.mcht_debit_bank, tbl_mcht_inf.mcht_debit_bank, sizeof(tbl_sett_rslt.mcht_debit_bank)-1);
        memcpy(tbl_sett_rslt.mcht_debit_bank_nm, tbl_mcht_inf.mcht_debit_bank_nm, sizeof(tbl_sett_rslt.mcht_debit_bank_nm)-1);
        memcpy(tbl_sett_rslt.mcht_debit_acct, tbl_mcht_inf.mcht_debit_acct, sizeof(tbl_sett_rslt.mcht_debit_acct)-1);
        memcpy(tbl_sett_rslt.mcht_debit_acct_nm, tbl_mcht_inf.mcht_debit_acct_nm, sizeof(tbl_sett_rslt.mcht_debit_acct_nm)-1);
    }  
    else if(flag == 2)
    {
        memcpy(tbl_sett_rslt.mcht_sett_acct_type, tbl_mcht_inf.spl_acct_type1, sizeof(tbl_sett_rslt.acct_type)-1);
        memcpy(tbl_sett_rslt.mcht_sett_bank, tbl_mcht_inf.spl_acct_bank1, sizeof(tbl_sett_rslt.mcht_sett_bank)-1);        
        memcpy(tbl_sett_rslt.mcht_sett_bank_nm, tbl_mcht_inf.spl_acct_banknm1, sizeof(tbl_sett_rslt.mcht_sett_bank_nm)-1);
        memcpy(tbl_sett_rslt.mcht_sett_acct, tbl_mcht_inf.spl_acct_no1, sizeof(tbl_sett_rslt.mcht_sett_acct)-1);
        memcpy(tbl_sett_rslt.mcht_sett_acct_nm, tbl_mcht_inf.spl_acct_nm1, sizeof(tbl_sett_rslt.mcht_sett_acct_nm)-1);
    }
    else if(flag == 3)
    {
        memcpy(tbl_sett_rslt.mcht_sett_acct_type, tbl_mcht_inf.spl_acct_type2, sizeof(tbl_sett_rslt.acct_type)-1);
        memcpy(tbl_sett_rslt.mcht_sett_bank, tbl_mcht_inf.spl_acct_bank2, sizeof(tbl_sett_rslt.mcht_sett_bank)-1);        
        memcpy(tbl_sett_rslt.mcht_sett_bank_nm, tbl_mcht_inf.spl_acct_banknm2, sizeof(tbl_sett_rslt.mcht_sett_bank_nm)-1);
        memcpy(tbl_sett_rslt.mcht_sett_acct, tbl_mcht_inf.spl_acct_no2, sizeof(tbl_sett_rslt.mcht_sett_acct)-1);
        memcpy(tbl_sett_rslt.mcht_sett_acct_nm, tbl_mcht_inf.spl_acct_nm2, sizeof(tbl_sett_rslt.mcht_sett_acct_nm)-1);
    }
    else if(flag == 4)
    {
        memcpy(tbl_sett_rslt.mcht_sett_acct_type, tbl_mcht_inf.spl_acct_type3, sizeof(tbl_sett_rslt.acct_type)-1);
        memcpy(tbl_sett_rslt.mcht_sett_bank, tbl_mcht_inf.spl_acct_bank3, sizeof(tbl_sett_rslt.mcht_sett_bank)-1);        
        memcpy(tbl_sett_rslt.mcht_sett_bank_nm, tbl_mcht_inf.spl_acct_banknm3, sizeof(tbl_sett_rslt.mcht_sett_bank_nm)-1);
        memcpy(tbl_sett_rslt.mcht_sett_acct, tbl_mcht_inf.spl_acct_no3, sizeof(tbl_sett_rslt.mcht_sett_acct)-1);
        memcpy(tbl_sett_rslt.mcht_sett_acct_nm, tbl_mcht_inf.spl_acct_nm3, sizeof(tbl_sett_rslt.mcht_sett_acct_nm)-1);
    }
    else if(flag == 5)
    {
        memcpy(tbl_sett_rslt.mcht_sett_acct_type, tbl_mcht_inf.spl_acct_type4, sizeof(tbl_sett_rslt.acct_type)-1);
        memcpy(tbl_sett_rslt.mcht_sett_bank, tbl_mcht_inf.spl_acct_bank4, sizeof(tbl_sett_rslt.mcht_sett_bank)-1);        
        memcpy(tbl_sett_rslt.mcht_sett_bank_nm, tbl_mcht_inf.spl_acct_banknm4, sizeof(tbl_sett_rslt.mcht_sett_bank_nm)-1);
        memcpy(tbl_sett_rslt.mcht_sett_acct, tbl_mcht_inf.spl_acct_no4, sizeof(tbl_sett_rslt.mcht_sett_acct)-1);
        memcpy(tbl_sett_rslt.mcht_sett_acct_nm, tbl_mcht_inf.spl_acct_nm4, sizeof(tbl_sett_rslt.mcht_sett_acct_nm)-1);
    }
    else if(flag == 6)
    {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_no: [%s][%s].", tbl_sett_rslt.mcht_no, tbl_sett_rslt.mcht_role );
    }
    else if(flag == 10)
    {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_no: [%s][%s].", tbl_sett_rslt.mcht_no, tbl_sett_rslt.mcht_role );
    }
    else
    {
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_acct: [%s].", tbl_sett_rslt.mcht_sett_acct);
        return 0;
    }
    
    /*过滤没有配置的分润机构*/
    CommonRTrim(tbl_sett_rslt.mcht_sett_acct);
    if(strlen(tbl_sett_rslt.mcht_sett_acct) <= 0 && flag != 10 && flag != 6)
    {
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_no: [%s][%s].", tbl_sett_rslt.mcht_no, tbl_sett_rslt.mcht_role );
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "mcht_sett_acct null: [%s].", tbl_sett_rslt.mcht_sett_acct);
        return 0;
    }
     
    if(memcmp(tbl_mcht_inf.js_type, "01", 2)==0)/*01: 按日顺延*/
    {
        if(memcmp(tbl_mcht_inf.clear_cycle, "w", 1)==0)/*w: 周*/
        {
            CountDate(tbl_sett_rslt.sett_date, 7-1, tbl_sett_rslt.acct_date);
        }
         else  if(memcmp(tbl_mcht_inf.clear_cycle, "m", 1)==0)  /*m: 月*/
         {
            CountDate(tbl_sett_rslt.sett_date, 30-1, tbl_sett_rslt.acct_date);
         }
         else
         {
            CountDate(tbl_sett_rslt.sett_date, atoi(tbl_mcht_inf.clear_cycle)-1, tbl_sett_rslt.acct_date);
         }
    }
    else  if(memcmp(tbl_mcht_inf.js_type, "02", 2)==0)  /*01: 周期顺延*/
    {
        memset(cDay, 0x00, sizeof(cDay));
        memset(cMonth, 0x00, sizeof(cMonth));
        memset(cYear, 0x00, sizeof(cYear));
        memcpy(cDay, tbl_sett_rslt.sett_date+6, 2);
        memcpy(cMonth, tbl_sett_rslt.sett_date+4, 2);
        memcpy(cYear, tbl_sett_rslt.sett_date, 4);
        iYear = atoi(cYear);
        iMonth=atoi(cMonth);
        iDay=atoi(cDay);
        
        if(memcmp(tbl_mcht_inf.clear_cycle, "w", 1)==0)/*w: 周*/
        {
            /*星期的计算公式*/
            if(iMonth==1||iMonth==2)
            {
                iMonth+=12;
                iYear--;
            }
            /*星期的计算公式，iWeek就是星期几，如1:星期一，7:星期天*/
            iWeek = (iDay+2*iMonth+3*(iMonth+1)/5+iYear+iYear/4-iYear/100+iYear/400)%7 + 1;
            
            CountDate(tbl_sett_rslt.sett_date, (8-iWeek), tbl_sett_rslt.acct_date);
            
        }
        else  if(memcmp(tbl_mcht_inf.clear_cycle, "m", 1)==0)  /*m: 月*/
        {
            /*获取当月最后一天*/
            iDay = RMONTHDAYS(iMonth, iYear);
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "iYear[%d],iMonth[%d]iDay[%d].", iYear,iMonth,iDay);
            sprintf(tbl_sett_rslt.acct_date, "%04.4s%02.2s%0d", cYear, cMonth,  iDay);
        }
    }
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date[%s][%s].", tbl_sett_rslt.sett_date, tbl_sett_rslt.acct_date);

    memset(&tbl_ext_mcht_inf, 0, sizeof(tbl_ext_mcht_inf));
    strcpy(tbl_ext_mcht_inf.ext_mch_no, ptbl_sett_flow->ext_mcht_no);
    CommonRTrim(tbl_ext_mcht_inf.ext_mch_no);
    iRet = DbsTblExtMchtInfTxn(DBS_SELECT, &tbl_ext_mcht_inf, 0, 0);
    if (iRet != 0)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblExtMchtInfTxn DBS_SELECT error, %d,mcht_no[%s].", iRet, tbl_ext_mcht_inf.ext_mch_no);
        return -1;
    }
    
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "bus_tp[%s].", ptbl_sett_flow->bus_tp+4);
    if(memcmp(ptbl_sett_flow->bus_tp+4, BT_SETT_TRAN_D, BT_SETT_TRAN_LEN) == 0)
    {
        /*直连模式*/
		CommonRTrim(tbl_ext_mcht_inf.ext_mch_acct_no);
        CommonRTrim(tbl_ext_mcht_inf.ext_mch_acct_banknm);
        CommonRTrim(tbl_ext_mcht_inf.ext_mch_acct_bank);
        CommonRTrim(tbl_ext_mcht_inf.ext_mch_acct_nm);
		
        CommonRTrim(tbl_ext_mcht_inf.spl_bank);
        CommonRTrim(tbl_ext_mcht_inf.spl_acct);
        CommonRTrim(tbl_ext_mcht_inf.spl_acct_nm);
        CommonRTrim(tbl_ext_mcht_inf.spl_acct_type);
            
        memcpy(tbl_sett_rslt.plat_debit_bank, tbl_ext_mcht_inf.spl_bank, sizeof(tbl_sett_rslt.plat_debit_bank)-1);
        strcpy(tbl_sett_rslt.plat_debit_bank_nm, "");
        memcpy(tbl_sett_rslt.plat_debit_acct, tbl_ext_mcht_inf.spl_acct, sizeof(tbl_sett_rslt.plat_debit_acct)-1);
        strcpy(tbl_sett_rslt.plat_debit_acct_nm, tbl_ext_mcht_inf.spl_acct_nm); 
        strcpy(tbl_sett_rslt.plat_debit_acct_type, tbl_ext_mcht_inf.spl_acct_type);

		memcpy(tbl_sett_rslt.plat_sett_bank, tbl_ext_mcht_inf.spl_bank, sizeof(tbl_sett_rslt.plat_sett_bank)-1);
        strcpy(tbl_sett_rslt.plat_sett_bank_nm, "");
        memcpy(tbl_sett_rslt.plat_sett_acct, tbl_ext_mcht_inf.spl_acct, sizeof(tbl_sett_rslt.plat_sett_acct)-1);
        strcpy(tbl_sett_rslt.plat_sett_acct_nm, tbl_ext_mcht_inf.spl_acct_nm); 
        strcpy(tbl_sett_rslt.plat_sett_acct_type, tbl_ext_mcht_inf.spl_acct_type);
    }
    else
    {
        CommonRTrim(tbl_ext_mcht_inf.ext_mch_acct_no);
        CommonRTrim(tbl_ext_mcht_inf.ext_mch_acct_banknm);
        CommonRTrim(tbl_ext_mcht_inf.ext_mch_acct_bank);
        CommonRTrim(tbl_ext_mcht_inf.ext_mch_acct_nm);

		CommonRTrim(tbl_ext_mcht_inf.spl_bank);
        CommonRTrim(tbl_ext_mcht_inf.spl_acct);
        CommonRTrim(tbl_ext_mcht_inf.spl_acct_nm);
        CommonRTrim(tbl_ext_mcht_inf.spl_acct_type);
            
        memcpy(tbl_sett_rslt.plat_debit_bank, tbl_ext_mcht_inf.spl_bank, sizeof(tbl_sett_rslt.plat_debit_bank)-1);
        strcpy(tbl_sett_rslt.plat_debit_bank_nm, "");
        memcpy(tbl_sett_rslt.plat_debit_acct, tbl_ext_mcht_inf.spl_acct, sizeof(tbl_sett_rslt.plat_debit_acct)-1);
        strcpy(tbl_sett_rslt.plat_debit_acct_nm, tbl_ext_mcht_inf.spl_acct_nm); 
        strcpy(tbl_sett_rslt.plat_debit_acct_type, tbl_ext_mcht_inf.spl_acct_type);

		memcpy(tbl_sett_rslt.plat_sett_bank, tbl_ext_mcht_inf.ext_mch_acct_bank, sizeof(tbl_sett_rslt.plat_sett_bank)-1);
        strcpy(tbl_sett_rslt.plat_sett_bank_nm, tbl_ext_mcht_inf.ext_mch_acct_banknm);
        memcpy(tbl_sett_rslt.plat_sett_acct, tbl_ext_mcht_inf.ext_mch_acct_no, sizeof(tbl_sett_rslt.plat_sett_acct)-1);
        strcpy(tbl_sett_rslt.plat_sett_acct_nm, tbl_ext_mcht_inf.ext_mch_acct_nm); 
        strcpy(tbl_sett_rslt.plat_sett_acct_type, tbl_ext_mcht_inf.ext_mch_acct_type);
    }

    if(memcmp(ptbl_sett_flow->bus_tp, BT_BUS_TP_CORE, BT_BUS_TP_LEN) == 0)
    {
        /*分润支付取外部商户号的平台账号信息*/
        memset(&tp_operater_acc_info, 0, sizeof(tp_operater_acc_info));
        memcpy(tp_operater_acc_info.core_acc_type, BT_ACC_TYPE_PLAT_DEP, BT_ACC_TYPE_PLAT_LEN);
        /*星民易付渠道从TP_OPERATER_ACC_INFO表取运营商支付账号信息*/        
        iRet = DbsTblOperaterAcctInf(DBS_SELECT, &tp_operater_acc_info);
        if (iRet != 0)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblOperaterAcctInf DBS_SELECT error, %d,core_acc_type[%s].", iRet, tp_operater_acc_info.core_acc_type);
            return -1;
        }
        CommonRTrim(tp_operater_acc_info.opr_acc_no);
        memcpy(tbl_sett_rslt.plat_debit_xingmin_acct, tp_operater_acc_info.opr_acc_no, sizeof(tbl_sett_rslt.plat_debit_acct)-1);
    }
    
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_debit_bank_nm: [%s].", tbl_sett_rslt.plat_debit_bank_nm);
    
    tbl_sett_rslt.debt_at= ptbl_sett_flow->debt_at_mcht_no1;
    tbl_sett_rslt.cret_at= ptbl_sett_flow->cret_at_mcht_no1;
    
    tbl_sett_rslt.share_profit= ptbl_sett_flow->share_profit_mcht_no1;
    tbl_sett_rslt.mcht_rate = ptbl_sett_flow->mcht_rate1;

    strncpy(tbl_sett_rslt.trans_date, ptbl_sett_flow->trans_date, sizeof(tbl_sett_rslt.trans_date)-1);
    strncpy(tbl_sett_rslt.trans_time, ptbl_sett_flow->trans_time, sizeof(tbl_sett_rslt.trans_time)-1);
    strncpy(tbl_sett_rslt.remark, ptbl_sett_flow->remark, sizeof(tbl_sett_rslt.remark)-1);
    CommonRTrim(tbl_sett_rslt.mcht_sett_bank);
    CommonRTrim(tbl_sett_rslt.mcht_sett_bank_nm);
    CommonRTrim(tbl_sett_rslt.mcht_sett_acct);
    CommonRTrim(tbl_sett_rslt.remark);
    CommonRTrim(tbl_sett_rslt.mcht_sett_acct_nm);
    CommonRTrim(tbl_sett_rslt.plat_debit_bank);
    CommonRTrim(tbl_sett_rslt.plat_debit_acct);
    CommonRTrim(tbl_sett_rslt.plat_debit_acct_nm);
    CommonRTrim(tbl_sett_rslt.mcht_no);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_no: [%s][%s][%lf].", tbl_sett_rslt.mcht_no, tbl_sett_rslt.mcht_role ,tbl_sett_rslt.debt_at);
    iRet = DbsTblSettRslt(DBS_INSERT, &tbl_sett_rslt, 0, 0);
    if (iRet != 0)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblSettRslt DBS_INSERT error, %d.", iRet);
        return -1;
    }

    return 0;
}

static int in_sett(ht_tbl_sett_flow_def *ptbl_sett_flow)
{
    int iRet;
    ht_tbl_sett_flow_def tbl_sett_flow;
    ht_tbl_mcht_inf_def  tbl_mcht_inf;
    ht_tbl_brh_inf_def tbl_brh_inf;
    
    memset(&tbl_mcht_inf, 0x00, sizeof(tbl_mcht_inf));
    memset(&tbl_brh_inf, 0x00, sizeof(tbl_brh_inf));
    
    memset(&tbl_sett_flow, 0x00, sizeof(tbl_sett_flow));
    memcpy(&tbl_sett_flow, ptbl_sett_flow, sizeof(tbl_sett_flow));
    
    /*结算商户*/
    if(ptbl_sett_flow->share_profit_mcht_no1 > 0)
    {
        tbl_sett_flow.debt_at_mcht_no1= ptbl_sett_flow->share_profit_mcht_no1;
    }
    else if(ptbl_sett_flow->share_profit_mcht_no1 < 0)
    {
        tbl_sett_flow.cret_at_mcht_no1= ptbl_sett_flow->share_profit_mcht_no1 * -1;
    }
    iRet = in_sett_settmcht_rslt(&tbl_sett_flow, 1, ptbl_sett_flow->mcht_no1);
    if (iRet != 0)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_sett_rslt error, %d.", iRet);
        return -1;
    }

    /*分润机构1*/
    memcpy(tbl_sett_flow.mcht_role1, ptbl_sett_flow->mcht_role2, sizeof(tbl_sett_flow.mcht_role1)-1);
    memcpy(tbl_sett_flow.mcht_no1, ptbl_sett_flow->mcht_no2, sizeof(tbl_sett_flow.mcht_no2)-1);
    tbl_sett_flow.debt_at_mcht_no1 = ptbl_sett_flow->debt_at_mcht_no2;
    tbl_sett_flow.cret_at_mcht_no1 = ptbl_sett_flow->cret_at_mcht_no2;
    tbl_sett_flow.debt_fee_mcht_no1 = ptbl_sett_flow->debt_fee_mcht_no2;
    tbl_sett_flow.cret_fee_mcht_no1 = ptbl_sett_flow->cret_fee_mcht_no2;
    tbl_sett_flow.share_profit_mcht_no1 = ptbl_sett_flow->share_profit_mcht_no2;
    tbl_sett_flow.mcht_rate1 = ptbl_sett_flow->mcht_rate2;
    if(ptbl_sett_flow->share_profit_mcht_no2 > 0)
    {
        tbl_sett_flow.debt_at_mcht_no1= ptbl_sett_flow->share_profit_mcht_no2;
    }
    else if(ptbl_sett_flow->share_profit_mcht_no2 < 0)
    {
        tbl_sett_flow.cret_at_mcht_no1= ptbl_sett_flow->share_profit_mcht_no2 * -1;
    }
    iRet = in_sett_settmcht_rslt(&tbl_sett_flow, 2, ptbl_sett_flow->mcht_no1);
    if (iRet != 0)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_sett_rslt error, %d.", iRet);
        return -1;
    }
    /*分润机构2*/
    memcpy(tbl_sett_flow.mcht_role1, ptbl_sett_flow->mcht_role3, sizeof(tbl_sett_flow.mcht_role1)-1);
    memcpy(tbl_sett_flow.mcht_no1, ptbl_sett_flow->mcht_no3, sizeof(tbl_sett_flow.mcht_no1)-1);
    tbl_sett_flow.debt_at_mcht_no1 = ptbl_sett_flow->debt_at_mcht_no3;
    tbl_sett_flow.cret_at_mcht_no1 = ptbl_sett_flow->cret_at_mcht_no3;
    tbl_sett_flow.debt_fee_mcht_no1 = ptbl_sett_flow->debt_fee_mcht_no3;
    tbl_sett_flow.cret_fee_mcht_no1 = ptbl_sett_flow->cret_fee_mcht_no3;
    tbl_sett_flow.share_profit_mcht_no1 = ptbl_sett_flow->share_profit_mcht_no3;
    tbl_sett_flow.mcht_rate1 = ptbl_sett_flow->mcht_rate3;
    if(ptbl_sett_flow->share_profit_mcht_no3 > 0)
    {
        tbl_sett_flow.debt_at_mcht_no1= ptbl_sett_flow->share_profit_mcht_no3;
    }
    else if(ptbl_sett_flow->share_profit_mcht_no3 < 0)
    {
        tbl_sett_flow.cret_at_mcht_no1= ptbl_sett_flow->share_profit_mcht_no3 * -1;
    }
    iRet = in_sett_settmcht_rslt(&tbl_sett_flow, 3, ptbl_sett_flow->mcht_no1);
    if (iRet != 0)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_sett_rslt error, %d.", iRet);
        return -1;
    }

    /*分润机构3*/
    memcpy(tbl_sett_flow.mcht_role1, ptbl_sett_flow->mcht_role4, sizeof(tbl_sett_flow.mcht_role1)-1);
    memcpy(tbl_sett_flow.mcht_no1, ptbl_sett_flow->mcht_no4, sizeof(tbl_sett_flow.mcht_no1)-1);
    tbl_sett_flow.debt_at_mcht_no1 = ptbl_sett_flow->debt_at_mcht_no4;
    tbl_sett_flow.cret_at_mcht_no1 = ptbl_sett_flow->cret_at_mcht_no4;
    tbl_sett_flow.debt_fee_mcht_no1 = ptbl_sett_flow->debt_fee_mcht_no4;
    tbl_sett_flow.cret_fee_mcht_no1 = ptbl_sett_flow->cret_fee_mcht_no4;
    tbl_sett_flow.share_profit_mcht_no1 = ptbl_sett_flow->share_profit_mcht_no4;
    tbl_sett_flow.mcht_rate1 = ptbl_sett_flow->mcht_rate4;
    if(ptbl_sett_flow->share_profit_mcht_no4 > 0)
    {
        tbl_sett_flow.debt_at_mcht_no1= ptbl_sett_flow->share_profit_mcht_no4;
    }
    else if(ptbl_sett_flow->share_profit_mcht_no4 < 0)
    {
        tbl_sett_flow.cret_at_mcht_no1= ptbl_sett_flow->share_profit_mcht_no4 * -1;
    }
    iRet = in_sett_settmcht_rslt(&tbl_sett_flow, 4, ptbl_sett_flow->mcht_no1);
    if (iRet != 0)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_sett_rslt error, %d.", iRet);
        return -1;
    }

    /*分润机构3*/
    memcpy(tbl_sett_flow.mcht_role1, ptbl_sett_flow->mcht_role5, sizeof(tbl_sett_flow.mcht_role1)-1);
    memcpy(tbl_sett_flow.mcht_no1, ptbl_sett_flow->mcht_no5, sizeof(tbl_sett_flow.mcht_no1)-1);
    tbl_sett_flow.debt_at_mcht_no1 = ptbl_sett_flow->debt_at_mcht_no5;
    tbl_sett_flow.cret_at_mcht_no1 = ptbl_sett_flow->cret_at_mcht_no5;
    tbl_sett_flow.debt_fee_mcht_no1 = ptbl_sett_flow->debt_fee_mcht_no5;
    tbl_sett_flow.cret_fee_mcht_no1 = ptbl_sett_flow->cret_fee_mcht_no5;
    tbl_sett_flow.share_profit_mcht_no1 = ptbl_sett_flow->share_profit_mcht_no5;
    tbl_sett_flow.mcht_rate1 = ptbl_sett_flow->mcht_rate5;
    if(ptbl_sett_flow->share_profit_mcht_no5 > 0)
    {
        tbl_sett_flow.debt_at_mcht_no1= ptbl_sett_flow->share_profit_mcht_no5;
    }
    else if(ptbl_sett_flow->share_profit_mcht_no5 < 0)
    {
        tbl_sett_flow.cret_at_mcht_no1= ptbl_sett_flow->share_profit_mcht_no5 * -1;
    }
    iRet = in_sett_settmcht_rslt(&tbl_sett_flow, 5, ptbl_sett_flow->mcht_no1);
    if (iRet != 0)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_sett_rslt error, %d.", iRet);
        return -1;
    }

    /*支付机构*/
    memcpy(tbl_sett_flow.mcht_role1, ptbl_sett_flow->mcht_role6, sizeof(tbl_sett_flow.mcht_role1)-1);
    memcpy(tbl_sett_flow.sett_tp1, ptbl_sett_flow->sett_tp6, sizeof(tbl_sett_flow.sett_tp1)-1);
    memcpy(tbl_sett_flow.mcht_no1, ptbl_sett_flow->mcht_no6, sizeof(tbl_sett_flow.mcht_no1)-1);
    tbl_sett_flow.debt_at_mcht_no1 = ptbl_sett_flow->debt_at_mcht_no6;
    tbl_sett_flow.cret_at_mcht_no1 = ptbl_sett_flow->cret_at_mcht_no6;
    tbl_sett_flow.debt_fee_mcht_no1 = ptbl_sett_flow->debt_fee_mcht_no6;
    tbl_sett_flow.cret_fee_mcht_no1 = ptbl_sett_flow->cret_fee_mcht_no6;
    tbl_sett_flow.share_profit_mcht_no1 = ptbl_sett_flow->share_profit_mcht_no6;
    tbl_sett_flow.mcht_rate1 = ptbl_sett_flow->mcht_rate6;
    if(ptbl_sett_flow->trans_at2 > 0)
    {
        tbl_sett_flow.debt_at_mcht_no1= ptbl_sett_flow->trans_at2;
    }
    else if(ptbl_sett_flow->trans_at2 < 0)
    {
        tbl_sett_flow.cret_at_mcht_no1= ptbl_sett_flow->trans_at2 * -1;
    }
    else
    {
        tbl_sett_flow.debt_at_mcht_no1= 0.0;
        tbl_sett_flow.cret_at_mcht_no1= 0.0;       
    }
    iRet = in_sett_settmcht_rslt(&tbl_sett_flow,6, ptbl_sett_flow->mcht_no1);
    if (iRet != 0)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_sett_rslt error, %d.", iRet);
        return -1;
    }

    /*合作方7*/
    memcpy(tbl_sett_flow.mcht_role1, ptbl_sett_flow->mcht_role7, sizeof(tbl_sett_flow.mcht_role1)-1);
    memcpy(tbl_sett_flow.mcht_no1, ptbl_sett_flow->mcht_no7, sizeof(tbl_sett_flow.mcht_no1)-1);
    tbl_sett_flow.debt_at_mcht_no1 = ptbl_sett_flow->debt_at_mcht_no7;
    tbl_sett_flow.cret_at_mcht_no1 = ptbl_sett_flow->cret_at_mcht_no7;
    tbl_sett_flow.debt_fee_mcht_no1 = ptbl_sett_flow->debt_fee_mcht_no7;
    tbl_sett_flow.cret_fee_mcht_no1 = ptbl_sett_flow->cret_fee_mcht_no7;
    tbl_sett_flow.share_profit_mcht_no1 = ptbl_sett_flow->share_profit_mcht_no7;
    tbl_sett_flow.mcht_rate1 = ptbl_sett_flow->mcht_rate7;
    if(ptbl_sett_flow->share_profit_mcht_no7 > 0)
    {
        tbl_sett_flow.debt_at_mcht_no1= ptbl_sett_flow->share_profit_mcht_no7;
    }
    else if(ptbl_sett_flow->share_profit_mcht_no7 < 0)
    {
        tbl_sett_flow.cret_at_mcht_no1= ptbl_sett_flow->share_profit_mcht_no7 * -1;
    }
    iRet = in_sett_settmcht_rslt(&tbl_sett_flow, 7, ptbl_sett_flow->mcht_no1);
    if (iRet != 0)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_sett_rslt error, %d.", iRet);
        return -1;
    }

    /*合作方8*/
    memcpy(tbl_sett_flow.mcht_role1, ptbl_sett_flow->mcht_role8, sizeof(tbl_sett_flow.mcht_role1)-1);
    memcpy(tbl_sett_flow.mcht_no1, ptbl_sett_flow->mcht_no8, sizeof(tbl_sett_flow.mcht_no1)-1);
    tbl_sett_flow.debt_at_mcht_no1 = ptbl_sett_flow->debt_at_mcht_no8;
    tbl_sett_flow.cret_at_mcht_no1 = ptbl_sett_flow->cret_at_mcht_no8;
    tbl_sett_flow.debt_fee_mcht_no1 = ptbl_sett_flow->debt_fee_mcht_no8;
    tbl_sett_flow.cret_fee_mcht_no1 = ptbl_sett_flow->cret_fee_mcht_no8;
    tbl_sett_flow.share_profit_mcht_no1 = ptbl_sett_flow->share_profit_mcht_no8;
    tbl_sett_flow.mcht_rate1 = ptbl_sett_flow->mcht_rate8;
    if(ptbl_sett_flow->share_profit_mcht_no8 > 0)
    {
        tbl_sett_flow.debt_at_mcht_no1= ptbl_sett_flow->share_profit_mcht_no8;
    }
    else if(ptbl_sett_flow->share_profit_mcht_no8 < 0)
    {
        tbl_sett_flow.cret_at_mcht_no1= ptbl_sett_flow->share_profit_mcht_no8 * -1;
    }
    iRet = in_sett_settmcht_rslt(&tbl_sett_flow, 8, ptbl_sett_flow->mcht_no1);
    if (iRet != 0)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_sett_rslt error, %d.", iRet);
        return -1;
    }

    /*合作方9*/
    memcpy(tbl_sett_flow.mcht_role1, ptbl_sett_flow->mcht_role9, sizeof(tbl_sett_flow.mcht_role1)-1);
    memcpy(tbl_sett_flow.mcht_no1, ptbl_sett_flow->mcht_no9, sizeof(tbl_sett_flow.mcht_no1)-1);
    tbl_sett_flow.debt_at_mcht_no1 = ptbl_sett_flow->debt_at_mcht_no9;
    tbl_sett_flow.cret_at_mcht_no1 = ptbl_sett_flow->cret_at_mcht_no9;
    tbl_sett_flow.debt_fee_mcht_no1 = ptbl_sett_flow->debt_fee_mcht_no9;
    tbl_sett_flow.cret_fee_mcht_no1 = ptbl_sett_flow->cret_fee_mcht_no9;
    tbl_sett_flow.share_profit_mcht_no1 = ptbl_sett_flow->share_profit_mcht_no9;
    tbl_sett_flow.mcht_rate1 = ptbl_sett_flow->mcht_rate9;
    if(ptbl_sett_flow->share_profit_mcht_no9 > 0)
    {
        tbl_sett_flow.debt_at_mcht_no1= ptbl_sett_flow->share_profit_mcht_no9;
    }
    else if(ptbl_sett_flow->share_profit_mcht_no9 < 0)
    {
        tbl_sett_flow.cret_at_mcht_no1= ptbl_sett_flow->share_profit_mcht_no9 * -1;
    }
    iRet = in_sett_settmcht_rslt(&tbl_sett_flow, 9, ptbl_sett_flow->mcht_no1);
    if (iRet != 0)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_sett_rslt error, %d.", iRet);
        return -1;
    }

    if(memcmp(ptbl_sett_flow->bus_tp+4, BT_SETT_TRAN_D, BT_SETT_TRAN_LEN) == 0)
    {
        /*直接模式*/
        memcpy(tbl_sett_flow.mcht_role1, BT_MCHT_ROLE_PLAT_D, BT_MCHT_ROLE_LEN);
    }
    else
    {
        memcpy(tbl_sett_flow.mcht_role1, BT_MCHT_ROLE_PLAT_IND, BT_MCHT_ROLE_LEN);
    }
    
    /*平台*/
    
    memcpy(tbl_sett_flow.mcht_no1, g_plat_mcht_no, sizeof(tbl_sett_flow.mcht_no1)-1);
    strcpy(tbl_sett_flow.sett_tp1, BT_SETT_TP_PLAT);
    tbl_sett_flow.debt_at_mcht_no1 = 0;
    tbl_sett_flow.cret_at_mcht_no1 = 0;
    tbl_sett_flow.share_profit_mcht_no1 = 0;
    if(ptbl_sett_flow->share_profit_plat >= 0)
    {
        tbl_sett_flow.debt_at_mcht_no1 = ptbl_sett_flow->share_profit_plat;
    }
    else
    {
        tbl_sett_flow.cret_at_mcht_no1 = ptbl_sett_flow->share_profit_plat * -1;
    }

    iRet = in_sett_settmcht_rslt(&tbl_sett_flow, 10, ptbl_sett_flow->mcht_no1);
    if (iRet != 0)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_sett_rslt error, %d.", iRet);
        return -1;
    }
    return 0;
    
}

/*************************************************
*函数名称∶ Total_5101
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_5101()
{
    int		nTotalNum = 0;
    int         iRet=0;
    ht_tbl_sett_flow_def tbl_sett_flow;

    memset(&tbl_sett_flow, 0x00, sizeof(tbl_sett_flow));
    memcpy(tbl_sett_flow.sett_date, dbtbl_date_inf.stoday, sizeof(tbl_sett_flow.sett_date)-1);
    memcpy(tbl_sett_flow.inter_brh_code, ext_inter_brh_code, sizeof(tbl_sett_flow.inter_brh_code)-1);
    iRet = DbsTblSettFlow(DBS_SELECT1, &tbl_sett_flow, 0, 0);
    if (iRet != 0)
    {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblSettFlow DBS_SELECT1 error, %d.", iRet);
            return -1;
    }
    nTotalNum = tbl_sett_flow.seq_num;
    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "nTotalNum =[%d].", nTotalNum);

    return nTotalNum;
}

/*************************************************
*函数名称∶ Task_5101
*函数功能∶
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_5101 ( int nBeginOffset, int nEndOffset )
{
    int	 iRet = 0, nTotCount=0;
   ht_tbl_sett_flow_def tbl_sett_flow;

    memset(&tbl_sett_flow, 0x00, sizeof(tbl_sett_flow));
    memcpy(tbl_sett_flow.sett_date, dbtbl_date_inf.stoday, sizeof(tbl_sett_flow.sett_date)-1);
    memcpy(tbl_sett_flow.inter_brh_code, ext_inter_brh_code, sizeof(tbl_sett_flow.inter_brh_code)-1);
    DbsTblSettFlow(DBS_CURSOR, &tbl_sett_flow, nBeginOffset, nEndOffset);
    iRet = DbsTblSettFlow(DBS_OPEN, &tbl_sett_flow, nBeginOffset, nEndOffset);
    if (iRet != 0)
    {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblSettFlow DBS_OPEN error, %d.", iRet);
            return -1;
    }

    while (1)
    {
        memset(&tbl_sett_flow, 0, sizeof(tbl_sett_flow));
        iRet = DbsTblSettFlow(DBS_FETCH, &tbl_sett_flow, nBeginOffset, nEndOffset);
        if (iRet)
        {
            if (iRet == DBS_NOTFOUND)
            {
                break;
            }

            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblDatFlow fetch sqlcode[%d]", iRet);
            DbsTblSettFlow(DBS_CLOSE, &tbl_sett_flow, nBeginOffset, nEndOffset);
            return -1;
        }
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "-----------------plat_key[%s]trans_tp[%s]---------", tbl_sett_flow.plat_key, tbl_sett_flow.trans_tp);
        nTotCount++;

        iRet = in_sett(&tbl_sett_flow);
        if (iRet != 0)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_sett error, %d.", iRet);
            return -1;
        }
    }
    DbsTblSettFlow(DBS_CLOSE, &tbl_sett_flow, nBeginOffset, nEndOffset);
    
    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "total num [%d].", nTotCount);
    HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_5101 Succ .");
    return 0;
}
/****************************** end of Task_5101.c ******************************/

