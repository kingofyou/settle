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
 *  文 件 名: Task_7301.c
 *  功    能: 查询银行批量付款响应文件
 *  编程人员: bcw
 *  开发时间: 2017-03-09
 *  备    注:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include "batch.h"
#include "cJSON.h"
#include "EPay.h"

extern char	gLogFile[LOG_NAME_LEN_MAX];
extern  tbl_date_inf_def dbtbl_date_inf;
extern	char ext_inter_brh_code[10 + 1];

#define BANK_STEP_NO                                     "07"

//int GetKeyInf(stKeyInf *pKeyInf)
//{
//    int nReturnCode;
//    
//    ht_tbl_com_config_def comconfig;
//    tp_payway_ext_def tp_payway_ext;
//    tp_merc_paywayconf_def tp_merc_paywayconf;
//    tp_cert_key_def  tp_cert_key;
//    tp_cert_key_def  tp_cert_key1;
//        
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
//	
//    memset(&tp_cert_key,0x00, sizeof(tp_cert_key));
//    strcpy(tp_cert_key.key,comconfig.value1);
//    strcat(tp_cert_key.key,tp_merc_paywayconf.paying_merc_code);
//    strcat(tp_cert_key.key,comconfig.value2);
//    CommonRTrim(tp_cert_key.key);  
//    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "sKey[%s]", tp_cert_key.key);       
//    nReturnCode = DbsTpCertKey(DBS_SELECT, &tp_cert_key);
//    if (nReturnCode)
//    {
//        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTpCertKey err[%d]",
//               nReturnCode);
//        return -1;
//    }
// 
//    memset(&tp_cert_key1,0x00, sizeof(tp_cert_key1));
//    strcpy(tp_cert_key1.key,comconfig.value1);
//    strcat(tp_cert_key1.key,tp_merc_paywayconf.paying_merc_code);
//    strcat(tp_cert_key1.key,comconfig.value3);
//    CommonRTrim(tp_cert_key1.key);
//    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "sKey1[%s]", tp_cert_key1.key);       
//    nReturnCode = DbsTpCertKey(DBS_SELECT, &tp_cert_key1);
//    if (nReturnCode)
//    {
//        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "tp_cert_key1 err[%d]",
//               nReturnCode);
//        return -1;
//    }
//    CommonRTrim(tp_merc_paywayconf.paying_merc_code); 
//    CommonRTrim(tp_cert_key.value);
//    CommonRTrim(tp_cert_key1.value);     
//    memcpy(pKeyInf->sMrchNo,tp_merc_paywayconf.paying_merc_code,sizeof(tp_merc_paywayconf.paying_merc_code)-1);
//    memcpy(pKeyInf->sKey,tp_cert_key.value,sizeof(pKeyInf->sKey)-1);	            
//    memcpy(pKeyInf->sMrchCert,tp_cert_key1.value,sizeof(pKeyInf->sMrchCert)-1);	
//    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "sMrchNo[%s]sKey[%s]sMrchCert[%s]", pKeyInf->sMrchNo,pKeyInf->sKey,pKeyInf->sMrchCert); 
//    return 0;
//}

