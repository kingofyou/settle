/*****************************************************************************/
/*                NCUP -- Shanghai Huateng Software System Inc.              */
/*****************************************************************************/
/* PROGRAM NAME: main.pc                                                     */
/* DESCRIPTIONS: main function for batch                                     */
/*****************************************************************************/
/*                        MODIFICATION LOG                                   */
/* DATE        PROGRAMMER     DESCRIPTION                                    */
/* 2008-10-20  harrison       initial                                        */
/*****************************************************************************/

#include "batch.h"

char    gLogFile[LOG_NAME_LEN_MAX];

char    ext_inter_brh_code[10 + 1];
char    ext_child_date[8 + 1];/*****************************************************************************/
/*                NCUP -- Shanghai Huateng Software System Inc.              */
/*****************************************************************************/
/* PROGRAM NAME: main.pc                                                     */
/* DESCRIPTIONS: main function for batch                                     */
/*****************************************************************************/
/*                        MODIFICATION LOG                                   */
/* DATE        PROGRAMMER     DESCRIPTION                                    */
/* 2008-10-20  harrison       initial                                        */
/*****************************************************************************/

#include "batch.h"

char    gLogFile[LOG_NAME_LEN_MAX];


char    ext_inter_brh_code[10 + 1];
char	ext_child_date[8 + 1];
int		ext_inter_brh_sta;

