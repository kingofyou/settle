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
 *  文 件 名: Task_7202.c
 *  功    能: 归集记录生成银行划账文件
 *  编程人员: bcw
 *  开发时间: 2017308
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

#define BANK_STEP_NO                                     "08"

static int RecodeToCIBMer(char* strBuf, char cTab, tbl_mission_info_def *vtdbl_miss_info);
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
*函数名称∶ Total_7202
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_7202()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_7202
*函数功能∶ 归集记录生成银行划账文件
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_7202 ( int nBeginOffset, int nEndOffset )
{
    int     nReturnCode;
    char    sDateTime[14+1];
    char    stlm_dt[8+1];    
    char    *pstr;
    char    sFileRecord[RECORD_LEN_MAX];
    FILE    *fp;
    int		nTotalCount = 0;
    int     batch_no= 0;  
 
    
    tbl_mission_info_def   dbtbl_miss_info;
    tbl_file_info_def   dbtbl_file_info;
    tbl_sett_chan_def   dbtbl_sett_chan;
    
    char    sFileFullName[100];
    char    sTmpFileName[30];
    
    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
    
    memset(sDateTime, 0, sizeof(sDateTime)-1);
    getSysTime(sDateTime);
    memset(&dbtbl_miss_info, 0, sizeof(dbtbl_miss_info)-1);
    memcpy(dbtbl_miss_info.sett_date, stlm_dt, 8);
    memcpy(dbtbl_miss_info.status, "0", 1);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date:[%s], status:[%s]", dbtbl_miss_info.sett_date, dbtbl_miss_info.status);
    /* 游标，所有归集明细 */
    nReturnCode = DbsTblMissInfo(DBS_CURSOR, &dbtbl_miss_info);
    
    nReturnCode = DbsTblMissInfo(DBS_OPEN, &dbtbl_miss_info);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN tbl_miss_info_cur error [%d].", nReturnCode);
        return -1;
    }
    
    while(1)
    {
        /* fetch */
        memset(&dbtbl_miss_info, 0x00, sizeof(dbtbl_miss_info)-1);
        nReturnCode = DbsTblMissInfo(DBS_FETCH, &dbtbl_miss_info);
        if (nReturnCode == DBS_NOTFOUND)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_FETCH tbl_miss_info_cur end");
            DbsTblMissInfo(DBS_CLOSE, &dbtbl_miss_info);
            break;
        }
        else if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_FETCH tbl_miss_info_cur error [%d].", nReturnCode);
            DbsTblMissInfo(DBS_CLOSE, &dbtbl_miss_info);
            return -1;
        }
        RightTrim(dbtbl_miss_info.bank_code);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "bank_code:[%s]\n", dbtbl_miss_info.bank_code);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "amt:[%.2f].", dbtbl_miss_info.amt);
        /* mark 选择渠道 */
