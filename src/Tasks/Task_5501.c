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
 *  文 件 名: Task_5501.c
 *  功    能: 商户入账数据准备
 *  编程人员: HELU
 *  开发时间: 2016-12-24
 *  备    注:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include "batch.h"

extern char	gLogFile[LOG_NAME_LEN_MAX];
extern  tbl_date_inf_def dbtbl_date_inf;
extern	char ext_inter_brh_code[10 + 1];

static int in_acct_rslt(ht_tbl_sett_rslt_def *tbl_sett_rslt)
{
    int iRet;

    ht_tbl_acct_rslt_def tbl_acct_rslt;
    ht_tbl_mcht_inf_def tbl_mcht_inf;
    ht_tbl_brh_inf_def  tbl_brh_inf;
    ht_tbl_sett_rslt_def tbl_sett_rslt_tmp;

    memset(&tbl_mcht_inf, 0x00, sizeof(tbl_mcht_inf));
    memset(&tbl_acct_rslt, 0x00, sizeof(tbl_acct_rslt));
    
    memcpy(&tbl_sett_rslt_tmp, tbl_sett_rslt, sizeof(tbl_sett_rslt_tmp));
    
    memcpy(tbl_sett_rslt_tmp.mcht_no, tbl_sett_rslt->mcht_no, sizeof(tbl_sett_rslt_tmp.mcht_no));
    memcpy(tbl_sett_rslt_tmp.mcht_role , tbl_sett_rslt->mcht_role, sizeof(tbl_sett_rslt_tmp.mcht_role));
    memcpy(tbl_sett_rslt_tmp.sett_date, tbl_sett_rslt->sett_date, sizeof(tbl_sett_rslt_tmp.sett_date));
    
    CommonRTrim(tbl_sett_rslt_tmp.mcht_no);
    CommonRTrim(tbl_sett_rslt_tmp.mcht_role);
    CommonRTrim(tbl_sett_rslt_tmp.sett_date);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_no[%s]mcht_role[%s]sett_date[%s]", tbl_sett_rslt_tmp.mcht_no, tbl_sett_rslt_tmp.mcht_role,tbl_sett_rslt_tmp.sett_date);
    
    iRet = DbsTblSettRslt(DBS_SELECT, &tbl_sett_rslt_tmp, 0, 0);
    if (iRet != 0)
    {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAcctRslt DBS_SELECT error, %d.", iRet);
            return -1;
    }
    
    strcpy(tbl_acct_rslt.inter_brh_code, BT_BRH_ID_CORE);
    memcpy(tbl_acct_rslt.sett_date, tbl_sett_rslt->sett_date, sizeof(tbl_acct_rslt.sett_date)-1);
    memcpy(tbl_acct_rslt.acct_date,  tbl_sett_rslt_tmp.acct_date, sizeof(tbl_acct_rslt.acct_date)-1);

    memcpy(tbl_acct_rslt.batch_no, tbl_sett_rslt->batch_no, sizeof(tbl_acct_rslt.batch_no)-1);    
    memcpy(tbl_acct_rslt.mcht_no, tbl_sett_rslt->mcht_no, sizeof(tbl_acct_rslt.mcht_no)-1);
    memcpy(tbl_acct_rslt.mcht_role, tbl_sett_rslt->mcht_role, sizeof(tbl_acct_rslt.mcht_role)-1);
    tbl_acct_rslt.trans_num=tbl_sett_rslt->trans_num;
    tbl_acct_rslt.debt_at = tbl_sett_rslt->debt_at;
    tbl_acct_rslt.cret_at = tbl_sett_rslt->cret_at;
    tbl_acct_rslt.debt_net_fee = tbl_sett_rslt->debt_net_fee;
    tbl_acct_rslt.cret_net_fee = tbl_sett_rslt->cret_net_fee;
    memcpy(tbl_acct_rslt.sett_md, tbl_sett_rslt_tmp.sett_md, sizeof(tbl_acct_rslt.sett_md)-1);
    memcpy(tbl_acct_rslt.mcht_sett_bank, tbl_sett_rslt_tmp.mcht_sett_bank, sizeof(tbl_acct_rslt.mcht_sett_bank)-1);
    memcpy(tbl_acct_rslt.mcht_sett_bank_nm, tbl_sett_rslt_tmp.mcht_sett_bank_nm, sizeof(tbl_acct_rslt.mcht_sett_bank_nm)-1);
    memcpy(tbl_acct_rslt.mcht_sett_acct_type, tbl_sett_rslt_tmp.mcht_sett_acct_type, sizeof(tbl_acct_rslt.mcht_sett_acct_type)-1);
    memcpy(tbl_acct_rslt.mcht_sett_acct, tbl_sett_rslt_tmp.mcht_sett_acct, sizeof(tbl_acct_rslt.mcht_sett_acct)-1);
    memcpy(tbl_acct_rslt.mcht_sett_acct_nm, tbl_sett_rslt_tmp.mcht_sett_acct_nm, sizeof(tbl_acct_rslt.mcht_sett_acct_nm)-1);
    memcpy(tbl_acct_rslt.mcht_sett_xingmin_acct, tbl_sett_rslt_tmp.mcht_sett_xingmin_acct, sizeof(tbl_acct_rslt.mcht_sett_xingmin_acct)-1);   

    memcpy(tbl_acct_rslt.mcht_debit_bank, tbl_sett_rslt_tmp.mcht_debit_bank, sizeof(tbl_acct_rslt.mcht_debit_bank)-1);
    memcpy(tbl_acct_rslt.mcht_debit_bank_nm, tbl_sett_rslt_tmp.mcht_debit_bank_nm, sizeof(tbl_acct_rslt.mcht_debit_bank_nm)-1);
    memcpy(tbl_acct_rslt.mcht_debit_acct_type, tbl_sett_rslt_tmp.mcht_debit_acct_type, sizeof(tbl_acct_rslt.mcht_debit_acct_type)-1);
    memcpy(tbl_acct_rslt.mcht_debit_acct, tbl_sett_rslt_tmp.mcht_debit_acct, sizeof(tbl_acct_rslt.mcht_debit_acct)-1);
    memcpy(tbl_acct_rslt.mcht_debit_acct_nm, tbl_sett_rslt_tmp.mcht_debit_acct_nm, sizeof(tbl_acct_rslt.mcht_debit_acct_nm)-1);  

    memcpy(tbl_acct_rslt.plat_debit_bank, tbl_sett_rslt_tmp.plat_debit_bank, sizeof(tbl_acct_rslt.plat_sett_bank)-1);
    strcpy(tbl_acct_rslt.plat_debit_bank_nm, tbl_sett_rslt_tmp.plat_debit_bank_nm);
    memcpy(tbl_acct_rslt.plat_debit_acct_type, tbl_sett_rslt_tmp.plat_debit_acct_type, sizeof(tbl_acct_rslt.plat_sett_acct_type)-1);
    memcpy(tbl_acct_rslt.plat_debit_acct, tbl_sett_rslt_tmp.plat_debit_acct, sizeof(tbl_acct_rslt.plat_sett_acct)-1);
    memcpy(tbl_acct_rslt.plat_debit_acct_nm, tbl_sett_rslt_tmp.plat_debit_acct_nm, sizeof(tbl_acct_rslt.plat_sett_acct_nm)-1);

	memcpy(tbl_acct_rslt.plat_sett_bank, tbl_sett_rslt_tmp.plat_sett_bank, sizeof(tbl_acct_rslt.plat_sett_bank)-1);
    strcpy(tbl_acct_rslt.plat_sett_bank_nm, tbl_sett_rslt_tmp.plat_sett_bank_nm);
    memcpy(tbl_acct_rslt.plat_sett_acct_type, tbl_sett_rslt_tmp.plat_sett_acct_type, sizeof(tbl_acct_rslt.plat_sett_acct_type)-1);
    memcpy(tbl_acct_rslt.plat_sett_acct, tbl_sett_rslt_tmp.plat_sett_acct, sizeof(tbl_acct_rslt.plat_sett_acct)-1);
    memcpy(tbl_acct_rslt.plat_sett_acct_nm, tbl_sett_rslt_tmp.plat_sett_acct_nm, sizeof(tbl_acct_rslt.plat_sett_acct_nm)-1);
    
    /*
    memcpy(tbl_acct_rslt.plat_debit_bank, tbl_sett_rslt_tmp.plat_debit_bank, sizeof(tbl_acct_rslt.plat_debit_bank)-1);
    memcpy(tbl_acct_rslt.plat_debit_acct_type, tbl_sett_rslt_tmp.plat_debit_acct_type, sizeof(tbl_acct_rslt.plat_debit_acct_type)-1);
    memcpy(tbl_acct_rslt.plat_debit_acct, tbl_sett_rslt_tmp.plat_debit_acct, sizeof(tbl_acct_rslt.plat_debit_acct)-1);
    memcpy(tbl_acct_rslt.plat_debit_acct_nm, tbl_sett_rslt_tmp.plat_debit_acct_nm, sizeof(tbl_acct_rslt.plat_debit_acct_nm)-1);
    */
    
    memcpy(tbl_acct_rslt.acct_statu, "N", 1);
    memcpy(tbl_acct_rslt.sz_status, "I", 1);
    memcpy(tbl_acct_rslt.batch_no, "01", 2);
    
    CommonRTrim(tbl_acct_rslt.sett_date);
    CommonRTrim(tbl_acct_rslt.mcht_no);
    CommonRTrim(tbl_acct_rslt.mcht_role);
    CommonRTrim(tbl_acct_rslt.batch_no);
    CommonRTrim(tbl_acct_rslt.mcht_sett_bank_nm);
    CommonRTrim(tbl_acct_rslt.mcht_sett_acct_nm);
    CommonRTrim(tbl_acct_rslt.plat_sett_bank_nm);
    CommonRTrim(tbl_acct_rslt.plat_sett_acct_nm);
    CommonRTrim(tbl_acct_rslt.mcht_debit_bank_nm);
    CommonRTrim(tbl_acct_rslt.mcht_debit_acct_nm);
    CommonRTrim(tbl_acct_rslt.plat_debit_bank_nm);
    CommonRTrim(tbl_acct_rslt.plat_debit_acct_nm);


	CommonRTrim(tbl_acct_rslt.plat_sett_bank);
    CommonRTrim(tbl_acct_rslt.plat_sett_bank_nm);
    CommonRTrim(tbl_acct_rslt.plat_sett_acct_type);
    CommonRTrim(tbl_acct_rslt.plat_sett_acct);
    CommonRTrim(tbl_acct_rslt.plat_sett_acct_nm);
    sprintf(tbl_acct_rslt.mcht_rslt_no, "%s%s%s%s", tbl_acct_rslt.sett_date, tbl_acct_rslt.mcht_no, tbl_acct_rslt.batch_no, tbl_acct_rslt.mcht_role);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_rslt_no[%s]", tbl_acct_rslt.mcht_rslt_no);

    iRet = DbsTblAcctRslt(DBS_INSERT, &tbl_acct_rslt, 0, 0);
    if (iRet != 0)
    {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAcctRslt DBS_INSERT error, %d.", iRet);
            return -1;
    }
    return 0;
}

