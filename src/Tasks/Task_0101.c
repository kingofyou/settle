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
 *  �� �� ��: Task_0101.c
 *  ��    ��: ȷ��������ˮ�ļ�
 *  �����Ա: HELU
 *  ����ʱ��: 2016-12-13
 *  ��    ע:
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
*�������ơ� Total_0101
*�������ܡ�
*���������
*���������
*�������ء� 1
***************************************************/
int Total_0101()
{
    return 1;
}

/*************************************************
*�������ơ� Task_0101
*�������ܡ�
*��������� nBeginOffset��nEndOffset
*���������
*�������ء� 0  -- �ɹ�
            -1 -- ʧ��
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

    /* ��ʼ����Ŀ¼�ļ���Ϣ */
    memset(ok_file_name, 0, sizeof(ok_file_name));

    /* ��ʼok�ļ���Ϣ */
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
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "���ڵȴ����ض���ok�ļ�:[%s]...", ok_file_name);
    
    
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
			// �ж��Ƿ�ʱδ��ȡ��
			/* �ж�ʱ���Ƿ�ﵽ��ʱ��Ҫ�� */
			// 4���δ���ص������ű���
            memset(sCurMisnHr, 0, 2 + 1);
            int nReturnCode = DbsTimeStamp(DBS_SELECT1, NULL, NULL, sCurMisnHr, NULL);
            if(nReturnCode)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "get time error [%d].", nReturnCode);               
                return -1;
            }
			if(memcmp("04", sCurMisnHr, 2) >0) {
                sleep(2);
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "����Ok�ļ�:[%s]...", onlinepay_ok_name);
                continue;
			}
			else {
                // ������
				sprintf(sms_content, "��ʱδ��ȡ�����ض����ļ�[%s]", onlinepay_ok_name);
                nReturnCode = sendMobileMsg(sms_content);
				if(nReturnCode) {
                    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "��������[%s]����ʧ��", sms_content);
					sleep(2);
					continue;
				}
				sleep(2);
			}              
        }
        else if(globbuf.gl_pathc != 1)
        {
			// �ж��Ƿ�ʱδ��ȡ��
			/* �ж�ʱ���Ƿ�ﵽ��ʱ��Ҫ�� */
			// 4���δ���ص������ű���
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
                // ������
				memset(sms_content, 0, sizeof(sms_content));
				sprintf(sms_content, "��ʱδ��ȡ�����ض����ļ�[%s]", onlinepay_ok_name);
                nReturnCode = sendMobileMsg(sms_content);
				if(nReturnCode) {
                    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "��������[%s]����ʧ��", sms_content);
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
        
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "����OK�ļ�:[%s]...", onlinepay_ok_name);
    while (1)
    {
        /* ȷ���ļ��Ƿ���� */
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
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "�����ļ�����:[%d][%s]...", nTotCount,sOut);
    
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
    
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "���ڵȴ����ض����ļ�:[%s]...", share_file_name);
           
        /* ��ʼ�����ļ���Ϣ */
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
        
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "����·��:[%s]...", onlinepay_file_name);
                
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
            
            /* ȷ���ļ��Ƿ���� */
            iRet = access(onlinepay_file_name, R_OK);
            if (iRet != 0)
            {
                HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sleep");
                sleep(2);
                continue;
            }
            
            /* ���Ŀ���ļ���СΪ0����ԴĿ���ļ���С��Ϊ0�����¸��� */
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

    /* ��ʼ�����ļ���Ϣ */
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
    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "���ض����ļ�[%d]�ѵ���.",i);
    return 0;
}

/****************************** end of Task_0104.c ******************************/

