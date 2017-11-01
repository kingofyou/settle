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
 *  �� �� ��: Task_8703.c
 *  ��    ��: ����T+2����
 *  �����Ա: 
 *  ����ʱ��: 2017308
 *  ��    ע:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include <sys/socket.h>
#include "batch.h"
#include "Common.h"


extern char	gLogFile[LOG_NAME_LEN_MAX];
extern  tbl_date_inf_def dbtbl_date_inf;
extern	char ext_inter_brh_code[10 + 1];

int SuspRecdToPlat(int flag, char* strBuf, char cTab, tbl_sett_list_def* vtdbl_sett_list,ht_tbl_acct_inf_def  tbl_acct_inf1,ht_tbl_acct_inf_def  tbl_acct_inf2);
int iInsertTblFileInfo(tbl_file_info_def   dbtbl_file_info);

/*************************************************
*�������ơ� Wait_9029_9030
*�������ܡ�
*���������
*���������
*�������ء�
***************************************************/
int Wait_200000_200003()
{
    int     nReturnCode;
    char    stlm_dt[8 + 1];
    
    tbl_child_inf_def        dbtbl_child_inf;

    /* �������� */
    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
    while(1)
    {
        memset(&dbtbl_child_inf, 0x00, sizeof(dbtbl_child_inf));
        memcpy(dbtbl_child_inf.inter_brh_code, "200000", BT_BRH_ID_LEN);
        memcpy(dbtbl_child_inf.settlmt_date, stlm_dt, BT_DATE_LEN);
        memcpy(dbtbl_child_inf.mission_index, BT_MISSION_INDEX_9901, BT_MISSION_INDEX_LEN);
        nReturnCode = DbsTblchildInf(DBS_SELECT1, &dbtbl_child_inf);
        if (nReturnCode != 0 && nReturnCode != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "select task status error[%d]", nReturnCode);
            return -1;
        }

        if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_SUSS)
        {
            break;
        }
        else if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_FAIL)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "task excuted error, please check.");
            return -1;
        }
        else
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "settlmt_date[%s],inter_brh_code[%s] task9901 excuting, please hold 30's.", stlm_dt,dbtbl_child_inf.inter_brh_code);
            sleep(2);
        }
    }
    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Check 200000 Task_9901 Success.");
        
    while(1)
    {
        memset(&dbtbl_child_inf, 0x00, sizeof(dbtbl_child_inf));
        memcpy(dbtbl_child_inf.inter_brh_code, "200002", BT_BRH_ID_LEN);
        memcpy(dbtbl_child_inf.settlmt_date, stlm_dt, BT_DATE_LEN);
        memcpy(dbtbl_child_inf.mission_index, BT_MISSION_INDEX_9901, BT_MISSION_INDEX_LEN);
        nReturnCode = DbsTblchildInf(DBS_SELECT1, &dbtbl_child_inf);
        if (nReturnCode != 0 && nReturnCode != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "select task status error[%d]", nReturnCode);
            return -1;
        }

        if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_SUSS)
        {
            break;
        }
        else if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_FAIL)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "task excuted error, please check.");
            return -1;
        }
        else
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "settlmt_date[%s],inter_brh_code[%s] task9901 excuting, please hold 30's.", stlm_dt,dbtbl_child_inf.inter_brh_code);
            sleep(2);
        }
    }
    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Check 200002 Task_9901 Success.");

    while(1)
    {
        memset(&dbtbl_child_inf, 0x00, sizeof(dbtbl_child_inf));
        memcpy(dbtbl_child_inf.inter_brh_code, "200003", BT_BRH_ID_LEN);
        memcpy(dbtbl_child_inf.settlmt_date, stlm_dt, BT_DATE_LEN);
        memcpy(dbtbl_child_inf.mission_index, BT_MISSION_INDEX_9901, BT_MISSION_INDEX_LEN);
        nReturnCode = DbsTblchildInf(DBS_SELECT1, &dbtbl_child_inf);
        if (nReturnCode != 0 && nReturnCode != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "select task status error[%d]", nReturnCode);
            return -1;
        }

        if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_SUSS)
        {
            break;
        }
        else if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_FAIL)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "task excuted error, please check.");
            return -1;
        }
        else
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "settlmt_date[%s],inter_brh_code[%s] task9901 excuting, please hold 30's.", stlm_dt,dbtbl_child_inf.inter_brh_code);
            sleep(2);
        }
    }
    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Check 200003 Task_9901 Success.");

    return 0;
}

