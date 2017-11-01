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
 *  文 件 名: Task_9004.c
 *  功    能: 获取兴业银行微信、支付宝流水文件
 *  编程人员: phb
 *  开发时间: 20170410
 *  备    注:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include <sys/socket.h>
#include "batch.h"
#include "Common.h"
#include "cJSON.h"
#include "EPay.h"

extern  char    gLogFile[LOG_NAME_LEN_MAX];
extern  char    ext_child_date[8 + 1];
extern  char    ext_inter_brh_code[10 + 1];
extern  int     ext_inter_brh_sta;

#define REDO_NUM		            100

//int GetKeyInf(stKeyInf *pKeyInf)
//{
//    int nReturnCode = 0;
//    
////    ht_tbl_com_config_def comconfig;
////    tp_payway_ext_def tp_payway_ext;
////    tp_merc_paywayconf_def tp_merc_paywayconf;
////    tp_cert_key_def  tp_cert_key;
////        
////    memset(&comconfig, 0x00, sizeof(comconfig));
////    strcpy(comconfig.key1,"200001");
////    strcpy(comconfig.key2,"CIBTENPAY");
////    strcpy(comconfig.key3,"PAYWAYTYPE");
////    nReturnCode = DbsbthComConfig (DBS_SELECT, &comconfig);
////     if( nReturnCode )
////    {
////        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsbthComConfig DBS_SELECT[%d]", nReturnCode);
////        return nReturnCode;
////    }
////
////    CommonRTrim(comconfig.value1);  
////    CommonRTrim(comconfig.value2);  
////    CommonRTrim(comconfig.value3);
////    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "[%s][%s][%s]", comconfig.value1,comconfig.value2,comconfig.value3); 
////    
////    memset(&tp_payway_ext, 0x00, sizeof(tp_payway_ext));
////    memcpy(tp_payway_ext.payway_type,comconfig.value1,sizeof(tp_payway_ext.payway_type)-1);    
////    nReturnCode = DbsTpPaywayExt (DBS_SELECT, &tp_payway_ext);
////	if (nReturnCode)
////	{
////	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTpPaywayExt error [%d]", nReturnCode);
////	    return -1;
////	}	
////	CommonRTrim(tp_payway_ext.payway_code);
////	memset(&tp_merc_paywayconf,0x0,sizeof(tp_merc_paywayconf));
////	memcpy(tp_merc_paywayconf.payway_code,tp_payway_ext.payway_code,sizeof(tp_merc_paywayconf.payway_code)-1);
////	HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "payway_code[%s]", tp_merc_paywayconf.payway_code);
////	nReturnCode = DbsTpMercPaywayConf (DBS_SELECT, &tp_merc_paywayconf);    
////	if (nReturnCode)
////	{
////	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTpMercPaywayConf error [%d]", nReturnCode);
////	    return -1;
////	}
////	CommonRTrim(tp_merc_paywayconf.paying_merc_code);
////	HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "paying_merc_code[%s]", tp_merc_paywayconf.paying_merc_code);
////	
////    memset(&tp_cert_key,0x00, sizeof(tp_cert_key));
////    strcpy(tp_cert_key.key,comconfig.value1);
////    strcat(tp_cert_key.key,tp_merc_paywayconf.paying_merc_code);
////    strcat(tp_cert_key.key,comconfig.value2);
////    CommonRTrim(tp_cert_key.key);  
////    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "sKey[%s]", tp_cert_key.key);       
////    nReturnCode = DbsTpCertKey(DBS_SELECT, &tp_cert_key);
////    if (nReturnCode)
////    {
////        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTpCertKey err[%d]",
////               nReturnCode);
////        return -1;
////    }
//// 
////    CommonRTrim(tp_merc_paywayconf.paying_merc_code); 
////    CommonRTrim(tp_cert_key.value);    
////    memcpy(pKeyInf->sMrchNo,tp_merc_paywayconf.paying_merc_code,sizeof(tp_merc_paywayconf.paying_merc_code)-1);
////    memcpy(pKeyInf->sKey,tp_cert_key.value,sizeof(pKeyInf->sKey)-1);	
//
//    tbl_key_cert_def tbl_key_cert_inf;
//	memset(&tbl_key_cert_inf, 0, sizeof(tbl_key_cert_inf));
//	strcpy(tbl_key_cert_inf.pay_way, "cib_tenpay");
//    nReturnCode = DbsKeyCertInf(DBS_SELECT, &tbl_key_cert_inf);
//	if(nReturnCode) {
//        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsKeyCertInf select err[%d]", nReturnCode);
//		return -1;
//	}
//    CommonRTrim(tbl_key_cert_inf.paying_merc_code); 
//    CommonRTrim(tbl_key_cert_inf.key);    
//    memcpy(pKeyInf->sMrchNo, tbl_key_cert_inf.paying_merc_code,sizeof(tbl_key_cert_inf.paying_merc_code)-1);
//    memcpy(pKeyInf->sKey, tbl_key_cert_inf.key,sizeof(pKeyInf->sKey)-1);	
//
//    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "sMrchNo[%s]sKey[%s]sMrchCert[%s]", pKeyInf->sMrchNo,pKeyInf->sKey,pKeyInf->sMrchCert); 
//    return 0;
//}
/*************************************************
*函数名称∶ Total_9004
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_9004()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_9004
*函数功能∶ 获取兴业银行快捷支付流水文件
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_9004 ( int nBeginOffset, int nEndOffset )
{
    int     nReturnCode;
    char    sDateTime[14+1];
    char    stlm_dt[8 + 1];
    char    sysDate[9];
    char    sysTime[14+1];

    char    *pstr;
    FILE    *fp;
    char    sFileFullName[200];
    char    sTmpFileName[100];

    char	host_file_name[200] = {0};
    tbl_key_cert_def KeyInf;

    stEpay_Merchant_Request sEPayReq;
    stEpay_Merchant_Response sEPayRsp;

    
    stExtnFileRegDef StExtnFileReg;
    int flag = 0;

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
	memcpy( stlm_dt, ext_child_date , 8);

    memset(&KeyInf, 0x00, sizeof(KeyInf));
	strcpy(KeyInf.pay_way, "cib_tenpay");
    strcpy(KeyInf.merc_type, "1");
	nReturnCode = DbsKeyCertInf(DBS_CURSOR, &KeyInf);
	if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsKeyCertInf cursor err. nReturnCode = [%d].", nReturnCode);
        return -1;
    }
    nReturnCode = DbsKeyCertInf(DBS_OPEN, &KeyInf);
	if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsKeyCertInf open err. nReturnCode = [%d].", nReturnCode);
        return -1;
    }
	while(1) {    
		memset(&KeyInf, 0x00, sizeof(KeyInf));
        nReturnCode = DbsKeyCertInf(DBS_FETCH, &KeyInf);
		if(nReturnCode && nReturnCode != DBS_NOTFOUND) {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsKeyCertInf fetch err. nReturnCode = [%d].", nReturnCode);
            return -1;
		}
		else if(nReturnCode == DBS_NOTFOUND) {
		    break;
	    }   
		CommonRTrim(KeyInf.paying_merc_code); 
        CommonRTrim(KeyInf.key);   
	    CommonRTrim(KeyInf.merc_cert); 

        memset(&sEPayReq,0x0, sizeof(sEPayReq));
        memset(&sEPayRsp,0x0, sizeof(sEPayRsp));
        memset(sysDate,0x00,sizeof(sysDate));      
        memset(sysTime,0x00,sizeof(sysTime));      
        CommonGetCurrentTime(sysDate);  
        CommonGetCurrentTime(sysTime);  
        
        memcpy(sEPayReq.sService,"pay.bill.merchant",sizeof(sEPayReq.sService)-1);
        memcpy(sEPayReq.sVersion,"1.0",sizeof(sEPayReq.sVersion)-1);
        memcpy(sEPayReq.sCharset,"UTF-8",sizeof(sEPayReq.sCharset)-1);
        
        memcpy(sEPayReq.sBillDate,stlm_dt,sizeof(sEPayReq.sBillDate)-1);
        memcpy(sEPayReq.sBillType,"ALL",sizeof(sEPayReq.sBillType)-1);
        memcpy(sEPayReq.sSignType,"MD5",sizeof(sEPayReq.sSignType)-1);
        memcpy(sEPayReq.sMchId,KeyInf.paying_merc_code,sizeof(sEPayReq.sMchId)-1);
        memcpy(sEPayReq.sKey,KeyInf.key,sizeof(sEPayReq.sKey)-1);

        /* 初始文件信息 */
        memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
        memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
        memcpy(StExtnFileReg.stlm_dt, stlm_dt, 8);
        memcpy(StExtnFileReg.host_name, "WECHAT", 6);
        sprintf(StExtnFileReg.src_file_name, "%s_YYYYMMDD.dat", KeyInf.paying_merc_code);

        pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
        if (pstr)
            memcpy (pstr, stlm_dt, 8);
        pstr = strstr (StExtnFileReg.src_file_name, "XX");
        if (pstr)
            memcpy (pstr, ext_inter_brh_code+4, 2);

        getShareFileFullName(&StExtnFileReg, host_file_name);
        memcpy(sEPayReq.sFileName,host_file_name,sizeof(sEPayReq.sFileName)-1);        
            
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "开始微信商户号[%s]流水文件...",sEPayReq.sMchId);     
         HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "流水文件[%s]...",sEPayReq.sFileName); 

        nReturnCode = Merchant(&sEPayReq,&sEPayRsp);
        if(nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " Merchant error [%d]",nReturnCode);
            return -1;
        }
        else
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " Merchant succ [%d]",nReturnCode); 
            /* 检查文件是否已下载 */
            nReturnCode = access(host_file_name, R_OK);
            if(nReturnCode)
            {
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "下载流水处理结果文件失败!filepath: [%s].", host_file_name);
                flag = 1;
            }            
        }
	}
	DbsKeyCertInf(DBS_CLOSE, &KeyInf);
	if(flag) return -1;

    return 0;
}

/****************************** end of Task_9004.c ******************************/


