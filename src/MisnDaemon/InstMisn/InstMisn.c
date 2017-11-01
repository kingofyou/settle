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

char ext_inter_brh_code[10 + 1];
int ext_inter_brh_sta = 0;
char gLogFile[LOG_NAME_LEN_MAX];
void vCTerminate();

int checkLiquStatus(char* sett_date, char* sInterBrhCode) {
	tbl_mission_inf_def dbtbl_mission_inf;
    int errflag = 0;
	
	memset((char *)&dbtbl_mission_inf, 0, sizeof(dbtbl_mission_inf));
    HtMemcpy(dbtbl_mission_inf.settlmt_date, sett_date, 8);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "settlmt_date:[%s].", dbtbl_mission_inf.settlmt_date);
	HtMemcpy(dbtbl_mission_inf.inter_brh_code, sInterBrhCode, 10);
	DbsTblmissionInf(DBS_CURSOR2, &dbtbl_mission_inf);
    int nReturnCode = DbsTblmissionInf(DBS_OPEN2, &dbtbl_mission_inf);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN HT_TBL_MISSION_INF error, %d.", nReturnCode);
        return nReturnCode;
    }
	while(1) {
        memset(&dbtbl_mission_inf, 0x00, sizeof(dbtbl_mission_inf));
        nReturnCode = DbsTblmissionInf(DBS_FETCH2, &dbtbl_mission_inf);
	    if (nReturnCode == DBS_NOTFOUND)
        {
            break;
        }
        else if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_FETCH HT_TBL_MISSION_INF error nReturnCode = [%d].", nReturnCode);
	        DbsTblmissionInf(DBS_CLOSE2, &dbtbl_mission_inf);
            return nReturnCode;
        }
		// 判断此子任务是否成功执行
		if(3 != dbtbl_mission_inf.mission_status) {
		    errflag = 1;
			HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date:%s Liquidation error.", sett_date);
		    break;
		}
			    //HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date:[%s],inter_brh_code:[%s],mission_index:[%s], mission_status:[%s], Liquidation success.",  dbtbl_mission_inf.settlmt_date,  dbtbl_mission_inf.inter_brh_code,  dbtbl_mission_inf.mission_index, dbtbl_mission_inf.mission_status);
	}
    DbsTblmissionInf(DBS_CLOSE2, &dbtbl_mission_inf);
	return errflag;
}
 
