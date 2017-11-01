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
 *  �� �� ��: Task_9006.c
 *  ��    ��: ��ȡ��ҵ���п��֧����ˮ�ļ�
 *  �����Ա: bcw
 *  ����ʱ��: 2017308
 *  ��    ע:
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
////    tp_cert_key_def  tp_cert_key1;
////        
////    memset(&comconfig, 0x00, sizeof(comconfig));
////    strcpy(comconfig.key1,"200001");
////    strcpy(comconfig.key2,"CIBGATEWAY");
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
////    memset(&tp_cert_key1,0x00, sizeof(tp_cert_key1));
////    strcpy(tp_cert_key1.key,comconfig.value1);
////    strcat(tp_cert_key1.key,tp_merc_paywayconf.paying_merc_code);
////    strcat(tp_cert_key1.key,comconfig.value3);
////    CommonRTrim(tp_cert_key1.key);
////    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "sKey1[%s]", tp_cert_key1.key);       
////    nReturnCode = DbsTpCertKey(DBS_SELECT, &tp_cert_key1);
////    if (nReturnCode)
////    {
////        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "tp_cert_key1 err[%d]",
////               nReturnCode);
////        return -1;
////    }
////    CommonRTrim(tp_merc_paywayconf.paying_merc_code); 
////    CommonRTrim(tp_cert_key.value);
////    CommonRTrim(tp_cert_key1.value);     
////    memcpy(pKeyInf->sMrchNo,tp_merc_paywayconf.paying_merc_code,sizeof(tp_merc_paywayconf.paying_merc_code)-1);
////    memcpy(pKeyInf->sKey,tp_cert_key.value,sizeof(pKeyInf->sKey)-1);	            
////    memcpy(pKeyInf->sMrchCert,tp_cert_key1.value,sizeof(pKeyInf->sMrchCert)-1);	
//
//    tbl_key_cert_def tbl_key_cert_inf;
//	memset(&tbl_key_cert_inf, 0, sizeof(tbl_key_cert_inf));
//	strcpy(tbl_key_cert_inf.pay_way, "cib_xydf");
//    nReturnCode = DbsKeyCertInf(DBS_SELECT, &tbl_key_cert_inf);
//	if(nReturnCode) {
//        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsKeyCertInf select err[%d]", nReturnCode);
//		return -1;
//	}
//    CommonRTrim(tbl_key_cert_inf.paying_merc_code); 
//    CommonRTrim(tbl_key_cert_inf.key);   
//	CommonRTrim(tbl_key_cert_inf.merc_cert);  
//    memcpy(pKeyInf->sMrchNo, tbl_key_cert_inf.paying_merc_code,sizeof(tbl_key_cert_inf.paying_merc_code)-1);
//    memcpy(pKeyInf->sKey, tbl_key_cert_inf.key,sizeof(pKeyInf->sKey)-1);
//	memcpy(pKeyInf->sMrchCert,tbl_key_cert_inf.merc_cert,sizeof(pKeyInf->sMrchCert)-1);
//
//    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "sMrchNo[%s]sKey[%s]sMrchCert[%s]", pKeyInf->sMrchNo,pKeyInf->sKey,pKeyInf->sMrchCert); 
//    return 0;
//}

