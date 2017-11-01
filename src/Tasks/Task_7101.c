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
 *  文 件 名: Task_7101.c
 *  功    能: 银行卡交易记录生成银行划账文件
 *  编程人员: bcw
 *  开发时间: 2017-03-09
 *  备    注:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include <sys/socket.h>
#include "batch.h"
#include "Common.h"

extern  char    gLogFile[LOG_NAME_LEN_MAX];
extern  char    ext_child_date[8 + 1];
extern  char    ext_inter_brh_code[10 + 1];
extern  int     ext_inter_brh_sta;
extern          tbl_date_inf_def dbtbl_date_inf;

#define BANK_STEP_NO                                     "07"
#define BATCH_TOTAL_COUNT                               1000

typedef struct
{
    char order_no[48+1]; 
    char sub_mrch[20+1];
    char to_acct_no[32+1];
    char to_acct_name[60+1];
    char to_bank_no[14+1];
    char acct_type[1+1];
    char cur[3+1];
    double trans_amt;
    char trans_usage[60+1];
} stBank_Acct_def;

/*************************************************
*函数名称∶ WriteFile
*函数功能∶
*输入参数∶ 生成记账流水文件
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
void WriteFile(stBank_Acct_def *pstbank_acct, FILE *fp)
{
    fprintf( fp, "%s%s", pstbank_acct->order_no, "|");
    fprintf( fp, "%s%s", pstbank_acct->sub_mrch, "|");
    fprintf( fp, "%s%s", pstbank_acct->to_acct_no, "|");
    fprintf( fp, "%s%s", pstbank_acct->to_acct_name, "|");
    fprintf( fp, "%s%s", pstbank_acct->to_bank_no, "|");
    fprintf( fp, "%s%s", pstbank_acct->acct_type, "|");
    fprintf( fp, "%.2f%s", pstbank_acct->trans_amt, "|");
    fprintf( fp, "%s%s", pstbank_acct->trans_usage, "|");
    fprintf( fp, "\r\n");
    fflush(fp);
    return;
}

int RecodeToCIB(char* strBuf, char cTab, tbl_sett_list_def* vtdbl_sett_list);
static int SetChan(double amt, tbl_sett_chan_def* vtdbl_sett_chan);

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
int iIns_r_file_info(tbl_file_info_def   dbtbl_file_info)
{
    int nReturnCode;
    char    *pstr;
    char    sFileFullName[200];
    char    sTmpFileName[100];    
        
    tbl_file_info_def   dbtbl_file_info_r;
    memset(&dbtbl_file_info_r,0x0,sizeof(dbtbl_file_info_r));
    
    memcpy(dbtbl_file_info_r.sett_date, dbtbl_file_info.sett_date, 8);
    memcpy(dbtbl_file_info_r.snd_time,  dbtbl_file_info.snd_time, 14);
    memcpy(dbtbl_file_info_r.step_no,   dbtbl_file_info.step_no, 2);    
    memcpy(dbtbl_file_info_r.batch_no,  dbtbl_file_info.batch_no, 2);
    memcpy(dbtbl_file_info_r.file_type, dbtbl_file_info.file_type, 1);
    memcpy(dbtbl_file_info_r.file_flag, "R", 1);
    memcpy(dbtbl_file_info_r.end_flag,  "N", 1);
    memcpy(dbtbl_file_info_r.bank_code, dbtbl_file_info.bank_code,sizeof(dbtbl_file_info_r.bank_code)-1);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "bank_code[%s].", dbtbl_file_info_r.bank_code);

    memset(sTmpFileName, 0, sizeof(sTmpFileName));
    sprintf(sTmpFileName, "BATCH_PAY_YYYYMMDD_%s%s.txt",dbtbl_file_info_r.step_no,dbtbl_file_info_r.batch_no);
        pstr = strstr(sTmpFileName, "YYYYMMDD");
    if (pstr)
        memcpy(pstr, dbtbl_file_info.sett_date, 8);       
    memset(sFileFullName, 0, sizeof(sFileFullName));
    sprintf(sFileFullName, "%s/%8.8s/%s/%s/%s/%s%s/",
                    getenv("SHARE_FILE_PATH"),
                    dbtbl_file_info_r.sett_date,
                    dbtbl_file_info_r.bank_code,
                    dbtbl_file_info_r.file_type,
                    dbtbl_file_info_r.file_flag,
                    dbtbl_file_info_r.step_no,
                    dbtbl_file_info_r.batch_no);
    nReturnCode = CheckDir(sFileFullName);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "CheckDir err[%d][%s][%s]",
            nReturnCode, strerror(errno), sFileFullName);
        return -1;
    }
    strcat(sFileFullName, sTmpFileName);
    memcpy(dbtbl_file_info_r.file_name, sFileFullName, sizeof(dbtbl_file_info_r.file_name)-1);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name:[%s].", dbtbl_file_info.file_name);    
    strcpy(dbtbl_file_info_r.status, "4");
        /* 插入文件信息表 */
    nReturnCode = iInsertTblFileInfo(dbtbl_file_info_r);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_INSERT ht_tbl_file_info error [%d].", nReturnCode);
        return -1;
    }   
    return 0;
}
/*************************************************
*函数名称∶ Total_7101
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_7101()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_7101
*函数功能∶ 银行卡交易记录生成银行划账文件
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_7101 ( int nBeginOffset, int nEndOffset )
{
    int     nReturnCode;
    char    sDateTime[14+1];
    char    stlm_dt[8+1];
    char    *pstr;
    char    sFileRecord[RECORD_LEN_MAX];
    char    sDelim[] = "|";
    FILE    *fp;
    char    tmpFile[30+1];
    char    sBbankCode[30+1];
    int		nBankCount = 0; 
    int		nTotalCount = 0;
    int     batch_no= 0;  
    char    sFileFullName[200];
    char    sTmpFileName[100];
 
    
    tbl_sett_list_def   dbtbl_sett_list, dbtbl_sett_list_dc;
    tbl_file_info_def   dbtbl_file_info;
    ht_tbl_com_config_def comconfig;
    
     
    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
     
    memset(tmpFile, 0, sizeof(tmpFile));
    
    memset(sBbankCode, 0, sizeof(sBbankCode));
    memset(&comconfig, 0x00, sizeof(comconfig));
    strcpy(comconfig.key1,"200001");
    strcpy(comconfig.key2,"7101");
    strcpy(comconfig.key3,"BANKCODE");
    nReturnCode = DbsbthComConfig (DBS_SELECT, &comconfig);
     if( nReturnCode )
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsbthComConfig DBS_SELECT[%d]", nReturnCode);
        return nReturnCode;
    }
    CommonRTrim(comconfig.value1); 
    memcpy(sBbankCode, comconfig.value1,sizeof(sBbankCode)-1);
                
        /* 初始文件信息 */
    memset(sDateTime, 0, sizeof(sDateTime));    
    getSysTime(sDateTime);
    memset(&dbtbl_file_info, 0x00, sizeof(dbtbl_file_info));
    memcpy(dbtbl_file_info.sett_date, stlm_dt, 8);
    memcpy(dbtbl_file_info.snd_time, sDateTime, 14);
    memcpy(dbtbl_file_info.step_no,BANK_STEP_NO, 2);
    memcpy(dbtbl_file_info.batch_no, "01", 2);
    memcpy(dbtbl_file_info.file_type, "B", 1);
    memcpy(dbtbl_file_info.file_flag, "S", 1);
    memcpy(dbtbl_file_info.end_flag, "N", 1);
    
    memset(sTmpFileName, 0, sizeof(sTmpFileName));
    sprintf(sTmpFileName, "BATCH_PAY_YYYYMMDD_%s%s.txt",dbtbl_file_info.step_no,dbtbl_file_info.batch_no);
    pstr = strstr(sTmpFileName, "YYYYMMDD");
    if (pstr)
        memcpy(pstr, stlm_dt, 8);
    
 
    memcpy(dbtbl_file_info.bank_code, sBbankCode, sizeof(dbtbl_file_info.bank_code)-1);
       
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
    memcpy(dbtbl_file_info.file_name, sFileFullName, sizeof(dbtbl_file_info.file_name)-1);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name:[%s].", dbtbl_file_info.file_name);
        
    fp = fopen(dbtbl_file_info.file_name,"w+");
    if (!fp)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen %s error.", dbtbl_file_info.file_name);
        return -1;
    }
        
    /* 游标，根据待划账的行号查询结算明细表 */
    memset(&dbtbl_sett_list_dc, 0x00, sizeof(dbtbl_sett_list_dc));
    memcpy(dbtbl_sett_list_dc.sett_date, stlm_dt, 8);
    memcpy(dbtbl_sett_list_dc.plat_sett_bank, dbtbl_file_info.bank_code, sizeof(dbtbl_sett_list_dc.plat_sett_bank)-1);
    memcpy(dbtbl_sett_list_dc.status, "2", 1);
    memcpy(dbtbl_sett_list_dc.dc_flag, "C", 1);
	memcpy(dbtbl_sett_list_dc.chl_id, "XY", 2);
        
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date:[%s] bank_code:[%s]dc_flag:[%s]status:[%s] chl_id[%s]", 
    dbtbl_sett_list_dc.sett_date,
    dbtbl_sett_list_dc.plat_sett_bank,
    dbtbl_sett_list_dc.dc_flag,
    dbtbl_sett_list_dc.status,
	dbtbl_sett_list_dc.chl_id);
	
            
    nReturnCode = DbsTblSettList(DBS_CURSOR1, &dbtbl_sett_list_dc);        
    nReturnCode = DbsTblSettList(DBS_OPEN1, &dbtbl_sett_list_dc);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN1 error, %d.", nReturnCode);
        fclose(fp);
        return -1;
    }               
    while(1)
    {
        /* fetch 每条待划账的记录 */
        memset(&dbtbl_sett_list_dc, 0x00, sizeof(dbtbl_sett_list_dc));
        nReturnCode = DbsTblSettList(DBS_FETCH1, &dbtbl_sett_list_dc);
        if (nReturnCode == DBS_NOTFOUND)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DbsTblSettList DBS_FETCH1 end.");
            break;
        }
        else if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblSettList DBS_FETCH1 error [%d].", nReturnCode);
            DbsTblSettList(DBS_CLOSE1, &dbtbl_sett_list_dc);
            fclose(fp);
            return -1;
        }
            
        if(dbtbl_sett_list_dc.amt <= 0.0000001 && dbtbl_sett_list_dc.amt >= -0.0000001)
        {
            HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "dbtbl_sett_list_dc.amt 0 [%f]",dbtbl_sett_list_dc.amt);
            continue;
        }
            
        nTotalCount++;
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "amt:[%.2f].", dbtbl_sett_list_dc.amt);        
        if(dbtbl_sett_list_dc.amt < 0.00)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "交易金额<0 出报表　amt:[%.2f].", dbtbl_sett_list_dc.amt);
            continue;            
        }
        if(strlen(dbtbl_sett_list_dc.mcht_sett_acct_nm) == 0)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_acct_nm:[%s].", dbtbl_sett_list_dc.mcht_sett_acct_nm);
            DbsTblSettList(DBS_CLOSE1, &dbtbl_sett_list_dc);
            fclose(fp);
            return -1;            
        }
            
        /* 将记录信息写入文件中 */
        memset(sFileRecord, 0x00, sizeof(sFileRecord));
        RecodeToCIB(sFileRecord, '|', &dbtbl_sett_list_dc);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "Record:[%s].", sFileRecord);
        nReturnCode=fputs(sFileRecord, fp);
        if(nReturnCode > 0 &&  nReturnCode != EOF)
        {
            RightTrim(dbtbl_sett_list_dc.seq_no);
            memcpy(dbtbl_sett_list_dc.status,"3",1);          
            sprintf(dbtbl_sett_list_dc.file_name, "%s", dbtbl_file_info.file_name);
            RightTrim(dbtbl_sett_list_dc.chl_id);
            RightTrim(dbtbl_sett_list_dc.chnl_name);
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "status:[%s], file_name:[%s]", dbtbl_sett_list_dc.status, dbtbl_sett_list_dc.file_name);
	            /* 更新结算明细表记录 */
            nReturnCode = DbsTblSettList(DBS_UPD_FL, &dbtbl_sett_list_dc);
            if (nReturnCode)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPD_FL ht_tbl_sett_list error [%d].", nReturnCode);
                DbsTblSettList(DBS_CLOSE1, &dbtbl_sett_list_dc);
                fclose(fp);
	            return -1;
            }
        }        
        if(nTotalCount%BATCH_TOTAL_COUNT == 0)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "========================================");
            fclose(fp);
            strcpy(dbtbl_file_info.status, "0");
            /* 插入文件信息表 */
            nReturnCode = iInsertTblFileInfo(dbtbl_file_info);
            if (nReturnCode)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_INSERT ht_tbl_file_info error [%d].", nReturnCode);
                return -1;
            }
            nReturnCode = iIns_r_file_info(dbtbl_file_info);
            if (nReturnCode)
            {
                 HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "iIns_r_file_info ht_tbl_file_info error [%d].", nReturnCode);
                return -1;
            }
            batch_no = (nTotalCount/BATCH_TOTAL_COUNT+1);             
            memset(&dbtbl_file_info, 0x00, sizeof(dbtbl_file_info));
            memcpy(dbtbl_file_info.sett_date, stlm_dt, 8);
            memcpy(dbtbl_file_info.snd_time, sDateTime, 14);
            memcpy(dbtbl_file_info.step_no, BANK_STEP_NO, 2);
            sprintf(dbtbl_file_info.batch_no, "%02d", batch_no);
            memcpy(dbtbl_file_info.file_type, "B", 1);
            memcpy(dbtbl_file_info.file_flag, "S", 1);
            memcpy(dbtbl_file_info.end_flag, "N", 1);
               
            memset(sTmpFileName, 0, sizeof(sTmpFileName));
            sprintf(sTmpFileName, "BATCH_PAY_YYYYMMDD_%s%s.txt",dbtbl_file_info.step_no,dbtbl_file_info.batch_no);
            pstr = strstr(sTmpFileName, "YYYYMMDD");
            if (pstr)
                memcpy(pstr, stlm_dt, 8);
 
            memcpy(dbtbl_file_info.bank_code, sBbankCode, sizeof(dbtbl_file_info.bank_code)-1);
               
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
            memcpy(dbtbl_file_info.file_name, sFileFullName, sizeof(dbtbl_file_info.file_name)-1);
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name:[%s].", dbtbl_file_info.file_name);  
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name:[%s].", dbtbl_file_info.file_name);
                
            fp = fopen(dbtbl_file_info.file_name,"w+");
            if (!fp)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen %s error.", dbtbl_file_info.file_name);
                return -1;
            } 
                                                      
        }        
    }
    fclose(fp);
    DbsTblSettList(DBS_CLOSE1, &dbtbl_sett_list_dc);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file:[%s] succ.", dbtbl_file_info.file_name);
        
    strcpy(dbtbl_file_info.status, "0");
        /* 插入文件信息表 */
    nReturnCode = iInsertTblFileInfo(dbtbl_file_info);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_INSERT ht_tbl_file_info error [%d].", nReturnCode);
        return -1;
    }
    nReturnCode = iIns_r_file_info(dbtbl_file_info);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "iIns_r_file_info ht_tbl_file_info error [%d].", nReturnCode);
       return -1;
    }
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nTotalCount:[%d]", nTotalCount);
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_7101 Succ .");
                    
    return 0;
}

