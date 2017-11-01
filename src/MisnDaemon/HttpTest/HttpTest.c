/* * ************************************************************************** */
/*                NCUP -- Shanghai Huateng Software System Inc.              */
/* * ************************************************************************** */
/* PROGRAM NAME: main.pc                                                     */
/* DESCRIPTIONS: main function for batch                                     */
/* * ************************************************************************** */
/*                        MODIFICATION LOG                                   */
/* DATE        PROGRAMMER     DESCRIPTION                                    */
/* 2008-10-20  harrison       initial                                        */
/* * ************************************************************************** */

#include "batch.h"
#include "cJSON.h"
#include "EPay.h"

#define LOG_ALLOC_GRAN			(1024)

char ext_inter_brh_code[10 + 1];
int ext_inter_brh_sta = 0;
char gLogFile[LOG_NAME_LEN_MAX];
int unpackEncryptMsgRdm(unsigned char *RecvEncryptMsg);
int packMsg(unsigned char *RecvEncryptMsg);
int WriteFile(char *sysDate,char *sFileContent);

/*************************************************
 * 函数名称：getValueByDelim
 * 函数描述：根据分隔符将源字符串sSrc中第一个分隔符sDelim之前的字符串复制到目的串sDest中
 *          并将源字符串sSrc的指针移动到第一个分隔符之后
 *函数参数： sSrc  源字符串
 *           sDest 目的字符串
 *           sDelim 分隔符
 *函数返回：  0 - 成功
 *            1 - 源字符串已到达尾部
 *            2 - 出错
***************************************************/
int getValueByDelim(char *sSrc, char *sDest, char *sDelim )
{
    char *sEnd;

    if( *sSrc == '\0')
    {
        return 1;
    }

    sEnd = strstr(sSrc, sDelim);
    if(sEnd == NULL)
    {
        if(strstr(sSrc, "\n") != NULL)
        {
            sEnd = sSrc + strlen(sSrc) - 1;
        }
        else
        {
            sEnd = sSrc + strlen(sSrc);
        }
    }

    memcpy(sDest, sSrc, sEnd - sSrc);

    memcpy(sSrc, sEnd + (int)strlen(sDelim), strlen(sSrc));
    return 0;
}

char *rtrimjson(char *pstring)
{
    int j;

    j = strlen(pstring);
    if (j <= 0)
        return(pstring);
    while ((j != 0) && (pstring[j - 1] == '\\'))
        j--;
    pstring[j] = '\0';
    return(pstring);
}

void CommonRTrimjson(char *caDest)
{
    int i;
    for( i = strlen(caDest) - 1 ; i >= 0 ; i-- )
    {
        if( caDest[i] != '"')
        {
            break;
        }
    }
    caDest[i + 1] = '\0';
}

int CommonLTrimjson( char *caDest)
{
    int i;
    int j = 0;
    char lsTmp[2048];
    memset(lsTmp, 0, sizeof(lsTmp));

    for( i = 0; i < strlen(caDest) ; i++ )
    {
        if( caDest[i] == '\\')
        {
            continue;
        }
        lsTmp[j++] = caDest[i];
    }
    HtStrcpy(caDest, lsTmp);

    return 0;
}