int updRsltByBnk(int tran_flag, tbl_sett_list_def* vtdbtbl_sett_list)
{
    int nReturnCode;
    tbl_sett_list_def dbtbl_sett_list;
    ht_tbl_acct_rslt_def   dbtbl_acct_rslt;
    ht_tbl_sett_flow_def tbl_sett_flow;
    
    int succ_flag = tran_flag;
    
    memset(&dbtbl_sett_list, 0, sizeof(dbtbl_sett_list));
    RightTrim(vtdbtbl_sett_list->seq_no);
    memcpy(dbtbl_sett_list.seq_no, vtdbtbl_sett_list->seq_no, sizeof(dbtbl_sett_list.seq_no)-1);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "seq_no:[%s]", dbtbl_sett_list.seq_no);
    nReturnCode = DbsTblSettList(DBS_SELECT, &dbtbl_sett_list);
	if (nReturnCode)
	{
	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_SELECT ht_tbl_sett_list error [%d].", nReturnCode);
	    return -1;
	}
	RightTrim(dbtbl_sett_list.mcht_rslt_no);
	RightTrim(dbtbl_sett_list.mcht_role);
	RightTrim(dbtbl_sett_list.dc_flag);
	RightTrim(dbtbl_sett_list.status);
	HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_rslt_no:[%s],mcht_role[%s],dc_flag:[%s],status:[%s]", 
	dbtbl_sett_list.mcht_rslt_no, 
	dbtbl_sett_list.mcht_role,
	dbtbl_sett_list.dc_flag, 
	dbtbl_sett_list.status);
    if(strncmp(dbtbl_sett_list.mcht_role, "0011",4) == 0 ||
        strncmp(dbtbl_sett_list.mcht_role, "0011",4) == 0)
    {
        //处理提现
 	    memset(&tbl_sett_flow, 0, sizeof(tbl_sett_flow));
	    memcpy(tbl_sett_flow.bank_order_no, vtdbtbl_sett_list->seq_no, sizeof(tbl_sett_flow.bank_order_no)-1);
	    memcpy(tbl_sett_flow.acct_statu, "1", 1);		
		memcpy(tbl_sett_flow.status, "5", 1);
		RightTrim(tbl_sett_flow.bank_order_no);
		nReturnCode = DbsTblSettFlow(DBS_UPDATE3, &tbl_sett_flow, 0, 0);
		if (nReturnCode)
		{
		    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPDATE1 DbsTblSettFlow error [%d].", nReturnCode);
		    return -1;
		}       
        return 0;
    }	
	memset(&dbtbl_acct_rslt, 0x00, sizeof(dbtbl_acct_rslt));
	memcpy(dbtbl_acct_rslt.mcht_rslt_no, dbtbl_sett_list.mcht_rslt_no, sizeof(dbtbl_acct_rslt.mcht_rslt_no)-1);
	memcpy(dbtbl_acct_rslt.mcht_role, dbtbl_sett_list.mcht_role, sizeof(dbtbl_acct_rslt.mcht_role)-1);
	nReturnCode = DbsTblAcctRslt(DBS_SELECT2, &dbtbl_acct_rslt,0,0);
	if (nReturnCode)
	{
	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_SELECT_MD ht_tbl_acct_rslt error [%d].", nReturnCode);
	    return -1;
	}
	RightTrim(dbtbl_acct_rslt.mcht_rslt_no);
	RightTrim(dbtbl_acct_rslt.sett_md);
	HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_md:[%s]", dbtbl_acct_rslt.sett_md);
	if(!strncmp(dbtbl_acct_rslt.sett_md, "01", 2) && 
	    !succ_flag)/* 轧差 */
	{
	    memcpy(dbtbl_acct_rslt.acct_statu, "1", 1);
	}
	else if(!strncmp(dbtbl_acct_rslt.sett_md, "02", 2))/* 收支 */
	{
	    if(!succ_flag)
    	{
    	    if(!strncmp(dbtbl_acct_rslt.acct_statu, "9",1) || 
    	        !strncmp(dbtbl_acct_rslt.acct_statu, "5",1) || 
    	        !strncmp(dbtbl_acct_rslt.acct_statu, "6",1))
    	    {
    	        memcpy(dbtbl_acct_rslt.acct_statu, "1", 1);
    	        if(!strncmp(dbtbl_sett_list.dc_flag, "C", 1))
    	        {
    	            memcpy(dbtbl_acct_rslt.sz_status, "1", 1);   	            
    	        }
    	        else if(!strncmp(dbtbl_sett_list.dc_flag, "D", 1))
    	        {
    	            memcpy(dbtbl_acct_rslt.sz_status, "2", 1);    	            
    	        }
    	    }
    	    else if(!strncmp(dbtbl_acct_rslt.acct_statu, "1",1))
    	    {
    	        memcpy(dbtbl_acct_rslt.sz_status, "3", 1);
    	    }
    	}
	}	
	nReturnCode = DbsTblAcctRslt(DBS_UPDATE, &dbtbl_acct_rslt,0,0);
	if (nReturnCode)
	{
	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPDATE ht_tbl_acct_rslt error [%d].", nReturnCode);
	    return -1;
	}
    
    return 0;
}