/*************************************************
*�������ơ� DownloadFile
*�������ܡ� �����ļ�
*���������
*���������
*�������ء� 0  -- �ɹ�
            -1  -- ʧ��
***************************************************/
static int DownloadFile(const char *filename,char *filetype)
{
    char    file_path[200 + 1];
    char    file_name[100+1];
    char    sh_path[200 + 1];
    char    stlm_dt[8 + 1];
    char    cmd[1024];
    char    *pstr;
    int     nReturnCode;
    int     i = 0;

    /* �ļ����� */
    memset(stlm_dt, 0x00, sizeof(stlm_dt));
	memcpy( stlm_dt, ext_child_date , 8);

    memset(file_path, 0x00, sizeof(file_path));
    sprintf(file_path, "%s/%s", "/shareDir/cib", filename);

    /* ����ļ��Ƿ������� */
    while(i < REDO_NUM)
    {
        i ++;
        nReturnCode = access(file_path, R_OK);
        if(nReturnCode)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "������ˮ�������ļ�ʧ��!filepath: [%s].", file_path);
            sleep(2);
            continue;
        }
        else
        {
            break;
        }
    }

    memset(sh_path, 0x00, sizeof(sh_path));
    sprintf(sh_path, "%s/sbin", getenv("BATCHHOME"));

    /* ftpȡ�ļ� */
    memset(cmd, 0x00, sizeof(cmd));
    sprintf(cmd, "cd %s; . ./downbankfile.sh %s %s %s", sh_path, stlm_dt, filename,filetype);
    
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "cmd[%s].", cmd);
    nReturnCode = cmdSystem(cmd);
    if(nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "system error: cmd[%s].", cmd);
        return -1;
    }      
    //memset(file_name, 0x00, sizeof(file_name));
    //sprintf(file_name,"BAT_%s_%s.txt",stlm_dt,filetype);
    
    //memset(file_path, 0x00, sizeof(file_path));
    //sprintf(file_path, "%s/%s/%s/%s/%s", getenv("BATCH_FILE_PATH"), stlm_dt,"987654455","HOST", file_name);

    /* ����ļ��Ƿ������� */
    //nReturnCode = access(file_path, R_OK);
    //if(nReturnCode)
    //{
    //    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "������ˮ�������ļ�ʧ��!filepath: [%s].", file_path);
    //    return -1;
    //}

    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "������ˮ�������ļ������سɹ�");

    return 0;
}

/*************************************************
*�������ơ� Total_9006
*�������ܡ�
*���������
*���������
*�������ء� 1
***************************************************/
int Total_9006()
{
    return 1;
}

