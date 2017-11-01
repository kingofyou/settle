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
 *  文 件 名: Task_2201.c
 *  功    能: 对账结果到清分准备表
 *  编程人员: HELU
 *  开发时间: 2016-12-21
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
*函数名称∶ in_dat_flow
*函数功能∶插入清分表
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
static int in_dat_flow(ht_tbl_cmp_flow_def *ptbl_cmp_flow)
{
    ht_tbl_dat_flow_def tbl_dat_flow;
    int iRet = 0;
    memset(&tbl_dat_flow, 0x00, sizeof(tbl_dat_flow));
    memcpy(tbl_dat_flow.inter_brh_code,  ext_inter_brh_code, sizeof(tbl_dat_flow.inter_brh_code)-1);      
    memcpy(tbl_dat_flow.sett_date	,      ptbl_cmp_flow->sett_date,  sizeof(tbl_dat_flow.sett_date)-1);
    memcpy(tbl_dat_flow.trans_date	,      ptbl_cmp_flow->trans_date,  sizeof(tbl_dat_flow.trans_date)-1);
    memcpy(tbl_dat_flow.trans_time	,     ptbl_cmp_flow->trans_time,  sizeof(tbl_dat_flow.trans_time)-1);
    memcpy(tbl_dat_flow.flag_result	,      ptbl_cmp_flow->flag_result,  1);
    memcpy(tbl_dat_flow.stlm_flag	,    BT_DAT_STLM_FLAG_N,  BT_DAT_STLM_FLAG_LEN);
    memcpy(tbl_dat_flow.order_no	,     ptbl_cmp_flow->order_no,  sizeof(tbl_dat_flow.order_no)-1);
    memcpy(tbl_dat_flow.plat_key	,     ptbl_cmp_flow->plat_key,  sizeof(tbl_dat_flow.plat_key)-1);
    memcpy(tbl_dat_flow.batch_no	,     ptbl_cmp_flow->batch_no,  sizeof(tbl_dat_flow.batch_no)-1);
    memcpy(tbl_dat_flow.bus_tp	,      ptbl_cmp_flow->bus_tp,  sizeof(tbl_dat_flow.bus_tp)-1);
    memcpy(tbl_dat_flow.trans_tp	,     ptbl_cmp_flow->trans_tp,  sizeof(tbl_dat_flow.trans_tp)-1);
    memcpy(tbl_dat_flow.acct_type	,     ptbl_cmp_flow->acct_type,  sizeof(tbl_dat_flow.acct_type)-1);
    memcpy(tbl_dat_flow.trans_state	,     ptbl_cmp_flow->trans_state,  sizeof(tbl_dat_flow.trans_state)-1);
    memcpy(tbl_dat_flow.revsal_flag	,     ptbl_cmp_flow->revsal_flag,  sizeof(tbl_dat_flow.revsal_flag)-1);
    memcpy(tbl_dat_flow.key_revsal,      ptbl_cmp_flow->key_revsal,  sizeof(tbl_dat_flow.key_revsal)-1);
    memcpy(tbl_dat_flow.cancel_flag,     ptbl_cmp_flow->cancel_flag,  sizeof(tbl_dat_flow.cancel_flag)-1);
    memcpy(tbl_dat_flow.key_cancel,     ptbl_cmp_flow->key_cancel,  sizeof(tbl_dat_flow.key_cancel)-1);
    memcpy(tbl_dat_flow.pan	,            ptbl_cmp_flow->pan,  sizeof(tbl_dat_flow.pan)-1);
    memcpy(tbl_dat_flow.pan1	,        ptbl_cmp_flow->pan1,  sizeof(tbl_dat_flow.pan1)-1);
    memcpy(tbl_dat_flow.ccy	,            ptbl_cmp_flow->ccy,  sizeof(tbl_dat_flow.ccy)-1);
    memcpy(tbl_dat_flow.trans_at,         ptbl_cmp_flow->trans_at,  sizeof(tbl_dat_flow.trans_at)-1);
    memcpy(tbl_dat_flow.trans_at2,      ptbl_cmp_flow->trans_at2,  sizeof(tbl_dat_flow.trans_at2)-1);
    memcpy(tbl_dat_flow.trans_at3,      ptbl_cmp_flow->trans_at3,  sizeof(tbl_dat_flow.trans_at3)-1);
    memcpy(tbl_dat_flow.trans_at4,      ptbl_cmp_flow->trans_at4,  sizeof(tbl_dat_flow.trans_at4)-1);
    memcpy(tbl_dat_flow.trans_at5,       ptbl_cmp_flow->trans_at5,  sizeof(tbl_dat_flow.trans_at5)-1);
    memcpy(tbl_dat_flow.ext_mcht_no, ptbl_cmp_flow->ext_mcht_no,  sizeof(tbl_dat_flow.ext_mcht_no)-1);
    memcpy(tbl_dat_flow.sett_tp1,	    ptbl_cmp_flow->sett_tp1,  sizeof(tbl_dat_flow.sett_tp1)-1);
    memcpy(tbl_dat_flow.mcht_no1,	    ptbl_cmp_flow->mcht_no1,  sizeof(tbl_dat_flow.mcht_no1)-1);
    memcpy(tbl_dat_flow.sett_tp2,	    ptbl_cmp_flow->sett_tp2,  sizeof(tbl_dat_flow.sett_tp2)-1);
    memcpy(tbl_dat_flow.mcht_no2	,    ptbl_cmp_flow->mcht_no2,  sizeof(tbl_dat_flow.mcht_no2)-1);
    memcpy(tbl_dat_flow.sett_tp3,	    ptbl_cmp_flow->sett_tp3,  sizeof(tbl_dat_flow.sett_tp3)-1);
    memcpy(tbl_dat_flow.mcht_no3,	    ptbl_cmp_flow->mcht_no3,  sizeof(tbl_dat_flow.mcht_no3)-1);
    memcpy(tbl_dat_flow.sett_tp4,	    ptbl_cmp_flow->sett_tp4,  sizeof(tbl_dat_flow.sett_tp4)-1);
    memcpy(tbl_dat_flow.mcht_no4,	    ptbl_cmp_flow->mcht_no4,  sizeof(tbl_dat_flow.mcht_no4)-1);
    memcpy(tbl_dat_flow.sett_tp5,	    ptbl_cmp_flow->sett_tp5,  sizeof(tbl_dat_flow.sett_tp5)-1);
    memcpy(tbl_dat_flow.mcht_no5,	    ptbl_cmp_flow->mcht_no5,  sizeof(tbl_dat_flow.mcht_no5)-1);
    memcpy(tbl_dat_flow.sett_tp6,	    ptbl_cmp_flow->sett_tp6,  sizeof(tbl_dat_flow.sett_tp6)-1);
    memcpy(tbl_dat_flow.mcht_no6,	    ptbl_cmp_flow->mcht_no6,  sizeof(tbl_dat_flow.mcht_no6)-1);
    memcpy(tbl_dat_flow.sett_tp7,	    ptbl_cmp_flow->sett_tp7,  sizeof(tbl_dat_flow.sett_tp7)-1);
    memcpy(tbl_dat_flow.mcht_no7,	    ptbl_cmp_flow->mcht_no7,  sizeof(tbl_dat_flow.mcht_no7)-1);
    memcpy(tbl_dat_flow.sett_tp8,	    ptbl_cmp_flow->sett_tp8,  sizeof(tbl_dat_flow.sett_tp8)-1);
    memcpy(tbl_dat_flow.mcht_no8,	    ptbl_cmp_flow->mcht_no8,  sizeof(tbl_dat_flow.mcht_no8)-1);
    memcpy(tbl_dat_flow.sett_tp9,	    ptbl_cmp_flow->sett_tp9,  sizeof(tbl_dat_flow.sett_tp9)-1);
    memcpy(tbl_dat_flow.mcht_no9,	    ptbl_cmp_flow->mcht_no9,  sizeof(tbl_dat_flow.mcht_no9)-1); 
    memcpy(tbl_dat_flow.acct_name,	    ptbl_cmp_flow->acct_name,  sizeof(tbl_dat_flow.acct_name)-1); 
    CommonRTrim(tbl_dat_flow.acct_name);   
    
    iRet = DbsTblDatFlow(DBS_INSERT, &tbl_dat_flow);
    if (iRet != 0)
    {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblDatFlow DBS_INSERT error, %d.", iRet);
            return -1;
    }
    return 0;
}

