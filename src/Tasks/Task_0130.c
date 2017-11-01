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
 *  文 件 名: Task_0130.c
 *  功    能: 确认风控流水文件
 *  编程人员: HELU
 *  开发时间: 2016-12-18
 *  备    注:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include "batch.h"

extern char	gLogFile[LOG_NAME_LEN_MAX];
extern  tbl_date_inf_def dbtbl_date_inf;
extern	char ext_inter_brh_code[10 + 1];

/*************************************************
*函数名称∶ Total_0130
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_0130()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_0130
*函数功能∶
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_0130( int nBeginOffset, int nEndOffset )
{
    int		iRet = 0;
    char 	sFlag[1 + 1] = {0};
    char    *pstr;
    char	onlinepay_file_name[200] = {0};
    char	share_file_name[200] = {0};
    char	ok_file_name[200] = {0};
    char    sCommand[256] = {0};
    struct stat stat_buf_share;
    struct stat stat_buf_online;
    stExtnFileRegDef StExtnFileReg;

    /* 初始共享目录文件信息 */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    memcpy(StExtnFileReg.host_name, "RISK", 9);
    memcpy(StExtnFileReg.src_file_name, "RISK_YYYYMMDD_XX.txt", 25);

    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);
    pstr = strstr (StExtnFileReg.src_file_name, "XX");
    if (pstr)
        memcpy (pstr, "01", 2);

    getShareFileFullName(&StExtnFileReg, share_file_name);

    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "正在等待网关风控文件:[%s]...", share_file_name);
    
    memset(ok_file_name, 0, sizeof(ok_file_name));

    /* 初始ok文件信息 */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    memcpy(StExtnFileReg.host_name, "RISK", 9);
    strcpy(StExtnFileReg.src_file_name, "RISK_YYYYMMDD_XX.ok");

    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);
    pstr = strstr (StExtnFileReg.src_file_name, "XX");
    if (pstr)
        memcpy (pstr, "01", 2);

    getShareFileFullName(&StExtnFileReg, ok_file_name);

    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "正在等待网关风控ok文件:[%s]...", ok_file_name);

    /* 初始本地文件信息 */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    memcpy(StExtnFileReg.host_name, "RISK", 9);
    memcpy(StExtnFileReg.src_file_name, "RISK_YYYYMMDD_XX.txt", 25);

    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);
    pstr = strstr (StExtnFileReg.src_file_name, "XX");
    if (pstr)
        memcpy (pstr, "01", 2);

    getExtnFileFullName(&StExtnFileReg, onlinepay_file_name);

    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "本地路径:[%s]...", onlinepay_file_name);
    
    while (1)
    {
        /* 确认文件是否存在 */
        iRet = access(ok_file_name, R_OK);
        if (iRet != 0)
        {
            sleep(2);
            continue;
        }
        else
        {
            break;
        }
    }
    
    while (1)
    {
        memset(sCommand, 0, sizeof(sCommand));
        sprintf(sCommand, "cp -f %s %s", share_file_name, onlinepay_file_name);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sCommand:[%s]...", sCommand);
        iRet = cmdSystem(sCommand);
        if (iRet != 0)
        {
            sleep(2);
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sCommand:[%s]...", sCommand);
            continue;
        }
        
        /* 确认文件是否存在 */
        iRet = access(onlinepay_file_name, R_OK);
        if (iRet != 0)
        {
            HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sleep");
            sleep(2);
            continue;
        }
        
        /* 如果目标文件大小为0，且源目标文件大小不为0，重新复制 */
        memset(&stat_buf_share, 0x00, sizeof(stat_buf_share));
        if(stat(share_file_name, &stat_buf_share) == -1)
        {
            HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sleep");
            sleep(2);
            continue;
        }

        memset(&stat_buf_online, 0x00, sizeof(stat_buf_online));
        if(stat(onlinepay_file_name, &stat_buf_online) == -1)
        {
            HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sleep");
            sleep(2);
            continue;
        }
        
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, " fileSize[%d][%d]", stat_buf_share.st_size, stat_buf_online.st_size);
        if(stat_buf_share.st_size != 0 && stat_buf_online.st_size == 0)
        {
            HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, " fileSize[%d][%d]", stat_buf_share.st_size, stat_buf_online.st_size);
            sleep(2);
            continue;
        }
    
        break;
    }


    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "网关风控文件已经到达");
    return 0;
}

/****************************** end of Task_0130.c ******************************/

