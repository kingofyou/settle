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
 *  文 件 名: Task_6602.c
 *  功    能: 提现记录初步处理
 *  编程人员: bcw
 *  开发时间: 2017308
 *  备    注:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include <sys/socket.h>
#include "batch.h"
#include "Common.h"

extern char	gLogFile[LOG_NAME_LEN_MAX];
extern  tbl_date_inf_def dbtbl_date_inf;
extern	char ext_inter_brh_code[10 + 1];


int GetAcctInf(tp_payway_ext_def *pvtp_payway_ext,ht_tbl_ext_mcht_inf_def * pvtbl_ext_mcht_inf_txn, char* ext_mcht_no)
{
    int nReturnCode;
    
    ht_tbl_com_config_def comconfig;
    tp_payway_ext_def tp_payway_ext;
    tp_merc_paywayconf_def tp_merc_paywayconf;
    ht_tbl_ext_mcht_inf_def  tbl_ext_mcht_inf;
        
//    memset(&comconfig, 0x00, sizeof(comconfig));
//    strcpy(comconfig.key1,"200001");
//    strcpy(comconfig.key2,"CIBGATEWAY");
//    strcpy(comconfig.key3,"PAYWAYTYPE");
//    nReturnCode = DbsbthComConfig (DBS_SELECT, &comconfig);
//     if( nReturnCode )
//    {
//        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsbthComConfig DBS_SELECT[%d]", nReturnCode);
//        return nReturnCode;
//    }
//
//    CommonRTrim(comconfig.value1);  
//    CommonRTrim(comconfig.value2);  
//    CommonRTrim(comconfig.value3);
//    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "[%s][%s][%s]", comconfig.value1,comconfig.value2,comconfig.value3); 
//    
//    memset(&tp_payway_ext, 0x00, sizeof(tp_payway_ext));
//    memcpy(tp_payway_ext.payway_type,comconfig.value1,sizeof(tp_payway_ext.payway_type)-1);    
//    nReturnCode = DbsTpPaywayExt (DBS_SELECT, &tp_payway_ext);
//	if (nReturnCode)
//	{
//	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTpPaywayExt error [%d]", nReturnCode);
//	    return -1;
//	}	
//	CommonRTrim(tp_payway_ext.payway_code);
//	memset(&tp_merc_paywayconf,0x0,sizeof(tp_merc_paywayconf));
//	memcpy(tp_merc_paywayconf.payway_code,tp_payway_ext.payway_code,sizeof(tp_merc_paywayconf.payway_code)-1);
//	HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "payway_code[%s]", tp_merc_paywayconf.payway_code);
//	nReturnCode = DbsTpMercPaywayConf (DBS_SELECT, &tp_merc_paywayconf);    
//	if (nReturnCode)
//	{
//	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTpMercPaywayConf error [%d]", nReturnCode);
//	    return -1;
//	}
//	CommonRTrim(tp_merc_paywayconf.paying_merc_code);
//	HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "paying_merc_code[%s]", tp_merc_paywayconf.paying_merc_code);

    memset(&tbl_ext_mcht_inf, 0, sizeof(tbl_ext_mcht_inf));
    //strcpy(tbl_ext_mcht_inf.ext_mch_no, tp_merc_paywayconf.paying_merc_code);
	CommonRTrim(ext_mcht_no);
	strcpy(tbl_ext_mcht_inf.ext_mch_no, ext_mcht_no);
    CommonRTrim(tbl_ext_mcht_inf.ext_mch_no);			
    nReturnCode = DbsTblExtMchtInfTxn(DBS_SELECT, &tbl_ext_mcht_inf, 0, 0);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblExtMchtInfTxn err[%d]",
               nReturnCode);
        return -1;
    }
    
    memcpy(pvtbl_ext_mcht_inf_txn,&tbl_ext_mcht_inf,sizeof(tbl_ext_mcht_inf)); 
   // memcpy(pvtp_payway_ext,&tp_payway_ext,sizeof(tp_payway_ext));
    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "ext_mch_no[%s]", tbl_ext_mcht_inf.ext_mch_no); 
    return 0;
}