int iTest()
{
    char buf1[2024+1];
    memset(buf1,0,sizeof(buf1));
    char buf[2024+1]="{\\\"bat_id\\\":\\\"2017051002342874\\\",\\\"file_content\\\":\\\"MjAxNzA1MTAwMDAwMDAwMDAxfHwyMDE3LTA1LTEwfDEwNTAwN3wxMTcwMTAxMDAxMDA0MzQ1ODF8wffQx9PqfDYyMjkwOTQ0MzQ0MjAxOTUxNHy73b7ZfDB8MTAyLjAyfMX6wb+0+ri2fDAuMDB8MXx8DQoyMDE3MDUxMDAwMDAwMDAwMDJ8fDIwMTctMDUtMTB8MTA1MDA4fDExNzAxMDEwMDEwMDQzNDU4MXzB99DH0+p8NjIyOTA4MTIxMDAwMTI3NjExfM30zfR8MHw3LjY4fMX6wb+0+ri2fDAuMDB8MXx8DQoyMDE3MDUxMDAwMDAwMDAwMDR8fDIwMTctMDUtMTB8MTA1MDA3fDExNzAxMDEwMDEwMDQzNDU4MXzB99DH0+p8NjIyOTA4MTIxMDAwMTI3NTEyfLi1zOzSu3wwfDE5LjE2fMX6wb+0+ri2fDAuMDB8MXx8DQoyMDE3MDUxMDAwMDAwMDAwMDN8fDIwMTctMDUtMTB8MTA1MDA3fDExNzAxMDEwMDEwMDQzNDU4MXzB99DH0+p8NjIyOTA4MTIxMDAwMTI3NjExfM30zfR8MHwxNS45OHzF+sG/tPq4tnwwLjAwfDF8fA0K\\\",\\\"fileName\\\":\\\"Q0000905_2017051002342874.out\\\",\\\"batStatus\\\":\\\"2\\\",\\\"sign_type\\\":\\\"RSA\\\",\\\"orderDate\\\":\\\"20170510\\\",\\\"batStatusLabel\\\":\\\"?o¤??“??????\\\",\\\"mac\\\":\\\"BnZWKzDDhY+8m0NUsbnaXZ3YJxmbanaJF/PG8D0jCM6uDhX0Dg9XTzbog2s0Zze9AytyTRC1BBazKEGdTYtaNU0NneRPNJTdl7uq2UDRRmoYSWUTnatLCCG0s7B5WnvhBZ8sXEU7LRsBCxNDmff3VxIcySmPrexJsnO7yxSzOpqPgkPEhy9U6ukFI04mzCayB/0hPQS0/J7u2AoLbTbvPJmhVlQvF58Gekfnn2DZi2dtsJgNfAp2k/0MhZxyO9KgIDyVAgwNhZ3hh79mxXLsJJY9Z4hN1eUoIrgaxQ9aEqm9Yj+pRNB46SP1LfgJZcXv9wR9dOc8cAWVCa5Nn3gMqw==\\\"}";
    sprintf(buf1,"%s",buf);
    //memcpy(buf1,buf,strlen(buf));
    //printf("%s\n\n",buf1);
    CommonLTrimjson(buf1);
    //printf("%s\n",buf1);
    
    
}
int iTestTopHttpCall(char *Filename,char *stlm_dt,char *KeyPath)
{
    int     nReturnCode =0;
    char    sysDate[9];
    char    sysTime[14+1];
 
	char    fName[128];
    char    Tmpbf[4096] = {0}; 
    char    sSoapCfgName[32+1] = {0};  /*配置文件名*/
    char    sRspBody[4096] = {0};
    int     iRspBodyLen = 0;

    int     iTmpLen=0;
    
    char    keyfile[300];
    memset(keyfile,0x00,sizeof(keyfile));    
    sprintf(keyfile,"%sappsvr_clientQ0000905.pfx",KeyPath);
    
    char *m_pJSONData= NULL;
    int m_nJSONAllocLen = 0;
    int m_nJSONDataLen = 0;
    int nLen = 0;

    m_nJSONAllocLen = LOG_ALLOC_GRAN;
    m_pJSONData = malloc (m_nJSONAllocLen);

    memset(sysDate,0x00,sizeof(sysDate));
    memset(sysTime,0x00,sizeof(sysTime));        
    CommonGetCurrentDate(sysDate);       
    CommonGetCurrentTime(sysTime);
             
    HttpRequest pstHttpRequest;
    HttpResponse pstHttpResponse;
    

    memset(&pstHttpRequest, 0x0, sizeof(pstHttpRequest));
    memset(&pstHttpResponse, 0x0, sizeof(pstHttpResponse));
    memset(sSoapCfgName,0x0,sizeof(sSoapCfgName));
    memset(Tmpbf,0x0,sizeof(Tmpbf));
    memset(fName, 0, sizeof(fName));    

	HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "============================================", sSoapCfgName);
    strcpy(sSoapCfgName,"httpcfg.cfg");
    CommonGetCurrentDate(sysDate);
    sprintf(fName, "%s%s%s%s%s.jpg", "905111122222E000", "10000001", "123456", "123456", sysDate);

	cJSON* root = cJSON_CreateObject();  	
		
    cJSON_AddStringToObject(root, "timestamp", sysTime);  
    cJSON_AddStringToObject(root, "appid", "Q0000905");  
    cJSON_AddStringToObject(root, "service","cib.epay.acquire.account.queryBalance");  
    cJSON_AddStringToObject(root, "ver", "02");  
    cJSON_AddStringToObject(root, "sign_type", "RSA");  
    cJSON_AddStringToObject(root, "key", "6A9E618FDF824F90AF72D9EF54D9BA32");  
    cJSON_AddStringToObject(root, "mrch_cert", keyfile);  

    cJSON_AddStringToObject(root, "mac", "0CD2273CC22142AB28EC900403DC7AE6B75ACF61");  
    
    nLen = strlen(cJSON_Print(root));           
    if (m_nJSONDataLen + nLen > m_nJSONAllocLen)
    {
        int nNewAllocLen = m_nJSONAllocLen * 2;
        if (nNewAllocLen < m_nJSONDataLen + nLen)
        {
            nNewAllocLen = (m_nJSONDataLen + nLen + LOG_ALLOC_GRAN - 1) 
                / LOG_ALLOC_GRAN * LOG_ALLOC_GRAN;
        }
        char *pSendNew = malloc (nNewAllocLen);
        if (m_nJSONDataLen > 0)
            memcpy(pSendNew, m_pJSONData, m_nJSONDataLen);
        free (m_pJSONData);
        m_pJSONData = pSendNew;
        m_nJSONAllocLen = nNewAllocLen;
    }
    memcpy(m_pJSONData + m_nJSONDataLen, cJSON_Print(root), nLen);
    m_nJSONDataLen += nLen;             	
    pstHttpRequest.pReqBody=m_pJSONData;  
    
    memcpy(pstHttpRequest.sHttpPost,"/front-converter/direct/queryBalance.do",sizeof(pstHttpRequest.sHttpPost)-1);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "pReqBody[%s] iTmpLen[%d].",pstHttpRequest.pReqBody,m_nJSONDataLen);
    nReturnCode = TopHttpCall(&sSoapCfgName, &pstHttpRequest, &pstHttpResponse, &fName,m_nJSONDataLen);
    if (m_pJSONData)
    {
        free (m_pJSONData);
        m_pJSONData = NULL;
    }      
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " TopHttpCall error [%d]",nReturnCode);
        return 0;
    }    
    if(pstHttpResponse.stRspHead.iHttpStatus == 0)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 发送报文失败[%d]",pstHttpResponse.stRspHead.iHttpStatus);
    }
    else
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 发送报文成功[%d]sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);
    
    }
    return 0;          
}


