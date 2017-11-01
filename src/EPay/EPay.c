/**************************************************************************
 *  Copyright 2009, Shanghai Huateng Software Systems Co., Ltd.           *
 *  All right reserved.                                                   *
 *                                                                        *
 *  THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF SHANGHAI HUATENG       *
 *  SOFTWARE SYSTEMS CO., LTD.  THE CONTENTS OF THIS FILE MAY NOT         *
 *  BE DISCLOSED TO THIRD PARTIES, COPIED OR DUPLICATED IN ANY FORM,      *
 *  IN WHOLE OR IN PART, WITHOUT THE PRIOR WRITTEN PERMISSION OF          *
 *  SHANGHAI HUATENG SOFTWARE SYSTEMS CO., LTD.                           *
 *                                                                        *
 *	开发者 : 彭洪斌                                                       *
 **************************************************************************/
#include "EPay.h"
#include <iconv.h>

extern  char    gLogFile[LOG_NAME_LEN_MAX];

const char * base64char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static int code_convert(char *from_charset, char *to_charset, char *inbuf, long inlen, char *outbuf, size_t *outlen);
static int u2g(char *inbuf,int inlen,char *outbuf,size_t outlen);
static int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen);

static int code_convert(char *from_charset, char *to_charset, char *inbuf, long inlen, char *outbuf, size_t *outlen)
{
/*	iconv_t cd;
	int rc;
	char **pin = &inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset,from_charset);
	if (cd==0) return -1;
	memset(outbuf,0,outlen);
	if (iconv(cd,pin,&inlen,pout,&outlen)==-1) return -1;
	iconv_close(cd);
*/
	iconv_t cd;
	int rc;
	int iOutLen = *outlen;
	char **pin = &inbuf;
	char **pout = &outbuf;
	HtLog("u2g.log", HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "code_convert_ny begin");
	cd = iconv_open(to_charset, from_charset);
	if (cd == 0)
	{
		HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "iconv_open error");
		return -1;
	}
	memset(outbuf, 0x00, iOutLen);
	if (iconv(cd, pin, &inlen, pout, outlen) == -1)
	{
		HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "iconv error");
		return -1;
	}
	*outlen = strlen(*pout);
	iconv_close(cd);
	return 0;
}
//UNICODE码转为GB2312码
static int u2g(char *inbuf,int inlen,char *outbuf,size_t outlen)
{
	return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,&outlen);
}
//GB2312码转为UNICODE码
static int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
	return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,&outlen);
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

int CommonLTrimjson( char *caDest )
{
    int i;
    int j = 0;
    char lsTmp[1024*20*200];
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


char * base64_encode( const unsigned char * bindata, char * base64, int binlength )
{
    int i, j;
    unsigned char current;

    for ( i = 0, j = 0 ; i < binlength ; i += 3 )
    {
        current = (bindata[i] >> 2) ;
        current &= (unsigned char)0x3F;
        base64[j++] = base64char[(int)current];

        current = ( (unsigned char)(bindata[i] << 4 ) ) & ( (unsigned char)0x30 ) ;
        if ( i + 1 >= binlength )
        {
            base64[j++] = base64char[(int)current];
            base64[j++] = '=';
            base64[j++] = '=';
            break;
        }
        current |= ( (unsigned char)(bindata[i+1] >> 4) ) & ( (unsigned char) 0x0F );
        base64[j++] = base64char[(int)current];

        current = ( (unsigned char)(bindata[i+1] << 2) ) & ( (unsigned char)0x3C ) ;
        if ( i + 2 >= binlength )
        {
            base64[j++] = base64char[(int)current];
            base64[j++] = '=';
            break;
        }
        current |= ( (unsigned char)(bindata[i+2] >> 6) ) & ( (unsigned char) 0x03 );
        base64[j++] = base64char[(int)current];

        current = ( (unsigned char)bindata[i+2] ) & ( (unsigned char)0x3F ) ;
        base64[j++] = base64char[(int)current];
    }
    base64[j] = '\0';
    return base64;
}

int base64_decode( const char * base64, unsigned char * bindata )
{
    int i, j;
    unsigned char k;
    unsigned char temp[4];
    for ( i = 0, j = 0; base64[i] != '\0' ; i += 4 )
    {
        memset( temp, 0xFF, sizeof(temp) );
        for ( k = 0 ; k < 64 ; k ++ )
        {
            if ( base64char[k] == base64[i] )
                temp[0]= k;
        }
        for ( k = 0 ; k < 64 ; k ++ )
        {
            if ( base64char[k] == base64[i+1] )
                temp[1]= k;
        }
        for ( k = 0 ; k < 64 ; k ++ )
        {
            if ( base64char[k] == base64[i+2] )
                temp[2]= k;
        }
        for ( k = 0 ; k < 64 ; k ++ )
        {
            if ( base64char[k] == base64[i+3] )
                temp[3]= k;
        }

        bindata[j++] = ((unsigned char)(((unsigned char)(temp[0] << 2))&0xFC)) |
                ((unsigned char)((unsigned char)(temp[1]>>4)&0x03));
        if ( base64[i+2] == '=' )
            break;

        bindata[j++] = ((unsigned char)(((unsigned char)(temp[1] << 4))&0xF0)) |
                ((unsigned char)((unsigned char)(temp[2]>>2)&0x0F));
        if ( base64[i+3] == '=' )
            break;

        bindata[j++] = ((unsigned char)(((unsigned char)(temp[2] << 6))&0xF0)) |
                ((unsigned char)(temp[3]&0x3F));
    }
    return j;
}

int getCfgFileInf(char* sSoapCfgName, char* valueBuf) {
    /*open配置文件*/
	if(TopSoap_OpenCfgFile(sSoapCfgName))
	{
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "TopSoap_OpenCfgFile  Failed [%s]",sSoapCfgName);
		return -1;
	}

	/*获取配置IP*/
	memset(valueBuf, 0x00, sizeof(valueBuf));
	if(GetHttpCfgItem("projectName", valueBuf)){
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "GetHttpCfgItem(projectName)  Failed");
		Top_CloseCfgFile();
		return (-1);	
	}
	RightTrim(valueBuf);
	return 0;
}

