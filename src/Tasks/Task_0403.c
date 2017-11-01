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
 *  文 件 名: Task_0403.c
 *  功    能: 导入支付宝对账流水
 *  编程人员: wyl
 *  开发时间: 2016-12-29
 *  备    注:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include <iconv.h>
#include "batch.h"
#include "Common.h"

extern char	gLogFile[LOG_NAME_LEN_MAX];
extern  tbl_date_inf_def dbtbl_date_inf;
extern	char ext_inter_brh_code[10 + 1];

static int code_convert(char *from_charset, char *to_charset, char *inbuf, long inlen, char *outbuf, size_t *outlen);
static int u2g(char *inbuf,int inlen,char *outbuf,size_t outlen);
static int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen);

static int code_convert(char *from_charset, char *to_charset, char *inbuf, long inlen, char *outbuf, size_t *outlen)
{

	iconv_t cd;
	int rc;
	int iOutLen = *outlen;
	char **pin = &inbuf;
	char **pout = &outbuf;
	cd = iconv_open(to_charset, from_charset);
	if (cd == 0)
	{
		HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "iconv_open error");
		return -1;
	}
	memset(outbuf, 0x00, iOutLen);
	if (iconv(cd, pin, &inlen, pout, outlen) == -1)
	{
		HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "iconv error");
		return -1;
	}
	*outlen = strlen(*pout);
	iconv_close(cd);
	return 0;
}
//UNICODE码转为GB2312码
static int u2g(char *inbuf,int inlen,char *outbuf,size_t outlen)
{
	return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,&outlen);
}
//GB2312码转为UNICODE码
static int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
	return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,&outlen);
}
/*************************************************
*函数名称∶ Total_0403
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_0403()
{
    int		nTotalNum = 0;
    char      sHostTxnNum[2 + 1];
    char	sFileRecord[RECORD_LEN_MAX];
    char    *pstr;
    char	alipay_file_name[200] = {0};
    FILE *fp;
    stExtnFileRegDef StExtnFileReg;

     /* 初始文件信息 */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    memcpy(StExtnFileReg.host_name, "ALPAY", 5);
    strcpy(StExtnFileReg.src_file_name, "aliPayFile_YYYYMMDD.dat");

    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);
    pstr = strstr (StExtnFileReg.src_file_name, "XX");
    if (pstr)
        memcpy (pstr, ext_inter_brh_code+4, 2);

    getExtnFileFullName(&StExtnFileReg, alipay_file_name);

    /* 取文件行数 */
    fp = fopen (alipay_file_name, "r");
    if (!fp)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen %s error.", alipay_file_name);
        return -1;
    }

    while(fgets( sFileRecord, RECORD_LEN_MAX, fp) != NULL)
    {
        if( strlen( sFileRecord) == 0)
            continue;
        nTotalNum++;
    }

    fclose (fp);

    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "nTotalNum =[%d].", nTotalNum);

    return nTotalNum;
}

