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
 *  文 件 名: Task_7302.c
 *  功    能: 确认银行归集响应文件
 *  编程人员: bcw
 *  开发时间: 2017-03-09
 *  备    注:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include "batch.h"

extern char	gLogFile[LOG_NAME_LEN_MAX];
extern  tbl_date_inf_def dbtbl_date_inf;
extern	char ext_inter_brh_code[10 + 1];

#define BANK_STEP_NO                                     "08"

/*************************************************
*函数名称∶ Total_7302
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_7302()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_7302
*函数功能∶
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_7302 ( int nBeginOffset, int nEndOffset )
{
    int     nReturnCode;
    char    sDateTime[14+1];
    char    stlm_dt[8+1]; 
    char    *pstr;
    char    sFileFullName[200];
    char    sTmpFileName[100];
    int     nFileCount = 0;

    tbl_file_info_def   dbtbl_file_info;

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);

    /* 初始文件信息 */
    getSysTime(sDateTime);
    memset(&dbtbl_file_info, 0x00, sizeof(dbtbl_file_info)-1);
    memcpy(dbtbl_file_info.sett_date, stlm_dt, 8);
    memcpy(dbtbl_file_info.step_no, BANK_STEP_NO, 2);
    memcpy(dbtbl_file_info.batch_no, "02", 2);
    memcpy(dbtbl_file_info.file_type, "M", 1);
    memcpy(dbtbl_file_info.file_flag, "R", 1);
    memcpy(dbtbl_file_info.status, "4", 1);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date:[%s], file_type:[%s], file_flag:[%s], status:[%s]", 
	                        dbtbl_file_info.sett_date, dbtbl_file_info.file_type, dbtbl_file_info.file_flag, dbtbl_file_info.status);
	
	/* 游标 文件信息表获取文件名和银行编号 */
	nReturnCode = DbsTblFileInfo(DBS_CURSOR, &dbtbl_file_info);
	
	nReturnCode = DbsTblFileInfo(DBS_OPEN, &dbtbl_file_info);
	if (nReturnCode)
	{
	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN tbl_file_info_cur2 error [%d]", nReturnCode);
	    return -1;
	}
    
    while(1)
    {
        /* FETCH数据，获取文件名 */
	    memset(&dbtbl_file_info, 0, sizeof(dbtbl_file_info)-1);
	    nReturnCode = DbsTblFileInfo(DBS_FETCH, &dbtbl_file_info);
	    if (nReturnCode == DBS_NOTFOUND)
	    {
	        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_FETCH tbl_file_info_cur2 end. nFileCount=[%d]",nFileCount);
	        break;
	    }
	    else if (nReturnCode)
	    {
	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_FETCH tbl_file_info_cur2 error [%d].", nReturnCode);
	        DbsTblFileInfo(DBS_CLOSE, &dbtbl_file_info);
	        return -1;
	    }
	    nFileCount++;
	    RightTrim(dbtbl_file_info.bank_code);
	    RightTrim(dbtbl_file_info.file_name);
	    memcpy(sFileFullName, dbtbl_file_info.file_name, strlen(dbtbl_file_info.file_name));
    
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "正在确认银行[%s]归集响应文件文件:[%s]...", dbtbl_file_info.bank_code, sFileFullName);
    
        while (1)
        {
            /* 确认文件是否存在 */
            nReturnCode = access(sFileFullName, R_OK);
            if (nReturnCode != 0)
            {
                sleep(2);
                continue;
            }
            else
            {
                break;
            }
        }
    }
    DbsTblFileInfo(DBS_CLOSE, &dbtbl_file_info);

    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nFileCount:[%d]", nFileCount);
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_7302 Succ .");

    return 0;
}

/****************************** end of Task_7302.c ******************************/