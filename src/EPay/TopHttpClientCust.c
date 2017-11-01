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
 *	开发者 : 李振先                                                       *
 **************************************************************************/
#include "TopHttpClientCust.h"
#include <iconv.h>

extern char *_TopCfg_LTrim(char *sChars); 

static char sTopCfgLogName[32];

static FILE *cfgFp = NULL;
static char sFullName[256];

static int 			clientSockfd;			/*客户端套接字*/ 
static struct sockaddr_in	server_addr;			/*服务器端地址*/ 

extern  char    gLogFile[LOG_NAME_LEN_MAX];

#define LOG_ALLOC_GRAN			(1024)

static char *m_pSendData;
static int m_nSendAllocLen;
static int m_nSendDataLen;

static char *m_pData;
static int m_nAllocLen;
static int m_nDataLen;

int IntSendDate()
{
	m_nSendAllocLen = 0;
	m_pSendData = NULL;
	m_nSendAllocLen = LOG_ALLOC_GRAN;
	m_pSendData = malloc (m_nSendAllocLen);
	m_nSendDataLen = 0;	  
}

void RawSendDate(const char *pStr, int nLen)
{
	if (nLen < 0)
		nLen = strlen(pStr);
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
	memcpy(m_pSendData + m_nSendDataLen, pStr, nLen);
	m_nSendDataLen += nLen;
}

void FlushSendLog()
{
	//写日志到文件中
	int nDataLen = m_nSendDataLen;
    //printf("nDataLen[%d]\n",nDataLen);
    //printf("m_pSendData[%s]\n",m_pSendData);
    HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,"[%s]",m_pSendData);
}

ResetSend()
{
	if (m_pSendData)
	{
		free (m_pSendData);
		m_pSendData = NULL;
	}       
}


int IntDate()
{
	m_nAllocLen = 0;
	m_pData = NULL;
	m_nDataLen = 0;
	m_nAllocLen = LOG_ALLOC_GRAN;
	m_pData = malloc (m_nAllocLen);
	m_nDataLen = 0;	  
}

void RawLog(const char *pStr, int nLen)
{
	if (nLen < 0)
		nLen = strlen(pStr);
	if (m_nDataLen + nLen > m_nAllocLen)
	{
		int nNewAllocLen = m_nAllocLen * 2;
		if (nNewAllocLen < m_nDataLen + nLen)
		{
			nNewAllocLen = (m_nDataLen + nLen + LOG_ALLOC_GRAN - 1) 
				/ LOG_ALLOC_GRAN * LOG_ALLOC_GRAN;
		}
		char *pNew = malloc (nNewAllocLen);
		if (m_nDataLen > 0)
			memcpy(pNew, m_pData, m_nDataLen);
		free (m_pData);
		m_pData = pNew;
		m_nAllocLen = nNewAllocLen;
	}
	memcpy(m_pData + m_nDataLen, pStr, nLen);
	m_nDataLen += nLen;
}

void FlushLog()
{
	//写日志到文件中
	int nDataLen = m_nDataLen;
    //printf("nDataLen[%d]\n",nDataLen);
    //printf("m_pData[%s]\n",m_pData);
    HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,"[%s]",m_pData);
}

Reset()
{
	if (m_pData)
	{
		free (m_pData);
		m_pData = NULL;
	}       
}

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

/*根据传入的配置文件名获取配置参数，并存入pstHttpReqHead结构体中*/
int TopHttpClientHeadInit(HttpReqHead *pstHttpReqHead, char *sSoapCfgName)
{
	char valueBuf[128];
	int headPairNum, i;
	char *pHeadPair = NULL;
	char headPairName[128];

	memset(pstHttpReqHead, 0x00, sizeof(HttpReqHead));
	
	/*open配置文件*/
	if(TopSoap_OpenCfgFile(sSoapCfgName))
	{
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "TopSoap_OpenCfgFile  Failed [%s]",sSoapCfgName);
		return -1;
	}

	/*获取配置IP*/
	memset(valueBuf, 0x00, sizeof(valueBuf));
	if(GetHttpCfgItem("serverip", valueBuf)){
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "GetHttpCfgItem(serverip)  Failed");
		Top_CloseCfgFile();
		return (-1);	
	}
	RightTrim(valueBuf);
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "CfgItem - serverip : [%s]",valueBuf);
	memcpy(pstHttpReqHead->HttpServAddr,valueBuf,strlen(valueBuf));
	
	/*获取配置端口  -- Brady.Lee @ 20091014*/
	memset(valueBuf, 0x00, sizeof(valueBuf));
	if(GetHttpCfgItem("serverport", valueBuf)){
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "GetHttpCfgItem(serverport)  Failed");
		return (-1);	
	}
	RightTrim(valueBuf);
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "CfgItem - serverport : [%s]", valueBuf);
	memcpy(pstHttpReqHead->HttpServPort,valueBuf,strlen(valueBuf));

	/*获取配置系统ID*/
	memset(valueBuf, 0x00, sizeof(valueBuf));
  if(GetHttpCfgItem("syscode", valueBuf)){
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,"GetHttpCfgItem(syscode)  Failed");
		return (-1);
	}
	RightTrim(valueBuf);
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "CfgItem - syscode : [%s]", valueBuf);
	memcpy(pstHttpReqHead->HttpSyscode,valueBuf,strlen(valueBuf));

	/*获取配置KEY值*/
	memset(valueBuf, 0x00, sizeof(valueBuf));
  if(GetHttpCfgItem("mkey", valueBuf)){
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,"GetHttpCfgItem(mkey)  Failed");
		return (-1);
	}
	RightTrim(valueBuf);
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "CfgItem - mkey : [%s]", valueBuf);
	memcpy(pstHttpReqHead->HttpMHttpSyscodekey,valueBuf,strlen(valueBuf));

	/*获取配置POST值*/
/*	memset(valueBuf, 0x00, sizeof(valueBuf));
  if(GetHttpCfgItem("POST", valueBuf)){
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,"GetHttpCfgItem(POST)  Failed");
		return (-1);
	}
	RightTrim(valueBuf);
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "CfgItem - POST : [%s]", valueBuf);
	memcpy(pstHttpReqHead->HttpPost,valueBuf,strlen(valueBuf));
*/
	/*获取配置ContentType值*/
	memset(valueBuf, 0x00, sizeof(valueBuf));
  if(GetHttpCfgItem("ContentType", valueBuf)){
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,"GetHttpCfgItem(ContentType)  Failed");
		return (-1);
	}
	RightTrim(valueBuf);
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "CfgItem - ContentType : [%s]", valueBuf);
	memcpy(pstHttpReqHead->HttpContentType,valueBuf,strlen(valueBuf));
		
	if(Top_CloseCfgFile()){
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,"Top_CloseCfgFile  Failed");
		return -1;	
	}
	return 0;
}

/*Rsp日志信息打印*/
void HttpResponseTrace(HttpResponse *pstHttpRsp)
{
	int i;
	
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "HttpResponseTrace ");
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "HTTP Status: [%d]", pstHttpRsp->stRspHead.iHttpStatus);
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "HTTP StatusDESC:[%s]", pstHttpRsp->stRspHead.sHttpStatusDesc);
}


