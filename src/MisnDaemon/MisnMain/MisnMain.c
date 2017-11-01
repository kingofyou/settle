
/* * ************************************************************************** */
/*                NCUP -- Shanghai Huateng Software System Inc.              */
/* * ************************************************************************** */
/* PROGRAM NAME: MisnMain.pc                                                     */
/* DESCRIPTIONS: main function for batch                                     */
/* * ************************************************************************** */
/*                        MODIFICATION LOG                                   */
/* DATE        PROGRAMMER     DESCRIPTION                                    */
/* 2016-5-20   HELU           initial                                        */
/* * ************************************************************************** */

#include "batch.h"

/* EXEC SQL INCLUDE sqlca; */

char gLogFile[LOG_NAME_LEN_MAX];
/* EXEC SQL BEGIN DECLARE SECTION; */
char ext_inter_brh_code[10 + 1];
int ext_inter_brh_sta = 0;
/* EXEC SQL END DECLARE SECTION; */

tbl_date_inf_def dbtbl_date_inf;
tbl_child_inf_def dbtbl_child_inf;

void vCTerminate();

int nLoadInst(stInstDef *ptInst)
{
    int liFldX;
    int nReturnCode;

    stInstInf stInstInfo;

    memset( (char *)&stInstInfo, 0, sizeof(stInstInf));

    DbsInstMisn(DBS_CURSOR, &stInstInfo);
    nReturnCode = DbsInstMisn(DBS_OPEN, &stInstInfo);
     if (nReturnCode)
     {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN error, %d.", nReturnCode);
            return nReturnCode;
    }
    liFldX = 0;
    while (1)
    {
        memset(&stInstInfo, 0, sizeof(stInstInf));

        nReturnCode = DbsInstMisn( DBS_FETCH, &stInstInfo);
        if (nReturnCode == DBS_NOTFOUND)
        {
            break;
        }
        else if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nReturnCode = [%d].", nReturnCode);
            return nReturnCode;
        }
        HtMemcpy( ptInst->inst_def[liFldX].inter_brh_code, stInstInfo.inter_brh_code, 10);
        HtMemcpy( ptInst->inst_def[liFldX].brh_level, stInstInfo.brh_level, 1);

        liFldX++;
    } /* end of while */

    ptInst->inst_num = liFldX;

    nReturnCode = DbsInstMisn(DBS_CLOSE, &stInstInfo);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Cursor close error, %d.", nReturnCode);
        return nReturnCode;
    }

    return 0;
}

int main( int argc, char **argv)
{
	int i = 0;
	int fpid = 0;
    int rc;
    int nReturnCode;
	stInstDef stInst;

    /*    EXEC SQL BEGIN DECLARE SECTION; */
   
    /*    EXEC SQL END DECLARE SECTION; */

    
    memset(gLogFile, 0, LOG_NAME_LEN_MAX);
    HtSprintf(gLogFile, "MisnMain.log");              /*  ��־Ŀ¼ MisnMain.log   */
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "misnmain started.");

    sigset(SIGHUP, SIG_IGN);

    if (signal(SIGTERM, vCTerminate) == SIG_ERR)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "sigset SIGTERM error, %d", errno);
        exit(-10);
    } /* end of if */


    while (1)
    {
        /* connect to database  */
        nReturnCode = DbsConnect();
        if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsConnect error, %d.", nReturnCode);
            DbsDisconnect();
            sleep(5);
            continue;
        }

		memset( (char *)&stInst, 0, sizeof(stInstDef));
        nReturnCode = nLoadInst(&stInst);
        if (nReturnCode)
        {
            /* disconnect to database */
			HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "select from ht_tbl_brh_info err sqlcode[%d].", nReturnCode);
            DbsDisconnect();
            sleep(5);
            continue;
        }
		break;
	}

	int flag = 0;
    for(; i < stInst.inst_num; i++)
    {
		flag = fork();   
	    if(!flag) {
			while(1) {
                checkTaskStatus(stInst.inst_def[i].inter_brh_code);	
				DbsDisconnect();
		    }
		}			
	    flag = 0;
	}
	DbsDisconnect();
} /* end of main */