/*************************************************
*�������ơ� Total_8703
*�������ܡ�
*���������
*���������
*�������ء� 1
***************************************************/
int Total_8703()
{
    return 1;
}

/*************************************************
*�������ơ� Task_8703
*�������ܡ� ���˼�¼��¼���ɺ��Ĺ����ļ�
*��������� nBeginOffset��nEndOffset
*���������
*�������ء� 0  -- �ɹ�
            -1 -- ʧ��
***************************************************/
int Task_8703 ( int nBeginOffset, int nEndOffset )
{
    int     nReturnCode;
    char    sDateTime[14+1];
    char    stlm_dt[8 + 1];

    char    *pstr;
    FILE    *fp;
    char    sFileFullName[200];
    char    sTmpFileName[100];

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);    
   
    tbl_file_info_def       dbtbl_file_info;

    /* �ȴ�������� */
    nReturnCode = Wait_200000_200003();
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Wait_200000_200003 err.");
        return -1;
    }

    memset(sTmpFileName, 0x00, sizeof(sTmpFileName));
    strcpy(sTmpFileName, "HOST_YYYYMMDD_XX_ACCT.txt");
    pstr = strstr(sTmpFileName, "YYYYMMDD");
    if (pstr)
        memcpy(pstr, stlm_dt, 8);
    pstr = strstr(sTmpFileName, "XX");
    if (pstr)
        memcpy (pstr, "06", 2);
                
    /* ��ʼ�ļ���Ϣ */
    memset(sDateTime, 0x00, sizeof(sDateTime));
    getSysTime(sDateTime);
    memset(&dbtbl_file_info, 0x00, sizeof(dbtbl_file_info));
    memcpy(dbtbl_file_info.sett_date, stlm_dt, 8);
    memcpy(dbtbl_file_info.snd_time, sDateTime, 14);
    memcpy(dbtbl_file_info.step_no, "04", 2);
    memcpy(dbtbl_file_info.batch_no, "06", 2);
    memcpy(dbtbl_file_info.file_type, "H", 1);
    memcpy(dbtbl_file_info.file_flag, "S", 1);
    memcpy(dbtbl_file_info.end_flag, "Y", 1);
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
    memcpy(dbtbl_file_info.file_name, sFileFullName, sizeof(dbtbl_file_info.file_name)-1);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name : [%s]", dbtbl_file_info.file_name);
    
    fp = fopen(dbtbl_file_info.file_name,"w+");
    if (fp == NULL)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen [%s] error.", dbtbl_file_info.file_name);
        return -1;
    }

    nReturnCode = SuspFail(fp);
    if(nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "SuspFail err[%d]", nReturnCode);
        fclose(fp);
        return -1;
    }
    
    nReturnCode = SuspPause(fp);
    if(nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "SuspFail err[%d]", nReturnCode);
        fclose(fp);
        return -1;
    }    
    
    fclose(fp);
    memcpy(dbtbl_file_info.status, "0", 1);   
    /* �����ļ���Ϣ�� */
    nReturnCode = iInsertTblFileInfo(dbtbl_file_info);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "iInsertTblFileInfo error [%d]", nReturnCode);
        return -1;
    }

    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_9070 Succ stlm_dt[%s].",stlm_dt);
 
    return 0;
}

