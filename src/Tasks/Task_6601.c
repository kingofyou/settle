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
 *  文 件 名: Task_6601.c
 *  功    能: 结算明细记录初步处理
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

int GetAcctInf(tp_payway_ext_def *pvtp_payway_ext,ht_tbl_ext_mcht_inf_def * pvtbl_ext_mcht_inf_txn)
{
    int nReturnCode;
    
   // ht_tbl_com_config_def comconfig;
    tp_payway_ext_def tp_payway_ext;
   // tp_merc_paywayconf_def tp_merc_paywayconf;
//ht_tbl_ext_mcht_inf_def  tbl_ext_mcht_inf;
        
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
    memset(&tp_payway_ext, 0x00, sizeof(tp_payway_ext));
    memcpy(tp_payway_ext.payway_code,"xydf",sizeof(tp_payway_ext.payway_code)-1);    
    nReturnCode = DbsTpPaywayExt (DBS_SELECT, &tp_payway_ext);
	if (nReturnCode)
	{
	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTpPaywayExt error [%d]", nReturnCode);
	    return -1;
	}	
	CommonRTrim(tp_payway_ext.payway_code);
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
//
//    memset(&tbl_ext_mcht_inf, 0, sizeof(tbl_ext_mcht_inf));
//    strcpy(tbl_ext_mcht_inf.ext_mch_no, tp_merc_paywayconf.paying_merc_code);
//    CommonRTrim(tbl_ext_mcht_inf.ext_mch_no);			
//    nReturnCode = DbsTblExtMchtInfTxn(DBS_SELECT, &tbl_ext_mcht_inf, 0, 0);
//    if (nReturnCode)
//    {
//        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblExtMchtInfTxn err[%d]",
//               nReturnCode);
//        return -1;
//    }
//    
//    memcpy(pvtbl_ext_mcht_inf_txn,&tbl_ext_mcht_inf,sizeof(tbl_ext_mcht_inf)); 
    memcpy(pvtp_payway_ext,&tp_payway_ext,sizeof(tp_payway_ext));
  //  HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "ext_mch_no[%s]", tbl_ext_mcht_inf.ext_mch_no); 
    return 0;
}

