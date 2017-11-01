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
 *  文 件 名: Task_7400.c
 *  功    能: 虚拟卡交易记录生成划账文件
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

int iWriteHostFile(ht_tbl_acct_rslt_def  *dbtbl_acct_rslt,
ht_tbl_acct_inf_def  tbl_acct_inf0026,
ht_tbl_acct_inf_def  tbl_acct_inf0041,
ht_tbl_acct_inf_def  tbl_acct_inf0022,
ht_tbl_acct_inf_def  tbl_acct_infG026,
FILE *fp)
{
    int     nReturnCode;
    char    sDateTime[14+1];
    double  amt;
    
    stHost_Acct_def  sthost_acct;
    memset(&sthost_acct,0x00,sizeof(sthost_acct));

    memset(sDateTime,0x00,sizeof(sDateTime));
    getSysTime(sDateTime);
    
    if( strncmp(dbtbl_acct_rslt->mcht_role, "1001",4) == 0 || 
        strncmp(dbtbl_acct_rslt->mcht_role, "1002",4) == 0 ||
        strncmp(dbtbl_acct_rslt->mcht_role, "1003",4) == 0 ||
        strncmp(dbtbl_acct_rslt->mcht_role, "1004",4) == 0 )
    {
//        amt = (dbtbl_acct_rslt->debt_at - dbtbl_acct_rslt->cret_at)/100;
//        if(amt < 0.0)
//        {
//            
//            if(strncmp(dbtbl_acct_rslt->acct_statu, "8", 1)== 0)
//            {
//                /*运营商存管账户G026-->机构往来户0026*/
//                memset(&sthost_acct,0x00,sizeof(sthost_acct));
//                rtrim(dbtbl_acct_rslt->mcht_rslt_no);
//                rtrim(dbtbl_acct_rslt->mcht_role);
//                sprintf(sthost_acct.seq_no,     "%s%s%s",    dbtbl_acct_rslt->sett_date, dbtbl_acct_rslt->mcht_rslt_no,dbtbl_acct_rslt->mcht_role);      /*结算流水号*/
//                memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
//                memcpy(sthost_acct.chl_id,     "二清",    sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
//                memcpy(sthost_acct.trans_date, dbtbl_acct_rslt->sett_date,  sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
//                memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
//                memcpy(sthost_acct.bank_code,  tbl_acct_infG026.bank_code, sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
//                memcpy(sthost_acct.acct_type,  tbl_acct_infG026.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
//                memcpy(sthost_acct.acct_no,    tbl_acct_infG026.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
//                memcpy(sthost_acct.acct_name,  tbl_acct_infG026.acct_name, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
//                memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* 币种 */  
//                sthost_acct.amt = -(dbtbl_acct_rslt->debt_at - dbtbl_acct_rslt->cret_at)/100;                                                   /*交易金额*/
//                memcpy(sthost_acct.bank_code1, tbl_acct_inf0026.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */        
//                memcpy(sthost_acct.acct_type1, tbl_acct_inf0026.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
//                memcpy(sthost_acct.acct_no1,   tbl_acct_inf0026.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
//                memcpy(sthost_acct.acct_name1, tbl_acct_inf0026.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */     
//                memcpy(sthost_acct.dc_flag,    "11",                    sizeof(sthost_acct.dc_flag)-1); 
//                memcpy(sthost_acct.param_1,    "",                      sizeof(sthost_acct.param_1)-1);                                  
//                    
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
//            }
//            else
//            {
//                /*运营商存管账户0041-->机构往来户G026*/
//                memset(&sthost_acct,0x00,sizeof(sthost_acct));
//                rtrim(dbtbl_acct_rslt->mcht_rslt_no);
//                rtrim(dbtbl_acct_rslt->mcht_role);
//                sprintf(sthost_acct.seq_no,     "%s%s%s",    dbtbl_acct_rslt->sett_date, dbtbl_acct_rslt->mcht_rslt_no,dbtbl_acct_rslt->mcht_role);      /*结算流水号*/
//                memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
//                memcpy(sthost_acct.chl_id,     "二清",    sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
//                memcpy(sthost_acct.trans_date, dbtbl_acct_rslt->sett_date,  sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
//                memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
//                memcpy(sthost_acct.bank_code,  tbl_acct_inf0041.bank_code, sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
//                memcpy(sthost_acct.acct_type,  tbl_acct_inf0041.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
//                memcpy(sthost_acct.acct_no,    tbl_acct_inf0041.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
//                memcpy(sthost_acct.acct_name,  tbl_acct_inf0041.acct_name, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
//                memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* 币种 */  
//                sthost_acct.amt = -(dbtbl_acct_rslt->debt_at - dbtbl_acct_rslt->cret_at)/100;                                                   /*交易金额*/
//                memcpy(sthost_acct.bank_code1, tbl_acct_infG026.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */        
//                memcpy(sthost_acct.acct_type1, tbl_acct_infG026.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
//                memcpy(sthost_acct.acct_no1,   tbl_acct_infG026.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
//                memcpy(sthost_acct.acct_name1, tbl_acct_infG026.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */     
//                memcpy(sthost_acct.dc_flag,    "11",                    sizeof(sthost_acct.dc_flag)-1); 
//                memcpy(sthost_acct.param_1,    "",                      sizeof(sthost_acct.param_1)-1);                                  
//                    
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
//            }
//        }    
    }

    if(strncmp(dbtbl_acct_rslt->mcht_role, "1004",4) == 0)
    {
//        /*运营商存管账户0022-->机构往来户0026*/
//        memset(&sthost_acct,0x00,sizeof(sthost_acct));
//        rtrim(dbtbl_acct_rslt->mcht_rslt_no);
//        rtrim(dbtbl_acct_rslt->mcht_role);
//        sprintf(sthost_acct.seq_no,     "%s%s%s",    dbtbl_acct_rslt->sett_date, dbtbl_acct_rslt->mcht_rslt_no,dbtbl_acct_rslt->mcht_role);      /*结算流水号*/
//        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
//        memcpy(sthost_acct.chl_id,     "二清",    sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
//        memcpy(sthost_acct.trans_date, dbtbl_acct_rslt->sett_date,  sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
//        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
//        memcpy(sthost_acct.bank_code,  tbl_acct_inf0022.bank_code, sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
//        memcpy(sthost_acct.acct_type,  tbl_acct_inf0022.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
//        memcpy(sthost_acct.acct_no,    tbl_acct_inf0022.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
//        memcpy(sthost_acct.acct_name,  tbl_acct_inf0022.acct_name, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
//        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* 币种 */  
//        sthost_acct.amt = (dbtbl_acct_rslt->debt_at - dbtbl_acct_rslt->cret_at)/100;                                                   /*交易金额*/
//        memcpy(sthost_acct.bank_code1, tbl_acct_inf0026.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */        
//        memcpy(sthost_acct.acct_type1, tbl_acct_inf0026.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
//        memcpy(sthost_acct.acct_no1,   tbl_acct_inf0026.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
//        memcpy(sthost_acct.acct_name1, tbl_acct_inf0026.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */     
//        memcpy(sthost_acct.dc_flag,    "08",                    sizeof(sthost_acct.dc_flag)-1); 
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
        
    }    
    else if(strncmp(dbtbl_acct_rslt->mcht_role, "2004",4) == 0)
    {
        /*运营商存管账户0041-->机构往来户0026*/ // 有
        memset(&sthost_acct,0x00,sizeof(sthost_acct));
        rtrim(dbtbl_acct_rslt->mcht_rslt_no);
        rtrim(dbtbl_acct_rslt->mcht_role);
        sprintf(sthost_acct.seq_no,     "%s%s%s",    dbtbl_acct_rslt->sett_date, dbtbl_acct_rslt->mcht_rslt_no,dbtbl_acct_rslt->mcht_role);      /*结算流水号*/
        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
        memcpy(sthost_acct.chl_id,     "直连",    sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
        memcpy(sthost_acct.trans_date, dbtbl_acct_rslt->sett_date,  sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
        memcpy(sthost_acct.bank_code,  tbl_acct_inf0041.bank_code, sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
        memcpy(sthost_acct.acct_type,  tbl_acct_inf0041.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
        memcpy(sthost_acct.acct_no,    tbl_acct_inf0041.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
        memcpy(sthost_acct.acct_name,  tbl_acct_inf0041.acct_name, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* 币种 */  
        sthost_acct.amt = (dbtbl_acct_rslt->debt_at - dbtbl_acct_rslt->cret_at)/100;                                                   /*交易金额*/
        memcpy(sthost_acct.bank_code1, tbl_acct_inf0026.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */        
        memcpy(sthost_acct.acct_type1, tbl_acct_inf0026.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
        memcpy(sthost_acct.acct_no1,   tbl_acct_inf0026.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
        memcpy(sthost_acct.acct_name1, tbl_acct_inf0026.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */     
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
    else if(strncmp(dbtbl_acct_rslt->mcht_role, "1003",4) == 0)
    {
        /*运营商存管账户0022-->机构往来户0041*/
        memset(&sthost_acct,0x00,sizeof(sthost_acct));
        rtrim(dbtbl_acct_rslt->mcht_rslt_no);
        rtrim(dbtbl_acct_rslt->mcht_role);
        sprintf(sthost_acct.seq_no,     "%s%s%s",    dbtbl_acct_rslt->sett_date, dbtbl_acct_rslt->mcht_rslt_no,dbtbl_acct_rslt->mcht_role);      /*结算流水号*/
        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
        memcpy(sthost_acct.chl_id,     "二清",    sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
        memcpy(sthost_acct.trans_date, dbtbl_acct_rslt->sett_date,  sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
        memcpy(sthost_acct.bank_code,  tbl_acct_inf0022.bank_code, sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
        memcpy(sthost_acct.acct_type,  tbl_acct_inf0022.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
        memcpy(sthost_acct.acct_no,    tbl_acct_inf0022.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
        memcpy(sthost_acct.acct_name,  tbl_acct_inf0022.acct_name, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* 币种 */  
        sthost_acct.amt = (dbtbl_acct_rslt->debt_at - dbtbl_acct_rslt->cret_at)/100;                                                   /*交易金额*/
        memcpy(sthost_acct.bank_code1, tbl_acct_inf0041.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */        
        memcpy(sthost_acct.acct_type1, tbl_acct_inf0041.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
        memcpy(sthost_acct.acct_no1,   tbl_acct_inf0041.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
        memcpy(sthost_acct.acct_name1, tbl_acct_inf0041.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */     
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
    else if(strncmp(dbtbl_acct_rslt->mcht_role, "2003",4) == 0)
    {
        /*运营商存管账户0022-->机构往来户0041*/
//        memset(&sthost_acct,0x00,sizeof(sthost_acct));
//        rtrim(dbtbl_acct_rslt->mcht_rslt_no);
//        rtrim(dbtbl_acct_rslt->mcht_role);
//        sprintf(sthost_acct.seq_no,     "%s%s%s",    dbtbl_acct_rslt->sett_date, dbtbl_acct_rslt->mcht_rslt_no,dbtbl_acct_rslt->mcht_role);      /*结算流水号*/
//        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
//        memcpy(sthost_acct.chl_id,     "直连",    sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
//        memcpy(sthost_acct.trans_date, dbtbl_acct_rslt->sett_date,  sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
//        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
//        memcpy(sthost_acct.bank_code,  tbl_acct_inf0022.bank_code, sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
//        memcpy(sthost_acct.acct_type,  tbl_acct_inf0022.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
//        memcpy(sthost_acct.acct_no,    tbl_acct_inf0022.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
//        memcpy(sthost_acct.acct_name,  tbl_acct_inf0022.acct_name, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
//        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* 币种 */  
//        sthost_acct.amt = (dbtbl_acct_rslt->debt_at - dbtbl_acct_rslt->cret_at)/100;                                                   /*交易金额*/
//        memcpy(sthost_acct.bank_code1, tbl_acct_inf0041.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */        
//        memcpy(sthost_acct.acct_type1, tbl_acct_inf0041.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
//        memcpy(sthost_acct.acct_no1,   tbl_acct_inf0041.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
//        memcpy(sthost_acct.acct_name1, tbl_acct_inf0041.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */     
//        memcpy(sthost_acct.dc_flag,    "08",                    sizeof(sthost_acct.dc_flag)-1); 
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
        
    }            
    else
    {       
        HtLog(gLogFile,HT_LOG_MODE_DEBUG,__FILE__,__LINE__,"不需要记账角色[%s]",dbtbl_acct_rslt->mcht_role);         
    }          
    return 0;
}


int VarRecdToPlat(char* strBuf, char cTab, tbl_sett_list_def* vtdbl_sett_list);

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

int RecordWithd(ht_tbl_acct_inf_def  tbl_acct_inf0041,ht_tbl_acct_inf_def  tbl_acct_inf0022,FILE *fp, char* sett_date)
{
	 char    sDateTime[14+1];
	 getSysTime(sDateTime);
	 
	// 查询提现平台分润
	ht_tbl_sett_flow_def  tbl_sett_flow;
	memset(&tbl_sett_flow, 0, sizeof(tbl_sett_flow));
	memcpy(tbl_sett_flow.sett_date, sett_date, 8);
	memcpy(tbl_sett_flow.trans_tp, "1301", 4);
	double share_profit_plat = DbsTblSettFlow(DBS_SELECT, &tbl_sett_flow, 0, 0);

	
	    stHost_Acct_def  sthost_acct;
        memset(&sthost_acct,0x00,sizeof(sthost_acct));
        sprintf(sthost_acct.seq_no,     "%s%s%s",   sett_date, sDateTime+8, "00000001");      /*结算流水号*/
        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
        memcpy(sthost_acct.chl_id,     "二清",    sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
        memcpy(sthost_acct.trans_date, sett_date,  sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
        memcpy(sthost_acct.bank_code,  tbl_acct_inf0041.bank_code, sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
        memcpy(sthost_acct.acct_type,  tbl_acct_inf0041.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
        memcpy(sthost_acct.acct_no,    tbl_acct_inf0041.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
        memcpy(sthost_acct.acct_name,  tbl_acct_inf0041.acct_name, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* 币种 */  
        sthost_acct.amt = share_profit_plat/100;                                                   /*交易金额*/
        memcpy(sthost_acct.bank_code1, tbl_acct_inf0022.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */        
        memcpy(sthost_acct.acct_type1, tbl_acct_inf0022.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
        memcpy(sthost_acct.acct_no1,   tbl_acct_inf0022.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
        memcpy(sthost_acct.acct_name1, tbl_acct_inf0022.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */     
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

	// 查询提现分润方分润
	memset(&tbl_sett_flow, 0, sizeof(tbl_sett_flow));
	memcpy(tbl_sett_flow.sett_date, sett_date, 8);
	memcpy(tbl_sett_flow.trans_tp, "1301", 4);
	double share_profit_mcht_no = DbsTblSettFlow(DBS_SELECT2, &tbl_sett_flow, 0, 0);

        memset(&sthost_acct,0x00,sizeof(sthost_acct));
        sprintf(sthost_acct.seq_no,     "%s%s%s",   sett_date, sDateTime+8, "00000002");      /*结算流水号*/
        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*调用方系统ID*/
        memcpy(sthost_acct.chl_id,     "二清",    sizeof(sthost_acct.chl_id)-1);      /* 渠道 */
        memcpy(sthost_acct.trans_date, sett_date,  sizeof(sthost_acct.trans_date)-1);  /*交易日期*/
        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*交易时间*/
        memcpy(sthost_acct.bank_code,  tbl_acct_inf0041.bank_code, sizeof(sthost_acct.bank_code)-1);   /*转出交易行号 */
        memcpy(sthost_acct.acct_type,  tbl_acct_inf0041.acct_type, sizeof(sthost_acct.acct_type)-1);   /* 转出账号类型 */
        memcpy(sthost_acct.acct_no,    tbl_acct_inf0041.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* 转出账号 */
        memcpy(sthost_acct.acct_name,  tbl_acct_inf0041.acct_name, sizeof(sthost_acct.acct_name)-1);   /* 转出方户名 */
        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* 币种 */  
        sthost_acct.amt = share_profit_mcht_no/100;                                                   /*交易金额*/
        memcpy(sthost_acct.bank_code1, tbl_acct_inf0022.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* 转入交易行号 */        
        memcpy(sthost_acct.acct_type1, tbl_acct_inf0022.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* 转入账号类型 */
        memcpy(sthost_acct.acct_no1,   tbl_acct_inf0022.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* 转入账号 */
        memcpy(sthost_acct.acct_name1, tbl_acct_inf0022.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* 转入方户名 */     
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
		return 0;
}

/*************************************************
*函数名称∶ Total_7400
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_7400()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_7400
*函数功能∶ 结算明细表虚拟卡记录生成划账文件
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_7400 ( int nBeginOffset, int nEndOffset )
{
    int     nReturnCode;
    char    sDateTime[14+1];
    char    stlm_dt[8+1];
    char    *pstr;
    char    sFileRecord[RECORD_LEN_MAX];
    FILE    *fp;
    char    sFileFullName[200];
    char    sTmpFileName[100];
    int		nTotalCount = 0;
    
    ht_tbl_acct_rslt_def   dbtbl_acct_rslt;
    tbl_file_info_def   dbtbl_file_info;
    ht_tbl_acct_inf_def  tbl_acct_inf0022;
    ht_tbl_acct_inf_def  tbl_acct_inf0026;
    ht_tbl_acct_inf_def  tbl_acct_inf0041;
    ht_tbl_acct_inf_def  tbl_acct_infG026;


    memset(&tbl_acct_inf0041,0x0,sizeof(tbl_acct_inf0041));
    memcpy(tbl_acct_inf0041.type,"10",2);
    nReturnCode = DbsTblAcctInf(DBS_SELECT, &tbl_acct_inf0041);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAcctInf [%s] err[%d]",tbl_acct_inf0041.type,nReturnCode);
        return -1;
    } 
    
    memset(&tbl_acct_inf0026,0x0,sizeof(tbl_acct_inf0026));
    memcpy(tbl_acct_inf0026.type,"07",2);
    nReturnCode = DbsTblAcctInf(DBS_SELECT, &tbl_acct_inf0026);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAcctInf [%s] err[%d]",tbl_acct_inf0026.type,nReturnCode);
        return -1;
    }   
 
    memset(&tbl_acct_inf0022,0x0,sizeof(tbl_acct_inf0022));
    memcpy(tbl_acct_inf0022.type,"03",2);
    nReturnCode = DbsTblAcctInf(DBS_SELECT, &tbl_acct_inf0022);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAcctInf [%s] err[%d]",tbl_acct_inf0022.type,nReturnCode);
        return -1;
    }
    
    memset(&tbl_acct_infG026,0x0,sizeof(tbl_acct_infG026));
//    memcpy(tbl_acct_infG026.type,"13",2);
//    nReturnCode = DbsTblAcctInf(DBS_SELECT, &tbl_acct_infG026);
//    if (nReturnCode)
//    {
//        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAcctInf [%s] err[%d]",tbl_acct_infG026.type,nReturnCode);
//        return -1;
//    }               
    /* 初始文件信息 */
    memset(&dbtbl_file_info, 0x00, sizeof(dbtbl_file_info));
    memset(sDateTime, 0x00, sizeof(sDateTime));
    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
    
    getSysTime(sDateTime);
    memcpy(dbtbl_file_info.sett_date, stlm_dt, 8);
    memcpy(dbtbl_file_info.snd_time, sDateTime, 14);
    
    memset(sTmpFileName, 0, sizeof(sTmpFileName));
    strcpy(sTmpFileName, "HOST_YYYYMMDD_XX_ACCT.txt");
    pstr = strstr(sTmpFileName, "YYYYMMDD");
    if (pstr)
        memcpy(pstr, stlm_dt, 8);
    pstr = strstr(sTmpFileName, "XX");
    if (pstr)
        memcpy (pstr, "05", 2);
    memcpy(dbtbl_file_info.step_no, "04", 2);
    memcpy(dbtbl_file_info.batch_no, "05", 2);
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
    memcpy(dbtbl_file_info.file_name, sFileFullName, strlen(sFileFullName));
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "file_name : [%s]", dbtbl_file_info.file_name);
    
    fp = fopen(dbtbl_file_info.file_name,"w+");
    if (fp == NULL)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen [%s] error.", dbtbl_file_info.file_name);
        return -1;
    }

    memset(&dbtbl_acct_rslt, 0, sizeof(dbtbl_acct_rslt));
    memcpy(dbtbl_acct_rslt.acct_date, stlm_dt, 8);
    memcpy(dbtbl_acct_rslt.acct_statu, "N", 1);       
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "acct_date:[%s], mcht_role:[%s] acct_statu[%s].", dbtbl_acct_rslt.acct_date, "2001|2002|1003|2003|1004|2004",dbtbl_acct_rslt.acct_statu);
    
    nReturnCode = DbsTblAcctRslt(DBS_CURSOR3, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);	
	nReturnCode = DbsTblAcctRslt(DBS_OPEN3, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
	if (nReturnCode)
	{
	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN3 tbl_rslt_sett_cur1 error [%d]", nReturnCode);
	    fclose (fp);
	    return -1;
	}
	
	while(1)
	{
	    memset(&dbtbl_acct_rslt, 0x0, sizeof(dbtbl_acct_rslt));
	    nReturnCode = DbsTblAcctRslt(DBS_FETCH3, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);   
        if (nReturnCode && nReturnCode != DBS_FETCHNULL)
        {
            if (nReturnCode == DBS_NOTFOUND)
            {
                break;
            }

	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_FETCH3 tbl_rslt_sett_cur_1 error [%d]", nReturnCode);
	        DbsTblAcctRslt(DBS_CLOSE3, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
	        fclose (fp);
            return -1;
        }	    
        if(dbtbl_acct_rslt.debt_at <= 0.0000001 && 
            dbtbl_acct_rslt.debt_at >= -0.0000001 &&
            dbtbl_acct_rslt.cret_at <= 0.0000001 && 
            dbtbl_acct_rslt.cret_at >= -0.0000001)
        {
            HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "debt_at 0 [%f]cret_at 0[%f]",dbtbl_acct_rslt.debt_at ,dbtbl_acct_rslt.cret_at,dbtbl_acct_rslt.cret_at);
            continue;
        }
        
        if(strncmp(dbtbl_acct_rslt.mcht_role, "2001",4) == 0 || 
            strncmp(dbtbl_acct_rslt.mcht_role, "2002",4) == 0 ||
            strncmp(dbtbl_acct_rslt.acct_statu, "Y", 1)== 0)
        {
            HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "mcht_role [%s]acct_statu[%s]",dbtbl_acct_rslt.mcht_role,dbtbl_acct_rslt.acct_statu);
            continue;
        }        
        HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "mcht_role [%s] debt_at[%f] cret_at[%f]",dbtbl_acct_rslt.mcht_role,dbtbl_acct_rslt.debt_at);
        iWriteHostFile(&dbtbl_acct_rslt, tbl_acct_inf0026,tbl_acct_inf0041,tbl_acct_inf0022,tbl_acct_infG026,fp);

        if( strncmp(dbtbl_acct_rslt.mcht_role, "1001",4) == 0 || 
            strncmp(dbtbl_acct_rslt.mcht_role, "1002",4) == 0 ||
            strncmp(dbtbl_acct_rslt.mcht_role, "1003",4) == 0 ||
            strncmp(dbtbl_acct_rslt.mcht_role, "1004",4) == 0 )
        {
            if(dbtbl_acct_rslt.debt_at < dbtbl_acct_rslt.cret_at)
            {
                if(strncmp(dbtbl_acct_rslt.acct_statu, "8", 1)== 0)
                {
                     RightTrim(dbtbl_acct_rslt.mcht_rslt_no);
            	    RightTrim(dbtbl_acct_rslt.mcht_role);
            	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_rslt_no:[%s].", dbtbl_acct_rslt.mcht_rslt_no);
            	    memcpy(dbtbl_acct_rslt.acct_statu, "Y", 1);
            	    nReturnCode = DbsTblAcctRslt(DBS_UPDATE, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
            	    if (nReturnCode == DBS_NOTFOUND)
            	    {
            	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPDATE ht_tbl_acct_rslt error [%d]", nReturnCode);
            	        DbsTblAcctRslt(DBS_CLOSE3, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
            	        fclose (fp);
            	        return -1;
            	    }                 
                }
                else
                {
                    RightTrim(dbtbl_acct_rslt.mcht_rslt_no);
            	    RightTrim(dbtbl_acct_rslt.mcht_role);
            	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_rslt_no:[%s].", dbtbl_acct_rslt.mcht_rslt_no);
            	    memcpy(dbtbl_acct_rslt.acct_statu, "7", 1);
            	    nReturnCode = DbsTblAcctRslt(DBS_UPDATE, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
            	    if (nReturnCode == DBS_NOTFOUND)
            	    {
            	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPDATE ht_tbl_acct_rslt error [%d]", nReturnCode);
            	        DbsTblAcctRslt(DBS_CLOSE3, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
            	        fclose (fp);
            	        return -1;
            	    }
	                 
                }
                continue;                                  
            }
            else
            {       
                if(strncmp(dbtbl_acct_rslt.mcht_role, "1001",4) == 0 || 
                    strncmp(dbtbl_acct_rslt.mcht_role, "1002",4) == 0 )
                {
                    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "mcht_role [%s]核心记账不更新状态",dbtbl_acct_rslt.mcht_role);
                    continue;
                }
            }  
        }
         
    	nTotalCount++;
	    RightTrim(dbtbl_acct_rslt.mcht_rslt_no);
	    RightTrim(dbtbl_acct_rslt.mcht_role);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_rslt_no:[%s].", dbtbl_acct_rslt.mcht_rslt_no);
	    memcpy(dbtbl_acct_rslt.acct_statu, "9", 1);
	    nReturnCode = DbsTblAcctRslt(DBS_UPDATE, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
	    if (nReturnCode == DBS_NOTFOUND)
	    {
	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPDATE ht_tbl_acct_rslt error [%d]", nReturnCode);
	        DbsTblAcctRslt(DBS_CLOSE3, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
	        fclose (fp);
	        return -1;
	    }
	}
	DbsTblAcctRslt(DBS_CLOSE3, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);

	// 记录提现平台分润
	nReturnCode = RecordWithd(tbl_acct_inf0041, tbl_acct_inf0022, fp, stlm_dt);
    fclose(fp);
    
    memcpy(dbtbl_file_info.status, "0", 1);        
    nReturnCode = iInsertTblFileInfo(dbtbl_file_info);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "iInsertTblFileInfo error [%d]", nReturnCode);
        return -1;
    }
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nTotalCount:[%d]", nTotalCount);
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_7400 Succ .");

    return 0;
}

int VarRecdToPlat(char* strBuf, char cTab, tbl_sett_list_def* vtdbl_sett_list)
{
    int     nReturnCode;
    char    sRcdBuf[RECORD_LEN_MAX] = {0};
    char    tmpBuf[50] = {0};
    char    sDelim[1];
    char    sDateTime[14+1];
    
    memset(sDelim, 0, sizeof(sDelim));
    sDelim[0]=cTab;
    
    RightTrim(vtdbl_sett_list->seq_no);
    strncat(sRcdBuf, vtdbl_sett_list->seq_no, strlen(vtdbl_sett_list->seq_no));                         /*结算流水号*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    strncat(sRcdBuf, "TOPCPG", 6);
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    RightTrim(vtdbl_sett_list->chl_id);
    strncat(sRcdBuf, vtdbl_sett_list->chl_id, strlen(vtdbl_sett_list->chl_id));                         /* 渠道 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    getSysTime(sDateTime);
    strncat(sRcdBuf, sDateTime, 8);                                                                     /*交易日期*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    strncat(sRcdBuf, sDateTime+8, 6);                                                                   /*交易时间*/
    strncat(sRcdBuf, sDelim, strlen(sDelim));

    RightTrim(vtdbl_sett_list->plat_sett_bank);
    strncat(sRcdBuf, vtdbl_sett_list->plat_sett_bank, strlen(vtdbl_sett_list->plat_sett_bank));                   /* 转出交易行号 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_sett_list->plat_sett_acct_type);
    strncat(sRcdBuf, "0022", 4);                   /* 转出账号类型 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_sett_list->plat_sett_acct);
    strncat(sRcdBuf, vtdbl_sett_list->plat_sett_acct, strlen(vtdbl_sett_list->plat_sett_acct));                       /* 转出账号 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_sett_list->plat_sett_acct_nm);
    strncat(sRcdBuf, vtdbl_sett_list->plat_sett_acct_nm, strlen(vtdbl_sett_list->plat_sett_acct_nm));                   /* 转出方户名 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    strncat(sRcdBuf, "156", 3);                                                                         /* 币种 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    sprintf(sRcdBuf+strlen(sRcdBuf), "%.2f", vtdbl_sett_list->amt);										/*交易金额*/
    strncat(sRcdBuf, sDelim, strlen(sDelim)); 

    RightTrim(vtdbl_sett_list->mcht_sett_bank);
    strncat(sRcdBuf, vtdbl_sett_list->mcht_sett_bank, strlen(vtdbl_sett_list->mcht_sett_bank));                 /* 转入交易行号 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_sett_list->mcht_sett_acct_type);
    strncat(sRcdBuf, "0031", 4);                 /* 转入账号类型 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_sett_list->mcht_sett_xingmin_acct);
    strncat(sRcdBuf, vtdbl_sett_list->mcht_sett_xingmin_acct, strlen(vtdbl_sett_list->mcht_sett_acct));                     /* 转入账号 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    
    RightTrim(vtdbl_sett_list->mcht_sett_acct_nm);
    strncat(sRcdBuf, vtdbl_sett_list->mcht_sett_acct_nm, strlen(vtdbl_sett_list->mcht_sett_acct_nm));                 /* 转入方户名 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    strncat(sRcdBuf, "02", 2);                                                                          /* 交易类型 */
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    strncat(sRcdBuf, sDelim, strlen(sDelim));
    strcat(sRcdBuf, "\n");
    memcpy(strBuf, sRcdBuf, strlen(sRcdBuf));
    
    return 0;
}


/****************************** end of Task_7400.c ******************************/


