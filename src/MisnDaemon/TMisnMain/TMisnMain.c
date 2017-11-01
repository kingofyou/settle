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
char    ext_child_date[8 + 1];
int		ext_inter_brh_sta;

void    vCTerminate();

int main( int argc, char **argv)
{
    int     rc;
    int     nReturnCode;
    int     nTotalNum = 0;
    int     nIndex;
    int     nMisnCnt;
    time_t  lTime;
    struct 	tm   *tTmLocal;
    char    cTime[12];

    tbl_mission_time_def    dbtbl_mission_time;
    tbl_mission_time_def    dbtbl_misn_tm_tmp;
    tbl_child_time_def		dbtbl_child_tm;

    char    sTmpTime[128];
    int     nChildNum;
    int     min_level;
    int     count_num;
    char    mission_index[4 + 1];
    char    mission_name[200 + 1];
    char    mission_date[8 + 1];
    char    mission_weekday[1 + 1];
    char    mission_hour[2 + 1];
    char    sCurMisnDt[8 + 1];
    char    sCurMisnWd[1 + 1];
    char    sCurMisnHr[2 + 1];
    char    sCurMisnMt[2 + 1];
    int     mission_date_adj;
    int     mission_level;
    int     mission_status;
    int     data_num;
    int     child_num;
    int     commit_flag;
    int     commit_num;
    int     sWT_STTL_STATUS;
    int     sIN_STTL_STATUS;
    int     sERR_OCC_STATUS;
    int     sSTTL_DONE_STATUS;
    char    sSettlmtDate[8 + 1];
    char    sBrhSta[1 + 1];
    char lib_name[256 + 1];
    char func_name[256 + 1];

    memset( sTmpTime, 0x00, sizeof(sTmpTime));
    memset( gLogFile, 0, LOG_NAME_LEN_MAX);
    HtSprintf( gLogFile, "TMisnMain.log");
    HtLog( gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "tmisnmain started.");

    sWT_STTL_STATUS = WT_STTL_STATUS;
    sIN_STTL_STATUS = IN_STTL_STATUS;
    sERR_OCC_STATUS = ERR_OCC_STATUS;
    sSTTL_DONE_STATUS = STTL_DONE_STATUS;

    sigset(SIGHUP, SIG_IGN);

    if(signal(SIGTERM, vCTerminate) == SIG_ERR)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "sigset SIGTERM error, %d", errno);
        exit(-10);
    } /* end of if */

    while(1)
    {
        /* connect to database  */
        nReturnCode = DbsConnect ();
        if (nReturnCode)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsConnect error, %d.", nReturnCode);
            sleep(60);
            continue;
        }
        memset(&dbtbl_misn_tm_tmp, 0x00, sizeof(dbtbl_misn_tm_tmp));
        nReturnCode = XbsMisnTime(DBS_CURSOR, &dbtbl_misn_tm_tmp);
        if(nReturnCode)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "open tbl_mission_time error , %d.", nReturnCode);
            sleep(60);
            continue;
        }
        while(1)
        {
            memset( ext_inter_brh_code, 0, 11);
            memset( mission_index, 0, 4 + 1);
            memset( mission_name, 0, 200 + 1);
            memset( mission_date, 0, 8 + 1);
            memset( mission_weekday, 0, 1 + 1);
            memset( mission_hour, 0, 2 + 1);
            memset(&dbtbl_misn_tm_tmp, 0x00, sizeof(dbtbl_misn_tm_tmp));
            mission_date_adj = 0;
            data_num = 0;
            child_num = 0;
            commit_flag = 0;
            commit_num = 0;
            nReturnCode = XbsMisnTime(DBS_FETCH, &dbtbl_misn_tm_tmp);
            if(nReturnCode)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,
                       "FETCH tbl_mission_time_cursor error [%d].", nReturnCode);
                XbsMisnTime(DBS_CLOSE, &dbtbl_misn_tm_tmp);
                DbsDisconnect();
                sleep(60);
                break;
            }

            HtMemcpy(ext_inter_brh_code, dbtbl_misn_tm_tmp.inter_brh_code, 10);
            rtrim(ext_inter_brh_code);
            HtMemcpy(mission_index, dbtbl_misn_tm_tmp.mission_index, 4);
            HtMemcpy(mission_name, dbtbl_misn_tm_tmp.mission_name, 200);
            HtMemcpy(mission_date, dbtbl_misn_tm_tmp.mission_date, 8);
            HtMemcpy(mission_weekday, dbtbl_misn_tm_tmp.mission_weekday, 1);
            HtMemcpy(mission_hour, dbtbl_misn_tm_tmp.mission_hour, 2);
            mission_date_adj = dbtbl_misn_tm_tmp.mission_date_adj;
            data_num = dbtbl_misn_tm_tmp.data_num;
            child_num = dbtbl_misn_tm_tmp.child_num;
            commit_flag = dbtbl_misn_tm_tmp.commit_flag;
            commit_num = dbtbl_misn_tm_tmp.commit_num;

            /* 判断时间是否达到定时的要求 */
            memset( sCurMisnDt, 0, 8 + 1);
            memset( sCurMisnWd, 0, 1 + 1);
            memset( sCurMisnHr, 0, 2 + 1);
            memset( sCurMisnMt, 0, 2 + 1);

            nReturnCode = DbsTimeStamp(DBS_SELECT, sCurMisnDt, sCurMisnWd, sCurMisnHr, sCurMisnMt);
            if(nReturnCode)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,
                       "get time error [%d].", nReturnCode);
                XbsMisnTime(DBS_CLOSE, &dbtbl_misn_tm_tmp);
                DbsDisconnect();
                sleep(60);
                break;
            }

            memset( ext_child_date, 0x00, sizeof(ext_child_date) );
			CalNewDate(sCurMisnDt, -1, ext_child_date);

            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "ext_child_date[%s]", ext_child_date);

            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "定时任务定时匹配条件:sCurMisnHr:[%s],sCurMisnMt:[%s]", sCurMisnHr, sCurMisnMt);

            /* 判断日期 */
            if( memcmp( mission_date, "*", 1) != 0
                    && memcmp( mission_date, ext_child_date, 8) != 0)
                continue;

            /* 判断星期 */
            switch( atoi(sCurMisnWd))
            {
            case 1:
                if( memcmp( mission_weekday, "*", 1) != 0
                        && memcmp( mission_weekday, "7", 1) != 0)
                    continue;
            case 2:
                if( memcmp( mission_weekday, "*", 1) != 0
                        && memcmp( mission_weekday, "1", 1) != 0)
                    continue;
            case 3:
                if( memcmp( mission_weekday, "*", 1) != 0
                        && memcmp( mission_weekday, "2", 1) != 0)
                    continue;
            case 4:
                if( memcmp( mission_weekday, "*", 1) != 0
                        && memcmp( mission_weekday, "3", 1) != 0)
                    continue;
            case 5:
                if( memcmp( mission_weekday, "*", 1) != 0
                        && memcmp( mission_weekday, "4", 1) != 0)
                    continue;
            case 6:
                if( memcmp( mission_weekday, "*", 1) != 0
                        && memcmp( mission_weekday, "5", 1) != 0)
                    continue;
            case 7:
                if( memcmp( mission_weekday, "*", 1) != 0
                        && memcmp( mission_weekday, "6", 1) != 0)
                    continue;
            }

            /* 判断小时 */
            if( memcmp( mission_hour, "*", 1) != 0
                    && memcmp( mission_hour, sCurMisnHr, 2) != 0)
                continue;

            /* 判断分钟 */
            if(  mission_date_adj != 0
                    && mission_date_adj >= atoi(sCurMisnMt))
                continue;

            /* 符合定时条件，判断是否任务已拉起 */
            count_num = 0;
            memset(&dbtbl_child_tm, 0x00, sizeof(dbtbl_child_tm));
            HtMemcpy(dbtbl_child_tm.inter_brh_code, ext_inter_brh_code, 10);
            HtMemcpy(dbtbl_child_tm.mission_index, mission_index, 4);
            HtMemcpy(dbtbl_child_tm.mission_date, ext_child_date, 8);
            HtMemcpy(dbtbl_child_tm.mission_weekday, sCurMisnWd, 1);
            HtMemcpy(dbtbl_child_tm.mission_hour, sCurMisnHr, 2);
            dbtbl_child_tm.mission_date_adj = mission_date_adj;

            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "AAAAAAA");

            count_num = XbsCldTime(DBS_SELECT, &dbtbl_child_tm);
            if(count_num > 0)
            {
                continue;
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "count_num[%d]", count_num);
            }

            memset( gLogFile, 0, LOG_NAME_LEN_MAX);
            HtSprintf( gLogFile, "TMisnMain_%s.log", mission_index);

            if( nIndex == MAXMISNS)
            {
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,
                       "mission didn't found from TMisn.h, mission_index = [%s].",
                       mission_index);
                continue;
            }
            else
            {
                memset( gLogFile, 0, LOG_NAME_LEN_MAX);
                HtSprintf (gLogFile, "Task_%s.log", mission_index);
                memset( lib_name , 0, sizeof(lib_name));
                memset( func_name, 0, sizeof(func_name));
                sprintf(lib_name, "%s/lib/libBTask_%s.so", getenv("BATCHHOME"),  mission_index);
                sprintf(func_name, "Total_%s",  mission_index);
                nTotalNum = funcProc(lib_name, func_name);

                memset( gLogFile, 0, LOG_NAME_LEN_MAX);
                HtSprintf( gLogFile, "TMisnMain_%s.log", mission_index);
                if ( nTotalNum < 0)
                {
                    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "get total number error, nTotalNum=[%d].",
                           nTotalNum);
                    continue;
                }
                else if( nTotalNum == 0 )
                {
                    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "get total number nTotalNum=[%d].", nTotalNum);
                    continue;
                }
            }
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "总记录数=[%d].", nTotalNum);
            /* 取总记录数完毕 */

            /* 根据每个子进程导入的最大行数和文件总行数，计算出需要的子进程数 */
            if( nTotalNum % data_num == 0)
                nChildNum = nTotalNum / data_num;
            else
                nChildNum = nTotalNum / data_num + 1;
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "子进程数=[%d].", nChildNum);

            memset( (char *)&dbtbl_mission_time, 0, sizeof(dbtbl_mission_time));
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "子进程数=[%d].", nChildNum);
            HtMemcpy( dbtbl_mission_time.inter_brh_code, ext_inter_brh_code, 10);
            HtMemcpy( dbtbl_mission_time.mission_index, mission_index, 4);
            HtMemcpy( dbtbl_mission_time.mission_date, ext_child_date, 8);
            HtMemcpy( dbtbl_mission_time.mission_weekday, sCurMisnWd, 1);
            HtMemcpy( dbtbl_mission_time.mission_hour, sCurMisnHr, 2);
            dbtbl_mission_time.mission_date_adj = mission_date_adj;
            dbtbl_mission_time.data_num = data_num;
            dbtbl_mission_time.commit_flag = commit_flag;
            dbtbl_mission_time.commit_num = commit_num;

            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "begin nIstChildTm[%d].", nChildNum);
            nReturnCode = nIstChildTm ( &dbtbl_mission_time, nChildNum, nTotalNum, sSettlmtDate);
            if (nReturnCode)
            {
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nIstChildTm mission_index=[%s] error.", mission_index);
            }
            else
            {
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nIstChildTm mission_index=[%s] completed.", mission_index);
            }
        }

        DbsDisconnect();
    }

} /* end of main */

