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
 *  文 件 名: Task_6101.c
 *  功    能: 生成清算结果流水上送核心
 *  编程人员: HELU
 *  开发时间: 2016-12-27
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
*函数名称∶ WriteFile
*函数功能∶
*输入参数∶ 生成记账流水文件
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
void WriteFile(stHost_Acct_def *psthost_acct, FILE *fp)
{
    fprintf( fp, "%s%s", psthost_acct->seq_no, "|");
    fprintf( fp, "%s%s", psthost_acct->top_cpg, "|");
    fprintf( fp, "%s%s", psthost_acct->chl_id, "|");
    fprintf( fp, "%s%s", psthost_acct->trans_date, "|");
    fprintf( fp, "%s%s", psthost_acct->trans_time, "|");
    fprintf( fp, "%s%s", psthost_acct->bank_code, "|");
    fprintf( fp, "%s%s", psthost_acct->acct_type, "|");
    fprintf( fp, "%s%s", psthost_acct->acct_no, "|");
    fprintf( fp, "%s%s", psthost_acct->acct_name, "|");
    fprintf( fp, "%s%s", psthost_acct->ccy, "|");
    fprintf( fp, "%.2f%s", psthost_acct->amt, "|");
    fprintf( fp, "%s%s", psthost_acct->bank_code1, "|");
    fprintf( fp, "%s%s", psthost_acct->acct_type1, "|");
    fprintf( fp, "%s%s", psthost_acct->acct_no1, "|");
    fprintf( fp, "%s%s", psthost_acct->acct_name1, "|");
    fprintf( fp, "%s%s", psthost_acct->dc_flag, "|");
    fprintf( fp, "%s%s", psthost_acct->param_1, "|");
    fprintf( fp, "\n");

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


/*************************************************
*函数名称∶ Total_6101
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_6101()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_6101
*函数功能∶
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_6101 ( int nBeginOffset, int nEndOffset )
{
    int	    nReturnCode = 0;
    int     nTotCount=0;
    char    sDateTime[14+1];
    char    stlm_dt[8+1];
    char    *pstr;
    char    sFileRecord[RECORD_LEN_MAX];
    FILE    *fp;
    char    sFileFullName[200];
    char    sTmpFileName[100];
    
    tbl_file_info_def   dbtbl_file_info;
    ht_tbl_sett_flow_def tbl_sett_flow;
    
    ht_tbl_acct_inf_def  tbl_acct_inf0041;//10	 0041	机构往来户
    ht_tbl_acct_inf_def  tbl_acct_inf0022;//03	 0022	运营商结算账户
    ht_tbl_acct_inf_def  tbl_acct_inf0021;//02	 0021	运营商存管账户
    stHost_Acct_def  sthost_acct;

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);

    /* 机构往来户*/
    memset(&tbl_acct_inf0041,0x0,sizeof(tbl_acct_inf0041));
    memcpy(tbl_acct_inf0041.type,"10",2);
    nReturnCode = DbsTblAcctInf(DBS_SELECT, &tbl_acct_inf0041);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAcctInf1 [%s] err[%d]",tbl_acct_inf0041.type,nReturnCode);
        return -1;
    }    

    /*运营商结算账户*/
    memset(&tbl_acct_inf0022,0x0,sizeof(tbl_acct_inf0022));
    memcpy(tbl_acct_inf0022.type,"03",2);
    nReturnCode = DbsTblAcctInf(DBS_SELECT, &tbl_acct_inf0022);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAcctInf2 [%s] err[%d]",tbl_acct_inf0022.type,nReturnCode);
        return -1;
    }    

    /*运营商存管账户*/
    memset(&tbl_acct_inf0021,0x0,sizeof(tbl_acct_inf0021));
    memcpy(tbl_acct_inf0021.type,"02",2);
    nReturnCode = DbsTblAcctInf(DBS_SELECT, &tbl_acct_inf0021);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "tbl_acct_inf0021 [%s] err[%d]",tbl_acct_inf0021.type,nReturnCode);
        return -1;
    }    
            
    /* 初始文件信息 */
    getSysTime(sDateTime);
    memset(&dbtbl_file_info, 0x00, sizeof(dbtbl_file_info));
    memcpy(dbtbl_file_info.sett_date, stlm_dt, 8);
    memcpy(dbtbl_file_info.snd_time, sDateTime, 14);
    memset(sTmpFileName, 0, sizeof(sTmpFileName));
    strcpy(sTmpFileName, "HOST_YYYYMMDD_XX_ACCT.txt");
    pstr = strstr(sTmpFileName, "YYYYMMDD");
    if (pstr)
        memcpy(pstr, stlm_dt, 8);
    pstr = strstr(sTmpFileName, "XX");
    if (pstr)
        memcpy (pstr, "01", 2);
    memcpy(dbtbl_file_info.step_no, "02", 2);
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
    

    memset(&tbl_sett_flow, 0x00, sizeof(tbl_sett_flow));
    memcpy(tbl_sett_flow.sett_date, stlm_dt, sizeof(tbl_sett_flow.sett_date)-1);
    memcpy(tbl_sett_flow.inter_brh_code, ext_inter_brh_code, sizeof(tbl_sett_flow.inter_brh_code)-1);
    DbsTblSettFlow(DBS_CURSOR6, &tbl_sett_flow, nBeginOffset, nEndOffset);
    nReturnCode = DbsTblSettFlow(DBS_OPEN6, &tbl_sett_flow, nBeginOffset, nEndOffset);
    if (nReturnCode != 0)
    {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblSettFlow DBS_OPEN1 error, %d.", nReturnCode);
            fclose (fp);
            return -1;
    }

    while (1)
    {
        memset(&tbl_sett_flow, 0, sizeof(tbl_sett_flow));
        nReturnCode = DbsTblSettFlow(DBS_FETCH6, &tbl_sett_flow, nBeginOffset, nEndOffset);
        if (nReturnCode && nReturnCode != DBS_FETCHNULL)
        {
            if (nReturnCode == DBS_NOTFOUND)
            {
                break;
            }

            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblDatFlow fetch sqlcode[%d]", nReturnCode);
            DbsTblSettFlow(DBS_CLOSE6, &tbl_sett_flow, nBeginOffset, nEndOffset);
            fclose (fp);
            return -1;
        }
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "-----------------plat_key[%s]---------", tbl_sett_flow.plat_key);
        nTotCount++;
        memset(&sthost_acct,0x00,sizeof(sthost_acct));
        
        
        if(strstr("1001|1101", tbl_sett_flow.trans_tp))//消费1001 缴费1101
        { 
            if(strstr("03", tbl_sett_flow.acct_type))//虚拟卡
            {        
                /*运营商存管账户0021 -> 运营商结算账户0022*/ //有
                memcpy(sthost_acct.seq_no,  tbl_sett_flow.plat_key ,    sizeof(sthost_acct.seq_no)-1);      /*结算流水号*/
                memcpy(sthost_acct.top_cpg, tbl_sett_flow.inter_brh_code,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
                memcpy(sthost_acct.chl_id,  ""  ,                       sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
                memcpy(sthost_acct.trans_date, tbl_sett_flow.trans_date,sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
                memcpy(sthost_acct.trans_time, tbl_sett_flow.trans_time,sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
                memcpy(sthost_acct.bank_code,  tbl_acct_inf0021.bank_code, sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
                memcpy(sthost_acct.acct_type,  tbl_acct_inf0021.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
                memcpy(sthost_acct.acct_no,    tbl_acct_inf0021.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
                memcpy(sthost_acct.acct_name,  tbl_acct_inf0021.acct_name, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
                memcpy(sthost_acct.ccy,        tbl_sett_flow.ccy,       sizeof(sthost_acct.ccy)-1);         /* 币种 */  
                sthost_acct.amt = tbl_sett_flow.trans_at/100;                                                   /*交易金额*/
                memcpy(sthost_acct.bank_code1, tbl_acct_inf0022.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */ 
                memcpy(sthost_acct.acct_type1, tbl_acct_inf0022.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
                memcpy(sthost_acct.acct_no1,   tbl_acct_inf0022.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
                memcpy(sthost_acct.acct_name1, tbl_acct_inf0022.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */  
                memcpy(sthost_acct.dc_flag,    "00",                    sizeof(sthost_acct.dc_flag)-1); 
                memcpy(sthost_acct.param_1,    tbl_sett_flow.trans_tp,  sizeof(sthost_acct.param_1)-1);
                rtrim(sthost_acct.top_cpg);                                  
                rtrim(sthost_acct.seq_no);
                rtrim(sthost_acct.bank_code);
                rtrim(sthost_acct.acct_type);
                rtrim(sthost_acct.acct_no);
                rtrim(sthost_acct.acct_name);
                rtrim(sthost_acct.bank_code1);
                rtrim(sthost_acct.acct_type1);
                rtrim(sthost_acct.acct_no1);
                rtrim(sthost_acct.acct_name1);         
                WriteFile(&sthost_acct, fp);
            }
            else
            {        
                /*机构往来户0041 -> 运营商结算账户0022*/ //有
                memcpy(sthost_acct.seq_no,  tbl_sett_flow.plat_key ,    sizeof(sthost_acct.seq_no)-1);      /*结算流水号*/
                memcpy(sthost_acct.top_cpg, tbl_sett_flow.inter_brh_code,sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
                memcpy(sthost_acct.chl_id,  ""  ,                       sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
                memcpy(sthost_acct.trans_date, tbl_sett_flow.trans_date,sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
                memcpy(sthost_acct.trans_time, tbl_sett_flow.trans_time,sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
                memcpy(sthost_acct.bank_code,  tbl_acct_inf0041.bank_code, sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
                memcpy(sthost_acct.acct_type,  tbl_acct_inf0041.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
                memcpy(sthost_acct.acct_no,    tbl_acct_inf0041.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
                memcpy(sthost_acct.acct_name,  tbl_acct_inf0041.acct_name, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
                memcpy(sthost_acct.ccy,        tbl_sett_flow.ccy,       sizeof(sthost_acct.ccy)-1);         /* 币种 */  
                sthost_acct.amt = tbl_sett_flow.trans_at/100;                                                   /*交易金额*/
                memcpy(sthost_acct.bank_code1, tbl_acct_inf0022.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */ 
                memcpy(sthost_acct.acct_type1, tbl_acct_inf0022.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
                memcpy(sthost_acct.acct_no1,   tbl_acct_inf0022.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
                memcpy(sthost_acct.acct_name1, tbl_acct_inf0022.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */  
                memcpy(sthost_acct.dc_flag,    "00",                    sizeof(sthost_acct.dc_flag)-1); 
                memcpy(sthost_acct.param_1,    tbl_sett_flow.trans_tp,  sizeof(sthost_acct.param_1)-1);                                  
                rtrim(sthost_acct.top_cpg);                                  
                rtrim(sthost_acct.seq_no);
                rtrim(sthost_acct.bank_code);
                rtrim(sthost_acct.acct_type);
                rtrim(sthost_acct.acct_no);
                rtrim(sthost_acct.acct_name);
                rtrim(sthost_acct.bank_code1);
                rtrim(sthost_acct.acct_type1);
                rtrim(sthost_acct.acct_no1);
                rtrim(sthost_acct.acct_name1);         
                WriteFile(&sthost_acct, fp);
            }
        }          
        else if(strstr(BT_TRANS_TP_CHARGE, tbl_sett_flow.trans_tp))//充值1201
        {           
                /*机构往来户0041-->运营商预收款账户0021*/ //有   
                memcpy(sthost_acct.seq_no,  tbl_sett_flow.plat_key ,    sizeof(sthost_acct.seq_no)-1);     /*结算流水号*/
                memcpy(sthost_acct.top_cpg, tbl_sett_flow.inter_brh_code,                  sizeof(sthost_acct.top_cpg)-1);    /*调用方系统ID*/
                memcpy(sthost_acct.chl_id,  ""  ,                       sizeof(sthost_acct.chl_id)-1);     /* 渠道 */
                memcpy(sthost_acct.trans_date, tbl_sett_flow.trans_date,sizeof(sthost_acct.trans_date)-1); /*交易日期*/
                memcpy(sthost_acct.trans_time, tbl_sett_flow.trans_time,sizeof(sthost_acct.trans_time)-1); /*交易时间*/
                memcpy(sthost_acct.bank_code,  tbl_acct_inf0041.bank_code, sizeof(sthost_acct.bank_code)-1);  /* 转出交易行号 */
                memcpy(sthost_acct.acct_type,  tbl_acct_inf0041.acct_type, sizeof(sthost_acct.acct_type)-1);  /* 转出账号类型 */
                memcpy(sthost_acct.acct_no,    tbl_sett_flow.pan,   sizeof(sthost_acct.acct_no)-1);    /* 转出账号 */
                memcpy(sthost_acct.acct_name,  tbl_acct_inf0041.acct_name, sizeof(sthost_acct.acct_name)-1);  /* 转出方户名 */
                memcpy(sthost_acct.ccy,        tbl_sett_flow.ccy,sizeof(sthost_acct.ccy)-1);               /* 币种 */  
                sthost_acct.amt = tbl_sett_flow.trans_at/100;                                                  /*交易金额*/
                memcpy(sthost_acct.bank_code1, tbl_acct_inf0021.bank_code,sizeof(sthost_acct.bank_code1)-1);                          /* 转入交易行号 */ 
                memcpy(sthost_acct.acct_type1, tbl_acct_inf0021.acct_type,sizeof(sthost_acct.acct_type1)-1);                          /* 转入账号类型 */
                memcpy(sthost_acct.acct_no1,   tbl_sett_flow.pan1,  sizeof(sthost_acct.acct_no1)-1);                              /* 转入账号 */
                memcpy(sthost_acct.acct_name1, tbl_acct_inf0021.acct_name,sizeof(sthost_acct.acct_name1)-1);                          /* 转入方户名 */  
                memcpy(sthost_acct.dc_flag,    "00",                   sizeof(sthost_acct.dc_flag)-1); 
                memcpy(sthost_acct.param_1,    tbl_sett_flow.trans_tp, sizeof(sthost_acct.param_1)-1);                                  
                rtrim(sthost_acct.top_cpg);                                  
                rtrim(sthost_acct.seq_no);
                rtrim(sthost_acct.bank_code);
                rtrim(sthost_acct.acct_type);
                rtrim(sthost_acct.acct_no);
                rtrim(sthost_acct.acct_name);
                rtrim(sthost_acct.bank_code1);
                rtrim(sthost_acct.acct_type1);
                rtrim(sthost_acct.acct_no1);
                rtrim(sthost_acct.acct_name1);         
                WriteFile(&sthost_acct, fp);         
            
        }
        else if(strstr(BT_TRANS_TP_WITHDRAW, tbl_sett_flow.trans_tp))//提现1301
        {                   
                /*运营商存管账户0021 -> 机构往来户0041*/ 
                memcpy(sthost_acct.seq_no,  tbl_sett_flow.plat_key ,    sizeof(sthost_acct.seq_no)-1);      /*结算流水号*/
                memcpy(sthost_acct.top_cpg, tbl_sett_flow.inter_brh_code,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
                memcpy(sthost_acct.chl_id,  ""  ,                       sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
                memcpy(sthost_acct.trans_date, tbl_sett_flow.trans_date,sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
                memcpy(sthost_acct.trans_time, tbl_sett_flow.trans_time,sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
                memcpy(sthost_acct.bank_code,  tbl_acct_inf0021.bank_code, sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
                memcpy(sthost_acct.acct_type,  tbl_acct_inf0021.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
                memcpy(sthost_acct.acct_no,    tbl_acct_inf0021.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
                memcpy(sthost_acct.acct_name,  tbl_acct_inf0021.acct_name, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
                memcpy(sthost_acct.ccy,        tbl_sett_flow.ccy,       sizeof(sthost_acct.ccy)-1);         /* 币种 */  
                sthost_acct.amt = -tbl_sett_flow.trans_at/100;                                                   /*交易金额*/
                memcpy(sthost_acct.bank_code1, tbl_acct_inf0041.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */ 
                memcpy(sthost_acct.acct_type1, tbl_acct_inf0041.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
                memcpy(sthost_acct.acct_no1,   tbl_acct_inf0041.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
                memcpy(sthost_acct.acct_name1, tbl_acct_inf0041.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */  
                memcpy(sthost_acct.dc_flag,    "07",                    sizeof(sthost_acct.dc_flag)-1); 
                memcpy(sthost_acct.param_1,    tbl_sett_flow.trans_tp,  sizeof(sthost_acct.param_1)-1);
                rtrim(sthost_acct.top_cpg);                                  
                rtrim(sthost_acct.seq_no);
                rtrim(sthost_acct.bank_code);
                rtrim(sthost_acct.acct_type);
                rtrim(sthost_acct.acct_no);
                rtrim(sthost_acct.acct_name);
                rtrim(sthost_acct.bank_code1);
                rtrim(sthost_acct.acct_type1);
                rtrim(sthost_acct.acct_no1);
                rtrim(sthost_acct.acct_name1);         
                WriteFile(&sthost_acct, fp);
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "需要记账交易   :[%s]\n", tbl_sett_flow.trans_tp);
        }                  
        else if(strstr("5001|5101", tbl_sett_flow.trans_tp))//退货
        {
           if(strstr("03", tbl_sett_flow.acct_type))//虚拟卡
            {        
              /*运营商结算账户0022-->运营商存管账户0021*/ //有
                memcpy(sthost_acct.seq_no,  tbl_sett_flow.plat_key ,    sizeof(sthost_acct.seq_no)-1);      /*结算流水号*/
                memcpy(sthost_acct.top_cpg, tbl_sett_flow.inter_brh_code,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
                memcpy(sthost_acct.chl_id,  ""  ,                       sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
                memcpy(sthost_acct.trans_date, tbl_sett_flow.trans_date,sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
                memcpy(sthost_acct.trans_time, tbl_sett_flow.trans_time,sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
                memcpy(sthost_acct.bank_code,  tbl_acct_inf0022.bank_code, sizeof(sthost_acct.bank_code)-1);   /* 转出交易行号 */
                memcpy(sthost_acct.acct_type,  tbl_acct_inf0022.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
                memcpy(sthost_acct.acct_no,    tbl_acct_inf0022.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
                memcpy(sthost_acct.acct_name,  tbl_acct_inf0022.acct_name, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
                memcpy(sthost_acct.ccy,        tbl_sett_flow.ccy,       sizeof(sthost_acct.ccy)-1);         /* 币种 */  
                sthost_acct.amt -= tbl_sett_flow.trans_at/100;                                                   /*交易金额*/
                memcpy(sthost_acct.bank_code1, tbl_acct_inf0021.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */ 
                memcpy(sthost_acct.acct_type1, tbl_acct_inf0021.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
                memcpy(sthost_acct.acct_no1,   tbl_acct_inf0021.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
                memcpy(sthost_acct.acct_name1, tbl_acct_inf0021.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */  
                memcpy(sthost_acct.dc_flag,   "00",                     sizeof(sthost_acct.dc_flag)-1); 
                memcpy(sthost_acct.param_1,    tbl_sett_flow.trans_tp,  sizeof(sthost_acct.param_1)-1);
                rtrim(sthost_acct.top_cpg);                                  
                rtrim(sthost_acct.seq_no);
                rtrim(sthost_acct.bank_code);
                rtrim(sthost_acct.acct_type);
                rtrim(sthost_acct.acct_no);
                rtrim(sthost_acct.acct_name);
                rtrim(sthost_acct.bank_code1);
                rtrim(sthost_acct.acct_type1);
                rtrim(sthost_acct.acct_no1);
                rtrim(sthost_acct.acct_name1);         
                WriteFile(&sthost_acct, fp); 
             } 
             else 
             {
               /*运营商结算账户0022-->机构往来户0041*/ //有
//                memcpy(sthost_acct.seq_no,  tbl_sett_flow.plat_key ,    sizeof(sthost_acct.seq_no)-1);      /*结算流水号*/
//                memcpy(sthost_acct.top_cpg, tbl_sett_flow.inter_brh_code,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
//                memcpy(sthost_acct.chl_id,  ""  ,                       sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
//                memcpy(sthost_acct.trans_date, tbl_sett_flow.trans_date,sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
//                memcpy(sthost_acct.trans_time, tbl_sett_flow.trans_time,sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
//                memcpy(sthost_acct.bank_code,  tbl_acct_inf0022.bank_code, sizeof(sthost_acct.bank_code)-1);   /* 转出交易行号 */
//                memcpy(sthost_acct.acct_type,  tbl_acct_inf0022.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
//                memcpy(sthost_acct.acct_no,    tbl_acct_inf0022.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
//                memcpy(sthost_acct.acct_name,  tbl_acct_inf0022.acct_name, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
//                memcpy(sthost_acct.ccy,        tbl_sett_flow.ccy,       sizeof(sthost_acct.ccy)-1);         /* 币种 */  
//                sthost_acct.amt -= tbl_sett_flow.trans_at/100;                                                   /*交易金额*/
//                memcpy(sthost_acct.bank_code1, tbl_acct_inf0041.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */ 
//                memcpy(sthost_acct.acct_type1, tbl_acct_inf0041.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
//                memcpy(sthost_acct.acct_no1,   tbl_acct_inf0041.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
//                memcpy(sthost_acct.acct_name1, tbl_acct_inf0041.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */  
//                memcpy(sthost_acct.dc_flag,   "00",                     sizeof(sthost_acct.dc_flag)-1); 
//                memcpy(sthost_acct.param_1,    tbl_sett_flow.trans_tp,  sizeof(sthost_acct.param_1)-1);
//                rtrim(sthost_acct.top_cpg);                                  
//                rtrim(sthost_acct.seq_no);
//                rtrim(sthost_acct.bank_code);
//                rtrim(sthost_acct.acct_type);
//                rtrim(sthost_acct.acct_no);
//                rtrim(sthost_acct.acct_name);
//                rtrim(sthost_acct.bank_code1);
//                rtrim(sthost_acct.acct_type1);
//                rtrim(sthost_acct.acct_no1);
//                rtrim(sthost_acct.acct_name1);         
//                WriteFile(&sthost_acct, fp);                 
                
            }            
        }        
        else
        {          
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "不需要记账交易   :[%s]\n", tbl_sett_flow.trans_tp); 
        }    
        
    }
    DbsTblSettFlow(DBS_CLOSE6, &tbl_sett_flow, nBeginOffset, nEndOffset);
    fclose (fp);
 
    memcpy(dbtbl_file_info.status, "0", 1);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name:[%s]\n", dbtbl_file_info.file_name);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date:[%s]\n", dbtbl_file_info.sett_date);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "snd_time :[%s]\n", dbtbl_file_info.snd_time); 
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "rcv_time :[%s]\n", dbtbl_file_info.rcv_time); 
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "status   :[%s]\n", dbtbl_file_info.status);   
    
    /* 插入文件信息表 */
    nReturnCode = iInsertTblFileInfo(dbtbl_file_info);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "iInsertTblFileInfo error [%d]", nReturnCode);
        return -1;
    }
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file [%s] Insert into ht_tbl_file_info success.", dbtbl_file_info.file_name);
      
    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "total num [%d].", nTotCount);
    HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_6101 Succ .");
    return 0;
}
/****************************** end of Task_6101.c ******************************/