int in_sett_list_c(ht_tbl_sett_flow_def tbl_sett_flow,tp_payway_ext_def tp_payway_ext,ht_tbl_ext_mcht_inf_def  tbl_ext_mcht_inf,char *pbank_order_no)
{
    int nReturnCode;
    char    stlm_dt[8+1];
    int     nBankNum = 0;
    char    sBankTime[8+1]; 
    char    sSqlBuf[128];  
   
    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
    
    tbl_sett_list_def   dbtbl_sett_clist;
    ht_tbl_brh_fee_inf_def tbl_brh_fee_inf;
    tp_bankcards_bin_def tp_bankcards_bin;
    
    memset(&dbtbl_sett_clist, 0, sizeof(dbtbl_sett_clist));
    
    RightTrim(tbl_sett_flow.pan);       
    memset(&tp_bankcards_bin,0x0,sizeof(tp_bankcards_bin));
    memcpy(tp_bankcards_bin.bank_card_bin,tbl_sett_flow.pan,6);
    sprintf(tp_bankcards_bin.card_length,"%d",strlen(tbl_sett_flow.pan));
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "pan[%s][%d]", tp_bankcards_bin.bank_card_bin,strlen(tp_bankcards_bin.bank_card_bin));
    nReturnCode = DbsTpBankCardsBin(DBS_SELECT,&tp_bankcards_bin);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTpBankCardsBin DBS_SELECT error [%d].", nReturnCode);            
        return -1;
    }
    RightTrim(tp_bankcards_bin.bank_code);
    RightTrim(tp_bankcards_bin.bank_name);
    RightTrim(tp_bankcards_bin.account_type);
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "bank_code[%s]bank_name[%s]account_type[%s].", tp_bankcards_bin.bank_code,tp_bankcards_bin.bank_name,tp_bankcards_bin.account_type);                                       
        
    RightTrim(tbl_sett_flow.inter_brh_code);
    memcpy(dbtbl_sett_clist.brh_id,         tbl_sett_flow.inter_brh_code,   sizeof(dbtbl_sett_clist.brh_id)-1); /*内部机构号*/
    memcpy(dbtbl_sett_clist.sett_date,      tbl_sett_flow.sett_date, 8);                                          /*结算日期*/
    memcpy(dbtbl_sett_clist.trans_date,     tbl_sett_flow.trans_date,       sizeof(dbtbl_sett_clist.trans_date)-1);/*交易日期*/
    RightTrim(tbl_sett_flow.trans_date);
    RightTrim(tbl_sett_flow.trans_time);
    
    RightTrim(tbl_sett_flow.mcht_no1);
    memcpy(dbtbl_sett_clist.mcht_no,        tbl_sett_flow.mcht_no1,       sizeof(dbtbl_sett_clist.mcht_no)-1);    /*商户号*/
       	        
    memcpy(dbtbl_sett_clist.mcht_role,   tbl_sett_flow.pan1,       4);                 /*角色类型*/ 
    
    RightTrim(tbl_sett_flow.order_no);    
    memcpy(dbtbl_sett_clist.mcht_rslt_no,   tbl_sett_flow.order_no,       sizeof(dbtbl_sett_clist.mcht_rslt_no)-1);  /*角色类型*/   	        
    RightTrim(dbtbl_sett_clist.mcht_rslt_no);                                                       /*商户入账流水号*/   	    
    
    memcpy(dbtbl_sett_clist.mcht_sett_bank,     tp_bankcards_bin.bank_code,     sizeof(dbtbl_sett_clist.mcht_sett_bank)-1);   /*商户结算行*/    
    RightTrim(tbl_sett_flow.acct_name);
    memcpy(dbtbl_sett_clist.mcht_sett_bank_nm,     tp_bankcards_bin.bank_name,  sizeof(dbtbl_sett_clist.mcht_sett_bank_nm)-1);    /*商户结算账户名称*/              
    memcpy(dbtbl_sett_clist.mcht_sett_acct_type,     tp_bankcards_bin.account_type, sizeof(dbtbl_sett_clist.mcht_sett_acct_type)-1);   /*商户结算账户类型*/    
    RightTrim(tbl_sett_flow.pan);
    memcpy(dbtbl_sett_clist.mcht_sett_acct,       tbl_sett_flow.pan,     sizeof(dbtbl_sett_clist.mcht_sett_acct)-1);    /*商户结算账户账号*/

    memcpy(dbtbl_sett_clist.mcht_sett_acct_nm,     tbl_sett_flow.acct_name,  sizeof(dbtbl_sett_clist.mcht_sett_acct_nm)-1);    /*商户结算账户名称*/            
    
    dbtbl_sett_clist.amt = tbl_sett_flow.cret_at_mcht_no1/100;
    memcpy(dbtbl_sett_clist.dc_flag, "C", 1);                                                                                    /*收入*/

    memset(sSqlBuf, 0, 128);
    sprintf(sSqlBuf, "select TBL_SETT_LIST_SEQ_ID_%s.nextval from dual", "200001");

    nReturnCode = SelectSequence(sSqlBuf, &nBankNum);
    if( nReturnCode )
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "select from TBL_SETT_LIST_SEQ_ID_%s error, sqlcode=[%d].", "200001", nReturnCode);
        return nReturnCode;
    }
    memset(sBankTime,0x0,sizeof(sBankTime)); 
    CommonGetCurrentDate(sBankTime);
    sprintf(dbtbl_sett_clist.seq_no,"%s%-10.10d",sBankTime,nBankNum);         /*结算流水号*/
    memcpy(pbank_order_no,dbtbl_sett_clist.seq_no,strlen(dbtbl_sett_clist.seq_no));
            
    RightTrim(tbl_ext_mcht_inf.ext_mch_acct_bank);
    memcpy(dbtbl_sett_clist.plat_sett_bank,  tbl_ext_mcht_inf.ext_mch_acct_bank,         sizeof(dbtbl_sett_clist.plat_sett_bank)-1);  /*平台结算行*/
    
    RightTrim(tbl_ext_mcht_inf.ext_mch_acct_banknm);
    memcpy(dbtbl_sett_clist.plat_sett_bank_nm,    tbl_ext_mcht_inf.ext_mch_acct_banknm,         sizeof(dbtbl_sett_clist.plat_sett_bank_nm)-1);  /*平台结算账户账号*/
   
    memcpy(dbtbl_sett_clist.plat_sett_acct_type,  tbl_ext_mcht_inf.ext_mch_acct_type,    sizeof(dbtbl_sett_clist.plat_sett_acct_type)-1);    /*平台结算账户类型*/
    
    RightTrim(tbl_ext_mcht_inf.ext_mch_acct_no);
    memcpy(dbtbl_sett_clist.plat_sett_acct,    tbl_ext_mcht_inf.ext_mch_acct_no,         sizeof(dbtbl_sett_clist.plat_sett_acct)-1);   /*平台结算账户账号*/
    
    RightTrim(tbl_ext_mcht_inf.ext_mch_acct_nm);
    memcpy(dbtbl_sett_clist.plat_sett_acct_nm,  tbl_ext_mcht_inf.ext_mch_acct_nm,      sizeof(dbtbl_sett_clist.plat_sett_acct_nm)-1);   /*平台结算账户名称*/
    
    RightTrim(tbl_sett_flow.acct_statu);
    memcpy(dbtbl_sett_clist.acct_statu, tbl_sett_flow.acct_statu,             sizeof(dbtbl_sett_clist.acct_statu)-1);                 /*入账状态*/

    if(strcmp(dbtbl_sett_clist.mcht_sett_bank, dbtbl_sett_clist.plat_sett_bank) == 0)
    {       
        dbtbl_sett_clist.plat_amt_fee=0.00;
    }
    else
    {
		dbtbl_sett_clist.plat_amt_fee=0.00;
//        memset(&tbl_brh_fee_inf,0x0,sizeof(tbl_brh_fee_inf));
//        memcpy(tbl_brh_fee_inf.payway_code, tp_payway_ext.payway_code, sizeof(tbl_brh_fee_inf.payway_code)-1);
//        RightTrim(tbl_brh_fee_inf.payway_code);
//        memcpy(tbl_brh_fee_inf.tran_code, "*", 4);
//        memcpy(tbl_brh_fee_inf.card_type, "00", 2);      
//        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "pay_brh[%s]trans_code[%s];card_type[%s][%s]", tbl_brh_fee_inf.payway_code, tbl_brh_fee_inf.tran_code, tbl_brh_fee_inf.card_type, tbl_brh_fee_inf.payway_code);         
//        nReturnCode = DbsTblBrhFeeInfTxn(DBS_SELECT, &tbl_brh_fee_inf, 0, 0);
//        if (nReturnCode != 0 && nReturnCode!=DBS_NOTFOUND)
//        {
//                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "tbl_mcht_inf.mcht_no[%s].", tbl_brh_fee_inf.payway_code);
//                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblBrhFeeInfTxn DBS_SELECT error, %d.", nReturnCode);
//                return -1;
//        }
//        if(tbl_brh_fee_inf.fee_flag[0] == '1')//1 ： 按笔收费固定值
//        {
//            dbtbl_sett_clist.plat_amt_fee=tbl_brh_fee_inf.fee_value;        
//        }
//        else//2 ： 按比例收费
//        {
//            dbtbl_sett_clist.plat_amt_fee=dbtbl_sett_clist.amt*tbl_brh_fee_inf.fee_value;
//            if(dbtbl_sett_clist.plat_amt_fee <= tbl_brh_fee_inf.fee_min)
//            {
//                dbtbl_sett_clist.plat_amt_fee=tbl_brh_fee_inf.fee_min;            
//            }
//            else if(dbtbl_sett_clist.plat_amt_fee >= tbl_brh_fee_inf.fee_max)
//            {
//                dbtbl_sett_clist.plat_amt_fee=tbl_brh_fee_inf.fee_max;
//            }
//            else
//            {
//                
//            }       
//        }
    }
   // HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "fee_flag[%s] fee_value[%f]", tbl_brh_fee_inf.fee_flag,tbl_brh_fee_inf.fee_value);                    
    
   // memcpy(dbtbl_sett_clist.chl_id,tp_payway_ext.payway_code,sizeof(dbtbl_sett_clist.chl_id)-1);
    memcpy(dbtbl_sett_clist.chnl_name,"兴业代付",sizeof(dbtbl_sett_clist.chnl_name)-1);
    RightTrim(tbl_sett_flow.pan1);
    memcpy(dbtbl_sett_clist.mcht_sett_xingmin_acct,tbl_sett_flow.pan1,sizeof(dbtbl_sett_clist.mcht_sett_xingmin_acct)-1);
    
	           
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "shouru:");
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==brh_id:[%s].", dbtbl_sett_clist.brh_id);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==sett_date:[%s].", dbtbl_sett_clist.sett_date);      
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==trans_date:[%s].", dbtbl_sett_clist.trans_date);    
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_role:[%s].", dbtbl_sett_clist.mcht_role);     
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_rslt_no:[%s].", dbtbl_sett_clist.mcht_rslt_no);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==seq_no:[%s].", dbtbl_sett_clist.seq_no);            
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==plat_sett_bank:[%s].", dbtbl_sett_clist.plat_sett_bank);      
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==plat_sett_bank_nm:[%s].", dbtbl_sett_clist.plat_sett_bank_nm);      
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==plat_sett_acct_type:[%s].", dbtbl_sett_clist.plat_sett_acct_type);      
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==plat_sett_acct:[%s].", dbtbl_sett_clist.plat_sett_acct);          
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==plat_sett_acct_nm:[%s].", dbtbl_sett_clist.plat_sett_acct_nm);      
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==dc_flag:[%s].", dbtbl_sett_clist.dc_flag);          
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_sett_xingmin_acct:[%s].", dbtbl_sett_clist.mcht_sett_xingmin_acct); 
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_sett_bank:[%s].", dbtbl_sett_clist.mcht_sett_bank);    
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_sett_bank_nm:[%s].", dbtbl_sett_clist.mcht_sett_bank_nm);    
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_sett_acct_type:[%s].", dbtbl_sett_clist.mcht_sett_acct_type);    
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_sett_acct:[%s].", dbtbl_sett_clist.mcht_sett_acct);        
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_sett_acct_nm:[%s].", dbtbl_sett_clist.mcht_sett_acct_nm);    
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==amt:[%.2f].", dbtbl_sett_clist.amt);                
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==plat_amt_fee:[%.2f].", dbtbl_sett_clist.plat_amt_fee);                
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==acct_statu:[%s].", dbtbl_sett_clist.acct_statu);            
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==chl_id:[%s].", dbtbl_sett_clist.chl_id);            
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==chnl_name:[%s].", dbtbl_sett_clist.chnl_name); 

    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"); 
            
    nReturnCode = preDealSett(&dbtbl_sett_clist);
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "preDealSett error!");
        return -1;
    }
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==status:[%s].", dbtbl_sett_clist.status);
            
            /* 插入结算明细表 */
    nReturnCode = DbsTblSettList(DBS_INSERT, &dbtbl_sett_clist);
    if (nReturnCode != 0 && nReturnCode != DBS_KEYDUPLICATE)
    {
        HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblSettList DBS_INSERT error  [%d].", nReturnCode);
        return -1;
    }
    else if (nReturnCode == DBS_KEYDUPLICATE)
    {
        HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblSettList DBS_KEYDUPLICATE, mcht_rslt_no[%s] dc_flag[%s].", dbtbl_sett_clist.mcht_rslt_no,dbtbl_sett_clist.dc_flag);
    }
    return 0;   
}


