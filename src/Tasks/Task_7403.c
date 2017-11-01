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
 *  文 件 名: Task_7403.c
 *  功    能: 富友T+2核心记账
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

typedef struct
{
    char seq_no[48+1]; 
    char pan[30+1];
    double amt;
}stOnlinepay_Acct_def;




/*************************************************
*函数名称∶ WriteFile
*函数功能∶
*输入参数∶ 生成记账流水文件
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
void WriteOnlinepayFile(stOnlinepay_Acct_def *psOnlinepay_acct, FILE *fp)
{
    fprintf( fp, "%s%s", psOnlinepay_acct->seq_no, "|");
    fprintf( fp, "%s%s", psOnlinepay_acct->pan, "|");
    fprintf( fp, "%.2f%s", psOnlinepay_acct->amt, "|");
    fprintf( fp, "\n");
    fflush(fp);

    return;
}
/*************************************************
*函数名称∶ in_dat_flow
*函数功能∶插入清分表
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
static int in_cost_cmp(tbl_sett_list_def* vtdbl_sett_list)
{
    ht_tbl_cost_cmp_inf_def tbl_cost_cmp_inf;
    int iRet = 0;

    memset(&tbl_cost_cmp_inf, 0x00, sizeof(tbl_cost_cmp_inf));
    memcpy(tbl_cost_cmp_inf.plat_key, vtdbl_sett_list->seq_no, sizeof(tbl_cost_cmp_inf.plat_key)-1);
    memcpy(tbl_cost_cmp_inf.sett_date, vtdbl_sett_list->sett_date, sizeof(tbl_cost_cmp_inf.sett_date)-1);
    memcpy(tbl_cost_cmp_inf.trans_tp, "*", sizeof(tbl_cost_cmp_inf.trans_tp)-1);
    tbl_cost_cmp_inf.trans_amt= vtdbl_sett_list->amt;
    tbl_cost_cmp_inf.plat_cost = vtdbl_sett_list->plat_amt_fee;
    tbl_cost_cmp_inf.file_cost = vtdbl_sett_list->amt_fee;
    if(tbl_cost_cmp_inf.plat_cost == tbl_cost_cmp_inf.file_cost)
    {
        tbl_cost_cmp_inf.is_equal[0] = 'E';  /* eqaul */
    }else{
        tbl_cost_cmp_inf.is_equal[0] = 'N';
    }
    tbl_cost_cmp_inf.cost_type[0]='D';
    getSysTime(tbl_cost_cmp_inf.rec_crt_ts);

    iRet = DbsTblCostCmpInf(DBS_INSERT, &tbl_cost_cmp_inf);
    if (iRet != 0 && iRet != DBS_KEYDUPLICATE)
    {
        HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblCostCmpInf DBS_INSERT error  [%d].", iRet);
        return -1;
    }
    else if (iRet == DBS_KEYDUPLICATE)
    {
        HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblCostCmpInf DBS_KEYDUPLICATE, plat_key[%s].", tbl_cost_cmp_inf.plat_key);
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

    return;
}

