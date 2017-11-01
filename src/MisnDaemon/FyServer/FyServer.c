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
#include "EPay.h"

char ext_inter_brh_code[10 + 1];
int ext_inter_brh_sta = 0;
char gLogFile[LOG_NAME_LEN_MAX];
void vCTerminate();

 
int main( int argc, char **argv)
{
    int rc;
    int i;
    int nReturnCode;
    int nTotalNum = 0;
    int nIndex;
    int nMisnCnt;
    struct tm *tTmLocal;

	int fpid = 0;
    memset(gLogFile, 0, LOG_NAME_LEN_MAX);
    HtSprintf(gLogFile, "FyServer.log");
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "FyServer started.");

    sigset(SIGHUP, SIG_IGN);
    if (signal(SIGTERM, vCTerminate) == SIG_ERR)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "sigset SIGTERM error, %d", errno);
        exit(-10);
    } /* end of if */

    while (1)
    {
        /* connect to database */
        nReturnCode = DbsConnect();
        if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsConnect error, %d.", nReturnCode);
            sleep(60);
            continue;
        }
		break;
    }

    fpid = fork();   
	if(!fpid) {
        // 查询代付状态
	    while(1) {
			HtSprintf(gLogFile, "queryPayRst.log");
			HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "queryPayRst started.");
			queryPayRst();
			DbsDisconnect();
			continue;
		}
	}

	fpid = fork();   
	if(!fpid) {
        // 接收富友退票通知
	    while(1) {
			HtSprintf(gLogFile, "recvFyNotice.log");
			HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "recvFyNotice started.");
			recvFyNotice();
			DbsDisconnect();
		    continue;
		}
	}
       
    DbsDisconnect();
} /* end of main */


void vCTerminate()
{
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "vCTerminate");
    DbsDisconnect();
    exit(0);
}/* end of vCTerminate */

int queryPayRst() {
    char sCurMisnDt[8 + 1];
	char sett_date[8 + 1]; 
	int nReturnCode;
	tbl_sett_list_def  dbtbl_sett_list_dc;
	stKeyInf KeyInf;
	while (1)
    {
        /* connect to database  */
        nReturnCode = DbsConnect();
        if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsConnect error, %d.", nReturnCode);
            DbsDisconnect();
            sleep(60);
            continue;
        }

	    nReturnCode = DbsTimeStamp(DBS_SELECT2, sCurMisnDt, NULL, NULL, NULL);
        if(nReturnCode)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "get time error [%d].", nReturnCode);
            DbsDisconnect();
            sleep(60);
            continue;
        }

		memset(&KeyInf, 0x00, sizeof(KeyInf));
//        nReturnCode = GetKeyInf(&KeyInf, "fuiou");
//        if (nReturnCode)
//        {
//            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "GetKeyInf err.");
//            return -1;
//        } 

        // 查询前一天状态在处理中的代付数据
		memset(&dbtbl_sett_list_dc, 0x00, sizeof(dbtbl_sett_list_dc));
		CalNewDate(sCurMisnDt, -2, dbtbl_sett_list_dc.sett_date);
        memcpy(dbtbl_sett_list_dc.status, "7", 1);
        memcpy(dbtbl_sett_list_dc.dc_flag, "C", 1);
		nReturnCode = DbsTblSettList(DBS_CURSOR1, &dbtbl_sett_list_dc);        
        nReturnCode = DbsTblSettList(DBS_OPEN1, &dbtbl_sett_list_dc);
        if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN1 error, %d.", nReturnCode);
            return -1;
        }               
        while(1)
        {
            memset(&dbtbl_sett_list_dc, 0x00, sizeof(dbtbl_sett_list_dc));
            nReturnCode = DbsTblSettList(DBS_FETCH1, &dbtbl_sett_list_dc);
            if (nReturnCode == DBS_NOTFOUND)
            {
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DbsTblSettList DBS_FETCH1 end.");
                break;
            }
            else if (nReturnCode)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblSettList DBS_FETCH1 error [%d].", nReturnCode);
                DbsTblSettList(DBS_CLOSE1, &dbtbl_sett_list_dc);
                return -1;
            }
			// 富友单笔代付请求
		    nReturnCode = iSingleQeryApi(&dbtbl_sett_list_dc, KeyInf.sMrchNo);
			if(nReturnCode) break;
			else sleep(180);
		}
		DbsTblSettList(DBS_CLOSE1, &dbtbl_sett_list_dc);
		DbsDisconnect();
		sleep(1000);
	}
}