//批量资金单笔查询
int iPaymentGet(char *Filename,char *stlm_dt,char *KeyPath)
{
    int     nReturnCode =0;
    char    sysDate[9];
    char    sysTime[14+1];
    char    base64[4096];

        
    char *m_pSendData= NULL;
    int m_nSendAllocLen = 0;
    int m_nSendDataLen = 0;
    int nLen = 0;
	    
    FILE    *fp;
    char    *pstr;
    char    sFileFullName[100];
    char    sFileRecord[RECORD_LEN_MAX];
           
    char    keyfile[300];
    memset(keyfile,0x00,sizeof(keyfile));    
    sprintf(keyfile,"%sappsvr_clientQ0000905.pfx",KeyPath);
    
    //批量资金付款
    stEpay_PaymentGet_Request sEPayReq;
    stEpay_PaymentGet_Response sEPayRsp;
    memset(&sEPayReq,0x0, sizeof(sEPayReq));
    memset(&sEPayRsp,0x0, sizeof(sEPayRsp));
    memset(sysDate,0x00,sizeof(sysDate));
    memset(sysTime,0x00,sizeof(sysTime));
        
    char binbug[2050];
    memset(binbug,0x00,sizeof(binbug));
    memset(sysDate, 0, sizeof(sysDate));
    CommonGetCurrentDate(sysDate);
        
    CommonGetCurrentTime(sysTime);
        

    memset(sFileFullName, 0, sizeof(sFileFullName));
    sprintf(sFileFullName, "%s/%8.8s/",
    getenv("BATCH_FILE_PATH"),
    sysDate); 
    nReturnCode = CheckDir(sFileFullName);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "CheckDir err[%d][%s][%s]",
        nReturnCode, strerror(errno), sFileFullName);
        return -1;
    }
    strcat(sFileFullName, Filename);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name : [%s]", sFileFullName);
            
    fp = fopen(sFileFullName, "r");
    if (!fp)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen error.\n");
        return -1;
    }
    rewind(fp);
        
    m_nSendAllocLen = LOG_ALLOC_GRAN;
    m_pSendData = malloc (m_nSendAllocLen);
    m_nSendDataLen = 0;	
    
	char order_no[32+1];
	memset(order_no,0x0,sizeof(order_no));       
    while(1)
    {
        memset(&sFileRecord, 0x00, sizeof(sFileRecord));
        if (fgets(sFileRecord, RECORD_LEN_MAX, fp) == NULL)
        {
            HtLog( gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file data null");
            break;
        }
        memset(order_no,0x0,sizeof(order_no));       
        getValueByDelim(sFileRecord, order_no, "|" );
        
        memcpy(sEPayReq.stEpayHead.sTimeStamp,sysTime,sizeof(sEPayReq.stEpayHead.sTimeStamp)-1);
        memcpy(sEPayReq.stEpayHead.sAppid,"Q0000905",sizeof(sEPayReq.stEpayHead.sAppid)-1);
        memcpy(sEPayReq.stEpayHead.sService,"cib.epay.payment.batchPayApi",sizeof(sEPayReq.stEpayHead.sService)-1);
        memcpy(sEPayReq.stEpayHead.sVer,"02",sizeof(sEPayReq.stEpayHead.sVer)-1);
        memcpy(sEPayReq.stEpayHead.sSignType,"RSA",sizeof(sEPayReq.stEpayHead.sSignType)-1);
        memcpy(sEPayReq.stEpayHead.sKey,"6A9E618FDF824F90AF72D9EF54D9BA32",sizeof(sEPayReq.stEpayHead.sKey)-1);
        memcpy(sEPayReq.stEpayHead.sMrchCert,keyfile,sizeof(sEPayReq.stEpayHead.sMrchCert)-1);
    
        memcpy(sEPayReq.sOrder_no,order_no,sizeof(sEPayReq.sOrder_no)-1);    
        memcpy(sEPayReq.sMac,"m+M5Bw3YkPdo1kAqnCTYUpaMJl2/Sp/y/YPNFpVyWbJ8REKdg2RXs473wbj/Xc0aAzY8eBU50U3e/wX2jOqrJ09sEXfUZdVku2RDAorqgu+xfGY3b2CmpAzpQIQH7l0PENc2xiI6IKnafC0sqKhv/c8yo/4Vb/hNnG9qtTAHOvm8bPh3/SgpcqnkkjbljnUhGR9aufQ6+DcZV/n2+xJSRZOr4tewKI9TiOoipNwEAVLz64mTRJbLCrZe2zwtgGv1dDfZ+e6EP7oMSPs/HJ60ToZnkWEc/c/hL/kyXw27MqgJBFbq5FsWbYjFLmKKNFz6zB+jG3Ebfz/IBCBtOmSnUw==",sizeof(sEPayReq.sMac)-1);
        
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "BatchPayApi begin");
        nReturnCode = PaymentGet(&sEPayReq,&sEPayRsp);
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "BatchPayApi end");
        if(nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " PaymentGet error [%d]",nReturnCode);
            return 0;
        }
        else
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " PaymentGet succ [%d]",nReturnCode);           
        }              
             
    }

    fclose(fp);
                    
  
    return 0;   
}
//批量资金付款
int iBatchPayApi(char *Filename,char *stlm_dt,char *KeyPath)
{
    int     nReturnCode =0;
    char    sysDate[9];
    char    sysTime[14+1];
    char    base64[4096];

        
    char *m_pSendData= NULL;
    int m_nSendAllocLen = 0;
    int m_nSendDataLen = 0;
    int nLen = 0;
	    
    FILE    *fp;
    char    *pstr;
    char    sFileFullName[100];
    char    sFileRecord[RECORD_LEN_MAX];

    char    keyfile[300];
    memset(keyfile,0x00,sizeof(keyfile));    
    sprintf(keyfile,"%sappsvr_clientQ0000905.pfx",KeyPath);
            
    
    //批量资金付款
    stEpay_BatchPayApi_Request sEPayReq;
    stEpay_BatchPayApi_Response sEPayRsp;
    memset(&sEPayReq,0x0, sizeof(sEPayReq));
    memset(&sEPayRsp,0x0, sizeof(sEPayRsp));
    memset(sysDate,0x00,sizeof(sysDate));
    memset(sysTime,0x00,sizeof(sysTime));
        
    char binbug[4096*10];
    memset(binbug,0x00,sizeof(binbug));
    memset(sysDate, 0, sizeof(sysDate));
    CommonGetCurrentDate(sysDate);
        
    CommonGetCurrentTime(sysTime);
        

    memset(sFileFullName, 0, sizeof(sFileFullName));
    sprintf(sFileFullName, "%s/%8.8s/",
    getenv("BATCH_FILE_PATH"),
    sysDate); 
    nReturnCode = CheckDir(sFileFullName);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "CheckDir err[%d][%s][%s]",
        nReturnCode, strerror(errno), sFileFullName);
        return -1;
    }
    strcat(sFileFullName, Filename);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name : [%s]", sFileFullName);
            
    fp = fopen(sFileFullName, "r");
    if (!fp)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen error.\n");
        return -1;
    }
    rewind(fp);
        
    m_nSendAllocLen = LOG_ALLOC_GRAN;
    m_pSendData = malloc (m_nSendAllocLen);
    m_nSendDataLen = 0;	
    
    char base641[4096*10];
	size_t bytes = 0;           
    while(1)
    {
        memset(&sFileRecord, 0x00, sizeof(sFileRecord));
        if (fgets(sFileRecord, RECORD_LEN_MAX, fp) == NULL)
        {
            HtLog( gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file data null");
            break;
        }
        /*strcat(binbug,sFileRecord);
        //strcat(sFileRecord,"\r\n");
        base64_encode( sFileRecord, base64, strlen(sFileRecord));            
        nLen = strlen(base64); 
        
    memset(base641,0,sizeof(base641));
    bytes = base64_decode(  base64, base641);
    //printf( "%s\n", base641 );
    //printf( "%d\n", bytes );  
                    
        if (m_nSendDataLen + nLen > m_nSendAllocLen)
        {
            int nNewAllocLen = m_nSendAllocLen * 2;
            if (nNewAllocLen < m_nSendDataLen + nLen)
            {
                nNewAllocLen = (m_nSendDataLen + nLen + LOG_ALLOC_GRAN - 1) 
        				/ LOG_ALLOC_GRAN * LOG_ALLOC_GRAN;
            }
            char *pSendNew = malloc (nNewAllocLen);
            if (m_nSendDataLen > 0)
                memcpy(pSendNew, m_pSendData, m_nSendDataLen);
            free (m_pSendData);
            m_pSendData = pSendNew;
        	m_nSendAllocLen = nNewAllocLen;
        }
	    memcpy(m_pSendData + m_nSendDataLen, base64, nLen);
	    m_nSendDataLen += nLen;    
	    */
	   strcat(binbug,sFileRecord);                  
    }

    fclose(fp);
    //printf( "%s\n", "===============");
    base64_encode( binbug, base64, strlen(binbug));  
    memset(base641,0,sizeof(base641));
     bytes = base64_decode(  base64, base641);
    //printf( "%s\n", base641 );
    //printf( "%d\n", bytes );    
    
    //printf( "%s\n", "===============");
    memset(base641,0,sizeof(base641));
     bytes = base64_decode(  m_pSendData, base641);
    //printf( "%s\n", base641 );
    //printf( "%d\n", bytes );
 
    memset(base641,0,sizeof(base641));
    bytes = base64_decode(  binbug, base641);
    //printf( "%s\n", base641 );
    //printf( "%d\n", bytes );    
                       
    memcpy(sEPayReq.stEpayHead.sTimeStamp,sysTime,sizeof(sEPayReq.stEpayHead.sTimeStamp)-1);
    memcpy(sEPayReq.stEpayHead.sAppid,"Q0000905",sizeof(sEPayReq.stEpayHead.sAppid)-1);
    memcpy(sEPayReq.stEpayHead.sService,"cib.epay.payment.batchPayApi",sizeof(sEPayReq.stEpayHead.sService)-1);
    memcpy(sEPayReq.stEpayHead.sVer,"02",sizeof(sEPayReq.stEpayHead.sVer)-1);
    memcpy(sEPayReq.stEpayHead.sSignType,"RSA",sizeof(sEPayReq.stEpayHead.sSignType)-1);
    memcpy(sEPayReq.stEpayHead.sKey,"6A9E618FDF824F90AF72D9EF54D9BA32",sizeof(sEPayReq.stEpayHead.sKey)-1);
    memcpy(sEPayReq.stEpayHead.sMrchCert,keyfile,sizeof(sEPayReq.stEpayHead.sMrchCert)-1);

    memcpy(sEPayReq.sOrderDate,sysDate,sizeof(sEPayReq.sOrderDate)-1);
    sprintf(sEPayReq.sFileName,"%s",Filename);
 
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "m_nSendDataLen[%d][%d][%s]",m_nSendDataLen,strlen(m_pSendData),m_pSendData);
    sEPayReq.pFileContent = base64;
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "sFileContent[%s]",sEPayReq.pFileContent);

    memcpy(sEPayReq.sMac,"m+M5Bw3YkPdo1kAqnCTYUpaMJl2/Sp/y/YPNFpVyWbJ8REKdg2RXs473wbj/Xc0aAzY8eBU50U3e/wX2jOqrJ09sEXfUZdVku2RDAorqgu+xfGY3b2CmpAzpQIQH7l0PENc2xiI6IKnafC0sqKhv/c8yo/4Vb/hNnG9qtTAHOvm8bPh3/SgpcqnkkjbljnUhGR9aufQ6+DcZV/n2+xJSRZOr4tewKI9TiOoipNwEAVLz64mTRJbLCrZe2zwtgGv1dDfZ+e6EP7oMSPs/HJ60ToZnkWEc/c/hL/kyXw27MqgJBFbq5FsWbYjFLmKKNFz6zB+jG3Ebfz/IBCBtOmSnUw==",sizeof(sEPayReq.sMac)-1);
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "BatchPayApi begin");
    nReturnCode = BatchPayApi(&sEPayReq,&sEPayRsp);
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "BatchPayApi end");
    if (m_pSendData)
    {
        free (m_pSendData);
        m_pSendData = NULL;
    }  
       
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " BatchPayApi error [%d]",nReturnCode);
        return 0;
    }
    else
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " BatchPayApi succ [%d]",nReturnCode);           
    }        
    return 0;   
}
//批量代付查询
int iBatchPayQuery(char *Filename,char *stlm_dt,char *KeyPath)
{
    int     nReturnCode =0;
    char    sysDate[9];
    char    sysTime[14+1];    
    char sFileContent[10240];

    FILE    *fp;
    char    *pstr;
    char    sFileFullName[100];
    char    sTmpFileName[30];  

    char    keyfile[300];
    memset(keyfile,0x00,sizeof(keyfile));    
    sprintf(keyfile,"%sappsvr_clientQ0000905.pfx",KeyPath);

      
    stEpay_BatchPayQuery_Request sEPayReq;
    stEpay_BatchPayQuery_Response sEPayRsp;
    memset(&sEPayReq,0x0, sizeof(sEPayReq));
    memset(&sEPayRsp,0x0, sizeof(sEPayRsp));
    memset(sysTime,0x00,sizeof(sysTime));      
    CommonGetCurrentTime(sysTime);  
       
    memcpy(sEPayReq.stEpayHead.sTimeStamp,sysTime,sizeof(sEPayReq.stEpayHead.sTimeStamp)-1);
    memcpy(sEPayReq.stEpayHead.sAppid,"Q0000905",sizeof(sEPayReq.stEpayHead.sAppid)-1);
    memcpy(sEPayReq.stEpayHead.sService,"cib.epay.payment.batchPayQueryApi",sizeof(sEPayReq.stEpayHead.sService)-1);
    memcpy(sEPayReq.stEpayHead.sVer,"02",sizeof(sEPayReq.stEpayHead.sVer)-1);
    memcpy(sEPayReq.stEpayHead.sSignType,"RSA",sizeof(sEPayReq.stEpayHead.sSignType)-1);
    memcpy(sEPayReq.stEpayHead.sKey,"6A9E618FDF824F90AF72D9EF54D9BA32",sizeof(sEPayReq.stEpayHead.sKey)-1);
    memcpy(sEPayReq.stEpayHead.sMrchCert,keyfile,sizeof(sEPayReq.stEpayHead.sMrchCert)-1);

    memset(sysDate, 0, sizeof(sysDate));
    CommonGetCurrentDate(sysDate);
    sprintf(sEPayReq.sFileName,"%s",Filename);
    memcpy(sEPayReq.sOrderDate,sysDate,sizeof(sEPayReq.sOrderDate)-1);
    memcpy(sEPayReq.sMac,"m+M5Bw3YkPdo1kAqnCTYUpaMJl2/Sp/y/YPNFpVyWbJ8REKdg2RXs473wbj/Xc0aAzY8eBU50U3e/wX2jOqrJ09sEXfUZdVku2RDAorqgu+xfGY3b2CmpAzpQIQH7l0PENc2xiI6IKnafC0sqKhv/c8yo/4Vb/hNnG9qtTAHOvm8bPh3/SgpcqnkkjbljnUhGR9aufQ6+DcZV/n2+xJSRZOr4tewKI9TiOoipNwEAVLz64mTRJbLCrZe2zwtgGv1dDfZ+e6EP7oMSPs/HJ60ToZnkWEc/c/hL/kyXw27MqgJBFbq5FsWbYjFLmKKNFz6zB+jG3Ebfz/IBCBtOmSnUw==",sizeof(sEPayReq.sMac)-1);  
         
    nReturnCode = BatchPayQuery(&sEPayReq,&sEPayRsp);
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " BatchPayQuery error [%d]",nReturnCode);
        return 0;
    }
    else
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " BatchPayQuery succ [%d]",nReturnCode);

        memset(sFileContent,0x00,sizeof(sFileContent));
        size_t bytes = base64_decode( sEPayRsp.sFileContent, sFileContent);
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " sFileContent[%s][%d]",sFileContent,bytes);
      
      
      //0-文件接收成功；
      //1-入批次明细成功；
      //2-交易成功；
      //3-交易失败；
      //4-交易部分成功 
        memset(sTmpFileName, 0, sizeof(sTmpFileName));
        strcpy(sTmpFileName, "HOST_YYYYMMDD_XX_ACCT.txt");
        pstr = strstr(sTmpFileName, "YYYYMMDD");
        if (pstr)
            memcpy(pstr, sysDate, 8);
        pstr = strstr(sTmpFileName, "XX");
        if (pstr)
            memcpy (pstr, "01", 2);
         
        memset(sFileFullName, 0, sizeof(sFileFullName));
        sprintf(sFileFullName, "%s/%8.8s/",
        getenv("BATCH_FILE_PATH"),
        sysDate); 
        nReturnCode = CheckDir(sFileFullName);
        if (nReturnCode)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "CheckDir err[%d][%s][%s]",
                nReturnCode, strerror(errno), sFileFullName);
            return -1;
        }
        strcat(sFileFullName, sTmpFileName);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name : [%s]", sFileFullName);
     
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
                
    }                 
    return 0;   
}
//商户结算账户余额查询
int iQueryBalance(char *KeyPath)
{
    int     nReturnCode =0;
    char    sysDate[9];
    char    sysTime[14+1];    
 
    char    keyfile[300];
    memset(keyfile,0x00,sizeof(keyfile));    
    sprintf(keyfile,"%sappsvr_clientQ0000905.pfx",KeyPath);
        
    stEpay_QueryBalance_Request sEPayReq;
    stEpay_QueryBalance_Response sEPayRsp;
    memset(&sEPayReq,0x0, sizeof(sEPayReq));
    memset(&sEPayRsp,0x0, sizeof(sEPayRsp));
    memset(sysTime,0x00,sizeof(sysTime));      
    CommonGetCurrentTime(sysTime);  
    
    memcpy(sEPayReq.stEpayHead.sTimeStamp,sysTime,sizeof(sEPayReq.stEpayHead.sTimeStamp)-1);
    memcpy(sEPayReq.stEpayHead.sAppid,"Q0000905",sizeof(sEPayReq.stEpayHead.sAppid)-1);
    memcpy(sEPayReq.stEpayHead.sService,"cib.epay.acquire.account.queryBalance",sizeof(sEPayReq.stEpayHead.sService)-1);
    memcpy(sEPayReq.stEpayHead.sVer,"02",sizeof(sEPayReq.stEpayHead.sVer)-1);
    memcpy(sEPayReq.stEpayHead.sSignType,"RSA",sizeof(sEPayReq.stEpayHead.sSignType)-1);
    memcpy(sEPayReq.stEpayHead.sKey,"6A9E618FDF824F90AF72D9EF54D9BA32",sizeof(sEPayReq.stEpayHead.sKey)-1);
    memcpy(sEPayReq.stEpayHead.sMrchCert,keyfile,sizeof(sEPayReq.stEpayHead.sMrchCert)-1);
    
    memcpy(sEPayReq.sMac,"0CD2273CC22142AB28EC900403DC7AE6B75ACF61",sizeof(sEPayReq.sMac)-1);  
            
    nReturnCode = QueryBalance(&sEPayReq,&sEPayRsp);
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " QueryBalance error [%d]",nReturnCode);
    }
    else
    {
         HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " QueryBalance succ [%d]",nReturnCode);           
    }     
    return 0;   
}