/*        memset(&dbtbl_sett_chan, 0, sizeof(dbtbl_sett_chan));
        nReturnCode = SetChan(dbtbl_miss_info.amt, &dbtbl_sett_chan);
        if(nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "SetChan error [%d].", nReturnCode);
            break;
        }
*/        
        /* 初始文件信息 */
        getSysTime(sDateTime);
        memset(&dbtbl_file_info, 0x00, sizeof(dbtbl_file_info)-1);
        memcpy(dbtbl_file_info.sett_date, stlm_dt, 8);
        memcpy(dbtbl_file_info.snd_time, sDateTime, 14);
        memcpy(dbtbl_file_info.step_no, BANK_STEP_NO, 2);
        memcpy(dbtbl_file_info.batch_no, "01", 2);
        memcpy(dbtbl_file_info.file_type, "M", 1);
        memcpy(dbtbl_file_info.file_flag, "S", 1);
        memcpy(dbtbl_file_info.end_flag, "N", 1);
               
        memset(sTmpFileName, 0, sizeof(sTmpFileName));
        sprintf(sTmpFileName, "BATCH_PAY_YYYYMMDD_%s%s.txt",dbtbl_file_info.step_no,dbtbl_file_info.batch_no);
        pstr = strstr(sTmpFileName, "YYYYMMDD");
        if (pstr)
            memcpy(pstr, stlm_dt, 8);

        memcpy(dbtbl_file_info.bank_code, dbtbl_miss_info.bank_code, sizeof(dbtbl_file_info.bank_code)-1);
        
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
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name:[%s].", dbtbl_file_info.file_name);
        fp = fopen(dbtbl_file_info.file_name,"w+");
        if (!fp)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen [%s] error.", dbtbl_file_info.file_name);
            return -1;
        }
            
        /* 将记录信息写入文件中 */
        memset(sFileRecord, 0x00, sizeof(sFileRecord)-1);
        RecodeToCIBMer(sFileRecord, '|', &dbtbl_miss_info);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "Record:[%s].", sFileRecord);
        nReturnCode=fputs(sFileRecord, fp);
        if(nReturnCode > 0 &&  nReturnCode != EOF)
        {
        	RightTrim(dbtbl_file_info.file_name);
        	memcpy(dbtbl_miss_info.status,"1",1);
        	memcpy(dbtbl_miss_info.file_name, dbtbl_file_info.file_name, strlen(dbtbl_file_info.file_name));
        	RightTrim(dbtbl_sett_chan.chnl_id);
        	memset(dbtbl_miss_info.chnl_id, 0, sizeof(dbtbl_miss_info.chnl_id));
        	memcpy(dbtbl_miss_info.chnl_id, dbtbl_sett_chan.chnl_id, strlen(dbtbl_sett_chan.chnl_id));
        	HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "status:[%s], file_name:[%s], sett_date:[%s]", dbtbl_miss_info.status, dbtbl_miss_info.file_name, dbtbl_miss_info.sett_date);
        	RightTrim(dbtbl_miss_info.seq_no);
        	HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "seq_no:[%s]\n", dbtbl_miss_info.seq_no);
            /* 更新归集明细表记录 */
            nReturnCode = DbsTblMissInfo(DBS_UPD_FL, &dbtbl_miss_info);
            if (nReturnCode)
            {
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_UPD_FL ht_tbl_mission_info error [%d].", nReturnCode);
                DbsTblMissInfo(DBS_CLOSE, &dbtbl_miss_info);
                fclose(fp);
                return -1;
            }
        }
        fclose(fp);
        
        strcpy(dbtbl_file_info.status, "0");
        /* 插入文件信息表 */
        nReturnCode = iInsertTblFileInfo(dbtbl_file_info);
        if (nReturnCode == DBS_NOTFOUND)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_INSERT ht_tbl_file_info error [%d].", nReturnCode);
            return -1;
        }
    }
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_7202 Succ .");
    return 0;
}

int RecodeToCIBMer(char* strBuf, char cTab, tbl_mission_info_def *vtdbl_miss_info)
{
    int     nReturnCode;
    char    sRcdBuf[RECORD_LEN_MAX] = {0};
    char    tmpBuf[50] = {0};
    char    sDelim[1+1];
    char    sDateTime[14+1];
    
    memset(sDelim, 0, sizeof(sDelim));
    sDelim[0]=cTab;
    
    RightTrim(vtdbl_miss_info->seq_no);
    strncat(sRcdBuf, vtdbl_miss_info->seq_no, strlen(vtdbl_miss_info->seq_no));                         /*交易流水号*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    strncat(sRcdBuf, sDelim, strlen(sDelim));                                                           /*二级商户为空*/
    
    RightTrim(vtdbl_miss_info->acct_no1);
    strncat(sRcdBuf, vtdbl_miss_info->acct_no1, strlen(vtdbl_miss_info->acct_no1));                     /*商户银行卡号*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_miss_info->acct_name1);
    strncat(sRcdBuf, vtdbl_miss_info->acct_name1, strlen(vtdbl_miss_info->acct_name1));                 /*商户账户名称*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));                                                           
    
    RightTrim(vtdbl_miss_info->bank_code1);
    strncat(sRcdBuf, vtdbl_miss_info->bank_code1, strlen(vtdbl_miss_info->bank_code1));                 /*银行代码*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_miss_info->acct_type1);
    strncat(sRcdBuf, vtdbl_miss_info->acct_type1, strlen(vtdbl_miss_info->acct_type1));                 /*商户账户类型*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));                                                           
    
    sprintf(sRcdBuf+strlen(sRcdBuf), "%.2lf", vtdbl_miss_info->amt);                                    /*金额*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
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

/****************************** end of Task_7202.c ******************************/


