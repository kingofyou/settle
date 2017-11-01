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
 *  �� �� ��: Task_7502.c
 *  ��    ��: �鼯�ɹ���¼���ɺ��ļ����ļ�
 *  �����Ա: bcw
 *  ����ʱ��: 2017308
 *  ��    ע:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include <sys/socket.h>
#include "batch.h"
#include "Common.h"

extern  char    gLogFile[LOG_NAME_LEN_MAX];
extern  char    ext_child_date[8 + 1];
extern  char    ext_inter_brh_code[10 + 1];
extern  int     ext_inter_brh_sta;
extern          tbl_date_inf_def dbtbl_date_inf;

int MerRecdToPlat(char* strBuf, char cTab, tbl_mission_info_def* vtdbl_miss_info);
int iInsertTblFileInfo(tbl_file_info_def   dbtbl_file_info);

/*************************************************
*�������ơ� Total_7502
*�������ܡ�
*���������
*���������
*�������ء� 1
***************************************************/
int Total_7502()
{
    return 1;
}

/*************************************************
*�������ơ� Task_7502
*�������ܡ� ������ϸ�����п���¼���ɺ��ļ����ļ�
*��������� nBeginOffset��nEndOffset
*���������
*�������ء� 0  -- �ɹ�
            -1 -- ʧ��
***************************************************/
int Task_7502 ( int nBeginOffset, int nEndOffset )
{
    int     nReturnCode;
    char    sDateTime[14+1];
    char    *pstr;
    char    sFileRecord[RECORD_LEN_MAX];
    FILE    *fp;
    char    sFileFullName[100];
    char    sTmpFileName[30];
    char    stlm_dt[8+1];   
    int     nFileCount =0; 
    
    tbl_mission_info_def   dbtbl_miss_info;
    tbl_file_info_def   dbtbl_file_info;

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);

    /* ��ʼ�ļ���Ϣ */
    getSysTime(sDateTime);
    memset(&dbtbl_file_info, 0x00, sizeof(dbtbl_file_info));
    memcpy(dbtbl_file_info.sett_date, stlm_dt, 8);
    memcpy(dbtbl_file_info.snd_time, sDateTime, 14);
    strcpy(sTmpFileName, "HOST_YYYYMMDD_XX_ACCT.txt");
    pstr = strstr(sTmpFileName, "YYYYMMDD");
    if (pstr)
        memcpy(pstr, stlm_dt, 8);
    pstr = strstr(sTmpFileName, "XX");
    if (pstr)
        memcpy (pstr, "03", 2);
    memcpy(dbtbl_file_info.step_no, "04", 2);
    memcpy(dbtbl_file_info.batch_no, "03", 2);
    memcpy(dbtbl_file_info.file_type, "H", 1);
    memcpy(dbtbl_file_info.file_flag, "S", 1);
    memcpy(dbtbl_file_info.end_flag, "N", 1);
    memcpy(dbtbl_file_info.bank_code, "HOST", 4);
    
    memset(sFileFullName, 0, sizeof(sFileFullName));
    sprintf(sFileFullName, "%s/%8.8s/%s/%s/%s/%s%s/",
            getenv("SHARE_FILE_PATH"),
            dbtbl_file_info.sett_date,
            dbtbl_file_info.bank_code,
            dbtbl_file_info.file_type,
            dbtbl_file_info.file_flag,
            dbtbl_file_info.step_no,
            dbtbl_file_info.batch_no);
    nReturnCode = CheckDir(sFileFullName);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "CheckDir err[%d][%s][%s]",
               nReturnCode, strerror(errno), sFileFullName);
        return -1;
    }
    strcat(sFileFullName, sTmpFileName);
    memcpy(dbtbl_file_info.file_name, sFileFullName, strlen(sFileFullName));
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name : [%s]", dbtbl_file_info.file_name);
    
    fp = fopen(dbtbl_file_info.file_name,"w+");
    if (fp == NULL)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen [%s] error.", dbtbl_file_info.file_name);
        return -1;
    }

    memset(&dbtbl_miss_info, 0x00, sizeof(dbtbl_miss_info));
    memcpy(dbtbl_miss_info.sett_date, stlm_dt, 8);
    memcpy(dbtbl_miss_info.status, "2", 1);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date:[%s], status:[%s]\n", dbtbl_miss_info.sett_date, dbtbl_miss_info.status);
    /* �α꣬�鼯��ϸ�� */
    nReturnCode = DbsTblMissInfo(DBS_CURSOR, &dbtbl_miss_info);
    
    nReturnCode = DbsTblMissInfo(DBS_OPEN, &dbtbl_miss_info);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN ht_tbl_mission_info error [%d]", nReturnCode);
        fclose(fp);
        return -1;
    }
    
    while(1)
    {
        /* fetch ÿ��δ���˵ļ�¼ */
        memset(&dbtbl_miss_info, 0, sizeof(dbtbl_miss_info));
        nReturnCode = DbsTblMissInfo(DBS_FETCH, &dbtbl_miss_info);
        if (nReturnCode == DBS_NOTFOUND)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_FETCH ht_tbl_mission_info end.");
            break;
        }
        else if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_FETCH ht_tbl_mission_info error [%d]", nReturnCode);
            DbsTblMissInfo(DBS_CLOSE, &dbtbl_miss_info);
            fclose (fp);
            return -1;
        }
        nFileCount++;
        
        /* ����¼��Ϣд���ļ��� */
        memset(sFileRecord, 0x00, sizeof(sFileRecord));
        MerRecdToPlat(sFileRecord, '|', &dbtbl_miss_info);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "Record:[%s]", sFileRecord);
        nReturnCode = fputs(sFileRecord, fp);
        if(nReturnCode >= 0 && nReturnCode != EOF)
        {
	        memcpy(&dbtbl_miss_info.status, "4", 1);
	        RightTrim(dbtbl_miss_info.seq_no);
	        /* ���¹鼯��ϸ���¼ */
	        nReturnCode = DbsTblMissInfo(DBS_UPDATE, &dbtbl_miss_info);
	        if (nReturnCode)
	        {
	            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPDATE ht_tbl_mission_info error [%d]", nReturnCode);
	            DbsTblMissInfo(DBS_CLOSE, &dbtbl_miss_info);
	            fclose (fp);
	            return -1;
	        }
	    }
    }
    DbsTblMissInfo(DBS_CLOSE, &dbtbl_miss_info);
    fclose(fp);
    
    memcpy(dbtbl_file_info.status, "0", 1);
    
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name:[%s]\n", dbtbl_file_info.file_name);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date:[%s]\n", dbtbl_file_info.sett_date);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "snd_time :[%s]\n", dbtbl_file_info.snd_time); 
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "rcv_time :[%s]\n", dbtbl_file_info.rcv_time); 
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "status   :[%s]\n", dbtbl_file_info.status);   
    
    /* �����ļ���Ϣ�� */
    nReturnCode = iInsertTblFileInfo(dbtbl_file_info);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_INSERT ht_tbl_file_info error [%d]", nReturnCode);
        return -1;
    }
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nFileCount:[%d]", nFileCount);
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_7502 Succ .");

    return 0;
}