//3.2	单笔代付查询
int PaymentGet(stEpay_PaymentGet_Request *pstEPayReq,stEpay_PaymentGet_Response *pstEPayRsp)
{       
	char    fName[128];
    char    sysDate[9];
    char    Tmpbf[1024*20*200] = {0}; 
    char    sSoapCfgName[32+1] = {0};  /*配置文件名*/
    char    sRspBody[1024*20*200] = {0};
    
    int     iRspBodyLen=0;
    int	    nInLen ;

    int     iTmpLen=0;
    int     nReturnCode =0;

    char *m_pJSONData= NULL;
    int m_nJSONAllocLen = 0;
    int m_nJSONDataLen = 0;
    int nLen = 0;

	char valueBuf[128];

    m_nJSONAllocLen = LOG_ALLOC_GRAN;
    m_pJSONData = malloc (m_nJSONAllocLen);
	        
    HttpRequest pstHttpRequest;
    HttpResponse pstHttpResponse;
    
    memset(&pstHttpRequest, 0x0, sizeof(pstHttpRequest));
    memset(&pstHttpResponse, 0x0, sizeof(pstHttpResponse));
    memset(sSoapCfgName,0x0,sizeof(sSoapCfgName));
    memset(Tmpbf,0x0,sizeof(Tmpbf));
    memset(fName, 0, sizeof(fName)); 
    memset(sRspBody,0x00,sizeof(sRspBody));   

	HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "============================================", sSoapCfgName);

    strcpy(sSoapCfgName,"httpcfg.cfg");
    CommonGetCurrentDate(sysDate);
    sprintf(fName, "%s%s%s%s%s.jpg", "905111122222E000", "10000001", "123456", "123456", sysDate);

	cJSON* root = cJSON_CreateObject();  	
	
    cJSON_AddStringToObject(root, "timestamp", pstEPayReq->stEpayHead.sTimeStamp);  
    cJSON_AddStringToObject(root, "appid", pstEPayReq->stEpayHead.sAppid);  
    cJSON_AddStringToObject(root, "service", pstEPayReq->stEpayHead.sService);  
    cJSON_AddStringToObject(root, "ver", pstEPayReq->stEpayHead.sVer);  
    cJSON_AddStringToObject(root, "sign_type", pstEPayReq->stEpayHead.sSignType);  
    cJSON_AddStringToObject(root, "key", pstEPayReq->stEpayHead.sKey);  
    cJSON_AddStringToObject(root, "mrch_cert", pstEPayReq->stEpayHead.sMrchCert);  

    cJSON_AddStringToObject(root, "order_no", pstEPayReq->sOrder_no);   
    cJSON_AddStringToObject(root, "mac", pstEPayReq->sMac);  

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
	
	getCfgFileInf(sSoapCfgName, valueBuf);
    sprintf(pstHttpRequest.sHttpPost,"/%s/direct/pyQuery.do",valueBuf);
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " SendMsg[%s]",pstHttpRequest.pReqBody);
    nReturnCode = TopHttpCall(&sSoapCfgName, &pstHttpRequest, &pstHttpResponse, &fName,m_nJSONDataLen);
    if (m_pJSONData)
    {
        free (m_pJSONData);
        m_pJSONData = NULL;
    }     
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " TopHttpCall error [%d]",nReturnCode);
        return nReturnCode;
    }    
    if(pstHttpResponse.stRspHead.iHttpStatus == 0)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文失败[%d]",pstHttpResponse.stRspHead.iHttpStatus);
    }
    else
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);
        if(pstHttpResponse.stRspHead.iHttpStatus == 200)
        {           
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);           
            if(pstHttpResponse.sRspBody==NULL)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " pstHttpResponse.sRspBody is NULL");
                return -1;
            }
            CommonLTrimjson(pstHttpResponse.sRspBody);
            CommonRTrimjson(pstHttpResponse.sRspBody);
            rtrimjson(pstHttpResponse.sRspBody);
            u2g(pstHttpResponse.sRspBody+1,strlen(pstHttpResponse.sRspBody+1)-1,sRspBody,sizeof(sRspBody));                        
        	HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " sRspBody[%s]",sRspBody);           
            cJSON * pJson = cJSON_Parse(pstHttpResponse.sRspBody+1);
            if(pJson == NULL)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " RecvEncryptMsg is NULL");
                return -1;
            }
            
            cJSON * pErrCodeSub = cJSON_GetObjectItem(pJson, "errcode");
            if(NULL == pErrCodeSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errcode is NULL");
            }
            else
            {                
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errcode[%s]",pErrCodeSub->valuestring);           
                memcpy(pstEPayRsp->stEpayErr.sErrCode,pErrCodeSub->valuestring,sizeof(pstEPayRsp->stEpayErr.sErrCode)-1);
                
                cJSON * pErrMsgSub = cJSON_GetObjectItem(pJson, "errmsg");
                if(NULL == pErrMsgSub)
                {
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errmsg is NULL");
                    return -1;
                }
                u2g(pErrMsgSub->valuestring,strlen(pErrMsgSub->valuestring),pstEPayRsp->stEpayErr.sErrMsg,sizeof(pstEPayRsp->stEpayErr.sErrMsg));                       
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errmsg[%s]",pstEPayRsp->stEpayErr.sErrMsg);           
               /*
                cJSON * pMacSub = cJSON_GetObjectItem(pJson, "mac");
                if(NULL == pMacSub)
                {
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " mac is NULL");
                    return -1;
                }    
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " mac[%s]",pMacSub->valuestring);
                memcpy(pstEPayRsp->sMac,pErrMsgSub->valuestring,sizeof(pstEPayRsp->sMac)-1);
                */
                cJSON_Delete(pJson); 
                return -1;                         
            }    
                        
            cJSON * pSnoSub = cJSON_GetObjectItem(pJson, "sno");
            if(NULL == pSnoSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " sno is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " bat_id[%s]",pSnoSub->valuestring);
            memcpy(pstEPayRsp->sNo,pSnoSub->valuestring,sizeof(pstEPayRsp->sNo)-1);

            cJSON * pMrchNoSub = cJSON_GetObjectItem(pJson, "mrchNo");
            if(NULL == pMrchNoSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " mrchNo is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " bat_status[%s]",pMrchNoSub->valuestring);
            memcpy(pstEPayRsp->sMrchNo,pMrchNoSub->valuestring,sizeof(pstEPayRsp->sMrchNo)-1);

            cJSON * pSubMrchNameSub = cJSON_GetObjectItem(pJson, "subMrchName");
            if(NULL == pSubMrchNameSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " subMrchName is NULL");
                return -1;
            }    
            u2g(pSubMrchNameSub->valuestring,strlen(pSubMrchNameSub->valuestring),pstEPayRsp->sSubMrchName,sizeof(pstEPayRsp->sSubMrchName));                       
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " subMrchName[%s]",pstEPayRsp->sSubMrchName);           


            cJSON * porderNoSub = cJSON_GetObjectItem(pJson, "orderNo");
            if(NULL == porderNoSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " orderNo is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " sOrderNo[%s]",porderNoSub->valuestring);
            memcpy(pstEPayRsp->sOrderNo,porderNoSub->valuestring,sizeof(pstEPayRsp->sOrderNo)-1);

            cJSON * ptransDateSub = cJSON_GetObjectItem(pJson, "transDate");
            if(NULL == ptransDateSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " transDate is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " transDate[%s]",ptransDateSub->valuestring);
            memcpy(pstEPayRsp->sTransDate,ptransDateSub->valuestring,sizeof(pstEPayRsp->sTransDate)-1);

            cJSON * ptransTimeSub = cJSON_GetObjectItem(pJson, "transTime");
            if(NULL == ptransTimeSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " transTime is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " transTime[%s]",ptransTimeSub->valuestring);
            memcpy(pstEPayRsp->sTransTime,ptransTimeSub->valuestring,sizeof(pstEPayRsp->sTransTime)-1);

            cJSON * ptransStatusSub = cJSON_GetObjectItem(pJson, "transStatus");
            if(NULL == ptransStatusSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " transStatus is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " transStatus[%s]",ptransStatusSub->valuestring);
            memcpy(pstEPayRsp->sTransStatus,ptransStatusSub->valuestring,sizeof(pstEPayRsp->sTransStatus)-1);

            cJSON * ptransAmtSub = cJSON_GetObjectItem(pJson, "transAmt");
            if(NULL == ptransAmtSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " transAmt is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " transAmt[%s]",ptransAmtSub->valuestring);
            pstEPayRsp->dTransAmt = atof(ptransAmtSub->valuestring);

            cJSON * ptransFeeSub = cJSON_GetObjectItem(pJson, "transFee");
            if(NULL == ptransFeeSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " transFee is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " transFee[%s]",ptransFeeSub->valuestring);
            pstEPayRsp->dTransFee = atof(ptransFeeSub->valuestring);

            cJSON * psToAcctNoSub = cJSON_GetObjectItem(pJson, "toAcctNo");
            if(NULL == psToAcctNoSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " toAcctNo is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " toAcctNo[%s]",psToAcctNoSub->valuestring);
            memcpy(pstEPayRsp->sToAcctNo,psToAcctNoSub->valuestring,sizeof(pstEPayRsp->sToAcctNo)-1);

             cJSON * ptoAcctNameSub = cJSON_GetObjectItem(pJson, "toAcctName");
            if(NULL == ptoAcctNameSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " toAcctName is NULL");
                return -1;
            } 
            u2g(ptoAcctNameSub->valuestring,strlen(ptoAcctNameSub->valuestring),pstEPayRsp->sToAcctName,sizeof(pstEPayRsp->sToAcctName));     
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " toAcctName[%s]",pstEPayRsp->sToAcctName);

             cJSON * ptransUsageSub = cJSON_GetObjectItem(pJson, "transUsage");
            if(NULL == ptransUsageSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " transUsage is NULL");
                return -1;
            }    
            u2g(ptransUsageSub->valuestring,strlen(ptransUsageSub->valuestring),pstEPayRsp->sTransUsage,sizeof(pstEPayRsp->sTransUsage));     
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " transUsage[%s]",pstEPayRsp->sTransUsage);

            /* cJSON * premarkSub = cJSON_GetObjectItem(pJson, "remark");
            if(NULL == premarkSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " remark is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " remark[%s]",premarkSub->valuestring);
            memcpy(pstEPayRsp->sRemark,premarkSub->valuestring,sizeof(pstEPayRsp->sRemark)-1);
            */


            cJSON * pMacSub = cJSON_GetObjectItem(pJson, "mac");
            if(NULL == pMacSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " mac is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " mac[%s]",pMacSub->valuestring);
            memcpy(pstEPayRsp->sMac,pMacSub->valuestring,sizeof(pstEPayRsp->sMac)-1);
                          
            cJSON_Delete(pJson); 
                
        }
        else if(pstHttpResponse.stRspHead.iHttpStatus == 400)
        {           
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]服务异常sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);           
            return -1;
        }
        else
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]未知错误sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);                       
            return -1;
        }
    }   
    
    return 0;   
}

