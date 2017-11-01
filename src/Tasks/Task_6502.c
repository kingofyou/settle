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
 *  文 件 名: Task_6502.c
 *  功    能: 头寸信息表同步到头寸试算表
 *  编程人员: bcw
 *  开发时间: 2017-03-10
 *  备    注:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include <sys/socket.h>
#include "batch.h"
#include "Common.h"
#include "cJSON.h"
#include "EPay.h"

extern char	gLogFile[LOG_NAME_LEN_MAX];
extern  tbl_date_inf_def dbtbl_date_inf;
extern	char ext_inter_brh_code[10 + 1];


typedef struct
{
    char    stlm_dt[8+1];
    char    sTransDate[8+1];
    char    sTransTime[6+1];
	char    sMrchNo[8+1];
	char    sAcctNo[32+1];
    char    cny[3+1];
	double dAccountBalance;
	double dAvailableBalanc;
	double dBlockedBalance;
	double dControlledBalance;
} stEpayBalance;

/*************************************************
*函数名称∶ WriteFile
*函数功能∶
*输入参数∶ 生成流水文件
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
void WriteFile(stEpayBalance *psEpayBalance, FILE *fp)
{
    fprintf( fp, "%s%s", psEpayBalance->stlm_dt, "|");
    fprintf( fp, "%s%s", psEpayBalance->sTransDate, "|");
    fprintf( fp, "%s%s", psEpayBalance->sTransTime, "|");
    fprintf( fp, "%s%s", psEpayBalance->sMrchNo, "|");
    fprintf( fp, "%s%s", psEpayBalance->sAcctNo, "|");
    fprintf( fp, "%s%s", psEpayBalance->cny, "|");
    fprintf( fp, "%.2f%s", psEpayBalance->dAccountBalance, "|");
    fprintf( fp, "%.2f%s", psEpayBalance->dAvailableBalanc, "|");
    fprintf( fp, "%.2f%s", psEpayBalance->dBlockedBalance, "|");
    fprintf( fp, "%.2f%s", psEpayBalance->dControlledBalance, "|");
    fprintf( fp, "\r\n");
    fflush(fp);

    return;
}

int iInsertTblFileInfo(tbl_file_info_def   dbtbl_file_info)
{
    int iRet = 0;
    HtLog(gLogFile,HT_LOG_MODE_DEBUG,__FILE__,__LINE__,
        "view the data dbtbl_file_info [%s][%s][%s][%s][%s][%s][%s]",dbtbl_file_info.sett_date,dbtbl_file_info.bank_code,dbtbl_file_info.file_type,dbtbl_file_info.file_flag,dbtbl_file_info.step_no,dbtbl_file_info.batch_no,dbtbl_file_info.file_name);
   
    iRet = DbsTblFileInfo(DBS_SELECT, &dbtbl_file_info);
    if(iRet == DBS_NOTFOUND)
    {
        iRet = DbsTblFileInfo(DBS_INSERT, &dbtbl_file_info);
        if(iRet)
        {
            HtLog(gLogFile,HT_LOG_MODE_ERROR,__FILE__,__LINE__,
                "DbsTblFileInfo DBS_INSERT error!:[%d]", iRet);
            return iRet;
        }
    }
    else if(iRet)
    {
        HtLog(gLogFile,HT_LOG_MODE_ERROR,__FILE__,__LINE__,
            "DbsTblFileInfo DBS_SELECT error!:[%d]", iRet);
        return iRet;
    }
    else
    {
        CommonRTrim(dbtbl_file_info.sett_date);
        CommonRTrim(dbtbl_file_info.file_name);
        iRet = DbsTblFileInfo(DBS_UPDATE, &dbtbl_file_info);
        if(iRet)
        {
            HtLog(gLogFile,HT_LOG_MODE_ERROR,__FILE__,__LINE__,
                      "DbsTblFileInfo DBS_UPDATE error!");
            return iRet;
        }
    }    
    return 0;
}

int iInsertTblPostionInfo(tbl_postion_info_def ptbl_postion_info)
{
    int iRet = 0;
    tbl_postion_info_def ptbl_postion_info1;
    memset(&ptbl_postion_info1,0x00,sizeof(ptbl_postion_info1));
    memcpy(&ptbl_postion_info1,&ptbl_postion_info,sizeof(ptbl_postion_info));
    HtLog(gLogFile,HT_LOG_MODE_DEBUG,__FILE__,__LINE__,
        "view the data ptbl_postion_info [%s][%s][%s][%f][%s][%f][%f][%f]",
        ptbl_postion_info.ext_mcht_no,
        ptbl_postion_info.bank_code,
        ptbl_postion_info.acct_no,
        ptbl_postion_info.interest,
        ptbl_postion_info.sett_date,
        ptbl_postion_info.bal,
        ptbl_postion_info.acct_bal,
        ptbl_postion_info.resv_bal);

	HtLog(gLogFile,HT_LOG_MODE_DEBUG,__FILE__,__LINE__,
        "view the data ptbl_postion_info1 [%s][%s][%s][%f][%s][%f][%f][%f]",
        ptbl_postion_info1.ext_mcht_no,
        ptbl_postion_info1.bank_code,
        ptbl_postion_info1.acct_no,
        ptbl_postion_info1.interest,
        ptbl_postion_info1.sett_date,
        ptbl_postion_info1.bal,
        ptbl_postion_info1.acct_bal,
        ptbl_postion_info1.resv_bal);
   
    iRet = DbsTblPostInfo(DBS_SELECT1, &ptbl_postion_info1);
    if(iRet == DBS_NOTFOUND)
    {
        iRet = DbsTblPostInfo(DBS_INSERT, &ptbl_postion_info1);
        if(iRet)
        {
            HtLog(gLogFile,HT_LOG_MODE_ERROR,__FILE__,__LINE__,
                "DbsTblPostInfo DBS_INSERT error!:[%d]", iRet);
            return iRet;
        }
    }
    else if(iRet)
    {
        HtLog(gLogFile,HT_LOG_MODE_ERROR,__FILE__,__LINE__,
            "DbsTblPostInfo DBS_SELECT1 error!:[%d]", iRet);
        return iRet;
    }
    else
    {
        memset(&ptbl_postion_info1,0x00,sizeof(ptbl_postion_info1));
        memcpy(&ptbl_postion_info1,&ptbl_postion_info,sizeof(ptbl_postion_info));
        CommonRTrim(ptbl_postion_info1.ext_mcht_no);
        CommonRTrim(ptbl_postion_info1.bank_code);
        CommonRTrim(ptbl_postion_info1.acct_no);
        iRet = DbsTblPostInfo(DBS_UPDATE, &ptbl_postion_info1);
        if(iRet)
        {
            HtLog(gLogFile,HT_LOG_MODE_ERROR,__FILE__,__LINE__,
                      "DbsTblFileInfo DBS_UPDATE error!");
            return iRet;
        }
    }    
    return 0;
}

int iInsertTblPostionTrial(tbl_postion_trial_def ptdbtbl_post_trial)
{
    int iRet = 0;
    tbl_postion_trial_def ptdbtbl_post_trial1;
    memset(&ptdbtbl_post_trial1,0x00,sizeof(ptdbtbl_post_trial1));
    
    memcpy(&ptdbtbl_post_trial1,&ptdbtbl_post_trial,sizeof(ptdbtbl_post_trial1));
    HtLog(gLogFile,HT_LOG_MODE_DEBUG,__FILE__,__LINE__,
        "view the data ptdbtbl_post_trial [%s][%s][%f][%s][%f][%f]",
        ptdbtbl_post_trial1.bank_code,
        ptdbtbl_post_trial1.acct_no,
        ptdbtbl_post_trial1.interest,
        ptdbtbl_post_trial1.sett_date,
        ptdbtbl_post_trial1.bal,
        ptdbtbl_post_trial1.resv_bal);
   
    iRet = DbsTblPostTrial(DBS_SELECT, &ptdbtbl_post_trial1);
    if(iRet == DBS_NOTFOUND)
    {
        iRet = DbsTblPostTrial(DBS_INSERT, &ptdbtbl_post_trial1);
        if(iRet)
        {
            HtLog(gLogFile,HT_LOG_MODE_ERROR,__FILE__,__LINE__,
                "ptdbtbl_post_trial DBS_INSERT error!:[%d]", iRet);
            return iRet;
        }
    }
    else if(iRet)
    {
        HtLog(gLogFile,HT_LOG_MODE_ERROR,__FILE__,__LINE__,
            "ptdbtbl_post_trial DBS_SELECT1 error!:[%d]", iRet);
        return iRet;
    }
    else
    {
        memset(&ptdbtbl_post_trial1,0x00,sizeof(ptdbtbl_post_trial1));
        memcpy(&ptdbtbl_post_trial1,&ptdbtbl_post_trial,sizeof(ptdbtbl_post_trial1));
        CommonRTrim(ptdbtbl_post_trial1.bank_code);
        CommonRTrim(ptdbtbl_post_trial1.acct_no);
        iRet = DbsTblPostTrial(DBS_UPDATE, &ptdbtbl_post_trial1);
        if(iRet)
        {
            HtLog(gLogFile,HT_LOG_MODE_ERROR,__FILE__,__LINE__,
                      "DbsTblPostTrial DBS_UPDATE error!");
            return iRet;
        }
    }    
    return 0;
}


int UpdateTblPostionInfo()
{    
    int	    nReturnCode = 0;
    int     nTotCount=0;
    char    sDateTime[14+1];
    char    *pstr;
    FILE    *fp;
    char    sFileFullName[200];
    char    sTmpFileName[100];
    char    sysTime[14+1];
    char    stlm_dt[8+1];

	char ret[7] = {};
	char ctamt[16] = {};
	char caamt[16] = {};
	char cuamt[16] = {};
	char cfamt[16] = {};
    
    tbl_file_info_def   dbtbl_file_info;
    tbl_postion_info_def ptbl_postion_info;
    stEpayBalance EpayBalance;
    ht_tbl_com_config_def comconfig;
    stKeyInf KeyInf;

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
    
    /* 初始文件信息 */
    memset(sDateTime, 0x00, sizeof(sDateTime));
    getSysTime(sDateTime);
    memset(&dbtbl_file_info, 0x00, sizeof(dbtbl_file_info));
    memcpy(dbtbl_file_info.sett_date, stlm_dt, 8);
    memcpy(dbtbl_file_info.snd_time, sDateTime, 14);
    memset(sTmpFileName, 0, sizeof(sTmpFileName));
    strcpy(sTmpFileName, "HOST_YYYYMMDD_XX_BALANCE.txt");
    pstr = strstr(sTmpFileName, "YYYYMMDD");
    if (pstr)
        memcpy(pstr, stlm_dt, 8);
    pstr = strstr(sTmpFileName, "XX");
    if (pstr)
        memcpy (pstr, "01", 2);
    memcpy(dbtbl_file_info.step_no, "03", 2);
    memcpy(dbtbl_file_info.batch_no, "01", 2);
    memcpy(dbtbl_file_info.file_type, "H", 1);
    memcpy(dbtbl_file_info.file_flag, "S", 1);
    memcpy(dbtbl_file_info.end_flag, "Y", 1);
    memcpy(dbtbl_file_info.bank_code, "HOST", 4);
    
    memset(sFileFullName, 0, sizeof(sFileFullName));
    sprintf(sFileFullName, "%s/%8.8s/%s/%s/%s/%s%s/",
            getenv("SHARE_FILE_PATH"),
            dbtbl_file_info.sett_date,
            dbtbl_file_info.bank_code,
            dbtbl_file_info.file_type,
            dbtbl_file_info.file_flag,
            dbtbl_file_info.step_no,
            dbtbl_file_info.batch_no);
    nReturnCode = CheckDir(sFileFullName);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "CheckDir err[%d][%s][%s]",
               nReturnCode, strerror(errno), sFileFullName);
        return -1;
    }
    strcat(sFileFullName, sTmpFileName);
    memcpy(dbtbl_file_info.file_name, sFileFullName, strlen(sFileFullName));
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name : [%s]", dbtbl_file_info.file_name);
    
    fp = fopen(dbtbl_file_info.file_name,"w+");
    if (fp == NULL)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen [%s] error.", dbtbl_file_info.file_name);
        return -1;
    }

    /* 查询外部商户号 */
    memset(&comconfig, 0x00, sizeof(comconfig));
    strcpy(comconfig.key1,"200001");
    strcpy(comconfig.key2,"200001");
    strcpy(comconfig.key3,"BANKCODE");
    nReturnCode = DbsbthComConfig (DBS_SELECT, &comconfig);
     if( nReturnCode )
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsbthComConfig DBS_SELECT[%d]", nReturnCode);
        fclose(fp);
        return nReturnCode;
    }
    CommonRTrim(comconfig.value1);  
    CommonRTrim(comconfig.value2);  
    CommonRTrim(comconfig.value3);  
                
    memset(&KeyInf, 0x00, sizeof(KeyInf));
