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
 *  文 件 名: Task_0102.c
 *  功    能: 确认核心流水文件
 *  编程人员: HELU
 *  开发时间: 2016-12-13
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
*函数名称∶ Total_0102
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_0102()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_0102
*函数功能∶
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_0102 ( int nBeginOffset, int nEndOffset )
{
    int		iRet = 0;
    char 	sFlag[1 + 1] = {0};
    char    *pstr;
    char	host_file_name[200] = {0};
    char	share_file_name[200] = {0};
    char	ok_file_name[200] = {0};
    char    sCommand[256] = {0};
	char    sCurMisnHr[2 + 1];
	char    sms_content[512] = {0};
    
    stExtnFileRegDef StExtnFileReg;
    struct stat stat_buf_share;
    struct stat stat_buf_host;
    
    /* 初始文件信息 */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    memcpy(StExtnFileReg.host_name, "HOST", 4);
    memcpy(StExtnFileReg.src_file_name, "HOST_YYYYMMDD_XX_TXN.txt", 24);

    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);
    pstr = strstr (StExtnFileReg.src_file_name, "XX");
    if (pstr)
        memcpy (pstr, "01", 2);

    iRet = getShareFileFullName(&StExtnFileReg, share_file_name);
    if (iRet != 0)
    {
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "getExtnFileFullName: error src_file_name[%s]...", StExtnFileReg.src_file_name);
        return -1;
    }
    /* 初始ok文件信息 */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    memcpy(StExtnFileReg.host_name, "HOST", 4);
    memcpy(StExtnFileReg.src_file_name, "HOST_YYYYMMDD_XX_TXN.ok", 24);

    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);
    pstr = strstr (StExtnFileReg.src_file_name, "XX");
    if (pstr)
        memcpy (pstr, "01", 2);

    memset(ok_file_name, 0, sizeof(ok_file_name));
    iRet = getShareFileFullName(&StExtnFileReg, ok_file_name);
    if (iRet != 0)
    {
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "getShareFileFullName: error src_file_name[%s]...", StExtnFileReg.src_file_name);
        return -1;
    }
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "正在等待核心对账文件:[%s]ok文件[%s]...", share_file_name, ok_file_name);

    /* 初始本地文件信息 */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    memcpy(StExtnFileReg.host_name, "HOST", 4);
    memcpy(StExtnFileReg.src_file_name, "HOST_YYYYMMDD_XX_TXN.txt", 24);

    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);
    pstr = strstr (StExtnFileReg.src_file_name, "XX");
    if (pstr)
        memcpy (pstr, "01", 2);

    iRet = getExtnFileFullName(&StExtnFileReg, host_file_name);
    if (iRet != 0)
    {
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "getShareFileFullName: error src_file_name[%s]...", StExtnFileReg.src_file_name);
        return -1;
    }
    
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "本地路径:[%s]...", host_file_name);
    while (1)
    {
        /* 确认文件是否存在 */
        iRet = access(ok_file_name, R_OK);
        if (iRet != 0)
        {
			// 判断是否超时未获取到
			/* 判断时间是否达到定时的要求 */
			// 4点后还未下载到发短信报警
            memset(sCurMisnHr, 0, 2 + 1);
            int nReturnCode = DbsTimeStamp(DBS_SELECT1, NULL, NULL, sCurMisnHr, NULL);
            if(nReturnCode)
            {
                sleep(30);
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sleep:30[%s]...", ok_file_name);
                continue;
            }
			if(memcmp("04", sCurMisnHr, 2) <= 0) {
                sleep(2);
				continue;
			}
			else {
                // 发短信
				memset(sms_content, 0, sizeof(sms_content));
				sprintf(sms_content, "超时未获取到账务核心对账文件[%s]", ok_file_name);
                nReturnCode = sendMobileMsg(sms_content);
				if(nReturnCode) {
                    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "报警短信[%s]发送失败", sms_content);
					sleep(2);
					continue;
				}

				sleep(2);
			}              
        }
        else
        {
            break;
        }
    }
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "本地路径:[%s]...", host_file_name);
    
    while (1)
    {
        memset(sCommand, 0, sizeof(sCommand));
        sprintf(sCommand, "cp -f %s %s", share_file_name, host_file_name);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sCommand:[%s]...", sCommand);
        iRet = cmdSystem(sCommand);
        if (iRet != 0)
        {
            sleep(2);
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sCommand:[%s]...", sCommand);
            continue;
        }
        
        
        /* 确认文件是否存在 */
        iRet = access(host_file_name, R_OK);
        if (iRet != 0)
        {
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

        memset(&stat_buf_host, 0x00, sizeof(stat_buf_host));
        if(stat(host_file_name, &stat_buf_host) == -1)
        {
            HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sleep");
            sleep(2);
            continue;
        }
        
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, " fileSize[%d][%d]", stat_buf_share.st_size, stat_buf_host.st_size);
        if(stat_buf_share.st_size != 0 && stat_buf_host.st_size == 0)
        {
            HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, " fileSize[%d][%d]", stat_buf_share.st_size, stat_buf_host.st_size);
            sleep(2);
            continue;
        }
        
        break;
    }
    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "核心对账文件已到达.");
    return 0;
}

/****************************** end of Task_0104.c ******************************/