/* 银行响应处理 */
int dealCibRsp(stEpay_PaymentGet_Response sEPayRsp)
{
	int nReturnCode;
	double tran_amt;
	double tran_fee;
	int succ_flag;
	char    stlm_dt[8+1]; 

	tbl_postion_info_def dbtbl_post_info;
	tbl_sett_list_def   dbtbl_sett_list;

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
	
	memset(&dbtbl_sett_list, 0, sizeof(dbtbl_sett_list));
	memcpy(dbtbl_sett_list.seq_no, sEPayRsp.sOrderNo, sizeof(dbtbl_sett_list.seq_no)-1);
	HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "seq_no:[%s]", dbtbl_sett_list.seq_no);
	HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "tran_status:[%s]", sEPayRsp.sTransStatus);

	if(strncmp(sEPayRsp.sTransStatus, "1", 1) == 0)/* 银行响应成功 */
	{

	    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "dTransAmt:[%.2f]", sEPayRsp.dTransAmt);
		HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "dTransFee:[%.lf]", sEPayRsp.dTransFee);
		
		/* 银行划付成功 */
		dbtbl_sett_list.amt_fee=sEPayRsp.dTransFee;
		memcpy(dbtbl_sett_list.status, "4", 1);
		nReturnCode = DbsTblSettList(DBS_UPDATE2, &dbtbl_sett_list);
		if (nReturnCode)
		{
		    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPD_SEQ ht_tbl_sett_list error [%d].", nReturnCode);
		    return -1;
		}
		succ_flag = 1;		
	    /* mark 更新入账结果表记录,挂账 */
	    nReturnCode = updRsltByBnk(succ_flag, &dbtbl_sett_list);
    	if (nReturnCode)
    	{
    	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "updRsltByKrnl error [%d].", nReturnCode);
    	    return -1;
    	}		
	}
	else if(strncmp(sEPayRsp.sTransStatus, "3", 1) == 0)/* 银行响应正在处理稍后查询 */
	{
	    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "dTransAmt:[%.2f]", sEPayRsp.dTransAmt);
		HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "dTransFee:[%.2f]", sEPayRsp.dTransFee);
				
		/* 银行划付处理中 */
		dbtbl_sett_list.amt_fee=sEPayRsp.dTransFee;
		memcpy(dbtbl_sett_list.status, "7", 1);
		nReturnCode = DbsTblSettList(DBS_UPDATE2, &dbtbl_sett_list);
		if (nReturnCode)
		{
		    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPD_SEQ ht_tbl_sett_list error [%d].", nReturnCode);
		    return -1;
		}
	}	
	else /* 银行响应失败 */
	{
		/* 银行划付失败 */
		memcpy(dbtbl_sett_list.status, "5", 1);
		
		nReturnCode = DbsTblSettList(DBS_UPD_SEQ, &dbtbl_sett_list);
		if (nReturnCode)
		{
		    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPD_SEQ ht_tbl_sett_list error [%d].", nReturnCode);
		    return -1;
		}
		succ_flag = 0;		
	    /* mark 更新入账结果表记录,挂账 */
	    nReturnCode = updRsltByBnk(succ_flag, &dbtbl_sett_list);
    	if (nReturnCode)
    	{
    	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "updRsltByKrnl error [%d].", nReturnCode);
    	    return -1;
    	}		
	}
	return 0;
}

