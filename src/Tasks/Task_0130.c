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
 *  �� �� ��: Task_0130.c
 *  ��    ��: ȷ�Ϸ����ˮ�ļ�
 *  �����Ա: HELU
 *  ����ʱ��: 2016-12-18
 *  ��    ע:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include "batch.h"

extern char	gLogFile[LOG_NAME_LEN_MAX];
extern  tbl_date_inf_def dbtbl_date_inf;
extern	char ext_inter_brh_code[10 + 1];

/*************************************************
*�������ơ� Total_0130
*�������ܡ�
*���������
*���������
*�������ء� 1
***************************************************/
int Total_0130()
{
    return 1;
}

/*************************************************
*�������ơ� Task_0130
*�������ܡ�
*��������� nBeginOffset��nEndOffset
*���������
*�������ء� 0  -- �ɹ�
            -1 -- ʧ��
***************************************************/
int Task_0130( int nBeginOffset, int nEndOffset )
{
    int		iRet = 0;
    char 	sFlag[1 + 1] = {0};
    char    *pstr;
    char	onlinepay_file_name[200] = {0};
    char	share_file_name[200] = {0};
    char	ok_file_name[200] = {0};
    char    sCommand[256] = {0};
    struct stat stat_buf_share;
    struct stat stat_buf_online;
    stExtnFileRegDef StExtnFileReg;

    /* ��ʼ����Ŀ¼�ļ���Ϣ */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    memcpy(StExtnFileReg.host_name, "RISK", 9);
    memcpy(StExtnFileReg.src_file_name, "RISK_YYYYMMDD_XX.txt", 25);

    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);
    pstr = strstr (StExtnFileReg.src_file_name, "XX");
    if (pstr)
        memcpy (pstr, "01", 2);

    getShareFileFullName(&StExtnFileReg, share_file_name);

    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "���ڵȴ����ط���ļ�:[%s]...", share_file_name);
    
    memset(ok_file_name, 0, sizeof(ok_file_name));

    /* ��ʼok�ļ���Ϣ */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    memcpy(StExtnFileReg.host_name, "RISK", 9);
    strcpy(StExtnFileReg.src_file_name, "RISK_YYYYMMDD_XX.ok");

    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);
    pstr = strstr (StExtnFileReg.src_file_name, "XX");
    if (pstr)
        memcpy (pstr, "01", 2);

    getShareFileFullName(&StExtnFileReg, ok_file_name);

    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "���ڵȴ����ط��ok�ļ�:[%s]...", ok_file_name);

    /* ��ʼ�����ļ���Ϣ */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    memcpy(StExtnFileReg.host_name, "RISK", 9);
    memcpy(StExtnFileReg.src_file_name, "RISK_YYYYMMDD_XX.txt", 25);

    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);
    pstr = strstr (StExtnFileReg.src_file_name, "XX");
    if (pstr)
        memcpy (pstr, "01", 2);

    getExtnFileFullName(&StExtnFileReg, onlinepay_file_name);

    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "����·��:[%s]...", onlinepay_file_name);
    
    while (1)
    {
        /* ȷ���ļ��Ƿ���� */
        iRet = access(ok_file_name, R_OK);
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


    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "���ط���ļ��Ѿ�����");
    return 0;
}

/****************************** end of Task_0130.c ******************************/