void PackHttpReqMesg(HttpRequest *pstHttpReq, char *sHttpFileName, int nnLen)
{
	char tmpBuf[9064];
	char tmpBuf2[512];
	char fileName[50+1];
	char sCurrentTime[14+1];
	char sDate[8+1];    /*交易日期*/
	int i;
	int nLenTmp = 0;
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "PackHttpReqMesg BEGIN");
	
	memset(tmpBuf2, 0x0, sizeof(tmpBuf2));
	memset(tmpBuf, 0x0, sizeof(tmpBuf));
	memset(fileName, 0x0, sizeof(fileName));
	memset(sCurrentTime, 0x0, sizeof(sCurrentTime));
	memset(sDate, 0x0, sizeof(sDate));
	CommonGetCurrentTime(sCurrentTime);
	memcpy(sDate,sCurrentTime,8);
	memcpy(fileName,sHttpFileName,strlen(sHttpFileName));
	
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sDate[%s]  fileName[%s]",sDate,fileName);
	

	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "tmpBuf2:[%s]",tmpBuf2);
 
	sprintf(tmpBuf2,"%s",pstHttpReq->sHttpPost);
    memset(tmpBuf, 0x00, sizeof(tmpBuf));
    sprintf(tmpBuf, "POST %s HTTP/1.1\r\nHost:%s:%d\r\n",tmpBuf2, pstHttpReq->stReqHead.HttpServAddr,atoi(pstHttpReq->stReqHead.HttpServPort)); 
 	RawSendDate(tmpBuf,strlen(tmpBuf));
  
    memset(tmpBuf, 0x00, sizeof(tmpBuf));
	sprintf(tmpBuf,"Content-Type:%s; Charset=UTF-8\r\n",pstHttpReq->stReqHead.HttpContentType);
	RawSendDate(tmpBuf,strlen(tmpBuf));
    
    memset(tmpBuf, 0x00, sizeof(tmpBuf));
    sprintf(tmpBuf,"Connection:Keep-Alive\r\n");
    RawSendDate(tmpBuf,strlen(tmpBuf));
		
    memset(tmpBuf, 0x00, sizeof(tmpBuf));
    sprintf(tmpBuf,"Cache-Control:no-cache\r\n");
    RawSendDate(tmpBuf,strlen(tmpBuf));

    memset(tmpBuf, 0x00, sizeof(tmpBuf));
    HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "nnLen:[%d]",nnLen);
    sprintf(tmpBuf, "Content-Length:%d\r\n\r\n",nnLen);
    RawSendDate(tmpBuf,strlen(tmpBuf));

    //nLenTmp = m_nSendDataLen + nnLen ;		
    //HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "m_pSendData:[%s] nLenTmp:[%d]",m_pSendData,m_nSendDataLen);
    //memcpy(pReqBuf+strlen(pReqBuf),pstHttpReq->sReqBody,nnLen);
    RawSendDate(pstHttpReq->pReqBody,nnLen);    
     HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sReqBody[%s][%d]", pstHttpReq->sReqBody,strlen(pstHttpReq->sReqBody));
 
    //memset(tmpBuf, 0x00, sizeof(tmpBuf));
	//strncat(tmpBuf,"\r\n",2);
	RawSendDate("\r\n\0",3);

    HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "pReqBuf[%s]", m_pSendData);
}

void PackHttpReqMesgEx(HttpRequest *pstHttpReq, char *sHttpReqBuf, char *sHttpFileName, int nnLen,char *snLen)
{
	char *pReqBuf = sHttpReqBuf;
	char tmpBuf[9064];
	char tmpBuf2[512];
	char fileName[50+1];
	char sCurrentTime[14+1];
	char sDate[8+1];    /*交易日期*/
	int i;
	int nLenTmp = 0;
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "PackHttpReqMesg BEGIN");
	
	memset(tmpBuf2, 0x0, sizeof(tmpBuf2));
	memset(tmpBuf, 0x0, sizeof(tmpBuf));
	memset(fileName, 0x0, sizeof(fileName));
	memset(sCurrentTime, 0x0, sizeof(sCurrentTime));
	memset(sDate, 0x0, sizeof(sDate));
	CommonGetCurrentTime(sCurrentTime);
	memcpy(sDate,sCurrentTime,8);
	memcpy(fileName,sHttpFileName,strlen(sHttpFileName));
	
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sDate[%s]  fileName[%s]",sDate,fileName);
	
	//sprintf(tmpBuf2,"/front-converter/netgate/serv1.do&syscode=%s&mkey=%s&FilePath=/%s/%s/V1.0/%s&Content-Length=%d", 
	//pstHttpReq->stReqHead.HttpSyscode,pstHttpReq->stReqHead.HttpMHttpSyscodekey,pstHttpReq->stReqHead.HttpSyscode, sDate,fileName,nnLen);
	//sprintf(tmpBuf2,"/front-converter/netgate/serv1.do");
	sprintf(tmpBuf2,"%s",pstHttpReq->sHttpPost);

	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "tmpBuf2:[%s]",tmpBuf2);
   //sprintf(pReqBuf, "POST %s HTTP/1.1\r\n",tmpBuf2);
   
    //sprintf(pReqBuf, "POST %s HTTP/1.1\r\nHost: %s\r\nConnection: keep-alive\r\n\r\n%s\r\n",tmpBuf2,pstHttpReq->stReqHead.HttpServAddr,pstHttpReq->sReqBody);


    sprintf(pReqBuf, "POST %s HTTP/1.1\r\nHost:%s:%d\r\n",tmpBuf2, pstHttpReq->stReqHead.HttpServAddr,atoi(pstHttpReq->stReqHead.HttpServPort)); 
    memset(tmpBuf, 0x00, sizeof(tmpBuf));
		sprintf(tmpBuf,"Content-Type:%s; Charset=UTF-8\r\n",pstHttpReq->stReqHead.HttpContentType);
		strncat(pReqBuf,tmpBuf,strlen(tmpBuf));
		
      /*memset(tmpBuf, 0x00, sizeof(tmpBuf));
		sprintf(tmpBuf,"Connection:Keep-Alive\r\n");
		strncat(pReqBuf,tmpBuf,strlen(tmpBuf));
		
    memset(tmpBuf, 0x00, sizeof(tmpBuf));
		sprintf(tmpBuf,"Cache-Control:no-cache\r\n");
		strncat(pReqBuf,tmpBuf,strlen(tmpBuf));
*/
		memset(tmpBuf, 0x00, sizeof(tmpBuf));
		HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "nnLen:[%d]",nnLen);
		sprintf(tmpBuf, "Content-Length:%d\r\n\r\n",nnLen);
		strncat(pReqBuf,tmpBuf,strlen(tmpBuf));

		nLenTmp = strlen(pReqBuf) + nnLen ;
		
		sprintf(snLen,"%d",nLenTmp);
		HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "pReqBuf:[%s] nLenTmp:[%d],snLen[%s]",pReqBuf,nLenTmp,snLen);
		memcpy(pReqBuf+strlen(pReqBuf),pstHttpReq->sReqBody,nnLen);
        
        strncat(pReqBuf,"\r\n",2);

	    HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "pReqBuf[%s]", pReqBuf);
}
int TopHttpCall(char *sSoapCfgName, HttpRequest *pstHttpReq, HttpResponse *pstHttpRsp, char *sFileName, int nLen)
{
	int  llResult;
	int ReqLen = 0; 
	char sHttpRspMesg[HTTP_PRE_RECV_LENGTH+1];
	char sNLen[8+1];
	
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "TopHttpCall(%s) Begin", sSoapCfgName);

	memset(sHttpRspMesg, 0x00, sizeof(sHttpRspMesg));
	
	/*获取配置文件head数据*/
	if(TopHttpClientHeadInit(&pstHttpReq->stReqHead, sSoapCfgName))
    {
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Call TopHttpClientHeadInit() Failed");
		return (-1);
	}
	IntSendDate();
	PackHttpReqMesg(pstHttpReq, sFileName,nLen);

	if(TopSocketClientInit(pstHttpReq->stReqHead.HttpServAddr, pstHttpReq->stReqHead.HttpServPort))
    {
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "TopSocketClientInit  Failed");
		ResetSend();
		return (-1);
	}
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "TopSocketClientInit  Success");
	
	if(TopSocketClientConnect())
    {
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "TopSocketClientConnect  Failed");
		ResetSend();
		return (-1);
	}

	ReqLen = atoi(sNLen);
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "TopSocketClientConnect  Success m_nSendDataLen:[%d] sNLen[%s]",m_nSendDataLen,sNLen);
	if(TopSocketClientSendMesg(m_pSendData, m_nSendDataLen))
    {
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "TopSocketClientSendMesg  Failed");
		TopSocketClientDisConnect();
		ResetSend();
		return (-1);
	}
	ResetSend();
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "TopSocketClientSendMesg  Success");

	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Begin To Call TopSocketClientRecvMesg...");
	memset(sHttpRspMesg, 0x00, sizeof(sHttpRspMesg));
	llResult = TopSocketClientRecvMesg(sHttpRspMesg, HTTP_PRE_RECV_LENGTH);
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "TopSocketClientRecvMesg  End[%d]", llResult);

	if(llResult <= 0)
    {
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,"TopSocketClientRecvMesg  Failed");
		TopSocketClientDisConnect();
		return (-1);
	}
	//HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,"TopSocketClientRecvMesg  Success[%s]", sHttpRspMesg);
	
	/*对接收信息进行解析*/
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,"ParseRespMesg  Begin...");
	if(ParseRespMesg(sHttpRspMesg, pstHttpRsp))
    {
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,"ParseRespMesg  Failed");
		TopSocketClientDisConnect();
		return (-1);
	}
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,"ParseRespMesg  Success");
  
	HttpResponseTrace(pstHttpRsp);
  
	TopSocketClientDisConnect();
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,"SOCKET 调用结束!!!!!!");
	return 0;
}