int WriteTestFile(char *Filename,char *stlm_dt)
{
    int     nReturnCode =0;
    char    sysDate[9];
    char    sysTime[14+1];   

    char binbug[2050];
    
    FILE    *fp;
    char    *pstr;
    char    sFileFullName[100];
    
    memset(sysDate,0x00,sizeof(sysDate));
    memset(sysTime,0x00,sizeof(sysTime));
        
    memset(binbug,0x00,sizeof(binbug));
        
    CommonGetCurrentTime(sysTime);
    CommonGetCurrentDate(sysDate);
        
    memset(sFileFullName, 0, sizeof(sFileFullName));
    sprintf(sFileFullName, "%s/%8.8s/",
    getenv("BATCH_FILE_PATH"),
    sysDate); 
    nReturnCode = CheckDir(sFileFullName);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "CheckDir err[%d][%s][%s]",
        nReturnCode, strerror(errno), sFileFullName);
        return -1;
    }
    strcat(sFileFullName, Filename);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name : [%s]", sFileFullName);
        
    fp = fopen(sFileFullName, "w+");
    if (!fp)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen error.\n");
        return -1;
    }
    rewind(fp);
    fprintf( fp, "%12.12s000003||6222801234567888953|张三|105100000017|0|8.00|代付测试|",sysTime);       