/*************************************************
*函数名称∶ Total_7301
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_7301()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_7301
*函数功能∶
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_7301 ( int nBeginOffset, int nEndOffset )
{
    int     nReturnCode;
    char    stlm_dt[8+1];
    int		nTotalCount = 0; 
    char    sysTime[14+1];
        
    tbl_sett_list_def   dbtbl_sett_list_dc;
    stEpay_PaymentGet_Request sEPayReq;
    stEpay_PaymentGet_Response sEPayRsp;
    stKeyInf KeyInf;
  
    memset(&sEPayReq,0x0, sizeof(sEPayReq));
    memset(&sEPayRsp,0x0, sizeof(sEPayRsp)); 
    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
 

    memset(&KeyInf, 0x00, sizeof(KeyInf));
    nReturnCode = GetKeyInf(&KeyInf, "cib_xydf");
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "GetKeyInf err.");
        return -1;
    } 
                    
    /* 游标，根据待划账的行号查询结算明细表 */
    memset(&dbtbl_sett_list_dc, 0x00, sizeof(dbtbl_sett_list_dc));
    memcpy(dbtbl_sett_list_dc.sett_date, stlm_dt, 8);
    memcpy(dbtbl_sett_list_dc.status, "7", 1);
    memcpy(dbtbl_sett_list_dc.dc_flag, "C", 1); 
	memcpy(dbtbl_sett_list_dc.chl_id, "XY", 2);
    nReturnCode = DbsTblSettList(DBS_CURSOR1, &dbtbl_sett_list_dc);        
    nReturnCode = DbsTblSettList(DBS_OPEN1, &dbtbl_sett_list_dc);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN1 error, %d.", nReturnCode);
        return -1;
    }
        
    nTotalCount = 0;
        
    while(1)
    {
        /* fetch 每条待划账的记录 */
        memset(&dbtbl_sett_list_dc, 0x00, sizeof(dbtbl_sett_list_dc));
        nReturnCode = DbsTblSettList(DBS_FETCH1, &dbtbl_sett_list_dc);
        if (nReturnCode == DBS_NOTFOUND)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DbsTblSettList DBS_FETCH1 end .");
            break;
        }
        else if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblSettList DBS_FETCH1 error [%d].", nReturnCode);
            DbsTblSettList(DBS_CLOSE1, &dbtbl_sett_list_dc);
            return -1;
        }
        nTotalCount++; 
        
       //查询
        while(1)
        {         
            memset(&sEPayReq,0x0, sizeof(sEPayReq));
            memset(&sEPayRsp,0x0, sizeof(sEPayRsp));
            memset(sysTime,0x00,sizeof(sysTime));      
            CommonGetCurrentTime(sysTime); 
                    
            memcpy(sEPayReq.stEpayHead.sTimeStamp,sysTime,sizeof(sEPayReq.stEpayHead.sTimeStamp)-1);
            memcpy(sEPayReq.stEpayHead.sAppid,KeyInf.sMrchNo,sizeof(sEPayReq.stEpayHead.sAppid)-1);
            memcpy(sEPayReq.stEpayHead.sService,"cib.epay.payment.batchPayApi",sizeof(sEPayReq.stEpayHead.sService)-1);
            memcpy(sEPayReq.stEpayHead.sVer,"02",sizeof(sEPayReq.stEpayHead.sVer)-1);
            memcpy(sEPayReq.stEpayHead.sSignType,"RSA",sizeof(sEPayReq.stEpayHead.sSignType)-1);
            memcpy(sEPayReq.stEpayHead.sKey,KeyInf.sKey,sizeof(sEPayReq.stEpayHead.sKey)-1);
            memcpy(sEPayReq.stEpayHead.sMrchCert,KeyInf.sMrchCert,sizeof(sEPayReq.stEpayHead.sMrchCert)-1);
        
            RightTrim(dbtbl_sett_list_dc.seq_no);
            memcpy(sEPayReq.sOrder_no,dbtbl_sett_list_dc.seq_no,sizeof(sEPayReq.sOrder_no)-1);    
            memcpy(sEPayReq.sMac,"m+M5Bw3YkPdo1kAqnCTYUpaMJl2/Sp/y/YPNFpVyWbJ8REKdg2RXs473wbj/Xc0aAzY8eBU50U3e/wX2jOqrJ09sEXfUZdVku2RDAorqgu+xfGY3b2CmpAzpQIQH7l0PENc2xiI6IKnafC0sqKhv/c8yo/4Vb/hNnG9qtTAHOvm8bPh3/SgpcqnkkjbljnUhGR9aufQ6+DcZV/n2+xJSRZOr4tewKI9TiOoipNwEAVLz64mTRJbLCrZe2zwtgGv1dDfZ+e6EP7oMSPs/HJ60ToZnkWEc/c/hL/kyXw27MqgJBFbq5FsWbYjFLmKKNFz6zB+jG3Ebfz/IBCBtOmSnUw==",sizeof(sEPayReq.sMac)-1);
            
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "PaymentGet begin");
            nReturnCode = PaymentGet(&sEPayReq,&sEPayRsp);
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "PaymentGet end");
            if(nReturnCode)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " PaymentGet error [%d]",nReturnCode);
                DbsTblSettList(DBS_CLOSE1, &dbtbl_sett_list_dc);
                return -1;
            }
            else
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " PaymentGet succ [%d]",nReturnCode); 
                if(strncmp(sEPayRsp.sTransStatus, "3", 1) == 0)
                {
                    
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " sTransStatus[%s]",sEPayRsp.sTransStatus); 
                    sleep(3);
                    continue;                   
                }
                else
                {                    
                    nReturnCode= dealCibRsp(sEPayRsp);
                    if(nReturnCode)
                    {
                        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " dealCibRsp error [%d]",nReturnCode);
                        DbsTblSettList(DBS_CLOSE1, &dbtbl_sett_list_dc);
                        return -1;
                    }                                       
                } 
                break;                                                        
            } 
        }                                          
    }
    
    DbsTblSettList(DBS_CLOSE1, &dbtbl_sett_list_dc);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nTotalCount:[%d]", nTotalCount);
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_7301 Succ .");
    return 0;
}

/****************************** end of Task_7301.c ******************************/