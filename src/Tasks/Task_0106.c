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
 *  文 件 名: Task_0106.c
 *  功    能: 确认兴业银行流水文件（网银支付）
 *  编程人员: wyl
 *  开发时间: 2017-03-27
 *  备    注:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include "batch.h"

extern char	gLogFile[LOG_NAME_LEN_MAX];
extern  tbl_date_inf_def dbtbl_date_inf;
extern	char ext_inter_brh_code[10 + 1];

void *GetFilename(char *p, char *out) 
{ 
    int x = strlen(p); 
    char sTmp[1024] = {0};
    strcpy(sTmp, p);
    char ch = '/'; 
    char *q = strrchr(sTmp,ch) + 1; 
    strcpy(out, q);
    return ; 
} 

//int GetKeyInf(stKeyInf *pKeyInf)
//{
//    int nReturnCode;
//    
//    ht_tbl_com_config_def comconfig;
//    tp_payway_ext_def tp_payway_ext;
//    tp_merc_paywayconf_def tp_merc_paywayconf;
//        
//    memset(&comconfig, 0x00, sizeof(comconfig));
//    strcpy(comconfig.key1,"200001");
//    strcpy(comconfig.key2,"CIBGATEWAY");
//    strcpy(comconfig.key3,"PAYWAYTYPE");
//    nReturnCode = DbsbthComConfig (DBS_SELECT, &comconfig);
//     if( nReturnCode )
//    {
//        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsbthComConfig DBS_SELECT[%d]", nReturnCode);
//        return nReturnCode;
//    }
//
//    CommonRTrim(comconfig.value1);  
//    CommonRTrim(comconfig.value2);  
//    CommonRTrim(comconfig.value3);
//    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "[%s][%s][%s]", comconfig.value1,comconfig.value2,comconfig.value3); 
//    
//    memset(&tp_payway_ext, 0x00, sizeof(tp_payway_ext));
//    memcpy(tp_payway_ext.payway_type,comconfig.value1,sizeof(tp_payway_ext.payway_type)-1);    
//    nReturnCode = DbsTpPaywayExt (DBS_SELECT, &tp_payway_ext);
//	if (nReturnCode)
//	{
//	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTpPaywayExt error [%d]", nReturnCode);
//	    return -1;
//	}	
//	CommonRTrim(tp_payway_ext.payway_code);
//	memset(&tp_merc_paywayconf,0x0,sizeof(tp_merc_paywayconf));
//	memcpy(tp_merc_paywayconf.payway_code,tp_payway_ext.payway_code,sizeof(tp_merc_paywayconf.payway_code)-1);
//	HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "payway_code[%s]", tp_merc_paywayconf.payway_code);
//	nReturnCode = DbsTpMercPaywayConf (DBS_SELECT, &tp_merc_paywayconf);    
//	if (nReturnCode)
//	{
//	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTpMercPaywayConf error [%d]", nReturnCode);
//	    return -1;
//	}
//	CommonRTrim(tp_merc_paywayconf.paying_merc_code);
//	HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "paying_merc_code[%s]", tp_merc_paywayconf.paying_merc_code);
//	
//    CommonRTrim(tp_merc_paywayconf.paying_merc_code);  
//    memcpy(pKeyInf->sMrchNo,tp_merc_paywayconf.paying_merc_code,sizeof(pKeyInf->sMrchNo)-1);
//    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "sMrchNo[%s]", pKeyInf->sMrchNo); 
//    return 0;
//}