//批量资金付款
int BatchPayApi(stEpay_BatchPayApi_Request *pstEPayReq,stEpay_BatchPayApi_Response *pstEPayRsp)
{       
	char    fName[128];
    char    sysDate[9];
    char    Tmpbf[1024*20*200] = {0}; 
    char    sSoapCfgName[32+1] = {0};  /*配置文件名*/
    char    sRspBody[1024*20*200] = {0};
    
    int     iRspBodyLen=0;
    int	    nInLen ;

    int     iTmpLen=0;
    int     nReturnCode =0;

    char *m_pJSONData= NULL;
    int m_nJSONAllocLen = 0;
    int m_nJSONDataLen = 0;
    int nLen = 0;
	char valueBuf[128];

    m_nJSONAllocLen = LOG_ALLOC_GRAN;
    m_pJSONData = malloc (m_nJSONAllocLen);
	        
    HttpRequest pstHttpRequest;
    HttpResponse pstHttpResponse;
    
    memset(&pstHttpRequest, 0x0, sizeof(pstHttpRequest));
    memset(&pstHttpResponse, 0x0, sizeof(pstHttpResponse));
    memset(sSoapCfgName,0x0,sizeof(sSoapCfgName));
    memset(Tmpbf,0x0,sizeof(Tmpbf));
    memset(fName, 0, sizeof(fName)); 
    memset(sRspBody,0x00,sizeof(sRspBody));   

	HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "============================================", sSoapCfgName);

    strcpy(sSoapCfgName,"httpcfg.cfg");
    CommonGetCurrentDate(sysDate);
    sprintf(fName, "%s%s%s%s%s.jpg", "905111122222E000", "10000001", "123456", "123456", sysDate);

	cJSON* root = cJSON_CreateObject();  	
	
    cJSON_AddStringToObject(root, "timestamp", pstEPayReq->stEpayHead.sTimeStamp);  
    cJSON_AddStringToObject(root, "appid", pstEPayReq->stEpayHead.sAppid);  
    cJSON_AddStringToObject(root, "service", pstEPayReq->stEpayHead.sService);  
    cJSON_AddStringToObject(root, "ver", pstEPayReq->stEpayHead.sVer);  
    cJSON_AddStringToObject(root, "sign_type", pstEPayReq->stEpayHead.sSignType);  
    cJSON_AddStringToObject(root, "key", pstEPayReq->stEpayHead.sKey);  
    cJSON_AddStringToObject(root, "mrch_cert", pstEPayReq->stEpayHead.sMrchCert);  

    cJSON_AddStringToObject(root, "order_date", pstEPayReq->sOrderDate);  
    cJSON_AddStringToObject(root, "file_name", pstEPayReq->sFileName);  
    cJSON_AddStringToObject(root, "file_content", pstEPayReq->pFileContent);  
    cJSON_AddStringToObject(root, "mac", pstEPayReq->sMac);  

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
	getCfgFileInf(sSoapCfgName, valueBuf);
    sprintf(pstHttpRequest.sHttpPost,"/%s/direct/batchPay.do",valueBuf);
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " SendMsg[%s]",pstHttpRequest.pReqBody);
    nReturnCode = TopHttpCall(&sSoapCfgName, &pstHttpRequest, &pstHttpResponse, &fName,m_nJSONDataLen);
    if (m_pJSONData)
    {
        free (m_pJSONData);
        m_pJSONData = NULL;
    }     
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " TopHttpCall error [%d]",nReturnCode);
        return nReturnCode;
    }    
    if(pstHttpResponse.stRspHead.iHttpStatus == 0)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文失败[%d]",pstHttpResponse.stRspHead.iHttpStatus);
    }
    else
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);
        if(pstHttpResponse.stRspHead.iHttpStatus == 200)
        {           
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);           
            if(pstHttpResponse.sRspBody==NULL)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " pstHttpResponse.sRspBody is NULL");
                return -1;
            }
            CommonLTrimjson(pstHttpResponse.sRspBody);
            CommonRTrimjson(pstHttpResponse.sRspBody);
            rtrimjson(pstHttpResponse.sRspBody);
            u2g(pstHttpResponse.sRspBody+1,strlen(pstHttpResponse.sRspBody+1)-1,sRspBody,sizeof(sRspBody));                        
        	HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " sRspBody[%s]",sRspBody);           
            cJSON * pJson = cJSON_Parse(pstHttpResponse.sRspBody+1);
            if(pJson == NULL)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " RecvEncryptMsg is NULL");
                return -1;
            }
            
            cJSON * pErrCodeSub = cJSON_GetObjectItem(pJson, "errcode");
            if(NULL == pErrCodeSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errcode is NULL");
            }
            else
            {                
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errcode[%s]",pErrCodeSub->valuestring);           
                memcpy(pstEPayRsp->stEpayErr.sErrCode,pErrCodeSub->valuestring,sizeof(pstEPayRsp->stEpayErr.sErrCode)-1);
                
                cJSON * pErrMsgSub = cJSON_GetObjectItem(pJson, "errmsg");
                if(NULL == pErrMsgSub)
                {
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errmsg is NULL");
                    return -1;
                }
                u2g(pErrMsgSub->valuestring,strlen(pErrMsgSub->valuestring),pstEPayRsp->stEpayErr.sErrMsg,sizeof(pstEPayRsp->stEpayErr.sErrMsg));                       
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errmsg[%s]",pstEPayRsp->stEpayErr.sErrMsg);           
               /*
                cJSON * pMacSub = cJSON_GetObjectItem(pJson, "mac");
                if(NULL == pMacSub)
                {
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " mac is NULL");
                    return -1;
                }    
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " mac[%s]",pMacSub->valuestring);
                memcpy(pstEPayRsp->sMac,pErrMsgSub->valuestring,sizeof(pstEPayRsp->sMac)-1);
                */
                cJSON_Delete(pJson); 
                return -1;                         
            }    
                        
            cJSON * pBatIdSub = cJSON_GetObjectItem(pJson, "bat_id");
            if(NULL == pBatIdSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " bat_id is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " bat_id[%s]",pBatIdSub->valuestring);
            memcpy(pstEPayRsp->sBatId,pBatIdSub->valuestring,sizeof(pstEPayRsp->sBatId)-1);

            cJSON * pBatStatusSub = cJSON_GetObjectItem(pJson, "bat_status");
            if(NULL == pBatStatusSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " bat_status is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " bat_status[%s]",pBatStatusSub->valuestring);
            memcpy(pstEPayRsp->sBatStatus,pBatStatusSub->valuestring,sizeof(pstEPayRsp->sBatStatus)-1);

            cJSON * pBatStatusLabelSub = cJSON_GetObjectItem(pJson, "bat_status_label");
            if(NULL == pBatStatusLabelSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " bat_status_label is NULL");
                return -1;
            }    
            u2g(pBatStatusLabelSub->valuestring,strlen(pBatStatusLabelSub->valuestring),pstEPayRsp->sBatStatusLabel,sizeof(pstEPayRsp->sBatStatusLabel));                       
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " bat_status_label[%s]",pstEPayRsp->sBatStatusLabel);           

            cJSON * pMacSub = cJSON_GetObjectItem(pJson, "mac");
            if(NULL == pMacSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " mac is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " mac[%s]",pMacSub->valuestring);
            memcpy(pstEPayRsp->sMac,pMacSub->valuestring,sizeof(pstEPayRsp->sMac)-1);
                          
            cJSON_Delete(pJson); 
                
        }
        else if(pstHttpResponse.stRspHead.iHttpStatus == 400)
        {           
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]服务异常sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);           
            return -1;
        }
        else
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]未知错误sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);                       
            return -1;
        }
    }   
    
    return 0;   
}
//批量代付查询
int BatchPayQuery(stEpay_BatchPayQuery_Request *pstEPayReq,stEpay_BatchPayQuery_Response *pstEPayRsp)
{    
	char    fName[128];
    char    sysDate[9];
    char    Tmpbf[1024*20*200] = {0}; 
    char    sSoapCfgName[32+1] = {0};  /*配置文件名*/
    char    sRspBody[1024*20*200] = {0};
    int     iRspBodyLen =0;

    int     iTmpLen=0;
    int     nReturnCode =0;

    char *m_pJSONData= NULL;
    int m_nJSONAllocLen = 0;
    int m_nJSONDataLen = 0;
    int nLen = 0;
	char valueBuf[128];

    m_nJSONAllocLen = LOG_ALLOC_GRAN;
    m_pJSONData = malloc (m_nJSONAllocLen);
        
    HttpRequest pstHttpRequest;
    HttpResponse pstHttpResponse;
    

    memset(&pstHttpRequest, 0x0, sizeof(pstHttpRequest));
    memset(&pstHttpResponse, 0x0, sizeof(pstHttpResponse));
    memset(sSoapCfgName,0x0,sizeof(sSoapCfgName));
    memset(Tmpbf,0x0,sizeof(Tmpbf));
    memset(fName, 0, sizeof(fName));    
    memset(sRspBody,0x00,sizeof(sRspBody));   

	HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "============================================", sSoapCfgName);
    strcpy(sSoapCfgName,"httpcfg.cfg");
    CommonGetCurrentDate(sysDate);
    sprintf(fName, "%s%s%s%s%s.jpg", "905111122222E000", "10000001", "123456", "123456", sysDate);

	cJSON* root = cJSON_CreateObject();  	
	
    cJSON_AddStringToObject(root, "timestamp", pstEPayReq->stEpayHead.sTimeStamp);  
    cJSON_AddStringToObject(root, "appid", pstEPayReq->stEpayHead.sAppid);  
    cJSON_AddStringToObject(root, "service", pstEPayReq->stEpayHead.sService);  
    cJSON_AddStringToObject(root, "ver", pstEPayReq->stEpayHead.sVer);  
    cJSON_AddStringToObject(root, "sign_type", pstEPayReq->stEpayHead.sSignType);  
    cJSON_AddStringToObject(root, "key", pstEPayReq->stEpayHead.sKey);  
    cJSON_AddStringToObject(root, "mrch_cert", pstEPayReq->stEpayHead.sMrchCert);  

    cJSON_AddStringToObject(root, "file_name", pstEPayReq->sFileName);  
    cJSON_AddStringToObject(root, "order_date", pstEPayReq->sOrderDate);  
    cJSON_AddStringToObject(root, "mac", pstEPayReq->sMac);  
	strcpy(Tmpbf, cJSON_Print(root));
	
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

    getCfgFileInf(sSoapCfgName, valueBuf);
    sprintf(pstHttpRequest.sHttpPost,"/%s/direct/batchPayQuery.do",valueBuf);

    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " SendMsg[%s]",pstHttpRequest.pReqBody);
    nReturnCode = TopHttpCall(&sSoapCfgName, &pstHttpRequest, &pstHttpResponse, &fName,m_nJSONDataLen);
    if (m_pJSONData)
    {
        free (m_pJSONData);
        m_pJSONData = NULL;
    }      
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " TopHttpCall error [%d]",nReturnCode);
        return nReturnCode;
    }    
    if(pstHttpResponse.stRspHead.iHttpStatus == 0)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文失败[%d]",pstHttpResponse.stRspHead.iHttpStatus);
    }
    else
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);
        if(pstHttpResponse.stRspHead.iHttpStatus == 200)
        {           
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);           
            if(pstHttpResponse.sRspBody==NULL)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " pstHttpResponse.sRspBody is NULL");
                return -1;
            }
            CommonLTrimjson(pstHttpResponse.sRspBody);
            CommonRTrimjson(pstHttpResponse.sRspBody);
            rtrimjson(pstHttpResponse.sRspBody);
            u2g(pstHttpResponse.sRspBody+1,strlen(pstHttpResponse.sRspBody+1)-1,sRspBody,sizeof(sRspBody));                        
        	HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " sRspBody[%s]",sRspBody);           
            cJSON * pJson = cJSON_Parse(pstHttpResponse.sRspBody+1);
            if(pJson == NULL)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " RecvEncryptMsg is NULL");
                return -1;
            }
            
            cJSON * pErrCodeSub = cJSON_GetObjectItem(pJson, "errcode");
            if(NULL == pErrCodeSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errcode is NULL");
            }
            else
            {                
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errcode[%s]",pErrCodeSub->valuestring);           
                memcpy(pstEPayRsp->stEpayErr.sErrCode,pErrCodeSub->valuestring,sizeof(pstEPayRsp->stEpayErr.sErrCode)-1);
                
                cJSON * pErrMsgSub = cJSON_GetObjectItem(pJson, "errmsg");
                if(NULL == pErrMsgSub)
                {
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errmsg is NULL");
                    return -1;
                }    
                u2g(pErrMsgSub->valuestring,strlen(pErrMsgSub->valuestring),pstEPayRsp->stEpayErr.sErrMsg,sizeof(pstEPayRsp->stEpayErr.sErrMsg));                       
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errmsg[%s]",pstEPayRsp->stEpayErr.sErrMsg);           
               /*
                cJSON * pMacSub = cJSON_GetObjectItem(pJson, "mac");
                if(NULL == pMacSub)
                {
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " mac is NULL");
                    return -1;
                }    
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " mac[%s]",pMacSub->valuestring);
                memcpy(pstEPayRsp->sMac,pErrMsgSub->valuestring,sizeof(pstEPayRsp->sMac)-1);
                */
                cJSON_Delete(pJson); 
                return -1;                         
            }    
                                              
            cJSON * pBatIdSub = cJSON_GetObjectItem(pJson, "bat_id");
            if(NULL == pBatIdSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " bat_id is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " bat_id[%s]",pBatIdSub->valuestring);           
            memcpy(pstEPayRsp->sBatId,pBatIdSub->valuestring,sizeof(pstEPayRsp->sBatId)-1);
            
            cJSON * pOrderDateSub = cJSON_GetObjectItem(pJson, "orderDate");
            if(NULL == pOrderDateSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " orderDate is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " orderDate[%s]",pOrderDateSub->valuestring);
            memcpy(pstEPayRsp->sOrderDate,pOrderDateSub->valuestring,sizeof(pstEPayRsp->sOrderDate)-1);
            
            cJSON * pBatStatusSub = cJSON_GetObjectItem(pJson, "batStatus");
            if(NULL == pBatStatusSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " batStatus is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " batStatus[%s]",pBatStatusSub->valuestring);
            memcpy(pstEPayRsp->sBatStatus,pBatStatusSub->valuestring,sizeof(pstEPayRsp->sBatStatus)-1);

            cJSON * pBatStatusLabelSub = cJSON_GetObjectItem(pJson, "batStatusLabel");
            if(NULL == pBatStatusLabelSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " batStatusLabel is NULL");
                return -1;
            } 
            u2g(pBatStatusLabelSub->valuestring,strlen(pBatStatusLabelSub->valuestring),pstEPayRsp->sBatStatusLabel,sizeof(pstEPayRsp->sBatStatusLabel));                       
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " batStatusLabel[%s]",pstEPayRsp->sBatStatusLabel);           
                           
            cJSON * pFileNameSub = cJSON_GetObjectItem(pJson, "fileName");
            if(NULL == pFileNameSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " fileName is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " fileName[%s]",pFileNameSub->valuestring);
            memcpy(pstEPayRsp->sFileName,pFileNameSub->valuestring,sizeof(pstEPayRsp->sFileName)-1);

            cJSON * pFileContentSub = cJSON_GetObjectItem(pJson, "file_content");
            if(NULL == pFileContentSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " file_content is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " file_content[%s]",pFileContentSub->valuestring);
            memcpy(pstEPayRsp->sFileContent,pFileContentSub->valuestring,sizeof(pstEPayRsp->sFileContent)-1);
            /*
            cJSON * pMacSub = cJSON_GetObjectItem(pJson, "mac");
            if(NULL == pMacSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " mac is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " mac[%s]",pMacSub->valuestring);
            memcpy(pstEPayRsp->sMac,pMacSub->valuestring,sizeof(pstEPayRsp->sMac)-1);
            */              
            cJSON_Delete(pJson);
            return 0; 
                
        }
        else if(pstHttpResponse.stRspHead.iHttpStatus == 400)
        {           
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]服务异常sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);           
            return -1;
        }
        else
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]未知错误sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);                       
            return -1;
        }
    }   
    
    
    return 0;       
}


