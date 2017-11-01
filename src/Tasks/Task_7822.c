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
 *  文 件 名: Task_7822.c
 *  功    能: 银行批量付款
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

#define BANK_STEP_NO                                     "09"

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

int iInsertTblFileInfo(tbl_file_info_def   dbtbl_file_info)
{
    int iRet = 0;
    HtLog(gLogFile,HT_LOG_MODE_DEBUG,__FILE__,__LINE__,
        "view the data dbtbl_file_info [%s][%s][%s][%s][%s][%s][%s]",dbtbl_file_info.sett_date,dbtbl_file_info.bank_code,dbtbl_file_info.file_type,dbtbl_file_info.file_flag,dbtbl_file_info.step_no,dbtbl_file_info.batch_no,dbtbl_file_info.file_name);
   
    iRet = DbsTblFileInfo(DBS_SELECT, &dbtbl_file_info);
    if(iRet == DBS_NOTFOUND)
    {
        iRet = DbsTblFileInfo(DBS_INSERT, &dbtbl_file_info);
        if(iRet)
        {
            HtLog(gLogFile,HT_LOG_MODE_ERROR,__FILE__,__LINE__,
                "DbsTblFileInfo DBS_INSERT error!:[%d]", iRet);
            return iRet;
        }
    }
    else if(iRet)
    {
        HtLog(gLogFile,HT_LOG_MODE_ERROR,__FILE__,__LINE__,
            "DbsTblFileInfo DBS_SELECT error!:[%d]", iRet);
        return iRet;
    }
    else
    {
        CommonRTrim(dbtbl_file_info.sett_date);
        CommonRTrim(dbtbl_file_info.file_name);
        iRet = DbsTblFileInfo(DBS_UPDATE, &dbtbl_file_info);
        if(iRet)
        {
            HtLog(gLogFile,HT_LOG_MODE_ERROR,__FILE__,__LINE__,
                      "DbsTblFileInfo DBS_UPDATE error!");
            return iRet;
        }
    }    
    return 0;
}