//    fprintf( fp, "%12.12s000003||6222809876543219810|大王|105100000017|0|8.00|代付测试|",sysTime);       
    fprintf( fp, "\r\n");
    fflush(fp);    
    fprintf( fp, "%12.12s000001||622909443442019514|惠举|309391000011|0|0.01|代付测试|",sysTime);
    fprintf( fp, "\r\n");
    fflush(fp);
    fprintf( fp, "%12.12s000002||6222801234567888953|张三|105100000017|0|8.00|代付测试|",sysTime);       
    fprintf( fp, "\r\n");
    fflush(fp);
    fclose(fp);     
    return 0;    
}
//批量代付资金对账文件下载
int iReceiptFile(char *Filename,char *stlm_dt,char *KeyPath)
{
    int     nReturnCode =0;
    char    sysDate[9];
    char    sysTime[14+1];   
    
    
    char    keyfile[300];
    memset(keyfile,0x00,sizeof(keyfile));    
    sprintf(keyfile,"%sappsvr_clientQ0000905.pfx",KeyPath);

    
    stEpay_ReceiptFile_Request sEPayReq;
    stEpay_ReceiptFile_Response sEPayRsp;
    memset(&sEPayReq,0x0, sizeof(sEPayReq));
    memset(&sEPayRsp,0x0, sizeof(sEPayRsp));
    memset(sysDate,0x00,sizeof(sysDate));      
    memset(sysTime,0x00,sizeof(sysTime));      
    CommonGetCurrentTime(sysDate);  
    CommonGetCurrentTime(sysTime);  
    
    memcpy(sEPayReq.stEpayHead.sTimeStamp,sysTime,sizeof(sEPayReq.stEpayHead.sTimeStamp)-1);
    memcpy(sEPayReq.stEpayHead.sAppid,"Q0000905",sizeof(sEPayReq.stEpayHead.sAppid)-1);
    memcpy(sEPayReq.stEpayHead.sService,"cib.epay.payment.receiptFile",sizeof(sEPayReq.stEpayHead.sService)-1);
    memcpy(sEPayReq.stEpayHead.sVer,"02",sizeof(sEPayReq.stEpayHead.sVer)-1);
    memcpy(sEPayReq.stEpayHead.sSignType,"RSA",sizeof(sEPayReq.stEpayHead.sSignType)-1);
    memcpy(sEPayReq.stEpayHead.sKey,"6A9E618FDF824F90AF72D9EF54D9BA32",sizeof(sEPayReq.stEpayHead.sKey)-1);
    memcpy(sEPayReq.stEpayHead.sMrchCert,keyfile,sizeof(sEPayReq.stEpayHead.sMrchCert)-1);
    
    memcpy(sEPayReq.sTransDate,stlm_dt,sizeof(sEPayReq.sTransDate)-1);
    memcpy(sEPayReq.sTcptType,"1",sizeof(sEPayReq.sTcptType)-1);      
    sprintf(sEPayReq.sFileName,"%s",Filename);
    
    memcpy(sEPayReq.sMac,"0CD2273CC22142AB28EC900403DC7AE6B75ACF61",sizeof(sEPayReq.sMac)-1);  
            
    nReturnCode = ReceiptFile(&sEPayReq,&sEPayRsp);
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " ReceiptFile error [%d]",nReturnCode);
    }
    else
    {
         HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " ReceiptFile succ [%d]",nReturnCode);           
}        
    
    return 0;   
}