//商户结算账户余额查询
int QueryBalance(stEpay_QueryBalance_Request *pstEPayReq,stEpay_QueryBalance_Response *pstEPayRsp)
{    
	char    fName[128];
    char    sysDate[9];
    char    Tmpbf[4096] = {0}; 
    char    sSoapCfgName[32+1] = {0};  /*配置文件名*/
    char    sRspBody[4096] = {0};
    int     iRspBodyLen = 0;

    int     iTmpLen=0;
    int     nReturnCode =0;

    char *m_pJSONData= NULL;
    int m_nJSONAllocLen = 0;
    int m_nJSONDataLen = 0;
    int nLen = 0;
	char valueBuf[128];

    m_nJSONAllocLen = LOG_ALLOC_GRAN;
    m_pJSONData = malloc (m_nJSONAllocLen);
     
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
	
    cJSON_AddStringToObject(root, "timestamp", pstEPayReq->stEpayHead.sTimeStamp);  
    cJSON_AddStringToObject(root, "appid", pstEPayReq->stEpayHead.sAppid);  
    cJSON_AddStringToObject(root, "service", pstEPayReq->stEpayHead.sService);  
    cJSON_AddStringToObject(root, "ver", pstEPayReq->stEpayHead.sVer);  
    cJSON_AddStringToObject(root, "sign_type", pstEPayReq->stEpayHead.sSignType);  
    cJSON_AddStringToObject(root, "key", pstEPayReq->stEpayHead.sKey);  
    cJSON_AddStringToObject(root, "mrch_cert", pstEPayReq->stEpayHead.sMrchCert);  

    cJSON_AddStringToObject(root, "mac", pstEPayReq->sMac);  
    
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

    getCfgFileInf(sSoapCfgName, valueBuf);
    sprintf(pstHttpRequest.sHttpPost,"/%s/direct/queryBalance.do",valueBuf);

    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " SendMsg[%s]",pstHttpRequest.pReqBody);
    nReturnCode = TopHttpCall(&sSoapCfgName, &pstHttpRequest, &pstHttpResponse, &fName,m_nJSONDataLen);
    if (m_pJSONData)
    {
        free (m_pJSONData);
        m_pJSONData = NULL;
    }      
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " TopHttpCall error [%d]",nReturnCode);
        return nReturnCode;
    }    
    if(pstHttpResponse.stRspHead.iHttpStatus == 0)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文失败[%d]",pstHttpResponse.stRspHead.iHttpStatus);
    }
    else
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);
        if(pstHttpResponse.stRspHead.iHttpStatus == 200)
        {           
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);           
            if(pstHttpResponse.sRspBody==NULL)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " pstHttpResponse.sRspBody is NULL");
                return -1;
            }
            CommonLTrimjson(pstHttpResponse.sRspBody);
            CommonRTrimjson(pstHttpResponse.sRspBody);
            rtrimjson(pstHttpResponse.sRspBody);
            u2g(pstHttpResponse.sRspBody+1,strlen(pstHttpResponse.sRspBody+1)-1,sRspBody,sizeof(sRspBody));                        
        	HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " sRspBody[%s]",sRspBody);           
            cJSON * pJson = cJSON_Parse(pstHttpResponse.sRspBody+1);
            if(pJson == NULL)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " RecvEncryptMsg is NULL");
                return -1;
            }
            
            cJSON * pErrCodeSub = cJSON_GetObjectItem(pJson, "errcode");
            if(NULL == pErrCodeSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errcode is NULL");
            }
            else
            {                
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errcode[%s]",pErrCodeSub->valuestring);           
                memcpy(pstEPayRsp->stEpayErr.sErrCode,pErrCodeSub->valuestring,sizeof(pstEPayRsp->stEpayErr.sErrCode)-1);
                
                cJSON * pErrMsgSub = cJSON_GetObjectItem(pJson, "errmsg");
                if(NULL == pErrMsgSub)
                {
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errmsg is NULL");
                    return -1;
                }    
                u2g(pErrMsgSub->valuestring,strlen(pErrMsgSub->valuestring),pstEPayRsp->stEpayErr.sErrMsg,sizeof(pstEPayRsp->stEpayErr.sErrMsg));                       
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errmsg[%s]",pstEPayRsp->stEpayErr.sErrMsg);           
              /*
                cJSON * pMacSub = cJSON_GetObjectItem(pJson, "mac");
                if(NULL == pMacSub)
                {
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " mac is NULL");
                    return -1;
                }    
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " mac[%s]",pMacSub->valuestring);
                memcpy(pstEPayRsp->sMac,pErrMsgSub->valuestring,sizeof(pstEPayRsp->sMac)-1);
                */
                cJSON_Delete(pJson); 
                return -1;                         
            }    
                                              
            cJSON * pMrchNoSub = cJSON_GetObjectItem(pJson, "mrch_no");
            if(NULL == pMrchNoSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " mrch_no is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " mrch_no[%s]",pMrchNoSub->valuestring);           
            memcpy(pstEPayRsp->sMrchNo,pMrchNoSub->valuestring,sizeof(pstEPayRsp->sMrchNo)-1);
            
            cJSON * pAcctNoSub = cJSON_GetObjectItem(pJson, "acct_no");
            if(NULL == pAcctNoSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " acct_no is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " acct_no[%s]",pAcctNoSub->valuestring);
            memcpy(pstEPayRsp->sAcctNo,pAcctNoSub->valuestring,sizeof(pstEPayRsp->sAcctNo)-1);
            
            cJSON * pAccountBalanceSub = cJSON_GetObjectItem(pJson, "account_balance");
            if(NULL == pAccountBalanceSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " account_balance is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " account_balance[%s]",pAccountBalanceSub->valuestring);
            pstEPayRsp->dAccountBalance = atof(pAccountBalanceSub->valuestring);


            cJSON * pAvailableBalancSub = cJSON_GetObjectItem(pJson, "available_balance");
            if(NULL == pAvailableBalancSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " available_balance is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " available_balance[%s]",pAvailableBalancSub->valuestring);
            pstEPayRsp->dAvailableBalanc = atof(pAvailableBalancSub->valuestring);

            cJSON * pBlockedBalanceSub = cJSON_GetObjectItem(pJson, "blocked_balance");
            if(NULL == pBlockedBalanceSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " blocked_balance is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " blocked_balance[%s]",pBlockedBalanceSub->valuestring);
            pstEPayRsp->dBlockedBalance = atof(pBlockedBalanceSub->valuestring);

            cJSON * pControlledBalanceSub = cJSON_GetObjectItem(pJson, "controlled_balance");
            if(NULL == pControlledBalanceSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " controlled_balance is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " controlled_balance[%s]",pControlledBalanceSub->valuestring);
            pstEPayRsp->dControlledBalance = atof(pControlledBalanceSub->valuestring);

            cJSON * pSignTypeSub = cJSON_GetObjectItem(pJson, "sign_type");
            if(NULL == pSignTypeSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " sign_type is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " sign_type[%s]",pSignTypeSub->valuestring);
            memcpy(pstEPayRsp->sSignType,pSignTypeSub->valuestring,sizeof(pstEPayRsp->sSignType)-1);
            
            /*
            cJSON * pMacSub = cJSON_GetObjectItem(pJson, "mac");
            if(NULL == pMacSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " mac is NULL");
                return -1;
            }    
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " mac[%s]",pMacSub->valuestring);
            memcpy(pstEPayRsp->sMac,pMacSub->valuestring,sizeof(pstEPayRsp->sMac)-1);
            */              
            cJSON_Delete(pJson); 
                
        }
        else if(pstHttpResponse.stRspHead.iHttpStatus == 400)
        {           
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]服务异常sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);           
            return -1;
        }
        else
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]未知错误sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);                       
            return -1;
        }
    }   
    
    
    return 0;       
}