/*****************************************************************************/
/* FUNC:   int nIstChildTm ( tbl_mission_inf_def *pstMissionTime,             */
/*                              int nChildNum, int nTotalNum)                */
/* INPUT:  pstMissionTime：任务信息                                           */
/*         nChildNum: 子进程数                                               */
/*         nTotalNum：总记录数                                               */
/* OUTPUT: 无                                                                */
/* RETURN: 0: 成功, 其它: 失败                                               */
/* DESC:   将任务分成几个子进程来处理                                        */
/*****************************************************************************/
int nIstChildTm ( tbl_mission_time_def *pstMissionTime, int nChildNum, int nTotalNum, char *sSettlmtDate)
{
    int rc;
    int i = 1;
    int j = 0;
    int nCmtNum = 0;
    int nChildBeginNum = 0;
    int nChildEndNum = 0;
    char sFileRecord[RECORD_LEN_MAX + 1];

    int     nBeginOffset = 0;
    int 	nReturnCode = 0;
    int     nEndOffset = 0;
    char    mission_index[4 + 1];
    char    mission_date[8 + 1];
    char    mission_weekday[1 + 1];
    char    mission_hour[2 + 1];
    int     mission_date_adj;
    int        child_id;
    int        commit_flag;
    int        commit_num;
    int        child_begin_point;
    int        child_end_point;
    int        child_finish_point;
    int        child_err_point;
    int        child_status;
    int        sWT_STTL_STATUS;
    int        sIN_STTL_STATUS;
    int        sERR_OCC_STATUS;
    int        sSTTL_DONE_STATUS;
    tbl_child_time_def		dbtbl_child_tm_tmp;
    sWT_STTL_STATUS = WT_STTL_STATUS;
    sIN_STTL_STATUS = IN_STTL_STATUS;
    sERR_OCC_STATUS = ERR_OCC_STATUS;
    sSTTL_DONE_STATUS = STTL_DONE_STATUS;

    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nChildNum=[%d].", nChildNum);
    while( i <= nChildNum)
    {
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "vnChildNum=[%d].", nChildNum);
        /* 向子进程控制表中插入一条记录 */
        memset( mission_index, 0, 5);
        memset( mission_date, 0, 9);
        memset( mission_weekday, 0, 2);
        memset( mission_hour, 0, 3);
        mission_date_adj = 0;
        HtMemcpy( mission_index, pstMissionTime->mission_index, 4);
        HtMemcpy( mission_date, pstMissionTime->mission_date, 8);
        HtMemcpy( mission_weekday, pstMissionTime->mission_weekday, 1);
        HtMemcpy( mission_hour, pstMissionTime->mission_hour, 4);
        mission_date_adj = pstMissionTime->mission_date_adj;
        child_id = i;
        commit_flag = pstMissionTime->commit_flag;
        commit_num = pstMissionTime->commit_num;
        child_begin_point = pstMissionTime->data_num * (i - 1) + 1;
        if( pstMissionTime->data_num * (i) > nTotalNum)
            child_end_point = nTotalNum;
        else
            child_end_point = pstMissionTime->data_num * (i);
        child_finish_point = child_begin_point - 1;
        child_err_point = child_begin_point;
        child_status = sWT_STTL_STATUS;
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mission_index=[%s].", mission_index);
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "child_id=[%d].", child_id);

        memset(&dbtbl_child_tm_tmp, 0x00, sizeof(dbtbl_child_tm_tmp));
        HtMemcpy(dbtbl_child_tm_tmp.inter_brh_code, ext_inter_brh_code, 10);
        HtMemcpy(dbtbl_child_tm_tmp.mission_index, mission_index, 4);
        HtMemcpy(dbtbl_child_tm_tmp.mission_date, mission_date, 8);
        HtMemcpy(dbtbl_child_tm_tmp.mission_weekday, mission_weekday, 1);
        HtMemcpy(dbtbl_child_tm_tmp.mission_hour, mission_hour, 2);
        dbtbl_child_tm_tmp.mission_date_adj = mission_date_adj;
        dbtbl_child_tm_tmp.child_id = child_id;
        dbtbl_child_tm_tmp.commit_flag = commit_flag;
        dbtbl_child_tm_tmp.commit_num = commit_num;
        dbtbl_child_tm_tmp.child_begin_point = child_begin_point;
        dbtbl_child_tm_tmp.child_end_point = child_end_point;
        dbtbl_child_tm_tmp.child_finish_point = child_finish_point;
        dbtbl_child_tm_tmp.child_err_point = child_err_point;
        dbtbl_child_tm_tmp.child_status = child_status;

        nReturnCode = XbsCldTime(DBS_INSERT, &dbtbl_child_tm_tmp);
        if ( nReturnCode)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "insert into tbl_child_time error, %d.", nReturnCode);
            return -1;
        }
        DbsCommit ();
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "insert into tbl_child_time success");
        i++;
    }
    return 0;
}/* end of ReadFile */

void vCTerminate()
{
    HtLog( gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "vCTerminate");
    DbsDisconnect();
    exit(0);
}/* end of vCTerminate */