int MerRecdToPlat(char* strBuf, char cTab, tbl_mission_info_def* vtdbl_miss_info)
{
    int     nReturnCode;
    char    sRcdBuf[RECORD_LEN_MAX] = {0};
    char    tmpBuf[50] = {0};
    char    sDelim[1+1];
    char    sDateTime[14+1];
    
    memset(sDelim, 0, sizeof(sDelim));
    sDelim[0]=cTab;
    
    RightTrim(vtdbl_miss_info->seq_no);
    strncat(sRcdBuf, vtdbl_miss_info->seq_no, strlen(vtdbl_miss_info->seq_no));                         /*ƽ̨��ˮ��*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    strncat(sRcdBuf, "TOPCPG", 6);
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    RightTrim(vtdbl_miss_info->chnl_id);
    strncat(sRcdBuf, vtdbl_miss_info->chnl_id, strlen(vtdbl_miss_info->chnl_id));                       /* ���� */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    getSysTime(sDateTime);
    strncat(sRcdBuf, sDateTime, 8);                                                                     /*��������*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    strncat(sRcdBuf, sDateTime+8, 6);                                                                   /*����ʱ��*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));

    RightTrim(vtdbl_miss_info->bank_code1);
    strncat(sRcdBuf, vtdbl_miss_info->bank_code1, strlen(vtdbl_miss_info->bank_code1));                 /* ת�������к� */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_miss_info->acct_type1);
    strncat(sRcdBuf, vtdbl_miss_info->acct_type1, strlen(vtdbl_miss_info->acct_type1));                 /* ת���˺����� */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_miss_info->acct_no1);
    strncat(sRcdBuf, vtdbl_miss_info->acct_no1, strlen(vtdbl_miss_info->acct_no1));                     /* ת���˺� */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_miss_info->acct_name1);
    strncat(sRcdBuf, vtdbl_miss_info->acct_name1, strlen(vtdbl_miss_info->acct_name1));                 /* ת�������� */
    strncat(sRcdBuf, sDelim, strlen(sDelim));

    strncat(sRcdBuf, "156", 3);                                                                         /* ���� */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    sprintf(sRcdBuf+strlen(sRcdBuf), "%.2lf", vtdbl_miss_info->amt);									/*���׽��*/
    strncat(sRcdBuf, sDelim, strlen(sDelim)); 

    RightTrim(vtdbl_miss_info->bank_code);
    strncat(sRcdBuf, vtdbl_miss_info->bank_code, strlen(vtdbl_miss_info->bank_code));                   /* ת�뽻���к� */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_miss_info->acct_type);
    strncat(sRcdBuf, vtdbl_miss_info->acct_type, strlen(vtdbl_miss_info->acct_type));                   /* ת���˺����� */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_miss_info->acct_no);
    strncat(sRcdBuf, vtdbl_miss_info->acct_no, strlen(vtdbl_miss_info->acct_no));                       /* ת���˺� */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_miss_info->acct_name);
    strncat(sRcdBuf, vtdbl_miss_info->acct_name, strlen(vtdbl_miss_info->acct_name));                   /* ת�뷽���� */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    strncat(sRcdBuf, "02", 2);                                                                          /* �������� */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    strcat(sRcdBuf, "\n");
    memcpy(strBuf, sRcdBuf, strlen(sRcdBuf));
    
    return 0;
}