char* rtrim(char* rptr)
{
    char* ptrTmp = rptr + strlen(rptr) - 1;
    while(*ptrTmp == ' ') ptrTmp--;
    *++ptrTmp = '\0';
    return rptr;
}

int iSingleQeryApi(tbl_sett_list_def* dbtbl_sett_list, char* mcht_no)
{
    int nReturnCode;   
    char    Filename[100];
    char    stlm_dt[8+1];

	char orderno[32+12];
	char startdt[24];
	char enddt[24];

	char ret[7] = {};
	char state[2] = {};
	char *sEnd;

	memset(orderno, 0, sizeof(orderno));
	memset(startdt, 0, sizeof(startdt));
	memset(enddt, 0, sizeof(enddt));

    Fy_Single_Request FyRequest;
    Fy_Single_Response FyResponse; 
    
   //批量资金付款
    memset(&FyRequest,0x0, sizeof(FyRequest));
    memset(&FyResponse,0x0, sizeof(FyResponse));

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    CalNewDate(dbtbl_sett_list->trans_date, 1, stlm_dt);

    memcpy(FyRequest.reqtype, "qrytransreq", sizeof(FyRequest.reqtype)-1);
	memcpy(FyRequest.merid, "0002900F0345178", sizeof(FyRequest.merid)-1);
	rtrim(dbtbl_sett_list->seq_no);

	sprintf(orderno, "<orderno>%s</orderno>", "2505980679322");//rtrim(dbtbl_sett_list->seq_no));
	sprintf(startdt, "<startdt>%s</startdt>", "20170922");//rtrim(stlm_dt));
	sprintf(enddt, "<enddt>%s</enddt>", "20170922");//rtrim(stlm_dt));
	
	strcat(FyRequest.xmlMsg, "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>");
    strcat(FyRequest.xmlMsg, "<qrytransreq>");
	strcat(FyRequest.xmlMsg, "<ver>1.00</ver>");
	strcat(FyRequest.xmlMsg, "<busicd>AP01</busicd>");
    strcat(FyRequest.xmlMsg, orderno);
	strcat(FyRequest.xmlMsg, startdt);
	strcat(FyRequest.xmlMsg, enddt);
	strcat(FyRequest.xmlMsg, "</qrytransreq>");
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "FyRequest.xmlMsg:[%s]", FyRequest.xmlMsg);

    nReturnCode = FySingleQueryApi(&FyRequest,&FyResponse);
    if(nReturnCode)
    {
        // 查询失败
		HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "FySingleQueryApi error [%d].", nReturnCode);
        return nReturnCode;
		dbtbl_sett_list->amt_fee=0;
		memcpy(dbtbl_sett_list->status, "5", 1);
		nReturnCode = DbsTblSettList(DBS_UPDATE2, dbtbl_sett_list);
		if (nReturnCode)
		{
		    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPD_SEQ ht_tbl_sett_list error [%d].", nReturnCode);
		    return -1;
		}
        return nReturnCode;
    }
    else
    {
		HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "FyResponse.xmlMsg:[%s]", FyResponse.xmlMsg);
		sEnd = strstr(FyResponse.xmlMsg, "<ret>");
	    memcpy(ret, sEnd+5, 6);
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "ret:[%s]", ret);

        if(!memcmp("000000", ret, 6)) {
            sEnd = strstr(FyResponse.xmlMsg, "<state>");
	        memcpy(state, sEnd+7, 1);
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "state:[%s]", state);
			if(!memcmp("1", state, 1)) {             
		        dbtbl_sett_list->amt_fee=2;
		        memcpy(dbtbl_sett_list->status, "4", 1);
		        nReturnCode = DbsTblSettList(DBS_UPDATE2, dbtbl_sett_list);
		        if (nReturnCode)
		        {
		            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPD_SEQ ht_tbl_sett_list error [%d].", nReturnCode);
		            return -1;
		        }
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " FySinglePayApi succ [%d]",nReturnCode);   
		    }
		    else if(!memcmp("2", state, 1)) {      
                dbtbl_sett_list->amt_fee=0;
		        memcpy(dbtbl_sett_list->status, "5", 1);
		        nReturnCode = DbsTblSettList(DBS_UPDATE2, dbtbl_sett_list);
		        if (nReturnCode)
		        {
		            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPD_SEQ ht_tbl_sett_list error [%d].", nReturnCode);
		            return -1;
		        }
		    }
		}
    }
    return 0;    
}

