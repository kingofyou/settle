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
 *  文 件 名: Task_0101.c
 *  功    能: 确认网关流水文件
 *  编程人员: HELU
 *  开发时间: 2016-12-13
 *  备    注:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include "batch.h"

extern char	gLogFile[LOG_NAME_LEN_MAX];
extern  tbl_date_inf_def dbtbl_date_inf;
extern	char ext_inter_brh_code[10 + 1];

void *GetFilename(char *p, char *out) 
{ 
    int x = strlen(p); 
    char sTmp[1024] = {0};
    strcpy(sTmp, p);
    char ch = '/'; 
    char *q = strrchr(sTmp,ch) + 1; 
    strcpy(out, q);
    return ; 
}

/*************************************************
*函数名称∶ Total_0101
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_0101()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_0101
*函数功能∶
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_0101 ( int nBeginOffset, int nEndOffset )
{
    int		iRet = 0;
    char 	sFlag[1 + 1] = {0};
    char    *pstr;    
    char	onlinepay_ok_name[256] = {0};
    char	cmd[2048] = {0};
    char	sOut[256] = {0};    
    char	onlinepay_file_name[200] = {0};
    char	share_file_name[200] = {0};
    char	ok_file_name[200] = {0};
    char    sCommand[256] = {0};
	char    sCurMisnHr[2 + 1];
	char    sms_content[512] = {0};
    glob_t globbuf;
    char    sTmp[2+1];
    int     nTotCount=0;
    int     i = 0;    
    struct stat stat_buf_share;
    struct stat stat_buf_online;
    stExtnFileRegDef StExtnFileReg;

    /* 初始共享目录文件信息 */
    memset(ok_file_name, 0, sizeof(ok_file_name));

    /* 初始ok文件信息 */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    memcpy(StExtnFileReg.host_name, "ONLINEPAY", 9);
    strcpy(StExtnFileReg.src_file_name, "ONLINEPAY_YYYYMMDD_??.ok");

    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);


    iRet = getShareFileFullName(&StExtnFileReg, ok_file_name);
    if (iRet != 0)
    {
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "getShareFileFullName: error src_file_name[%s]...", StExtnFileReg.src_file_name);
        return -1;
    }
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "正在等待网关对账ok文件:[%s]...", ok_file_name);
    
    
    sprintf(onlinepay_ok_name, "%s/%8.8s/%s/%s/",
            getenv("SHARE_FILE_PATH"),
            StExtnFileReg.stlm_dt,
            StExtnFileReg.inter_brh_code,
            StExtnFileReg.host_name);

    iRet = CheckDir(onlinepay_ok_name);
    if (iRet)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "CheckDir err[%d][%s][%s]",
               iRet, strerror(errno), onlinepay_ok_name);
        return -1;
    }    
    
    while (1)
    {
        iRet = glob (ok_file_name, GLOB_NOSORT, NULL, &globbuf);
        if(iRet && iRet != GLOB_NOMATCH)
        {
           HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "glob error [%d %s]", iRet, ok_file_name);
           globfree (&globbuf);
           return -1;
        }
        else if (iRet == GLOB_NOMATCH)
        {
			// 判断是否超时未获取到
			/* 判断时间是否达到定时的要求 */
			// 4点后还未下载到发短信报警
            memset(sCurMisnHr, 0, 2 + 1);
            int nReturnCode = DbsTimeStamp(DBS_SELECT1, NULL, NULL, sCurMisnHr, NULL);
            if(nReturnCode)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "get time error [%d].", nReturnCode);               
                return -1;
            }
			if(memcmp("04", sCurMisnHr, 2) >0) {
                sleep(2);
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "本地Ok文件:[%s]...", onlinepay_ok_name);
                continue;
			}
			else {
                // 发短信
				sprintf(sms_content, "超时未获取到网关对账文件[%s]", onlinepay_ok_name);
                nReturnCode = sendMobileMsg(sms_content);
				if(nReturnCode) {
                    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "报警短信[%s]发送失败", sms_content);
					sleep(2);
					continue;
				}
				sleep(2);
			}              
        }
        else if(globbuf.gl_pathc != 1)
        {
			// 判断是否超时未获取到
			/* 判断时间是否达到定时的要求 */
			// 4点后还未下载到发短信报警
            memset(sCurMisnHr, 0, 2 + 1);
            int nReturnCode = DbsTimeStamp(DBS_SELECT1, NULL, NULL, sCurMisnHr, NULL);
            if(nReturnCode)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "get time error [%d].", nReturnCode);               
                return -1;
            }
			if(memcmp("04", sCurMisnHr, 2) <= 0) {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "file num err [%d] [%d]", iRet, globbuf.gl_pathc);
                globfree (&globbuf);
                return -1;
			}
			else {
                // 发短信
				memset(sms_content, 0, sizeof(sms_content));
				sprintf(sms_content, "超时未获取到网关对账文件[%s]", onlinepay_ok_name);
                nReturnCode = sendMobileMsg(sms_content);
				if(nReturnCode) {
                    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "报警短信[%s]发送失败", sms_content);
					sleep(2);
					continue;
				}
				sleep(2);
			}              
           
        }
        else
        {
             HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "file name [%s]", globbuf.gl_pathv[0]);
             memset(ok_file_name, 0, sizeof(ok_file_name));
             strcpy(ok_file_name, globbuf.gl_pathv[0]);
             
             memset(sOut, 0, sizeof(sOut));
             GetFilename(ok_file_name, sOut);
             strcat(onlinepay_ok_name, sOut);             
             break;    
        }
    }
    globfree (&globbuf);
        
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "本地OK文件:[%s]...", onlinepay_ok_name);
    while (1)
    {
        /* 确认文件是否存在 */
        iRet = access(onlinepay_ok_name, R_OK);
        if (iRet != 0)
        {
            sleep(2);
            continue;
        }
        else
        {
            break;
        }
    }
        
    memset(sTmp,0x00,sizeof(sTmp));
    memcpy(sTmp,sOut+19,2);
    nTotCount = atoi(sTmp);    
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "对账文件个数:[%d][%s]...", nTotCount,sOut);
    
    memset(cmd,0x00,sizeof(cmd));
    sprintf(cmd, "cat ");
    for(i=0; i<nTotCount; i++)
    {
        memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
        memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
        memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
        memcpy(StExtnFileReg.host_name, "ONLINEPAY", 9);
        memcpy(StExtnFileReg.src_file_name, "ONLINEPAY_YYYYMMDD_XX.txt", 25);
    
        pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
        if (pstr)
            memcpy (pstr, dbtbl_date_inf.stoday, 8);
        pstr = strstr (StExtnFileReg.src_file_name, "XX");
        if (pstr)
        {
            memset(sTmp,0x00,sizeof(sTmp));
            sprintf(sTmp,"%02d",i+1);
            memcpy (pstr, sTmp, 2);
            
        }    
        getShareFileFullName(&StExtnFileReg, share_file_name);
    
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "正在等待网关对账文件:[%s]...", share_file_name);
           
        /* 初始本地文件信息 */
        memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
        memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
        memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
        memcpy(StExtnFileReg.host_name, "ONLINEPAY", 9);
        memcpy(StExtnFileReg.src_file_name, "ONLINEPAY_YYYYMMDD_XX.txt", 25);
    
        pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
        if (pstr)
            memcpy (pstr, dbtbl_date_inf.stoday, 8);
        pstr = strstr (StExtnFileReg.src_file_name, "XX");
        if (pstr)
        {
            memset(sTmp,0x00,sizeof(sTmp));
            sprintf(sTmp,"%02d",i+1);
            memcpy (pstr, sTmp, 2);            
        } 
    
        getExtnFileFullName(&StExtnFileReg, onlinepay_file_name);
        
        strcat(cmd, " ");
        strcat(cmd, onlinepay_file_name);
        
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "本地路径:[%s]...", onlinepay_file_name);
                
        while (1)
        {
            memset(sCommand, 0, sizeof(sCommand));
            sprintf(sCommand, "cp -f %s %s", share_file_name, onlinepay_file_name);
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sCommand:[%s]...", sCommand);
            iRet = cmdSystem(sCommand);
            if (iRet != 0)
            {
                sleep(2);
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sCommand:[%s]...", sCommand);
                continue;
            }
            
            /* 确认文件是否存在 */
            iRet = access(onlinepay_file_name, R_OK);
            if (iRet != 0)
            {
                HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sleep");
                sleep(2);
                continue;
            }
            
            /* 如果目标文件大小为0，且源目标文件大小不为0，重新复制 */
            memset(&stat_buf_share, 0x00, sizeof(stat_buf_share));
            if(stat(share_file_name, &stat_buf_share) == -1)
            {
                HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sleep");
                sleep(2);
                continue;
            }
    
            memset(&stat_buf_online, 0x00, sizeof(stat_buf_online));
            if(stat(onlinepay_file_name, &stat_buf_online) == -1)
            {
                HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sleep");
                sleep(2);
                continue;
            }
            
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, " fileSize[%d][%d]", stat_buf_share.st_size, stat_buf_online.st_size);
            if(stat_buf_share.st_size != 0 && stat_buf_online.st_size == 0)
            {
                HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, " fileSize[%d][%d]", stat_buf_share.st_size, stat_buf_online.st_size);
                sleep(2);
                continue;
            }
        
            break;
        }
    }

    /* 初始本地文件信息 */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    memcpy(StExtnFileReg.host_name, "ONLINEPAY", 9);
    memcpy(StExtnFileReg.src_file_name, "ONLINEPAY_YYYYMMDD_XX.txt", 25);
    
    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);
    pstr = strstr (StExtnFileReg.src_file_name, "XX");
    if (pstr)
        memcpy (pstr, "99", 2);            
    getExtnFileFullName(&StExtnFileReg, onlinepay_file_name);
    strcat(cmd, ">");
    strcat(cmd, onlinepay_file_name);
    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "cmd[%s].",cmd);
    iRet = cmdSystem(cmd);
    if (iRet != 0)
    {
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "cmd:[%s]...", cmd);
        return -1;
    }
    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "网关对账文件[%d]已到达.",i);
    return 0;
}

/****************************** end of Task_0104.c ******************************/