int WriteWeiXinFile(char *sFileName, char *sFileContent)
{
    int		nReturnCode = 0;
    FILE    *fp;    
    char	host_file_name[200] = {0};
    char    sDest[2048];

    memset(host_file_name,0,sizeof(host_file_name)-1);
    memcpy(host_file_name,sFileName,sizeof(host_file_name)-1);        
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "生成对账文件 : [%s]", host_file_name);
    fp = fopen(host_file_name, "w+");
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

int parseMerchant(HttpResponse *pstHttpResponse,char *pRspBody)
{
    char sRspBody[1024];
    char sMsg[1024];
    memset(sRspBody,0,sizeof(sRspBody));
    memset(sMsg,0,sizeof(sMsg));
    
    if(pstHttpResponse->stRspHead.iHttpStatus == 0)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文失败[%d]",pstHttpResponse->stRspHead.iHttpStatus);
        return -1;
    }
    else
    {
        if(pstHttpResponse->stRspHead.iHttpStatus == 200)
        {           
            if(pRspBody==NULL)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " pstHttpResponse->pRspBody is NULL");
                return -1;
            }
            memcpy(sRspBody,pRspBody+1,sizeof(sRspBody)-1);
            CommonLTrimjson(sRspBody);
            CommonRTrimjson(sRspBody);
            rtrimjson(sRspBody); 
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " sRspBody[%s]",sRspBody);     	              
            cJSON * pJson = cJSON_Parse(sRspBody);
            if(pJson == NULL)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " RecvEncryptMsg is NULL");                            
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
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " result[%s]",pResultSub->valuestring);           
                    //memcpy(pstEPayRsp->stEpayErr.sErrCode,pResultSub->valuestring,sizeof(pstEPayRsp->stEpayErr.sErrCode)-1);
                    
                    cJSON * pErrMsgSub = cJSON_GetObjectItem(pJson, "message");
                    if(NULL == pErrMsgSub)
                    {
                        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errmsg is NULL");
                        return -1;
                    }    
                    u2g(pErrMsgSub->valuestring,strlen(pErrMsgSub->valuestring),sMsg,sizeof(sMsg));                       
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errmsg[%s]",sMsg);           
                    cJSON_Delete(pJson); 
                    
                    //if(memcmp(pstEPayRsp->stEpayErr.sErrCode, "EPAY_00000", 10)==0)
                    //{
                    //    return 0;
                        
                    //}                             
                    return -1;                         
                }
            }                                
            else
            {                
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " status[%d]",pErrCodeSub->valueint);           
                
                cJSON * pErrMsgSub = cJSON_GetObjectItem(pJson, "message");
                if(NULL == pErrMsgSub)
                {
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errmsg is NULL");
                    return -1;
                }    
                u2g(pErrMsgSub->valuestring,strlen(pErrMsgSub->valuestring),sMsg,sizeof(sMsg));                       
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " errmsg[%s]",sMsg);           
                cJSON_Delete(pJson); 
                
                            
                return -1;                         
            }                                                 
            cJSON_Delete(pJson); 
                
        }
        else if(pstHttpResponse->stRspHead.iHttpStatus == 400)
        {           
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]服务异常pRspBody[%s]",pstHttpResponse->stRspHead.iHttpStatus,pRspBody);           
            return -1;
        }
        else
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " 接收报文成功[%d]未知错误pRspBody[%s]",pstHttpResponse->stRspHead.iHttpStatus,pRspBody);                       
            return -1;
        }
    }    
    return 0;   
}

int TopHttpCallEx(char *sSoapCfgName, HttpRequest *pstHttpReq, HttpResponse *pstHttpRsp, char *sFileName, int nLen)
{
	int  llResult;
	int nReturnCode;
	int ReqLen = 0; 
	char	*sHttpReqMesg = malloc(REQUEST_BODY_LEN);
	char sHttpRspMesg[HTTP_PRE_RECV_LENGTH+1];
	char sNLen[8+1];
	memset(sNLen,0x0,sizeof(sNLen));
	
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "TopHttpCall(%s) Begin", sSoapCfgName);

	memset(sHttpRspMesg, 0x00, sizeof(sHttpRspMesg));
	/*获取配置文件head数据*/
	if(TopHttpClientHeadInit(&pstHttpReq->stReqHead, sSoapCfgName))
    {
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Call TopHttpClientHeadInit() Failed");
		free(sHttpReqMesg);
		return (-1);
	}

	PackHttpReqMesgEx(pstHttpReq, sHttpReqMesg, sFileName,nLen,sNLen);

	if(TopSocketClientInit(pstHttpReq->stReqHead.HttpServAddr, pstHttpReq->stReqHead.HttpServPort))
    {
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "TopSocketClientInit  Failed");
		free(sHttpReqMesg);
		return (-1);
	}
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "TopSocketClientInit  Success");
	
	if(TopSocketClientConnect())
    {
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "TopSocketClientConnect  Failed");
		free(sHttpReqMesg);
		return (-1);
	}

	ReqLen = atoi(sNLen);
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "TopSocketClientConnect  Success ReqLen:[%d] sNLen[%s]",ReqLen,sNLen);
	/*if(TopSocketClientSendMesg(sHttpReqMesg, strlen(sHttpReqMesg))){*/
	if(TopSocketClientSendMesg(sHttpReqMesg, ReqLen))
    {
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "TopSocketClientSendMesg  Failed");
		TopSocketClientDisConnect();
		free(sHttpReqMesg);
		return (-1);
	}
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "TopSocketClientSendMesg  Success");

	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Begin To Call TopSocketClientRecvMesg...");
	memset(sHttpRspMesg, 0x00, sizeof(sHttpRspMesg));
	llResult = TopSocketClientRecvMesg(sHttpRspMesg, HTTP_PRE_RECV_LENGTH);
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "TopSocketClientRecvMesg  End[%d]", llResult);

	if(llResult <= 0)
    {
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,"TopSocketClientRecvMesg  Failed");
		TopSocketClientDisConnect();
		free(sHttpReqMesg);
		return (-1);
	}
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,"TopSocketClientRecvMesg  Success");
	
	/*对接收信息进行解析*/
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,"ParseRespMesgEx  Begin...");
	if(ParseRespMesgEx(sHttpRspMesg, pstHttpRsp))
    {
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,"ParseRespMesgEx  Failed");
		TopSocketClientDisConnect();
		free(sHttpReqMesg);
		Reset();
		return (-1);
	}
		
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,"ParseRespMesgEx  Success");
  
	HttpResponseTrace(pstHttpRsp);
  
	TopSocketClientDisConnect();
	free(sHttpReqMesg);
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,"SOCKET 调用结束!!!!!!");
	
    FlushLog();           
	nReturnCode = parseMerchant(pstHttpRsp,m_pData);
	if(nReturnCode != 0)
    {
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,"parseMerchant  Failed");		
        Reset();
		return (-1);
	}	
	WriteWeiXinFile(sFileName, m_pData);
	Reset();	
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,"处理理报文件 调用结束!!!!!!");
	return 0;
}


char *_TopCfg_RTrim(char *sChars){
	int i = strlen(sChars) - 1;
	while(sChars[i] == ' ' || sChars[i] == '\n'){
		i--;
	}
	sChars[i+1] = '\0';	
	return sChars;
}

char *_TopCfg_LTrim(char *sChars){
	int i = 0;
	while(sChars[i] == ' ' || sChars[i] == '\r' || sChars[i] == '\n'){
		i++;
	}
	return sChars + i;
}


/***********************************************************
 * 关闭配置文件
 ***********************************************************/
int Top_CloseCfgFile()
{
	if(cfgFp != NULL)
		return fclose(cfgFp);
	return 0;
}

/***********************************************************
 * 读取配置文件中某个配置段的某个配置项的值
 ***********************************************************/