//批量代付资金对账文件下载
int ReceiptFile(stEpay_ReceiptFile_Request *pstEPayReq,stEpay_ReceiptFile_Response *pstEPayRsp)
{    
	char    fName[128];
    char    sysDate[9];
    char    Tmpbf[1024*20*200] = {0}; 
    char    sSoapCfgName[32+1] = {0};  /*配置文件名*/
    char    sRspBody[1024*20*200] = {0};
    int     iRspBodyLen = 0;

    int     iTmpLen=0;
    int     nReturnCode =0;

    char *m_pJSONData= NULL;
    int m_nJSONAllocLen = 0;
    int m_nJSONDataLen = 0;
    int nLen = 0;
	char valueBuf[128];

    m_nJSONAllocLen = LOG_ALLOC_GRAN;
    m_pJSONData = malloc (m_nJSONAllocLen);
     
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
	
    cJSON_AddStringToObject(root, "timestamp", pstEPayReq->stEpayHead.sTimeStamp);  
    cJSON_AddStringToObject(root, "appid", pstEPayReq->stEpayHead.sAppid);  
    cJSON_AddStringToObject(root, "service", pstEPayReq->stEpayHead.sService);  
    cJSON_AddStringToObject(root, "ver", pstEPayReq->stEpayHead.sVer);  
    cJSON_AddStringToObject(root, "sign_type", pstEPayReq->stEpayHead.sSignType);  
    cJSON_AddStringToObject(root, "key", pstEPayReq->stEpayHead.sKey);  
    cJSON_AddStringToObject(root, "mrch_cert", pstEPayReq->stEpayHead.sMrchCert);  
    
    cJSON_AddStringToObject(root, "trans_date", pstEPayReq->sTransDate);  
    cJSON_AddStringToObject(root, "rcpt_type", pstEPayReq->sTcptType);  
    cJSON_AddStringToObject(root, "file_name", pstEPayReq->sFileName);  

    cJSON_AddStringToObject(root, "mac", pstEPayReq->sMac);  
    
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

	getCfgFileInf(sSoapCfgName, valueBuf);
    sprintf(pstHttpRequest.sHttpPost,"/%s/direct/receiptFile.do",valueBuf);

    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " SendMsg[%s]",pstHttpRequest.pReqBody);
    nReturnCode = TopHttpCall(&sSoapCfgName, &pstHttpRequest, &pstHttpResponse, &fName,m_nJSONDataLen);
    if (m_pJSONData)
    {
        free (m_pJSONData);
        m_pJSONData = NULL;
    }      
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " TopHttpCall error [%d]",nReturnCode);
        return nReturnCode;
    }    
    if(pstHttpResponse.stRspHead.iHttpStatus == 0)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文失败[%d]",pstHttpResponse.stRspHead.iHttpStatus);
    }
    else
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);
        if(pstHttpResponse.stRspHead.iHttpStatus == 200)
        {           
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);           
            if(pstHttpResponse.sRspBody==NULL)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " pstHttpResponse.sRspBody is NULL");
                return -1;
            }
            CommonLTrimjson(pstHttpResponse.sRspBody);
            CommonRTrimjson(pstHttpResponse.sRspBody);
            rtrimjson(pstHttpResponse.sRspBody);
            u2g(pstHttpResponse.sRspBody+1,strlen(pstHttpResponse.sRspBody+1)-1,sRspBody,sizeof(sRspBody));                        
        	HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " sRspBody[%s]",sRspBody);           
            cJSON * pJson = cJSON_Parse(pstHttpResponse.sRspBody+1);
            if(pJson == NULL)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " RecvEncryptMsg is NULL");
                return -1;
            }
            
            cJSON * pErrCodeSub = cJSON_GetObjectItem(pJson, "errcode");
            if(NULL == pErrCodeSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errcode is NULL");
            }
            else
            {                
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errcode[%s]",pErrCodeSub->valuestring);           
                memcpy(pstEPayRsp->stEpayErr.sErrCode,pErrCodeSub->valuestring,sizeof(pstEPayRsp->stEpayErr.sErrCode)-1);
                
                cJSON * pErrMsgSub = cJSON_GetObjectItem(pJson, "errmsg");
                if(NULL == pErrMsgSub)
                {
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errmsg is NULL");
                    return -1;
                }    
                u2g(pErrMsgSub->valuestring,strlen(pErrMsgSub->valuestring),pstEPayRsp->stEpayErr.sErrMsg,sizeof(pstEPayRsp->stEpayErr.sErrMsg));                       
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errmsg[%s]",pstEPayRsp->stEpayErr.sErrMsg);           
              /*
                cJSON * pMacSub = cJSON_GetObjectItem(pJson, "mac");
                if(NULL == pMacSub)
                {
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " mac is NULL");
                    return -1;
                }    
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " mac[%s]",pMacSub->valuestring);
                memcpy(pstEPayRsp->sMac,pErrMsgSub->valuestring,sizeof(pstEPayRsp->sMac)-1);
                */
                cJSON_Delete(pJson); 
                
                if(memcmp(pstEPayRsp->stEpayErr.sErrCode, "EPAY_00000", 10)==0)
                {
                    return 0;
                    
                }                             
                return -1;                         
            }                                                 
            cJSON_Delete(pJson); 
                
        }
        else if(pstHttpResponse.stRspHead.iHttpStatus == 400)
        {           
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]服务异常sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);           
            return -1;
        }
        else
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]未知错误sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);                       
            return -1;
        }
    }   
   
    return 0;       
}


