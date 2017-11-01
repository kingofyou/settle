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
 *  文 件 名: Task_0109.c
 *  功    能: 确认富友对账流水文件
 *  编程人员: wyl
 *  开发时间: 2016-12-29
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
*函数名称∶ Total_0109
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_0109()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_0109
*函数功能∶
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_0109 ( int nBeginOffset, int nEndOffset )
{
    int		iRet = 0;
    char 	sFlag[1 + 1] = {0};
    char    *pstr;
    char	local_file_name[256] = {0};
    char	share_file_name[256] = {0};
    char    sCommand[256] = {0};
	char    sCmd[256] = {0};
	char    sFileName[256] = {0};
	int     nReturnCode = 0;
	char    sCurMisnHr[2 + 1];
	char    sms_content[512] = {0};

    stExtnFileRegDef StExtnFileReg;
    struct stat stat_buf_share;
    struct stat stat_buf_alipay;
    
    /* 初始共享文件信息 */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    //memcpy(StExtnFileReg.host_name, "ALPAY", 5);
	strcpy(StExtnFileReg.src_file_name, "fuiouFile_YYYYMMDD_TXN.txt");
	pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);

	iRet = getExtnFileFullName(&StExtnFileReg, sFileName);
    if (iRet != 0)
    {
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "getExtnFileFullName: error src_file_name[%s]...", StExtnFileReg.src_file_name);
        return -1;
    }

	tbl_key_cert_def KeyInf;
	memset(&KeyInf, 0x00, sizeof(KeyInf));
	strcpy(KeyInf.pay_way, "fuiou");
	strcpy(KeyInf.merc_type, "2");
	nReturnCode = DbsKeyCertInf(DBS_CURSOR, &KeyInf);
	if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsKeyCertInf cursor err. nReturnCode = [%d].", nReturnCode);
        return -1;
    }
    DbsKeyCertInf(DBS_OPEN, &KeyInf);
	while(1) {
        memset(&KeyInf, 0x00, sizeof(KeyInf));
        nReturnCode = DbsKeyCertInf(DBS_FETCH, &KeyInf);
		if(nReturnCode && nReturnCode != DBS_NOTFOUND) {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsKeyCertInf fetch err. nReturnCode = [%d].", nReturnCode);
            return -1;
		}
		else if(nReturnCode == DBS_NOTFOUND) {
		    break;
	    }    
        CommonRTrim(KeyInf.paying_merc_code); 
        /* 初始共享文件信息 */
		memset(local_file_name, 0, sizeof(local_file_name));
		memset(share_file_name, 0, sizeof(share_file_name));

        memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
        memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
        memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
        //memcpy(StExtnFileReg.host_name, "ALPAY", 5);
        sprintf(StExtnFileReg.src_file_name, "%s_YYYYMMDD_TXN.txt", KeyInf.paying_merc_code);

        pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
        if (pstr)
            memcpy (pstr, dbtbl_date_inf.stoday, 8);

        iRet = getShareFileFullName(&StExtnFileReg, share_file_name);
        if (iRet != 0)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "getShareFileFullName: error src_file_name[%s]...", StExtnFileReg.src_file_name);
            return -1;
        }
    
        /* 初始文件信息 */
        memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
        memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
        memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
        //memcpy(StExtnFileReg.host_name, "ALPAY", 5);
        sprintf(StExtnFileReg.src_file_name, "%s_YYYYMMDD_TXN.txt", KeyInf.paying_merc_code);

        pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
        if (pstr)
             memcpy (pstr, dbtbl_date_inf.stoday, 8);

        iRet = getExtnFileFullName(&StExtnFileReg, local_file_name);
        if (iRet != 0)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "getExtnFileFullName: error src_file_name[%s]...", StExtnFileReg.src_file_name);
            return -1;
        }
   
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "正在等待支付宝对账文件:[%s]...", local_file_name);

        while (1)
        {
            /* 确认文件是否存在 */
            iRet = access(share_file_name, R_OK);
            if (iRet != 0)
            {
		    	// 判断是否超时未获取到
		    	/* 判断时间是否达到定时的要求 */
			    // 4点后还未下载到发短信报警
                memset(sCurMisnHr, 0, 2 + 1);
                int nReturnCode = DbsTimeStamp(DBS_SELECT1, NULL, NULL, sCurMisnHr, NULL);
                if(nReturnCode)
                {
                    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "get time error [%d].", nReturnCode);               
                    return -1;
                }
		    	if(memcmp("04", sCurMisnHr, 2) <= 0) {
                    sleep(2);
			    	continue;
		    	}
		    	else {
                    // 发短信
				    memset(sms_content, 0, sizeof(sms_content));
				    sprintf(sms_content, "超时未获取到支付宝对账文件[%s]", share_file_name);
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
    
    	int sharefilesize1 = 0;
	    int sharefilesize2 = 0;
	    // 检查文件是否下载完整
	    memset(&stat_buf_share, 0x00, sizeof(stat_buf_share));
    	if(stat(share_file_name, &stat_buf_share) == -1)
        {
            HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sleep");
            sharefilesize1 = 0;
        }
    	else {
	    	sharefilesize1 = stat_buf_share.st_size;
    	}
	    sleep(2);
        while (1)
        {
		    memset(&stat_buf_share, 0x00, sizeof(stat_buf_share));
	    	if(stat(share_file_name, &stat_buf_share) == -1)
            {
                HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sleep");
                sleep(2);
                continue;
            }
	    	sharefilesize2 = stat_buf_share.st_size;
		    // 文件已完全下载
		    if(sharefilesize1 == sharefilesize2) {
                // 将对账文件由共享目录下载到本地路径
		        memset(sCommand, 0, sizeof(sCommand));
		    	sprintf(sCommand, "cp -f %s %s", share_file_name, local_file_name);
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sCommand:[%s]...", sCommand);
                iRet = cmdSystem(sCommand);
                if (iRet != 0)
                {
                    sleep(2);
                    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sCommand:[%s]...", sCommand);
                    continue;
                }

		        /* 确认文件是否存在 */
                iRet = access(local_file_name, R_OK);
                if (iRet != 0)
                {
                    sleep(2);
                    continue;
                }

		        /* 如果目标文件大小为0，且源目标文件大小不为0，重新复制 */   
                memset(&stat_buf_alipay, 0x00, sizeof(stat_buf_alipay));
                if(stat(local_file_name, &stat_buf_alipay) == -1)
                {
                    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sleep");
                    sleep(2);
                    continue;
                }

                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, " fileSize[%d][%d]", stat_buf_share.st_size, stat_buf_alipay.st_size);
                if(sharefilesize1 != 0 && stat_buf_alipay.st_size == 0)
                {
                    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, " fileSize[%d][%d]", stat_buf_share.st_size, stat_buf_alipay.st_size);
                    sleep(2);
                    continue;
                }
            }
	    	else {
			    sharefilesize1 = sharefilesize2;
			    sleep(2);
                continue;
		    }        
            break;
        }

		while(1) {
            memset(sCmd, 0, sizeof(sCmd));
            sprintf(sCmd, "cat %s >> %s", local_file_name, sFileName);
		    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sCmd[%s].",sCmd);
            iRet = cmdSystem(sCmd);
            if (iRet != 0)
            {
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sCmd:[%s]...", sCmd);
                continue;
            }
			else break;
		}
		HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "支付宝商户[%s]对账文件已到达.", KeyInf.paying_merc_code);
    }

    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "支付宝对账文件已到达.");
    return 0;
}

/****************************** end of Task_0104.c ******************************/
