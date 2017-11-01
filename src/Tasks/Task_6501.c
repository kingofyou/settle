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
 *  文 件 名: Task_6501.c
 *  功    能: 头寸信息表同步到头寸试算表
 *  编程人员: bcw
 *  开发时间: 2017-03-10
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

extern char	gLogFile[LOG_NAME_LEN_MAX];
extern  tbl_date_inf_def dbtbl_date_inf;
extern	char ext_inter_brh_code[10 + 1];


typedef struct
{
    char    stlm_dt[8+1];
    char    sTransDate[8+1];
    char    sTransTime[6+1];
	char    sMrchNo[8+1];
	char    sAcctNo[32+1];
    char    cny[3+1];
	double dAccountBalance;
	double dAvailableBalanc;
	double dBlockedBalance;
	double dControlledBalance;
} stEpayBalance;


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
/*************************************************
*函数名称∶ WriteFile
*函数功能∶
*输入参数∶ 生成流水文件
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
void WriteFile(stEpayBalance *psEpayBalance, FILE *fp)
{
    fprintf( fp, "%s%s", psEpayBalance->stlm_dt, "|");
    fprintf( fp, "%s%s", psEpayBalance->sTransDate, "|");
    fprintf( fp, "%s%s", psEpayBalance->sTransTime, "|");
    fprintf( fp, "%s%s", psEpayBalance->sMrchNo, "|");
    fprintf( fp, "%s%s", psEpayBalance->sAcctNo, "|");
    fprintf( fp, "%s%s", psEpayBalance->cny, "|");
    fprintf( fp, "%.2f%s", psEpayBalance->dAccountBalance, "|");
    fprintf( fp, "%.2f%s", psEpayBalance->dAvailableBalanc, "|");
    fprintf( fp, "%.2f%s", psEpayBalance->dBlockedBalance, "|");
    fprintf( fp, "%.2f%s", psEpayBalance->dControlledBalance, "|");
    fprintf( fp, "\r\n");
    fflush(fp);

    return;
}

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

int iInsertTblPostionInfo(tbl_postion_info_def ptbl_postion_info)
{
    int iRet = 0;
    tbl_postion_info_def ptbl_postion_info1;
    memset(&ptbl_postion_info1,0x00,sizeof(ptbl_postion_info1));
    memcpy(&ptbl_postion_info1,&ptbl_postion_info,sizeof(ptbl_postion_info));
    HtLog(gLogFile,HT_LOG_MODE_DEBUG,__FILE__,__LINE__,
        "view the data ptbl_postion_info [%s][%s][%s][%f][%s][%f][%f][%f]",
        ptbl_postion_info.ext_mcht_no,
        ptbl_postion_info.bank_code,
        ptbl_postion_info.acct_no,
        ptbl_postion_info.interest,
        ptbl_postion_info.sett_date,
        ptbl_postion_info.bal,
        ptbl_postion_info.acct_bal,
        ptbl_postion_info.resv_bal);
   
    iRet = DbsTblPostInfo(DBS_SELECT1, &ptbl_postion_info1);
    if(iRet == DBS_NOTFOUND)
    {
        iRet = DbsTblPostInfo(DBS_INSERT, &ptbl_postion_info1);
        if(iRet)
        {
            HtLog(gLogFile,HT_LOG_MODE_ERROR,__FILE__,__LINE__,
                "DbsTblPostInfo DBS_INSERT error!:[%d]", iRet);
            return iRet;
        }
    }
    else if(iRet)
    {
        HtLog(gLogFile,HT_LOG_MODE_ERROR,__FILE__,__LINE__,
            "DbsTblPostInfo DBS_SELECT1 error!:[%d]", iRet);
        return iRet;
    }
    else
    {
        memset(&ptbl_postion_info1,0x00,sizeof(ptbl_postion_info1));
        memcpy(&ptbl_postion_info1,&ptbl_postion_info,sizeof(ptbl_postion_info));
        CommonRTrim(ptbl_postion_info1.ext_mcht_no);
        CommonRTrim(ptbl_postion_info1.bank_code);
        CommonRTrim(ptbl_postion_info1.acct_no);
        iRet = DbsTblPostInfo(DBS_UPDATE, &ptbl_postion_info1);
        if(iRet)
        {
            HtLog(gLogFile,HT_LOG_MODE_ERROR,__FILE__,__LINE__,
                      "DbsTblFileInfo DBS_UPDATE error!");
            return iRet;
        }
    }    
    return 0;
}

int iInsertTblPostionTrial(tbl_postion_trial_def ptdbtbl_post_trial)
{
    int iRet = 0;
    tbl_postion_trial_def ptdbtbl_post_trial1;
    memset(&ptdbtbl_post_trial1,0x00,sizeof(ptdbtbl_post_trial1));
    
    memcpy(&ptdbtbl_post_trial1,&ptdbtbl_post_trial,sizeof(ptdbtbl_post_trial1));
    HtLog(gLogFile,HT_LOG_MODE_DEBUG,__FILE__,__LINE__,
        "view the data ptdbtbl_post_trial [%s][%s][%f][%s][%f][%f]",
        ptdbtbl_post_trial1.bank_code,
        ptdbtbl_post_trial1.acct_no,
        ptdbtbl_post_trial1.interest,
        ptdbtbl_post_trial1.sett_date,
        ptdbtbl_post_trial1.bal,
        ptdbtbl_post_trial1.resv_bal);
   
    iRet = DbsTblPostTrial(DBS_SELECT, &ptdbtbl_post_trial1);
    if(iRet == DBS_NOTFOUND)
    {
        iRet = DbsTblPostTrial(DBS_INSERT, &ptdbtbl_post_trial1);
        if(iRet)
        {
            HtLog(gLogFile,HT_LOG_MODE_ERROR,__FILE__,__LINE__,
                "ptdbtbl_post_trial DBS_INSERT error!:[%d]", iRet);
            return iRet;
        }
    }
    else if(iRet)
    {
        HtLog(gLogFile,HT_LOG_MODE_ERROR,__FILE__,__LINE__,
            "ptdbtbl_post_trial DBS_SELECT1 error!:[%d]", iRet);
        return iRet;
    }
    else
    {
        memset(&ptdbtbl_post_trial1,0x00,sizeof(ptdbtbl_post_trial1));
        memcpy(&ptdbtbl_post_trial1,&ptdbtbl_post_trial,sizeof(ptdbtbl_post_trial1));
        CommonRTrim(ptdbtbl_post_trial1.bank_code);
        CommonRTrim(ptdbtbl_post_trial1.acct_no);
        iRet = DbsTblPostTrial(DBS_UPDATE, &ptdbtbl_post_trial1);
        if(iRet)
        {
            HtLog(gLogFile,HT_LOG_MODE_ERROR,__FILE__,__LINE__,
                      "DbsTblPostTrial DBS_UPDATE error!");
            return iRet;
        }
    }    
    return 0;
}


int UpdateTblPostionInfo()
{    
    int	    nReturnCode = 0;
    int     nTotCount=0;
    char    sDateTime[14+1];
    char    *pstr;
    FILE    *fp;
    char    sFileFullName[200];
    char    sTmpFileName[100];
    char    sysTime[14+1];
    char    stlm_dt[8+1];
    
    tbl_file_info_def   dbtbl_file_info;
    tbl_postion_info_def ptbl_postion_info;
    stEpayBalance EpayBalance;
    ht_tbl_com_config_def comconfig;
    stKeyInf KeyInf;

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
    
    /* 初始文件信息 */
    memset(sDateTime, 0x00, sizeof(sDateTime));
    getSysTime(sDateTime);
    memset(&dbtbl_file_info, 0x00, sizeof(dbtbl_file_info));
    memcpy(dbtbl_file_info.sett_date, stlm_dt, 8);
    memcpy(dbtbl_file_info.snd_time, sDateTime, 14);
    memset(sTmpFileName, 0, sizeof(sTmpFileName));
    strcpy(sTmpFileName, "HOST_YYYYMMDD_XX_BALANCE.txt");
    pstr = strstr(sTmpFileName, "YYYYMMDD");
    if (pstr)
        memcpy(pstr, stlm_dt, 8);
    pstr = strstr(sTmpFileName, "XX");
    if (pstr)
        memcpy (pstr, "01", 2);
    memcpy(dbtbl_file_info.step_no, "03", 2);
    memcpy(dbtbl_file_info.batch_no, "01", 2);
    memcpy(dbtbl_file_info.file_type, "H", 1);
    memcpy(dbtbl_file_info.file_flag, "S", 1);
    memcpy(dbtbl_file_info.end_flag, "Y", 1);
    memcpy(dbtbl_file_info.bank_code, "HOST", 4);
    
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
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name : [%s]", dbtbl_file_info.file_name);
    
    fp = fopen(dbtbl_file_info.file_name,"w+");
    if (fp == NULL)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen [%s] error.", dbtbl_file_info.file_name);
        return -1;
    }

    /* 查询外部商户号 */
    memset(&comconfig, 0x00, sizeof(comconfig));
    strcpy(comconfig.key1,"200001");
    strcpy(comconfig.key2,"200001");
    strcpy(comconfig.key3,"BANKCODE");
    nReturnCode = DbsbthComConfig (DBS_SELECT, &comconfig);
     if( nReturnCode )
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsbthComConfig DBS_SELECT[%d]", nReturnCode);
        fclose(fp);
        return nReturnCode;
    }
    CommonRTrim(comconfig.value1);  
    CommonRTrim(comconfig.value2);  
    CommonRTrim(comconfig.value3);  
                
    memset(&KeyInf, 0x00, sizeof(KeyInf));
    nReturnCode = GetKeyInf(&KeyInf, "cib_xydf");
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "GetKeyInf err.");
        fclose(fp);
        return -1;
    }
                  
    stEpay_QueryBalance_Request sEPayReq;
    stEpay_QueryBalance_Response sEPayRsp;
    memset(&sEPayReq,0x0, sizeof(sEPayReq));
    memset(&sEPayRsp,0x0, sizeof(sEPayRsp));
    memset(sysTime,0x00,sizeof(sysTime));      
    CommonGetCurrentTime(sysTime);  
         
    memcpy(sEPayReq.stEpayHead.sTimeStamp,sysTime,sizeof(sEPayReq.stEpayHead.sTimeStamp)-1);
    memcpy(sEPayReq.stEpayHead.sAppid,KeyInf.sMrchNo,sizeof(sEPayReq.stEpayHead.sAppid)-1);
    memcpy(sEPayReq.stEpayHead.sService,"cib.epay.acquire.account.queryBalance",sizeof(sEPayReq.stEpayHead.sService)-1);
    memcpy(sEPayReq.stEpayHead.sVer,"02",sizeof(sEPayReq.stEpayHead.sVer)-1);
    memcpy(sEPayReq.stEpayHead.sSignType,"RSA",sizeof(sEPayReq.stEpayHead.sSignType)-1);
    memcpy(sEPayReq.stEpayHead.sKey,KeyInf.sKey,sizeof(sEPayReq.stEpayHead.sKey)-1);
    memcpy(sEPayReq.stEpayHead.sMrchCert,KeyInf.sMrchCert,sizeof(sEPayReq.stEpayHead.sMrchCert)-1);

    memcpy(sEPayReq.sMac,"0CD2273CC22142AB28EC900403DC7AE6B75ACF61",sizeof(sEPayReq.sMac)-1);  
                
    nReturnCode = QueryBalance(&sEPayReq,&sEPayRsp);
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " QueryBalance error [%d]",nReturnCode);
        fclose(fp);
        return nReturnCode;
    }
    else
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " sMrchNo[%d]",sEPayRsp.sMrchNo);           
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " sAcctNo[%s]",sEPayRsp.sAcctNo);           
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " dAccountBalance[%.2f]",sEPayRsp.dAccountBalance);           
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " dAvailableBalanc[%.2f]",sEPayRsp.dAvailableBalanc);           
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " dBlockedBalance[%.2f]",sEPayRsp.dBlockedBalance);           
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " dControlledBalance[%.2f]",sEPayRsp.dControlledBalance);           
    }                    
    
    memset(&EpayBalance,0x00,sizeof(EpayBalance));
    
    memcpy(EpayBalance.stlm_dt,stlm_dt,8);
    memcpy(EpayBalance.sTransDate,sDateTime,8);
    memcpy(EpayBalance.sTransTime,sDateTime+8,6);   
	memcpy(EpayBalance.sMrchNo,sEPayRsp.sMrchNo,sizeof(EpayBalance.sMrchNo)-1);
	memcpy(EpayBalance.sAcctNo,sEPayRsp.sAcctNo,sizeof(EpayBalance.sAcctNo)-1);
    memcpy(EpayBalance.cny,"156",3);
	EpayBalance.dAccountBalance = sEPayRsp.dAccountBalance;
	EpayBalance.dAvailableBalanc = sEPayRsp.dAvailableBalanc;
	EpayBalance.dBlockedBalance = sEPayRsp.dBlockedBalance;
	EpayBalance.dControlledBalance  = sEPayRsp.dControlledBalance;    
    WriteFile(&EpayBalance, fp);   
    	
    memcpy(dbtbl_file_info.status, "0", 1);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name:[%s]", dbtbl_file_info.file_name);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date:[%s]", dbtbl_file_info.sett_date);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "snd_time :[%s]", dbtbl_file_info.snd_time); 
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "rcv_time :[%s]", dbtbl_file_info.rcv_time); 
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "status   :[%s]", dbtbl_file_info.status);   
    //update
    memset(&ptbl_postion_info,0x00,sizeof(ptbl_postion_info));
    memcpy(ptbl_postion_info.ext_mcht_no,sEPayRsp.sMrchNo,sizeof(ptbl_postion_info.ext_mcht_no)-1);
    memcpy(ptbl_postion_info.acct_no,sEPayRsp.sAcctNo,sizeof(ptbl_postion_info.acct_no)-1);
    ptbl_postion_info.bal = sEPayRsp.dAccountBalance; 
    ptbl_postion_info.acct_bal = sEPayRsp.dAccountBalance;                       /*账户余额*/
    ptbl_postion_info.resv_bal = sEPayRsp.dBlockedBalance;
    ptbl_postion_info.avai_bal = sEPayRsp.dAvailableBalanc;                       /*可用余额*/
    ptbl_postion_info.block_bal = sEPayRsp.dBlockedBalance;                      /*冻结余额*/
    ptbl_postion_info.ctrl_bal = sEPayRsp.dControlledBalance;                       /*控制余额*/
       
    memcpy(ptbl_postion_info.bank_code,comconfig.value1,sizeof(ptbl_postion_info.bank_code)-1);
    memcpy(ptbl_postion_info.bank_name,comconfig.value2,sizeof(ptbl_postion_info.bank_name)-1);
    memcpy(ptbl_postion_info.acct_name,comconfig.value3,sizeof(ptbl_postion_info.acct_name)-1);
    ptbl_postion_info.interest=2.968;
    memcpy(ptbl_postion_info.sett_date,stlm_dt,sizeof(ptbl_postion_info.sett_date)-1); 
    memcpy(ptbl_postion_info.sync_time,sysTime,sizeof(ptbl_postion_info.sync_time)-1);   
  
    nReturnCode = iInsertTblPostionInfo(ptbl_postion_info);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "iInsertTblPostionInfo error, %d.", nReturnCode);
        fclose(fp);
        return -1;
    }
      
    /* 插入文件信息表 */
    nReturnCode = iInsertTblFileInfo(dbtbl_file_info);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "iInsertTblFileInfo error [%d]", nReturnCode);
        fclose(fp);
        return -1;
    }
    fclose(fp);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file [%s] Insert into ht_tbl_file_info success.", dbtbl_file_info.file_name);
     
    HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "UpdateTblPostionInfo Succ .");
    return 0;                  
}