//快捷支付资金对账文件下载
int settleFile(stEpay_SettleFile_Request *pstEPayReq,stEpay_SettleFile_Response *pstEPayRsp)
{    
	char    fName[128];
    char    sysDate[9];
    char    Tmpbf[1024*20*200] = {0}; 
    char    sSoapCfgName[32+1] = {0};  /*配置文件名*/
    char    sRspBody[1024*20*200] = {0};
    int     iRspBodyLen = 0;

    int     iTmpLen=0;
    int     nReturnCode =0;
    char *m_pJSONData= NULL;
    int m_nJSONAllocLen = 0;
    int m_nJSONDataLen = 0;
    int nLen = 0;
	char valueBuf[128];

    m_nJSONAllocLen = LOG_ALLOC_GRAN;
    m_pJSONData = malloc (m_nJSONAllocLen);
     
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
	
    cJSON_AddStringToObject(root, "timestamp", pstEPayReq->stEpayHead.sTimeStamp);  
    cJSON_AddStringToObject(root, "appid", pstEPayReq->stEpayHead.sAppid);  
    cJSON_AddStringToObject(root, "service", pstEPayReq->stEpayHead.sService);  
    cJSON_AddStringToObject(root, "ver", pstEPayReq->stEpayHead.sVer);  
    cJSON_AddStringToObject(root, "sign_type", pstEPayReq->stEpayHead.sSignType);  
    cJSON_AddStringToObject(root, "key", pstEPayReq->stEpayHead.sKey);  
    cJSON_AddStringToObject(root, "mrch_cert", pstEPayReq->stEpayHead.sMrchCert);  
    
    cJSON_AddStringToObject(root, "trans_date", pstEPayReq->sTransDate);  
    cJSON_AddStringToObject(root, "rcpt_type", pstEPayReq->sTcptType);  
    cJSON_AddStringToObject(root, "file_name", pstEPayReq->sFileName);  

    cJSON_AddStringToObject(root, "mac", pstEPayReq->sMac);  
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

	getCfgFileInf(sSoapCfgName, valueBuf);
    sprintf(pstHttpRequest.sHttpPost,"/%s/direct/settleFile.do",valueBuf);

    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " SendMsg[%s]",pstHttpRequest.pReqBody);
    nReturnCode = TopHttpCall(&sSoapCfgName, &pstHttpRequest, &pstHttpResponse, &fName,m_nJSONDataLen);
    if (m_pJSONData)
    {
        free (m_pJSONData);
        m_pJSONData = NULL;
    }      
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " TopHttpCall error [%d]",nReturnCode);
        return nReturnCode;
    }    
    if(pstHttpResponse.stRspHead.iHttpStatus == 0)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文失败[%d]",pstHttpResponse.stRspHead.iHttpStatus);
    }
    else
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);
        if(pstHttpResponse.stRspHead.iHttpStatus == 200)
        {           
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);           
            if(pstHttpResponse.sRspBody==NULL)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " pstHttpResponse.sRspBody is NULL");
                return -1;
            }
            CommonLTrimjson(pstHttpResponse.sRspBody);
            CommonRTrimjson(pstHttpResponse.sRspBody);
            rtrimjson(pstHttpResponse.sRspBody);
            u2g(pstHttpResponse.sRspBody+1,strlen(pstHttpResponse.sRspBody+1)-1,sRspBody,sizeof(sRspBody));                        
        	HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " sRspBody[%s]",sRspBody);           
            cJSON * pJson = cJSON_Parse(pstHttpResponse.sRspBody+1);
            if(pJson == NULL)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " RecvEncryptMsg is NULL");
                return -1;
            }
            
            cJSON * pErrCodeSub = cJSON_GetObjectItem(pJson, "errcode");
            if(NULL == pErrCodeSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errcode is NULL");
            }
            else
            {                
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errcode[%s]",pErrCodeSub->valuestring);           
                memcpy(pstEPayRsp->stEpayErr.sErrCode,pErrCodeSub->valuestring,sizeof(pstEPayRsp->stEpayErr.sErrCode)-1);
                
                cJSON * pErrMsgSub = cJSON_GetObjectItem(pJson, "errmsg");
                if(NULL == pErrMsgSub)
                {
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errmsg is NULL");
                    return -1;
                }    
                u2g(pErrMsgSub->valuestring,strlen(pErrMsgSub->valuestring),pstEPayRsp->stEpayErr.sErrMsg,sizeof(pstEPayRsp->stEpayErr.sErrMsg));                       
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errmsg[%s]",pstEPayRsp->stEpayErr.sErrMsg);           
              /*
                cJSON * pMacSub = cJSON_GetObjectItem(pJson, "mac");
                if(NULL == pMacSub)
                {
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " mac is NULL");
                    return -1;
                }    
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " mac[%s]",pMacSub->valuestring);
                memcpy(pstEPayRsp->sMac,pErrMsgSub->valuestring,sizeof(pstEPayRsp->sMac)-1);
                */
                cJSON_Delete(pJson); 
                
                if(memcmp(pstEPayRsp->stEpayErr.sErrCode, "EPAY_00000", 10)==0)
                {
                    return 0;
                    
                }                             
                return -1;                         
            }                                                 
            cJSON_Delete(pJson); 
                
        }
        else if(pstHttpResponse.stRspHead.iHttpStatus == 400)
        {           
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]服务异常sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);           
            return -1;
        }
        else
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]未知错误sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);                       
            return -1;
        }
    }   
   
    return 0;       
}


//威富通商户文件下载
int Merchant(stEpay_Merchant_Request *pstEPayReq,stEpay_Merchant_Response *pstEPayRsp)
{    
	char    fName[128];
    char    sysDate[9];
    char    Tmpbf[1024*20*200] = {0}; 
    char    sSoapCfgName[32+1] = {0};  /*配置文件名*/
    char    sRspBody[1024*20*200] = {0};
    int     iRspBodyLen = 0;

    int     iTmpLen=0;
    int     nReturnCode =0;
	char valueBuf[128];
    
    HttpRequest pstHttpRequest;
    HttpResponse pstHttpResponse;
    

    memset(&pstHttpRequest, 0x0, sizeof(pstHttpRequest));
    memset(&pstHttpResponse, 0x0, sizeof(pstHttpResponse));
    memset(sSoapCfgName,0x0,sizeof(sSoapCfgName));
    memset(Tmpbf,0x0,sizeof(Tmpbf));
    memset(fName, 0, sizeof(fName));    

	HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "============================================");
    strcpy(sSoapCfgName,"httpcfg.cfg");
    CommonGetCurrentDate(sysDate);
    sprintf(fName, "%s%s%s%s%s.jpg", "905111122222E000", "10000001", "123456", "123456", sysDate);

	cJSON* root = cJSON_CreateObject();  	
	
    cJSON_AddStringToObject(root, "service", pstEPayReq->sService);  
    cJSON_AddStringToObject(root, "version", pstEPayReq->sVersion);  
    cJSON_AddStringToObject(root, "charset", pstEPayReq->sCharset);  
    
    cJSON_AddStringToObject(root, "bill_date", pstEPayReq->sBillDate);  
    cJSON_AddStringToObject(root, "bill_type", pstEPayReq->sBillType);      
    cJSON_AddStringToObject(root, "sign_type", pstEPayReq->sSignType);  
    cJSON_AddStringToObject(root, "mch_id", pstEPayReq->sMchId); 
    cJSON_AddStringToObject(root, "key", pstEPayReq->sKey); 
    cJSON_AddStringToObject(root, "nonce_str", pstEPayReq->sNonceStr);  
    cJSON_AddStringToObject(root, "sign", pstEPayReq->sSign);  
	strcpy(Tmpbf, cJSON_Print(root));
	
    iTmpLen = strlen(Tmpbf);
    memcpy(pstHttpRequest.sReqBody,Tmpbf,iTmpLen);

	getCfgFileInf(sSoapCfgName, valueBuf);
    sprintf(pstHttpRequest.sHttpPost,"/%s/gateway/merchant.do",valueBuf);

    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " SendMsg[%s]",pstHttpRequest.sReqBody);

    nReturnCode = TopHttpCallEx(&sSoapCfgName, &pstHttpRequest, &pstHttpResponse, pstEPayReq->sFileName,iTmpLen);
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " TopHttpCall error [%d]",nReturnCode);
        return nReturnCode;
    }    
    if(pstHttpResponse.stRspHead.iHttpStatus == 0)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文失败[%d]",pstHttpResponse.stRspHead.iHttpStatus);
    }
    else
    {
        return 0;
        //HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);
        if(pstHttpResponse.stRspHead.iHttpStatus == 200)
        {           
            //HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);           
            if(pstHttpResponse.sRspBody==NULL)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " pstHttpResponse.sRspBody is NULL");
                return -1;
            }
            memset(sRspBody,0,sizeof(sRspBody));
            memcpy(sRspBody,pstHttpResponse.sRspBody,sizeof(sRspBody));
            CommonLTrimjson(sRspBody);
            CommonRTrimjson(sRspBody);
            rtrimjson(sRspBody);
            //u2g(pstHttpResponse.sRspBody+1,strlen(pstHttpResponse.sRspBody+1)-1,sRspBody,sizeof(sRspBody));                        
        	HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " sRspBody[%s]",pstHttpResponse.sRspBody);           
            cJSON * pJson = cJSON_Parse(sRspBody+1);
            if(pJson == NULL)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " RecvEncryptMsg is NULL");
                memcpy(pstEPayRsp->sFileContent,pstHttpResponse.sRspBody+1,sizeof(pstEPayRsp->sFileContent)-1);                              
                return 0;
            }
            
            cJSON * pErrCodeSub = cJSON_GetObjectItem(pJson, "status");
            if(NULL == pErrCodeSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errcode is NULL");
                cJSON * pResultSub = cJSON_GetObjectItem(pJson, "result");
                if(NULL == pResultSub)
                {
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " result is NULL");               
                }
                else
                {                
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " result[%d]",pResultSub->valuestring);           
                    memcpy(pstEPayRsp->stEpayErr.sErrCode,pResultSub->valuestring,sizeof(pstEPayRsp->stEpayErr.sErrCode)-1);
                    
                    cJSON * pErrMsgSub = cJSON_GetObjectItem(pJson, "message");
                    if(NULL == pErrMsgSub)
                    {
                        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errmsg is NULL");
                        return -1;
                    }    
                    u2g(pErrMsgSub->valuestring,strlen(pErrMsgSub->valuestring),pstEPayRsp->stEpayErr.sErrMsg,sizeof(pstEPayRsp->stEpayErr.sErrMsg));                       
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errmsg[%s]",pstEPayRsp->stEpayErr.sErrMsg);           
                    cJSON_Delete(pJson); 
                    
                    if(memcmp(pstEPayRsp->stEpayErr.sErrCode, "EPAY_00000", 10)==0)
                    {
                        return 0;
                        
                    }                             
                    return -1;                         
                }
            }                                
            else
            {                
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " status[%d]",pErrCodeSub->valueint);           
                sprintf(pstEPayRsp->stEpayErr.sErrCode,"%d",pErrCodeSub->valueint);
                
                cJSON * pErrMsgSub = cJSON_GetObjectItem(pJson, "message");
                if(NULL == pErrMsgSub)
                {
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errmsg is NULL");
                    return -1;
                }    
                u2g(pErrMsgSub->valuestring,strlen(pErrMsgSub->valuestring),pstEPayRsp->stEpayErr.sErrMsg,sizeof(pstEPayRsp->stEpayErr.sErrMsg));                       
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errmsg[%s]",pstEPayRsp->stEpayErr.sErrMsg);           
                cJSON_Delete(pJson); 
                
                if(memcmp(pstEPayRsp->stEpayErr.sErrCode, "EPAY_00000", 10)==0)
                {
                    return 0;
                    
                }                             
                return -1;                         
            }                                                 
            cJSON_Delete(pJson); 
                
        }
        else if(pstHttpResponse.stRspHead.iHttpStatus == 400)
        {           
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]服务异常sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);           
            return -1;
        }
        else
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]未知错误sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);                       
            return -1;
        }
    }   
   
    return 0;       
}