int Top_GetCfgItem(char *sSectionName, char *sItemName, char *sItemValue)
{
	char sLine[256];
	char *pTmp = NULL;
	char *pTmpED = NULL;
	int  isFind = 0;
	char *pTempP = NULL;
	char sTempBuf[256];
	
	if(cfgFp == NULL){
		return (-1);	
	}
	
	rewind(cfgFp);
	
	while(fgets(sLine, sizeof(sLine), cfgFp)){
		if(sLine[0] == '#'){
			continue;	
		}
		/*左侧去除空格*/
		pTmp = _TopCfg_LTrim(sLine);

		if(!isFind){
			/*最好不要直接去搜section名称,万一出现那种一个配置段的名字中包含另一个配置段的名字时就会有问题*/
			if(pTmp[0] == '[') {
				pTempP = pTmp+1;
				pTmpED =  strstr(pTempP, "]");
				if(pTmpED == NULL){
					HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Section名配置节有问题, 应该为[section_name]!");
					return (-1);
				}
				pTempP = _TopCfg_LTrim(pTempP);

				memset(sTempBuf, 0x00, sizeof(sTempBuf));
				memcpy(sTempBuf, pTempP, pTmpED - pTempP);
				_TopCfg_RTrim(sTempBuf);

				if(strcmp(sTempBuf, sSectionName) == 0) {	
					/*查找此段中对应item的值*/
					isFind = 1;
				}
			}
			continue;
		} else {
			/*判断是否到达了下一个Section了*/
			if(pTmp[0] == '[') {
				HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "当前Section[%s]所有的配置项已经找完,但没有找到配置项[%s]!", sSectionName, sItemName);
				return (-1);
			}
			
			pTmp = strstr(pTmp, sItemName);
			if(pTmp == NULL){
				continue;
			} else {
				/*读取此段中对应item的值*/
				pTmp += (strlen(sItemName) + 1);
				pTmpED = strstr(pTmp, "#");
				if(pTmpED != NULL){
					*pTmpED = '\0';
				}
				sprintf(sItemValue, "%s", _TopCfg_RTrim(pTmp));
				return 0;
			}
		}
	}
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Cfg Item Can't be found[%s->%s->%s]", sFullName, sSectionName, sItemName);
				
	return (-1);
}

ssize_t HtRead(int fildes, void *buf, size_t nbyte)
{
    /* this HtRead can not create any problem */
    return read(fildes, buf, nbyte);
}

int HtRecv(int s, void *buf, int len, int flags)
{
    return recv(s, buf, len, flags);
}

/***********************************************************
 * 配置文件目录：
 * (1)open配置文件 etc/httpcfg.cfg
 ***********************************************************/
int TopSoap_OpenCfgFile(char *sFileName)
{
	memset(sFullName, 0x00, sizeof(sFullName));
	char PathTmp[50+1];
	memset(PathTmp,0x0,sizeof(PathTmp));
	
	if(getenv (TOPSOAP_CFG_PATH)){
		strncpy(sFullName, (char *)getenv(TOPSOAP_CFG_PATH), strlen((char *)getenv(TOPSOAP_CFG_PATH)) + 1);
	}
	if(strlen(sFullName) == 0) {
		strcpy(PathTmp, getenv("BATCHHOME"));
		RightTrim(PathTmp);
		strncpy(sFullName, PathTmp, strlen(PathTmp) + 1);
		strcat(sFullName, "/etc");
	}
	
	strncat(sFullName, "/",1 + 1);
	strncat(sFullName, sFileName, strlen(sFileName) + 1);
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "cfg file name [%s]", sFullName);
	
	cfgFp = fopen(sFullName, "r");
	if (cfgFp == NULL ){
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Can't fOpen Cfg File[%s]", sFullName);
		return (errno);
	}
	return 0;
}


int GetHttpCfgItem(char *sItemName, char *sItemValue)
{
	return Top_GetCfgItem("HTTP", sItemName, sItemValue);
} 

/**发送报文**/
int TopSocketSendMesg(int socketfd, char *pMesgBuf, int iMesgLen)
{
	int	unsendLen,sendLen;
	char	*pBuf = pMesgBuf;
	
	unsendLen = iMesgLen;


	//HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "-----------------send buf-----------------iMesgLen[%d]",iMesgLen);
	//HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "pBuf:[%s],unsendLen:[%d]" ,pBuf, unsendLen); 

	do{
		if((sendLen = send(socketfd, pBuf, unsendLen, 0)) < 0)
		{
	   		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "send request to server failed[ERROR: %s]",strerror(errno));
			return (-1);
		}
		pBuf = pBuf + sendLen;
		unsendLen -= sendLen;
	}while(unsendLen > 0);
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sendLen:[%d]" ,sendLen); 
	return 0;
}

/**
接收报文
Modified By Brady.Lee @ 20090925 
	修改函数的返回值:成功返回接收到的长度;失败则返回-1
**/
int TopSocketRecvMesg(int socketfd, char *pMesgBuf, int iMesgLen)
{
	int	    recvLen = 0; 

	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Require Recv Len[%d]", iMesgLen);

	if((recvLen = HtRecv(socketfd, pMesgBuf, iMesgLen, 0)) < 0)
	{
	  HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "recv response from server failed[%s]",strerror(errno)); 
		return (-1); 
	} 
		
		//HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,  "recvLen[%d]", recvLen );
		HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "-----------------recv buf-----------------");
		//HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "pMesgBuf:[%s], recvLen:[%d]", pMesgBuf, recvLen);
		
	return recvLen;	 
}

/*连接参数初始化*/
int TopSocketClientInit(char *sSvrIpAddr, char *sSvrPort)
{
	int  llResult = 0;
	int  nSvrPort = 0;
	nSvrPort = atoi(sSvrPort);
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,  "TopSocketClientInit Start...sSvrIpAddr[%s]  nSvrPort[%d]",sSvrIpAddr,nSvrPort);
	
	/*初始化 Server端 的地址信息*/ 
	memset(&server_addr, 0x00, sizeof(struct sockaddr_in)); 
	server_addr.sin_family = AF_INET; 
	server_addr.sin_addr.s_addr = inet_addr(sSvrIpAddr);

	server_addr.sin_port = htons(nSvrPort); 
	
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "[SrvIP  ][%s]", sSvrIpAddr);
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "[SrvPort][%d]", nSvrPort);

	/*初始化Client端*/ 
	if((clientSockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
	{ 
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Socket Error[%s] ,[%d]", strerror(errno),clientSockfd);
		return (-1); 
	} 
 else
  {
     HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "[SrvIP  ][%s]", strerror(errno));
     HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "[SrvIP  ][%d]", clientSockfd);
  }
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "TopSocketClientInit Success");
	
	return 0;
}


/**连接到Server端**/ 
int TopSocketClientConnect()
{
	if(connect(clientSockfd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr))==-1){ 
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Connect Error : %s ",strerror(errno));

		switch(errno) {
			case ECONNREFUSED:/*The attempt to connect was forcefully rejected*/
			case ENETUNREACH:/*The network is not reachable from this host*/
			case ETIMEDOUT:/*Connection establishment timed out without establishing a connection.*/

				break;
			default:

				break;
		}
		return (-1); 
	}
	HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "Connect Success ");
	return 0;
}

/*向服务端发送消息*/
int TopSocketClientSendMesg(char *pReqMesgBuf, int iMesgLen)
{
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "iMesgLen[%d]", iMesgLen);
	if(TopSocketSendMesg(clientSockfd, pReqMesgBuf, iMesgLen)){
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "CLIENT SEND REQUEST MESG FAILED!");
		return (-1);
	}
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "CLIENT SEND REQUEST MESG SUCCSSFULLY...\n%s", pReqMesgBuf);
	return 0;
}

/**
Modified By Brady.Lee @ 20090925 
	修改函数的返回值:成功返回接收到的长度;失败则返回-1
**/
int TopSocketClientRecvMesg(char *pRspMesgBuf, int iMesgLen)
{
	int llResult = 0;
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "TopSocketClientRecvMesg Begin ...");

	llResult = TopSocketRecvMesg(clientSockfd, pRspMesgBuf, iMesgLen);
	if(llResult < 0){
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "CLIENT RESV RESPONSE MESG FAILED!");
		return (-1);
	}
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "CLIENT RESV RESPONSE MESG SUCCSSFULLY...\n%s", pRspMesgBuf);
	return llResult;
}

