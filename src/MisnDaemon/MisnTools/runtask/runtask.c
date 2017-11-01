/*****************************************************************************
 *  Copyright 2012, Shanghai Huateng Software Systems Co., Ltd.
 *  All right reserved.
 *
 *  THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF SHANGHAI HUATENG
 *  SOFTWARE SYSTEMS CO., LTD.  THE CONTENTS OF THIS FILE MAY NOT
 *  BE DISCLOSED TO THIRD PARTIES, COPIED OR DUPLICATED IN ANY FORM,
 *  IN WHOLE OR IN PART, WITHOUT THE PRIOR WRITTEN PERMISSION OF
 *  SHANGHAI HUATENG SOFTWARE SYSTEMS CO., LTD.
 *
 *  文 件 名: runtask.c
 *  功    能: 批量单任务调用程序
 *  编程人员: XISTON
 *  开发时间: 2012-03-02
 *  备    注:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/

#include "batch.h"

#define TIME_MISN 1
#define LINE_MISN 2

int 	ext_inter_brh_sta;
char    gLogFile[LOG_NAME_LEN_MAX];
char    ext_child_date[8 + 1];
char    ext_inter_brh_code[10 + 1];

tbl_date_inf_def dbtbl_date_inf;
stIpcDftRuleDef gtIpcDftRule;
bciMBufChgInfDef gtBufChgRule;
IPCRuleInf ipcRuleInf;

/* runtask 0102 101000 20160505 */
int main(int argc, char *argv[])
{
    int  sMFlag = 0;
    int  nIndex = 0;
    int  nReturnCode = 0;
    int  nTotalNum = 0;
    char cIsdebug[1];

    char sCfgCode[1 + 1];
    char sTaskCode[4 + 1];
    char cfgFileName[256];
    char lib_name[256 + 1];
    char func_name[256 + 1];
    char sTmpTime[128];

    memset(sCfgCode, 0x00, sizeof(sCfgCode));
    memset(sTaskCode, 0x00, sizeof(sTaskCode));
    memset(gLogFile, 0x00, sizeof(gLogFile));
    memset(ext_child_date, 0x00, sizeof(ext_child_date));
    memset(ext_inter_brh_code, 0x00, sizeof(ext_inter_brh_code));
    memset(&dbtbl_date_inf, 0x00, sizeof(dbtbl_date_inf));

    if (4 == argc)
    {
        memcpy(sTaskCode, argv[1], sizeof(sTaskCode) - 1);
        memcpy(ext_inter_brh_code, argv[2], sizeof(ext_inter_brh_code) - 1);
        memcpy(ext_child_date, argv[3], sizeof(ext_child_date) - 1);
        memcpy(dbtbl_date_inf.stoday, argv[3], sizeof(dbtbl_date_inf.stoday) - 1);

        CountDate(dbtbl_date_inf.stoday, 1, dbtbl_date_inf.snextdate);
        CountDate(dbtbl_date_inf.stoday, -1, dbtbl_date_inf.syesterday);

    }
    else if (2 == argc || 3 == argc)
    {
        if (3 == argc)
        {
            memcpy(sCfgCode, argv[2], 1);
        }
        else
        {
            memcpy(sCfgCode, "1", 1);
        }

        memcpy(sTaskCode, argv[1], sizeof(sTaskCode) - 1);
        sprintf(cfgFileName, "%s/config/runtaskdebug.cfg", getenv("BATCHHOME"));
        glbPflGetString(sCfgCode, "INTER_BRH_CODE", cfgFileName, ext_inter_brh_code);
        glbPflGetString(sCfgCode, "SETTLMT_DATE", cfgFileName, ext_child_date);


        memcpy(dbtbl_date_inf.stoday, ext_child_date, sizeof(dbtbl_date_inf.stoday) - 1);

        memset(ext_inter_brh_code + sizeof(ext_inter_brh_code), 0, 1);
        memset(ext_child_date + sizeof(ext_child_date), 0, 1);
    }
    else
    {
        //printf("Usage: %s TaskCode \n", argv[0]);
        //printf("       %s TaskCode CfgCode\n", argv[0]);
        //printf("       %s TaskCode InterBrhCode SettlMtDate\n", argv[0]);

        return 0;
    }

    sprintf(gLogFile, "%s_debug.log", sTaskCode);

    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Mission [%s]  [%s]  [%s]   Debug Starting...", sTaskCode, ext_inter_brh_code,  dbtbl_date_inf.stoday);

    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "connecting database...");

    while(1)
    {
        nReturnCode = DbsConnect();
        if (nReturnCode)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsConnect err.");
            DbsDisconnect();
            sleep(5);
            continue;
        }
        else
        {
            break;
        }
    }

    memset(&gtBufChgRule, 0x00, sizeof(bciMBufChgInfDef));
    nReturnCode = BufChgLoad(BUF_CHG_USAGE_KEY, &gtBufChgRule);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "BufChgLoad error, %d.", nReturnCode);
        nReturnCode = DbsDisconnect();
        if (nReturnCode)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsDisconnect error, %d.", nReturnCode);
        }

        return -1;
    }
    memset(&gtIpcDftRule, 0x00, sizeof(stIpcDftRuleDef));
    nReturnCode = IpcDftLoad(BUF_CHG_USAGE_KEY, &gtIpcDftRule);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "IpcDftLoad error, %d.", nReturnCode);
        nReturnCode = DbsDisconnect();
        if (nReturnCode)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsDisconnect error, %d.", nReturnCode);
        }

        return -1;
    }

    memset(&ipcRuleInf, 0x00, sizeof(ipcRuleInf));
    nReturnCode = LoadRuleInf(&ipcRuleInf);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "LoadRuleInf error, %d.", nReturnCode);
        DbsDisconnect();
        return -1;
    }

    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "---------------- Mission %s Debug start ---------------------",
           sTaskCode);


    memset(lib_name, 0, sizeof(lib_name));
    memset(func_name, 0, sizeof(func_name));
    sprintf(lib_name, "%s/lib/libBTask_%s.so", getenv("BATCHHOME"), sTaskCode);
    sprintf(func_name, "Total_%s", sTaskCode);
    nTotalNum = funcProc(lib_name, func_name);


    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Mission %s Total return num: [%d]",
           sTaskCode, nTotalNum);

    if (nTotalNum > 0)
    {
        memset(lib_name, 0, sizeof(lib_name));
        memset(func_name, 0, sizeof(func_name));
        sprintf(lib_name, "%s/lib/libBTask_%s.so", getenv("BATCHHOME"), sTaskCode);
        sprintf(func_name, "Task_%s", sTaskCode);
        nReturnCode = funcSubProc(lib_name, func_name, 1, nTotalNum);

        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Mission %s return code: [%d]",
               sTaskCode, nReturnCode);
    }

    if (nReturnCode != 0 || nTotalNum < 0)
    {
        DbsRollback();
        //printf("\n task exec failed, please read log\n\n");
    }
    else
    {
        DbsCommit();
        //printf("\n task exec succ \n\n");
    }
    DbsDisconnect();
    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "---------------- Mission %s Debug finished. -----------------", sTaskCode);
    return 0;
}


/************************************** end of runtask.c ********************************************/