char* stringDelete(char* src, char* dest, char* sDelim) {
	char *p = NULL;
	while(src) {
        if((p=strstr(src, sDelim))) {
		    strncat(dest, src, p-src);
            src = p+strlen(sDelim);
		}
		else break;
	}
    strcat(dest, src);
	return dest;
}

int FySinglePayApi(Fy_Single_Request* FyRequest, Fy_Single_Response* FyResponse)
{       
	char    fName[128];
    char    sysDate[9];
    char    Tmpbf[1024*20*200] = {0}; 
    char    sSoapCfgName[32+1] = {0};  /*配置文件名*/
    char    sRspBody[1024*20*200] = {0};
    
    int     iRspBodyLen=0;
    int	    nInLen ;

    int     iTmpLen=0;
    int     nReturnCode =0;

	char requestMsg[2048];

    char *m_pJSONData= NULL;
    int m_nJSONAllocLen = 0;
    int m_nJSONDataLen = 0;
    int nLen = 0;
	char valueBuf[128];

    m_nJSONAllocLen = LOG_ALLOC_GRAN;
    m_pJSONData = malloc (m_nJSONAllocLen);
	        
    HttpRequest pstHttpRequest;
    HttpResponse pstHttpResponse;
    
    memset(&pstHttpRequest, 0x0, sizeof(pstHttpRequest));
    memset(&pstHttpResponse, 0x0, sizeof(pstHttpResponse));
    memset(sSoapCfgName,0x0,sizeof(sSoapCfgName));
    memset(Tmpbf,0x0,sizeof(Tmpbf));
    memset(fName, 0, sizeof(fName)); 
    memset(sRspBody,0x00,sizeof(sRspBody));   

    strcpy(sSoapCfgName,"httpcfg.cfg");
    CommonGetCurrentDate(sysDate);
    sprintf(fName, "%s%s%s%s%s.jpg", "905111122222E000", "10000001", "123456", "123456", sysDate);

	cJSON* root = cJSON_CreateObject();  	

	cJSON_AddStringToObject(root, "reqtype", FyRequest->reqtype);  
	cJSON_AddStringToObject(root, "merid", FyRequest->merid);  
	cJSON_AddStringToObject(root, "xml", FyRequest->xmlMsg);  
	
//    cJSON_AddStringToObject(root, "merdt", FyRequest->merdt);  
//	cJSON_AddStringToObject(root, "orderno", FyRequest->orderno);  
//	cJSON_AddStringToObject(root, "bankno", FyRequest->bankno);  
//	cJSON_AddStringToObject(root, "cityno", FyRequest->cityno);  
//	cJSON_AddStringToObject(root, "branchnm", FyRequest->branchnm);  
//	cJSON_AddStringToObject(root, "accntno", FyRequest->accntno);  
//	cJSON_AddStringToObject(root, "accntnm", FyRequest->accntnm);  
//	char amtTmp[24] = {};
//	sprintf(amtTmp, "%d", FyRequest->amt);
//	cJSON_AddStringToObject(root, "amt", amtTmp);  
    
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " pstHttpRequest[%s]",cJSON_Print(root));
//	memset(requestMsg, 0, sizeof(requestMsg));
//	stringDelete(cJSON_Print(root), requestMsg, "\\");
//	HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " requestMsg[%s]",requestMsg);

    nLen = strlen(cJSON_Print(root));           
    if (m_nJSONDataLen + nLen > m_nJSONAllocLen)
    {
        int nNewAllocLen = m_nJSONAllocLen * 2;
        if (nNewAllocLen < m_nJSONDataLen + nLen)
        {
            nNewAllocLen = (m_nJSONDataLen + nLen + LOG_ALLOC_GRAN - 1) / LOG_ALLOC_GRAN * LOG_ALLOC_GRAN;
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
	
	getCfgFileInf(sSoapCfgName, valueBuf);
    sprintf(pstHttpRequest.sHttpPost,"/%s/direct/fuiouPayForReq.do",valueBuf);

    nReturnCode = TopHttpCall(&sSoapCfgName, &pstHttpRequest, &pstHttpResponse, &fName,m_nJSONDataLen);
    if (m_pJSONData)
    {
        free (m_pJSONData);
        m_pJSONData = NULL;
    }     
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " TopHttpCall error [%d]",nReturnCode);
        return nReturnCode;
    }    
    if(pstHttpResponse.stRspHead.iHttpStatus == 0)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文失败[%d]",pstHttpResponse.stRspHead.iHttpStatus);
		return -1;
    }
    else
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);
        if(pstHttpResponse.stRspHead.iHttpStatus == 200)
        {           
            if(pstHttpResponse.sRspBody==NULL)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " pstHttpResponse.sRspBody is NULL");
                return -1;
            }
            CommonLTrimjson(pstHttpResponse.sRspBody);
            CommonRTrimjson(pstHttpResponse.sRspBody);
            rtrimjson(pstHttpResponse.sRspBody);
            u2g(pstHttpResponse.sRspBody+1,strlen(pstHttpResponse.sRspBody+1)-1,sRspBody,sizeof(sRspBody));                        
        	HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " sRspBody[%s]",pstHttpResponse.sRspBody);  
			memcpy(FyResponse->xmlMsg,pstHttpResponse.sRspBody,sizeof(FyResponse->xmlMsg)-1);
			return 0;
            cJSON * pJson = cJSON_Parse(pstHttpResponse.sRspBody+1);
            if(pJson == NULL)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " RecvEncryptMsg is NULL");
                return -1;
            }

            cJSON * pErrCodeSub = cJSON_GetObjectItem(pJson, "xml");
            if(NULL == pErrCodeSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " xml is NULL");
				return -1;
            }
            else
            {                
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " xml[%s]",pErrCodeSub->valuestring);           
                memcpy(FyResponse->xmlMsg,pErrCodeSub->valuestring,sizeof(FyResponse->xmlMsg)-1);
                   
                cJSON_Delete(pJson); 
				
                return 0;                         
            }                               
            cJSON_Delete(pJson);           
        }
        else if(pstHttpResponse.stRspHead.iHttpStatus == 400)
        {           
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]服务异常sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);           
            return -1;
        }
        else
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]未知错误sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);                       
            return -1;
        }
    }   
    
    return 0;   
}