//快捷支付资金对账文件下载
int isettleFile(char *Filename,char *stlm_dt,char *KeyPath)
{
    int     nReturnCode =0;
    char    sysDate[9];
    char    sysTime[14+1];   

    char    keyfile[300];
    memset(keyfile,0x00,sizeof(keyfile));    
    sprintf(keyfile,"%sappsvr_clientQ0000905.pfx",KeyPath);
    
    stEpay_SettleFile_Request sEPayReq;
    stEpay_SettleFile_Response sEPayRsp;
    memset(&sEPayReq,0x0, sizeof(sEPayReq));
    memset(&sEPayRsp,0x0, sizeof(sEPayRsp));
    memset(sysDate,0x00,sizeof(sysDate));      
    memset(sysTime,0x00,sizeof(sysTime));      
    CommonGetCurrentTime(sysDate);  
    CommonGetCurrentTime(sysTime);  
    
    memcpy(sEPayReq.stEpayHead.sTimeStamp,sysTime,sizeof(sEPayReq.stEpayHead.sTimeStamp)-1);
    memcpy(sEPayReq.stEpayHead.sAppid,"Q0000905",sizeof(sEPayReq.stEpayHead.sAppid)-1);
    memcpy(sEPayReq.stEpayHead.sService,"cib.epay.acquire.account.queryBalance",sizeof(sEPayReq.stEpayHead.sService)-1);
    memcpy(sEPayReq.stEpayHead.sVer,"02",sizeof(sEPayReq.stEpayHead.sVer)-1);
    memcpy(sEPayReq.stEpayHead.sSignType,"RSA",sizeof(sEPayReq.stEpayHead.sSignType)-1);
    memcpy(sEPayReq.stEpayHead.sKey,"6A9E618FDF824F90AF72D9EF54D9BA32",sizeof(sEPayReq.stEpayHead.sKey)-1);
    memcpy(sEPayReq.stEpayHead.sMrchCert,keyfile,sizeof(sEPayReq.stEpayHead.sMrchCert)-1);
    
    memcpy(sEPayReq.sTransDate,stlm_dt,sizeof(sEPayReq.sTransDate)-1);
    memcpy(sEPayReq.sTcptType,"3",sizeof(sEPayReq.sTcptType)-1);      
    sprintf(sEPayReq.sFileName,"%s",Filename);
    
    memcpy(sEPayReq.sMac,"0CD2273CC22142AB28EC900403DC7AE6B75ACF61",sizeof(sEPayReq.sMac)-1);  
            
    nReturnCode = settleFile(&sEPayReq,&sEPayRsp);
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " ReceiptFile error [%d]",nReturnCode);
    }
    else
    {
         HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " ReceiptFile succ [%d]",nReturnCode);           
    }         
    return 0;   
}

