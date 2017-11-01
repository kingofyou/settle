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

/* EXEC SQL INCLUDE sqlca; */

char gLogFile[LOG_NAME_LEN_MAX];

tbl_date_inf_def dbtbl_date_inf;

/* EXEC SQL BEGIN DECLARE SECTION; */
char ext_inter_brh_code[10 + 1];
int ext_inter_brh_sta;
/* EXEC SQL END DECLARE SECTION; */

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
    
	int nReturnCode = 0;
	int flag = 0;
	int i = 0;
	stInstDef stInst;
	memset(gLogFile, 0, LOG_NAME_LEN_MAX);
    HtSprintf(gLogFile, "SubMisnMain.log");

    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "childmain started.");

    sigset(SIGCLD, SIG_IGN);
    sigset(SIGHUP, SIG_IGN);

    /* 载入初始数据 */
    /* connect to database */
    nReturnCode = DbsConnect();
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsConnect error, %d.", nReturnCode);
        exit(-1);
    }
    memset(&gtBufChgRule, 0, sizeof(bciMBufChgInfDef));
    nReturnCode = BufChgLoad(BUF_CHG_USAGE_KEY, &gtBufChgRule);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "BufChgLoad error, %d.", nReturnCode);
        nReturnCode = DbsDisconnect();
        if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsDisconnect error, %d.", nReturnCode);
        }
        exit(-1);
    }
    memset(&gtIpcDftRule, 0, sizeof(stIpcDftRuleDef));
    nReturnCode = IpcDftLoad(BUF_CHG_USAGE_KEY, &gtIpcDftRule);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "IpcDftLoad error, %d.", nReturnCode);
        nReturnCode = DbsDisconnect();
        if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsDisconnect error, %d.", nReturnCode);
        }
        exit(-1);
    }
    memset(&ipcRuleInf, 0, sizeof(ipcRuleInf));
    nReturnCode = LoadRuleInf(&ipcRuleInf);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "LoadRuleInf error, %d.", nReturnCode);
        DbsDisconnect();
        exit(-1);
    }

    /* disconnect database */
    nReturnCode = DbsDisconnect();
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsDisconnect error, %d.", nReturnCode);
    }

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

		memset( (char *)&stInst, 0, sizeof(stInstDef));
        nReturnCode = nLoadInst(&stInst);
        if (nReturnCode)
        {
            /* disconnect to database */
            DbsDisconnect();
            sleep(5);
            continue;
        }
		break;
    }

    for(; i < stInst.inst_num; i++)
    {
		flag = fork();   
	    if(!flag) {
			HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "---------%s-------", stInst.inst_def[i].inter_brh_code);

			while(1) {
                runChiletask(stInst.inst_def[i].inter_brh_code);	
				DbsDisconnect();
		    }
		}			
	    flag = 0;
	}
	DbsDisconnect();
} /* end of main */