int WriteBRFile(tbl_file_info_def   dbtbl_file_info1)
{
    int     nReturnCode;
    char    sysDate[9];
    char    sysTime[14+1];
    char    sFileContent[10240];
    char    *pstr;
    char    sFileFullName[200];
    char    sTmpFileName[100];
    char    Filename[100];
    char    sDateTime[14+1];
    FILE    *fp;    
    char    stlm_dt[8+1];
 
    tbl_file_info_def   dbtbl_file_info;

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
    
    memset(sDateTime, 0x00, sizeof(sDateTime));
    getSysTime(sDateTime);
    memset(&dbtbl_file_info, 0x00, sizeof(dbtbl_file_info));
    memcpy(dbtbl_file_info.sett_date, stlm_dt, 8);
    memcpy(dbtbl_file_info.snd_time, sDateTime, 14);
    memset(sTmpFileName, 0, sizeof(sTmpFileName));
    
    memcpy(dbtbl_file_info.step_no, dbtbl_file_info1.step_no, 2);    
    memcpy(dbtbl_file_info.batch_no, dbtbl_file_info1.batch_no, 2);
    memcpy(dbtbl_file_info.file_type, dbtbl_file_info1.file_type, 1);
    memcpy(dbtbl_file_info.file_flag, "R", 1);
    memcpy(dbtbl_file_info.end_flag, "N", 1);
    
    sprintf(sTmpFileName, "BATCH_PAY_YYYYMMDD_%s%s.txt",dbtbl_file_info.step_no,dbtbl_file_info.batch_no);
    pstr = strstr(sTmpFileName, "YYYYMMDD");
    if (pstr)
        memcpy(pstr, stlm_dt, 8);
    memcpy(dbtbl_file_info.bank_code, dbtbl_file_info1.bank_code, sizeof(dbtbl_file_info.bank_code)-1);
     HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "bank_code[%s].", dbtbl_file_info.bank_code);
       
    memset(sFileFullName, 0, sizeof(sFileFullName));
    sprintf(sFileFullName, "%s/%8.8s/%s/%s/%s/%s%s/",
                    getenv("SHARE_FILE_PATH"),
                    dbtbl_file_info.sett_date,
                    dbtbl_file_info.bank_code,
                    dbtbl_file_info.file_type,
                    dbtbl_file_info.file_flag,
                    dbtbl_file_info.step_no,
                    dbtbl_file_info.batch_no);
    nReturnCode = CheckDir(sFileFullName);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "CheckDir err[%d][%s][%s]",
            nReturnCode, strerror(errno), sFileFullName);
        return -1;
    }
    strcat(sFileFullName, sTmpFileName);
    memcpy(dbtbl_file_info.file_name, sFileFullName, sizeof(dbtbl_file_info.file_name)-1);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name:[%s].", dbtbl_file_info.file_name);
    
    fp = fopen(sFileFullName, "w+");
    if (!fp)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen error.\n");
        return -1;
    }
    rewind(fp);
    //fflush(fp);
    fclose(fp);
        
    strcpy(dbtbl_file_info.status, "4");
            /* 插入文件信息表 */
    nReturnCode = iInsertTblFileInfo(dbtbl_file_info);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_INSERT ht_tbl_file_info error [%d].", nReturnCode);
        return -1;
    }                        
    return 0;
}
int iBatchPayApi(char *sFileFullName,tbl_file_info_def   dbtbl_file_info,char *pstFileContent)
{
    int nReturnCode;
    char    sysDate[9];
    char    sysTime[14+1];    
    char    Filename[100];
    char    stlm_dt[8+1];

    stEpay_BatchPayApi_Request sEPayReq;
    stEpay_BatchPayApi_Response sEPayRsp; 
    stKeyInf KeyInf;
    
   //批量资金付款
    memset(&sEPayReq,0x0, sizeof(sEPayReq));
    memset(&sEPayRsp,0x0, sizeof(sEPayRsp));
    memset(sysDate,0x00,sizeof(sysDate));
    memset(sysTime,0x00,sizeof(sysTime));

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
        
    CommonGetCurrentDate(sysDate);
    CommonGetCurrentTime(sysTime);

    memset(&KeyInf,0x00,sizeof(KeyInf));
    nReturnCode = GetKeyInf(&KeyInf, "cib_xydf");
	if (nReturnCode)
	{
	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN tbl_file_info_cur2 error [%d]", nReturnCode);
	    return -1;
	}
        
    memcpy(sEPayReq.stEpayHead.sTimeStamp,sysTime,sizeof(sEPayReq.stEpayHead.sTimeStamp)-1);
    memcpy(sEPayReq.stEpayHead.sAppid,KeyInf.sMrchNo,sizeof(sEPayReq.stEpayHead.sAppid)-1);
    memcpy(sEPayReq.stEpayHead.sService,"cib.epay.payment.batchPayApi",sizeof(sEPayReq.stEpayHead.sService)-1);
    memcpy(sEPayReq.stEpayHead.sVer,"02",sizeof(sEPayReq.stEpayHead.sVer)-1);
    memcpy(sEPayReq.stEpayHead.sSignType,"RSA",sizeof(sEPayReq.stEpayHead.sSignType)-1);
    memcpy(sEPayReq.stEpayHead.sKey,KeyInf.sKey,sizeof(sEPayReq.stEpayHead.sKey)-1);
    memcpy(sEPayReq.stEpayHead.sMrchCert,KeyInf.sMrchCert,sizeof(sEPayReq.stEpayHead.sMrchCert)-1);

    memcpy(sEPayReq.sOrderDate,stlm_dt,sizeof(sEPayReq.sOrderDate)-1);
    sprintf(sEPayReq.sFileName,"BATCH_PAY_%s_%s%s.txt",stlm_dt,dbtbl_file_info.step_no,dbtbl_file_info.batch_no);
    sEPayReq.pFileContent=pstFileContent;
    memcpy(sEPayReq.sMac,"m+M5Bw3YkPdo1kAqnCTYUpaMJl2/Sp/y/YPNFpVyWbJ8REKdg2RXs473wbj/Xc0aAzY8eBU50U3e/wX2jOqrJ09sEXfUZdVku2RDAorqgu+xfGY3b2CmpAzpQIQH7l0PENc2xiI6IKnafC0sqKhv/c8yo/4Vb/hNnG9qtTAHOvm8bPh3/SgpcqnkkjbljnUhGR9aufQ6+DcZV/n2+xJSRZOr4tewKI9TiOoipNwEAVLz64mTRJbLCrZe2zwtgGv1dDfZ+e6EP7oMSPs/HJ60ToZnkWEc/c/hL/kyXw27MqgJBFbq5FsWbYjFLmKKNFz6zB+jG3Ebfz/IBCBtOmSnUw==",sizeof(sEPayReq.sMac)-1);
    nReturnCode = BatchPayApi(&sEPayReq,&sEPayRsp);
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " BatchPayApi error [%d]",nReturnCode);
        if (memcmp(sEPayRsp.stEpayErr.sErrCode, "EPAY_20253", 10) == 0)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " BatchPayApi error [%d] sErrCode[%s] sErrMsg[%s]",nReturnCode,sEPayRsp.stEpayErr.sErrCode,sEPayRsp.stEpayErr.sErrMsg);
            return 0;
        }          
        return nReturnCode;
    }
    else
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " BatchPayApi succ [%d]",nReturnCode);           
    }
    return 0;    
}