int in_sett_list_c(ht_tbl_acct_rslt_def   dbtbl_acct_rslt,tp_payway_ext_def tp_payway_ext)
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
    
    memset(&dbtbl_sett_clist, 0, sizeof(dbtbl_sett_clist));
    
    RightTrim(dbtbl_acct_rslt.inter_brh_code);
    memcpy(dbtbl_sett_clist.brh_id,         dbtbl_acct_rslt.inter_brh_code,     sizeof(dbtbl_sett_clist.brh_id)-1);                       /*内部机构号*/
    memcpy(dbtbl_sett_clist.sett_date,      stlm_dt, 8);                                                                         /*结算日期*/
    memcpy(dbtbl_sett_clist.trans_date,     dbtbl_acct_rslt.acct_date,          sizeof(dbtbl_sett_clist.trans_date)-1);                   /*交易日期*/
    RightTrim(dbtbl_acct_rslt.mcht_no);
    memcpy(dbtbl_sett_clist.mcht_no,   dbtbl_acct_rslt.mcht_no,       sizeof(dbtbl_sett_clist.mcht_no)-1);                 /*商户号*/   	        
    RightTrim(dbtbl_acct_rslt.mcht_role);
    memcpy(dbtbl_sett_clist.mcht_role,   dbtbl_acct_rslt.mcht_role,       sizeof(dbtbl_sett_clist.mcht_role)-1);                 /*角色类型*/   	        
    RightTrim(dbtbl_acct_rslt.mcht_rslt_no);
    memcpy(dbtbl_sett_clist.mcht_rslt_no,   dbtbl_acct_rslt.mcht_rslt_no,       sizeof(dbtbl_sett_clist.mcht_rslt_no)-1);                 /*商户入账流水号*/   	    
    RightTrim(dbtbl_acct_rslt.mcht_sett_bank);
    memcpy(dbtbl_sett_clist.mcht_sett_bank,     dbtbl_acct_rslt.mcht_sett_bank,     sizeof(dbtbl_sett_clist.mcht_sett_bank)-1);                   /*商户结算行*/
    RightTrim(dbtbl_acct_rslt.mcht_sett_bank_nm);
    memcpy(dbtbl_sett_clist.mcht_sett_bank_nm,     dbtbl_acct_rslt.mcht_sett_bank_nm,  sizeof(dbtbl_sett_clist.mcht_sett_bank_nm)-1);                   /*商户结算账户名称*/            
    memcpy(dbtbl_sett_clist.mcht_sett_acct_type,     dbtbl_acct_rslt.mcht_sett_acct_type, sizeof(dbtbl_sett_clist.mcht_sett_acct_type)-1);                  /*商户结算账户类型*/
    RightTrim(dbtbl_acct_rslt.mcht_sett_acct);
    memcpy(dbtbl_sett_clist.mcht_sett_acct,       dbtbl_acct_rslt.mcht_sett_acct,     sizeof(dbtbl_sett_clist.mcht_sett_acct)-1);                     /*商户结算账户账号*/
    RightTrim(dbtbl_acct_rslt.mcht_sett_acct_nm);
    memcpy(dbtbl_sett_clist.mcht_sett_acct_nm,     dbtbl_acct_rslt.mcht_sett_acct_nm,  sizeof(dbtbl_sett_clist.mcht_sett_acct_nm)-1);                   /*商户结算账户名称*/            
    if(!strncmp(dbtbl_acct_rslt.sett_md, "01", 2))//01：扎差
    {
        /*划账金额=            入账金额贷 - 入账金额借 + 手续费净额贷-手续费净额借*/
        //dbtbl_sett_clist.amt = dbtbl_acct_rslt.cret_at - dbtbl_acct_rslt.debt_at;  /*划账金额*/
         /*划账金额=            入账金额借 -  入账金额贷*/
        dbtbl_sett_clist.amt = (dbtbl_acct_rslt.debt_at - dbtbl_acct_rslt.cret_at)/100;  /*划账金额*/  
       
    }
    else if(!strncmp(dbtbl_acct_rslt.sett_md, "02", 2))//02：收支两条线
    {
        /*划账金额=            入账金额贷  + 手续费净额贷*/
        dbtbl_sett_clist.amt = dbtbl_acct_rslt.debt_at/100;                                              
    }
    memcpy(dbtbl_sett_clist.dc_flag, "C", 1);                                                                                    /*收入*/
    memcpy(dbtbl_sett_clist.seq_no, dbtbl_acct_rslt.sett_date, 8);

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
    sprintf(dbtbl_sett_clist.seq_no,"%s%-10.10d",sBankTime,nBankNum);     
    //sprintf(dbtbl_sett_clist.seq_no+8, "%010d%s", dbtbl_acct_rslt.seq_num, "01");                    /*结算流水号*/
            
    RightTrim(dbtbl_acct_rslt.plat_sett_bank);
    memcpy(dbtbl_sett_clist.plat_sett_bank,  dbtbl_acct_rslt.plat_sett_bank,         sizeof(dbtbl_sett_clist.plat_sett_bank)-1);                  /*平台结算行*/
    RightTrim(dbtbl_acct_rslt.plat_sett_bank_nm);
    memcpy(dbtbl_sett_clist.plat_sett_bank_nm,    dbtbl_acct_rslt.plat_sett_bank_nm,         sizeof(dbtbl_sett_clist.plat_sett_bank_nm)-1);                    /*平台结算账户账号*/
    memcpy(dbtbl_sett_clist.plat_sett_acct_type,  dbtbl_acct_rslt.plat_sett_acct_type,    sizeof(dbtbl_sett_clist.plat_sett_acct_type)-1);                  /*平台结算账户类型*/
    RightTrim(dbtbl_acct_rslt.plat_sett_acct);
    memcpy(dbtbl_sett_clist.plat_sett_acct,    dbtbl_acct_rslt.plat_sett_acct,         sizeof(dbtbl_sett_clist.plat_sett_acct)-1);                    /*平台结算账户账号*/
    RightTrim(dbtbl_acct_rslt.plat_sett_acct_nm);
    memcpy(dbtbl_sett_clist.plat_sett_acct_nm,  dbtbl_acct_rslt.plat_sett_acct_nm,      sizeof(dbtbl_sett_clist.plat_sett_acct_nm)-1);                  /*平台结算账户名称*/
    RightTrim(dbtbl_acct_rslt.acct_statu);
    memcpy(dbtbl_sett_clist.acct_statu, dbtbl_acct_rslt.acct_statu,             sizeof(dbtbl_sett_clist.acct_statu)-1);                 /*入账状态*/

    if(strcmp(dbtbl_sett_clist.mcht_sett_bank, dbtbl_sett_clist.plat_sett_bank) == 0)
    {       
        dbtbl_sett_clist.plat_amt_fee=0.00;
    }
    else
    {
          dbtbl_sett_clist.plat_amt_fee=0.00;
//        memset(&tbl_brh_fee_inf,0x0,sizeof(tbl_brh_fee_inf));
//        memcpy(tbl_brh_fee_inf.payway_code, tp_payway_ext.channel_code, sizeof(tbl_brh_fee_inf.payway_code)-1);
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
//        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "fee_flag[%s] fee_value[%f]", tbl_brh_fee_inf.fee_flag,tbl_brh_fee_inf.fee_value);                    
    }
    
    //memcpy(dbtbl_sett_clist.chl_id,tp_payway_ext.channel_code,sizeof(dbtbl_sett_clist.chl_id)-1);
    memcpy(dbtbl_sett_clist.chnl_name,"兴业代付",sizeof(dbtbl_sett_clist.chnl_name)-1);
    memcpy(dbtbl_sett_clist.mcht_sett_xingmin_acct,dbtbl_acct_rslt.mcht_sett_xingmin_acct,sizeof(dbtbl_sett_clist.mcht_sett_xingmin_acct)-1);
    
	           
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