/*************************************************
*函数名称∶ Total_6602
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_6602()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_6602
*函数功能∶ 从入账结果表中抽取数据导入结算明细表,并将结算明细记录进行分类
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_6602 ( int nBeginOffset, int nEndOffset )
{

    int nReturnCode;
    char    sDateTime[14+1];
    char    stlm_dt[8+1];    
    int		nTotalCount = 0;
    char  bank_order_no[48+1];
    
    int c_flag;
    int d_flag;
 
    ht_tbl_sett_flow_def tbl_sett_flow;
    ht_tbl_ext_mcht_inf_def  tbl_ext_mcht_inf;   
    tp_payway_ext_def tp_payway_ext;
          
    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
  
    memset(&tbl_ext_mcht_inf, 0x00, sizeof(tbl_ext_mcht_inf));
    memset(&tp_payway_ext, 0x00, sizeof(tp_payway_ext));
			
	memset(sDateTime, 0, sizeof(sDateTime));	 		       
	getSysTime(sDateTime);
	
    memset(&tbl_sett_flow, 0x00, sizeof(tbl_sett_flow));
    memcpy(tbl_sett_flow.sett_date, stlm_dt, sizeof(tbl_sett_flow.sett_date)-1);
    memcpy(tbl_sett_flow.trans_tp,BT_TRANS_TP_WITHDRAW,sizeof(tbl_sett_flow.trans_tp)-1);
    memcpy(tbl_sett_flow.flag_result, "1", sizeof(tbl_sett_flow.flag_result)-1);
    memcpy(tbl_sett_flow.acct_statu, "N", sizeof(tbl_sett_flow.acct_statu)-1);
    memcpy(tbl_sett_flow.status, "0", sizeof(tbl_sett_flow.status)-1);
    DbsTblSettFlow(DBS_CURSOR3, &tbl_sett_flow, nBeginOffset, nEndOffset);
    nReturnCode = DbsTblSettFlow(DBS_OPEN3, &tbl_sett_flow, nBeginOffset, nEndOffset);
    if (nReturnCode != 0)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblSettFlow DBS_OPEN3 error, %d.", nReturnCode);
        return -1;
    }
    while (1)
    {
        memset(&tbl_sett_flow, 0, sizeof(tbl_sett_flow));
        nReturnCode = DbsTblSettFlow(DBS_FETCH3, &tbl_sett_flow, nBeginOffset, nEndOffset);
        if (nReturnCode && nReturnCode != DBS_FETCHNULL)
        {
            if (nReturnCode == DBS_NOTFOUND)
            {
                break;
            }
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblDatFlow DBS_FETCH3 sqlcode[%d]", nReturnCode);
            DbsTblSettFlow(DBS_CLOSE3, &tbl_sett_flow, nBeginOffset, nEndOffset);
            return -1;
        }
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "-----------------plat_key[%s]---------", tbl_sett_flow.plat_key);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "cret_at_mcht_no1:[%.2f].", tbl_sett_flow.cret_at_mcht_no1/100);

		nReturnCode = GetAcctInf(&tp_payway_ext,&tbl_ext_mcht_inf, tbl_sett_flow.ext_mcht_no);
	    if (nReturnCode)
	    {
	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "GetAcctInf error [%d]", nReturnCode);
	        return -1;
	    }
        
        if(tbl_sett_flow.cret_at_mcht_no1/100 <= 0.0000001 && tbl_sett_flow.cret_at_mcht_no1/100 >= -0.0000001)
        {
            HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "tbl_sett_flow.cret_at_mcht_no1 0 [%f]",tbl_sett_flow.cret_at_mcht_no1);
            continue;
        }
                    
        if(tbl_sett_flow.cret_at_mcht_no1/100 < 0.00)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "交易金额<0 出报表　amt:[%.2f].", tbl_sett_flow.cret_at_mcht_no1);
            continue;            
        }
        nTotalCount++; 
        memset(bank_order_no,0,sizeof(bank_order_no));
        nReturnCode = in_sett_list_c(tbl_sett_flow,tp_payway_ext,tbl_ext_mcht_inf,bank_order_no);          
        if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_sett_list_c error [%d]", nReturnCode);
            DbsTblSettFlow(DBS_CLOSE3, &tbl_sett_flow, nBeginOffset, nEndOffset);
            return -1;
        }          
        /* 更新结算明细表记录 */ 
        memcpy(tbl_sett_flow.bank_order_no,bank_order_no,sizeof(tbl_sett_flow.bank_order_no)-1);     
        memcpy(tbl_sett_flow.acct_statu,"9",1);                    
        nReturnCode = DbsTblSettFlow(DBS_UPDATE, &tbl_sett_flow, nBeginOffset, nEndOffset);
        if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPDATE ht_tbl_sett_list error [%d].", nReturnCode);
            DbsTblSettFlow(DBS_CLOSE3, &tbl_sett_flow, nBeginOffset, nEndOffset);
            return -1;
        } 
                         
    }
    DbsTblSettFlow(DBS_CLOSE3, &tbl_sett_flow, nBeginOffset, nEndOffset);
    
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nTotalCount:[%d]", nTotalCount);
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_6602 Succ stlm_dt[%s].",stlm_dt);
   
    return 0;
}