/*���л���ʧ�ܼ�¼����*/
int SuspFail(FILE* fp)
{
    int     nReturnCode;
    char    stlm_dt[8+1]; 
    int     iTotolCount = 0;   
    char    sFileRecord[RECORD_LEN_MAX];
    tbl_sett_list_def   dbtbl_sett_list;      
    ht_tbl_acct_inf_def  tbl_acct_inf1;
    ht_tbl_acct_inf_def  tbl_acct_inf2;

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
    /*��Ӫ�̽����˻� �˺���Ϣ*/
    memset(&tbl_acct_inf1,0x0,sizeof(tbl_acct_inf1));
    memcpy(tbl_acct_inf1.type,"03",2);
    nReturnCode = DbsTblAcctInf(DBS_SELECT, &tbl_acct_inf1);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAcctInf [%s] err[%d]",tbl_acct_inf1.type,nReturnCode);
        return -1;
    }       

    /*��Ӫ�̽����˻� �˺���Ϣ*/
    memset(&tbl_acct_inf2,0x0,sizeof(tbl_acct_inf2));
    memcpy(tbl_acct_inf2.type,"12",2);
    nReturnCode = DbsTblAcctInf(DBS_SELECT, &tbl_acct_inf2);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAcctInf [%s] err[%d]",tbl_acct_inf2.type,nReturnCode);
        return -1;
    }           

	// �ȼ���Ƿ��д������δ֪������δ����
    memset(&dbtbl_sett_list, 0x00, sizeof(dbtbl_sett_list));
    memcpy(dbtbl_sett_list.sett_date, dbtbl_date_inf.syesterday, 8);
    memcpy(dbtbl_sett_list.dc_flag, "C", 1); 
	while(1) {
	    nReturnCode = DbsTblSettList(DBS_SELECT3, &dbtbl_sett_list);  
		if(nReturnCode) {
			sleep(10);
			continue;
		}
		else break;
	}
           
    memset(&dbtbl_sett_list, 0x00, sizeof(dbtbl_sett_list));    
    memcpy(dbtbl_sett_list.sett_date, dbtbl_date_inf.syesterday, 8);
    //memcpy(dbtbl_sett_list.mcht_sett_acct_type, "1", 1);
    memcpy(dbtbl_sett_list.status, "5", 1);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date:[%s], acct_type1:[%s], status:[%s]", dbtbl_sett_list.sett_date, dbtbl_sett_list.mcht_sett_acct_type, dbtbl_sett_list.status);
    /* �α꣬������ϸ�� */
    nReturnCode = DbsTblSettList(DBS_CURSOR2, &dbtbl_sett_list);    
    nReturnCode = DbsTblSettList(DBS_OPEN2, &dbtbl_sett_list);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN2 ht_tbl_sett_list_cur2 error [%d]", nReturnCode);
        return -1;
    }   
    while(1)
    {
        /* fetch ÿ��δ���˵ļ�¼ */
        memset(&dbtbl_sett_list, 0, sizeof(dbtbl_sett_list));
        nReturnCode = DbsTblSettList(DBS_FETCH2, &dbtbl_sett_list);
        if (nReturnCode == DBS_NOTFOUND)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_FETCH_VAR tbl_sett_var_cur end .");
            break;
        }
        else if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_FETCH_VAR tbl_sett_var_cur error [%d]", nReturnCode);
            DbsTblSettList(DBS_CLOSE2, &dbtbl_sett_list);
            return -1;
        }
        if(strncmp(dbtbl_sett_list.mcht_role, "0011",4) == 0 ||
            strncmp(dbtbl_sett_list.mcht_role, "0031",4) == 0)
        { 
            continue;         
        }      
        iTotolCount++;       
        /* ����¼��Ϣд���ļ��� */
        memset(sFileRecord, 0x00, sizeof(sFileRecord));
        SuspRecdToPlat(0, sFileRecord, '|', &dbtbl_sett_list, tbl_acct_inf1,tbl_acct_inf2);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "Record:[%s]", sFileRecord);
        nReturnCode = fputs(sFileRecord, fp);
        if(nReturnCode>=0 && nReturnCode != EOF)
        {
	        memcpy(&dbtbl_sett_list.status, "a", 1);
	        CommonRTrim(dbtbl_sett_list.seq_no);  
	        /* ���½�����ϸ���¼ */
	        nReturnCode = DbsTblSettList(DBS_UPD_SEQ, &dbtbl_sett_list);
	        if (nReturnCode)
	        {
	            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPD_SEQ ht_tbl_acct_rslt error [%d]", nReturnCode);
	            DbsTblSettList(DBS_CLOSE2, &dbtbl_sett_list);
	            return -1;
	        }
	    }
    }
    DbsTblSettList(DBS_CLOSE2, &dbtbl_sett_list);     
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "iTotolCount:[%d]", iTotolCount);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "SuspFail succ");
  
    return 0;
}