/*************************************************
*函数名称∶ Total_2201
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_2201()
{
    int		nTotalNum = 0;
    int         iRet=0;
    ht_tbl_cmp_flow_def tbl_cmp_flow;

    memset(&tbl_cmp_flow, 0x00, sizeof(tbl_cmp_flow));
    memcpy(tbl_cmp_flow.sett_date, dbtbl_date_inf.stoday, sizeof(tbl_cmp_flow.sett_date)-1);
    memcpy(tbl_cmp_flow.inter_brh_code, ext_inter_brh_code, sizeof(tbl_cmp_flow.inter_brh_code)-1);
    iRet = DbsTblCmpFlow(DBS_SELECT1, &tbl_cmp_flow, 0, 0);
    if (iRet != 0)
    {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblCmpFlow DBS_SELECT1 error, %d.", iRet);
            return -1;
    }
    nTotalNum = tbl_cmp_flow.seq_num;
    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "nTotalNum =[%d].", nTotalNum);

    return nTotalNum;
}

/*************************************************
*函数名称∶ Task_2201
*函数功能∶
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_2201 ( int nBeginOffset, int nEndOffset )
{
    int	 iRet = 0, nTotCount=0;
    ht_tbl_cmp_flow_def tbl_cmp_flow;

    memset(&tbl_cmp_flow, 0x00, sizeof(tbl_cmp_flow));
    memcpy(tbl_cmp_flow.sett_date, dbtbl_date_inf.stoday, sizeof(tbl_cmp_flow.sett_date)-1);
    memcpy(tbl_cmp_flow.inter_brh_code, ext_inter_brh_code, sizeof(tbl_cmp_flow.inter_brh_code)-1);
    DbsTblCmpFlow(DBS_CURSOR, &tbl_cmp_flow, nBeginOffset, nEndOffset);
    iRet = DbsTblCmpFlow(DBS_OPEN, &tbl_cmp_flow, nBeginOffset, nEndOffset);
    if (iRet != 0)
    {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblCmpFlow DBS_OPEN error, %d.", iRet);
            return -1;
    }

    while (1)
    {
        memset(&tbl_cmp_flow, 0, sizeof(tbl_cmp_flow));
        iRet = DbsTblCmpFlow(DBS_FETCH, &tbl_cmp_flow, nBeginOffset, nEndOffset);
        if (iRet)
        {
            if (iRet == DBS_NOTFOUND)
            {
                break;
            }

            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblCmpFlow fetch sqlcode[%d]", iRet);
            DbsTblCmpFlow(DBS_CLOSE, &tbl_cmp_flow, nBeginOffset, nEndOffset);
            return -1;
        }
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "-----------------plat_key[%s]---------", tbl_cmp_flow.plat_key);
        nTotCount++;
        iRet = in_dat_flow(&tbl_cmp_flow);
        if (iRet != 0)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "in_dat_flow error, %d.", iRet);
            return -1;
        }
    }
    DbsTblCmpFlow(DBS_CLOSE, &tbl_cmp_flow, nBeginOffset, nEndOffset);
    
    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "total num [%d].", nTotCount);
    HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_2201 Succ .");
    return 0;
}
/****************************** end of Task_2201.c ******************************/