int main( int argc, char **argv)
{
    int rc;
    int i;
    int nReturnCode;
    int nTotalNum = 0;
    int nIndex;
    int nMisnCnt;
    struct tm *tTmLocal;

    tbl_mission_inf_def dbtbl_mission_inf;

    char sTmpTime[128];
    int nChildNum;
    int min_level;
    int count_num;
    char mission_index[4 + 1];
    char mission_name[40 + 1];
    int mission_level;
    int mission_status;
    int data_num;
    int child_num;
    int commit_flag;
    int commit_num;
    char sInterBrhCode[10 + 1];

    stInstDef stInst;
    tbl_date_inf_def dbtbl_date_inf;


    memset(sTmpTime, 0x00, sizeof(sTmpTime));
    memset( &dbtbl_date_inf, 0, sizeof(tbl_date_inf_def));
    memset(gLogFile, 0, LOG_NAME_LEN_MAX);
    HtSprintf(gLogFile, "InstMisn.log");
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "instmisn started.");

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

        memset( (char *)&stInst, 0, sizeof(stInstDef));
        nReturnCode = nLoadInst(&stInst);
        if (nReturnCode)
        {
            /* disconnect to database */
            DbsDisconnect();
            sleep(30);
            continue;
        }

        int errflag = 0;
		for( i = 0; i < stInst.inst_num; i++)
        {
            memset(sInterBrhCode, 0, sizeof(sInterBrhCode));

            HtMemcpy(sInterBrhCode, stInst.inst_def[i].inter_brh_code, 10);
            rtrim(sInterBrhCode);

            memset(&dbtbl_date_inf, 0x00, sizeof(dbtbl_date_inf));
            HtMemcpy(dbtbl_date_inf.inter_brh_code, sInterBrhCode, 10);
            nReturnCode = DbsTbldateInf(DBS_SELECT, &dbtbl_date_inf);
            if (nReturnCode)
            {
				i--;
                continue;
            }

            // 检查上一天的清算任务是否完成
			errflag = checkLiquStatus(dbtbl_date_inf.syesterday, sInterBrhCode);
			if(errflag != 1 && errflag != 0) return -1;
            else if(1 == errflag) break;
		}
        if(1 == errflag) {
			sleep(60);
			continue;
        }

		// 当前清算日期数据已成功清结算
		// 检查报表数据是否生成，没有则生成清结算报表
		nReturnCode = DbsLiquidationReport(DBS_SELECT);
        if (nReturnCode == 0)
        {
		    nReturnCode = DbsLiquidationReport(DBS_INSERT);
		    if(nReturnCode && nReturnCode != DBS_NOTFOUND) {
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "清算日期[%s], 报表生成失败. errorcode:[%d]", dbtbl_date_inf.syesterday, nReturnCode);
			    sleep(30);
	            continue;
		    }
		    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "清算日期[%s], 报表生成成功", dbtbl_date_inf.syesterday);
			DbsCommit();
		}

        for( i = 0; i < stInst.inst_num; i++)
        {
            memset(sInterBrhCode, 0, sizeof(sInterBrhCode));

            HtMemcpy(sInterBrhCode, stInst.inst_def[i].inter_brh_code, 10);
            rtrim(sInterBrhCode);

            memset(&dbtbl_date_inf, 0x00, sizeof(dbtbl_date_inf));
            HtMemcpy(dbtbl_date_inf.inter_brh_code, sInterBrhCode, 10);
            nReturnCode = DbsTbldateInf(DBS_SELECT, &dbtbl_date_inf);
            if (nReturnCode)
            {
                continue;
            }

            /*拷贝完ext_inter_brh_code，日志就会根据ext_inter_brh_code分类打印*/
            HtMemcpy(ext_inter_brh_code, stInst.inst_def[i].inter_brh_code, 10);
            count_num = 0;
            memset(&dbtbl_mission_inf, 0x00, sizeof(dbtbl_mission_inf));
            HtMemcpy(dbtbl_mission_inf.inter_brh_code, sInterBrhCode, 10);

            HtMemcpy(dbtbl_mission_inf.settlmt_date, dbtbl_date_inf.stoday, 8);
            nReturnCode = DbsTblmissionInf(DBS_SELECT, &dbtbl_mission_inf);

            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mission_type=[%s].", stInst.inst_def[i].brh_level);
            if (nReturnCode > 0)
            {
                continue;
            }
            else
            {
                nReturnCode = DbsInsertMisn(sInterBrhCode, dbtbl_date_inf.stoday, stInst.inst_def[i].brh_level);
                if (nReturnCode)
                {
                    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DbsInsertMisn sInterBrhCode=[%s] error.", sInterBrhCode);
                    DbsRollback();
                }
                else
                {
                    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DbsInsertMisn sInterBrhCode=[%s] completed.", sInterBrhCode);
                    DbsCommit();
                }
            }
        }
/*
		errflag = 0;
		for( i = 0; i < stInst.inst_num; i++)
        {
            memset(sInterBrhCode, 0, sizeof(sInterBrhCode));

            HtMemcpy(sInterBrhCode, stInst.inst_def[i].inter_brh_code, 10);
            rtrim(sInterBrhCode);

            memset( sCurMisnDt, 0, 8 + 1);
            memset( sCurMisnWd, 0, 1 + 1);
            memset( sCurMisnHr, 0, 2 + 1);
            memset( sCurMisnMt, 0, 2 + 1);
			memset( sett_date, 0, 2 + 1);

            nReturnCode = DbsTimeStamp(DBS_SELECT, sCurMisnDt, sCurMisnWd, sCurMisnHr, sCurMisnMt);
            if(nReturnCode)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "get time error [%d].", nReturnCode);
				errflag = 2;
                break;
            }
			
            CalNewDate(sCurMisnDt, -1, sett_date);
            // 检查当前清算日期的清算任务是否完成
			errflag = checkLiquStatus(sett_date, sInterBrhCode);
			if(errflag != 0) break;
		}
		if(errflag != 0) continue;

		// 当前清算日期数据已成功清结算
		// 检查报表数据是否生成，没有则生成清结算报表
		nReturnCode = DbsLiquidationReport(DBS_SELECT);
        if (nReturnCode > 0)
        {
                continue;
        }
        else
        {
		    nReturnCode = DbsLiquidationReport(DBS_INSERT);
		    if(nReturnCode && nReturnCode != DBS_NOTFOUND) {
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "清算日期[%s], 报表生成失败. errorcode:[%d]", dbtbl_date_inf.syesterday, nReturnCode);
			    sleep(10);
	            continue;
		    }
		    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "清算日期[%s], 报表生成成功", dbtbl_date_inf.syesterday);
		}
*/
        DbsDisconnect();
        /*200秒检测是否重新初始化*/
        sleep(1000);
    }
} /* end of main */

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

void vCTerminate()
{
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "vCTerminate");
    DbsDisconnect();
    exit(0);
}/* end of vCTerminate */