int iBatchPayQuery(char *sFileName,tbl_file_info_def   dbtbl_file_info1)
{
    int     nReturnCode;
    char    sysDate[9];
    char    sysTime[14+1];
    char    sFileContent[10240];
    char    *pstr;
    char    sFileFullName[200];
    char    sTmpFileName[100];
    char    Filename[100];
    char    sDateTime[14+1];
    FILE    *fp;
    char    stlm_dt[8+1];
     
    tbl_file_info_def   dbtbl_file_info;
    stKeyInf KeyInf;
    
    stEpay_BatchPayQuery_Request sEPayReq;
    stEpay_BatchPayQuery_Response sEPayRsp;
    memset(Filename,0x00,sizeof(Filename));      
    memset(sysDate, 0, sizeof(sysDate));
    memset(sysTime,0x00,sizeof(sysTime));      
    CommonGetCurrentDate(sysDate);
    CommonGetCurrentTime(sysTime);  

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
   
    memset(&KeyInf,0x00,sizeof(KeyInf));
    nReturnCode = GetKeyInf(&KeyInf, "cib_xydf");
	if (nReturnCode)
	{
	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN tbl_file_info_cur2 error [%d]", nReturnCode);
	    return -1;
	}
    while(1)
    {            
        memset(&sEPayReq,0x0, sizeof(sEPayReq));
        memset(&sEPayRsp,0x0, sizeof(sEPayRsp));
	                          
        memcpy(sEPayReq.stEpayHead.sTimeStamp,sysTime,sizeof(sEPayReq.stEpayHead.sTimeStamp)-1);
        memcpy(sEPayReq.stEpayHead.sAppid,KeyInf.sMrchNo,sizeof(sEPayReq.stEpayHead.sAppid)-1);
        memcpy(sEPayReq.stEpayHead.sService,"cib.epay.payment.batchPayQueryApi",sizeof(sEPayReq.stEpayHead.sService)-1);
        memcpy(sEPayReq.stEpayHead.sVer,"02",sizeof(sEPayReq.stEpayHead.sVer)-1);
        memcpy(sEPayReq.stEpayHead.sSignType,"RSA",sizeof(sEPayReq.stEpayHead.sSignType)-1);
        memcpy(sEPayReq.stEpayHead.sKey,KeyInf.sKey,sizeof(sEPayReq.stEpayHead.sKey)-1);
        memcpy(sEPayReq.stEpayHead.sMrchCert,KeyInf.sMrchCert,sizeof(sEPayReq.stEpayHead.sMrchCert)-1);
        sprintf(sEPayReq.sFileName,"BATCH_PAY_%s_%s%s.txt",stlm_dt,dbtbl_file_info1.step_no,dbtbl_file_info1.batch_no);
        memcpy(sEPayReq.sOrderDate,stlm_dt,sizeof(sEPayReq.sOrderDate)-1);
        memcpy(sEPayReq.sMac,"m+M5Bw3YkPdo1kAqnCTYUpaMJl2/Sp/y/YPNFpVyWbJ8REKdg2RXs473wbj/Xc0aAzY8eBU50U3e/wX2jOqrJ09sEXfUZdVku2RDAorqgu+xfGY3b2CmpAzpQIQH7l0PENc2xiI6IKnafC0sqKhv/c8yo/4Vb/hNnG9qtTAHOvm8bPh3/SgpcqnkkjbljnUhGR9aufQ6+DcZV/n2+xJSRZOr4tewKI9TiOoipNwEAVLz64mTRJbLCrZe2zwtgGv1dDfZ+e6EP7oMSPs/HJ60ToZnkWEc/c/hL/kyXw27MqgJBFbq5FsWbYjFLmKKNFz6zB+jG3Ebfz/IBCBtOmSnUw==",sizeof(sEPayReq.sMac)-1);  
        nReturnCode = BatchPayQuery(&sEPayReq,&sEPayRsp);
        if(nReturnCode)
        {
            if (memcmp(sEPayRsp.stEpayErr.sErrCode, "EPAY_20255", 10) == 0)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " BatchPayQuery error [%d] sErrCode[%s] sErrMsg[%s]",nReturnCode,sEPayRsp.stEpayErr.sErrCode,sEPayRsp.stEpayErr.sErrMsg);
                sleep(2);
                continue;
            }                
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " BatchPayQuery error [%d]",nReturnCode);
            return -1;
        }
        else
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " BatchPayQuery succ [%d]",nReturnCode);
            memset(sFileContent,0x00,sizeof(sFileContent));
            size_t bytes = base64_decode( sEPayRsp.sFileContent, sFileContent);
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " sFileContent[%s][%d]",sFileContent,bytes);
                           
            //0-文件接收成功；
            if (memcmp(sEPayRsp.sBatStatus, "0", 1) == 0)
            {
                sleep(2);
                continue;
            }
            /*else if (memcmp(sEPayRsp.sBatStatus, "3", 1) == 0)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 交易失败；[%s]",sEPayRsp.sBatStatus);
                return -1;
            }*/
            else
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 交易处理完成；[%s]",sEPayRsp.sBatStatus);                
            }                        
            //1-入批次明细成功；
            //2-交易成功；
            //3-交易失败；
            //4-交易部分成功                           
        /* 初始文件信息 */
            memset(sDateTime, 0x00, sizeof(sDateTime));
            getSysTime(sDateTime);
            memset(&dbtbl_file_info, 0x00, sizeof(dbtbl_file_info));
            memcpy(dbtbl_file_info.sett_date, stlm_dt, 8);
            memcpy(dbtbl_file_info.snd_time, sDateTime, 14);
            memset(sTmpFileName, 0, sizeof(sTmpFileName));
            sprintf(sTmpFileName, "BATCH_PAY_YYYYMMDD_%s%s.txt",dbtbl_file_info1.step_no,dbtbl_file_info1.batch_no);
            pstr = strstr(sTmpFileName, "YYYYMMDD");
            if (pstr)
                memcpy(pstr, stlm_dt, 8);
            memcpy(dbtbl_file_info.step_no, BANK_STEP_NO, 2);    
            memcpy(dbtbl_file_info.batch_no, dbtbl_file_info1.batch_no, 2);
            memcpy(dbtbl_file_info.file_type, "B", 1);
            memcpy(dbtbl_file_info.file_flag, "R", 1);
            memcpy(dbtbl_file_info.end_flag, "N", 1);
            memcpy(dbtbl_file_info.bank_code, dbtbl_file_info1.bank_code, sizeof(dbtbl_file_info.bank_code)-1);
        
            memset(sFileFullName, 0, sizeof(sFileFullName));
            sprintf(sFileFullName, "%s/%8.8s/%s/%s/%s/%s%s/",
                    getenv("SHARE_FILE_PATH"),
                    dbtbl_file_info.sett_date,
                    dbtbl_file_info.bank_code,
                    dbtbl_file_info.file_type,
                    dbtbl_file_info.file_flag,
                    dbtbl_file_info.step_no,
                    dbtbl_file_info.batch_no);
            nReturnCode = CheckDir(sFileFullName);
            if (nReturnCode)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "CheckDir err[%d][%s][%s]",
                   nReturnCode, strerror(errno), sFileFullName);
                return -1;
            }
            strcat(sFileFullName, sTmpFileName);
            memcpy(dbtbl_file_info.file_name, sFileFullName, strlen(sFileFullName));
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name:[%s].", dbtbl_file_info.file_name);

    
            fp = fopen(sFileFullName, "w+");
            if (!fp)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen error.\n");
                return -1;
            }
            rewind(fp);
            fprintf( fp, "%s", sFileContent);
            fflush(fp);
            fclose(fp);
        
            strcpy(dbtbl_file_info.status, "4");
            /* 插入文件信息表 */
            nReturnCode = iInsertTblFileInfo(dbtbl_file_info);
            if (nReturnCode)
            {
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "iInsertTblFileInfo error [%d].", nReturnCode);
                return -1;
            }
            return 0;                                   
        }            
    }         
    return 0;    
}