/*************************************************
*函数名称∶ Wait_9029_9030
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶
***************************************************/
int Wait_100001_100005()
{
    int nReturnCode;
    char    stlm_dt[8 + 1];
    
    tbl_child_inf_def        dbtbl_child_inf;

    /* 清算日期 */
    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
    
    while(1)
    {
        memset(&dbtbl_child_inf, 0x00, sizeof(dbtbl_child_inf));
        memcpy(dbtbl_child_inf.inter_brh_code, "100001", BT_BRH_ID_LEN);
        memcpy(dbtbl_child_inf.settlmt_date, stlm_dt, BT_DATE_LEN);
        memcpy(dbtbl_child_inf.mission_index, BT_MISSION_INDEX_9901, BT_MISSION_INDEX_LEN);
        nReturnCode = DbsTblchildInf(DBS_SELECT1, &dbtbl_child_inf);
        if (nReturnCode != 0 && nReturnCode != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "select task status error[%d]", nReturnCode);
            return -1;
        }

        if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_SUSS)
        {
            break;
        }
        else if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_FAIL)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "task excuted error, please check.");
            return -1;
        }
        else
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "inter_brh_code[%s]settlmt_date[%s],task9901 excuting, please hold 30's.", dbtbl_child_inf.inter_brh_code,dbtbl_child_inf.settlmt_date);
            sleep(2);
        }
    }
    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Check inter_brh_code[%s] Task_9901 Success.",dbtbl_child_inf.inter_brh_code);
    while(1)
    {
        memset(&dbtbl_child_inf, 0x00, sizeof(dbtbl_child_inf));
        memcpy(dbtbl_child_inf.inter_brh_code, "100002", BT_BRH_ID_LEN);
        memcpy(dbtbl_child_inf.settlmt_date, stlm_dt, BT_DATE_LEN);
        memcpy(dbtbl_child_inf.mission_index, BT_MISSION_INDEX_9901, BT_MISSION_INDEX_LEN);
        nReturnCode = DbsTblchildInf(DBS_SELECT1, &dbtbl_child_inf);
        if (nReturnCode != 0 && nReturnCode != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "select task status error[%d]", nReturnCode);
            return -1;
        }

        if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_SUSS)
        {
            break;
        }
        else if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_FAIL)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "task excuted error, please check.");
            return -1;
        }
        else
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "inter_brh_code[%s]settlmt_date[%s],task9901 excuting, please hold 30's.", dbtbl_child_inf.inter_brh_code,dbtbl_child_inf.settlmt_date);
            sleep(2);
        }
    }
    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Check inter_brh_code[%s] Task_9901 Success.",dbtbl_child_inf.inter_brh_code);

    while(1)
    {
        memset(&dbtbl_child_inf, 0x00, sizeof(dbtbl_child_inf));
        memcpy(dbtbl_child_inf.inter_brh_code, "100003", BT_BRH_ID_LEN);
        memcpy(dbtbl_child_inf.settlmt_date, stlm_dt, BT_DATE_LEN);
        memcpy(dbtbl_child_inf.mission_index, BT_MISSION_INDEX_9901, BT_MISSION_INDEX_LEN);
        nReturnCode = DbsTblchildInf(DBS_SELECT1, &dbtbl_child_inf);
        if (nReturnCode != 0 && nReturnCode != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "select task status error[%d]", nReturnCode);
            return -1;
        }

        if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_SUSS)
        {
            break;
        }
        else if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_FAIL)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "task excuted error, please check.");
            return -1;
        }
        else
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "inter_brh_code[%s]settlmt_date[%s],task9901 excuting, please hold 30's.", dbtbl_child_inf.inter_brh_code,dbtbl_child_inf.settlmt_date);
            sleep(2);
        }
    }
    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Check inter_brh_code[%s] Task_9901 Success.",dbtbl_child_inf.inter_brh_code);

    while(1)
    {
        memset(&dbtbl_child_inf, 0x00, sizeof(dbtbl_child_inf));
        memcpy(dbtbl_child_inf.inter_brh_code, "100004", BT_BRH_ID_LEN);
        memcpy(dbtbl_child_inf.settlmt_date, stlm_dt, BT_DATE_LEN);
        memcpy(dbtbl_child_inf.mission_index, BT_MISSION_INDEX_9901, BT_MISSION_INDEX_LEN);
        nReturnCode = DbsTblchildInf(DBS_SELECT1, &dbtbl_child_inf);
        if (nReturnCode != 0 && nReturnCode != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "select task status error[%d]", nReturnCode);
            return -1;
        }

        if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_SUSS)
        {
            break;
        }
        else if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_FAIL)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "task excuted error, please check.");
            return -1;
        }
        else
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "inter_brh_code[%s]settlmt_date[%s],task9901 excuting, please hold 30's.", dbtbl_child_inf.inter_brh_code,dbtbl_child_inf.settlmt_date);
            sleep(2);
        }
    }    

    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Check inter_brh_code[%s] Task_9901 Success.",dbtbl_child_inf.inter_brh_code);

    while(1)
    {
        memset(&dbtbl_child_inf, 0x00, sizeof(dbtbl_child_inf));
        memcpy(dbtbl_child_inf.inter_brh_code, "100005", BT_BRH_ID_LEN);
        memcpy(dbtbl_child_inf.settlmt_date, stlm_dt, BT_DATE_LEN);
        memcpy(dbtbl_child_inf.mission_index, BT_MISSION_INDEX_9901, BT_MISSION_INDEX_LEN);
        nReturnCode = DbsTblchildInf(DBS_SELECT1, &dbtbl_child_inf);
        if (nReturnCode != 0 && nReturnCode != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "select task status error[%d]", nReturnCode);
            return -1;
        }

        if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_SUSS)
        {
            break;
        }
        else if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_FAIL)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "task excuted error, please check.");
            return -1;
        }
        else
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "inter_brh_code[%s]settlmt_date[%s],task9901 excuting, please hold 30's.", dbtbl_child_inf.inter_brh_code,dbtbl_child_inf.settlmt_date);
            sleep(2);
        }
    }    
    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Check inter_brh_code[%s] Task_9901 Success.",dbtbl_child_inf.inter_brh_code);


    return 0;
}
/*************************************************
*函数名称∶ Total_6501
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_6501()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_6501
*函数功能∶ 头寸信息表同步到头寸试算表
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_6501 ( int nBeginOffset, int nEndOffset )
{
    int nReturnCode;
    char    stlm_dt[8 + 1];
    int		nTotalCount = 0;
    
    tbl_child_inf_def        dbtbl_child_inf;
    tbl_postion_info_def ptbl_postion_info;
    tbl_postion_trial_def ptbl_postion_trial;
    /* 清算日期 */
    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
        
    /* 等待任务完成 */
    /*nReturnCode = Wait_100001_100005();
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Wait_100001_100005 err.");
        return -1;
    }
    */
    
    nReturnCode = UpdateTblPostionInfo();
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "UpdateTblPostionInfo error, %d.", nReturnCode);
        return -1;
    }
    
    /*memset(&ptbl_postion_trial, 0x00, sizeof(ptbl_postion_trial));
    nReturnCode = DbsTblPostTrial(DBS_TRUNCATE, &ptbl_postion_trial);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblPostTrial DBS_TRUNCATE error, %d.", nReturnCode);
        return -1;
    }
    */

    memset(&ptbl_postion_info, 0x00, sizeof(ptbl_postion_info));
    memcpy(ptbl_postion_info.sett_date, stlm_dt,sizeof(ptbl_postion_info.sett_date)-1);   
    nReturnCode = DbsTblPostInfo(DBS_CURSOR, &ptbl_postion_info);    
    nReturnCode = DbsTblPostInfo(DBS_OPEN, &ptbl_postion_info);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblPostInfo DBS_OPEN error, %d.", nReturnCode);
        return -1;
    }
	while (1)
    {
        nReturnCode = DbsTblPostInfo(DBS_FETCH, &ptbl_postion_info);
        if (nReturnCode == DBS_NOTFOUND)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_FETCH DBS_FETCH end.", nReturnCode);
            break;
        }
        else if(nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblPostInfo DBS_FETCH error, %d.", nReturnCode);
            DbsTblPostInfo(DBS_CLOSE, &ptbl_postion_info);
            return -1;
        }
        nTotalCount++;
        RightTrim(ptbl_postion_info.bank_code);
        RightTrim(ptbl_postion_info.bank_name);
        RightTrim(ptbl_postion_info.acct_no);
        RightTrim(ptbl_postion_info.acct_name);
        RightTrim(ptbl_postion_info.sett_date);
        RightTrim(ptbl_postion_info.sync_time);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "bank_code:[%s],sett_date:[%s],sync_time:[%s]", ptbl_postion_info.bank_code,ptbl_postion_info.sett_date,ptbl_postion_info.sync_time);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "interest:[%.3lf],bal:[%.2lf],resv_bal:[%.2lf]", ptbl_postion_info.interest,ptbl_postion_info.acct_bal,ptbl_postion_info.resv_bal);
        
        memset(&ptbl_postion_trial, 0x00, sizeof(ptbl_postion_trial));
        memcpy(ptbl_postion_trial.bank_code , ptbl_postion_info.bank_code , sizeof(ptbl_postion_trial.bank_code)-1);
        memcpy(ptbl_postion_trial.bank_name , ptbl_postion_info.bank_name , sizeof(ptbl_postion_trial.bank_name)-1);
        memcpy(ptbl_postion_trial.acct_no   , ptbl_postion_info.acct_no   , sizeof(ptbl_postion_trial.acct_no)-1);
        memcpy(ptbl_postion_trial.acct_name , ptbl_postion_info.acct_name , sizeof(ptbl_postion_trial.acct_name)-1);
        memcpy(ptbl_postion_trial.sett_date , ptbl_postion_info.sett_date , sizeof(ptbl_postion_trial.sett_date)-1);
        memcpy(ptbl_postion_trial.sync_time , ptbl_postion_info.sync_time , sizeof(ptbl_postion_trial.sync_time)-1);
        memcpy(ptbl_postion_trial.seq_no, "0", 1);
        ptbl_postion_trial.interest = ptbl_postion_info.interest;
        ptbl_postion_trial.bal = ptbl_postion_info.acct_bal;
        ptbl_postion_trial.resv_bal = ptbl_postion_info.resv_bal;
        ptbl_postion_trial.bal_trial = ptbl_postion_info.acct_bal;
        
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "bank_code:[%s]", ptbl_postion_trial.bank_code);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "bank_name:[%s]", ptbl_postion_trial.bank_name);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "acct_no:[%s]", ptbl_postion_trial.acct_no);    
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "acct_name:[%s]", ptbl_postion_trial.acct_name);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date:[%s]", ptbl_postion_trial.sett_date);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sync_time:[%s]", ptbl_postion_trial.sync_time);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "seq_no:[%s]", ptbl_postion_trial.seq_no);      
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "interest:[%.3lf]", ptbl_postion_trial.interest);  
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "bal:[%.2lf]", ptbl_postion_trial.bal);            
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "resv_bal:[%.2lf]", ptbl_postion_trial.resv_bal);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "bal_trial:[%.2lf]", ptbl_postion_trial.bal_trial);
        nReturnCode = iInsertTblPostionTrial(ptbl_postion_trial);
        if (nReturnCode)
	    {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "iInsertTblPostionTrial DBS_INSERT error, %d.", nReturnCode);
            DbsTblPostInfo(DBS_CLOSE, &ptbl_postion_info);
            return -1;
	    }
    }
    DbsTblPostInfo(DBS_CLOSE, &ptbl_postion_info);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nTotalCount:[%d]", nTotalCount);
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_6501 Succ stlm_dt[%s].",stlm_dt);

    return 0;
}

/****************************** end of Task_6501.c ******************************/