int iInsertTblFileInfo(tbl_file_info_def   dbtbl_file_info)
{
    int iRet = 0;
    HtLog(gLogFile,HT_LOG_MODE_DEBUG,__FILE__,__LINE__,
        "view the data dbtbl_file_info [%s][%s][%s][%s][%s][%s][%s]",dbtbl_file_info.sett_date,dbtbl_file_info.bank_code,dbtbl_file_info.file_type,dbtbl_file_info.file_flag,dbtbl_file_info.step_no,dbtbl_file_info.batch_no,dbtbl_file_info.file_name);
   
    iRet = DbsTblFileInfo(DBS_SELECT, &dbtbl_file_info);
    if(iRet == DBS_NOTFOUND)
    {
        iRet = DbsTblFileInfo(DBS_INSERT, &dbtbl_file_info);
        if(iRet)
        {
            HtLog(gLogFile,HT_LOG_MODE_ERROR,__FILE__,__LINE__,
                "DbsTblFileInfo DBS_INSERT error!:[%d]", iRet);
            return iRet;
        }
    }
    else if(iRet)
    {
        HtLog(gLogFile,HT_LOG_MODE_ERROR,__FILE__,__LINE__,
            "DbsTblFileInfo DBS_SELECT error!:[%d]", iRet);
        return iRet;
    }
    else
    {
        CommonRTrim(dbtbl_file_info.sett_date);
        CommonRTrim(dbtbl_file_info.file_name);
        iRet = DbsTblFileInfo(DBS_UPDATE, &dbtbl_file_info);
        if(iRet)
        {
            HtLog(gLogFile,HT_LOG_MODE_ERROR,__FILE__,__LINE__,
                      "DbsTblFileInfo DBS_UPDATE error!");
            return iRet;
        }
    }    
    return 0;
}
/****************************** end of Task_7502.c ******************************/