int EPAY_BankBatchPay(char *sFileFullName,tbl_file_info_def   dbtbl_file_info)
{
    int     nReturnCode;

    char    sFileContent[1024*1000];
    char    sFileRecord[RECORD_LEN_MAX];
    FILE    *fp;
    char    base64[1024*1000];   
    int		nTotalNum = 0;
    HtLog( gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "[%s][%s]",getenv("SETT_BAND_NO"),dbtbl_file_info.bank_code);
    if(strstr(getenv("SETT_BAND_NO"),dbtbl_file_info.bank_code))    
    {
                    
        fp = fopen(sFileFullName, "r");
        if (!fp)
        {
            HtLog( gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "fopen sFileFullName[%s]",sFileFullName);        
            return -1;
        }
        rewind(fp);
        memset(&sFileContent, 0x00, sizeof(sFileContent));
        memset(base64, 0x00, sizeof(base64));     
        while(1)
        {
            memset(sFileRecord, 0x00, sizeof(sFileRecord));
            if (fgets(sFileRecord, RECORD_LEN_MAX, fp) == NULL)
            {
                HtLog( gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file data null nTotalNum[%d]",nTotalNum);
                break;
            }
            strcat(sFileContent,sFileRecord);                                         
            nTotalNum++;           
        }
        
        fclose(fp);                        
        if(nTotalNum == 0)
        {
            HtLog( gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "EPAY_BankBatchPay bank_code[%s] nTotalNum=[%d]",dbtbl_file_info.bank_code,nTotalNum);                       
            return 0;
        }
        base64_encode( sFileContent, base64, strlen(sFileContent));                    
        HtLog( gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "iBatchPayApi begin");                            
        nReturnCode = iBatchPayApi(sFileFullName,dbtbl_file_info,base64);                  
        if(nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " iBatchPayApi bank_code[%s] error [%d]",dbtbl_file_info.bank_code,nReturnCode);                    
            return nReturnCode;
        }
        else
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " iBatchPayApi bank_code[%s] succ [%d]",dbtbl_file_info.bank_code,nReturnCode);           
        }
        return 0; 
 
    }
    else
    {
        HtLog( gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "未开通支付渠道bank_code[%s] nTotalNum=[%d]",dbtbl_file_info.bank_code,nTotalNum);
        /*nReturnCode = WriteBRFile(dbtbl_file_info);
        if(nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " WriteBRFile bank_code[%s] error [%d]",dbtbl_file_info.bank_code,nReturnCode);
            DbsTblFileInfo(DBS_CLOSE, &dbtbl_file_info);
            return nReturnCode;
        }*/
        return 0;                    
    }
    return 0;   
}