/**断开与Server端的连接**/ 
int TopSocketClientDisConnect()
{
	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "TopSocketClientDisConnect Begin ...");
	shutdown(clientSockfd,2);
	close(clientSockfd); 	
}

int ParseRespMesg(char *sHttpRspBuf, HttpResponse *pstHttpRsp)
{
	char *pTmpST = NULL;
	char *pTmpED = NULL;
	char rspHeadBuf[1024];
	char tempBuf[128];
	char *pRspBody = NULL;
	int  isChunked = 0;
	
	int  i, llResult;
	char sHttpChunkedMesg[HTTP_PRE_RECV_LENGTH+1];
	
	int  nExistContentLen = 0;	/*返回报文中是否存在Content-Length*/
	int  nRespBodyLen = 0;	/*只对返回报文非chunked编码时可用*/
	
	pstHttpRsp->stRspHead.iRspHeadPairNum = 0;
	
	pTmpST = sHttpRspBuf;
	pTmpED = strstr(pTmpST, "\r\n\r\n");
	/*响应内容*/
	pRspBody = pTmpED + 4;
	/*响应头*/
	memset(rspHeadBuf, 0x00, sizeof(rspHeadBuf));
	memcpy(rspHeadBuf, pTmpST, pTmpED-pTmpST);
	/*"HTTP/1.1 200 OK"*/
	pTmpST = rspHeadBuf;
	pTmpST = _TopCfg_LTrim(pTmpST);
	pTmpST += 8;
	pTmpST = _TopCfg_LTrim(pTmpST);
	pTmpED = strstr(pTmpST, " ");
	if(pTmpED == NULL){
		return (-1);	
	}
	memset(tempBuf, 0x00, sizeof(tempBuf));
	memcpy(tempBuf, pTmpST, pTmpED-pTmpST);
	pstHttpRsp->stRspHead.iHttpStatus = atoi(tempBuf);
	pTmpST = _TopCfg_LTrim(pTmpED);
	pTmpED = strstr(pTmpST, "\r\n");
	if(pTmpED == NULL){
		return (-1);	
	}
	memcpy(pstHttpRsp->stRspHead.sHttpStatusDesc, pTmpST, pTmpED-pTmpST);
	/*开始处理键值对*/
	pTmpST = pTmpED + 2;
	pTmpST = _TopCfg_LTrim(pTmpED);

	while((pTmpED = strstr(pTmpST, "\r\n")) != NULL)
	{
		memset(tempBuf, 0x00, sizeof(tempBuf));
		memcpy(tempBuf, pTmpST, pTmpED-pTmpST);
		pTmpST = pTmpED + 2;
		
		pTmpED = strstr(tempBuf, ":");
		if(pTmpED == NULL){
			return (-1);	
		}
		memcpy(pstHttpRsp->stRspHead.stRspHeadPairs[pstHttpRsp->stRspHead.iRspHeadPairNum].Key, tempBuf, pTmpED-tempBuf);
		sprintf(pstHttpRsp->stRspHead.stRspHeadPairs[pstHttpRsp->stRspHead.iRspHeadPairNum].Value,"%s",_TopCfg_LTrim(pTmpED+1));
		pstHttpRsp->stRspHead.iRspHeadPairNum++;
	}
	/*最后的键值对*/
	pTmpED = strstr(pTmpST, ":");
	if(pTmpED == NULL)
    {
		return (-1);	
	}
	memcpy(pstHttpRsp->stRspHead.stRspHeadPairs[pstHttpRsp->stRspHead.iRspHeadPairNum].Key, pTmpST, pTmpED-pTmpST);
	sprintf(pstHttpRsp->stRspHead.stRspHeadPairs[pstHttpRsp->stRspHead.iRspHeadPairNum].Value,"%s",_TopCfg_LTrim(pTmpED+1));
	pstHttpRsp->stRspHead.iRspHeadPairNum++;
	
	for( i = 0; i < pstHttpRsp->stRspHead.iRspHeadPairNum; i++) 
	{   
		HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Key [%s]", pstHttpRsp->stRspHead.stRspHeadPairs[i].Key);
		HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Vlaue [%s]", pstHttpRsp->stRspHead.stRspHeadPairs[i].Value);

        if(_XML_cmpStrIgCase(pstHttpRsp->stRspHead.stRspHeadPairs[i].Key, "Transfer-Encoding") == 0
            &&  _XML_cmpStrIgCase(pstHttpRsp->stRspHead.stRspHeadPairs[i].Value, "chunked") == 0)
        {
            isChunked = 1;    
        }
        if(_XML_cmpStrIgCase(pstHttpRsp->stRspHead.stRspHeadPairs[i].Key, "Content-Length") == 0){
						nExistContentLen = 1;
            nRespBodyLen = atoi(pstHttpRsp->stRspHead.stRspHeadPairs[i].Value);
        }
        if(_XML_cmpStrIgCase(pstHttpRsp->stRspHead.stRspHeadPairs[i].Key, "trans_status") == 0){
						pstHttpRsp->stRspHead.iHttpStatus = atoi(pstHttpRsp->stRspHead.stRspHeadPairs[i].Value);
        }
    }
	
	/*解析响应报文体*/
	if(isChunked)
    {
		HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Chunked Response Mesg");
		HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "strlen(pRspBody) = %d", strlen(pRspBody));
        if(strlen(pRspBody) <= 0) 
        {
        	/*Receive Mesg Again*/
            HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Begin To Call TopSocketClientRecvMesg...");
            memset(sHttpChunkedMesg, 0x00, sizeof(sHttpChunkedMesg));
            llResult = TopSocketClientRecvMesg(sHttpChunkedMesg, HTTP_PRE_RECV_LENGTH);
            if(llResult < 0)
            {
                HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "TopSocketClientRecvMesg  Failed");
                return (-1);
            }
            HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "TopSocketClientRecvMesg  Success");
            pRspBody = sHttpChunkedMesg;
        } 
		/*解析chunked报文*/
		HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Begin to parseChunkedMesg...");
		if(parseChunkedMesg(pRspBody, pstHttpRsp->sRspBody, sizeof(pstHttpRsp->sRspBody)))
        {
			HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "parseChunkedMesg Failed");
			return (-1);
		} 
		else 
		{
			return 0;
		}
	} 
	else 
	{
		HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,  "NOT Chunked Response Mesg");

		if(!nExistContentLen) {
					HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,  "响应报文格式不正确,没有报文长度Content-Length!");
        	return (-1);
		}
		
		if(nRespBodyLen == 0) {
			return 0;
		}
        
		if(nRespBodyLen >= sizeof(pstHttpRsp->sRspBody)) {
					HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,  "响应报文的长度[%d]不小于预定义的BUF长度[%d]", nRespBodyLen, sizeof(pstHttpRsp->sRspBody));
        	return (-1);
        }
		
		if(strlen(pRspBody) >= nRespBodyLen) {
			memcpy(pstHttpRsp->sRspBody, pRspBody, nRespBodyLen);
			return 0;
		}
        
        memcpy(pstHttpRsp->sRspBody, pRspBody, strlen(pRspBody));
        nRespBodyLen -= strlen(pRspBody);
        pRspBody = pstHttpRsp->sRspBody + strlen(pRspBody);
        
        while(1) 
        {
        	/*Receive Mesg Again*/
        	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,  "Begin To Call TopSocketClientRecvMesg...");
            memset(sHttpChunkedMesg, 0x00, sizeof(sHttpChunkedMesg));
            llResult = TopSocketClientRecvMesg(sHttpChunkedMesg, HTTP_PRE_RECV_LENGTH);

            if(llResult < 0){
            		HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,  "TopSocketClientRecvMesg  Failed");
                return (-1);
            }
            HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,  "TopSocketClientRecvMesg  Success");

            memcpy(pRspBody, sHttpChunkedMesg, llResult);
            pRspBody += llResult;
            nRespBodyLen -= llResult;
            if(nRespBodyLen <= 0) {
            	break;
            }
            
        }
        
        return 0;
	}
	
	return 0;
}