/*************************************************
*函数名称∶ Total_0106
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_0106()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_0106
*函数功能∶
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_0106 ( int nBeginOffset, int nEndOffset )
{
    int		iRet = 0;
    int     i = 0;
    char 	sFlag[1 + 1] = {0};
    char    *pstr;
    char	cib_infile_name[256] = {0};
    char	cib_outfile_name[256] = {0};
    char    sOut[256] = {0};
    
    char	share_infile_name[256] = {0};
    char	share_outfile_name[256] = {0};
    char sFileRecord[RECORD_LEN_MAX + 1] = {0};
    char    sCommand[256] = {0};
	char    sCurMisnHr[2 + 1];
	char    sms_content[512] = {0};
	int nReturnCode = 0;
    glob_t globbuf;

    stExtnFileRegDef StExtnFileReg;
    struct stat stat_buf_share;
    struct stat stat_buf_cib;
    stKeyInf KeyInf;

    memset(&KeyInf, 0x00, sizeof(KeyInf));
    iRet = GetKeyInf(&KeyInf, "cib_xydf");
    if (iRet)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "GetKeyInf err.");
        return -1;
    }     
    /*获取网关支付入账回单*/
    /* 初始共享盘文件信息 */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    memcpy(StExtnFileReg.host_name, "CIB", 3); 
    /*网关支付支付流水文件：商户号_交易日期(yyyymmdd)_回单类型(3/4)_文件生成时间(yyyymmddhhmmss).txt*/
    sprintf(StExtnFileReg.src_file_name, "%s_YYYYMMDD_3.txt",KeyInf.sMrchNo);
    
    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);

    iRet = getShareFileFullName(&StExtnFileReg, share_infile_name);
    if (iRet != 0)
    {
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "getShareFileFullName: error src_file_name[%s]...", StExtnFileReg.src_file_name);
        return -1;
    }  
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "正在等待兴业银行对账文件:[%s]...", share_infile_name);
    
    /* 初始本地文件信息 */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    memcpy(StExtnFileReg.host_name, "CIB", 3);
    /*文件名：商户号_交易日期(yyyymmdd)_回单类型(3/4)_文件生成时间(yyyymmddhhmmss).txt*/
    sprintf(StExtnFileReg.src_file_name, "%s_YYYYMMDD_3.txt",KeyInf.sMrchNo);
    
    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);

    sprintf(cib_infile_name, "%s/%8.8s/%s/%s/",
            getenv("BATCH_FILE_PATH"),
            StExtnFileReg.stlm_dt,
            StExtnFileReg.inter_brh_code,
            StExtnFileReg.host_name);

    iRet = CheckDir(cib_infile_name);
    if (iRet)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "CheckDir err[%d][%s][%s]",
               iRet, strerror(errno), cib_infile_name);
        return -1;
    }

    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "本地对账文件:[%s]...", cib_infile_name);

    while (1)
    {
        iRet = glob (share_infile_name, GLOB_NOSORT, NULL, &globbuf);
        if(iRet && iRet != GLOB_NOMATCH)
        {
           HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "glob error [%d %s]", iRet, share_infile_name);
           globfree (&globbuf);
           return -1;
        }
        else if (iRet == GLOB_NOMATCH)
        {
			// 判断是否超时未获取到
			/* 判断时间是否达到定时的要求 */
			// 4点后还未下载到发短信报警
            memset(sCurMisnHr, 0, 2 + 1);
            nReturnCode = DbsTimeStamp(DBS_SELECT1, NULL, NULL, sCurMisnHr, NULL);
            if(nReturnCode)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "get time error [%d].", nReturnCode);               
                return -1;
            }
			if(memcmp("04", sCurMisnHr, 2) <= 0) {
                sleep(30);
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "本地对账文件:[%s]...", cib_infile_name);
                continue;
			}
			else {
                // 发短信
				memset(sms_content, 0, sizeof(sms_content));
				sprintf(sms_content, "超时未获取到兴业银行网关支付入账回单文件[%s]", share_infile_name);
                nReturnCode = sendMobileMsg(sms_content);
				if(nReturnCode) {
                    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "报警短信[%s]发送失败", sms_content);
					sleep(30);
					continue;
				}

				sleep(30);
			}    
        }
        else if(globbuf.gl_pathc != 1)
        {
			
			// 判断是否超时未获取到
			/* 判断时间是否达到定时的要求 */
			// 4点后还未下载到发短信报警
            memset(sCurMisnHr, 0, 2 + 1);
            nReturnCode = DbsTimeStamp(DBS_SELECT1, NULL, NULL, sCurMisnHr, NULL);
            if(nReturnCode)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "get time error [%d].", nReturnCode);               
                return -1;
            }
			if(memcmp("04", sCurMisnHr, 2) <= 0) {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "file num err [%d] [%d]", iRet, globbuf.gl_pathc);
                globfree (&globbuf);
                return -1;
			}
			else {
                // 发短信
				memset(sms_content, 0, sizeof(sms_content));
				sprintf(sms_content, "超时未获取到兴业银行网关支付入账回单文件[%s]", share_infile_name);
                nReturnCode = sendMobileMsg(sms_content);
				if(nReturnCode) {
                    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "报警短信[%s]发送失败", sms_content);
					sleep(30);
					continue;
				}

				sleep(30);
			}   
        }
        else
        {
             HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "file name [%s]", globbuf.gl_pathv[0]);
             memset(share_infile_name, 0, sizeof(share_infile_name));
             strcpy(share_infile_name, globbuf.gl_pathv[0]);
             
             memset(sOut, 0, sizeof(sOut));
             GetFilename(share_infile_name, sOut);
             strcat(cib_infile_name, sOut);
             break;    
        }
    }
    globfree (&globbuf);

    /*获取网关支付出账回单*/
    /* 初始共享盘文件信息 */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    memcpy(StExtnFileReg.host_name, "CIB", 3); 
    /*网关支付支付流水文件：商户号_交易日期(yyyymmdd)_回单类型(3/4)_文件生成时间(yyyymmddhhmmss).txt*/
    sprintf(StExtnFileReg.src_file_name, "%s_YYYYMMDD_4.txt",KeyInf.sMrchNo);
    
    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);

    iRet = getShareFileFullName(&StExtnFileReg, share_outfile_name);
    if (iRet != 0)
    {
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "getShareFileFullName: error src_file_name[%s]...", StExtnFileReg.src_file_name);
        return -1;
    }  
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "正在等待兴业银行对账文件:[%s]...", share_outfile_name);
    
    /* 初始本地文件信息 */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    memcpy(StExtnFileReg.host_name, "CIB", 3);
    /*文件名：商户号_交易日期(yyyymmdd)_回单类型(0/1)_文件生成时间(yyyymmddhhmmss).txt*/
    sprintf(StExtnFileReg.src_file_name, "%s_YYYYMMDD_4.txt",KeyInf.sMrchNo);
    
    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);

    sprintf(cib_outfile_name, "%s/%8.8s/%s/%s/",
            getenv("BATCH_FILE_PATH"),
            StExtnFileReg.stlm_dt,
            StExtnFileReg.inter_brh_code,
            StExtnFileReg.host_name);

    iRet = CheckDir(cib_outfile_name);
    if (iRet)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "CheckDir err[%d][%s][%s]",
               iRet, strerror(errno), cib_outfile_name);
        return -1;
    }

    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "本地对账文件:[%s]...", cib_outfile_name);

    while (1)
    {
        iRet = glob (share_outfile_name, GLOB_NOSORT, NULL, &globbuf);
        if(iRet && iRet != GLOB_NOMATCH)
        {
           HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "glob error [%d %s]", iRet, share_outfile_name);
           globfree (&globbuf);
           return -1;
        }
        else if (iRet == GLOB_NOMATCH)
        {
			// 判断是否超时未获取到
			/* 判断时间是否达到定时的要求 */
			// 4点后还未下载到发短信报警
            memset(sCurMisnHr, 0, 2 + 1);
            nReturnCode = DbsTimeStamp(DBS_SELECT1, NULL, NULL, sCurMisnHr, NULL);
            if(nReturnCode)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "get time error [%d].", nReturnCode);               
                return -1;
            }
			if(memcmp("04", sCurMisnHr, 2) <= 0) {
                sleep(2);
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "本地对账文件:[%s]...", cib_outfile_name);
                continue;
			}
			else {
                // 发短信
				memset(sms_content, 0, sizeof(sms_content));
				sprintf(sms_content, "超时未获取到兴业银行网关支付出账回单文件[%s]", share_outfile_name);
                nReturnCode = sendMobileMsg(sms_content);
				if(nReturnCode) {
                    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "报警短信[%s]发送失败", sms_content);
					sleep(2);
					continue;
				}

				sleep(2);
			}     
        }
        else if(globbuf.gl_pathc != 1)
        {
           // 判断是否超时未获取到
			/* 判断时间是否达到定时的要求 */
			// 4点后还未下载到发短信报警
            memset(sCurMisnHr, 0, 2 + 1);
            nReturnCode = DbsTimeStamp(DBS_SELECT1, NULL, NULL, sCurMisnHr, NULL);
            if(nReturnCode)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "get time error [%d].", nReturnCode);               
                return -1;
            }
			if(memcmp("04", sCurMisnHr, 2) <= 0) {
               HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "file num err [%d] [%d]", iRet, globbuf.gl_pathc);
               globfree (&globbuf);
               return -1;
			}
			else {
                // 发短信
				memset(sms_content, 0, sizeof(sms_content));
				sprintf(sms_content, "超时未获取到兴业银行网关支付出账回单文件[%s]", share_outfile_name);
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
             HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "file name [%s]", globbuf.gl_pathv[0]);
             memset(share_outfile_name, 0, sizeof(share_outfile_name));
             strcpy(share_outfile_name, globbuf.gl_pathv[0]);
             break;    
        }
    }
    globfree (&globbuf);
    
    /*拷贝到本地目录(网关支付入账回单)*/
	int shareinfilesize1 = 0;
	int shareinfilesize2 = 0;
	// 检查文件是否下载完整
	memset(&stat_buf_share, 0x00, sizeof(stat_buf_share));
	if(stat(share_infile_name, &stat_buf_share) == -1)
    {
        HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sleep");
        shareinfilesize1 = 0;
    }
	else {
		shareinfilesize1 = stat_buf_share.st_size;
	}
	sleep(2);
    while (1)
    {
		memset(&stat_buf_share, 0x00, sizeof(stat_buf_share));
		if(stat(share_infile_name, &stat_buf_share) == -1)
        {
            HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sleep");
            sleep(2);
            continue;
        }
		shareinfilesize2 = stat_buf_share.st_size;
		// 文件已完全下载
		if(shareinfilesize1 == shareinfilesize2) {
            // 将对账文件由共享目录下载到本地路径
		    memset(sCommand, 0, sizeof(sCommand));
			sprintf(sCommand, "cp -f %s %s", share_infile_name, cib_infile_name);
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sCommand:[%s]...", sCommand);
            iRet = cmdSystem(sCommand);
            if (iRet != 0)
            {
                sleep(2);
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sCommand:[%s]...", sCommand);
                continue;
            }

		    /* 确认文件是否存在 */
            iRet = access(cib_infile_name, R_OK);
            if (iRet != 0)
            {
                sleep(2);
                continue;
            }

		    /* 如果目标文件大小为0，且源目标文件大小不为0，重新复制 */   
            memset(&stat_buf_cib, 0x00, sizeof(stat_buf_cib));
            if(stat(cib_infile_name, &stat_buf_cib) == -1)
            {
                HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sleep");
                sleep(2);
                continue;
            }

            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, " fileSize[%d][%d]", stat_buf_share.st_size, stat_buf_cib.st_size);
            if(shareinfilesize1 != 0 && stat_buf_cib.st_size == 0)
            {
                HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, " fileSize[%d][%d]", stat_buf_share.st_size, stat_buf_cib.st_size);
                sleep(2);
                continue;
            }
        }
		else {
            shareinfilesize1 = shareinfilesize2;
			sleep(2);
            continue;
		}        
        break;
    } 

    /*拷贝到本地目录(网关支付出账回单)*/
	int shareoutfilesize1 = 0;
	int shareoutfilesize2 = 0;
	// 检查文件是否下载完整
	memset(&stat_buf_share, 0x00, sizeof(stat_buf_share));
	if(stat(share_outfile_name, &stat_buf_share) == -1)
    {
        HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sleep");
        shareinfilesize1 = 0;
    }
	else {
		shareinfilesize1 = stat_buf_share.st_size;
	}
	sleep(2);
    while (1)
    {
		memset(&stat_buf_share, 0x00, sizeof(stat_buf_share));
		if(stat(share_outfile_name, &stat_buf_share) == -1)
        {
            HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sleep");
            sleep(2);
            continue;
        }
		shareinfilesize2 = stat_buf_share.st_size;
		// 文件已完全下载
		if(shareinfilesize1 == shareinfilesize2) {
            // 将对账文件由共享目录下载到本地路径
		    memset(sCommand, 0, sizeof(sCommand));
			sprintf(sCommand, "cp -f %s %s", share_outfile_name, cib_outfile_name);
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sCommand:[%s]...", sCommand);
            iRet = cmdSystem(sCommand);
            if (iRet != 0)
            {
                sleep(2);
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sCommand:[%s]...", sCommand);
                continue;
            }

		    /* 确认文件是否存在 */
            iRet = access(cib_outfile_name, R_OK);
            if (iRet != 0)
            {
                sleep(2);
                continue;
            }

		    /* 如果目标文件大小为0，且源目标文件大小不为0，重新复制 */   
            memset(&stat_buf_cib, 0x00, sizeof(stat_buf_cib));
            if(stat(cib_outfile_name, &stat_buf_cib) == -1)
            {
                HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sleep");
                sleep(2);
                continue;
            }

            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, " fileSize[%d][%d]", stat_buf_share.st_size, stat_buf_cib.st_size);
            if(shareinfilesize1 != 0 && stat_buf_cib.st_size == 0)
            {
                HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, " fileSize[%d][%d]", stat_buf_share.st_size, stat_buf_cib.st_size);
                sleep(2);
                continue;
            }
        }
		else {
            shareinfilesize1 = shareinfilesize2;
			sleep(2);
            continue;
		}        
        break;
    } 
    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "兴业银行对账文件已到达.");
    return 0;
}

/****************************** end of Task_0104.c ******************************/