//威富通商户文件下载
int iAlipayMerchant(char *Filename,char *stlm_dt)
{
    int     nReturnCode =0;
    char    sysDate[9];
    char    sysTime[14+1];   

    char    *pstr;
    
    stEpay_Merchant_Request sEPayReq;
    stEpay_Merchant_Response sEPayRsp;
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
    memcpy(sEPayReq.sMchId,"101520000465",sizeof(sEPayReq.sMchId)-1);
    
    stExtnFileRegDef StExtnFileReg;
    
    /* 初始文件信息 */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, "100001", 6);
    memcpy(StExtnFileReg.stlm_dt, stlm_dt, 8);
    memcpy(StExtnFileReg.host_name, "ALPAY", 6);
    strcpy(StExtnFileReg.src_file_name, "aliPayFile_YYYYMMDD.dat");
    
    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, stlm_dt, 8);
    pstr = strstr (StExtnFileReg.src_file_name, "XX");
    if (pstr)
        memcpy (pstr, StExtnFileReg.inter_brh_code+4, 2);
    
    getExtnFileFullName(&StExtnFileReg, sEPayReq.sFileName);
    
    
    nReturnCode = Merchant(&sEPayReq,&sEPayRsp);
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " Merchant error [%d]",nReturnCode);
    }
    else
    {
         HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " Merchant succ [%d]",nReturnCode);
         WriteFile(sysDate,sEPayRsp.sFileContent);           
    }       
    return 0;   
}
//威富通商户文件下载
int iWechatMerchant(char *Filename,char *stlm_dt)
{
    int     nReturnCode =0;
    char    sysDate[9];
    char    sysTime[14+1];   
 
 
    char    *pstr;
    
    stEpay_Merchant_Request sEPayReq;
    stEpay_Merchant_Response sEPayRsp;
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
    //memcpy(sEPayReq.sMchId,"7551000001",sizeof(sEPayReq.sMchId)-1);
    memcpy(sEPayReq.sMchId,"7552900037",sizeof(sEPayReq.sMchId)-1);
    
     stExtnFileRegDef StExtnFileReg;
    
    /* 初始文件信息 */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, "100002", 6);
    memcpy(StExtnFileReg.stlm_dt, stlm_dt, 8);
    memcpy(StExtnFileReg.host_name, "WECHAT", 6);
    strcpy(StExtnFileReg.src_file_name, "WeiXinFile_YYYYMMDD.dat");
    
    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, stlm_dt, 8);
    pstr = strstr (StExtnFileReg.src_file_name, "XX");
    if (pstr)
        memcpy (pstr, StExtnFileReg.inter_brh_code+4, 2);
    
    getExtnFileFullName(&StExtnFileReg, sEPayReq.sFileName);       
    nReturnCode = Merchant(&sEPayReq,&sEPayRsp);
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " Merchant error [%d]",nReturnCode);
    }
    else
    {
         HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " Merchant succ [%d]",nReturnCode); 
         WriteFile(sysDate,sEPayRsp.sFileContent);           
}        
    return 0;   
}
int main( int argc, char **argv)
{
    char    sCfgCode[1 + 1];
    int     nReturnCode =0;
    char    sysDate[9];
    char    sysTime[14+1];
    char    stlm_dt[8+1];
    char    base64[4096];
    char    Filename[100];
    char    KeyPath[200];
    
    memset(base64,0x0,sizeof(base64));
    memset(Filename,0x0,sizeof(Filename));
    memset(stlm_dt,0x0,sizeof(stlm_dt));
    
   
    if(argc== 5)
    {        
         memcpy(sCfgCode, argv[1], sizeof(sCfgCode) - 1);
         memcpy(Filename, argv[2], sizeof(Filename) - 1);
         memcpy(stlm_dt, argv[3], sizeof(stlm_dt) - 1);
         memcpy(KeyPath, argv[4], sizeof(KeyPath) - 1);
       
    }
    else
    {
        //printf("Usage: %s HttpTest \n", argv[0]);
        //printf("       %s HttpTest CfgCode FileName stlm_dt KeyPath\n", argv[0]);
        return 0;
    }
    
    //printf("1商户结算账户余额查询\n");
    //printf("2批量资金付款\n");
    //printf("3批量代付查询\n");
    //printf("4账户余额查询\n");
    //printf("5生成结算文件\n");
    //printf("6批量代付资金对账文件下载\n");
    //printf("7快捷支付资金对账文件下载\n");
    //printf("8威富通支付宝商户文件下载\n");
    //printf("9威富通微信商户文件下载\n");
     
    if(sCfgCode[0] == '1')
    {
        iTestTopHttpCall(Filename,stlm_dt,KeyPath);     
    }
    else if(sCfgCode[0] == '2')
    {
          iBatchPayApi(Filename,stlm_dt,KeyPath);          
    }
    else if(sCfgCode[0] == '3')
    {
         iBatchPayQuery(Filename,stlm_dt,KeyPath);
    }
    else if(sCfgCode[0] == '4')
    {
          iQueryBalance(KeyPath);  
    }
    else if(sCfgCode[0] == '5')
    {
         WriteTestFile(Filename,stlm_dt);                        
    }
    else if(sCfgCode[0] == '6')
    {
        iReceiptFile(Filename,stlm_dt,KeyPath);    
    } 
    else if(sCfgCode[0] == '7')
    {
         isettleFile(Filename,stlm_dt,KeyPath);          
    } 
    else if(sCfgCode[0] == '8')
    {
        iAlipayMerchant(Filename,stlm_dt);             
    } 
    else if(sCfgCode[0] == '9')
    {
        iWechatMerchant(Filename,stlm_dt);   
    }
    else if(sCfgCode[0] == 'A')
    {
        iPaymentGet(Filename,stlm_dt,KeyPath);   
    }                                            
    else
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " sCfgCode error [%s]",sCfgCode); 
    }    
} /* end of main */