int iWriteHostFile(tbl_sett_list_def* vtdbl_sett_list,
ht_tbl_acct_inf_def  tbl_acct_inf0021,
ht_tbl_acct_inf_def  tbl_acct_inf0022,
ht_tbl_acct_inf_def  tbl_acct_infG022,
ht_tbl_acct_inf_def  tbl_acct_inf0031,
ht_tbl_acct_inf_def  tbl_acct_inf0041,
FILE *fp)
{
    int     nReturnCode;
    char    sDateTime[14+1];
    
    stHost_Acct_def  sthost_acct;
    memset(&sthost_acct,0x00,sizeof(sthost_acct));

    memset(sDateTime,0x00,sizeof(sDateTime));
    getSysTime(sDateTime);
    
    if(vtdbl_sett_list->acct_statu[0] == '2' || 
        vtdbl_sett_list->acct_statu[0] == 'R') 
    {
        /*运营商存管账户(挂账余额)G022-->运营商存管账户0022*/ // 有
        memset(&sthost_acct,0x00,sizeof(sthost_acct));
    
        getSysTime(sDateTime);
        memcpy(sthost_acct.seq_no,     vtdbl_sett_list->seq_no,    sizeof(sthost_acct.seq_no)-1);      /*结算流水号*/
        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
        memcpy(sthost_acct.chl_id,     vtdbl_sett_list->chl_id,    sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
        memcpy(sthost_acct.trans_date, vtdbl_sett_list->sett_date,  sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
        memcpy(sthost_acct.bank_code,  tbl_acct_infG022.bank_code, sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
        memcpy(sthost_acct.acct_type,  tbl_acct_infG022.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
        memcpy(sthost_acct.acct_no,    tbl_acct_infG022.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
        memcpy(sthost_acct.acct_name,  tbl_acct_infG022.acct_name, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* 币种 */  
        sthost_acct.amt = vtdbl_sett_list->amt;                                                   /*交易金额*/
        memcpy(sthost_acct.bank_code1, tbl_acct_inf0022.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */ 
        memcpy(sthost_acct.acct_type1, tbl_acct_inf0022.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */         
        memcpy(sthost_acct.acct_no1,   tbl_acct_inf0022.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
        memcpy(sthost_acct.acct_name1, tbl_acct_inf0022.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */ 
        if(vtdbl_sett_list->acct_statu[0] == '2')
        {
             memcpy(sthost_acct.dc_flag,    "04",                    sizeof(sthost_acct.dc_flag)-1);  
        }
        else if(vtdbl_sett_list->acct_statu[0] == 'R')
        {            
            memcpy(sthost_acct.dc_flag,    "06",                    sizeof(sthost_acct.dc_flag)-1);  
        }   
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
    
    
    if(strncmp(vtdbl_sett_list->mcht_role, "0031",4) == 0)
    {
        /*运营商存管账户0022-->商户结算账户0031*/
        memset(&sthost_acct,0x00,sizeof(sthost_acct));
        memcpy(sthost_acct.seq_no,     vtdbl_sett_list->seq_no,    sizeof(sthost_acct.seq_no)-1);      /*结算流水号*/
        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
        memcpy(sthost_acct.chl_id,     vtdbl_sett_list->chl_id,    sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
        memcpy(sthost_acct.trans_date, vtdbl_sett_list->sett_date,  sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
        memcpy(sthost_acct.bank_code,  tbl_acct_inf0022.bank_code, sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
        memcpy(sthost_acct.acct_type,  tbl_acct_inf0022.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
        memcpy(sthost_acct.acct_no,    tbl_acct_inf0022.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
        memcpy(sthost_acct.acct_name,  tbl_acct_inf0022.acct_name, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* 币种 */  
        sthost_acct.amt = vtdbl_sett_list->amt;                                                   /*交易金额*/
        memcpy(sthost_acct.bank_code1, vtdbl_sett_list->mcht_sett_bank, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */ 
        memcpy(sthost_acct.acct_type1, tbl_acct_inf0031.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
        memcpy(sthost_acct.acct_no1,   vtdbl_sett_list->mcht_sett_xingmin_acct,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
        memcpy(sthost_acct.acct_name1, vtdbl_sett_list->mcht_sett_acct_nm, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */     
        memcpy(sthost_acct.dc_flag,    "02",                    sizeof(sthost_acct.dc_flag)-1);
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
 
 
        /*商户结算账户0031-->机构往来户0041*/
        memset(&sthost_acct,0x00,sizeof(sthost_acct));
        memcpy(sthost_acct.seq_no,     vtdbl_sett_list->seq_no,    sizeof(sthost_acct.seq_no)-1);      /*结算流水号*/
        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
        memcpy(sthost_acct.chl_id,     vtdbl_sett_list->chl_id,    sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
        memcpy(sthost_acct.trans_date, vtdbl_sett_list->sett_date,  sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
        memcpy(sthost_acct.bank_code,  vtdbl_sett_list->mcht_sett_bank, sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
        memcpy(sthost_acct.acct_type,  tbl_acct_inf0031.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
        memcpy(sthost_acct.acct_no,    vtdbl_sett_list->mcht_sett_xingmin_acct,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
        memcpy(sthost_acct.acct_name,  vtdbl_sett_list->mcht_sett_acct_nm, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* 币种 */  
        sthost_acct.amt = vtdbl_sett_list->amt;                                                   /*交易金额*/
        memcpy(sthost_acct.bank_code1, tbl_acct_inf0041.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */ 
        memcpy(sthost_acct.acct_type1, tbl_acct_inf0041.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
        memcpy(sthost_acct.acct_no1,   tbl_acct_inf0041.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
        memcpy(sthost_acct.acct_name1, tbl_acct_inf0041.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */     
        memcpy(sthost_acct.dc_flag,    "07",                    sizeof(sthost_acct.dc_flag)-1);
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
    else if(strncmp(vtdbl_sett_list->mcht_role, "1001",4) == 0)
    {
        /*运营商存管账户0022-->商户结算账户0031*/ // 有
        memset(&sthost_acct,0x00,sizeof(sthost_acct));
        memcpy(sthost_acct.seq_no,     vtdbl_sett_list->seq_no,    sizeof(sthost_acct.seq_no)-1);      /*结算流水号*/
        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
        memcpy(sthost_acct.chl_id,     vtdbl_sett_list->chl_id,    sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
        memcpy(sthost_acct.trans_date, vtdbl_sett_list->sett_date,  sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
        memcpy(sthost_acct.bank_code,  tbl_acct_inf0022.bank_code, sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
        memcpy(sthost_acct.acct_type,  tbl_acct_inf0022.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
        memcpy(sthost_acct.acct_no,    tbl_acct_inf0022.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
        memcpy(sthost_acct.acct_name,  tbl_acct_inf0022.acct_name, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* 币种 */  
        sthost_acct.amt = vtdbl_sett_list->amt;                                                   /*交易金额*/
        memcpy(sthost_acct.bank_code1, vtdbl_sett_list->mcht_sett_bank, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */ 
        memcpy(sthost_acct.acct_type1, tbl_acct_inf0031.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
        memcpy(sthost_acct.acct_no1,   vtdbl_sett_list->mcht_sett_xingmin_acct,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
        memcpy(sthost_acct.acct_name1, vtdbl_sett_list->mcht_sett_acct_nm, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */     
        memcpy(sthost_acct.dc_flag,    "02",                    sizeof(sthost_acct.dc_flag)-1);
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
 
 
        /*商户结算账户0031-->机构往来户0041*/ // 有
        memset(&sthost_acct,0x00,sizeof(sthost_acct));
        memcpy(sthost_acct.seq_no,     vtdbl_sett_list->seq_no,    sizeof(sthost_acct.seq_no)-1);      /*结算流水号*/
        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
        memcpy(sthost_acct.chl_id,     vtdbl_sett_list->chl_id,    sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
        memcpy(sthost_acct.trans_date, vtdbl_sett_list->sett_date,  sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
        memcpy(sthost_acct.bank_code,  vtdbl_sett_list->mcht_sett_bank, sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
        memcpy(sthost_acct.acct_type,  tbl_acct_inf0031.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
        memcpy(sthost_acct.acct_no,    vtdbl_sett_list->mcht_sett_xingmin_acct,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
        memcpy(sthost_acct.acct_name,  vtdbl_sett_list->mcht_sett_acct_nm, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* 币种 */  
        sthost_acct.amt = vtdbl_sett_list->amt;                                                   /*交易金额*/
        memcpy(sthost_acct.bank_code1, tbl_acct_inf0041.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */ 
        memcpy(sthost_acct.acct_type1, tbl_acct_inf0041.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
        memcpy(sthost_acct.acct_no1,   tbl_acct_inf0041.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
        memcpy(sthost_acct.acct_name1, tbl_acct_inf0041.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */     
        memcpy(sthost_acct.dc_flag,    "07",                    sizeof(sthost_acct.dc_flag)-1);
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
    else if(strncmp(vtdbl_sett_list->mcht_role, "1002",4) == 0)
    {
        /*运营商存管账户0022-->机构往来户0041*/ // 有
        memset(&sthost_acct,0x00,sizeof(sthost_acct));
        memcpy(sthost_acct.seq_no,     vtdbl_sett_list->seq_no,    sizeof(sthost_acct.seq_no)-1);      /*结算流水号*/
        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
        memcpy(sthost_acct.chl_id,     vtdbl_sett_list->chl_id,    sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
        memcpy(sthost_acct.trans_date, vtdbl_sett_list->sett_date,  sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
        memcpy(sthost_acct.bank_code,  tbl_acct_inf0022.bank_code, sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
        memcpy(sthost_acct.acct_type,  tbl_acct_inf0022.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
        memcpy(sthost_acct.acct_no,    tbl_acct_inf0022.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
        memcpy(sthost_acct.acct_name,  tbl_acct_inf0022.acct_name, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* 币种 */  
        sthost_acct.amt = vtdbl_sett_list->amt;                                                   /*交易金额*/
        memcpy(sthost_acct.bank_code1, vtdbl_sett_list->mcht_sett_bank, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */        
        memcpy(sthost_acct.acct_type1, tbl_acct_inf0041.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
        memcpy(sthost_acct.dc_flag,    "01",                    sizeof(sthost_acct.dc_flag)-1); 
        memcpy(sthost_acct.acct_no1,   vtdbl_sett_list->mcht_sett_acct,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
        memcpy(sthost_acct.acct_name1, vtdbl_sett_list->mcht_sett_acct_nm, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */     
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
    else if(strncmp(vtdbl_sett_list->mcht_role, "2002",4) == 0)
    {
        /*运营商存管账户0022-->机构往来户0041*/
//        memset(&sthost_acct,0x00,sizeof(sthost_acct));
//        memcpy(sthost_acct.seq_no,     vtdbl_sett_list->seq_no,    sizeof(sthost_acct.seq_no)-1);      /*结算流水号*/
//        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
//        memcpy(sthost_acct.chl_id,     vtdbl_sett_list->chl_id,    sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
//        memcpy(sthost_acct.trans_date, vtdbl_sett_list->sett_date,  sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
//        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
//        memcpy(sthost_acct.bank_code,  tbl_acct_inf0041.acct_type, sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
//        memcpy(sthost_acct.acct_type,  tbl_acct_inf0041.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
//        memcpy(sthost_acct.acct_no,    tbl_acct_inf0041.acct_type,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
//        memcpy(sthost_acct.acct_name,  tbl_acct_inf0041.acct_type, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
//        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* 币种 */  
//        sthost_acct.amt = vtdbl_sett_list->amt;                                                   /*交易金额*/
//        memcpy(sthost_acct.bank_code1, "0026", sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */        
//        memcpy(sthost_acct.acct_type1, "0026", sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
//        memcpy(sthost_acct.dc_flag,    "08",                    sizeof(sthost_acct.dc_flag)-1); 
//        memcpy(sthost_acct.acct_no1,   "0026",   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
//        memcpy(sthost_acct.acct_name1, "直连机构分润", sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */     
//        memcpy(sthost_acct.param_1,    "",                      sizeof(sthost_acct.param_1)-1);                                  
//            
//        rtrim(sthost_acct.seq_no);
//        rtrim(sthost_acct.bank_code);
//        rtrim(sthost_acct.acct_type);
//        rtrim(sthost_acct.acct_no);
//        rtrim(sthost_acct.acct_name);
//        rtrim(sthost_acct.bank_code1);
//        rtrim(sthost_acct.acct_type1);
//        rtrim(sthost_acct.acct_no1);
//        rtrim(sthost_acct.acct_name1);         
//        WriteFile(&sthost_acct, fp);        
        
        /*运营商存管账户0026-->机构往来户0041*/ // 有
        memset(&sthost_acct,0x00,sizeof(sthost_acct));
        memcpy(sthost_acct.seq_no,     vtdbl_sett_list->seq_no,    sizeof(sthost_acct.seq_no)-1);      /*结算流水号*/
        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
        memcpy(sthost_acct.chl_id,     vtdbl_sett_list->chl_id,    sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
        memcpy(sthost_acct.trans_date, vtdbl_sett_list->sett_date,  sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
        memcpy(sthost_acct.bank_code,  "0026", sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
        memcpy(sthost_acct.acct_type,  "0026", sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
        memcpy(sthost_acct.acct_no,    "0026",   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
        memcpy(sthost_acct.acct_name,  "直连机构分润", sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* 币种 */  
        sthost_acct.amt = vtdbl_sett_list->amt;                                                   /*交易金额*/
        memcpy(sthost_acct.bank_code1, vtdbl_sett_list->mcht_sett_bank, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */        
        memcpy(sthost_acct.acct_type1, tbl_acct_inf0041.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
        memcpy(sthost_acct.dc_flag,    "01",                    sizeof(sthost_acct.dc_flag)-1); 
        memcpy(sthost_acct.acct_no1,   vtdbl_sett_list->mcht_sett_acct,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
        memcpy(sthost_acct.acct_name1, vtdbl_sett_list->mcht_sett_acct_nm, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */     
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
    else
    {
        HtLog(gLogFile,HT_LOG_MODE_DEBUG,__FILE__,__LINE__,"mcht_role[%s]",vtdbl_sett_list->mcht_role);
    }
    
    HtLog(gLogFile,HT_LOG_MODE_DEBUG,__FILE__,__LINE__,"amt_fee[%f]",vtdbl_sett_list->amt_fee);
    if(vtdbl_sett_list->amt_fee > 0.0)
    {    
        /*运营商存管账户0022-->机构往来户0041*/  // 有
        memset(&sthost_acct,0x00,sizeof(sthost_acct)); 
        memcpy(sthost_acct.seq_no,     vtdbl_sett_list->seq_no,    sizeof(sthost_acct.seq_no)-1);      /*结算流水号*/
        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
        memcpy(sthost_acct.chl_id,     vtdbl_sett_list->chl_id,    sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
        memcpy(sthost_acct.trans_date, sDateTime,                  sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
        memcpy(sthost_acct.bank_code,  tbl_acct_inf0022.bank_code, sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
        memcpy(sthost_acct.acct_type,  tbl_acct_inf0022.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
        memcpy(sthost_acct.acct_no,    tbl_acct_inf0022.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
        memcpy(sthost_acct.acct_name,  tbl_acct_inf0022.acct_name, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* 币种 */  
        sthost_acct.amt = vtdbl_sett_list->amt_fee;                                                   /*交易金额*/
        memcpy(sthost_acct.bank_code1, vtdbl_sett_list->mcht_sett_bank, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */ 
        memcpy(sthost_acct.acct_type1, tbl_acct_inf0041.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
        memcpy(sthost_acct.acct_no1,   vtdbl_sett_list->mcht_sett_acct,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
        memcpy(sthost_acct.acct_name1, vtdbl_sett_list->mcht_sett_acct_nm, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */  
        memcpy(sthost_acct.dc_flag,    "08",                    sizeof(sthost_acct.dc_flag)-1);    
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
    return 0;
}

int updRsltByKrnl(int tran_flag, tbl_sett_list_def* vtdbtbl_sett_list)
{
    int nReturnCode;
    tbl_sett_list_def dbtbl_sett_list;
    ht_tbl_acct_rslt_def   dbtbl_acct_rslt;
    ht_tbl_sett_flow_def tbl_sett_flow;
    int succ_flag = tran_flag;
    
    memset(&dbtbl_sett_list, 0, sizeof(dbtbl_sett_list));
    RightTrim(vtdbtbl_sett_list->seq_no);
    memcpy(dbtbl_sett_list.seq_no, vtdbtbl_sett_list->seq_no, sizeof(dbtbl_sett_list.seq_no)-1);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "seq_no:[%s]", dbtbl_sett_list.seq_no);
    nReturnCode = DbsTblSettList(DBS_SELECT, &dbtbl_sett_list);
	if (nReturnCode)
	{
	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_SELECT ht_tbl_sett_list error [%d].", nReturnCode);
	    return -1;
	}
	RightTrim(dbtbl_sett_list.mcht_rslt_no);
	RightTrim(dbtbl_sett_list.mcht_role);
	RightTrim(dbtbl_sett_list.dc_flag);
	RightTrim(dbtbl_sett_list.status);
	HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_rslt_no:[%s],mcht_role[%s],dc_flag:[%s],status:[%s]", 
	dbtbl_sett_list.mcht_rslt_no, 
	dbtbl_sett_list.mcht_role,
	dbtbl_sett_list.dc_flag, 
	dbtbl_sett_list.status);
    if(strncmp(dbtbl_sett_list.mcht_role, "0011",4) == 0 ||
        strncmp(dbtbl_sett_list.mcht_role, "0031",4) == 0)
    {
        //处理提现
 	    memset(&tbl_sett_flow, 0, sizeof(tbl_sett_flow));
	    memcpy(tbl_sett_flow.bank_order_no, vtdbtbl_sett_list->seq_no, sizeof(tbl_sett_flow.bank_order_no)-1);
	    memcpy(tbl_sett_flow.acct_statu, "Y", 1);		
		memcpy(tbl_sett_flow.status, "6", 1);
		RightTrim(tbl_sett_flow.bank_order_no);
		nReturnCode = DbsTblSettFlow(DBS_UPDATE3, &tbl_sett_flow, 0, 0);
		if (nReturnCode)
		{
		    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPDATE3 DbsTblSettFlow [%s]error [%d].", tbl_sett_flow.bank_order_no,nReturnCode);
		    return -1;
		}       
        return 0;
    }	
    		
	memset(&dbtbl_acct_rslt, 0x00, sizeof(dbtbl_acct_rslt));
	memcpy(dbtbl_acct_rslt.mcht_rslt_no, dbtbl_sett_list.mcht_rslt_no, sizeof(dbtbl_acct_rslt.mcht_rslt_no)-1);
	memcpy(dbtbl_acct_rslt.mcht_role, dbtbl_sett_list.mcht_role, sizeof(dbtbl_acct_rslt.mcht_role)-1);
	nReturnCode = DbsTblAcctRslt(DBS_SELECT2, &dbtbl_acct_rslt,0,0);
	if (nReturnCode)
	{
	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_SELECT_MD ht_tbl_acct_rslt error [%d].", nReturnCode);
	    return -1;
	}
	RightTrim(dbtbl_acct_rslt.mcht_rslt_no);
	RightTrim(dbtbl_acct_rslt.mcht_role);
	RightTrim(dbtbl_acct_rslt.sett_md);
	HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_md:[%s][%s]", dbtbl_acct_rslt.sett_md,dbtbl_acct_rslt.mcht_role);
	if(!strncmp(dbtbl_acct_rslt.sett_md, "01", 2))/* 轧差 */
	{
	    if(succ_flag)
	    {
	        if(strncmp(vtdbtbl_sett_list->acct_statu, "2",1) == 0)
	        {
	            memcpy(dbtbl_acct_rslt.acct_statu, "3", 1);
	        }
	        else
	        {	            
	            memcpy(dbtbl_acct_rslt.acct_statu, "Y", 1);	        
	        }
	    }
	    else
	    {
	        memcpy(dbtbl_acct_rslt.acct_statu, "4", 1);	        
	    }	    
	}
	else if(!strncmp(dbtbl_acct_rslt.sett_md, "02", 2))/* 收支 */
	{
	    if(succ_flag)
	    {
    	    if(!strncmp(dbtbl_acct_rslt.acct_statu, "9",1))
    	    {
    	        if(!strncmp(dbtbl_sett_list.dc_flag, "C", 1))
    	        {
    	            memcpy(dbtbl_acct_rslt.acct_statu, "5", 1);
    	        }
    	        else if(!strncmp(dbtbl_sett_list.dc_flag, "D", 1))
    	        {
    	            memcpy(dbtbl_acct_rslt.acct_statu, "6", 1);
    	        }
    	    }
    	    else if(!strncmp(dbtbl_acct_rslt.acct_statu, "5",1) || 
    	        !strncmp(dbtbl_acct_rslt.acct_statu, "6",1))
    	    {
	            memcpy(dbtbl_acct_rslt.acct_statu, "Y", 1);
	        }
    	}
    	else
    	{
    	    if(!strncmp(dbtbl_acct_rslt.acct_statu, "9",1) || 
    	       !strncmp(dbtbl_acct_rslt.acct_statu, "5",1) || 
    	       !strncmp(dbtbl_acct_rslt.acct_statu, "6",1))
    	    {
    	        memcpy(dbtbl_acct_rslt.acct_statu, "4", 1);
    	        if(!strncmp(dbtbl_sett_list.dc_flag, "C", 1))
    	        {
    	            memcpy(dbtbl_acct_rslt.sz_status, "1", 1);
    	        }
    	        else if(!strncmp(dbtbl_sett_list.dc_flag, "D", 1))
    	        {
    	            memcpy(dbtbl_acct_rslt.sz_status, "2", 1);
    	        }
    	    }
    	    else if(!strncmp(dbtbl_acct_rslt.acct_statu, "4",1))
    	    {
    	        memcpy(dbtbl_acct_rslt.sz_status, "3", 1);
    	    }
    	}
	}
	nReturnCode = DbsTblAcctRslt(DBS_UPDATE, &dbtbl_acct_rslt,0,0);
	if (nReturnCode)
	{
	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPDATE ht_tbl_acct_rslt error [%d].", nReturnCode);
	    return -1;
	}
    
    return 0;
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
*函数名称∶ Total_7403
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_7403()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_7403
*函数功能∶ 结算明细表银行卡记录生成核心记账文件
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_7403 ( int nBeginOffset, int nEndOffset )
{
    int     nReturnCode;
    char    sDateTime[14+1];
    char    stlm_dt[8+1];
    char    *pstr;
    FILE    *fp;
    char    sFileFullName[200];
    char    sTmpFileName[100];
    int		nTotalCount = 0;
    int		nPayTotalCount = 0;

    char    sFilePath[200];
    char    sBatchFullName[300+1];
    char    sTmpBatchFileName[100];    
    char    sOkBatchFileName[300];
    char    sOkTmpBatchFileName[100];
    FILE    *fppay;
    FILE    *fpok;  
    
    stOnlinepay_Acct_def  psOnlinepay_acct;  
    tbl_sett_list_def   dbtbl_sett_list;
    tbl_file_info_def   dbtbl_file_info;
    ht_tbl_acct_inf_def  tbl_acct_inf0041;
    ht_tbl_acct_inf_def  tbl_acct_inf0031;
    ht_tbl_acct_inf_def  tbl_acct_inf0021;    
    ht_tbl_acct_inf_def  tbl_acct_inf0022;
    ht_tbl_acct_inf_def  tbl_acct_infG022;
    
    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);

    /*运营商结算账户 账号信息*/
    memset(&tbl_acct_inf0021,0x0,sizeof(tbl_acct_inf0021));
    memcpy(tbl_acct_inf0021.type,"02",2);
    nReturnCode = DbsTblAcctInf(DBS_SELECT, &tbl_acct_inf0021);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAcctInf [%s] err[%d]",tbl_acct_inf0021.type,nReturnCode);
        return -1;
    }  
        
    /*运营商结算账户 账号信息*/
    memset(&tbl_acct_inf0022,0x0,sizeof(tbl_acct_inf0022));
    memcpy(tbl_acct_inf0022.type,"03",2);
    nReturnCode = DbsTblAcctInf(DBS_SELECT, &tbl_acct_inf0022);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAcctInf [%s] err[%d]",tbl_acct_inf0022.type,nReturnCode);
        return -1;
    }  
    /*运营商结算账户 账号信息*/
    memset(&tbl_acct_infG022,0x0,sizeof(tbl_acct_infG022));
    memcpy(tbl_acct_infG022.type,"12",2);
    nReturnCode = DbsTblAcctInf(DBS_SELECT, &tbl_acct_infG022);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAcctInf [%s] err[%d]",tbl_acct_infG022.type,nReturnCode);
        return -1;
    }     

    /*0031-商户结算账户 账号信息*/
    memset(&tbl_acct_inf0031,0x0,sizeof(tbl_acct_inf0031));
    memcpy(tbl_acct_inf0031.type,"08",2);
    nReturnCode = DbsTblAcctInf(DBS_SELECT, &tbl_acct_inf0031);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAcctInf [%s] err[%d]",tbl_acct_inf0031.type,nReturnCode);
        return -1;
    }
    /*0041-商户结算账户 账号信息*/
    memset(&tbl_acct_inf0041,0x0,sizeof(tbl_acct_inf0041));
    memcpy(tbl_acct_inf0041.type,"10",2);
    nReturnCode = DbsTblAcctInf(DBS_SELECT, &tbl_acct_inf0041);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAcctInf [%s] err[%d]",tbl_acct_inf0041.type,nReturnCode);
        return -1;
    } 
    
    memset(sFilePath, 0, sizeof(sFilePath));
    sprintf(sFilePath, "%s/%8.8s/%s/",
            getenv("SHARE_FILE_PATH"),
            stlm_dt,
            "ONLINEPAY");
    nReturnCode = CheckDir(sFilePath);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "CheckDir err[%d][%s][%s]",
               nReturnCode, strerror(errno), sBatchFullName);
        return -1;
    }
        
                   
    /* 初始文件信息 */
    memset(sDateTime, 0x00, sizeof(sDateTime));
    getSysTime(sDateTime);
    memset(&dbtbl_file_info, 0x00, sizeof(dbtbl_file_info));
    memcpy(dbtbl_file_info.sett_date, stlm_dt, 8);
    memcpy(dbtbl_file_info.snd_time, sDateTime, 14);
    
    memset(sTmpFileName, 0x00, sizeof(sTmpFileName));
    strcpy(sTmpFileName, "HOST_YYYYMMDD_XX_ACCT.txt");
    pstr = strstr(sTmpFileName, "YYYYMMDD");
    if (pstr)
        memcpy(pstr, stlm_dt, 8);
    pstr = strstr(sTmpFileName, "XX");
    if (pstr)
        memcpy (pstr, "02", 2);
    memcpy(dbtbl_file_info.step_no, "04", 2);
    memcpy(dbtbl_file_info.batch_no, "02", 2);
    memcpy(dbtbl_file_info.file_type, "H", 1);
    memcpy(dbtbl_file_info.file_flag, "S", 1);
    memcpy(dbtbl_file_info.end_flag, "N", 1);
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


    memset(sTmpBatchFileName, 0, sizeof(sTmpBatchFileName)); 
    sprintf(sTmpBatchFileName, "BATCH_%s_01.txt",stlm_dt);
    
    memset(sOkTmpBatchFileName, 0, sizeof(sOkTmpBatchFileName)); 
    sprintf(sOkTmpBatchFileName, "BATCH_%s_01.ok",stlm_dt);
   
    memset(sBatchFullName, 0, sizeof(sBatchFullName));
    sprintf(sBatchFullName,"%s%s",sFilePath,sTmpBatchFileName);

    memset(sOkBatchFileName, 0, sizeof(sOkBatchFileName)); 
    sprintf(sOkBatchFileName,"%s%s",sFilePath,sOkTmpBatchFileName);
   
    fppay = fopen(sBatchFullName,"a+");
    if (fppay == NULL)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen [%s] error.", sBatchFullName);
        fclose(fp);
        return -1;
    }
    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "sBatchFullName[%s].", sBatchFullName);

	// 先检查是否有代付结果未知的数据未记账
    memset(&dbtbl_sett_list, 0x00, sizeof(dbtbl_sett_list));
    memcpy(dbtbl_sett_list.sett_date, dbtbl_date_inf.syesterday, 8);
    memcpy(dbtbl_sett_list.dc_flag, "C", 1); 
	while(1) {
	    nReturnCode = DbsTblSettList(DBS_SELECT1, &dbtbl_sett_list);  
		if(nReturnCode) {
			sleep(10);
			continue;
		}
		else break;
	}
	
    
    memset(&dbtbl_sett_list, 0x00, sizeof(dbtbl_sett_list));
    memcpy(dbtbl_sett_list.sett_date, dbtbl_date_inf.syesterday, 8);
    memcpy(dbtbl_sett_list.dc_flag, "C", 1);    
    memcpy(dbtbl_sett_list.status, "4", 1);
	memcpy(dbtbl_sett_list.chl_id, "FY", 2);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date:[%s], acct_type1:[%s], status:[%s]", dbtbl_sett_list.sett_date, dbtbl_sett_list.status);
    /* 游标，结算明细表 */
    nReturnCode = DbsTblSettList(DBS_CURSOR1, &dbtbl_sett_list);   
    nReturnCode = DbsTblSettList(DBS_OPEN1, &dbtbl_sett_list);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN1 tbl_sett_var_cur error [%d]", nReturnCode);
        fclose(fp);
        fclose(fppay);
        return -1;
    }
    
    while(1)
    {
        /* fetch 每条未入账的记录 */
        memset(&dbtbl_sett_list, 0, sizeof(dbtbl_sett_list));
        nReturnCode = DbsTblSettList(DBS_FETCH1, &dbtbl_sett_list);
        if (nReturnCode == DBS_NOTFOUND)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DBS_FETCH1 tbl_sett_var_cur end.");
            break;
        }
        else if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_FETCH1 tbl_sett_var_cur error [%d]", nReturnCode);
            DbsTblSettList(DBS_CLOSE1, &dbtbl_sett_list);
            fclose(fp);
            fclose(fppay);                
            return -1;
        }
        nTotalCount++;
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "seq_no -------[%s]-[%s]------", dbtbl_sett_list.seq_no,dbtbl_sett_list.mcht_role);
        if(strncmp(dbtbl_sett_list.mcht_role, "0011",4) == 0 ||
            strncmp(dbtbl_sett_list.mcht_role, "0031",4) == 0)
        {
            nPayTotalCount++;
            memset(&psOnlinepay_acct,0x0,sizeof(psOnlinepay_acct)); 
            memcpy(psOnlinepay_acct.seq_no,dbtbl_sett_list.mcht_rslt_no ,sizeof(psOnlinepay_acct.seq_no));
            memcpy(psOnlinepay_acct.pan,dbtbl_sett_list.mcht_sett_acct,sizeof(psOnlinepay_acct.pan)-1);
            psOnlinepay_acct.amt= dbtbl_sett_list.amt;
            RightTrim(psOnlinepay_acct.seq_no); 
            RightTrim(psOnlinepay_acct.pan);                                 
            WriteOnlinepayFile(&psOnlinepay_acct, fppay);
        }
        /* 将记录信息写入文件中 */
        iWriteHostFile(&dbtbl_sett_list,tbl_acct_inf0021,tbl_acct_inf0022,tbl_acct_infG022,tbl_acct_inf0031,tbl_acct_inf0041,fp);  
              
        nReturnCode = updRsltByKrnl(1, &dbtbl_sett_list);
        if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "updRsltByKrnl error [%d]", nReturnCode);
            DbsTblSettList(DBS_CLOSE1, &dbtbl_sett_list);
            fclose(fp);
            fclose(fppay);            
            return -1;
        }
        nReturnCode = in_cost_cmp(&dbtbl_sett_list);	        
        if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_cost_cmp error [%d]", nReturnCode);
            DbsTblSettList(DBS_CLOSE1, &dbtbl_sett_list);
            fclose(fp);
            fclose(fppay);             
            return -1;
        }	
                        
        memcpy(&dbtbl_sett_list.status, "6", 1);
        CommonRTrim(dbtbl_sett_list.seq_no);        
        /* 更新结算明细表记录 */
        nReturnCode = DbsTblSettList(DBS_UPD_SEQ, &dbtbl_sett_list);
        if (nReturnCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPD_SEQ ht_tbl_acct_rslt error [%d]", nReturnCode);
            DbsTblSettList(DBS_CLOSE1, &dbtbl_sett_list);
            fclose(fp);
            fclose(fppay);             
            return -1;
        }	   
    }
    DbsTblSettList(DBS_CLOSE1, &dbtbl_sett_list);
    fclose(fp);
    fclose(fppay);     
 
    /* 插入文件信息表 */   
    memcpy(dbtbl_file_info.status, "0", 1);    
    nReturnCode = iInsertTblFileInfo(dbtbl_file_info);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "iInsertTblFileInfo error [%d]", nReturnCode);
        return -1;
    }
    
    fpok = fopen(sOkBatchFileName,"w+");
    if (fpok == NULL)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen [%s] error.", sOkBatchFileName);
        return -1;
    }    
    fprintf( fpok, "%d", nPayTotalCount);
    fflush(fpok);  
    fclose(fpok);  
        
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nTotalCount:[%d]", nTotalCount);
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_7403 Succ .");

    return 0;
}

/****************************** end of Task_7403.c ******************************/