/*���˽���ݻ���¼����*/
int SuspPause(FILE* fp)
{
    int     nReturnCode;
    char    sDateTime[14+1];
    char    stlm_dt[8+1];     
    char    sFileRecord[RECORD_LEN_MAX];  
    int     iTotolCount = 0;
    
    ht_tbl_acct_rslt_def   dbtbl_acct_rslt;
    tbl_sett_list_def   dbtbl_sett_clist, dbtbl_sett_dlist;
    ht_tbl_acct_inf_def  tbl_acct_inf1;
    ht_tbl_acct_inf_def  tbl_acct_inf2;

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
     
    /*��Ӫ�̽����˻� �˺���Ϣ*/
    memset(&tbl_acct_inf1,0x0,sizeof(tbl_acct_inf1));
    memcpy(tbl_acct_inf1.type,"03",2);
    nReturnCode = DbsTblAcctInf(DBS_SELECT, &tbl_acct_inf1);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAcctInf [%s] err[%d]",tbl_acct_inf1.type,nReturnCode);
        return -1;
    }       

    /*��Ӫ�̽����˻� �˺���Ϣ*/
    memset(&tbl_acct_inf2,0x0,sizeof(tbl_acct_inf2));
    memcpy(tbl_acct_inf2.type,"12",2);
    nReturnCode = DbsTblAcctInf(DBS_SELECT, &tbl_acct_inf2);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAcctInf [%s] err[%d]",tbl_acct_inf2.type,nReturnCode);
        return -1;
    }     
    
    memset(&dbtbl_acct_rslt, 0x00, sizeof(dbtbl_acct_rslt));
    memset(sDateTime, 0x00, sizeof(sDateTime));
    getSysTime(sDateTime);
    memcpy(dbtbl_acct_rslt.sett_date, stlm_dt, 8);
    /*�������ڴ�����������,��������״̬Ϊ'N'*/
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date:[%s].", dbtbl_acct_rslt.sett_date);

    nReturnCode = DbsTblAcctRslt(DBS_CURSOR2, &dbtbl_acct_rslt,0,0);    
    nReturnCode = DbsTblAcctRslt(DBS_OPEN2, &dbtbl_acct_rslt,0,0);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN2 tbl_rslt_sett_cur2 error [%d]", nReturnCode);
        return -1;
    }
    
    while(1)
    {
        /* fetch ÿ��δ���˵ļ�¼ */
        memset(&dbtbl_acct_rslt, 0, sizeof(dbtbl_acct_rslt));
        nReturnCode = DbsTblAcctRslt(DBS_FETCH2, &dbtbl_acct_rslt,0,0);
        if (nReturnCode == DBS_NOTFOUND)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_FETCH2 tbl_rslt_sett_cur2 end.");
            break;
        }
        else if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_FETCH2 tbl_rslt_sett_cur2 error [%d]", nReturnCode);
            DbsTblAcctRslt(DBS_CLOSE2, &dbtbl_acct_rslt,0,0);
            return -1;
        }
        iTotolCount++; 
        memset(&dbtbl_sett_clist, 0, sizeof(dbtbl_sett_clist));
        RightTrim(dbtbl_acct_rslt.inter_brh_code);
        memcpy(dbtbl_sett_clist.brh_id, dbtbl_acct_rslt.inter_brh_code, strlen(dbtbl_acct_rslt.inter_brh_code));                    /*�ڲ�������*/
        memcpy(dbtbl_sett_clist.sett_date, sDateTime, 8);                                                                           /*��������*/
        memcpy(dbtbl_sett_clist.trans_date, dbtbl_acct_rslt.acct_date, strlen(dbtbl_acct_rslt.acct_date));                          /*��������*/
        RightTrim(dbtbl_acct_rslt.mcht_rslt_no);
        memcpy(dbtbl_sett_clist.mcht_rslt_no, dbtbl_acct_rslt.mcht_rslt_no, strlen(dbtbl_acct_rslt.mcht_rslt_no));                  /*�̻�������ˮ��*/
    	    
        RightTrim(dbtbl_acct_rslt.mcht_sett_bank);
        memcpy(dbtbl_sett_clist.mcht_sett_bank, dbtbl_acct_rslt.mcht_sett_bank, strlen(dbtbl_acct_rslt.mcht_sett_bank));                 /*�̻�������*/
        memcpy(dbtbl_sett_clist.mcht_sett_acct_type, dbtbl_acct_rslt.mcht_sett_acct_type, strlen(dbtbl_acct_rslt.mcht_sett_acct_type));       /*�̻������˻�����*/
        RightTrim(dbtbl_acct_rslt.mcht_sett_acct);
        memcpy(dbtbl_sett_clist.mcht_sett_acct, dbtbl_acct_rslt.mcht_sett_acct, strlen(dbtbl_acct_rslt.mcht_sett_acct));                   /*�̻������˻��˺�*/
        RightTrim(dbtbl_acct_rslt.mcht_sett_acct_nm);
        memcpy(dbtbl_sett_clist.mcht_sett_acct_nm, dbtbl_acct_rslt.mcht_sett_acct_nm, strlen(dbtbl_acct_rslt.mcht_sett_acct_nm));           /*�̻������˻�����*/
            
        dbtbl_sett_clist.amt = (dbtbl_acct_rslt.debt_at - dbtbl_acct_rslt.cret_at)/100;                                          /*���˽��*/
        
        memcpy(dbtbl_sett_clist.seq_no, dbtbl_acct_rslt.sett_date, 8);
        sprintf(dbtbl_sett_clist.seq_no+8, "%010d%s", dbtbl_acct_rslt.seq_num, "01");                                               /*������ˮ��*/
            
        RightTrim(dbtbl_acct_rslt.plat_sett_bank);
        memcpy(dbtbl_sett_clist.plat_sett_bank, dbtbl_acct_rslt.plat_sett_bank, strlen(dbtbl_acct_rslt.plat_sett_bank));                 /*ƽ̨������*/
        memcpy(dbtbl_sett_clist.plat_sett_acct_type, dbtbl_acct_rslt.plat_sett_acct_type, strlen(dbtbl_acct_rslt.plat_sett_acct_type));       /*ƽ̨�����˻�����*/
        RightTrim(dbtbl_acct_rslt.plat_sett_acct);
        memcpy(dbtbl_sett_clist.plat_sett_acct, dbtbl_acct_rslt.plat_sett_acct, strlen(dbtbl_acct_rslt.plat_sett_acct));                   /*ƽ̨�����˻��˺�*/
        RightTrim(dbtbl_acct_rslt.plat_sett_acct_nm);
        memcpy(dbtbl_sett_clist.plat_sett_acct_nm, dbtbl_acct_rslt.plat_sett_acct_nm, strlen(dbtbl_acct_rslt.plat_sett_acct_nm));           /*ƽ̨�����˻�����*/

        memset(sFileRecord, 0x00, sizeof(sFileRecord));
        SuspRecdToPlat(1, sFileRecord, '|', &dbtbl_sett_clist,tbl_acct_inf1,tbl_acct_inf2);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "Record:[%s]", sFileRecord);
        nReturnCode = fputs(sFileRecord, fp);
        if(nReturnCode < 0 || nReturnCode == EOF)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fputs error.");
            DbsTblAcctRslt(DBS_CLOSE2, &dbtbl_acct_rslt,0,0);
            return -1;
        }
    	RightTrim(dbtbl_acct_rslt.mcht_rslt_no);
    	RightTrim(dbtbl_acct_rslt.mcht_role);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_rslt_no:[%s]mcht_role[%s].", dbtbl_acct_rslt.mcht_rslt_no,dbtbl_acct_rslt.mcht_role);
	    memcpy(dbtbl_acct_rslt.acct_statu, "R", 1);
	    nReturnCode = DbsTblAcctRslt(DBS_UPDATE, &dbtbl_acct_rslt,0,0);
	    if (nReturnCode == DBS_NOTFOUND)
	    {
	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPDATE ht_tbl_acct_rslt error [%d]", nReturnCode);
	        DbsTblAcctRslt(DBS_CLOSE2, &dbtbl_acct_rslt,0,0);
	        return -1;
	    }
    }
    DbsTblAcctRslt(DBS_CLOSE2, &dbtbl_acct_rslt,0,0);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "iTotolCount:[%d]", iTotolCount);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "SuspPause succ");
    return 0;
}