/*************************************************
*�������ơ� Task_9006
*�������ܡ� ��ȡ��ҵ���п��֧����ˮ�ļ�
*��������� nBeginOffset��nEndOffset
*���������
*�������ء� 0  -- �ɹ�
            -1 -- ʧ��
***************************************************/
int Task_9006 ( int nBeginOffset, int nEndOffset )
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

    stKeyInf KeyInf;
    stEpay_SettleFile_Request sEPayReq;
    stEpay_SettleFile_Response sEPayRsp;
    
    memset(stlm_dt, 0x00, sizeof(stlm_dt));
	memcpy( stlm_dt, ext_child_date , 8);

    memset(&KeyInf, 0x00, sizeof(KeyInf));
    nReturnCode = GetKeyInf(&KeyInf, "cib_xydf");
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "GetKeyInf err.");
        return -1;
    }  
       
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "��ʼ����0-������˻ص��ļ�...");     


    memset(&sEPayReq,0x0, sizeof(sEPayReq));
    memset(&sEPayRsp,0x0, sizeof(sEPayRsp));
    memset(sysDate,0x00,sizeof(sysDate));      
    memset(sysTime,0x00,sizeof(sysTime));      
    CommonGetCurrentTime(sysDate);  
    CommonGetCurrentTime(sysTime);  
     
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "��ʼ����3-����֧�����˻ص��ļ�...");     

    memset(&sEPayReq,0x0, sizeof(sEPayReq));
    memset(&sEPayRsp,0x0, sizeof(sEPayRsp));
    
    memset(sysDate,0x00,sizeof(sysDate));      
    memset(sysTime,0x00,sizeof(sysTime));      
    CommonGetCurrentTime(sysDate);  
    CommonGetCurrentTime(sysTime);  
    
    memcpy(sEPayReq.stEpayHead.sTimeStamp,sysTime,sizeof(sEPayReq.stEpayHead.sTimeStamp)-1);
    memcpy(sEPayReq.stEpayHead.sAppid,KeyInf.sMrchNo,sizeof(sEPayReq.stEpayHead.sAppid)-1);
    memcpy(sEPayReq.stEpayHead.sService,"cib.epay.acquire.settleFile",sizeof(sEPayReq.stEpayHead.sService)-1);
    memcpy(sEPayReq.stEpayHead.sVer,"02",sizeof(sEPayReq.stEpayHead.sVer)-1);
    memcpy(sEPayReq.stEpayHead.sSignType,"RSA",sizeof(sEPayReq.stEpayHead.sSignType)-1);
    memcpy(sEPayReq.stEpayHead.sKey,KeyInf.sKey,sizeof(sEPayReq.stEpayHead.sKey)-1);
    memcpy(sEPayReq.stEpayHead.sMrchCert,KeyInf.sMrchCert,sizeof(sEPayReq.stEpayHead.sMrchCert)-1);

    memcpy(sEPayReq.sTransDate,stlm_dt,sizeof(sEPayReq.sTransDate)-1);
    memcpy(sEPayReq.sTcptType,"3",sizeof(sEPayReq.sTcptType)-1);      
    sprintf(sEPayReq.sFileName,"BAT_%s_%s.zip",stlm_dt,sEPayReq.sTcptType);

    memcpy(sEPayReq.sMac,"0CD2273CC22142AB28EC900403DC7AE6B75ACF61",sizeof(sEPayReq.sMac)-1);  
            
    nReturnCode = settleFile(&sEPayReq,&sEPayRsp);
    if(nReturnCode)
    {
        if(strstr(sEPayRsp.stEpayErr.sErrCode, "EPAY_10008"))
        {
           HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " ����0-������˻ص��Ѿ����� [%s]",sEPayRsp.stEpayErr.sErrCode);              
        }
        else
        {                
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " ����3-����֧�����˻ص�settleFile error [%d]",nReturnCode);
            return -1;
        }
    }
    else
    {
         HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "����3-����֧�����˻ص� settleFile succ [%d]",nReturnCode);           
    } 
 
    nReturnCode = DownloadFile(sEPayReq.sFileName,sEPayReq.sTcptType);         
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " DownloadFile error [%d]",nReturnCode);
        return -1;
    }
    else
    {
         HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DownloadFile succ [%d]",nReturnCode);           
    }
    
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "��ʼ����4-����֧�����˻ص��ļ�...");     

    memset(&sEPayReq,0x0, sizeof(sEPayReq));
    memset(&sEPayRsp,0x0, sizeof(sEPayRsp));
    
    memset(sysDate,0x00,sizeof(sysDate));      
    memset(sysTime,0x00,sizeof(sysTime));      
    CommonGetCurrentTime(sysDate);  
    CommonGetCurrentTime(sysTime);  
    
    memcpy(sEPayReq.stEpayHead.sTimeStamp,sysTime,sizeof(sEPayReq.stEpayHead.sTimeStamp)-1);
    memcpy(sEPayReq.stEpayHead.sAppid,KeyInf.sMrchNo,sizeof(sEPayReq.stEpayHead.sAppid)-1);
    memcpy(sEPayReq.stEpayHead.sService,"cib.epay.acquire.settleFile",sizeof(sEPayReq.stEpayHead.sService)-1);
    memcpy(sEPayReq.stEpayHead.sVer,"02",sizeof(sEPayReq.stEpayHead.sVer)-1);
    memcpy(sEPayReq.stEpayHead.sSignType,"RSA",sizeof(sEPayReq.stEpayHead.sSignType)-1);
    memcpy(sEPayReq.stEpayHead.sKey,KeyInf.sKey,sizeof(sEPayReq.stEpayHead.sKey)-1);
    memcpy(sEPayReq.stEpayHead.sMrchCert,KeyInf.sMrchCert,sizeof(sEPayReq.stEpayHead.sMrchCert)-1);

    memcpy(sEPayReq.sTransDate,stlm_dt,sizeof(sEPayReq.sTransDate)-1);
    memcpy(sEPayReq.sTcptType,"4",sizeof(sEPayReq.sTcptType)-1);      
    sprintf(sEPayReq.sFileName,"BAT_%s_%s.zip",stlm_dt,sEPayReq.sTcptType);

    memcpy(sEPayReq.sMac,"0CD2273CC22142AB28EC900403DC7AE6B75ACF61",sizeof(sEPayReq.sMac)-1);  
            
    nReturnCode = settleFile(&sEPayReq,&sEPayRsp);
    if(nReturnCode)
    {
        if(strstr(sEPayRsp.stEpayErr.sErrCode, "EPAY_10008"))
        {
           HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " ����4-����֧�����˻ص��Ѿ����� [%s]",sEPayRsp.stEpayErr.sErrCode);              
        }
        else
        {                
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " ����4-����֧�����˻ص�settleFile error [%d]",nReturnCode);
            return -1;
        }
    }
    else
    {
         HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "����4-����֧�����˻ص� settleFile succ [%d]",nReturnCode);           
    } 
 
    nReturnCode = DownloadFile(sEPayReq.sFileName,sEPayReq.sTcptType);         
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " DownloadFile error [%d]",nReturnCode);
        return -1;
    }
    else
    {
         HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DownloadFile succ [%d]",nReturnCode);           
    }    
    
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_9006 Succ .");   
    return 0;
}

/****************************** end of Task_9006.c ******************************/


