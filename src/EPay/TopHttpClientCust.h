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
 * �ͻ�����������������󣬲�������Ӧ����
 **************************************************************/
int HttpCall(char *sSoapCfgName, HttpRequest *pstHttpReq, HttpResponse *pstHttpRsp);


/**************************************************************
 * ����HTTP������
 **************************************************************/
int HttpServerStart(char *sSoapCfgName);

/**************************************************************
 * �ر�HTTP������
 **************************************************************/
void HttpServerStop();

/**************************************************************
 * ����һ��HTTP�ͻ��ˣ�����sockfd
 **************************************************************/
int HttpServerAccept();

/**************************************************************
 * �ӿͻ��˽���������
 **************************************************************/
int HttpServerRecvClient(HttpRequest *pstHttpClientReq);

/**************************************************************
 * ��ͻ��˷�����Ӧ����
 **************************************************************/
int HttpServerSendClient(HttpResponse *pstHttpClientRsp);

/*Http Trace ׷��*/
void HttpRequestTrace(HttpRequest *pstHttpReq);
void HttpResponseTrace(HttpResponse *pstHttpRsp);

/*Socket TCP Common*/
int SocketSendMesg(int socketfd, char *pMesgBuf, int iMesgLen);
int SocketRecvMesg(int socketfd, char *pMesgBuf, int iMesgLen);

/**************************************************************
 * �ͻ��˳�ʼ��
 **************************************************************/
int SocketClientInit(char *sSrvIp, int srvPort);

/**************************************************************
 * �ͻ������ӷ�����
 **************************************************************/
int SocketClientConnect();

/**************************************************************
 * �ͻ��˶Ͽ��������������
 **************************************************************/
void SocketClientDisConnect();

/**************************************************************
 * �ͻ��������������������
 **************************************************************/
int SocketClientSendMesg(char *pReqMesgBuf, int iMesgLen);

/**************************************************************
 * �ͻ��˴ӷ�����������Ӧ����
 **************************************************************/
int SocketClientRecvMesg(char *pRspMesgBuf, int iMesgLen);


/**************************************************************
 * ����������
 **************************************************************/
int SocketServerStart(int srvPort, int srvListenNum);

/**************************************************************
 * �رշ�����
 **************************************************************/
void SocketServerStop();


/**************************************************************
 * �Ͽ���ͻ��˵�����
 **************************************************************/
void SocketServerCloseClient();

/**************************************************************
 * ����һ���ͻ��ˣ�����sockfd
 **************************************************************/
int SocketServerAccept();

/**************************************************************
 * �ӿͻ��˽���������
 **************************************************************/
int SocketServerResvMesg(char *pReqMesgBuf, int iMesgLen);

/**************************************************************
 * ��ͻ��˷�����Ӧ����
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