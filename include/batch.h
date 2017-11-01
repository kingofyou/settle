#ifndef _BATCH_H_
#define _BATCH_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <memory.h>
#include <signal.h>
#include <glob.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/times.h>
#include <assert.h>


#include "HtLog.h"
#include "DbsDef.h"
#include "DbsTbl.h"
#include "BufChg.h"
#include "DbsTblSett.h"
#include "Common.h"

#define BRH_STA_NO          "1"
#define BRH_STA_PROC        "2"
#define BRH_STA_ALL         "3"
#define BRH_STA_HANG        "4"
#define BRH_STA_CC          "6"
#define CHIEF_INST_CODE     "100093"

#define MISN_TYPE_PROC      "C"

#define TFRZ_TXN_NUM        "1131"

#define POS_UNFLAG          "0"
#define POS_FLAG            "1"

#define PR                  "0"
#define PK                  "1"

#define ERR_Y                "1"
#define    ERR_N                "0"

#define PFL_LINE_BUFFER_SIZE     1024    /* before PFL_MEMO_CHARACTER or '\n' */
#define PFL_FILE_NAME_LEN        1024

#define FAILURE                  -1        /* 错误返回值 */
#define SUCCESS                  0         /* 成功返回值 */

#define TRUE                     1
#define FALSE                    0

#define YES                      '1'
#define NO                       '0'

#define ___IsLeapYear(N) (((N % 4 == 0) && (N % 100 != 0)) || (N % 400 == 0)) /*是否为闰年*/
#define YEARDAYS(a) ___IsLeapYear(a)?366:365/*一年的天数*/
#define MON2DAYS(a) ___IsLeapYear(a)?29:28/*二月的天数*/
/*一个月的天数*/
#define RMONTHDAYS(a, b) ((a) == 2?(MON2DAYS(b)):((a) < 8 && (a) % 2 == 0 || (a) >= 8 && (a) % 2 != 0)?30:31)

stIpcDftRuleDef     gtIpcDftRule;
bciMBufChgInfDef    gtBufChgRule;
IPCRuleInf   ipcRuleInf;

typedef struct
{
    char       sTblName[60 + 1];
    char       sPartName[60 + 1];
    char       sTblSpcName[60 + 1];
    int        iTblOprFlag;
    int        iIntervalDays;
    int        iOprFlag;
} stPartInfDef;

typedef struct
{
    int     nPartInfN;
    stPartInfDef stPartInf[NMMaxPartInfN];
} stPartitionInfDef;
typedef struct
{
    int           sTblOprFlag;
    char       sObjectName[60 + 1];
    char       sParam_1[60 + 1];
    char       sParam_2[60 + 1];
    char       sParam_3[60 + 1];
    char       sParam_4[60 + 1];
} stOprInfDef;
typedef struct
{
    int     nOprInfN;
    stOprInfDef stOprInf[NMMaxOprInfN];
} stOperationInfDef;

#endif
