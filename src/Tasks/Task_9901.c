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
 *  文 件 名: Task_9901.c
 *  功    能：更新清算日期
 *  编程人员:
 *  开发时间:
 *  备    注:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/

#include "batch.h"

/*EXEC SQL INCLUDE sqlca; */

extern	char	gLogFile[LOG_NAME_LEN_MAX];
extern  tbl_date_inf_def dbtbl_date_inf;
extern	char	ext_inter_brh_code[10 + 1];

/*************************************************
*函数名称∶ Total_9901
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1

***************************************************/
int Total_9901()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_9901
*函数功能∶更新清算日期
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败

***************************************************/
int Task_9901 ( int nBeginOffset, int nEndOffset )
{
    /*  EXEC SQL BEGIN DECLARE SECTION; */
    char	sToday[8 + 1];
    char	sChgDate[8 + 1];
	char    sTwodaysago[8 + 1];
    char	sYesterday[8 + 1];
    char	sNextdate[8 + 1];
    char	sStartdate[8 + 1];
    char	sForestartdate[8 + 1];
    char	sInterBrhCode[10 + 1];
    int nReturnCode = 0;
    /*    EXEC SQL END DECLARE SECTION;*/
    tbl_date_inf_def dbtbl_date_inf1;
    tbl_brh_info_def1 dbtbl_brh_info;
    memset( sInterBrhCode,	0, 11);
    memset( sToday, 		0, 9);
    memset( sChgDate, 		0, 9);
	memset( sTwodaysago, 	0, 9);
    memset( sYesterday, 	0, 9);
    memset( sNextdate, 		0, 9);
    memset( sStartdate, 	0, 9);
    memset( sForestartdate, 0, 9);

    HtMemcpy( sToday, dbtbl_date_inf.snextdate, 8);
	HtMemcpy( sTwodaysago, dbtbl_date_inf.syesterday, 8);
    HtMemcpy( sInterBrhCode, ext_inter_brh_code, 10);

    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "dbtbl_date_inf.stoday=[%s].", dbtbl_date_inf.stoday);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "dbtbl_date_inf.syesterday=[%s].", dbtbl_date_inf.syesterday);
	HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "dbtbl_date_inf.snextdate=[%s].", dbtbl_date_inf.snextdate);

    CalNewDate( &sToday, 1, &sNextdate);
    CalNewDate( &sToday, -1, &sYesterday);

	HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sToday=[%s].", sToday);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sYesterday=[%s].", sYesterday);
	HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sNextdate=[%s].", sNextdate);

    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sNextdate=[%s].", sNextdate);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sYesterday=[%s].", sYesterday);

    if( memcmp( sNextdate + 6, "01", 2) == 0)
    {
        HtMemcpy( sStartdate, sToday, 6);
        HtMemcpy( sStartdate + 6, "01", 2);

        CalNewDate( &sStartdate, -1, &sForestartdate);
    }
    else
    {
        HtMemcpy( sStartdate, sToday, 8);
        HtMemcpy( sForestartdate, sYesterday, 8);
    }

    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sInterBrhCode=[%s].", sInterBrhCode);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sToday=[%s].", sToday);
	HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sTwodaysago=[%s].", sTwodaysago);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sYesterday=[%s].", sYesterday);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sNextdate=[%s].", sNextdate);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sStartdate=[%s].", sStartdate);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sForestartdate=[%s].\n", sForestartdate);
    memset(&dbtbl_date_inf1, 0x00, sizeof(dbtbl_date_inf1));
    HtMemcpy(dbtbl_date_inf1.stoday, sToday, 8);
	HtMemcpy(dbtbl_date_inf1.stwodaysago, sTwodaysago, 8);
    HtMemcpy(dbtbl_date_inf1.syesterday, sYesterday, 8);
    HtMemcpy(dbtbl_date_inf1.snextdate, sNextdate, 8);
    HtMemcpy(dbtbl_date_inf1.sstartdate, sStartdate, 8);
    HtMemcpy(dbtbl_date_inf1.sforestartdate, sToday, 8);
    HtMemcpy(dbtbl_date_inf1.inter_brh_code, sInterBrhCode, 10);
	dbtbl_date_inf1.status = WT_STTL_STATUS;
    nReturnCode = DbsTbldateInf(DBS_UPDATE, &dbtbl_date_inf1);
    /*
    	EXEC SQL update tbl_date_inf set
    		sToday = :sToday,
    		stoday = :stoday,
    		sNextdate = :sNextdate,
    		sStartdate = :sStartdate
    		WHERE inter_brh_code = :sInterBrhCode;
    */
    if ( nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "update tbl_date_inf err sqlca=[%d].\n", nReturnCode);
        return -1;
    }
    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "update tbl_date_inf succ.");

    return 0;
}
/****************************** end of Task_9901.c ******************************/