int parseChunkedMesgMEx(char *sChunkedMesg, char *sRspBodyBuf, int *lMaxLen)
{
	char *pTmpST = NULL;
	char *pTmpED = NULL;
	char *pTmpED2 = NULL;
	char sHttpChunkedMesg[HTTP_PRE_RECV_LENGTH+1];
	char tmpBuf[1024];
	int  chunkedSize = 0;
	char *pRspMsg= sRspBodyBuf;
	int  llResult = 0;
	int  nBufLeftLen = *lMaxLen;
    int  iTimes;
	pTmpST = sChunkedMesg;
		
	int m_nRspDataLen = 0;
	int nNewAllocLen = 0;
	int m_nRspAllocLen = HTTP_PRE_RECV_LENGTH;
	int nLen = 0;

    HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,  "nBufLeftLen=[%d]", nBufLeftLen);
    
    

	while(1) 
	{		
		pTmpED = strstr(pTmpST, "\r\n");		
		if(pTmpED == NULL)
        {
			memset(tmpBuf, 0x00, sizeof(tmpBuf));
			memcpy(tmpBuf, pTmpST, strlen(pTmpST));
			HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,  "pTmpED == NULL Begin To Call TopSocketClientRecvMesg...");
			memset(sHttpChunkedMesg, 0x00, sizeof(sHttpChunkedMesg));
            llResult = TopSocketClientRecvMesg(sHttpChunkedMesg, HTTP_PRE_RECV_LENGTH);
            if(llResult < 0)
            {
            	HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,  "TopSocketClientRecvMesg  Failed");
                return (-1);
            }
			pTmpST = sHttpChunkedMesg;
			pTmpED = strstr(pTmpST, "\r\n");
			if(pTmpED == NULL)
            {
                HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,  "pTmpED == NULL");
				return (-1);
			}
			memcpy(tmpBuf + strlen(tmpBuf), pTmpST, pTmpED - pTmpST);
		}
        else
        {
 			memset(tmpBuf, 0x00, sizeof(tmpBuf));
			if(pTmpED-pTmpST > sizeof(tmpBuf)) 
			{
				HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,  "pTmpED-pTmpST[%d] > sizeof(tmpBuf)[%d]",pTmpED-pTmpST > sizeof(tmpBuf));
				return (-1);
			}
			memcpy(tmpBuf, pTmpST, pTmpED-pTmpST);
			HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "chunked Len[%s]", tmpBuf);
		}
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,"Begin to Call _XML_s16to10...");
		chunkedSize = _XML_s16to10(tmpBuf); /*16进制转十进制 */		
		if(chunkedSize == 0)
        {
            HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,"m_nRspDataLen=[%d]",m_nDataLen); 
            *lMaxLen = m_nDataLen;
            //memcpy(sRspBodyBuf,m_pData,m_nDataLen);
			return 0;
		}
		
		pTmpST = pTmpED + 2;
		HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,"chunkedSize[%d] , buf剩余Len[%d]", chunkedSize, strlen(pTmpST));

		if(chunkedSize > strlen(pTmpST)) 
        {
            HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,"branch 1: chunkedSize[%d] > buf剩余Len[%d]",chunkedSize,strlen(pTmpST));
            //memcpy(pRspMsg, pTmpST, strlen(pTmpST));
			chunkedSize = chunkedSize - strlen(pTmpST);
			//pRspMsg = pRspMsg + strlen(pTmpST);	
            RawLog(pTmpST, strlen(pTmpST));
            //FlushLog();	
	        
            iTimes = 0;
			while(1) 
			{
				memset(sHttpChunkedMesg, 0x00, sizeof(sHttpChunkedMesg));
				HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,"chunkedSize [%d]!", chunkedSize);
				HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,"parseChunkedMesgEx2 Begin To TopSocketClientRecvMesg...");

	            llResult = TopSocketClientRecvMesg(sHttpChunkedMesg, HTTP_PRE_RECV_LENGTH);

	            HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,"parseChunkedMesgEx2 Call TopSocketClientRecvMesg[%d]!",llResult);

	            if(llResult < 0)
                {
                	HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,"TopSocketClientRecvMesg Failed");
	                return (-1);
	            }
				if(llResult < chunkedSize) 
                {					
                    //memcpy(pRspMsg, sHttpChunkedMesg, llResult);
					chunkedSize -= llResult;
					//pRspMsg = pRspMsg + llResult;
                    RawLog(sHttpChunkedMesg, llResult);
                    //FlushLog();		
	                					
                    /**最多接收10次 by xcl 20101011**/
                    //iTimes ++;
                    //if(iTimes > 10 )
                    //{
                    //    HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,"接收次数超限" );
					//    return (-1);
                    //}
					continue;
				} 				
                RawLog(sHttpChunkedMesg, chunkedSize);
                //FlushLog();		
                pTmpST = sHttpChunkedMesg + chunkedSize;	            
				
				break;
			}

		} 
		else 
        {
			HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,"branch 2: chunkedSize[%d] <= buf剩余Len[%d]",chunkedSize,strlen(pTmpST));
			pTmpST = _TopCfg_LTrim(pTmpED);			
            RawLog(pTmpST, chunkedSize);
            //FlushLog();		
			pTmpST = pTmpST + chunkedSize;

		}
		pTmpST = _TopCfg_LTrim(pTmpST);
	}
	*lMaxLen=m_nRspDataLen;
	return 0;
}

