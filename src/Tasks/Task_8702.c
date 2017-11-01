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
 *  文 件 名: Task_8702.c
 *  功    能: 差错处理挂账、销账-代付金额为负数和为零处理
 *  编程人员: Jason Yu
 *  开发时间: 20170707
 *  备    注:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include <sys/socket.h>
#include "batch.h"
#include "Common.h"


extern char	gLogFile[LOG_NAME_LEN_MAX];
extern  tbl_date_inf_def dbtbl_date_inf;
extern	char ext_inter_brh_code[10 + 1];


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

    return  ;
}



/*代付负金额要到核心进行挂账*/
int NegaHand(FILE* fp, int nBeginOffset, int nEndOffset)
{
    int     nReturnCode;
    char    stlm_dt[8+1]; 
    int     iTotolCount = 0;   
    tbl_acct_rslt_def   dbtbl_acct_rslt;      
    ht_tbl_acct_inf_def  tbl_acct_inf0041;
    ht_tbl_acct_inf_def  tbl_acct_infG026;
	char* sDateTime;
	
    stHost_Acct_def  sthost_acct;
    memset(&sthost_acct,0x00,sizeof(sthost_acct));

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
	
    /*0041-获取机构往来账户信息*/
    memset(&tbl_acct_inf0041,0x0,sizeof(tbl_acct_inf0041));
    memcpy(tbl_acct_inf0041.type,"10",2);
    nReturnCode = DbsTblAcctInf(DBS_SELECT, &tbl_acct_inf0041);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAcctInf [%s] err[%d]",tbl_acct_inf0041.type,nReturnCode);
        return -1;
    }       

    /*G026-运营商分润账户(挂账余额)*/
    memset(&tbl_acct_infG026,0x0,sizeof(tbl_acct_infG026));
    memcpy(tbl_acct_infG026.type,"13",2);
    nReturnCode = DbsTblAcctInf(DBS_SELECT, &tbl_acct_infG026);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAcctInf [%s] err[%d]",tbl_acct_infG026.type,nReturnCode);
        return -1;
    }           
           
    memset(&dbtbl_acct_rslt, 0x00, sizeof(dbtbl_acct_rslt));    
    memcpy(dbtbl_acct_rslt.acct_date, stlm_dt, 8);
    memcpy(dbtbl_acct_rslt.acct_statu , "N", 1);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "acct_date:[%s], acct_statu:[%s]", dbtbl_acct_rslt.acct_date, dbtbl_acct_rslt.acct_statu);
    /* 游标，入账结果表*/
    nReturnCode = DbsTblAcctRslt(DBS_CURSOR1, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);	
    nReturnCode = DbsTblAcctRslt(DBS_OPEN1, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
    if (nReturnCode)
    {
	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN1 tbl_rslt_sett_cur1 error [%d]", nReturnCode);
	    return -1;
    }
    
    while(1)
    {
        /* fetch 每条未入账的记录 */
        memset(&dbtbl_acct_rslt, 0x0, sizeof(dbtbl_acct_rslt));
	    nReturnCode = DbsTblAcctRslt(DBS_FETCH1, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);   
        if (nReturnCode && nReturnCode != DBS_FETCHNULL)
        {
            if (nReturnCode == DBS_NOTFOUND)
            {
                break;
            }

	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_FETCH1 tbl_rslt_sett_cur_1 error [%d]", nReturnCode);
	        DbsTblAcctRslt(DBS_CLOSE1, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
            return -1;
        }	    
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "inter_brh_code:[%s].", dbtbl_acct_rslt.inter_brh_code);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_role:[%s].", dbtbl_acct_rslt.mcht_role);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_rslt_no:[%s].", dbtbl_acct_rslt.mcht_rslt_no);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "trans_num:[%d].", dbtbl_acct_rslt.trans_num);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "debt_at:[%.2f].", dbtbl_acct_rslt.debt_at);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "cret_at:[%.2f].", dbtbl_acct_rslt.cret_at);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "debt_net_fee:[%.2f].", dbtbl_acct_rslt.debt_net_fee);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "cret_net_fee:[%.2f].", dbtbl_acct_rslt.cret_net_fee);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_md:[%s].", dbtbl_acct_rslt.sett_md);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_bank:[%s].", dbtbl_acct_rslt.mcht_sett_bank);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_bank_nm:[%s].", dbtbl_acct_rslt.mcht_sett_bank_nm);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_acct_type:[%s].", dbtbl_acct_rslt.mcht_sett_acct_type);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_acct:[%s].", dbtbl_acct_rslt.mcht_sett_acct);            
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_acct_nm:[%s].", dbtbl_acct_rslt.mcht_sett_acct_nm);      
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_sett_bank:[%s].", dbtbl_acct_rslt.plat_sett_bank);            
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_sett_bank_nm:[%s].", dbtbl_acct_rslt.plat_sett_bank_nm);      
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_sett_acct_type:[%s].", dbtbl_acct_rslt.plat_sett_acct_type);  
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_sett_acct:[%s].", dbtbl_acct_rslt.plat_sett_acct);            
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_sett_acct_nm:[%s].", dbtbl_acct_rslt.plat_sett_acct_nm);      
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "acct_statu:[%s].", dbtbl_acct_rslt.acct_statu);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sz_status:[%s].", dbtbl_acct_rslt.sz_status);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "seq_num:[%d].", dbtbl_acct_rslt.seq_num);      
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "========================="); 

	    if(dbtbl_acct_rslt.debt_at <= 0.0000001 && 
            dbtbl_acct_rslt.debt_at >= -0.0000001 &&
            dbtbl_acct_rslt.cret_at <= 0.0000001 && 
            dbtbl_acct_rslt.cret_at >= -0.0000001)
        {
            HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "debt_at 0 [%f] cret_at 0[%f] 借贷金额为零",dbtbl_acct_rslt.debt_at ,dbtbl_acct_rslt.cret_at);

	        RightTrim(dbtbl_acct_rslt.mcht_rslt_no);
	        RightTrim(dbtbl_acct_rslt.mcht_role);
	        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_rslt_no:[%s].", dbtbl_acct_rslt.mcht_rslt_no);
	        memcpy(dbtbl_acct_rslt.acct_statu, "8", 1);
	        nReturnCode = DbsTblAcctRslt(DBS_UPDATE, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
	        if (nReturnCode == DBS_NOTFOUND)
	        {
	            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPDATE ht_tbl_acct_rslt error [%d]", nReturnCode);
	            DbsTblAcctRslt(DBS_CLOSE1, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
	            fclose (fp);
	            return -1;
	        }
        }
	
	    if(dbtbl_acct_rslt.debt_at -dbtbl_acct_rslt.cret_at < 0.0000001 )
        {
            HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "debt_at 0 [%f]ret_at 0[%f] ,代付金额为负数，挂账处理",dbtbl_acct_rslt.debt_at ,dbtbl_acct_rslt.cret_at);
            iTotolCount++;       
            /* 将记录信息写入文件中 */
	        memset(sDateTime,0x00,sizeof(sDateTime));
	        getSysTime(sDateTime);

	        if(strncmp(dbtbl_acct_rslt.mcht_role, "1001",4) == 0)
	        {
	        /*机构往来账户0041 -->运营商分润账户(挂账余额) G026*/
	        memset(&sthost_acct,0x00,sizeof(sthost_acct));
	        rtrim(dbtbl_acct_rslt.mcht_rslt_no);
	        rtrim(dbtbl_acct_rslt.mcht_role);
	        sprintf(sthost_acct.seq_no,     "%s%s",    dbtbl_acct_rslt.mcht_rslt_no,dbtbl_acct_rslt.mcht_role);      /*结算流水号*/
	        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
	        memcpy(sthost_acct.chl_id,     "二清",    sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
	        memcpy(sthost_acct.trans_date, dbtbl_acct_rslt.sett_date,  sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
	        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
	        memcpy(sthost_acct.bank_code,  tbl_acct_inf0041.bank_code, sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
	        memcpy(sthost_acct.acct_type,  tbl_acct_inf0041.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
	        memcpy(sthost_acct.acct_no,    tbl_acct_inf0041.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
	        memcpy(sthost_acct.acct_name,  tbl_acct_inf0041.acct_name, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
	        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* 币种 */  
	        sthost_acct.amt = (dbtbl_acct_rslt.debt_at - dbtbl_acct_rslt.cret_at)/100;                                                   /*交易金额*/
	        memcpy(sthost_acct.bank_code1, tbl_acct_infG026.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */        
	        memcpy(sthost_acct.acct_type1, tbl_acct_infG026.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
	        memcpy(sthost_acct.acct_no1,   tbl_acct_infG026.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
	        memcpy(sthost_acct.acct_name1, tbl_acct_infG026.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */     
	        memcpy(sthost_acct.dc_flag,    "11",                    sizeof(sthost_acct.dc_flag)-1); 
	        memcpy(sthost_acct.param_1,    "",                      sizeof(sthost_acct.param_1)-1);                                  
	            
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
	        else if(strncmp(dbtbl_acct_rslt.mcht_role, "1002",4) == 0)
	        {
	         /*机构往来账户0041 -->运营商分润账户(挂账余额) G026*/
	        memset(&sthost_acct,0x00,sizeof(sthost_acct));
	        rtrim(dbtbl_acct_rslt.mcht_rslt_no);
	        rtrim(dbtbl_acct_rslt.mcht_role);
	        sprintf(sthost_acct.seq_no,     "%s%s",    dbtbl_acct_rslt.mcht_rslt_no,dbtbl_acct_rslt.mcht_role);      /*结算流水号*/
	        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
	        memcpy(sthost_acct.chl_id,     "直连",    sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
	        memcpy(sthost_acct.trans_date, dbtbl_acct_rslt.sett_date,  sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
	        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
	        memcpy(sthost_acct.bank_code,  tbl_acct_inf0041.bank_code, sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
	        memcpy(sthost_acct.acct_type,  tbl_acct_inf0041.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
	        memcpy(sthost_acct.acct_no,    tbl_acct_inf0041.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
	        memcpy(sthost_acct.acct_name,  tbl_acct_inf0041.acct_name, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
	        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* 币种 */  
	        sthost_acct.amt = (dbtbl_acct_rslt.debt_at - dbtbl_acct_rslt.cret_at)/100;                                                   /*交易金额*/
	        memcpy(sthost_acct.bank_code1, tbl_acct_infG026.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */        
	        memcpy(sthost_acct.acct_type1, tbl_acct_infG026.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
	        memcpy(sthost_acct.acct_no1,   tbl_acct_infG026.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
	        memcpy(sthost_acct.acct_name1, tbl_acct_infG026.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */     
	        memcpy(sthost_acct.dc_flag,    "11",                    sizeof(sthost_acct.dc_flag)-1); 
	        memcpy(sthost_acct.param_1,    "",                      sizeof(sthost_acct.param_1)-1);                                  
	            
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
	        else if(strncmp(dbtbl_acct_rslt.mcht_role, "2002",4) == 0)
	        {
	         /*机构往来账户0041 -->运营商分润账户(挂账余额) G026*/
	        memset(&sthost_acct,0x00,sizeof(sthost_acct));
	        rtrim(dbtbl_acct_rslt.mcht_rslt_no);
	        rtrim(dbtbl_acct_rslt.mcht_role);
	        sprintf(sthost_acct.seq_no,     "%s%s",    dbtbl_acct_rslt.mcht_rslt_no,dbtbl_acct_rslt.mcht_role);      /*结算流水号*/
	        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
	        memcpy(sthost_acct.chl_id,     "直连",    sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
	        memcpy(sthost_acct.trans_date, dbtbl_acct_rslt.sett_date,  sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
	        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
	        memcpy(sthost_acct.bank_code,  tbl_acct_inf0041.bank_code, sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
	        memcpy(sthost_acct.acct_type,  tbl_acct_inf0041.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
	        memcpy(sthost_acct.acct_no,    tbl_acct_inf0041.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
	        memcpy(sthost_acct.acct_name,  tbl_acct_inf0041.acct_name, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
	        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* 币种 */  
	        sthost_acct.amt = (dbtbl_acct_rslt.debt_at - dbtbl_acct_rslt.cret_at)/100;                                                   /*交易金额*/
	        memcpy(sthost_acct.bank_code1, tbl_acct_infG026.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */        
	        memcpy(sthost_acct.acct_type1, tbl_acct_infG026.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
	        memcpy(sthost_acct.acct_no1,   tbl_acct_infG026.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
	        memcpy(sthost_acct.acct_name1, tbl_acct_infG026.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */     
	        memcpy(sthost_acct.dc_flag,    "11",                    sizeof(sthost_acct.dc_flag)-1); 
	        memcpy(sthost_acct.param_1,    "",                      sizeof(sthost_acct.param_1)-1);                                  
	            
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

	    RightTrim(dbtbl_acct_rslt.mcht_rslt_no);
	    RightTrim(dbtbl_acct_rslt.mcht_role);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_rslt_no:[%s].", dbtbl_acct_rslt.mcht_rslt_no);
	    memcpy(dbtbl_acct_rslt.acct_statu, "7", 1);
	    nReturnCode = DbsTblAcctRslt(DBS_UPDATE, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
	    if (nReturnCode == DBS_NOTFOUND)
	    {
	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPDATE ht_tbl_acct_rslt error [%d]", nReturnCode);
	        DbsTblAcctRslt(DBS_CLOSE1, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
	        fclose (fp);
	        return -1;
	    }
     
}

	DbsTblAcctRslt(DBS_CLOSE1, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
	return 0;
}

int NegaCancel(FILE* fp, int nBeginOffset, int nEndOffset)
{
    int     nReturnCode;
    char    stlm_dt[8+1]; 
    int     iTotolCount = 0;   
    tbl_acct_rslt_def   dbtbl_acct_rslt;      
    ht_tbl_acct_inf_def  tbl_acct_inf0026;
    ht_tbl_acct_inf_def  tbl_acct_infG026;
	char* sDateTime;
	
    stHost_Acct_def  sthost_acct;
    memset(&sthost_acct,0x00,sizeof(sthost_acct));

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
    /*0026-获取运营商分润账户信息*/
    memset(&tbl_acct_inf0026,0x0,sizeof(tbl_acct_inf0026));
    memcpy(tbl_acct_inf0026.type,"7",2);
    nReturnCode = DbsTblAcctInf(DBS_SELECT, &tbl_acct_inf0026);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAcctInf [%s] err[%d]",tbl_acct_inf0026.type,nReturnCode);
        return -1;
    }       

    /*G026-运营商分润账户(挂账余额)*/
    memset(&tbl_acct_infG026,0x0,sizeof(tbl_acct_infG026));
    memcpy(tbl_acct_infG026.type,"13",2);
    nReturnCode = DbsTblAcctInf(DBS_SELECT, &tbl_acct_infG026);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAcctInf [%s] err[%d]",tbl_acct_infG026.type,nReturnCode);
        return -1;
    }           
           
    memset(&dbtbl_acct_rslt, 0x00, sizeof(dbtbl_acct_rslt));    
    memcpy(dbtbl_acct_rslt.acct_date, stlm_dt, 8);
    memcpy(dbtbl_acct_rslt.acct_statu , "7", 1);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "acct_date:[%s], acct_statu:[%s]", dbtbl_acct_rslt.acct_date, dbtbl_acct_rslt.acct_statu);
    /* 游标，入账结果表*/
    nReturnCode = DbsTblAcctRslt(DBS_CURSOR2, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);	
    nReturnCode = DbsTblAcctRslt(DBS_OPEN2, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
    if (nReturnCode)
    {
	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN2 tbl_acct_rslt_cur2 error [%d]", nReturnCode);
	    return -1;
    }
    
    while(1)
    {
        /* fetch 每条未入账的记录 */
        memset(&dbtbl_acct_rslt, 0x0, sizeof(dbtbl_acct_rslt));
	    nReturnCode = DbsTblAcctRslt(DBS_FETCH2, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);   
        if (nReturnCode && nReturnCode != DBS_FETCHNULL)
        {
            if (nReturnCode == DBS_NOTFOUND)
            {
                break;
            }

	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_FETCH2 tbl_acct_rslt_cur2 error [%d]", nReturnCode);
	        DbsTblAcctRslt(DBS_CLOSE2, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
            return -1;
        }	    
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "inter_brh_code:[%s].", dbtbl_acct_rslt.inter_brh_code);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_role:[%s].", dbtbl_acct_rslt.mcht_role);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_rslt_no:[%s].", dbtbl_acct_rslt.mcht_rslt_no);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "trans_num:[%d].", dbtbl_acct_rslt.trans_num);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "debt_at:[%.2f].", dbtbl_acct_rslt.debt_at);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "cret_at:[%.2f].", dbtbl_acct_rslt.cret_at);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "debt_net_fee:[%.2f].", dbtbl_acct_rslt.debt_net_fee);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "cret_net_fee:[%.2f].", dbtbl_acct_rslt.cret_net_fee);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_md:[%s].", dbtbl_acct_rslt.sett_md);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_bank:[%s].", dbtbl_acct_rslt.mcht_sett_bank);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_bank_nm:[%s].", dbtbl_acct_rslt.mcht_sett_bank_nm);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_acct_type:[%s].", dbtbl_acct_rslt.mcht_sett_acct_type);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_acct:[%s].", dbtbl_acct_rslt.mcht_sett_acct);            
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_acct_nm:[%s].", dbtbl_acct_rslt.mcht_sett_acct_nm);      
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_sett_bank:[%s].", dbtbl_acct_rslt.plat_sett_bank);            
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_sett_bank_nm:[%s].", dbtbl_acct_rslt.plat_sett_bank_nm);      
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_sett_acct_type:[%s].", dbtbl_acct_rslt.plat_sett_acct_type);  
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_sett_acct:[%s].", dbtbl_acct_rslt.plat_sett_acct);            
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_sett_acct_nm:[%s].", dbtbl_acct_rslt.plat_sett_acct_nm);      
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "acct_statu:[%s].", dbtbl_acct_rslt.acct_statu);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sz_status:[%s].", dbtbl_acct_rslt.sz_status);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "seq_num:[%d].", dbtbl_acct_rslt.seq_num);      
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "========================="); 

	
	    if(dbtbl_acct_rslt.debt_at -dbtbl_acct_rslt.cret_at < 0.0000001 )
        {
            HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "debt_at 0 [%f]ret_at 0[%f] ,代付金额为负数，挂账处理",dbtbl_acct_rslt.debt_at ,dbtbl_acct_rslt.cret_at);
            iTotolCount++;       
            /* 将记录信息写入文件中 */
	        memset(sDateTime,0x00,sizeof(sDateTime));
	        getSysTime(sDateTime);

	        if(strncmp(dbtbl_acct_rslt.mcht_role, "1001",4) == 0)
	        {
	        /*运营商分润账户(挂账余额) G026 --> 运营商分润账户0026*/
	        memset(&sthost_acct,0x00,sizeof(sthost_acct));
	        rtrim(dbtbl_acct_rslt.mcht_rslt_no);
	        rtrim(dbtbl_acct_rslt.mcht_role);
	        sprintf(sthost_acct.seq_no,     "%s%s",    dbtbl_acct_rslt.mcht_rslt_no,dbtbl_acct_rslt.mcht_role);      /*结算流水号*/
	        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
	        memcpy(sthost_acct.chl_id,     "二清",    sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
	        memcpy(sthost_acct.trans_date, dbtbl_acct_rslt.sett_date,  sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
	        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
	        memcpy(sthost_acct.bank_code,  tbl_acct_infG026.bank_code, sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
	        memcpy(sthost_acct.acct_type,  tbl_acct_infG026.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
	        memcpy(sthost_acct.acct_no,    tbl_acct_infG026.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
	        memcpy(sthost_acct.acct_name,  tbl_acct_infG026.acct_name, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
	        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* 币种 */  
	        sthost_acct.amt = (dbtbl_acct_rslt.debt_at - dbtbl_acct_rslt.cret_at)/100;                                                   /*交易金额*/
	        memcpy(sthost_acct.bank_code1, tbl_acct_inf0026.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */        
	        memcpy(sthost_acct.acct_type1, tbl_acct_inf0026.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
	        memcpy(sthost_acct.acct_no1,   tbl_acct_inf0026.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
	        memcpy(sthost_acct.acct_name1, tbl_acct_inf0026.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */     
	        memcpy(sthost_acct.dc_flag,    "11",                    sizeof(sthost_acct.dc_flag)-1); 
	        memcpy(sthost_acct.param_1,    "",                      sizeof(sthost_acct.param_1)-1);                                  
	            
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
	        else if(strncmp(dbtbl_acct_rslt.mcht_role, "1002",4) == 0)
	        {
	             /*运营商分润账户(挂账余额) G026 --> 运营商分润账户0026*/
	        memset(&sthost_acct,0x00,sizeof(sthost_acct));
	        rtrim(dbtbl_acct_rslt.mcht_rslt_no);
	        rtrim(dbtbl_acct_rslt.mcht_role);
	        sprintf(sthost_acct.seq_no,     "%s%s",    dbtbl_acct_rslt.mcht_rslt_no,dbtbl_acct_rslt.mcht_role);      /*结算流水号*/
	        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
	        memcpy(sthost_acct.chl_id,     "直连",    sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
	        memcpy(sthost_acct.trans_date, dbtbl_acct_rslt.sett_date,  sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
	        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
	        memcpy(sthost_acct.bank_code,  tbl_acct_infG026.bank_code, sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
	        memcpy(sthost_acct.acct_type,  tbl_acct_infG026.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
	        memcpy(sthost_acct.acct_no,    tbl_acct_infG026.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
	        memcpy(sthost_acct.acct_name,  tbl_acct_infG026.acct_name, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
	        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* 币种 */  
	        sthost_acct.amt = (dbtbl_acct_rslt.debt_at - dbtbl_acct_rslt.cret_at)/100;                                                   /*交易金额*/
	        memcpy(sthost_acct.bank_code1, tbl_acct_inf0026.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */        
	        memcpy(sthost_acct.acct_type1, tbl_acct_inf0026.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
	        memcpy(sthost_acct.acct_no1,   tbl_acct_inf0026.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
	        memcpy(sthost_acct.acct_name1, tbl_acct_inf0026.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */     
	        memcpy(sthost_acct.dc_flag,    "11",                    sizeof(sthost_acct.dc_flag)-1); 
	        memcpy(sthost_acct.param_1,    "",                      sizeof(sthost_acct.param_1)-1);                                  
	            
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
	        else if(strncmp(dbtbl_acct_rslt.mcht_role, "2002",4) == 0)
	        {
	             /*运营商分润账户(挂账余额) G026 --> 运营商分润账户0026*/
	        memset(&sthost_acct,0x00,sizeof(sthost_acct));
	        rtrim(dbtbl_acct_rslt.mcht_rslt_no);
	        rtrim(dbtbl_acct_rslt.mcht_role);
	        sprintf(sthost_acct.seq_no,     "%s%s",    dbtbl_acct_rslt.mcht_rslt_no,dbtbl_acct_rslt.mcht_role);      /*结算流水号*/
	        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
	        memcpy(sthost_acct.chl_id,     "直连",    sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
	        memcpy(sthost_acct.trans_date, dbtbl_acct_rslt.sett_date,  sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
	        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
	        memcpy(sthost_acct.bank_code,  tbl_acct_infG026.bank_code, sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
	        memcpy(sthost_acct.acct_type,  tbl_acct_infG026.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
	        memcpy(sthost_acct.acct_no,    tbl_acct_infG026.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
	        memcpy(sthost_acct.acct_name,  tbl_acct_infG026.acct_name, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
	        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* 币种 */  
	        sthost_acct.amt = (dbtbl_acct_rslt.debt_at - dbtbl_acct_rslt.cret_at)/100;                                                   /*交易金额*/
	        memcpy(sthost_acct.bank_code1, tbl_acct_inf0026.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */        
	        memcpy(sthost_acct.acct_type1, tbl_acct_inf0026.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
	        memcpy(sthost_acct.acct_no1,   tbl_acct_inf0026.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
	        memcpy(sthost_acct.acct_name1, tbl_acct_inf0026.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */     
	        memcpy(sthost_acct.dc_flag,    "11",                    sizeof(sthost_acct.dc_flag)-1); 
	        memcpy(sthost_acct.param_1,    "",                      sizeof(sthost_acct.param_1)-1);                                  
	            
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
		   
        RightTrim(dbtbl_acct_rslt.mcht_rslt_no);
	    RightTrim(dbtbl_acct_rslt.mcht_role);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_rslt_no:[%s].", dbtbl_acct_rslt.mcht_rslt_no);
	    memcpy(dbtbl_acct_rslt.acct_statu, "Z", 1);
	    nReturnCode = DbsTblAcctRslt(DBS_UPDATE, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
	    if (nReturnCode == DBS_NOTFOUND)
	    {
	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPDATE ht_tbl_acct_rslt error [%d]", nReturnCode);
	        DbsTblAcctRslt(DBS_CLOSE2, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
	        fclose (fp);
	        return -1;
	    }
  

}
	
	DbsTblAcctRslt(DBS_CLOSE2, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
	return 0;

}


/*************************************************
*函数名称∶ Total_8702
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_8702()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_8702
*函数功能∶ 代付负金额记录生成核心挂账文件
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_8702 ( int nBeginOffset, int nEndOffset )
{
    int     nReturnCode;
    char    sDateTime[14+1];
    char    stlm_dt[8 + 1];

    char    *pstr;
    FILE    *fp;
    char    sFileFullName[200];
    char    sTmpFileName[100];  
    tbl_file_info_def       dbtbl_file_info;

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);    
   
    memset(sTmpFileName, 0x00, sizeof(sTmpFileName));
    strcpy(sTmpFileName, "HOST_YYYYMMDD_XX_ACCT.txt");
    pstr = strstr(sTmpFileName, "YYYYMMDD");
    if (pstr)
        memcpy(pstr, stlm_dt, 8);
    pstr = strstr(sTmpFileName, "XX");
    if (pstr)
        memcpy (pstr, "11", 2);
                
    /* 初始文件信息 */
    memset(sDateTime, 0x00, sizeof(sDateTime));
    getSysTime(sDateTime);
    memset(&dbtbl_file_info, 0x00, sizeof(dbtbl_file_info));
    memcpy(dbtbl_file_info.sett_date, stlm_dt, 8);
    memcpy(dbtbl_file_info.snd_time, sDateTime, 14);
    memcpy(dbtbl_file_info.step_no, "04", 2);
    memcpy(dbtbl_file_info.batch_no, "11", 2);
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
    memcpy(dbtbl_file_info.file_name, sFileFullName, sizeof(dbtbl_file_info.file_name)-1);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name : [%s]", dbtbl_file_info.file_name);
    
    fp = fopen(dbtbl_file_info.file_name,"w+");
    if (fp == NULL)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen [%s] error.", dbtbl_file_info.file_name);
        return -1;
    }

    nReturnCode = NegaHand(fp, nBeginOffset, nEndOffset);
    if(nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "NegaHand err[%d]", nReturnCode);
        fclose(fp);
        return -1;
    }

    nReturnCode = NegaCancel(fp, nBeginOffset, nEndOffset);
    if(nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "NegaCancel err[%d]", nReturnCode);
        fclose(fp);
        return -1;
    }

    fclose(fp);
    memcpy(dbtbl_file_info.status, "0", 1);   
    /* 插入文件信息表 */
    nReturnCode = iInsertTblFileInfo(dbtbl_file_info);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "iInsertTblFileInfo error [%d]", nReturnCode);
        return -1;
    }

    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_8702 Succ stlm_dt[%s].",stlm_dt);
 
    return 0;
}





/****************************** end of Task_8701.c ******************************/