int runChiletask(char* inter_brh_code) {
    int rc;
    int i;
    int j = 0;
    int nChildCnt;
    int nCmtNum = 0;
    int nChildBeginNum = 0;
    int nChildEndNum = 0;
    int nChildId = 0;
    int nAppTotal = 0;
    int nAppCurr = 0;
    time_t lTime;
    struct tm *tTmLocal;
    char lib_name[256 + 1];
    char func_name[256 + 1];

    /*   EXEC SQL BEGIN DECLARE SECTION; */
    char sTmpTime[128];
    int nCountNum = 0;
    int nReturnCode = 0;
    int nBeginOffset = 0;
    int nEndOffset = 0;
    char settlmt_date[8 + 1];
    char mission_index[4 + 1];
    int child_id;
    int commit_flag;
    int commit_num;
    int child_begin_point;
    int child_end_point;
    int child_finish_point;
    int child_err_point;
    int child_status;
    int child_num;
    int sWT_STTL_STATUS;
    int sIN_STTL_STATUS;
    int sERR_OCC_STATUS;
    int sSTTL_DONE_STATUS;
    char sBrhSta[1 + 1];

    tbl_child_inf_def dbtbl_child_inf[MAXCHILDS];
    tbl_child_inf_def dbtbl_child_inf1;
    tbl_brh_info_def1 tbl_brh_info;
    tbl_mission_inf_def dbtbl_mission_inf;

    /*    EXEC SQL END DECLARE SECTION; */

    memset(sTmpTime, 0x00, sizeof(sTmpTime));
    memset( &dbtbl_date_inf, 0, sizeof(tbl_date_inf_def));

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
            sleep(60);
            continue;
        }
		break;
	}
          
    memset(&dbtbl_child_inf1, 0x00, sizeof(dbtbl_child_inf1));
    dbtbl_child_inf1.child_status = sWT_STTL_STATUS;
	memcpy(dbtbl_child_inf1.inter_brh_code, inter_brh_code, 10);
    nCountNum = DbsTblchildInf(DBS_SELECT3, &dbtbl_child_inf1);
	HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "----------%d----------", nCountNum);
    if (nCountNum == 0)
    {
        /* disconnect database */
        nReturnCode = DbsDisconnect();
        if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsDisconnect error, %d.", nReturnCode);
        }
        sleep(3);
        return 1;
    }
       
    DbsTblchildInf(DBS_CURSOR1, &dbtbl_child_inf1);
    DbsTblchildInf(DBS_OPEN1, &dbtbl_child_inf1);
    i = 0;
    while (1)
    {
            memset(settlmt_date, 0, 9);
            memset(mission_index, 0, 5);
            child_id = 0;
            commit_flag = 0;
            commit_num = 0;
            child_begin_point = 0;
            child_end_point = 0;
            child_finish_point = 0;
            child_err_point = 0;
            child_status = 0;
            memset(&dbtbl_child_inf1, 0x00, sizeof(dbtbl_child_inf1));
			/*
              EXEC SQL FETCH tbl_child_inf_cursor INTO
              :inter_brh_code,
              :settlmt_date,
              :mission_index,
              :child_id,
              :commit_flag,
              :commit_num,
              :child_begin_point,
              :child_end_point,
              :child_finish_point,
              :child_err_point,
              :child_status;
              HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__,__LINE__, "FETCH tbl_child_inf_cursor sqlca=[%d].", nReturnCode);
             */
            nReturnCode = DbsTblchildInf(DBS_FETCH1, &dbtbl_child_inf1);
			if (nReturnCode && nReturnCode != DBS_NOTFOUND) {
                return -1;
            }
			else if(nReturnCode == DBS_NOTFOUND)
			{
				break;
			}

            HtMemcpy(inter_brh_code, dbtbl_child_inf1.inter_brh_code, 10);
            HtMemcpy(settlmt_date, dbtbl_child_inf1.settlmt_date, 8);
            HtMemcpy(mission_index, dbtbl_child_inf1.mission_index, 4);
            child_id = dbtbl_child_inf1.child_id;
            commit_flag = dbtbl_child_inf1.commit_flag;
            commit_num = dbtbl_child_inf1.commit_num;
            child_begin_point = dbtbl_child_inf1.child_begin_point;
            child_end_point = dbtbl_child_inf1.child_end_point;
            child_finish_point = dbtbl_child_inf1.child_finish_point;
            child_err_point = dbtbl_child_inf1.child_err_point;
            child_status = dbtbl_child_inf1.child_status;

			memset(gLogFile, 0, LOG_NAME_LEN_MAX);
            HtSprintf(gLogFile, "SubMisnMain_%4.4s.%d.log", dbtbl_child_inf1.mission_index, child_id);

            /*
              EXEC SQL update tbl_child_inf
              set child_status = :sIN_STTL_STATUS
              where inter_brh_code = :inter_brh_code
              and settlmt_date = :settlmt_date
              and mission_index = :mission_index
              and child_id = :child_id;
              if ( nReturnCode)
              {
              HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__,__LINE__, "update tbl_child_inf err[%d], settlmt_date=[%s], mission_index=[%s], child_id=[%d].\n",nReturnCode, settlmt_date, mission_index, child_id);
              continue;
              }
              DbsCommit ();
             */
            /*
              HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__,__LINE__, "update tbl_child_inf ok.");
             */

            HtMemcpy(dbtbl_child_inf[i].inter_brh_code, inter_brh_code, 10);
            HtMemcpy(dbtbl_child_inf[i].settlmt_date, settlmt_date, 8);
            HtMemcpy(dbtbl_child_inf[i].mission_index, mission_index, 4);
            dbtbl_child_inf[i].child_id = child_id;
            dbtbl_child_inf[i].commit_flag = commit_flag;
            dbtbl_child_inf[i].commit_num = commit_num;
            dbtbl_child_inf[i].child_begin_point = child_begin_point;
            dbtbl_child_inf[i].child_end_point = child_end_point;
            dbtbl_child_inf[i].child_finish_point = child_finish_point;
            dbtbl_child_inf[i].child_err_point = child_err_point;
            dbtbl_child_inf[i].child_status = child_status;
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "%d child_begin_point[%d]", i, dbtbl_child_inf[i].child_begin_point);
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "%d child_end_point[%d]", i, dbtbl_child_inf[i].child_end_point);

            /*
              HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__,__LINE__, "i=[%d].", i);
             */

            i++;
        }
        /*
          EXEC SQL CLOSE tbl_child_inf_cursor;
         */
        DbsTblchildInf(DBS_CLOSE1, &dbtbl_child_inf1);
        memset(gLogFile, 0, LOG_NAME_LEN_MAX);
        HtSprintf(gLogFile, "SubMisnMain.log");

        nChildCnt = i;
        /*
          HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__,__LINE__, "nChildCnt = [%d].", nChildCnt);
         */

        /* disconnect database */
        nReturnCode = DbsDisconnect();
        if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsDisconnect error, %d.", nReturnCode);
        }

        i = 0;
        while (i < nChildCnt)
        {
            /* fork，为了使分配给子任务表的任务实现并行执行 */
            rc = fork();
            switch (rc)
            {
            case 0:
                memset(gLogFile, 0, LOG_NAME_LEN_MAX);
                HtSprintf(gLogFile, "SubMisnMain_%4.4s.%d.log", dbtbl_child_inf[i].mission_index, dbtbl_child_inf[i].child_id);

                /* connect to database */
                nReturnCode = DbsConnect();
                if (nReturnCode)
                {
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsConnect error, %d.", nReturnCode);
                    return -1;
                }
                memset(&dbtbl_child_inf1, 0x00, sizeof(dbtbl_child_inf1));
                HtMemcpy(dbtbl_child_inf1.inter_brh_code, dbtbl_child_inf[i].inter_brh_code, 10);
                HtMemcpy(dbtbl_child_inf1.settlmt_date, dbtbl_child_inf[i].settlmt_date, 8);
                HtMemcpy(dbtbl_child_inf1.mission_index, dbtbl_child_inf[i].mission_index, 4);
                dbtbl_child_inf1.child_status = sIN_STTL_STATUS;
                dbtbl_child_inf1.child_id = dbtbl_child_inf[i].child_id;
                nReturnCode = DbsTblchildInf(DBS_UPDATE, &dbtbl_child_inf1);
                /*
                  EXEC SQL update tbl_child_inf
                  set child_status = :sIN_STTL_STATUS
                  where inter_brh_code = :dbtbl_child_inf[i].inter_brh_code
                  and settlmt_date = :dbtbl_child_inf[i].settlmt_date
                  and mission_index = :dbtbl_child_inf[i].mission_index
                  and child_id = :dbtbl_child_inf[i].child_id;
                 */
                if (nReturnCode)
                {
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "update tbl_child_inf error, %d.", nReturnCode);
                    exit(-1);
                }
                DbsCommit();
                HtMemcpy(dbtbl_date_inf.inter_brh_code, dbtbl_child_inf[i].inter_brh_code, 10);
				memcpy(dbtbl_date_inf.stoday, dbtbl_child_inf[i].settlmt_date, 8);
                CalNewDate(dbtbl_date_inf.stoday, -1, dbtbl_date_inf.syesterday);
				CalNewDate(dbtbl_date_inf.stoday, 1, dbtbl_date_inf.snextdate);
//                nReturnCode = DbsTbldateInf(DBS_SELECT, &dbtbl_date_inf);
//                /*
//                  EXEC SQL select
//                  sToday,
//                  sYesterday,
//                  sNextdate,
//                  sStartdate,
//                  sForestartdate
//                  into
//                  :dbtbl_date_inf.sToday,
//                  :dbtbl_date_inf.sYesterday,
//                  :dbtbl_date_inf.sNextdate,
//                  :dbtbl_date_inf.sStartdate,
//                  :dbtbl_date_inf.sForestartdate
//                  from tbl_date_inf
//                  where inter_brh_code = :dbtbl_child_inf[i].inter_brh_code;
//                 */
//                if (nReturnCode)
//                {
//                    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "select from tbl_date_inf err [%d].\n", nReturnCode);
//                }
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "dbtbl_date_inf.sToday = [%s].", dbtbl_date_inf.stoday);
                memset(&tbl_brh_info, 0x00, sizeof(tbl_brh_info));
                memset(sBrhSta, 0, sizeof(sBrhSta));
                HtMemcpy(tbl_brh_info.brh_id, dbtbl_child_inf[i].inter_brh_code, 10);
                nReturnCode = DbsTblbrhInf(DBS_SELECT, &tbl_brh_info);
                HtMemcpy(sBrhSta, tbl_brh_info.brh_level, 1);
                /*
                  EXEC SQL SELECT * FROM bth_brh_info
                  where brh_id = :dbtbl_child_inf[i].inter_brh_code;
                 */
                if (nReturnCode)
                {
                    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "select from tbl_brh_info err [%d].\n", nReturnCode);
                }

                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "dbtbl_child_inf[i].inter_brh_code = [%s].", dbtbl_child_inf[i].inter_brh_code);
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sBrhSta = [%s].", sBrhSta);

                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "child_id=[%d].", dbtbl_child_inf[i].child_id);
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "i=[%d].", i);
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mission_index[%s].", dbtbl_child_inf[i].mission_index);


                /* 计算子进程的开始和结束点 */
                if (dbtbl_child_inf[i].commit_flag == 1)
                    nChildBeginNum = dbtbl_child_inf[i].child_finish_point + 1;
                else
                    nChildBeginNum = dbtbl_child_inf[i].child_err_point;
                nChildEndNum = dbtbl_child_inf[i].child_end_point;

                /* 计算commit的次数 */
                if (( nChildEndNum - nChildBeginNum + 1) % dbtbl_child_inf[i].commit_num == 0)
                    nCmtNum = ( nChildEndNum - nChildBeginNum + 1) / dbtbl_child_inf[i].commit_num;
                else
                    nCmtNum = ( nChildEndNum - nChildBeginNum + 1) / dbtbl_child_inf[i].commit_num + 1;
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "commit num=[%d].", nCmtNum);
                while (j < nCmtNum)
                {
                    /* 计算每次commit的开始和结束点 */
                    nBeginOffset = nChildBeginNum + dbtbl_child_inf[i].commit_num * j;
                    if (nChildBeginNum + dbtbl_child_inf[i].commit_num * (j + 1) - 1 > nChildEndNum)
                        nEndOffset = nChildEndNum;
                    else
                        nEndOffset = nChildBeginNum + dbtbl_child_inf[i].commit_num * (j + 1) - 1;

                    /* 调用处理函数 */
                    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "begin pfMisnFun, nBeginOffset=[%d], nEndOffset=[%d].", nBeginOffset, nEndOffset);
                    memset(gLogFile, 0, LOG_NAME_LEN_MAX);
                    HtSprintf(gLogFile, "Task_%4.4s.%d.log", dbtbl_child_inf[i].mission_index, dbtbl_child_inf[i].child_id);

                    ext_inter_brh_sta = atoi(sBrhSta);
                    memset(ext_inter_brh_code, 0, 11);
                    HtMemcpy(ext_inter_brh_code, dbtbl_child_inf[i].inter_brh_code, 10);
                    rtrim(ext_inter_brh_code);
                    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nBeginOffset = [%d].", nBeginOffset);
                    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nEndOffset = [%d].", nEndOffset);

                    memset(lib_name, 0, sizeof(lib_name));
                    memset(func_name, 0, sizeof(func_name));
                    sprintf(lib_name, "%s/lib/libBTask_%s.so", getenv("BATCHHOME"), dbtbl_child_inf[i].mission_index);
                    sprintf(func_name, "Task_%s", dbtbl_child_inf[i].mission_index);
                    nReturnCode = funcSubProc(lib_name, func_name, nBeginOffset, nEndOffset);

                    memset(gLogFile, 0, LOG_NAME_LEN_MAX);
                    HtSprintf(gLogFile, "SubMisnMain_%4.4s.%d.log", dbtbl_child_inf[i].mission_index, dbtbl_child_inf[i].child_id);
                    if (nReturnCode)
                    {
                        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "pfMisnFun error, %d.", nReturnCode);
                        /* 更新子进程表，记录错误信息，nReturnCode为返回的错误点 */
                        DbsRollback();
                        dbtbl_child_inf1.child_err_point = nReturnCode;
                        dbtbl_child_inf1.child_status = sERR_OCC_STATUS;
                        nReturnCode = DbsTblchildInf(DBS_UPDATE, &dbtbl_child_inf1);
                        /*
                          EXEC SQL update tbl_child_inf
                          set child_err_point = :nReturnCode,
                          child_status = :sERR_OCC_STATUS
                          where inter_brh_code = :dbtbl_child_inf[i].inter_brh_code
                          and settlmt_date = :dbtbl_child_inf[i].settlmt_date
                          
                          and mission_index = :dbtbl_child_inf[i].mission_index
                          and child_id = :dbtbl_child_inf[i].child_id;
                         */
                        if (nReturnCode)
                        {
                            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "update tbl_child_inf error, %d.", nReturnCode);
                        }
                        memset(&dbtbl_mission_inf, 0x00, sizeof(dbtbl_mission_inf));
                        HtMemcpy(dbtbl_mission_inf.inter_brh_code, dbtbl_child_inf[i].inter_brh_code, 10);
                        HtMemcpy(dbtbl_mission_inf.settlmt_date, dbtbl_child_inf[i].settlmt_date, 8);
                        HtMemcpy(dbtbl_mission_inf.mission_index, dbtbl_child_inf[i].mission_index, 4);
                        dbtbl_mission_inf.mission_status = sERR_OCC_STATUS;
                        nReturnCode = DbsTblmissionInf(DBS_UPDATE3, &dbtbl_mission_inf);
                        /* 更新任务表，将状态置成错误
                          EXEC SQL update tbl_mission_inf
                          set mission_status = :sERR_OCC_STATUS
                          where inter_brh_code = :dbtbl_child_inf[i].inter_brh_code
                          and settlmt_date = :dbtbl_child_inf[i].settlmt_date
                          and mission_index = :dbtbl_child_inf[i].mission_index;
                         */
                        if (nReturnCode)
                        {
                            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "update tbl_child_inf error, %d.", nReturnCode);
                        }
                        DbsCommit();
                        nReturnCode = DbsDisconnect();
                        if (nReturnCode)
                        {
                            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsDisconnect error, %d.", nReturnCode);
                        }
                        exit(-1);
                    }

                    /* 记录完成点信息 */
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "nEndOffset=[%d].", nEndOffset);
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "mission_index=[%s].", dbtbl_child_inf[i].mission_index);
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "child_id=[%d].", dbtbl_child_inf[i].child_id);
                    dbtbl_child_inf1.child_finish_point = nEndOffset;
                    nReturnCode = DbsTblchildInf(DBS_UPDATE1, &dbtbl_child_inf1);
                    /*
                      EXEC SQL update tbl_child_inf
                      set child_finish_point = :nEndOffset
                      where inter_brh_code = :dbtbl_child_inf[i].inter_brh_code
                      and settlmt_date = :dbtbl_child_inf[i].settlmt_date
                      and mission_index = :dbtbl_child_inf[i].mission_index
                      and child_id = :dbtbl_child_inf[i].child_id;
                     */
                    if (nReturnCode)
                    {
                        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "update tbl_child_inf error, %d.", nReturnCode);
                    }
                    else
                    {
                        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "update tbl_child_inf success, %d.", nReturnCode);
                    }
                    DbsCommit();
                    j++;
                }
                dbtbl_child_inf1.child_status = sSTTL_DONE_STATUS;
                dbtbl_child_inf1.child_err_point = 0;
                nReturnCode = DbsTblchildInf(DBS_UPDATE, &dbtbl_child_inf1);
                /*
                  EXEC SQL update tbl_child_inf
                  set child_status = :sSTTL_DONE_STATUS,
                  child_err_point = 0
                  where inter_brh_code = :dbtbl_child_inf[i].inter_brh_code
                  and settlmt_date = :dbtbl_child_inf[i].settlmt_date
                  and mission_index = :dbtbl_child_inf[i].mission_index
                  and child_id = :dbtbl_child_inf[i].child_id;
                 */
                if (nReturnCode)
                {
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "update tbl_child_inf error, %d.", nReturnCode);
                }
                memset(&dbtbl_mission_inf, 0x00, sizeof(dbtbl_mission_inf));
                HtMemcpy(dbtbl_mission_inf.inter_brh_code, dbtbl_child_inf[i].inter_brh_code, 10);
                HtMemcpy(dbtbl_mission_inf.settlmt_date, dbtbl_child_inf[i].settlmt_date, 8);
                HtMemcpy(dbtbl_mission_inf.mission_index, dbtbl_child_inf[i].mission_index, 4);
                dbtbl_mission_inf.mission_status = sERR_OCC_STATUS;
                nReturnCode = DbsTblmissionInf(DBS_SELECT3, &dbtbl_mission_inf);
                /*
                  EXEC SQL select child_num into :child_num
                  from tbl_mission_inf
                  where inter_brh_code = :dbtbl_child_inf[i].inter_brh_code
                  and settlmt_date = :dbtbl_child_inf[i].settlmt_date
                  and mission_index = :dbtbl_child_inf[i].mission_index
                  for update;
                 */
                child_num = dbtbl_mission_inf.child_num;
                if (nReturnCode)
                {
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "update tbl_child_inf error, %d.", nReturnCode);
                }

                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "child_num=[%d].", child_num);

                if (child_num > 1)
                {
                    child_num--;
                    dbtbl_mission_inf.child_num = child_num;
                    nReturnCode = DbsTblmissionInf(DBS_UPDATE1, &dbtbl_mission_inf);
                    /*
                      EXEC SQL update tbl_mission_inf
                      set child_num = :child_num
                      where inter_brh_code = :dbtbl_child_inf[i].inter_brh_code
                      and settlmt_date = :dbtbl_child_inf[i].settlmt_date
                      and mission_index = :dbtbl_child_inf[i].mission_index;
                     */
                    if (nReturnCode)
                    {
                        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblmissionInf DBS_UPDATE1, %d.", nReturnCode);
                    }
                }
                else
                {
                    lTime = time(NULL);
                    tTmLocal = localtime( &lTime);
                    strftime(sTmpTime, sizeof(sTmpTime), "%H:%M:%S", tTmLocal);
                    dbtbl_mission_inf.child_num = 0;
                    dbtbl_mission_inf.mission_status = sSTTL_DONE_STATUS;
                    HtMemcpy(dbtbl_mission_inf.end_time, sTmpTime, sizeof(sTmpTime));
                    nReturnCode = DbsTblmissionInf(DBS_UPDATE2, &dbtbl_mission_inf);
                    /*
                      EXEC SQL update tbl_mission_inf
                      set mission_status = :sSTTL_DONE_STATUS,
                      child_num = 0,
                      end_time = :sTmpTime
                      where inter_brh_code = :dbtbl_child_inf[i].inter_brh_code
                      and settlmt_date = :dbtbl_child_inf[i].settlmt_date
                      and mission_index = :dbtbl_child_inf[i].mission_index;
                     */
                    if (nReturnCode)
                    {
                        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblmissionInf DBS_UPDATE1, %d.", nReturnCode);
                    }
                }
                DbsCommit();

                nReturnCode = DbsDisconnect();
                if (nReturnCode)
                {
                    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsDisconnect error, %d.", nReturnCode);
                }
                exit(0);
            case -1:
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fork error[%d][%s]", errno, strerror(errno));
                usleep(20000);
                continue;
            default:
                usleep(20000);
                i++;
                continue;
            }
        }

}
