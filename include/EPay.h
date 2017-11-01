#ifndef _EPAY_H_
#define _EPAY_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <memory.h>
#include <signal.h>
#include <glob.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/times.h>
#include <assert.h>
#include <fcntl.h>
#include <stdbool.h>



#include "batch.h"
#include "cJSON.h"

#define URL_MAX_HOST_LEN    128
#define URL_MAX_PATH_LEN    128
#define PAIR_KEY_LEN        128
#define PAIR_VALUE_LEN      128
#define REQUEST_BODY_LEN    12400
#define RESPONSE_MAX_DESC_SIZE 1024
#define RESPONSE_BODY_LEN   4096
#define HTTP_HEAD_PAIR_NUM  10
#define HTTP_PRE_RECV_LENGTH 1024*10
#define HTTP_REQUEST_POST  1
#define PROTOCOL_HTTP      1
#define LOG_ALLOC_GRAN			(1024)

/*
int m_nRspAllocLen = 0;
int m_nRspDataLen = 0;
char *m_pRspData = NULL;
*/
/*
  The URL object. A representation of an URL like: [protocol]://[host]:[port]/[context]
参数serverip：影像服务器IP（特别注意，此参数需要可配置）
参数serverport: 影像服务器端口（特别注意，此参数需要可配置）
参数syscode:系统ID，如007 
参数mkey:加密key，由影像平台给出，定期更新
参数remotefilepath：影像远程路径，

*/
typedef struct _HTTP_REQ_HEAD
{
    char   HttpServAddr[128];
    char   HttpServPort[10+1];
    char   HttpSyscode[3+1];
    char   HttpMHttpSyscodekey[20+1];
    char   HttpRemotefilepath[128];
    char   HttpContentType[100+1];   
} HttpReqHead;

/*键值对*/
typedef struct _KEY_VALUE_PAIR
{
    char       Key[PAIR_KEY_LEN];
    char       Value[PAIR_VALUE_LEN];
} KeyValuePair;

typedef struct _HTTP_REQUEST
{
    HttpReqHead     stReqHead;
    char            sHttpPost[128];
    char            sReqBody[REQUEST_BODY_LEN];
    char            *pReqBody;
} HttpRequest;

typedef struct _HTTP_RSP_HEAD
{
    int             iHttpStatus;
    char            sHttpStatusDesc[1024];
    KeyValuePair    stRspHeadPairs[10];
    int             iRspHeadPairNum;
} HttpRspHead;

typedef struct _HTTP_RESPONSE
{
    HttpRspHead     stRspHead;
    char            sRspBody[1024*20*200];
    char            *pRspBody;
    int             nDataLen;        
} HttpResponse;




//支付接口

typedef struct
{
	char sTimeStamp[14+1];
	char sAppid[8+1];
	char sService[100+1];
	char sVer[2+1];
	char sSignType[4+1];
	char sKey[60+1];
	char sMrchCert[100+1];
} stEpayHeadDef;


typedef struct
{
	char sErrCode[14+1];
	char sErrMsg[255+1];
	char sMac[400+1];
} stEpayErrDef;

//3.2	单笔代付查询
//应答
typedef struct
{
	stEpayHeadDef stEpayHead;
	char sOrder_no[32+1];
	char sMac[400+1];
} stEpay_PaymentGet_Request;

//响应
typedef struct
{
    stEpayErrDef stEpayErr;
	char sNo[16+1];
	char sMrchNo[8+1];
	char sSubMrchName[60+1];
	char sOrderNo[32+1];
	char sTransDate[8+1];
	char sTransTime[6+1];
	char sTransStatus[1+1];
	double dTransAmt;
	double dTransFee;
	char sToAcctNo[32+1];
	char sToAcctName[60+1];
	char sTransUsage[60+1];
	char sRemark[255+1];	
	char sMac[400+1];
} stEpay_PaymentGet_Response;

//3.4	批量资金付款
//应答
typedef struct
{
	stEpayHeadDef stEpayHead;
	char sOrderDate[8+1];
	char sFileName[50+1];
	char *pFileContent;
	char sMac[400+1];
} stEpay_BatchPayApi_Request;

//响应
typedef struct
{
    stEpayErrDef stEpayErr;
	char sBatId[16+1];
	char sBatStatus[1+1];
	char sBatStatusLabel[10+1];
	char sMac[400+1];
} stEpay_BatchPayApi_Response;


//3.5	批量代付查询
typedef struct
{
	stEpayHeadDef stEpayHead;
	char sFileName[50+1];
	char sOrderDate[8+1];
	char sMac[400+1];
} stEpay_BatchPayQuery_Request;