int FySingleQueryApi(Fy_Single_Request* FyRequest, Fy_Single_Response* FyResponse)
{       
	char    fName[128];
    char    sysDate[9];
    char    Tmpbf[1024*20*200] = {0}; 
    char    sSoapCfgName[32+1] = {0};  /*配置文件名*/
    char    sRspBody[1024*20*200] = {0};
    
    int     iRspBodyLen=0;
    int	    nInLen ;

    int     iTmpLen=0;
    int     nReturnCode =0;

    char *m_pJSONData= NULL;
    int m_nJSONAllocLen = 0;
    int m_nJSONDataLen = 0;
    int nLen = 0;
	char valueBuf[128];

    m_nJSONAllocLen = LOG_ALLOC_GRAN;
    m_pJSONData = malloc (m_nJSONAllocLen);
	        
    HttpRequest pstHttpRequest;
    HttpResponse pstHttpResponse;
    
    memset(&pstHttpRequest, 0x0, sizeof(pstHttpRequest));
    memset(&pstHttpResponse, 0x0, sizeof(pstHttpResponse));
    memset(sSoapCfgName,0x0,sizeof(sSoapCfgName));
    memset(Tmpbf,0x0,sizeof(Tmpbf));
    memset(fName, 0, sizeof(fName)); 
    memset(sRspBody,0x00,sizeof(sRspBody));   

    strcpy(sSoapCfgName,"httpcfg.cfg");
    CommonGetCurrentDate(sysDate);
    sprintf(fName, "%s%s%s%s%s.jpg", "905111122222E000", "10000001", "123456", "123456", sysDate);

	cJSON* root = cJSON_CreateObject();  	

	cJSON_AddStringToObject(root, "reqtype", FyRequest->reqtype);  
	cJSON_AddStringToObject(root, "merid", FyRequest->merid);  
	cJSON_AddStringToObject(root, "xml", FyRequest->xmlMsg);   
    
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " pstHttpRequest[%s]",cJSON_Print(root));
    nLen = strlen(cJSON_Print(root));           
    if (m_nJSONDataLen + nLen > m_nJSONAllocLen)
    {
        int nNewAllocLen = m_nJSONAllocLen * 2;
        if (nNewAllocLen < m_nJSONDataLen + nLen)
        {
            nNewAllocLen = (m_nJSONDataLen + nLen + LOG_ALLOC_GRAN - 1) / LOG_ALLOC_GRAN * LOG_ALLOC_GRAN;
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
	
	getCfgFileInf(sSoapCfgName, valueBuf);
    sprintf(pstHttpRequest.sHttpPost,"/%s/direct/fuiouQryTransReq.do",valueBuf);

    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " SendMsg[%s]",pstHttpRequest.pReqBody);
    nReturnCode = TopHttpCall(&sSoapCfgName, &pstHttpRequest, &pstHttpResponse, &fName,m_nJSONDataLen);
    if (m_pJSONData)
    {
        free (m_pJSONData);
        m_pJSONData = NULL;
    }     
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " TopHttpCall error [%d]",nReturnCode);
        return nReturnCode;
    }    
    if(pstHttpResponse.stRspHead.iHttpStatus == 0)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文失败[%d]",pstHttpResponse.stRspHead.iHttpStatus);
		return -1;
    }
    else
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);
        if(pstHttpResponse.stRspHead.iHttpStatus == 200)
        {           
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);           
            if(pstHttpResponse.sRspBody==NULL)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " pstHttpResponse.sRspBody is NULL");
                return -1;
            }
            CommonLTrimjson(pstHttpResponse.sRspBody);
            CommonRTrimjson(pstHttpResponse.sRspBody);
            rtrimjson(pstHttpResponse.sRspBody);
            u2g(pstHttpResponse.sRspBody+1,strlen(pstHttpResponse.sRspBody+1)-1,sRspBody,sizeof(sRspBody));                        
        	HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " sRspBody[%s]",pstHttpResponse.sRspBody);  
			memcpy(FyResponse->xmlMsg,pstHttpResponse.sRspBody,sizeof(FyResponse->xmlMsg)-1);
			return 0;
            cJSON * pJson = cJSON_Parse(pstHttpResponse.sRspBody+1);
            if(pJson == NULL)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " RecvEncryptMsg is NULL");
                return -1;
            }

            cJSON * pErrCodeSub = cJSON_GetObjectItem(pJson, "xml");
            if(NULL == pErrCodeSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " xml is NULL");
				return -1;
            }
            else
            {                
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " xml[%s]",pErrCodeSub->valuestring);           
                memcpy(FyResponse->xmlMsg,pErrCodeSub->valuestring,sizeof(FyResponse->xmlMsg)-1);
                   
                cJSON_Delete(pJson); 
				
                return 0;                         
            }                               
            cJSON_Delete(pJson);           
        }
        else if(pstHttpResponse.stRspHead.iHttpStatus == 400)
        {           
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]服务异常sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);           
            return -1;
        }
        else
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]未知错误sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);                       
            return -1;
        }
    }   
    
    return 0;   
}

int FyBalanceQueryApi(Fy_Single_Request* FyRequest, Fy_Single_Response* FyResponse)
{       
	char    fName[128];
    char    sysDate[9];
    char    Tmpbf[1024*20*200] = {0}; 
    char    sSoapCfgName[32+1] = {0};  /*配置文件名*/
    char    sRspBody[1024*20*200] = {0};
    
    int     iRspBodyLen=0;
    int	    nInLen ;

    int     iTmpLen=0;
    int     nReturnCode =0;

    char *m_pJSONData= NULL;
    int m_nJSONAllocLen = 0;
    int m_nJSONDataLen = 0;
    int nLen = 0;
	char valueBuf[128];

    m_nJSONAllocLen = LOG_ALLOC_GRAN;
    m_pJSONData = malloc (m_nJSONAllocLen);
	        
    HttpRequest pstHttpRequest;
    HttpResponse pstHttpResponse;
    
    memset(&pstHttpRequest, 0x0, sizeof(pstHttpRequest));
    memset(&pstHttpResponse, 0x0, sizeof(pstHttpResponse));
    memset(sSoapCfgName,0x0,sizeof(sSoapCfgName));
    memset(Tmpbf,0x0,sizeof(Tmpbf));
    memset(fName, 0, sizeof(fName)); 
    memset(sRspBody,0x00,sizeof(sRspBody));   

    strcpy(sSoapCfgName,"httpcfg.cfg");
    CommonGetCurrentDate(sysDate);
    sprintf(fName, "%s%s%s%s%s.jpg", "905111122222E000", "10000001", "123456", "123456", sysDate);

	cJSON* root = cJSON_CreateObject();  	

	cJSON_AddStringToObject(root, "ver", FyRequest->ver);  
	cJSON_AddStringToObject(root, "merid", FyRequest->merid);    
    
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " pstHttpRequest[%s]",cJSON_Print(root));
    nLen = strlen(cJSON_Print(root));           
    if (m_nJSONDataLen + nLen > m_nJSONAllocLen)
    {
        int nNewAllocLen = m_nJSONAllocLen * 2;
        if (nNewAllocLen < m_nJSONDataLen + nLen)
        {
            nNewAllocLen = (m_nJSONDataLen + nLen + LOG_ALLOC_GRAN - 1) / LOG_ALLOC_GRAN * LOG_ALLOC_GRAN;
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

	getCfgFileInf(sSoapCfgName, valueBuf);
    sprintf(pstHttpRequest.sHttpPost,"/%s/direct/fuiouQryacnt.do",valueBuf);

    nReturnCode = TopHttpCall(&sSoapCfgName, &pstHttpRequest, &pstHttpResponse, &fName,m_nJSONDataLen);
    if (m_pJSONData)
    {
        free (m_pJSONData);
        m_pJSONData = NULL;
    }     
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " TopHttpCall error [%d]",nReturnCode);
        return nReturnCode;
    }    
    if(pstHttpResponse.stRspHead.iHttpStatus == 0)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文失败[%d]",pstHttpResponse.stRspHead.iHttpStatus);
		return -1;
    }
    else
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);
        if(pstHttpResponse.stRspHead.iHttpStatus == 200)
        {           
            if(pstHttpResponse.sRspBody==NULL)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " pstHttpResponse.sRspBody is NULL");
                return -1;
            }
            CommonLTrimjson(pstHttpResponse.sRspBody);
            CommonRTrimjson(pstHttpResponse.sRspBody);
            rtrimjson(pstHttpResponse.sRspBody);
            u2g(pstHttpResponse.sRspBody+1,strlen(pstHttpResponse.sRspBody+1)-1,sRspBody,sizeof(sRspBody));                        
        	HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " sRspBody[%s]",pstHttpResponse.sRspBody);  
			memcpy(FyResponse->xmlMsg,pstHttpResponse.sRspBody,sizeof(FyResponse->xmlMsg)-1);
            char code[6+1] = {};
			char* sEnd = NULL;
			sEnd = strstr(FyResponse->xmlMsg, "code");
			memcpy(code, sEnd+7, 6); 
			HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " code:[%s]", code);
			if(!memcmp("000000", code,6)) {
                return 0;
		    }     
			else return -1;

            cJSON * pJson = cJSON_Parse(pstHttpResponse.sRspBody);
            if(pJson == NULL)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " RecvEncryptMsg is NULL");
                return -1;
            }

            cJSON * pErrCodeSub = cJSON_GetObjectItem(pJson, "code");
            if(NULL == pErrCodeSub)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " code is NULL");
				return -1;
            }
            else
            {                
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " code:[%s]",pErrCodeSub->valuestring);           
                if(!memcmp("000000", pErrCodeSub->valuestring,6)) {
                    return 0;
				}                   
                cJSON_Delete(pJson); 
				
                return -1;                         
            }                               
            cJSON_Delete(pJson);           
        }
        else if(pstHttpResponse.stRspHead.iHttpStatus == 400)
        {           
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]服务异常sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);           
            return -1;
        }
        else
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]未知错误sRspBody[%s]",pstHttpResponse.stRspHead.iHttpStatus,pstHttpResponse.sRspBody);                       
            return -1;
        }
    }   
    
    return 0;   
}