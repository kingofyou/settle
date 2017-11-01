#ifndef __HTTP_CUST_H
#define __HTTP_CUST_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <errno.h>
#include <time.h>
#include <sys/times.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netdb.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/shm.h>
#include <sys/stat.h>

//#include "SrvDef.h"
//#include "TxnNum.h"
//#include "IpcInt.h"
//#include "DbsDef.h"
//#include "DbsTbl.h"
//#include "POSdb.h"
//#include "db.h"
//#include "pos_stlm.h"
//#include "TxnLogProc.h"
#include "HtLog.h"
#include "Common.h"
#include "EPay.h"
//#include "SrvParam.h"
//#include "ErrCode.h"
//#include "EncOpr.h"
//#include "CstDbsTbl.h"
//#include "Cust.h"
//#include "db/SETTLE_DOC_INFO.h"
//#include "../../include/db.h"
//#include "memdberr.h"
//#include "trace.h"

char	gsSwtCustLogFile[LOG_NAME_LEN_MAX] = "TopHttpClinetCust.log";


/**************************************************************
 * 客户端向服务器发送请求，并接受响应数据
 **************************************************************/
int HttpCall(char *sSoapCfgName, HttpRequest *pstHttpReq, HttpResponse *pstHttpRsp);


/**************************************************************
 * 启动HTTP服务器
 **************************************************************/
int HttpServerStart(char *sSoapCfgName);

/**************************************************************
 * 关闭HTTP服务器
 **************************************************************/
void HttpServerStop();

/**************************************************************
 * 接受一个HTTP客户端，返回sockfd
 **************************************************************/
int HttpServerAccept();

/**************************************************************
 * 从客户端接受请求报文
 **************************************************************/
int HttpServerRecvClient(HttpRequest *pstHttpClientReq);

/**************************************************************
 * 向客户端发送响应报文
 **************************************************************/
int HttpServerSendClient(HttpResponse *pstHttpClientRsp);

/*Http Trace 追踪*/
void HttpRequestTrace(HttpRequest *pstHttpReq);
void HttpResponseTrace(HttpResponse *pstHttpRsp);

/*Socket TCP Common*/
int SocketSendMesg(int socketfd, char *pMesgBuf, int iMesgLen);
int SocketRecvMesg(int socketfd, char *pMesgBuf, int iMesgLen);

/**************************************************************
 * 客户端初始化
 **************************************************************/
int SocketClientInit(char *sSrvIp, int srvPort);

/**************************************************************
 * 客户端连接服务器
 **************************************************************/
int SocketClientConnect();

/**************************************************************
 * 客户端断开与服务器的连接
 **************************************************************/
void SocketClientDisConnect();

/**************************************************************
 * 客户端向服务器发送请求报文
 **************************************************************/
int SocketClientSendMesg(char *pReqMesgBuf, int iMesgLen);

/**************************************************************
 * 客户端从服务器接受响应报文
 **************************************************************/
int SocketClientRecvMesg(char *pRspMesgBuf, int iMesgLen);


/**************************************************************
 * 启动服务器
 **************************************************************/
int SocketServerStart(int srvPort, int srvListenNum);

/**************************************************************
 * 关闭服务器
 **************************************************************/
void SocketServerStop();


/**************************************************************
 * 断开与客户端的连接
 **************************************************************/
void SocketServerCloseClient();

/**************************************************************
 * 接受一个客户端，返回sockfd
 **************************************************************/
int SocketServerAccept();

/**************************************************************
 * 从客户端接受请求报文
 **************************************************************/
int SocketServerResvMesg(char *pReqMesgBuf, int iMesgLen);

/**************************************************************
 * 向客户端发送响应报文
 **************************************************************/
int SocketServerSendMesg(char *pRspMesgBuf, int iMesgLen);

#define TOPSOAP_CFG_PATH "TOPSOAP_CFG_PATH"

int OpenCfgFile(char *sFileName);
int CloseCfgFile();
int GetHttpCfgItem(char *sItemName, char *sItemValue);
int GetSoapRootCfgItem(char *sItemName, char *sItemValue);
int GetSoapEnvelopCfgItem(char *sItemName, char *sItemValue);
int GetSoapHeaderCfgItem(char *sItemName, char *sItemValue);
int GetSoapBodyCfgItem(char *sItemName, char *sItemValue);
int GetXmlCfgItem(char *sItemName, char *sItemValue);

#endif