int ParseRespMesgEx(char *sHttpRspBuf, HttpResponse *pstHttpRsp)
{
	char *pTmpST = NULL;
	char *pTmpED = NULL;
	char rspHeadBuf[1024];
	char tempBuf[128];
	char *pRspBody = NULL;
	int  isChunked = 0;
	
	int  i, llResult;
	char sHttpChunkedMesg[HTTP_PRE_RECV_LENGTH+1];
	
	int  nExistContentLen = 0;	/*返回报文中是否存在Content-Length*/
	int  nRespBodyLen = 0;	/*只对返回报文非chunked编码时可用*/
	
	pstHttpRsp->stRspHead.iRspHeadPairNum = 0;
	
	pTmpST = sHttpRspBuf;
	pTmpED = strstr(pTmpST, "\r\n\r\n");
	/*响应内容*/
	pRspBody = pTmpED + 4;
	/*响应头*/
	memset(rspHeadBuf, 0x00, sizeof(rspHeadBuf));
	memcpy(rspHeadBuf, pTmpST, pTmpED-pTmpST);
	
	/*"HTTP/1.1 200 OK"*/
	pTmpST = rspHeadBuf;
	pTmpST = _TopCfg_LTrim(pTmpST);
	pTmpST += 8;
	pTmpST = _TopCfg_LTrim(pTmpST);
	pTmpED = strstr(pTmpST, " ");
	if(pTmpED == NULL){
		return (-1);	
	}
	memset(tempBuf, 0x00, sizeof(tempBuf));
	memcpy(tempBuf, pTmpST, pTmpED-pTmpST);
	pstHttpRsp->stRspHead.iHttpStatus = atoi(tempBuf);
	pTmpST = _TopCfg_LTrim(pTmpED);
	pTmpED = strstr(pTmpST, "\r\n");
	if(pTmpED == NULL){
		return (-1);	
	}
	memcpy(pstHttpRsp->stRspHead.sHttpStatusDesc, pTmpST, pTmpED-pTmpST);
	/*开始处理键值对*/
	pTmpST = pTmpED + 2;
	pTmpST = _TopCfg_LTrim(pTmpED);

	while((pTmpED = strstr(pTmpST, "\r\n")) != NULL)
	{
		memset(tempBuf, 0x00, sizeof(tempBuf));
		memcpy(tempBuf, pTmpST, pTmpED-pTmpST);
		pTmpST = pTmpED + 2;
		
		pTmpED = strstr(tempBuf, ":");
		if(pTmpED == NULL){
			return (-1);	
		}
		memcpy(pstHttpRsp->stRspHead.stRspHeadPairs[pstHttpRsp->stRspHead.iRspHeadPairNum].Key, tempBuf, pTmpED-tempBuf);
		sprintf(pstHttpRsp->stRspHead.stRspHeadPairs[pstHttpRsp->stRspHead.iRspHeadPairNum].Value,"%s",_TopCfg_LTrim(pTmpED+1));
		pstHttpRsp->stRspHead.iRspHeadPairNum++;
	}
	/*最后的键值对*/
	pTmpED = strstr(pTmpST, ":");
	if(pTmpED == NULL)
    {
		return (-1);	
	}
	memcpy(pstHttpRsp->stRspHead.stRspHeadPairs[pstHttpRsp->stRspHead.iRspHeadPairNum].Key, pTmpST, pTmpED-pTmpST);
	sprintf(pstHttpRsp->stRspHead.stRspHeadPairs[pstHttpRsp->stRspHead.iRspHeadPairNum].Value,"%s",_TopCfg_LTrim(pTmpED+1));
	pstHttpRsp->stRspHead.iRspHeadPairNum++;
	
	for( i = 0; i < pstHttpRsp->stRspHead.iRspHeadPairNum; i++) 
	{   
		HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Key [%s]", pstHttpRsp->stRspHead.stRspHeadPairs[i].Key);
		HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Vlaue [%s]", pstHttpRsp->stRspHead.stRspHeadPairs[i].Value);

        if(_XML_cmpStrIgCase(pstHttpRsp->stRspHead.stRspHeadPairs[i].Key, "Transfer-Encoding") == 0
            &&  _XML_cmpStrIgCase(pstHttpRsp->stRspHead.stRspHeadPairs[i].Value, "chunked") == 0)
        {
            isChunked = 1;    
        }
        if(_XML_cmpStrIgCase(pstHttpRsp->stRspHead.stRspHeadPairs[i].Key, "Content-Length") == 0){
						nExistContentLen = 1;
            nRespBodyLen = atoi(pstHttpRsp->stRspHead.stRspHeadPairs[i].Value);
        }
        if(_XML_cmpStrIgCase(pstHttpRsp->stRspHead.stRspHeadPairs[i].Key, "trans_status") == 0){
						pstHttpRsp->stRspHead.iHttpStatus = atoi(pstHttpRsp->stRspHead.stRspHeadPairs[i].Value);
        }
    }
	
	IntDate();
	/*解析响应报文体*/
	if(isChunked)
    {
		HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Chunked Response Mesg");
		HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "strlen(pRspBody) = %d", strlen(pRspBody));
        if(strlen(pRspBody) <= 0) 
        {
        	/*Receive Mesg Again*/
            HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Begin To Call TopSocketClientRecvMesg...");
            memset(sHttpChunkedMesg, 0x00, sizeof(sHttpChunkedMesg));
            llResult = TopSocketClientRecvMesg(sHttpChunkedMesg, HTTP_PRE_RECV_LENGTH);
            if(llResult < 0)
            {
                HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "TopSocketClientRecvMesg  Failed");
                return (-1);
            }
            HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "TopSocketClientRecvMesg  Success");
            pRspBody = sHttpChunkedMesg;
        } 
		/*解析chunked报文*/
		HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Begin to parseChunkedMesg...");
		if(parseChunkedMesgMEx(pRspBody, pstHttpRsp->pRspBody, &pstHttpRsp->nDataLen))
        {
			HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "parseChunkedMesg Failed");			
			return (-1);
		} 
		else 
		{
			return 0;
		}
	} 
	else 
	{
		HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,  "NOT Chunked Response Mesg");

		if(!nExistContentLen) 
		{
					HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,  "响应报文格式不正确,没有报文长度Content-Length!");
        	return (-1);
		}
		
		if(nRespBodyLen == 0) {
			return 0;
		}
        
		//if(nRespBodyLen >= sizeof(pstHttpRsp->sRspBody)) {
		//			HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,  "响应报文的长度[%d]不小于预定义的BUF长度[%d]", nRespBodyLen, sizeof(pstHttpRsp->sRspBody));
        //	return (-1);
        //}
		
		if(strlen(pRspBody) >= nRespBodyLen) {
			//memcpy(pstHttpRsp->pRspBody, pRspBody, nRespBodyLen);
			RawLog(pRspBody, nRespBodyLen);
			return 0;
		}
        
        //memcpy(pstHttpRsp->pRspBody, pRspBody, strlen(pRspBody));
        RawLog(pRspBody, strlen(pRspBody));
        //nRespBodyLen -= strlen(pRspBody);
        //pRspBody = pstHttpRsp->pRspBody + strlen(pRspBody);
        
        while(1) 
        {
        	/*Receive Mesg Again*/
        	HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,  "Begin To Call TopSocketClientRecvMesg...");
            memset(sHttpChunkedMesg, 0x00, sizeof(sHttpChunkedMesg));
            llResult = TopSocketClientRecvMesg(sHttpChunkedMesg, HTTP_PRE_RECV_LENGTH);

            if(llResult < 0){
            		HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,  "TopSocketClientRecvMesg  Failed");
                return (-1);
            }
            HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,  "TopSocketClientRecvMesg  Success");

            //memcpy(pRspBody, sHttpChunkedMesg, llResult);
            RawLog(sHttpChunkedMesg, llResult);
            //pRspBody += llResult;
            nRespBodyLen -= llResult;
            if(nRespBodyLen <= 0) {
            	break;
            }
            
        }
        
        return 0;
	}
	
	return 0;
}
/**忽略大小写，进行字符串比较,结果只有 相等 或者 不相等**/
int _XML_cmpStrIgCase(char *s1, char *s2)
{
	int i;
	char *ps1 = s1;
	char *ps2 = s2;
	
	if (s1 == NULL && s2 == NULL)
		return (0);
	if (s1 == NULL || s2 == NULL)
		return (-1);
	if(strlen(ps1) != strlen(ps2)){
		return (-1);	
	}
	for (i = 0; i < strlen(ps1); i++){
		if (toupper(ps1[i]) != toupper(ps2[i]))
			return (-1);
	}
	return 0;
}

int parseChunkedMesg(char *sChunkedMesg, char *sRspBodyBuf, int lMaxLen)
{
	char *pTmpST = NULL;
	char *pTmpED = NULL;
	char *pTmpED2 = NULL;
	char sHttpChunkedMesg[HTTP_PRE_RECV_LENGTH+1];
	char tmpBuf[1024];
	int  chunkedSize = 0;
	char *pRspBody = sRspBodyBuf;
	int  llResult = 0;
	int  nBufLeftLen = lMaxLen;
    int  iTimes;
	pTmpST = sChunkedMesg;

    HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,  "nBufLeftLen=[%d]", nBufLeftLen);

	while(1) 
	{		
		pTmpED = strstr(pTmpST, "\r\n");		
		if(pTmpED == NULL)
        {
			memset(tmpBuf, 0x00, sizeof(tmpBuf));
			memcpy(tmpBuf, pTmpST, strlen(pTmpST));
			HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,  "Begin To Call TopSocketClientRecvMesg...");
			memset(sHttpChunkedMesg, 0x00, sizeof(sHttpChunkedMesg));
            llResult = TopSocketClientRecvMesg(sHttpChunkedMesg, HTTP_PRE_RECV_LENGTH);
            if(llResult < 0)
            {
            	HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,  "TopSocketClientRecvMesg  Failed");
                return (-1);
            }
			pTmpST = sHttpChunkedMesg;
			pTmpED = strstr(pTmpST, "\r\n");
			if(pTmpED == NULL)
            {
				return (-1);
			}
			memcpy(tmpBuf + strlen(tmpBuf), pTmpST, pTmpED - pTmpST);
		}
        else
        {
			memset(tmpBuf, 0x00, sizeof(tmpBuf));
			if(pTmpED-pTmpST > sizeof(tmpBuf)) 
			{
				HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,  "Begin to Call _XML_s16to10...");
				return (-1);
			}
			memcpy(tmpBuf, pTmpST, pTmpED-pTmpST);
			HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "chunked Len[%s]", tmpBuf);
		}
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,"Begin to Call _XML_s16to10...");
		chunkedSize = _XML_s16to10(tmpBuf); /*16进制转十进制 */		
		if(chunkedSize == 0)
        {
			return 0;
		}
		
		pTmpST = pTmpED + 2;
		HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,"chunkedSize[%d] , buf剩余Len[%d]", chunkedSize, strlen(pTmpST));

		if(chunkedSize > strlen(pTmpST)) 
        {
            HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,"branch 1: chunkedSize[%d] > buf剩余Len[%d]",chunkedSize,strlen(pTmpST));
			if(nBufLeftLen < strlen(pTmpST)) 
			{
				HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,"Http Body预留的Buffer长度不够!!");
				return (-1);
			}
			memcpy(pRspBody, pTmpST, strlen(pTmpST));
			chunkedSize = chunkedSize - strlen(pTmpST);
			pRspBody = pRspBody + strlen(pTmpST);
			nBufLeftLen -= strlen(pTmpST);
			
            iTimes = 0;
			while(1) 
			{
				memset(sHttpChunkedMesg, 0x00, sizeof(sHttpChunkedMesg));
				HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,"chunkedSize [%d]!", chunkedSize);
				HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,"Begin To TopSocketClientRecvMesg...");

	            llResult = TopSocketClientRecvMesg(sHttpChunkedMesg, HTTP_PRE_RECV_LENGTH);

	            HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,"Call TopSocketClientRecvMesg[%d]!",llResult);

	            if(llResult < 0)
                {
                	HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,"TopSocketClientRecvMesg Failed");
	                return (-1);
	            }
				if(llResult < chunkedSize) 
                {
					if(nBufLeftLen < strlen(pTmpST)) 
                    {
                        HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,"Http Body预留的Buffer长度不够!!");
						return (-1);
					}
					memcpy(pRspBody, sHttpChunkedMesg, llResult);
					chunkedSize -= llResult;
					pRspBody = pRspBody + llResult;
					nBufLeftLen -= llResult;
                    /**最多接收10次 by xcl 20101011**/
                    iTimes ++;
                    if(iTimes > 10 )
                    {
                        HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,"接收次数超限" );
					    return (-1);
                    }
					continue;
				} 
				
				if(nBufLeftLen < chunkedSize)
                {
                    HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,"Http Body预留的Buffer长度不够!!");
					return (-1);
				}
				memcpy(pRspBody, sHttpChunkedMesg, chunkedSize);
				pTmpST = sHttpChunkedMesg + chunkedSize;
				pRspBody = pRspBody + chunkedSize;
				nBufLeftLen -= chunkedSize;
				break;
			}

		} 
		else 
        {
			HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,"branch 2: chunkedSize <= buf剩余Len");
			pTmpST = _TopCfg_LTrim(pTmpED);
			if(nBufLeftLen < strlen(pTmpST)) 
            {
				HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,"Http Body预留的Buffer长度不够!!");
				return (-1);
			}
			memcpy(pRspBody, pTmpST, chunkedSize);
			pTmpST = pTmpST + chunkedSize;
			pRspBody = pRspBody + chunkedSize;
			nBufLeftLen -= chunkedSize;
		}
		pTmpST = _TopCfg_LTrim(pTmpST);
	}
	
	return 0;
}