int preDealSett(tbl_sett_list_def* vtdbtbl_sett_list)
{
    tbl_postion_trial_def ptdbtbl_post_trial;
    tbl_sett_chan_def ptdbtbl_sett_chan;
    char bank_no[10+1];
    int nReturnCode;
    char    stlm_dt[8+1];
    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);   
    memset(bank_no, 0, sizeof(bank_no)-1);
    
    if(vtdbtbl_sett_list->mcht_sett_acct_type[0] == '3')/*虚拟卡*/
    {
        memcpy(vtdbtbl_sett_list->status, "2", 1); 
        memcpy(vtdbtbl_sett_list->plat_sett_bank, "core_xmyf", sizeof(vtdbtbl_sett_list->plat_sett_bank)-1);      
    }
    else
    {
        /* 判断平台结算行头寸,足则可划扣并修改最优行,不够则需归集 */
        memset(&ptdbtbl_post_trial, 0, sizeof(ptdbtbl_post_trial));
        RightTrim(vtdbtbl_sett_list->plat_sett_bank);
        memcpy(ptdbtbl_post_trial.bank_code, vtdbtbl_sett_list->plat_sett_bank, sizeof(ptdbtbl_post_trial.bank_code)-1);
        memcpy(ptdbtbl_post_trial.sett_date, stlm_dt, sizeof(ptdbtbl_post_trial.sett_date)-1);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "trial bank_code:[%s].", ptdbtbl_post_trial.bank_code);
        nReturnCode = DbsTblPostTrial(DBS_SELECT, &ptdbtbl_post_trial);
	    if(nReturnCode)
	    {
	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_SELECT ht_tbl_postion_trial error [%d]", nReturnCode);
	        return -1;
	    }
	    else
	    {
	        /* 头寸判断(头寸金额-保底金额 >= 交易金额) */
    	    if(ptdbtbl_post_trial.bal_trial-ptdbtbl_post_trial.resv_bal >= vtdbtbl_sett_list->amt)
    	    {
    	        memcpy(vtdbtbl_sett_list->status, "2", 1);
    	        /* 更新试算头寸信息 */
    	        ptdbtbl_post_trial.bal_trial -= vtdbtbl_sett_list->amt;
    	        RightTrim(ptdbtbl_post_trial.bank_code);
    	        memcpy(ptdbtbl_post_trial.sett_date, stlm_dt, sizeof(ptdbtbl_post_trial.sett_date)-1);
    	        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "bank_code:[%s].", ptdbtbl_post_trial.bank_code); 
    	        nReturnCode = DbsTblPostTrial(DBS_UPD_BAL, &ptdbtbl_post_trial);
        	    if(nReturnCode)
        	    {
        	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPD_BAL ht_tbl_postion_trial error [%d]", nReturnCode);
        	        return -1;
        	    }
    	    }
    	    else
    	    {
    	         //memcpy(vtdbtbl_sett_list->status, "1", 1);   /*归集状态*/
    	         memcpy(vtdbtbl_sett_list->status, "2", 1);   /*归集状态*/
    	         HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "归集状态");
    	    }
        }
    }
    
    return 0;
}


/****************************** end of Task_6602.c ******************************/