int checkTaskStatus(char* inter_brh_code) {
	tbl_mission_inf_def dbtbl_mission_inf;
	tbl_mission_inf_def dbtbl_mission_inf2;
	char sSettlmtDate[8 + 1];
	int nTotalNum = 0;
    int nMisnCnt;
    time_t lTime;
    struct tm *tTmLocal;
    char lib_name[256 + 1];
    char func_name[256 + 1];
	int nReturnCode = 0;

	char sTmpTime[128];
    int nChildNum;    
    int sWT_STTL_STATUS;
    int sIN_STTL_STATUS;
    int sERR_OCC_STATUS;
    int sSTTL_DONE_STATUS;
    char sBrhSta[1 + 1];
	char sms_content[512] = {0};

	memset(sTmpTime, 0x00, sizeof(sTmpTime));
	sWT_STTL_STATUS = WT_STTL_STATUS;
    sIN_STTL_STATUS = IN_STTL_STATUS;
    sERR_OCC_STATUS = ERR_OCC_STATUS;
    sSTTL_DONE_STATUS = STTL_DONE_STATUS;

	while (1)
    {
        /* connect to database  */
        nReturnCode = DbsConnect();
        if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsConnect error, %d.", nReturnCode);
            DbsDisconnect();
            sleep(5);
            continue;
        }
		break;
	}

	while(1) {
        // ��ȡ��������
		memset(&dbtbl_date_inf, 0, sizeof(tbl_date_inf_def));
		HtMemcpy(dbtbl_date_inf.inter_brh_code, inter_brh_code, 6);
        nReturnCode = DbsTbldateInf(DBS_SELECT, &dbtbl_date_inf);      /*��ȡ��������*/
        if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "select from tbl_date_inf err sqlcode[%d].", nReturnCode);
            sleep(3);
            continue;
        }
	    break;
	}

	memset(sSettlmtDate, 0, 9);
    HtMemcpy(sSettlmtDate, dbtbl_date_inf.stoday, 8);
    while(1) {
        // �жϵ�ǰ�������ڵ����������Ƿ����������ر�
	    memset(&dbtbl_mission_inf, 0x00, sizeof(dbtbl_mission_inf));
		HtMemcpy(dbtbl_mission_inf.inter_brh_code, inter_brh_code, 10);
        HtMemcpy(dbtbl_mission_inf.settlmt_date, sSettlmtDate, 8);
	    nReturnCode = DbsTblmissionInf(DBS_SELECT, &dbtbl_mission_inf);       
		if(!nReturnCode) 
		{
            sleep(3);
			continue;
		}
		break;
    }
				
    // ��ȡ������ִ�е���������״̬mission_status������3������״̬�����񼶱�
    memset(&dbtbl_mission_inf, 0x00, sizeof(dbtbl_mission_inf));
    HtMemcpy(dbtbl_mission_inf.inter_brh_code, inter_brh_code, 10);
    HtMemcpy(dbtbl_mission_inf.settlmt_date, sSettlmtDate, 8);
    dbtbl_mission_inf.mission_status = STTL_DONE_STATUS; 
    DbsTblmissionInf(DBS_CURSOR3, &dbtbl_mission_inf);
    DbsTblmissionInf(DBS_OPEN3, &dbtbl_mission_inf);
    while (1)  {
		memset(&dbtbl_mission_inf, 0x00, sizeof(dbtbl_mission_inf));
		nReturnCode = DbsTblmissionInf(DBS_FETCH3, &dbtbl_mission_inf);         
        if (nReturnCode && nReturnCode != DBS_NOTFOUND)
        {
		    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "select ht_tbl_mission_inf err sqlcode[%d].", nReturnCode);
		    DbsTblmissionInf(DBS_CLOSE3, &dbtbl_mission_inf);
            return -1;
        }
		// ����������ִ�����
	    else if(nReturnCode == DBS_NOTFOUND) 
	    {
            // �����������ڿ��Ʊ�
		    dbtbl_date_inf.status = STTL_DONE_STATUS;
		    nReturnCode = DbsTbldateInf(DBS_UPDATE2, &dbtbl_date_inf);      
            if (nReturnCode)
            {
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "update tbl_date_inf err sqlcode[%d].", nReturnCode);
                return -1;
            }
			DbsTblmissionInf(DBS_CLOSE3, &dbtbl_mission_inf);
		    DbsCommit();	
		    return 0;
	    }

		memset(gLogFile, 0, LOG_NAME_LEN_MAX);
		HtSprintf(gLogFile, "MisnMain_%s.log", dbtbl_mission_inf.mission_index);
	    // �������״̬1��ȴ�3s
	    if(1 == dbtbl_mission_inf.mission_status) {
			DbsTblmissionInf(DBS_CLOSE3, &dbtbl_mission_inf);
			HtMemcpy(dbtbl_mission_inf.inter_brh_code, inter_brh_code, 10);
            HtMemcpy(dbtbl_mission_inf.settlmt_date, sSettlmtDate, 8);
			dbtbl_mission_inf.mission_status = STTL_DONE_STATUS; 
			DbsTblmissionInf(DBS_CURSOR3, &dbtbl_mission_inf);
			DbsTblmissionInf(DBS_OPEN3, &dbtbl_mission_inf);
			sleep(3);
			continue;					
		}
		// �������״̬Ϊ2�򷢶��ű���
		else if(2 == dbtbl_mission_inf.mission_status) {
			DbsTblmissionInf(DBS_CLOSE3, &dbtbl_mission_inf);
			memset(sms_content, 0, sizeof(sms_content));
		    sprintf(sms_content, "����[%s]������[%s]ִ��ʧ�ܣ�", dbtbl_mission_inf.inter_brh_code, dbtbl_mission_inf.mission_index);
            nReturnCode = sendMobileMsg(sms_content);
		    if(nReturnCode) {
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "��������[%s]����ʧ��", sms_content);
				sleep(30);
			}
			sleep(10);
			DbsTblmissionInf(DBS_CLOSE3, &dbtbl_mission_inf);
			HtMemcpy(dbtbl_mission_inf.inter_brh_code, inter_brh_code, 10);
            HtMemcpy(dbtbl_mission_inf.settlmt_date, sSettlmtDate, 8);
			dbtbl_mission_inf.mission_status = STTL_DONE_STATUS; 
			DbsTblmissionInf(DBS_CURSOR3, &dbtbl_mission_inf);
			DbsTblmissionInf(DBS_OPEN3, &dbtbl_mission_inf);
			sleep(3);
			continue;
		}
		// �������״̬Ϊ0��ʼִ���������
		else if(0 == dbtbl_mission_inf.mission_status) {
			lTime = time(NULL);
			tTmLocal = localtime( &lTime);
			strftime(sTmpTime, sizeof(sTmpTime), "%H:%M:%S", tTmLocal);
			memcpy(dbtbl_mission_inf.start_time, sTmpTime, 40);
			dbtbl_mission_inf.mission_status = sIN_STTL_STATUS;
			memcpy(dbtbl_mission_inf.end_time, "Waiting...", 10);
			nReturnCode = DbsTblmissionInf(DBS_UPDATE, &dbtbl_mission_inf);
			if (nReturnCode)
			{
			    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "update tbl_mission_inf err[%d].", nReturnCode);
			    DbsTblmissionInf(DBS_CLOSE3, &dbtbl_mission_inf);
			    return -1;
			}
			DbsCommit();

			memset(gLogFile, 0, LOG_NAME_LEN_MAX);
			sprintf(gLogFile, "Task_%s.log", dbtbl_mission_inf.mission_index);
			memcpy(ext_inter_brh_code, dbtbl_mission_inf.inter_brh_code, sizeof(dbtbl_mission_inf.inter_brh_code));
			memset(lib_name, 0, sizeof(lib_name));
			memset(func_name, 0, sizeof(func_name));
			sprintf(lib_name, "%s/lib/libBTask_%s.so", getenv("BATCHHOME"), dbtbl_mission_inf.mission_index);
			sprintf(func_name, "Total_%s", dbtbl_mission_inf.mission_index);
			HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "ext_inter_brh_code[%s],ext_inter_brh_sta[%d]", ext_inter_brh_code, ext_inter_brh_sta);
			nTotalNum = funcProc(lib_name, func_name);
			memset(gLogFile, 0, LOG_NAME_LEN_MAX);
			HtSprintf(gLogFile, "MisnMain_%s.log", dbtbl_mission_inf.mission_index);
			if (nTotalNum < 0)
			{
			    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "get total number error, %d.", nTotalNum);
			    HtMemcpy(dbtbl_mission_inf . end_time, "error occur!", 12);
			    dbtbl_mission_inf.mission_status = sERR_OCC_STATUS;
			    nReturnCode = DbsTblmissionInf(DBS_UPDATE, &dbtbl_mission_inf);
			    if (nReturnCode)
			        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "update tbl_mission_inf err[%d].", nReturnCode);
			    DbsCommit();
			    DbsTblmissionInf(DBS_CLOSE3, &dbtbl_mission_inf);
				HtMemcpy(dbtbl_mission_inf.inter_brh_code, inter_brh_code, 10);
                HtMemcpy(dbtbl_mission_inf.settlmt_date, sSettlmtDate, 8);
			    dbtbl_mission_inf.mission_status = STTL_DONE_STATUS; 
			    DbsTblmissionInf(DBS_CURSOR3, &dbtbl_mission_inf);
			    DbsTblmissionInf(DBS_OPEN3, &dbtbl_mission_inf);
			    continue;
			}
			else if (nTotalNum == 0)
			{
			    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "get total number nTotalNum=[%d].", nTotalNum);

			    lTime = time(NULL);
			    tTmLocal = localtime( &lTime);
			    strftime(sTmpTime, sizeof(sTmpTime), "%H:%M:%S", tTmLocal);
			    HtMemcpy(dbtbl_mission_inf.end_time, sTmpTime, 40);
			    dbtbl_mission_inf.mission_status = sSTTL_DONE_STATUS;
			    nReturnCode = DbsTblmissionInf(DBS_UPDATE, &dbtbl_mission_inf);
			    if (nReturnCode) 
			        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "update tbl_mission_inf err[%d].", nReturnCode);
			    DbsCommit();
			    continue;
			}
			// ȡ�ܼ�¼����� 
			HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "this task child nTotalNum=[%d].", nTotalNum);
                       

			// ����ÿ���ӽ��̵��������������ļ����������������Ҫ���ӽ����� 
			if (nTotalNum % dbtbl_mission_inf.data_num == 0)
			    nChildNum = nTotalNum / dbtbl_mission_inf.data_num;
			else
			    nChildNum = nTotalNum / dbtbl_mission_inf.data_num + 1;
			HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "�ӽ�����=[%d].", nChildNum);
			dbtbl_mission_inf.child_num = nChildNum;
			nReturnCode = DbsTblmissionInf(DBS_UPDATE1, &dbtbl_mission_inf);
			if (nReturnCode)
			    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "update tbl_mission_inf err[%d].", nReturnCode);
			DbsCommit();
			memset(&dbtbl_mission_inf2, 0, sizeof(dbtbl_mission_inf2));
			HtMemcpy(dbtbl_mission_inf2.mission_index, dbtbl_mission_inf.mission_index, 4);
			dbtbl_mission_inf2.data_num = dbtbl_mission_inf.data_num;
			dbtbl_mission_inf2.commit_flag = dbtbl_mission_inf.commit_flag;
			dbtbl_mission_inf2.commit_num = dbtbl_mission_inf.commit_num;

			HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "begin nIstChildTb.");
			nReturnCode = nIstChildTb ( &dbtbl_mission_inf2, nChildNum, nTotalNum, sSettlmtDate);
			if (nReturnCode)
			{
			    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nIstChildTb mission_index=[%s] error[%d].", dbtbl_mission_inf2.mission_index, nReturnCode);
			    DbsTblmissionInf(DBS_CLOSE3, &dbtbl_mission_inf);
			    return -1;
			}
			else
			{
			    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nIstChildTb mission_index=[%s] completed.", dbtbl_mission_inf2.mission_index);
			}
			DbsTblmissionInf(DBS_CLOSE3, &dbtbl_mission_inf);
			HtMemcpy(dbtbl_mission_inf.inter_brh_code, inter_brh_code, 10);
            HtMemcpy(dbtbl_mission_inf.settlmt_date, sSettlmtDate, 8);
			dbtbl_mission_inf.mission_status = STTL_DONE_STATUS; 
			DbsTblmissionInf(DBS_CURSOR3, &dbtbl_mission_inf);
			DbsTblmissionInf(DBS_OPEN3, &dbtbl_mission_inf);
			sleep(3);
		}
	}
	DbsTblmissionInf(DBS_CLOSE3, &dbtbl_mission_inf);
	DbsDisconnect();
	return 0;
}