int in_sett_list_d(ht_tbl_acct_rslt_def   dbtbl_acct_rslt,tp_payway_ext_def tp_payway_ext)
{
    int nReturnCode;
    char    stlm_dt[8+1];
    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);    
    tbl_sett_list_def   dbtbl_sett_dlist;
    ht_tbl_brh_fee_inf_def tbl_brh_fee_inf;
    
    int     nBankNum = 0;
    char    sBankTime[8+1]; 
    char    sSqlBuf[128];  
    
    if(strncmp(dbtbl_acct_rslt.sett_md, "02", 2) == 0)/*收支两条线*/
    {
        nReturnCode = DbsTblAcctRslt(DBS_SELECT4, &dbtbl_acct_rslt);
        if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_SELECT_D ht_tbl_acct_rslt error [%d]", nReturnCode);
            return -1;
        }
        	    
        memset(&dbtbl_sett_dlist, 0, sizeof(dbtbl_sett_dlist));
        RightTrim(dbtbl_acct_rslt.inter_brh_code);
        memcpy(dbtbl_sett_dlist.brh_id, dbtbl_acct_rslt.inter_brh_code,     sizeof(dbtbl_sett_dlist.brh_id)-1);                    /*内部机构号*/
        memcpy(dbtbl_sett_dlist.sett_date, stlm_dt, 8);                                                                     /*结算日期*/
        memcpy(dbtbl_sett_dlist.trans_date, dbtbl_acct_rslt.acct_date,      sizeof(dbtbl_sett_dlist.trans_date)-1);                /*交易日期*/
        RightTrim(dbtbl_acct_rslt.mcht_no);
        memcpy(dbtbl_sett_dlist.mcht_no,   dbtbl_acct_rslt.mcht_no,       sizeof(dbtbl_sett_dlist.mcht_no)-1);                 /*商户号*/   	        
        RightTrim(dbtbl_acct_rslt.mcht_role);
        memcpy(dbtbl_sett_dlist.mcht_role,   dbtbl_acct_rslt.mcht_role,       sizeof(dbtbl_sett_dlist.mcht_role)-1);                 /*角色类型*/   	        
        RightTrim(dbtbl_acct_rslt.mcht_rslt_no);
        memcpy(dbtbl_sett_dlist.mcht_rslt_no, dbtbl_acct_rslt.mcht_rslt_no, sizeof(dbtbl_sett_dlist.mcht_rslt_no)-1);              /*商户入账流水号*/
        	    
        dbtbl_sett_dlist.amt = dbtbl_acct_rslt.cret_at/100;                                             /*划账金额*/
        	    
        RightTrim(dbtbl_acct_rslt.mcht_debit_bank);
        memcpy(dbtbl_sett_dlist.mcht_sett_bank, dbtbl_acct_rslt.mcht_debit_bank, sizeof(dbtbl_sett_dlist.mcht_sett_bank)-1);              /*商户结算行*/
        RightTrim(dbtbl_acct_rslt.mcht_debit_bank_nm);
        memcpy(dbtbl_sett_dlist.mcht_sett_bank_nm, dbtbl_acct_rslt.mcht_debit_bank_nm, sizeof(dbtbl_sett_dlist.mcht_sett_bank_nm)-1);              /*商户结算行*/
        RightTrim(dbtbl_acct_rslt.mcht_debit_acct_type);
        memcpy(dbtbl_sett_dlist.mcht_sett_acct_type, dbtbl_acct_rslt.mcht_debit_acct_type, sizeof(dbtbl_sett_dlist.mcht_sett_acct_type)-1);         /*商户结算账户类型*/
        RightTrim(dbtbl_acct_rslt.mcht_debit_acct);
        memcpy(dbtbl_sett_dlist.mcht_sett_acct, dbtbl_acct_rslt.mcht_debit_acct, sizeof(dbtbl_sett_dlist.mcht_sett_acct)-1);                  /*商户结算账户账号*/
        RightTrim(dbtbl_acct_rslt.mcht_debit_acct_nm);
        memcpy(dbtbl_sett_dlist.mcht_sett_acct_nm, dbtbl_acct_rslt.mcht_debit_acct_nm, sizeof(dbtbl_sett_dlist.mcht_sett_acct_nm)-1);            /*商户结算账户名称*/
    	        
        memcpy(dbtbl_sett_dlist.dc_flag, "D", 1);                                                                                   /*支出*/
 
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
        sprintf(dbtbl_sett_dlist.seq_no,"%s%-10.10d",sBankTime,nBankNum);       
        //sprintf(dbtbl_sett_dlist.seq_no, "%s%010d%s", dbtbl_sett_clist.sett_date, dbtbl_acct_rslt.seq_num, "02");                   /*结算流水号*/
    	        
        RightTrim(dbtbl_acct_rslt.plat_debit_bank);
        memcpy(dbtbl_sett_dlist.plat_sett_bank, dbtbl_acct_rslt.plat_debit_bank, sizeof(dbtbl_sett_dlist.plat_sett_bank)-1);                 /*平台结算行*/
        RightTrim(dbtbl_acct_rslt.plat_debit_bank_nm);
        memcpy(dbtbl_sett_dlist.plat_sett_bank_nm, dbtbl_acct_rslt.plat_debit_bank_nm, sizeof(dbtbl_sett_dlist.plat_sett_bank_nm)-1);                 /*平台结算行*/        
        memcpy(dbtbl_sett_dlist.plat_sett_acct_type, dbtbl_acct_rslt.plat_debit_acct_type, sizeof(dbtbl_sett_dlist.plat_sett_acct_type)-1);            /*平台结算账户类型*/
        RightTrim(dbtbl_acct_rslt.plat_debit_acct);
        memcpy(dbtbl_sett_dlist.plat_sett_acct, dbtbl_acct_rslt.plat_debit_acct, sizeof(dbtbl_sett_dlist.plat_sett_acct)-1);                     /*平台结算账户账号*/
        RightTrim(dbtbl_acct_rslt.plat_debit_acct_nm);
        memcpy(dbtbl_sett_dlist.plat_sett_acct_nm, dbtbl_acct_rslt.plat_debit_acct_nm, sizeof(dbtbl_sett_dlist.plat_sett_acct_nm)-1);             /*平台结算账户名称*/
        memcpy(dbtbl_sett_dlist.acct_statu, dbtbl_acct_rslt.acct_statu, sizeof(dbtbl_sett_dlist.acct_statu)-1);                    /*入账状态*/

        if(strcmp(dbtbl_sett_dlist.mcht_sett_bank, dbtbl_sett_dlist.plat_sett_bank) == 0)
        {       
            dbtbl_sett_dlist.plat_amt_fee=0.00;
        }
        else
        {
			  dbtbl_sett_dlist.plat_amt_fee=0.00;
//            memset(&tbl_brh_fee_inf,0x0,sizeof(tbl_brh_fee_inf));
//            memcpy(tbl_brh_fee_inf.payway_code, tp_payway_ext.channel_code, sizeof(tbl_brh_fee_inf.payway_code)-1);
//            RightTrim(tbl_brh_fee_inf.payway_code);
//            memcpy(tbl_brh_fee_inf.tran_code, "*", 4);
//            memcpy(tbl_brh_fee_inf.card_type, "00", 2);    
//            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "pay_brh[%s]trans_code[%s];card_type[%s][%s]", tbl_brh_fee_inf.payway_code, tbl_brh_fee_inf.tran_code, tbl_brh_fee_inf.card_type, tbl_brh_fee_inf.payway_code);         
//            nReturnCode = DbsTblBrhFeeInfTxn(DBS_SELECT, &tbl_brh_fee_inf, 0, 0);
//            if (nReturnCode != 0 && nReturnCode!=DBS_NOTFOUND)
//            {
//                    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "tbl_mcht_inf.mcht_no[%s].", tbl_brh_fee_inf.payway_code);
//                    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblBrhFeeInfTxn DBS_SELECT error, %d.", nReturnCode);
//                    return -1;
//            }
//            if(tbl_brh_fee_inf.fee_flag[0] == '1')//1 ： 按笔收费固定值
//            {
//                dbtbl_sett_dlist.plat_amt_fee=tbl_brh_fee_inf.fee_value;        
//            }
//            else//2 ： 按比例收费
//            {
//                dbtbl_sett_dlist.plat_amt_fee=dbtbl_sett_dlist.amt*tbl_brh_fee_inf.fee_value;
//                if(dbtbl_sett_dlist.plat_amt_fee <= tbl_brh_fee_inf.fee_min)
//                {
//                    dbtbl_sett_dlist.plat_amt_fee=tbl_brh_fee_inf.fee_min;            
//                }
//                else if(dbtbl_sett_dlist.plat_amt_fee >= tbl_brh_fee_inf.fee_max)
//                {
//                    dbtbl_sett_dlist.plat_amt_fee=tbl_brh_fee_inf.fee_max;
//                }
//                else
//                {
//                    
//                }       
//            }
//            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "fee_flag[%s] fee_value[%f]", tbl_brh_fee_inf.fee_flag,tbl_brh_fee_inf.fee_value);           
        }                            
    //    memcpy(dbtbl_sett_dlist.chl_id,tp_payway_ext.channel_code,sizeof(dbtbl_sett_dlist.chl_id)-1);
        memcpy(dbtbl_sett_dlist.chnl_name,"兴业代扣",sizeof(dbtbl_sett_dlist.chnl_name)-1);
        memcpy(dbtbl_sett_dlist.mcht_sett_xingmin_acct,dbtbl_acct_rslt.mcht_sett_xingmin_acct,sizeof(dbtbl_sett_dlist.mcht_sett_xingmin_acct)-1);
           	        
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "zhichu:");
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==brh_id:[%s].", dbtbl_sett_dlist.brh_id);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==sett_date:[%s].", dbtbl_sett_dlist.sett_date);      
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==trans_date:[%s].", dbtbl_sett_dlist.trans_date);    
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_role:[%s].", dbtbl_sett_dlist.mcht_role);     
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_rslt_no:[%s].", dbtbl_sett_dlist.mcht_rslt_no);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==seq_no:[%s].", dbtbl_sett_dlist.seq_no);            
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==plat_sett_bank:[%s].", dbtbl_sett_dlist.plat_sett_bank);      
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==plat_sett_bank_nm:[%s].", dbtbl_sett_dlist.plat_sett_bank_nm);      
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==plat_sett_acct_type:[%s].", dbtbl_sett_dlist.plat_sett_acct_type);      
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==plat_sett_acct:[%s].", dbtbl_sett_dlist.plat_sett_acct);          
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==plat_sett_acct_nm:[%s].", dbtbl_sett_dlist.plat_sett_acct_nm);      
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==dc_flag:[%s].", dbtbl_sett_dlist.dc_flag);          
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_sett_xingmin_acct:[%s].", dbtbl_sett_dlist.mcht_sett_xingmin_acct); 
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_sett_bank:[%s].", dbtbl_sett_dlist.mcht_sett_bank);    
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_sett_bank_nm:[%s].", dbtbl_sett_dlist.mcht_sett_bank_nm);    
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_sett_acct_type:[%s].", dbtbl_sett_dlist.mcht_sett_acct_type);    
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_sett_acct:[%s].", dbtbl_sett_dlist.mcht_sett_acct);        
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_sett_acct_nm:[%s].", dbtbl_sett_dlist.mcht_sett_acct_nm);    
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==amt:[%.2f].", dbtbl_sett_dlist.amt);                
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==plat_amt_fee:[%.2f].", dbtbl_sett_dlist.plat_amt_fee);                
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==acct_statu:[%s].", dbtbl_sett_dlist.acct_statu);            
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==chl_id:[%s].", dbtbl_sett_dlist.chl_id);            
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==chnl_name:[%s].", dbtbl_sett_dlist.chnl_name); 
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"); 
    	        
        nReturnCode = preDealSett(&dbtbl_sett_dlist);
        if(nReturnCode)
        {
           HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "preDealSett error.");
            return -1;
        }
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==status:[%s].", dbtbl_sett_dlist.status);
    	        
    	        /* 插入结算明细表 */
        nReturnCode = DbsTblSettList(DBS_INSERT, &dbtbl_sett_dlist);
        if (nReturnCode != 0 && nReturnCode != DBS_KEYDUPLICATE)
        {
            HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblSettList DBS_INSERT error  [%d].", nReturnCode);
            return -1;
        }
        else if (nReturnCode == DBS_KEYDUPLICATE)
        {
            HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblSettList DBS_KEYDUPLICATE, mcht_rslt_no[%s] dc_flag[%s].", dbtbl_sett_dlist.mcht_rslt_no,dbtbl_sett_dlist.dc_flag);
        }
    }
    return 0;   
}