//    nReturnCode = GetKeyInf(&KeyInf, "fuiou");
//    if (nReturnCode)
//    {
//        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "GetKeyInf err.");
//        fclose(fp);
//        return -1;
//    }
                  
    Fy_Single_Request FyRequest;
    Fy_Single_Response FyResponse; 
    
    memset(&FyRequest,0x0, sizeof(FyRequest));
    memset(&FyResponse,0x0, sizeof(FyResponse));
      
    memcpy(FyRequest.merid, "0002900F0345178", sizeof(FyRequest.merid)-1);
	memcpy(FyRequest.ver, "1", sizeof(FyRequest.ver)-1);
                
    nReturnCode = FyBalanceQueryApi(&FyRequest,&FyResponse);
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " FyBalanceQueryApi error [%d]",nReturnCode);
        fclose(fp);
        return nReturnCode;
    }
    else
    {
        char ret[7] = {};
	    char *start = NULL;
	    char *sEnd = NULL;

		start = strstr(FyResponse.xmlMsg, "<ret>");
		sEnd = strstr(FyResponse.xmlMsg, "</ret>");
	    memcpy(ret, start+5, sEnd-start-5);
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "ret:[%s]", ret);

		if(!memcmp("000000", ret, 6)) {
		    start = strstr(FyResponse.xmlMsg, "<ctamt>");
		    sEnd = strstr(FyResponse.xmlMsg, "</ctamt>");
	        memcpy(ctamt, start+7, sEnd-start-7);
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "ctamt:[%s]", ctamt);

		    start = strstr(FyResponse.xmlMsg, "<caamt>");
		    sEnd = strstr(FyResponse.xmlMsg, "</caamt>");
	        memcpy(caamt, start+7, sEnd-start-7);
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "caamt:[%s]", caamt);

		    start = strstr(FyResponse.xmlMsg, "<cuamt>");
		    sEnd = strstr(FyResponse.xmlMsg, "</cuamt>");
	        memcpy(cuamt, start+7, sEnd-start-7);
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "cuamt:[%s]", cuamt);

		    start = strstr(FyResponse.xmlMsg, "<cfamt>");
		    sEnd = strstr(FyResponse.xmlMsg, "</cfamt>");
	        memcpy(cfamt, start+7, sEnd-start-7);
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "cfamt:[%s]", cfamt);
		}

    }                   
	
	HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "ctamt:[%f]", atof(ctamt)/100);

    
    memset(&EpayBalance,0x00,sizeof(EpayBalance));
    
    memcpy(EpayBalance.stlm_dt,stlm_dt,8);
    memcpy(EpayBalance.sTransDate,sDateTime,8);
    memcpy(EpayBalance.sTransTime,sDateTime+8,6);   
	memcpy(EpayBalance.sMrchNo,"309391000011",sizeof(EpayBalance.sMrchNo)-1);
	memcpy(EpayBalance.sAcctNo,"309391000011",sizeof(EpayBalance.sAcctNo)-1);
    memcpy(EpayBalance.cny,"156",3);
	EpayBalance.dAccountBalance = atof(ctamt)/100;
	EpayBalance.dAvailableBalanc = atof(caamt)/100;
	EpayBalance.dBlockedBalance = atof(cfamt)/100;
	EpayBalance.dControlledBalance  = atof(cuamt)/100;    
    WriteFile(&EpayBalance, fp);   
    	
    memcpy(dbtbl_file_info.status, "0", 1);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name:[%s]", dbtbl_file_info.file_name);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date:[%s]", dbtbl_file_info.sett_date);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "snd_time :[%s]", dbtbl_file_info.snd_time); 
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "rcv_time :[%s]", dbtbl_file_info.rcv_time); 
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "status   :[%s]", dbtbl_file_info.status);   
    //update
    memset(&ptbl_postion_info,0x00,sizeof(ptbl_postion_info));
    memcpy(ptbl_postion_info.ext_mcht_no,"309391000011",sizeof(ptbl_postion_info.ext_mcht_no)-1);
    memcpy(ptbl_postion_info.acct_no,"309391000011",sizeof(ptbl_postion_info.acct_no)-1);
    ptbl_postion_info.bal =atoi(ctamt);
    ptbl_postion_info.acct_bal = atof(ctamt)/100;                       /*账户余额*/
    ptbl_postion_info.resv_bal = atof(cfamt)/100;
    ptbl_postion_info.avai_bal =  atof(caamt)/100;                      /*可用余额*/
    ptbl_postion_info.block_bal = atof(cfamt)/100;                    /*冻结余额*/
    ptbl_postion_info.ctrl_bal =  atof(cuamt)/100;                           /*控制余额*/
       
    memcpy(ptbl_postion_info.bank_code,comconfig.value1,sizeof(ptbl_postion_info.bank_code)-1);
    memcpy(ptbl_postion_info.bank_name,comconfig.value2,sizeof(ptbl_postion_info.bank_name)-1);
    memcpy(ptbl_postion_info.acct_name,comconfig.value3,sizeof(ptbl_postion_info.acct_name)-1);
    ptbl_postion_info.interest=2.968;
    memcpy(ptbl_postion_info.sett_date,stlm_dt,sizeof(ptbl_postion_info.sett_date)-1); 
    memcpy(ptbl_postion_info.sync_time,sysTime,sizeof(ptbl_postion_info.sync_time)-1);   
  
    nReturnCode = iInsertTblPostionInfo(ptbl_postion_info);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "iInsertTblPostionInfo error, %d.", nReturnCode);
        fclose(fp);
        return -1;
    }
      
    /* 插入文件信息表 */
    nReturnCode = iInsertTblFileInfo(dbtbl_file_info);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "iInsertTblFileInfo error [%d]", nReturnCode);
        fclose(fp);
        return -1;
    }
    fclose(fp);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file [%s] Insert into ht_tbl_file_info success.", dbtbl_file_info.file_name);
     
    HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "UpdateTblPostionInfo Succ .");
    return 0;                  
}