int EPAY_BatchPayQuery(char *sFileFullName,tbl_file_info_def   dbtbl_file_info)
{
    int     nReturnCode;

    char    sFileContent[10240];
    char    sFileRecord[RECORD_LEN_MAX];
    FILE    *fp;  
    int		nTotalNum = 0;  
    if(strstr(getenv("SETT_BAND_NO"),dbtbl_file_info.bank_code))    
    {                    
        fp = fopen(sFileFullName, "r");
        if (!fp)
        {
            HtLog( gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "fopen sFileFullName[%s]",sFileFullName);        
            return -1;
        }
        rewind(fp);
    
        while(1)
        {
            memset(sFileRecord, 0x00, sizeof(sFileRecord));
            if (fgets(sFileRecord, RECORD_LEN_MAX, fp) == NULL)
            {
                HtLog( gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file data null nTotalNum[%d]",nTotalNum);
                break;
            }                       
            nTotalNum++;           
        }
        fclose(fp);    
                    
        if(nTotalNum == 0)
        {
            HtLog( gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "EPAY_BankBatchPay bank_code[%s] nTotalNum=[%d]",dbtbl_file_info.bank_code,nTotalNum);                       
            nReturnCode = WriteBRFile(dbtbl_file_info);       
            if(nReturnCode)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " WriteBRFile bank_code[%s] error [%d]",dbtbl_file_info.bank_code,nReturnCode);
                return nReturnCode;
            }
            return 0;
        }
        if(strstr(dbtbl_file_info.batch_no,"01"))
        {
            HtLog( gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "等待兴业银行批量代付处理");
            sleep(2);           
        }       
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "iBatchPayQuery开始[%s].", sFileFullName);
        nReturnCode = iBatchPayQuery(sFileFullName,dbtbl_file_info);
        if(nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "iBatchPayQuery error [%d]", nReturnCode);
            return -1;
        }
        else
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " iBatchPayQuery bank_code[%s] succ [%d]",dbtbl_file_info.bank_code,nReturnCode);           
        }
        return 0; 
 
    }
    else
    {
        HtLog( gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "未开通支付渠道bank_code[%s] nTotalNum=[%d]",dbtbl_file_info.bank_code,nTotalNum);
        nReturnCode = WriteBRFile(dbtbl_file_info);
        if(nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " WriteBRFile bank_code[%s] error [%d]",dbtbl_file_info.bank_code,nReturnCode);
            return nReturnCode;
        }
        return 0;                    
    }    
    return 0;
}
/*************************************************
*函数名称∶ Total_7822
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_7822()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_7822
*函数功能∶
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_7822 ( int nBeginOffset, int nEndOffset )
{
    int     nReturnCode;
    char    sDateTime[14+1];
    char    stlm_dt[8+1];
    char    sFileFullName[200];  
    int		nFileCount = 0;

    tbl_file_info_def   dbtbl_file_info;
    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);

    /* 初始文件信息 */
    memset(sDateTime, 0x00, sizeof(sDateTime));
    getSysTime(sDateTime);
    memset(&dbtbl_file_info, 0x00, sizeof(dbtbl_file_info));
    memcpy(dbtbl_file_info.sett_date, stlm_dt, 8);
    memcpy(dbtbl_file_info.step_no, BANK_STEP_NO, 2);
    //memcpy(dbtbl_file_info.batch_no, "02", 2);
    memcpy(dbtbl_file_info.file_type, "B", 1);
    memcpy(dbtbl_file_info.file_flag, "S", 1);
    memcpy(dbtbl_file_info.status, "0", 1);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date:[%s], step_no:[%s], file_type:[%s],file_flag[%s] status:[%s]", 
	                        dbtbl_file_info.sett_date, dbtbl_file_info.step_no, dbtbl_file_info.file_type, dbtbl_file_info.file_flag, dbtbl_file_info.status);
	
	/* 游标 文件信息表获取文件名和银行编号 */
	DbsTblFileInfo(DBS_CURSOR, &dbtbl_file_info);
	nReturnCode = DbsTblFileInfo(DBS_OPEN, &dbtbl_file_info);
	if (nReturnCode)
	{
	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN tbl_file_info_cur2 error [%d]", nReturnCode);
	    return -1;
	}
    
    while(1)
    {
        /* FETCH数据，获取文件名 */
	    memset(&dbtbl_file_info, 0, sizeof(dbtbl_file_info));
	    nReturnCode = DbsTblFileInfo(DBS_FETCH, &dbtbl_file_info);
	    if (nReturnCode == DBS_NOTFOUND)
	    {
	        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_FETCH tbl_file_info_cur2 end.nFileCount=[%d]",nFileCount);
	        break;
	    }
	    else if (nReturnCode)
	    {
	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_FETCH tbl_file_info_cur2 error [%d].", nReturnCode);
	        DbsTblFileInfo(DBS_CLOSE, &dbtbl_file_info);
	        return -1;
	    }
	    nFileCount++;
	    RightTrim(dbtbl_file_info.bank_code);
	    RightTrim(dbtbl_file_info.file_name);
	    memset(sFileFullName, 0x00, sizeof(sFileFullName));
	    memcpy(sFileFullName, dbtbl_file_info.file_name, strlen(dbtbl_file_info.file_name));
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "正在确认银行[%s][%d]批量付款响应文件:[%s]...", dbtbl_file_info.bank_code,nFileCount, sFileFullName);
    
        while (1)
        {
            /* 确认文件是否存在 */
            nReturnCode = access(sFileFullName, R_OK);
            if (nReturnCode != 0)
            {
                sleep(2);
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "access sFileFullName[%s].", sFileFullName);
                continue;
            }
            else
            {
                
                //更新状态文件状态
                //交通银行
                //农业银行...               
                //兴业银行
        	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "EPAY_BankBatchPay开始[%s].", sFileFullName);
        	    nReturnCode = EPAY_BankBatchPay(sFileFullName,dbtbl_file_info);
        	    if(nReturnCode)
        	    {
        	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "EPAY_BankBatchPay error [%d]", nReturnCode);
        	        DbsTblFileInfo(DBS_CLOSE, &dbtbl_file_info);
        	        return -1;
        	    }
        	    else
        	    {
        	        strcpy(dbtbl_file_info.status, "1");
                    /* 插入文件信息表 */
                    nReturnCode = iInsertTblFileInfo(dbtbl_file_info);
                    if (nReturnCode)
                    {
                        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_INSERT ht_tbl_file_info error [%d].", nReturnCode);
                        return -1;
                    }                                 	        
        	    }
        	    break; 
        	}

        }
    }
    DbsTblFileInfo(DBS_CLOSE, &dbtbl_file_info);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nFileCount[%d].....",nFileCount);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "等待兴业银行批量代付.....");
    
    nFileCount= 0;
    /* 初始文件信息 */
    memset(sDateTime, 0x00, sizeof(sDateTime));
    getSysTime(sDateTime);
    memset(&dbtbl_file_info, 0x00, sizeof(dbtbl_file_info));
    memcpy(dbtbl_file_info.sett_date, stlm_dt, 8);
    memcpy(dbtbl_file_info.step_no, BANK_STEP_NO, 2);
    //memcpy(dbtbl_file_info.batch_no, "01", 2);
    memcpy(dbtbl_file_info.file_type, "B", 1);
    memcpy(dbtbl_file_info.file_flag, "S", 1);
    memcpy(dbtbl_file_info.status, "1", 1);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date:[%s], step_no:[%s], file_type:[%s],file_flag[%s] status:[%s]", 
	                        dbtbl_file_info.sett_date, dbtbl_file_info.step_no, dbtbl_file_info.file_type, dbtbl_file_info.file_flag, dbtbl_file_info.status);
	
	/* 游标 文件信息表获取文件名和银行编号 */
	DbsTblFileInfo(DBS_CURSOR, &dbtbl_file_info);
	nReturnCode = DbsTblFileInfo(DBS_OPEN, &dbtbl_file_info);
	if (nReturnCode)
	{
	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN tbl_file_info_cur2 error [%d]", nReturnCode);
	    return -1;
	}
    
    while(1)
    {
        /* FETCH数据，获取文件名 */
	    memset(&dbtbl_file_info, 0, sizeof(dbtbl_file_info));
	    nReturnCode = DbsTblFileInfo(DBS_FETCH, &dbtbl_file_info);
	    if (nReturnCode == DBS_NOTFOUND)
	    {
	        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_FETCH tbl_file_info_cur2 end.nFileCount=[%d]",nFileCount);
	        break;
	    }
	    else if (nReturnCode)
	    {
	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_FETCH tbl_file_info_cur2 error [%d].", nReturnCode);
	        DbsTblFileInfo(DBS_CLOSE, &dbtbl_file_info);
	        return -1;
	    }
	    nFileCount++;
	    RightTrim(dbtbl_file_info.bank_code);
	    RightTrim(dbtbl_file_info.file_name);
	    
	    memset(sFileFullName, 0x00, sizeof(sFileFullName));
	    memcpy(sFileFullName, dbtbl_file_info.file_name, strlen(dbtbl_file_info.file_name));
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "正在确认银行[%s][%d]批量付款响应文件:[%s]...", dbtbl_file_info.bank_code,nFileCount, sFileFullName);
    
        while (1)
        {
            /* 确认文件是否存在 */
            nReturnCode = access(sFileFullName, R_OK);
            if (nReturnCode != 0)
            {
                sleep(2);
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "access sFileFullName[%s].", sFileFullName);
                continue;
            }
            else
            {

        	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "EPAY_BatchPayQuery开始[%s].", sFileFullName);
        	    nReturnCode = EPAY_BatchPayQuery(sFileFullName,dbtbl_file_info);
        	    if(nReturnCode)
        	    {
        	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "EPAY_BankBatchPay error [%d]", nReturnCode);
        	        DbsTblFileInfo(DBS_CLOSE, &dbtbl_file_info);
        	        return -1;
        	    }
        	    else
        	    {
        	        strcpy(dbtbl_file_info.status, "4");
                    /* 插入文件信息表 */
                    nReturnCode = iInsertTblFileInfo(dbtbl_file_info);
                    if (nReturnCode)
                    {
                        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_INSERT ht_tbl_file_info error [%d].", nReturnCode);
                        return -1;
                    }              	        
        	    }
        	}
        	break;

        }
    }
    DbsTblFileInfo(DBS_CLOSE, &dbtbl_file_info);        
     HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nFileCount:[%d]", nFileCount);
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_7811 Succ .");
   
        
    return 0;
}
/****************************** end of Task_7822.c ******************************/