int socket_bind(int host_port)
{
	HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "服务器正在绑定通信地址...");
	// 准备通信地址
	int sockfd=0;
    struct sockaddr_in serveraddr = {};
	memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(host_port);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);	
    // 创建socket
	while(1) {
	    sockfd = socket(AF_INET,SOCK_STREAM,0);
	    if(-1 == sockfd)
	    {
			HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "socket server error!");
			sleep(1);
			continue;
	    }
	    // 设置套接字选项避免地址使用错误   
        int on=1;  
        if((setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)  
        {   
			HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "setsockopt failed!");
            exit(EXIT_FAILURE);  
        } 
    
	    // 进行socket和地址的绑定	
	    int res = bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	    if(-1 == res)
	    {
			HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "bind server error!");
			close(sockfd);
			sleep(1);
			continue;
	    }
		HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "服务器地址绑定成功。");
		break;
	}
	return sockfd;
}

//设置socket为非阻塞的  
static int  make_socket_non_blocking (int sfd)  
{  
    int flags, s;  
  
    //得到文件状态标志  
    flags = fcntl (sfd, F_GETFL, 0);  
    if (flags == -1)  
    {  
        perror ("fcntl");  
        return -1;  
    }  
  
    //设置文件状态标志  
    flags |= O_NONBLOCK;  
    s = fcntl (sfd, F_SETFL, flags);  
    if (s == -1)  
    {  
        perror ("fcntl");  
        return -1;  
    }  
  
    return 0;  
} 

int dealNotice(int clientfd) {
	char orderno[32] = {};
	char *start = NULL;
	char *sEnd = NULL;
	int nReturnCode;
	tbl_sett_list_def  dbtbl_sett_list;
	memset(&dbtbl_sett_list, 0, sizeof(dbtbl_sett_list));

    char recv_buf[2048] = {};
    int recv_len = recv(clientfd,recv_buf,2048,0);
    if (recv_len < 0)
    {
		HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "recv error!");
        close(clientfd);
    }
    else if (recv_len == 0)
    {
		HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "client close.");
        close(clientfd);
    }
    else
    {
		HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "recvMsg:[%s]", recv_buf);
		send(clientfd, "1", 1, 0);
		start = strstr(recv_buf, "<orderno>");
		sEnd = strstr(recv_buf, "</orderno>");
	    memcpy(orderno, start+9, sEnd-start-9);
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "orderno:[%s]", orderno);
        dbtbl_sett_list.amt_fee=0;
		memcpy(dbtbl_sett_list.status, "5", 1);
		strcpy(dbtbl_sett_list.seq_no, orderno);
		nReturnCode = DbsTblSettList(DBS_UPDATE2, &dbtbl_sett_list);
		if (nReturnCode && nReturnCode != DBS_NOTFOUND)
		{
		    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPD_SEQ ht_tbl_sett_list error [%d].", nReturnCode);
		    return -1;
		}
	}

}
int recvFyNotice() {
	int sockfd = 0;
	int clientfd = 0;
	int iret = 0;

	while (1)
    {
        /* connect to database  */
        iret = DbsConnect();
        if (iret)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsConnect error, %d.", iret);
            DbsDisconnect();
            sleep(60);
            continue;
        }
		break;
	}

    sockfd = socket_bind(8007);
	listen(sockfd,10);
	struct sockaddr_in clientaddr;
	memset(&clientaddr, 0, sizeof(clientaddr));
    socklen_t  clientaddrlen = sizeof(struct sockaddr_in);
	while(1) {
        clientfd = accept(sockfd,(struct sockaddr*)&clientaddr,&clientaddrlen);
        if (clientfd == -1) {
			HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "accpet error!");
			break;
		}
        else
        {
			//HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "accept a new clientfd: %s:%d.", inet_ntoa(clientaddr.sin_addr),clientaddr.sin_port);
//			iret = make_socket_non_blocking (clientfd);  
//			if(-1 == iret) {
//				HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "设置[%d]非阻塞失败!", clientfd);
//				exit(0);
//			}
			dealNotice(clientfd);
		}
	}
	DbsDisconnect();
    return 0;
}