typedef struct
{
    stEpayErrDef stEpayErr;
	char sBatId[16+1];
	char sOrderDate[8+1];
	char sBatStatus[1+1];
	char sBatStatusLabel[10+1];
	char sFileName[50+1];
	char sFileContent[REQUEST_BODY_LEN+1];
	char sMac[400+1];
} stEpay_BatchPayQuery_Response;

//6.1	商户结算账户余额查询
typedef struct
{
	stEpayHeadDef stEpayHead;
	char sMac[400+1];
} stEpay_QueryBalance_Request;


typedef struct
{
    stEpayErrDef stEpayErr;
	char sMrchNo[8+1];
	char sAcctNo[32+1];
	double dAccountBalance;
	double dAvailableBalanc;
	double dBlockedBalance;
	double dControlledBalance;
	char sSignType[4+1];
	char sMac[400+1];
} stEpay_QueryBalance_Response;


//4.1	批量代付资金对账文件下载
typedef struct
{
	stEpayHeadDef stEpayHead;
	char sTransDate[8+1];
	char sTcptType[1+1];
	char sFileName[50+1];
	char sMac[400+1];
} stEpay_ReceiptFile_Request;

typedef struct
{
    stEpayErrDef stEpayErr;
} stEpay_ReceiptFile_Response;

//7.1	快捷支付资金对账文件下载
typedef struct
{
	stEpayHeadDef stEpayHead;
	char sTransDate[8+1];
	char sTcptType[1+1];
	char sFileName[50+1];
	char sMac[400+1];
} stEpay_SettleFile_Request;

typedef struct
{
    stEpayErrDef stEpayErr;
} stEpay_SettleFile_Response;


typedef struct
{
	char sService[32+1];
	char sVersion[8+1];
	char sCharset[8+1];
	char sBillDate[8+1];
	char sBillType[8+1];
	char sSignType[8+1];
	char sMchId[32+1];
	char sKey[60+1];
	char sNonceStr[32+1];
	char sSign[32+1];
	char sFileName[200+1];
} stEpay_Merchant_Request;

typedef struct
{
    stEpayErrDef stEpayErr;
	char sFileContent[1024*20*200];    
} stEpay_Merchant_Response;


// 富友单笔代付请求
typedef struct
{
	char ver[2];
	char reqtype[16];
	char merid[32];
	char merdt[27];
	char orderno[32+12];
	char bankno[24];
	char cityno[24];
	char branchnm[256];
	char accntno[128];
	char accntnm[128];
	int amt;
	char xmlMsg[2048];
} Fy_Single_Request;

//富友单笔代付响应
typedef struct
{
    char ret[7];
	char xmlMsg[2048];
} Fy_Single_Response;

char * base64_encode( const unsigned char * bindata, char * base64, int binlength );
int base64_decode( const char * base64, unsigned char * bindata );

//3.2	单笔代付查询
int PaymentGet(stEpay_PaymentGet_Request *pstEPayReq,stEpay_PaymentGet_Response *pstEPayRsp);
//批量资金付款
int BatchPayApi(stEpay_BatchPayApi_Request *pstEPayReq,stEpay_BatchPayApi_Response *pstEPayRsp);
//批量代付查询
int BatchPayQuery(stEpay_BatchPayQuery_Request *pstEPayReq,stEpay_BatchPayQuery_Response *pstEPayRsp);
//商户结算账户余额查询
int QueryBalance(stEpay_QueryBalance_Request *pstEPayReq,stEpay_QueryBalance_Response *pstEPayRsp);
//批量代付资金对账文件下载
int ReceiptFile(stEpay_ReceiptFile_Request *pstEPayReq,stEpay_ReceiptFile_Response *pstEPayRsp);

//快捷支付资金对账文件下载
int settleFile(stEpay_SettleFile_Request *pstEPayReq,stEpay_SettleFile_Response *pstEPayRsp);

//威富通商户文件下载
int Merchant(stEpay_Merchant_Request *pstEPayReq,stEpay_Merchant_Response *pstEPayRsp);

// 富友单笔代付
int FySinglePayApi(Fy_Single_Request* FyRequest, Fy_Single_Response* FyResponse);

// 富友单笔代付查询
int FySingleQueryApi(Fy_Single_Request* FyRequest, Fy_Single_Response* FyResponse);

// 富友虚拟账户头寸查询
int FyBalanceQueryApi(Fy_Single_Request* FyRequest, Fy_Single_Response* FyResponse);
#endif