int RecodeToCIB(char* strBuf, char cTab, tbl_sett_list_def* vtdbl_sett_list)
{
    int     nReturnCode;
    char    sRcdBuf[RECORD_LEN_MAX] = {0};
    char    tmpBuf[50] = {0};
    char    sDelim[1+1];
    char    sDateTime[14+1];
    
    memset(sDelim, 0, sizeof(sDelim));
    sDelim[0]=cTab;
    
    RightTrim(vtdbl_sett_list->seq_no);
    strncat(sRcdBuf, vtdbl_sett_list->seq_no, strlen(vtdbl_sett_list->seq_no));                         /*交易流水号*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    strncat(sRcdBuf, sDelim, strlen(sDelim));                                                           /*二级商户为空*/
    
    RightTrim(vtdbl_sett_list->mcht_sett_acct);
    strncat(sRcdBuf, vtdbl_sett_list->mcht_sett_acct, strlen(vtdbl_sett_list->mcht_sett_acct));                     /*商户银行卡号*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_sett_list->mcht_sett_acct_nm);
    strncat(sRcdBuf, vtdbl_sett_list->mcht_sett_acct_nm, strlen(vtdbl_sett_list->mcht_sett_acct_nm));                 /*商户账户名称*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));                                                           
    
    RightTrim(vtdbl_sett_list->mcht_sett_bank);
    strncat(sRcdBuf, vtdbl_sett_list->mcht_sett_bank, strlen(vtdbl_sett_list->mcht_sett_bank));                 /*银行代码*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_sett_list->mcht_sett_acct_type);
    strncat(sRcdBuf, vtdbl_sett_list->mcht_sett_acct_type, strlen(vtdbl_sett_list->mcht_sett_acct_type));                 /*商户账户类型*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));                                                           
    
    sprintf(sRcdBuf+strlen(sRcdBuf), "%.2lf", vtdbl_sett_list->amt);                                    /*金额*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    if(strncmp(vtdbl_sett_list->mcht_role, "1001",4) == 0)
    {
        sprintf(sRcdBuf+strlen(sRcdBuf), "%s", "商户结算");                                                 /*用途*/
    }
    else if (strncmp(vtdbl_sett_list->mcht_role, "0031",4) == 0)
    {
        sprintf(sRcdBuf+strlen(sRcdBuf), "%s", "商户提现");                                                 /*用途*/
    }
    else if (strncmp(vtdbl_sett_list->mcht_role, "1002",4) == 0)
    {
        sprintf(sRcdBuf+strlen(sRcdBuf), "%s", "机构分润");                                                 /*用途*/
    }   
    else if (strncmp(vtdbl_sett_list->mcht_role, "2002",4) == 0)
    {
        sprintf(sRcdBuf+strlen(sRcdBuf), "%s", "机构分润");                                                 /*用途*/
    }          
    else if (strncmp(vtdbl_sett_list->mcht_role, "0011",4) == 0)
    {
        sprintf(sRcdBuf+strlen(sRcdBuf), "%s", "个人提现");                                                 /*用途*/
    } 
    else
    {
        sprintf(sRcdBuf+strlen(sRcdBuf), "%s", "批量代付");                                                 /*用途*/        
    }       
    strncat(sRcdBuf, sDelim, strlen(sDelim));                                                           /*用途*/
    
    strcat(sRcdBuf, "\n");
    memcpy(strBuf, sRcdBuf, strlen(sRcdBuf));
    
    return 0;
}