/*************************************************
*函数名称∶ Task_0403
*函数功能∶
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_0403 ( int nBeginOffset, int nEndOffset )
{
    int		iRet = 0;
    int	        i  = 1, nTotCount=0;
    char    *pstr;
    char	alipay_file_name[200] = {0};
    char	sFileRecord[RECORD_LEN_MAX];
    char	sgFileRecord[RECORD_LEN_MAX];
    char    sTmp[RECORD_LEN_MAX] = {0};
    char    sDelim[] = ",";
    char    sDateTime[22+1] = {0};
    FILE   *fp;
    stExtnFileRegDef StExtnFileReg;
    ht_tbl_wift_txn_def ht_tbl_wift_txn;

    double dtrans_at = 0;
    double dtrans_at2 = 0;
    double dtrans_at3 = 0;
    double dtrans_at4 = 0;
    double dtrans_at5 = 0;
    
     /* 初始文件信息 */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    memcpy(StExtnFileReg.host_name, "ALPAY", 5);
    strcpy(StExtnFileReg.src_file_name, "aliPayFile_YYYYMMDD.dat");

    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);
    pstr = strstr (StExtnFileReg.src_file_name, "XX");
    if (pstr)
        memcpy (pstr, ext_inter_brh_code+4, 2);

    getExtnFileFullName(&StExtnFileReg, alipay_file_name);

    fp = fopen (alipay_file_name, "r");
    if (!fp)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen %s error", alipay_file_name);
        return -1;
    }

    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "nBeginOffset=[%d] nEndOffset=[%d].", nBeginOffset, nEndOffset);

    /* 置文件起始点 */
    while (i < nBeginOffset && fgets( sFileRecord, RECORD_LEN_MAX, fp) != NULL)
    {
        if (strlen(sFileRecord) == 0)
        {
            continue;
        }

        i++;
    }

    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "start point = [%d].", i);

    while (i < nEndOffset + 1)
    {
        memset(sFileRecord, 0x00, sizeof(sFileRecord));
        memset(sgFileRecord, 0x00, sizeof(sgFileRecord));
        memset(&ht_tbl_wift_txn, 0x00, sizeof(ht_tbl_wift_txn));
        if (fgets(sgFileRecord, RECORD_LEN_MAX, fp) == NULL)
        {
            break;
        }
        /*u2g(sgFileRecord,strlen(sgFileRecord),sFileRecord,sizeof(sFileRecord));*/
        strcpy(sFileRecord, sgFileRecord);
        if((memcmp(sFileRecord, "`", 1) != 0) || (strlen(sFileRecord) <= 50))
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "continue sFileRecord:[%s].", sFileRecord);
            continue;
        }
        
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sFileRecord:[%s].", sFileRecord);
        
        i++;
        nTotCount++;

        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//交易时间
        strcpy(sDateTime, sTmp+1);
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sDateTime:[%s].", sDateTime);
        memcpy(ht_tbl_wift_txn.trans_date, sDateTime, 4);
        memcpy(ht_tbl_wift_txn.trans_date+4, sDateTime+5, 2);
        memcpy(ht_tbl_wift_txn.trans_date+6, sDateTime+8, 2);
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "ht_tbl_wift_txn.trans_date:[%s].", ht_tbl_wift_txn.trans_date);
        memcpy(ht_tbl_wift_txn.trans_time, sDateTime+11, 2);
        memcpy(ht_tbl_wift_txn.trans_time, sDateTime+11, 2);
        memcpy(ht_tbl_wift_txn.trans_time+2, sDateTime+14, 2);
        memcpy(ht_tbl_wift_txn.trans_time+4, sDateTime+17, 2);
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "ht_tbl_wift_txn.trans_time:[%s].", ht_tbl_wift_txn.trans_time);

        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//公众账号ID
        strcpy(ht_tbl_wift_txn.gener_id, sTmp+1);

        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//第三方商户号
        strcpy(ht_tbl_wift_txn.thr_mcht_no, sTmp+1);
         
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//商户号
        strcpy(ht_tbl_wift_txn.ext_mcht_no, sTmp+1);
        
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//子商户号
        strcpy(ht_tbl_wift_txn.ex_ext_mcht_no, sTmp+1);
        
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//设备编号
        u2g(sTmp+1,strlen(sTmp+1),ht_tbl_wift_txn.equipment_id,sizeof(ht_tbl_wift_txn.equipment_id));
        
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//威富通订单号
        strcpy(ht_tbl_wift_txn.wehat_mcht_no, sTmp+1);
 
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//第三方订单号
        strcpy(ht_tbl_wift_txn.thr_order_no, sTmp+1);
      
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//商户订单号
        strcpy(ht_tbl_wift_txn.order_no, sTmp+1);
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "order_no:[%s].", ht_tbl_wift_txn.order_no);
        sprintf(ht_tbl_wift_txn.plat_key, "%8.8s%s", ht_tbl_wift_txn.trans_date, ht_tbl_wift_txn.order_no);
        memcpy(ht_tbl_wift_txn.plat_key+4, ht_tbl_wift_txn.order_no+3, 4);
        RightTrim(ht_tbl_wift_txn.plat_key);
        
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim); //用户标识
        strcpy(ht_tbl_wift_txn.user_id, sTmp+1);       
        
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//交易类型
        strcpy(ht_tbl_wift_txn.wift_trans_tp, sTmp+1);
        
        
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//交易状态
        u2g(sTmp+1,strlen(sTmp+1),ht_tbl_wift_txn.trans_state,sizeof(ht_tbl_wift_txn.trans_state));
        
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//付款银行  
        strcpy(ht_tbl_wift_txn.pay_bank, sTmp+1);      
        
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim); //货币种类   
        if(memcmp(sTmp, "CNY", 3) == 0) 
        {
            strcpy(ht_tbl_wift_txn.ccy, "156");  
        }
        else
        {
            strcpy(ht_tbl_wift_txn.ccy, sTmp+1);    
        }
        
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//总金额
        strcpy(ht_tbl_wift_txn.trans_at, sTmp+1);
        RightTrim(ht_tbl_wift_txn.trans_at);
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "test:[%s].", ht_tbl_wift_txn.trans_at);             
        dtrans_at = atof(ht_tbl_wift_txn.trans_at);
        dtrans_at = floor(dtrans_at * 100 + 0.50001);
        sprintf(ht_tbl_wift_txn.trans_at, "%012.0lf", dtrans_at);
        
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim); //企业红包金额
        strcpy(ht_tbl_wift_txn.trans_at2, sTmp+1);
        RightTrim(ht_tbl_wift_txn.trans_at2);
        dtrans_at2 = atof(ht_tbl_wift_txn.trans_at2);
        dtrans_at2 = floor(dtrans_at2 * 100 + 0.50001);
        sprintf(ht_tbl_wift_txn.trans_at2, "%012.0lf", dtrans_at2);
              
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//威富通退款单号
        strcpy(ht_tbl_wift_txn.wx_refund_order, sTmp+1);
        if(strlen(ht_tbl_wift_txn.wx_refund_order) == 0)
        {
            sprintf(ht_tbl_wift_txn.wx_refund_order, "%8.8s111161411111", ht_tbl_wift_txn.trans_date);          
        }
                
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim); //商户退款单号
        strcpy(ht_tbl_wift_txn.mcht_refund_order, sTmp+1);       

                
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim); //退款金额
        strcpy(ht_tbl_wift_txn.trans_at3, sTmp+1);
        RightTrim(ht_tbl_wift_txn.trans_at3);
        dtrans_at3 = atof(ht_tbl_wift_txn.trans_at3);
        dtrans_at3 = floor(dtrans_at3 * 100 + 0.50001);
        sprintf(ht_tbl_wift_txn.trans_at3, "%012.0lf", dtrans_at3);
        
               
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);  
        strcpy(ht_tbl_wift_txn.trans_at4, sTmp+1);//企业红包退款金额
        RightTrim(ht_tbl_wift_txn.trans_at4);
        dtrans_at4 = atof(ht_tbl_wift_txn.trans_at4);
        dtrans_at4 = floor(dtrans_at4 * 100 + 0.50001);
        sprintf(ht_tbl_wift_txn.trans_at4, "%012.0lf", dtrans_at4);        
            
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//退款类型
        strcpy(ht_tbl_wift_txn.refund_type, sTmp+1);
        
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//退款状态
        u2g(sTmp+1,strlen(sTmp+1),ht_tbl_wift_txn.refund_status,sizeof(ht_tbl_wift_txn.refund_status));        
        
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//商品名称
        u2g(sTmp+1,strlen(sTmp+1),ht_tbl_wift_txn.goods_name,sizeof(ht_tbl_wift_txn.goods_name));
        memset(sTmp, 0, sizeof(sTmp));
        
        getValueByDelim(sFileRecord, sTmp, sDelim);//商户数据包   
        u2g(sTmp+1,strlen(sTmp+1),ht_tbl_wift_txn.mcht_data,sizeof(ht_tbl_wift_txn.mcht_data));
            
        memset(sTmp, 0, sizeof(sTmp));        
        getValueByDelim(sFileRecord, sTmp, sDelim);//手续费
        strcpy(ht_tbl_wift_txn.wx_fee, sTmp+1);
        
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//费率
        strcpy(ht_tbl_wift_txn.wx_rate, sTmp+1);
        
        memset(sTmp, 0, sizeof(sTmp));        
        getValueByDelim(sFileRecord, sTmp, sDelim);//终端类型
       
        memset(sTmp, 0, sizeof(sTmp));        
        getValueByDelim(sFileRecord, sTmp, sDelim);//对账标识

        memset(sTmp, 0, sizeof(sTmp));        
        getValueByDelim(sFileRecord, sTmp, sDelim);//门店编号

        memset(sTmp, 0, sizeof(sTmp));        
        getValueByDelim(sFileRecord, sTmp, sDelim);//商户名称
 
        memset(sTmp, 0, sizeof(sTmp));        
        getValueByDelim(sFileRecord, sTmp, sDelim);//收银员
 
        memset(sTmp, 0, sizeof(sTmp));        
        getValueByDelim(sFileRecord, sTmp, sDelim);//子商户ID

        memset(sTmp, 0, sizeof(sTmp));        
        getValueByDelim(sFileRecord, ht_tbl_wift_txn.trans_at5, sDelim);//免充值券金额

        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);  
        strcpy(ht_tbl_wift_txn.trans_at5, sTmp+1);//免充值券金额
        RightTrim(ht_tbl_wift_txn.trans_at5);
        dtrans_at5 = atof(ht_tbl_wift_txn.trans_at5);
        dtrans_at5 = floor(dtrans_at5 * 100 + 0.50001);
        sprintf(ht_tbl_wift_txn.trans_at5, "%012.0lf", dtrans_at5);   
        
        memset(sTmp, 0, sizeof(sTmp));        
        getValueByDelim(sFileRecord, sTmp, sDelim);//实收金额

        memset(sTmp, 0, sizeof(sTmp));        
        getValueByDelim(sFileRecord, sTmp, sDelim);//扩展字段1

        memset(sTmp, 0, sizeof(sTmp));        
        getValueByDelim(sFileRecord, sTmp, sDelim);//扩展字段2

        memset(sTmp, 0, sizeof(sTmp));        
        getValueByDelim(sFileRecord, sTmp, sDelim);//扩展字段3

        memset(sTmp, 0, sizeof(sTmp));        
        getValueByDelim(sFileRecord, sTmp, sDelim);//扩展字段4        
        
        strcpy(ht_tbl_wift_txn.pan, "0000000000000000000");
        strcpy(ht_tbl_wift_txn.trans_tp, "0000");
        sprintf(ht_tbl_wift_txn.bus_tp, "%4.4s%4.4s", BT_BUS_TP_ALIPAY, BT_SETT_TRAN_D);
  
        memcpy(ht_tbl_wift_txn.inter_brh_code, ext_inter_brh_code, sizeof(ht_tbl_wift_txn.inter_brh_code)-1);
        memcpy(ht_tbl_wift_txn.sett_date, dbtbl_date_inf.stoday, sizeof(ht_tbl_wift_txn.sett_date)-1);
        memcpy(ht_tbl_wift_txn.flag_result, BT_FLAG_RESULT_NULL, BT_FLAG_RESULT_LEN);
        memcpy(ht_tbl_wift_txn.batch_no,"01", 2);
        
        
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "-----------------test[%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%d]---------", 
        ht_tbl_wift_txn.inter_brh_code,   
        ht_tbl_wift_txn.sett_date ,       
        ht_tbl_wift_txn.trans_date,       
        ht_tbl_wift_txn.trans_time ,      
        ht_tbl_wift_txn.flag_result,      
        ht_tbl_wift_txn.gener_id  ,       
        ht_tbl_wift_txn.ext_mcht_no ,     
        ht_tbl_wift_txn.ex_ext_mcht_no   ,
        ht_tbl_wift_txn.equipment_id  ,   
        ht_tbl_wift_txn.wehat_mcht_no ,   
        ht_tbl_wift_txn.order_no  ,       
        ht_tbl_wift_txn.user_id  ,        
        ht_tbl_wift_txn.plat_key ,        
        ht_tbl_wift_txn.batch_no  ,       
        ht_tbl_wift_txn.trans_tp  ,       
        ht_tbl_wift_txn.trans_state  ,    
        ht_tbl_wift_txn.pay_bank   ,      
        ht_tbl_wift_txn.wx_refund_order  ,
        ht_tbl_wift_txn.mcht_refund_order,
        ht_tbl_wift_txn.refund_type  ,    
        ht_tbl_wift_txn.refund_status   , 
        ht_tbl_wift_txn.goods_name ,      
        ht_tbl_wift_txn.mcht_data ,       
        ht_tbl_wift_txn.wx_fee   ,        
        ht_tbl_wift_txn.wx_rate  ,        
        ht_tbl_wift_txn.acct_type  ,      
        ht_tbl_wift_txn.pan    ,          
        ht_tbl_wift_txn.ccy    ,          
        ht_tbl_wift_txn.trans_at   ,      
        ht_tbl_wift_txn.trans_at2   ,     
        ht_tbl_wift_txn.trans_at3   ,     
        ht_tbl_wift_txn.trans_at4    ,    
        ht_tbl_wift_txn.trans_at5  ,     
        ht_tbl_wift_txn.seq_num         
        );
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "inter_brh_code[%s]",ht_tbl_wift_txn.inter_brh_code); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sett_date[%s]",ht_tbl_wift_txn.sett_date); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "trans_date[%s]",ht_tbl_wift_txn.trans_date); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "trans_time[%s]",ht_tbl_wift_txn.trans_time); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "flag_result[%s]",ht_tbl_wift_txn.flag_result); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "gener_id[%s]",ht_tbl_wift_txn.gener_id); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "ext_mcht_no[%s]",ht_tbl_wift_txn.ext_mcht_no); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "ex_ext_mcht_no[%s]",ht_tbl_wift_txn.ex_ext_mcht_no); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "equipment_id[%s]",ht_tbl_wift_txn.equipment_id); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "wehat_mcht_no[%s]",ht_tbl_wift_txn.wehat_mcht_no); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "order_no[%s]",ht_tbl_wift_txn.order_no); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "user_id[%s]",ht_tbl_wift_txn.user_id); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "plat_key[%s]",ht_tbl_wift_txn.plat_key); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "batch_no[%s]",ht_tbl_wift_txn.batch_no); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "wift_trans_tp[%s]",ht_tbl_wift_txn.wift_trans_tp); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "trans_tp[%s]",ht_tbl_wift_txn.trans_tp); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "trans_state[%s]",ht_tbl_wift_txn.trans_state); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "pay_bank[%s]",ht_tbl_wift_txn.pay_bank); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "wx_refund_order[%s]",ht_tbl_wift_txn.wx_refund_order); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "mcht_refund_order[%s]",ht_tbl_wift_txn.mcht_refund_order); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "refund_type[%s]",ht_tbl_wift_txn.refund_type); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "refund_status[%s]",ht_tbl_wift_txn.refund_status); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "goods_name[%s]",ht_tbl_wift_txn.goods_name); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "mcht_data[%s]",ht_tbl_wift_txn.mcht_data); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "wx_fee[%s]",ht_tbl_wift_txn.wx_fee); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "wx_rate[%s]",ht_tbl_wift_txn.wx_rate); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "acct_type[%s]",ht_tbl_wift_txn.acct_type); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "pan[%s]",ht_tbl_wift_txn.pan); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "ccy[%s]",ht_tbl_wift_txn.ccy); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "trans_at[%s]",ht_tbl_wift_txn.trans_at); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "trans_at2[%s]",ht_tbl_wift_txn.trans_at2); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "trans_at3[%s]",ht_tbl_wift_txn.trans_at3); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "trans_at4[%s]",ht_tbl_wift_txn.trans_at4); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "trans_at5[%s]",ht_tbl_wift_txn.trans_at5); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "seq_num[%s]",ht_tbl_wift_txn.seq_num); 
   
        iRet = DbsTblWiftTxn(DBS_INSERT, &ht_tbl_wift_txn,0,0);
        if (iRet != 0 && iRet != DBS_KEYDUPLICATE)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblWiftTxn DBS_INSERT error, %d.", iRet);
            fclose (fp);
            return -1;
        }
        else if(iRet == DBS_KEYDUPLICATE)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblWiftTxn DBS_KEYDUPLICATE");
        }
    }
    
    fclose (fp);
    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "total num [%d].", nTotCount);
    HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_0403 Succ .");
    return 0;
}

/****************************** end of Task_0403.c ******************************/