int SuspRecdToPlat(int flag, char* strBuf, char cTab, tbl_sett_list_def* vtdbl_sett_list,ht_tbl_acct_inf_def  tbl_acct_inf1,ht_tbl_acct_inf_def  tbl_acct_inf2)
{
    int     nReturnCode;
    char    sRcdBuf[RECORD_LEN_MAX] = {0};
    char    tmpBuf[50] = {0};
    char    sDelim[1+1];
    char    sDateTime[14+1];
    
    memset(sDelim, 0, sizeof(sDelim));
    sDelim[0]=cTab;
    
    RightTrim(vtdbl_sett_list->seq_no);
    strncat(sRcdBuf, vtdbl_sett_list->seq_no, strlen(vtdbl_sett_list->seq_no));                         /*ƽ̨��ˮ��*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    strncat(sRcdBuf, "TOPCPG", 6);
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    getSysTime(sDateTime);
    strncat(sRcdBuf, vtdbl_sett_list->sett_date, 8);                                                                     /*��������*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    strncat(sRcdBuf, sDateTime+8, 6);                                                                   /*����ʱ��*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));

    RightTrim(tbl_acct_inf1.bank_code);
    strncat(sRcdBuf, tbl_acct_inf1.bank_code, strlen(tbl_acct_inf1.bank_code));                   /* ת�������к� */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(tbl_acct_inf1.acct_type);
    strncat(sRcdBuf, tbl_acct_inf1.acct_type, strlen(tbl_acct_inf1.acct_type));                   /* ת���˺����� */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(tbl_acct_inf1.acct_no);
    strncat(sRcdBuf, tbl_acct_inf1.acct_no, strlen(tbl_acct_inf1.acct_no));                       /* ת���˺� */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(tbl_acct_inf1.acct_name);
    strncat(sRcdBuf, tbl_acct_inf1.acct_name, strlen(tbl_acct_inf1.acct_name));                   /* ת�������� */
    strncat(sRcdBuf, sDelim, strlen(sDelim));

    strncat(sRcdBuf, "156", 3);                                                                         /* ���� */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    sprintf(sRcdBuf+strlen(sRcdBuf), "%.2f", vtdbl_sett_list->amt);										/*���׽��*/
    strncat(sRcdBuf, sDelim, strlen(sDelim)); 

    RightTrim(tbl_acct_inf2.bank_code);
    strncat(sRcdBuf, tbl_acct_inf2.bank_code, strlen(tbl_acct_inf2.bank_code));                 /* ת�뽻���к� */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(tbl_acct_inf2.acct_type);
    strncat(sRcdBuf, tbl_acct_inf2.acct_type, strlen(tbl_acct_inf2.acct_type));                 /* ת���˺����� */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(tbl_acct_inf2.acct_no);
    strncat(sRcdBuf, tbl_acct_inf2.acct_no, strlen(tbl_acct_inf2.acct_no));                     /* ת���˺� */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(tbl_acct_inf2.acct_name);
    strncat(sRcdBuf, tbl_acct_inf2.acct_name, strlen(tbl_acct_inf2.acct_name));                 /* ת�뷽���� */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    if(flag == 0)
    {
        strncat(sRcdBuf, "03", 2);
    }
    else if(flag == 1)
    {
        strncat(sRcdBuf, "05", 2);
    }
    
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
/****************************** end of Task_8703.c ******************************/