int parseChunkedMesgEx(char *sChunkedMesg, char *sRspBodyBuf, int *lMaxLen)
{
	char *pTmpST = NULL;
	char *pTmpED = NULL;
	char *pTmpED2 = NULL;
	char sHttpChunkedMesg[HTTP_PRE_RECV_LENGTH+1];
	char tmpBuf[1024];
	int  chunkedSize = 0;
	char *pRspMsg= sRspBodyBuf;
	int  llResult = 0;
	int  nBufLeftLen = *lMaxLen;
    int  iTimes;
	pTmpST = sChunkedMesg;
	int m_nDataLen = 0;
	int nNewAllocLen = 0;

    HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,  "nBufLeftLen=[%d]", nBufLeftLen);

	while(1) 
	{		
		pTmpED = strstr(pTmpST, "\r\n");		
		if(pTmpED == NULL)
        {
			memset(tmpBuf, 0x00, sizeof(tmpBuf));
			memcpy(tmpBuf, pTmpST, strlen(pTmpST));
			HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__,  "pTmpED == NULL Begin To Call TopSocketClientRecvMesg...");
			memset(sHttpChunkedMesg, 0x00, sizeof(sHttpChunkedMesg));
            llResult = TopSocketClientRecvMesg(sHttpChunkedMesg, HTTP_PRE_RECV_LENGTH);
            if(llResult < 0)
            {
            	HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,  "TopSocketClientRecvMesg  Failed");
                return (-1);
            }
			pTmpST = sHttpChunkedMesg;
			pTmpED = strstr(pTmpST, "\r\n");
			if(pTmpED == NULL)
            {
                HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,  "pTmpED == NULL");
				return (-1);
			}
			memcpy(tmpBuf + strlen(tmpBuf), pTmpST, pTmpED - pTmpST);
		}
        else
        {
 			memset(tmpBuf, 0x00, sizeof(tmpBuf));
			if(pTmpED-pTmpST > sizeof(tmpBuf)) 
			{
				HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,  "pTmpED-pTmpST[%d] > sizeof(tmpBuf)[%d]",pTmpED-pTmpST > sizeof(tmpBuf));
				return (-1);
			}
			memcpy(tmpBuf, pTmpST, pTmpED-pTmpST);
			HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "chunked Len[%s]", tmpBuf);
		}
		HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,"Begin to Call _XML_s16to10...");
		chunkedSize = _XML_s16to10(tmpBuf); /*16进制转十进制 */		
		if(chunkedSize == 0)
        {
            *lMaxLen = m_nDataLen;
			return 0;
		}
		
		pTmpST = pTmpED + 2;
		HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,"chunkedSize[%d] , buf剩余Len[%d]", chunkedSize, strlen(pTmpST));

		if(chunkedSize > strlen(pTmpST)) 
        {
            HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,"branch 1: chunkedSize[%d] > buf剩余Len[%d]",chunkedSize,strlen(pTmpST));
            memcpy(pRspMsg, pTmpST, strlen(pTmpST));
			chunkedSize = chunkedSize - strlen(pTmpST);
			pRspMsg = pRspMsg + strlen(pTmpST);	
            iTimes = 0;
			while(1) 
			{
				memset(sHttpChunkedMesg, 0x00, sizeof(sHttpChunkedMesg));
				HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,"chunkedSize [%d]!", chunkedSize);
				HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,"parseChunkedMesgEx2 Begin To TopSocketClientRecvMesg...");

	            llResult = TopSocketClientRecvMesg(sHttpChunkedMesg, HTTP_PRE_RECV_LENGTH);

	            HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,"parseChunkedMesgEx2 Call TopSocketClientRecvMesg[%d]!",llResult);

	            if(llResult < 0)
                {
                	HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,"TopSocketClientRecvMesg Failed");
	                return (-1);
	            }
				if(llResult < chunkedSize) 
                {					
                    memcpy(pRspMsg, sHttpChunkedMesg, llResult);
					chunkedSize -= llResult;
					pRspMsg = pRspMsg + llResult;
                    /**最多接收10次 by xcl 20101011**/
                    iTimes ++;
                    if(iTimes > 10 )
                    {
                        HtLog(gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,"接收次数超限" );
					    return (-1);
                    }
					continue;
				} 				
                memcpy(pRspMsg, sHttpChunkedMesg, chunkedSize);
				pTmpST = sHttpChunkedMesg + chunkedSize;
				pRspMsg = pRspMsg + chunkedSize;
				break;
			}

		} 
		else 
        {
			HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,"branch 2: chunkedSize[%d] <= buf剩余Len[%d]",chunkedSize,strlen(pTmpST));
			pTmpST = _TopCfg_LTrim(pTmpED);
			memcpy(pRspMsg, pTmpST, chunkedSize);
			HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,"branch 2: chunkedSize[%d] <= buf剩余Len[%d]",chunkedSize,strlen(pTmpST));
			pTmpST = pTmpST + chunkedSize;
			pRspMsg = pRspMsg + chunkedSize;
		}
		pTmpST = _TopCfg_LTrim(pTmpST);
	}
	*lMaxLen=m_nDataLen;
	return 0;
}
/*16进制转十进制*/
int _XML_s16to10(char *s)
{
	int lRet;
	char *pch = NULL;
	char ch;
	
	lRet = 0;
	pch = s;
	
	while (*pch) {
		ch=*pch;
		if ((ch >= '0') && (ch <= '9')) ch -= '0'; 
		else if ((ch >= 'A') && (ch <= 'F')) ch -= '7'; 
		else if ((ch >= 'a') && (ch <= 'f')) ch -= 'W'; 
		
		lRet= (lRet << 4 ) + ch;
		pch ++;
	}
	HtLog(gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,  "_XML_s16to10 : [%s]->[%d]", s, lRet);
	
	return lRet;
} 