/*************************************************
*函数名称∶ Wait_9069
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶
***************************************************/
int Wait_9069()
{
    int nReturnCode;    
    char    stlm_dt[8+1];
    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);

    tbl_child_time_def dbtbl_child_time;
    memset(&dbtbl_child_time, 0x00, sizeof(dbtbl_child_time));
    memcpy(dbtbl_child_time.mission_date, stlm_dt, 8);
    memcpy(dbtbl_child_time.mission_index, "9069", 4);

    while (1)
    {
        nReturnCode = XbsCldTime(DBS_SELECT3, &dbtbl_child_time);
        if (nReturnCode != 0 && nReturnCode != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "XbsCldTime DBS_SELECT3 err:[%d]stlm_dt[%s]", nReturnCode,stlm_dt);
            return -1;
        }

        if (dbtbl_child_time.child_status == 3)
        {
            break;
        }
        else if (dbtbl_child_time.child_status == 2)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,  "sWaitDate:[%s] Task_9069 err! Please check.", stlm_dt);
            sleep(2);
        }
        else
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "sWaitDate:[%s] waiting Task_9069 finished...", stlm_dt);
            sleep(2);
        }
    }

    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Check Task_9069 [%s]Success.",stlm_dt);

    return 0;
}

int GatherNegativedata() {
    int	 nReturnCode = 0;
	ht_tbl_acct_rslt_def dbtbl_acct_rslt;
	ht_tbl_acct_rslt_def dbtbl_acct_rslt1;
	memset(&dbtbl_acct_rslt, 0x00, sizeof(dbtbl_acct_rslt));
	memset(&dbtbl_acct_rslt1, 0x00, sizeof(dbtbl_acct_rslt1));
	memcpy(dbtbl_acct_rslt.acct_statu, "8", 1);
    nReturnCode = DbsTblAcctRslt(DBS_CURSOR4, &dbtbl_acct_rslt, 0, 0);
	nReturnCode = DbsTblAcctRslt(DBS_OPEN4, &dbtbl_acct_rslt, 0, 0);
	if (nReturnCode)
	{
	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN4 tbl_rslt_sett_cur1 error [%d]", nReturnCode);
	    return -1;
	}
	while(1)
	{
	    memset(&dbtbl_acct_rslt, 0x0, sizeof(dbtbl_acct_rslt));
		memset(&dbtbl_acct_rslt1, 0x00, sizeof(dbtbl_acct_rslt1));
	    nReturnCode = DbsTblAcctRslt(DBS_FETCH4, &dbtbl_acct_rslt,0, 0);   		
        if (nReturnCode && nReturnCode != DBS_FETCHNULL)
        {
            if (nReturnCode == DBS_NOTFOUND)
            {
                break;
            }

	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_FETCH4 tbl_rslt_sett_cur_4 error [%d]", nReturnCode);
	        DbsTblAcctRslt(DBS_CLOSE4, &dbtbl_acct_rslt,0,0);
            return -1;
        }
		memcpy(dbtbl_acct_rslt1.mcht_no, dbtbl_acct_rslt.mcht_no, sizeof(dbtbl_acct_rslt1.mcht_no)-1);
		nReturnCode = DbsTblAcctRslt(DBS_FETCH4, &dbtbl_acct_rslt1,0, 0);
		if (nReturnCode && nReturnCode != DBS_FETCHNULL)
        {
            if (nReturnCode == DBS_NOTFOUND)
            {
                // 更新
                RightTrim(dbtbl_acct_rslt.mcht_rslt_no);
            	RightTrim(dbtbl_acct_rslt.mcht_role);
            	HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_rslt_no:[%s].", dbtbl_acct_rslt.mcht_rslt_no);
            	memcpy(dbtbl_acct_rslt.acct_statu, "7", 1);
            	nReturnCode = DbsTblAcctRslt(DBS_UPDATE, &dbtbl_acct_rslt,0,0);
            	if (nReturnCode == DBS_NOTFOUND)
            	{
            	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPDATE ht_tbl_acct_rslt error [%d]", nReturnCode);
            	    DbsTblAcctRslt(DBS_CLOSE4, &dbtbl_acct_rslt,0,0);
            	    return -1;
            	}
            }

	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_FETCH4 tbl_rslt_sett_cur_4 error [%d]", nReturnCode);
	        DbsTblAcctRslt(DBS_CLOSE4, &dbtbl_acct_rslt,0,0);
            return -1;
        }

		dbtbl_acct_rslt1.debt_at += dbtbl_acct_rslt.debt_at;
		dbtbl_acct_rslt1.cret_at += dbtbl_acct_rslt.cret_at;
		RightTrim(dbtbl_acct_rslt1.mcht_rslt_no);
		RightTrim(dbtbl_acct_rslt1.mcht_role);
		nReturnCode = DbsTblAcctRslt(DBS_UPDATE1, &dbtbl_acct_rslt1,0,0);
        if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPDATE ht_tbl_acct_rslt error [%d]", nReturnCode);
            DbsTblAcctRslt(DBS_CLOSE4, &dbtbl_acct_rslt,0,0);
            return -1;
        }		

	}
	DbsTblAcctRslt(DBS_CLOSE4, &dbtbl_acct_rslt,0,0);
	return 0;	
}