/* * ************************************************************************** */
/* FUNC:   int nIstChildTb ( tbl_mission_inf_def *pstMissionInf,             */
/*                              int nChildNum, int nTotalNum)                */
/* INPUT:  pstMissionInf��������Ϣ                                           */
/*         nChildNum: �ӽ�����                                               */
/*         nTotalNum���ܼ�¼��                                               */
/* OUTPUT: ��                                                                */
/* RETURN: 0: �ɹ�, ����: ʧ��                                               */
/* DESC:   ������ֳɼ����ӽ���������                                        */
/* * ************************************************************************** */
int nIstChildTb ( tbl_mission_inf_def *pstMissionInf, int nChildNum, int nTotalNum, char *sSettlmtDate)
{
    int rc;
    int i = 1;
    int j = 0;
    int nCmtNum = 0;
    int nChildBeginNum = 0;
    int nChildEndNum = 0;
    int nReturnCode = 0;
    char sFileRecord[RECORD_LEN_MAX + 1];
    tbl_child_inf_def dbtbl_child_inf1;
    tbl_mission_inf_def dbtbl_mission_inf1;
    /*   EXEC SQL BEGIN DECLARE SECTION; */
    int nBeginOffset = 0;
    int nEndOffset = 0;
    char mission_index[4 + 1];
    int child_id;
    int commit_flag;
    int commit_num;
    int child_begin_point;
    int child_end_point;
    int child_finish_point;
    int child_err_point;
    int child_status;
    int sWT_STTL_STATUS;
    int sIN_STTL_STATUS;
    int sERR_OCC_STATUS;
    int sSTTL_DONE_STATUS;
    /*   EXEC SQL END DECLARE SECTION; */

    sWT_STTL_STATUS = WT_STTL_STATUS;
    sIN_STTL_STATUS = IN_STTL_STATUS;
    sERR_OCC_STATUS = ERR_OCC_STATUS;
    sSTTL_DONE_STATUS = STTL_DONE_STATUS;

    while (i <= nChildNum)
    {
        /* ���ӽ��̿��Ʊ��в���һ����¼ */
        memset(mission_index, 0, 5);
        HtMemcpy( mission_index, pstMissionInf->mission_index, 4);
        child_id = i;
        commit_flag = pstMissionInf->commit_flag;
        commit_num = pstMissionInf->commit_num;
        child_begin_point = pstMissionInf->data_num * (i - 1) + 1;
        if( pstMissionInf->data_num * (i) > nTotalNum)
            child_end_point = nTotalNum;
        else
            child_end_point = pstMissionInf->data_num * (i);
        child_finish_point = child_begin_point - 1;
        child_err_point = child_begin_point;
        child_status = sWT_STTL_STATUS;
        memset(&dbtbl_child_inf1, 0, sizeof(dbtbl_child_inf1));
        HtMemcpy(dbtbl_child_inf1.inter_brh_code, ext_inter_brh_code, 10);
        HtMemcpy(dbtbl_child_inf1.settlmt_date, sSettlmtDate, 8);
        HtMemcpy(dbtbl_child_inf1.mission_index, pstMissionInf->mission_index, 4);
        dbtbl_child_inf1.child_id = child_id;
        dbtbl_child_inf1.commit_flag = commit_flag;
        dbtbl_child_inf1.commit_num = commit_num;
        dbtbl_child_inf1.child_begin_point = child_begin_point;
        dbtbl_child_inf1.child_end_point = child_end_point;
        dbtbl_child_inf1.child_finish_point = child_finish_point;
        dbtbl_child_inf1.child_err_point = child_err_point;
        dbtbl_child_inf1.child_status = child_status;


        /* add by wangc@20120526 for test */
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "inter_brh_code=[%s],settlmt_date=[%s],mission_index=[%s],child_id=[%d],commit_flag=[%d],commit_num=[%d],child_begin_point=[%d],child_end_point=[%d],child_finish_point=[%d],child_err_point=[%d],child_status=[%d]", dbtbl_child_inf1.inter_brh_code, dbtbl_child_inf1.settlmt_date, dbtbl_child_inf1.mission_index, dbtbl_child_inf1.child_id, dbtbl_child_inf1.commit_flag, dbtbl_child_inf1.commit_num, dbtbl_child_inf1.child_begin_point, dbtbl_child_inf1.child_end_point, dbtbl_child_inf1.child_finish_point, dbtbl_child_inf1.child_err_point, dbtbl_child_inf1.child_status);
        /* add by wangc@20120526 for test */



        nReturnCode = DbsTblchildInf(DBS_INSERT, &dbtbl_child_inf1);

        /*
          EXEC SQL insert into tbl_child_inf values
          (
          :ext_inter_brh_code,
          :sSettlmtDate,
          :mission_index,
          :child_id,
          :commit_flag,
          :commit_num,
          :child_begin_point,
          :child_end_point,
          :child_finish_point,
          :child_err_point,
          :child_status
          );
         */

        if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "insert into tbl_child_inf error=[%d].", nReturnCode);
            memset(&dbtbl_mission_inf1, 0x00, sizeof(dbtbl_mission_inf1));
            HtMemcpy(dbtbl_mission_inf1.inter_brh_code, ext_inter_brh_code, 10);
            HtMemcpy(dbtbl_mission_inf1.settlmt_date, sSettlmtDate, 8);
            HtMemcpy(dbtbl_mission_inf1.mission_index, mission_index, 4);
            dbtbl_mission_inf1.mission_status = sERR_OCC_STATUS;
            HtMemcpy(dbtbl_mission_inf1.end_time, "error occur!", 12);
            nReturnCode = DbsTblmissionInf(DBS_UPDATE2, &dbtbl_mission_inf1);
            /*
              EXEC SQL update tbl_mission_inf
              set mission_status = :sERR_OCC_STATUS,
              end_time = 'error occur!'
              where inter_brh_code = :ext_inter_brh_code
              and settlmt_date = :sSettlmtDate
              and mission_index = :mission_index;
             */
            if (nReturnCode)
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "update tbl_mission_inf err[%d].", nReturnCode);
            DbsCommit();
            return -1;
        }
        DbsCommit();
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "insert into tbl_child_inf success");
        i++;
    }
    return 0;
}/* end of ReadFile */

void vCTerminate()
{
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "vCTerminate");
    DbsDisconnect();
    exit(0);
}/* end of vCTerminate */