/*************************************************
*函数名称∶ Total_6502
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_6502()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_6502
*函数功能∶ 头寸信息表同步到头寸试算表
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_6502 ( int nBeginOffset, int nEndOffset )
{
    int nReturnCode;
    char    stlm_dt[8 + 1];
    int		nTotalCount = 0;
    
    tbl_child_inf_def        dbtbl_child_inf;
    tbl_postion_info_def ptbl_postion_info;
    tbl_postion_trial_def ptbl_postion_trial;
    /* 清算日期 */
    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
    
    nReturnCode = UpdateTblPostionInfo();
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "UpdateTblPostionInfo error, %d.", nReturnCode);
        return -1;
    }
    
    /*memset(&ptbl_postion_trial, 0x00, sizeof(ptbl_postion_trial));
    nReturnCode = DbsTblPostTrial(DBS_TRUNCATE, &ptbl_postion_trial);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblPostTrial DBS_TRUNCATE error, %d.", nReturnCode);
        return -1;
    }
    */

    memset(&ptbl_postion_info, 0x00, sizeof(ptbl_postion_info));
    memcpy(ptbl_postion_info.sett_date, stlm_dt,sizeof(ptbl_postion_info.sett_date)-1);   
    nReturnCode = DbsTblPostInfo(DBS_CURSOR, &ptbl_postion_info);    
    nReturnCode = DbsTblPostInfo(DBS_OPEN, &ptbl_postion_info);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblPostInfo DBS_OPEN error, %d.", nReturnCode);
        return -1;
    }
	while (1)
    {
        nReturnCode = DbsTblPostInfo(DBS_FETCH, &ptbl_postion_info);
        if (nReturnCode == DBS_NOTFOUND)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_FETCH DBS_FETCH end.", nReturnCode);
            break;
        }
        else if(nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblPostInfo DBS_FETCH error, %d.", nReturnCode);
            DbsTblPostInfo(DBS_CLOSE, &ptbl_postion_info);
            return -1;
        }
        nTotalCount++;
        RightTrim(ptbl_postion_info.bank_code);
        RightTrim(ptbl_postion_info.bank_name);
        RightTrim(ptbl_postion_info.acct_no);
        RightTrim(ptbl_postion_info.acct_name);
        RightTrim(ptbl_postion_info.sett_date);
        RightTrim(ptbl_postion_info.sync_time);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "bank_code:[%s],sett_date:[%s],sync_time:[%s]", ptbl_postion_info.bank_code,ptbl_postion_info.sett_date,ptbl_postion_info.sync_time);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "interest:[%.3lf],bal:[%.2lf],resv_bal:[%.2lf]", ptbl_postion_info.interest,ptbl_postion_info.acct_bal,ptbl_postion_info.resv_bal);
        
        memset(&ptbl_postion_trial, 0x00, sizeof(ptbl_postion_trial));
        memcpy(ptbl_postion_trial.bank_code , ptbl_postion_info.bank_code , sizeof(ptbl_postion_trial.bank_code)-1);
        memcpy(ptbl_postion_trial.bank_name , ptbl_postion_info.bank_name , sizeof(ptbl_postion_trial.bank_name)-1);
        memcpy(ptbl_postion_trial.acct_no   , ptbl_postion_info.acct_no   , sizeof(ptbl_postion_trial.acct_no)-1);
        memcpy(ptbl_postion_trial.acct_name , ptbl_postion_info.acct_name , sizeof(ptbl_postion_trial.acct_name)-1);
        memcpy(ptbl_postion_trial.sett_date , ptbl_postion_info.sett_date , sizeof(ptbl_postion_trial.sett_date)-1);
        memcpy(ptbl_postion_trial.sync_time , ptbl_postion_info.sync_time , sizeof(ptbl_postion_trial.sync_time)-1);
        memcpy(ptbl_postion_trial.seq_no, "0", 1);
        ptbl_postion_trial.interest = ptbl_postion_info.interest;
        ptbl_postion_trial.bal = ptbl_postion_info.acct_bal;
        ptbl_postion_trial.resv_bal = ptbl_postion_info.resv_bal;
        ptbl_postion_trial.bal_trial = ptbl_postion_info.acct_bal;
        
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "bank_code:[%s]", ptbl_postion_trial.bank_code);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "bank_name:[%s]", ptbl_postion_trial.bank_name);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "acct_no:[%s]", ptbl_postion_trial.acct_no);    
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "acct_name:[%s]", ptbl_postion_trial.acct_name);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date:[%s]", ptbl_postion_trial.sett_date);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sync_time:[%s]", ptbl_postion_trial.sync_time);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "seq_no:[%s]", ptbl_postion_trial.seq_no);      
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "interest:[%.3lf]", ptbl_postion_trial.interest);  
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "bal:[%.2lf]", ptbl_postion_trial.bal);            
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "resv_bal:[%.2lf]", ptbl_postion_trial.resv_bal);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "bal_trial:[%.2lf]", ptbl_postion_trial.bal_trial);
        nReturnCode = iInsertTblPostionTrial(ptbl_postion_trial);
        if (nReturnCode)
	    {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "iInsertTblPostionTrial DBS_INSERT error, %d.", nReturnCode);
            DbsTblPostInfo(DBS_CLOSE, &ptbl_postion_info);
            return -1;
	    }
    }
    DbsTblPostInfo(DBS_CLOSE, &ptbl_postion_info);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nTotalCount:[%d]", nTotalCount);
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_6502 Succ stlm_dt[%s].",stlm_dt);

    return 0;
}

/****************************** end of Task_6502.c ******************************/