/*************************************************
*函数名称∶ Total_6601
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_6601()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_6601
*函数功能∶ 从入账结果表中抽取数据导入结算明细表,并将结算明细记录进行分类
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_6601 ( int nBeginOffset, int nEndOffset )
{

    int nReturnCode;
    char    sDateTime[14+1];
    char    stlm_dt[8+1];    
    int		nTotalCount = 0;
    
    int c_flag;
    int d_flag;
 
    ht_tbl_acct_rslt_def   dbtbl_acct_rslt;
    tbl_sett_list_def   dbtbl_sett_clist, dbtbl_sett_dlist;
    ht_tbl_ext_mcht_inf_def  tbl_ext_mcht_inf;   
    tp_payway_ext_def tp_payway_ext;
          
    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);

    memset(&tbl_ext_mcht_inf, 0x00, sizeof(tbl_ext_mcht_inf));
    memset(&tp_payway_ext, 0x00, sizeof(tp_payway_ext));
//    nReturnCode = GetAcctInf(&tp_payway_ext,&tbl_ext_mcht_inf);
//	if (nReturnCode)
//	{
//	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "GetAcctInf error [%d]", nReturnCode);
//	    return -1;
//	}
//	CommonRTrim(tp_payway_ext.external_org_no);
//	CommonRTrim(tp_payway_ext.channel_code);

	// 汇总轧差代付金额为负的数据
	nReturnCode = GatherNegativedata();
	if (nReturnCode != 0)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "GatherNegativedata error, %d.", nReturnCode);
        return -1;
    }
	
	memset(sDateTime, 0, sizeof(sDateTime));	 		       
	getSysTime(sDateTime);
    memset(&dbtbl_acct_rslt, 0, sizeof(dbtbl_acct_rslt));
    memcpy(dbtbl_acct_rslt.acct_date, stlm_dt, 8);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "acct_date:[%s], mcht_role:[%s] acct_statu[%s].", dbtbl_acct_rslt.acct_date, "0000|0001","N|R|2");
    
    nReturnCode = DbsTblAcctRslt(DBS_CURSOR1, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);	
	nReturnCode = DbsTblAcctRslt(DBS_OPEN1, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
	if (nReturnCode)
	{
	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN1 tbl_rslt_sett_cur1 error [%d]", nReturnCode);
	    return -1;
	}
	
	while(1)
	{
	    c_flag = 0;
	    d_flag = 0;
	    memset(&dbtbl_acct_rslt, 0x0, sizeof(dbtbl_acct_rslt));
	    nReturnCode = DbsTblAcctRslt(DBS_FETCH1, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);   
        if (nReturnCode && nReturnCode != DBS_FETCHNULL)
        {
            if (nReturnCode == DBS_NOTFOUND)
            {
                break;
            }

	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_FETCH1 tbl_rslt_sett_cur_1 error [%d]", nReturnCode);
	        DbsTblAcctRslt(DBS_CLOSE1, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
            return -1;
        }	    
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "inter_brh_code:[%s].", dbtbl_acct_rslt.inter_brh_code);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_role:[%s].", dbtbl_acct_rslt.mcht_role);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_rslt_no:[%s].", dbtbl_acct_rslt.mcht_rslt_no);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "trans_num:[%d].", dbtbl_acct_rslt.trans_num);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "debt_at:[%.2f].", dbtbl_acct_rslt.debt_at);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "cret_at:[%.2f].", dbtbl_acct_rslt.cret_at);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "debt_net_fee:[%.2f].", dbtbl_acct_rslt.debt_net_fee);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "cret_net_fee:[%.2f].", dbtbl_acct_rslt.cret_net_fee);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_md:[%s].", dbtbl_acct_rslt.sett_md);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_bank:[%s].", dbtbl_acct_rslt.mcht_sett_bank);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_bank_nm:[%s].", dbtbl_acct_rslt.mcht_sett_bank_nm);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_acct_type:[%s].", dbtbl_acct_rslt.mcht_sett_acct_type);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_acct:[%s].", dbtbl_acct_rslt.mcht_sett_acct);            
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_acct_nm:[%s].", dbtbl_acct_rslt.mcht_sett_acct_nm);      
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_sett_bank:[%s].", dbtbl_acct_rslt.plat_sett_bank);            
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_sett_bank_nm:[%s].", dbtbl_acct_rslt.plat_sett_bank_nm);      
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_sett_acct_type:[%s].", dbtbl_acct_rslt.plat_sett_acct_type);  
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_sett_acct:[%s].", dbtbl_acct_rslt.plat_sett_acct);            
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_sett_acct_nm:[%s].", dbtbl_acct_rslt.plat_sett_acct_nm);      
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "acct_statu:[%s].", dbtbl_acct_rslt.acct_statu);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sz_status:[%s].", dbtbl_acct_rslt.sz_status);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "seq_num:[%d].", dbtbl_acct_rslt.seq_num);      
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "========================="); 
		RightTrim(dbtbl_acct_rslt.mcht_rslt_no);
        if(dbtbl_acct_rslt.debt_at <= 0.0000001 && 
            dbtbl_acct_rslt.debt_at >= -0.0000001 &&
            dbtbl_acct_rslt.cret_at <= 0.0000001 && 
            dbtbl_acct_rslt.cret_at >= -0.0000001)
        {
            HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "debt_at 0 [%f]cret_at 0[%f]",dbtbl_acct_rslt.debt_at ,dbtbl_acct_rslt.cret_at);
            continue;
        }
        	    
	    if(strncmp(dbtbl_acct_rslt.sett_md, "01", 2) && 
	       strncmp(dbtbl_acct_rslt.sett_md, "02", 2))
	    {
	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "sett_md:[%s] error.", dbtbl_acct_rslt.sett_md);
	        DbsTblAcctRslt(DBS_CLOSE1, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
	        return -1;
	    }
	    
	    /* mark 若acct_statu='2',则判断sz_status,'1'-取收信息,'2'-取支信息,'3'-取收、支信息 */
	    if(strncmp(dbtbl_acct_rslt.acct_statu, "N", 1)== 0)//N：未入账
	    {
	        if(strncmp(dbtbl_acct_rslt.sett_md, "01", 2) == 0)
	            c_flag = 1;
	        else if(strncmp(dbtbl_acct_rslt.sett_md, "02", 2) == 0)
	        {
	            c_flag = 1;
	            d_flag = 1;
	        }
	    }
	    else if(strncmp(dbtbl_acct_rslt.acct_statu, "2", 1)== 0)//2：挂账恢复
	    {
	        if(strncmp(dbtbl_acct_rslt.sett_md, "01", 2) == 0)
	            c_flag = 1;
	        else if(strncmp(dbtbl_acct_rslt.sett_md, "02", 2) == 0)
	        {
	            c_flag = 1;
	            d_flag = 1;
	        }
	    }
	    else if(strncmp(dbtbl_acct_rslt.acct_statu, "R", 1) == 0)//R：暂缓恢复
	    {
	        if(strncmp(dbtbl_acct_rslt.sett_md, "01", 2) == 0)
	            c_flag = 1;
	        else if(strncmp(dbtbl_acct_rslt.sett_md, "02", 2) == 0)
	        {
	            c_flag = 1;
	            d_flag = 1;
	        }
	    }
	    else
	    {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "acct_statu [%s]  error ", dbtbl_acct_rslt.acct_statu);
            DbsTblAcctRslt(DBS_CLOSE1, &dbtbl_sett_dlist,nBeginOffset,nEndOffset);
            return -1;
	          
	    }
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "c_flag:[%d], d_flag:[%d].", c_flag, d_flag);
	    
        if(c_flag)
        {
            nReturnCode = in_sett_list_c(dbtbl_acct_rslt,tp_payway_ext);
            if (nReturnCode)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_sett_list_c error [%d]", nReturnCode);
                DbsTblAcctRslt(DBS_CLOSE1, &dbtbl_sett_dlist,nBeginOffset,nEndOffset);
                return -1;
            }            
        }

        if(d_flag)
        {
            nReturnCode = in_sett_list_d(dbtbl_acct_rslt,tp_payway_ext);
            if (nReturnCode)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_sett_list_d error [%d]", nReturnCode);
                DbsTblAcctRslt(DBS_CLOSE1, &dbtbl_sett_dlist,nBeginOffset,nEndOffset);
                return -1;
            }             
    	}
    	
    	nTotalCount++;
	    RightTrim(dbtbl_acct_rslt.mcht_rslt_no);
	    RightTrim(dbtbl_acct_rslt.mcht_role);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_rslt_no:[%s].", dbtbl_acct_rslt.mcht_rslt_no);
	    memcpy(dbtbl_acct_rslt.acct_statu, "9", 1);
	    nReturnCode = DbsTblAcctRslt(DBS_UPDATE, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
	    if (nReturnCode == DBS_NOTFOUND)
	    {
	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPDATE ht_tbl_acct_rslt error [%d]", nReturnCode);
	        DbsTblAcctRslt(DBS_CLOSE1, &dbtbl_sett_dlist,nBeginOffset,nEndOffset);
	        return -1;
	    }
	}
	DbsTblAcctRslt(DBS_CLOSE1, &dbtbl_sett_dlist,nBeginOffset,nEndOffset);
 
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nTotalCount:[%d]", nTotalCount);
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_6601 Succ stlm_dt[%s].",stlm_dt);
   
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


/****************************** end of Task_6601.c ******************************/