int SetChan(double amt, tbl_sett_chan_def* vtdbl_sett_chan)
{
    int     nReturnCode;
    tbl_sett_chan_def   dbtbl_sett_chan;
    
    memset(&dbtbl_sett_chan, 0, sizeof(dbtbl_sett_chan));
    RightTrim(vtdbl_sett_chan->status);
    memcpy(dbtbl_sett_chan.status, "Y", 1);
    
    nReturnCode = DbsTblSettChan(DBS_SELECT, amt, &dbtbl_sett_chan);
    if(nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_SELECT ht_tbl_sett_chan error, %d.", nReturnCode);
        return -1;
    }
      
    RightTrim(dbtbl_sett_chan.chnl_id);
    RightTrim(dbtbl_sett_chan.chnl_name);
    RightTrim(dbtbl_sett_chan.status);
    RightTrim(dbtbl_sett_chan.index_day);
    RightTrim(dbtbl_sett_chan.index_time);
    RightTrim(dbtbl_sett_chan.index_fee);
    RightTrim(dbtbl_sett_chan.period);
    RightTrim(dbtbl_sett_chan.shadom);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "chnl_id:[%s].", dbtbl_sett_chan.chnl_id);
    dbtbl_sett_chan.bal -= amt;
    nReturnCode = DbsTblSettChan(DBS_UPDATE, amt, &dbtbl_sett_chan);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_UPDATE ht_tbl_sett_chan error [%d].", nReturnCode);
        return -1;
    }
    memcpy(vtdbl_sett_chan, &dbtbl_sett_chan, sizeof(dbtbl_sett_chan)-1);
    
    return 0;
}


/****************************** end of Task_7101.c ******************************/