int unpackEncryptMsgRdm(unsigned char *RecvEncryptMsg) {
	cJSON* root = cJSON_CreateObject();  	
    cJSON_AddStringToObject(root, "timestamp", "20140831212747");  
    cJSON_AddStringToObject(root, "appid", "Q0000905");  
    cJSON_AddStringToObject(root, "service", "cib.epay.payment.batchPayApi");  
    cJSON_AddStringToObject(root, "ver", "02");  
    cJSON_AddStringToObject(root, "sign_type", "RSA");  
    cJSON_AddStringToObject(root, "order_date", "20151223");  
    cJSON_AddStringToObject(root, "file_name", "BATCH_PAY_20151201.txt");  
    cJSON_AddStringToObject(root, "file_content", "MjAxNTEyMDQwMDAxMnx8NjIyMjgwMTIzNDU2Nzg4ODk1M3y0+ri2uMTU7DAxfDEwNTEwMDAwMDAxN3wwfDEwMDAwLjAwfNeq1cvTw82+o7q0+reiuaTXynw=");  
	strcpy(RecvEncryptMsg, cJSON_Print(root));
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " RecvEncryptMsg[%s]",RecvEncryptMsg);
	return 0;
}

int packMsg(unsigned char *RecvEncryptMsg)
{
    if(RecvEncryptMsg==NULL)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " RecvEncryptMsg is NULL");
        return -1;
    }
    cJSON * pJson = cJSON_Parse(RecvEncryptMsg);
    if(pJson == NULL)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " RecvEncryptMsg is NULL");
        return -1;
    }
    cJSON * pSub = cJSON_GetObjectItem(pJson, "file_name");
    if(NULL == pSub)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " timestamp is NULL");
        return -1;
    }    
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " timestamp[%s]",pSub->valuestring);
   //cJSON * pSubSub = cJSON_GetObjectItem(pSub, "subjsonobj");
    //if(NULL == pSubSub)
    //{
    //}
    //printf("sub_obj_1 : %s\n", pSubSub->valuestring);
    
    cJSON_Delete(pJson);   
}


int WriteFile(char *sysDate,char *sFileContent)
{
    int nReturnCode;
    FILE    *fp;
    char    *pstr;
    char    *pFilestr = NULL;
    char    sFileFullName[200];
    char    sTmpFileName[100];
    char    sDest[2048];
    memset(sTmpFileName, 0, sizeof(sTmpFileName));
    strcpy(sTmpFileName, "HOST_YYYYMMDD_XX_ACCT.txt");
    pstr = strstr(sTmpFileName, "YYYYMMDD");
    if (pstr)
        memcpy(pstr, sysDate, 8);
    pstr = strstr(sTmpFileName, "XX");
    if (pstr)
        memcpy (pstr, "01", 2);
                
    memset(sFileFullName, 0, sizeof(sFileFullName));
    sprintf(sFileFullName, "%s/%8.8s/",
    getenv("BATCH_FILE_PATH"),
    sysDate); 
    nReturnCode = CheckDir(sFileFullName);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "CheckDir err[%d][%s][%s]",
                    nReturnCode, strerror(errno), sFileFullName);
        return -1;
    }
    strcat(sFileFullName, sTmpFileName);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name : [%s]", sFileFullName);
            
    fp = fopen(sFileFullName, "w+");
    if (!fp)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen error.\n");
        return -1;
    }
    rewind(fp);
    while(1)
    {
        memset(sDest, 0, sizeof(sDest));
        nReturnCode = getValueByDelim(sFileContent, sDest, "\\r\\n" );
        
        //pFilestr = strstr(sFileContent, "\\r\\n");
        if(nReturnCode==1)
        {
            break;            
        }
        fprintf( fp, "%s", sDest); 
        fprintf( fp, "\r\n");            
    }  
    fflush(fp);
    fclose(fp);    
    return 0;      
}