int main( int argc, char **argv)
{
    int rc;
    int i;
    int j = 0;
    int nIndex;
    int nChildCnt;
    int nCmtNum = 0;
    int nChildBeginNum = 0;
    int nChildEndNum = 0;
    int nChildId = 0;
    int nAppTotal = 0;
    int nAppCurr = 0;
    time_t  lTime;
    struct tm   *tTmLocal;

    char    sTmpTime[128];
    int     nCountNum = 0;
    int     nReturnCode = 0;
    int     nBeginOffset = 0;
    int     nEndOffset = 0;
    char    inter_brh_code[10 + 1];
    char    settlmt_date[8 + 1];
    char    mission_index[4 + 1];
    char    mission_date[8 + 1];
    char    mission_weekday[1 + 1];
    char    mission_hour[2 + 1];
    int     mission_date_adj;
    int     child_id;
    int     commit_flag;
    int     commit_num;
    int     child_begin_point;
    int     child_end_point;
    int     child_finish_point;
    int     child_err_point;
    int     child_status;
    int     child_num;
    int     sWT_STTL_STATUS;
    int     sIN_STTL_STATUS;
    int     sERR_OCC_STATUS;
    int     sSTTL_DONE_STATUS;
    int		empty_file;
    char    sBrhSta[1 + 1];
    char lib_name[256 + 1];
    char func_name[256 + 1];

    tbl_child_time_def    dbtbl_child_tm;
    tbl_child_time_def    dbtbl_child_time[MAXCHILDS];


    memset( sTmpTime, 0x00, sizeof(sTmpTime));
    memset( gLogFile, 0, LOG_NAME_LEN_MAX);
    HtSprintf (gLogFile, "TSubMisnMain.log");

    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "childmain started.");

    sWT_STTL_STATUS = WT_STTL_STATUS;
    sIN_STTL_STATUS = IN_STTL_STATUS;
    sERR_OCC_STATUS = ERR_OCC_STATUS;
    sSTTL_DONE_STATUS = STTL_DONE_STATUS;
    empty_file = 0;

    sigset(SIGCLD, SIG_IGN);
    sigset(SIGHUP, SIG_IGN);

    while(1)
    {
        nCountNum = 0;

        /* connect to database */
        nReturnCode = DbsConnect ();
        if (nReturnCode)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsConnect error, %d.", nReturnCode);
            sleep(60);
            continue;
        }

        memset(&dbtbl_child_tm, 0x00, sizeof(dbtbl_child_tm));
        dbtbl_child_tm.child_status = sWT_STTL_STATUS;
        nCountNum = XbsCldTime(DBS_SELECT1, &dbtbl_child_tm);
        if ( nCountNum == 0)
        {
            /* disconnect database */
            nReturnCode = DbsDisconnect ();
            if (nReturnCode)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsDisconnect error, %d.", nReturnCode);
            }
            sleep(10);
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "no data[%d]", nReturnCode);
            continue;
        }

        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "childmain started.");
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, " child_status[%d]", dbtbl_child_tm.child_status);

        memset(&dbtbl_child_tm, 0x00, sizeof(dbtbl_child_tm));
        dbtbl_child_tm.child_status = sWT_STTL_STATUS;

        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, " child_status[%d]", dbtbl_child_tm.child_status);

        XbsCldTime(DBS_CURSOR, &dbtbl_child_tm);

        i = 0;
        memset( (char *)&dbtbl_child_time, 0, sizeof(tbl_child_time_def));
        while(1)
        {

            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "AAAAAABBBBB");

            memset( mission_index, 0, 5);
            memset( mission_date, 0, 9);
            memset( mission_weekday, 0, 2);
            memset( mission_hour, 0, 3);
            mission_date_adj = 0;
            child_id = 0;
            commit_flag = 0;
            commit_num = 0;
            child_begin_point = 0;
            child_end_point = 0;
            child_finish_point = 0;
            child_err_point = 0;
            child_status = 0;
            nReturnCode = XbsCldTime(DBS_FETCH, &dbtbl_child_tm);
            if ( nReturnCode)
            {
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, " child_status[%d]", nReturnCode );
                break;
            }
            HtMemcpy(inter_brh_code, dbtbl_child_tm.inter_brh_code, 10);
            HtMemcpy(mission_index, dbtbl_child_tm.mission_index, 4);
            HtMemcpy(mission_date, dbtbl_child_tm.mission_date, 8);
            HtMemcpy(mission_weekday, dbtbl_child_tm.mission_weekday, 1);
            HtMemcpy(mission_hour, dbtbl_child_tm.mission_hour, 2);
            mission_date_adj = dbtbl_child_tm.mission_date_adj;
            child_id = dbtbl_child_tm.child_id;
            commit_flag = dbtbl_child_tm.commit_flag;
            commit_num = dbtbl_child_tm.commit_num;
            child_begin_point = dbtbl_child_tm.child_begin_point;
            child_end_point = dbtbl_child_tm.child_end_point;
            child_finish_point = dbtbl_child_tm.child_finish_point;
            child_err_point = dbtbl_child_tm.child_err_point;
            child_status = dbtbl_child_tm.child_status;


            memset( gLogFile, 0, LOG_NAME_LEN_MAX);

            HtSprintf (gLogFile, "TSubMisnMain_%s.%d.log", mission_index, child_id);

            HtMemcpy( dbtbl_child_time[i].inter_brh_code, inter_brh_code, 10);
            HtMemcpy( dbtbl_child_time[i].mission_index, mission_index, 4);
            HtMemcpy( dbtbl_child_time[i].mission_date, mission_date, 8);
            HtMemcpy( dbtbl_child_time[i].mission_weekday, mission_weekday, 1);
            HtMemcpy( dbtbl_child_time[i].mission_hour, mission_hour, 2);
            dbtbl_child_time[i].mission_date_adj = mission_date_adj;
            dbtbl_child_time[i].child_id = child_id;
            dbtbl_child_time[i].commit_flag = commit_flag;
            dbtbl_child_time[i].commit_num = commit_num;
            dbtbl_child_time[i].child_begin_point = child_begin_point;
            dbtbl_child_time[i].child_end_point = child_end_point;
            dbtbl_child_time[i].child_finish_point = child_finish_point;
            dbtbl_child_time[i].child_err_point = child_err_point;
            dbtbl_child_time[i].child_status = child_status;

            i++;

            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "AAAAAABBBBB[%d]", i);
        }

        memset( gLogFile, 0, LOG_NAME_LEN_MAX);
        HtSprintf (gLogFile, "TSubMisnMain.log");

        nChildCnt = i;
        XbsCldTime(DBS_CLOSE, &dbtbl_child_tm);

        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "AAAAAAAAAAA");

        /* disconnect database */
        nReturnCode = DbsDisconnect ();
        if (nReturnCode)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsDisconnect error, %d.", nReturnCode);
        }

        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "AAAAAAAAAAA[%d]", nChildCnt);

        i = 0;
        while( i < nChildCnt)
        {
            /* fork，为了使分配给子任务表的任务实现并行执行 */
            rc = fork();
            switch( rc)
            {
            case 0:
                memset( gLogFile, 0, LOG_NAME_LEN_MAX);
                HtSprintf (gLogFile, "TSubMisnMain_%s.%d.log", dbtbl_child_time[i].mission_index, dbtbl_child_time[i].child_id);

                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "AAAAAAAAAAA");

                /* 加上条件判断，看当前子任务是否是本应用执行的 /
                nChildId = dbtbl_child_time[i].child_id;
                nAppTotal = atoi( getenv("APP_TOTAL"));
                nAppCurr = atoi( getenv("APP_CURR"));
                if( nAppCurr != nChildId%nAppTotal)
                {
                    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__,__LINE__,
                            "该子进程不是本应用执行的, nAppCurr = [%d], nChildId = [%d].",
                            nAppCurr, nChildId);
                    exit(-1);
                }
                */

                /* connect to database */
                nReturnCode = DbsConnect ();
                if (nReturnCode)
                {
                    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsConnect error, %d.", nReturnCode);
                    exit(-1);
                }
                memset(&dbtbl_child_tm, 0x00, sizeof(dbtbl_child_tm));
                HtMemcpy(dbtbl_child_tm.inter_brh_code, dbtbl_child_time[i].inter_brh_code, 10);
                HtMemcpy(dbtbl_child_tm.mission_index, dbtbl_child_time[i].mission_index, 4);
                HtMemcpy(dbtbl_child_tm.mission_date, dbtbl_child_time[i].mission_date, 8);
                HtMemcpy(dbtbl_child_tm.mission_weekday, dbtbl_child_time[i].mission_weekday, 1);
                HtMemcpy(dbtbl_child_tm.mission_hour, dbtbl_child_time[i].mission_hour, 2);
                dbtbl_child_tm.mission_date_adj = dbtbl_child_time[i].mission_date_adj;
                dbtbl_child_tm.child_id = dbtbl_child_time[i].child_id;
                dbtbl_child_tm.child_status = sIN_STTL_STATUS;
                dbtbl_child_tm.child_err_point = 0;

                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "AAAAAAAAAAA");
                nReturnCode = XbsCldTime(DBS_UPDATE, &dbtbl_child_tm);
                if ( nReturnCode)
                {
                    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "update tbl_child_time error, %d.", nReturnCode);
                    exit(-1);
                }
                DbsCommit ();

                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "child_id=[%d].", dbtbl_child_time[i].child_id);
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "i=[%d].", i);

                if( nIndex == MAXMISNS)
                {
                    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,
                           "mission didn't found from Misn.h, mission_index = [%s].", dbtbl_child_time[i].mission_index);
                    exit(-1);
                }

                /* 计算子进程的开始和结束点 */
                if( dbtbl_child_time[i].commit_flag == 1)
                    nChildBeginNum = dbtbl_child_time[i].child_finish_point + 1;
                else
                    nChildBeginNum = dbtbl_child_time[i].child_err_point;
                nChildEndNum = dbtbl_child_time[i].child_end_point;

                /* 计算commit的次数 */
                if( ( nChildEndNum - nChildBeginNum + 1) % dbtbl_child_time[i].commit_num == 0)
                    nCmtNum = ( nChildEndNum - nChildBeginNum + 1) / dbtbl_child_time[i].commit_num;
                else
                    nCmtNum = ( nChildEndNum - nChildBeginNum + 1) / dbtbl_child_time[i].commit_num + 1;
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "commit的次数=[%d].", nCmtNum);
                while( j < nCmtNum)
                {
                    /* 计算每次commit的开始和结束点 */
                    nBeginOffset = nChildBeginNum + dbtbl_child_time[i].commit_num * j;
                    if( nChildBeginNum + dbtbl_child_time[i].commit_num * (j + 1) - 1 > nChildEndNum)
                        nEndOffset = nChildEndNum;
                    else
                        nEndOffset = nChildBeginNum + dbtbl_child_time[i].commit_num * (j + 1) - 1;

                    /* 调用处理函数 */
                    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "begin pfMisnFun, nBeginOffset=[%d], nEndOffset=[%d].", nBeginOffset, nEndOffset);
                    memset( gLogFile, 0, LOG_NAME_LEN_MAX);
                    HtSprintf (gLogFile, "Task_%s.%d.log", dbtbl_child_time[i].mission_index, dbtbl_child_time[i].child_id);

                    memset( ext_inter_brh_code, 0, 11);
                    memset(ext_child_date, 0, 9);
                    HtMemcpy( ext_inter_brh_code, dbtbl_child_time[i].inter_brh_code, 10);
                    HtMemcpy( ext_child_date, dbtbl_child_time[i].mission_date, 8);
                    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "ext_child_date[%s]", ext_child_date);
                    /* modify by zhangm 20120926
                    nReturnCode = gaMisns_t[nIndex].pfMisnFun( nBeginOffset, nEndOffset);
                    */
                    memset( lib_name , 0, sizeof(lib_name));
                    memset( func_name, 0, sizeof(func_name));
                    sprintf(lib_name, "%s/lib/libBTask_%s.so", getenv("BATCHHOME"),  dbtbl_child_time[i].mission_index);
                    sprintf(func_name, "Task_%s",  dbtbl_child_time[i].mission_index);
                    nReturnCode = funcSubProc(lib_name, func_name, nBeginOffset, nEndOffset);

                    memset( gLogFile, 0, LOG_NAME_LEN_MAX);
                    HtSprintf (gLogFile, "TSubMisnMain_%s.%d.log", dbtbl_child_time[i].mission_index, dbtbl_child_time[i].child_id);
                    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nReturnCode=[%d].", nReturnCode);
                    if(nReturnCode == DW_EMPTY_FILE)
                    {
                        empty_file = nReturnCode;
                    }
                    else if( nReturnCode)
                    {
                        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "pfMisnFun error,[%s] %d.", dbtbl_child_time[i].mission_index, nReturnCode);
                        /* 更新子进程表，记录错误信息，nReturnCode为返回的错误点 */
                        DbsRollback();
                        dbtbl_child_tm.child_status = sERR_OCC_STATUS;
                        dbtbl_child_tm.child_err_point = nReturnCode;
                        nReturnCode = XbsCldTime(DBS_UPDATE, &dbtbl_child_tm);
                        if ( nReturnCode)
                        {
                            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "update tbl_child_time error, %d.", nReturnCode);
                        }
                        DbsCommit ();
                        nReturnCode = DbsDisconnect ();
                        if (nReturnCode)
                        {
                            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsDisconnect error, %d.", nReturnCode);
                        }
                        exit(0);
                    }

                    /* 记录完成点信息 */
                    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "nEndOffset=[%d].", nEndOffset);
                    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "mission_index=[%s].", dbtbl_child_time[i].mission_index);
                    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "child_id=[%d].", dbtbl_child_time[i].child_id);
                    dbtbl_child_tm.child_finish_point = nEndOffset;
                    nReturnCode = XbsCldTime(DBS_UPDATE1, &dbtbl_child_tm);
                    if ( nReturnCode)
                    {
                        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "update tbl_child_time error, %d.", nReturnCode);
                    }
                    else
                    {
                        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "update tbl_child_time success, %d.", nReturnCode);
                    }
                    DbsCommit ();
                    j++;
                }

                if(empty_file == DW_EMPTY_FILE)
                {
                    dbtbl_child_tm.child_status = DW_EMPTY_FILE;
                }
                else
                {
                    dbtbl_child_tm.child_status = sSTTL_DONE_STATUS;
                }
                dbtbl_child_tm.child_err_point = 0;
                nReturnCode = XbsCldTime(DBS_UPDATE, &dbtbl_child_tm);
                if ( nReturnCode)
                {
                    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "update tbl_child_time error, %d.", nReturnCode);
                }

                DbsCommit ();

                nReturnCode = DbsDisconnect ();
                if (nReturnCode)
                {
                    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsDisconnect error, %d.", nReturnCode);
                }
                exit(0);
            default:
                i++;
                continue;
            }
        }
        sleep(10);
    }

} /* end of main */