/*************************************************
*函数名称∶ Total_5501
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_5501()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_5501
*函数功能∶
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_5501 ( int nBeginOffset, int nEndOffset )
{
    int	 iRet = 0, nTotCount=0;
    ht_tbl_sett_rslt_def tbl_sett_rslt;

    memset(&tbl_sett_rslt, 0x00, sizeof(tbl_sett_rslt));
    memcpy(tbl_sett_rslt.sett_date, dbtbl_date_inf.stoday, sizeof(tbl_sett_rslt.sett_date)-1);
    /*memcpy(tbl_sett_rslt.inter_brh_code, ext_inter_brh_code, sizeof(tbl_sett_rslt.inter_brh_code)-1);*/
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date[%s].inter_brh_code[%s]", tbl_sett_rslt.sett_date, tbl_sett_rslt.inter_brh_code);
    /*过滤充值、取现交易,支付机构(通过清算结果表生成流水入账)*/
    DbsTblSettRslt(DBS_CURSOR, &tbl_sett_rslt, nBeginOffset, nEndOffset);
    iRet = DbsTblSettRslt(DBS_OPEN, &tbl_sett_rslt, nBeginOffset, nEndOffset);
    if (iRet != 0)
    {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblCmpFlow DBS_OPEN error, %d.", iRet);
            return -1;
    }

    while (1)
    {
        memset(&tbl_sett_rslt, 0, sizeof(tbl_sett_rslt));
        iRet = DbsTblSettRslt(DBS_FETCH, &tbl_sett_rslt, nBeginOffset, nEndOffset);
        if (iRet)
        {
            if (iRet == DBS_NOTFOUND)
            {
                break;
            }

            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblSettRslt fetch sqlcode[%d]", iRet);
            DbsTblSettRslt(DBS_CLOSE, &tbl_sett_rslt, nBeginOffset, nEndOffset);
            return -1;
        }
        
        nTotCount++;
        
        iRet = in_acct_rslt(&tbl_sett_rslt);
        if (iRet != 0)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_dat_flow error, %d.", iRet);
            return -1;
        }
    }
    DbsTblSettRslt(DBS_CLOSE, &tbl_sett_rslt, nBeginOffset, nEndOffset);
    
    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "total num [%d].", nTotCount);


    nTotCount = 0;
	memset(&tbl_sett_rslt, 0x00, sizeof(tbl_sett_rslt));
    memcpy(tbl_sett_rslt.sett_date, dbtbl_date_inf.stoday, sizeof(tbl_sett_rslt.sett_date)-1);
    /*memcpy(tbl_sett_rslt.inter_brh_code, ext_inter_brh_code, sizeof(tbl_sett_rslt.inter_brh_code)-1);*/
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date[%s].inter_brh_code[%s]", tbl_sett_rslt.sett_date, tbl_sett_rslt.inter_brh_code);
    /*过滤充值,支付机构(通过清算结果表生成流水入账)*/
    DbsTblSettRslt(DBS_CURSOR2, &tbl_sett_rslt, nBeginOffset, nEndOffset);
    iRet = DbsTblSettRslt(DBS_OPEN2, &tbl_sett_rslt, nBeginOffset, nEndOffset);
    if (iRet != 0)
    {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblCmpFlow DBS_OPEN error, %d.", iRet);
            return -1;
    }

    while (1)
    {
        memset(&tbl_sett_rslt, 0, sizeof(tbl_sett_rslt));
        iRet = DbsTblSettRslt(DBS_FETCH2, &tbl_sett_rslt, nBeginOffset, nEndOffset);
        if (iRet)
        {
            if (iRet == DBS_NOTFOUND)
            {
                break;
            }

            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblSettRslt fetch sqlcode[%d]", iRet);
            DbsTblSettRslt(DBS_CLOSE2, &tbl_sett_rslt, nBeginOffset, nEndOffset);
            return -1;
        }
        
        nTotCount++;
        
        iRet = in_acct_rslt(&tbl_sett_rslt);
        if (iRet != 0)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_dat_flow error, %d.", iRet);
            return -1;
        }
    }
    DbsTblSettRslt(DBS_CLOSE2, &tbl_sett_rslt, nBeginOffset, nEndOffset);
    
    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "total num [%d].", nTotCount);
    
    nTotCount=0;
    memset(&tbl_sett_rslt, 0x00, sizeof(tbl_sett_rslt));
    memcpy(tbl_sett_rslt.sett_date, dbtbl_date_inf.stoday, sizeof(tbl_sett_rslt.sett_date)-1);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_date[%s].inter_brh_code[%s]", tbl_sett_rslt.sett_date, tbl_sett_rslt.inter_brh_code);
    /*过滤提现交易，生成支付机构手续费*/
    DbsTblSettRslt(DBS_CURSOR1, &tbl_sett_rslt, nBeginOffset, nEndOffset);
    iRet = DbsTblSettRslt(DBS_OPEN1, &tbl_sett_rslt, nBeginOffset, nEndOffset);
    if (iRet != 0)
    {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblCmpFlow DBS_OPEN error, %d.", iRet);
            return -1;
    }

    while (1)
    {
        memset(&tbl_sett_rslt, 0, sizeof(tbl_sett_rslt));
        iRet = DbsTblSettRslt(DBS_FETCH1, &tbl_sett_rslt, nBeginOffset, nEndOffset);
        if (iRet)
        {
            if (iRet == DBS_NOTFOUND)
            {
                break;
            }

            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblSettRslt fetch1 sqlcode[%d]", iRet);
            DbsTblSettRslt(DBS_CLOSE1, &tbl_sett_rslt, nBeginOffset, nEndOffset);
            return -1;
        }
        
        nTotCount++;
        
        iRet = in_acct_rslt(&tbl_sett_rslt);
        if (iRet != 0)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_dat_flow error, %d.", iRet);
            return -1;
        }
    }
    DbsTblSettRslt(DBS_CLOSE1, &tbl_sett_rslt, nBeginOffset, nEndOffset);
    
    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "total num [%d].", nTotCount);
    
    
    HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_5501 Succ .");

	
    return 0;
}
/****************************** end of Task_5501.c ******************************/



