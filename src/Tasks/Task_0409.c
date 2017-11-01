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
 *  文 件 名: Task_0409.c
 *  功    能: 导入富友微信支付宝对账流水
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
*函数名称∶ Total_0409
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_0409()
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
    //memcpy(StExtnFileReg.host_name, "ALPAY", 5);
    strcpy(StExtnFileReg.src_file_name, "fuiouFile_YYYYMMDD_TXN.txt");

    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);

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
*函数名称∶ Task_0409
*函数功能∶
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_0409 ( int nBeginOffset, int nEndOffset )
{
    int		iRet = 0;
    int	        i  = 1, nTotCount=0;
    char    *pstr;
    char	alipay_file_name[200] = {0};
    char	sFileRecord[RECORD_LEN_MAX];
    char	sgFileRecord[RECORD_LEN_MAX];
    char    sTmp[RECORD_LEN_MAX] = {0};
    char    sDelim[] = "|";
    char    sDateTime[22+1] = {0};
    FILE   *fp;
    stExtnFileRegDef StExtnFileReg;
    tbl_alipay_wechat_def tbl_alipay_wechat;

    double dtrans_at = 0;
    double dtrans_at2 = 0;
    double dtrans_at3 = 0;
    double dtrans_at4 = 0;
    double dtrans_at5 = 0;
    
     /* 初始文件信息 */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    //memcpy(StExtnFileReg.host_name, "ALPAY", 5);
    strcpy(StExtnFileReg.src_file_name, "fuiouFile_YYYYMMDD_TXN.txt");

    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);

    getExtnFileFullName(&StExtnFileReg, alipay_file_name);

    fp = fopen (alipay_file_name, "r");
    if (!fp)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen %s error", alipay_file_name);
        return -1;
    }

    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "nBeginOffset=[%d] nEndOffset=[%d].", nBeginOffset, nEndOffset);

    /* 置文件起始点 */
	memset(sFileRecord, 0x00, sizeof(sFileRecord));
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
        memset(&tbl_alipay_wechat, 0x00, sizeof(tbl_alipay_wechat));
//		if (1 == i)
//		{
//			if (fgets(sgFileRecord, RECORD_LEN_MAX, fp) == NULL)
//            {
//                break;
//            }
//		}
		memset(sgFileRecord, 0x00, sizeof(sgFileRecord));
        if (fgets(sgFileRecord, RECORD_LEN_MAX, fp) == NULL)
        {
            break;
        }
        /*u2g(sgFileRecord,strlen(sgFileRecord),sFileRecord,sizeof(sFileRecord));*/
        strcpy(sFileRecord, sgFileRecord);   
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sFileRecord:[%s].", sFileRecord);
        
        i++;
        nTotCount++;

        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//商户号
        strcpy(tbl_alipay_wechat.ext_mcht_no, sTmp);

		memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//卡号
        strcpy(tbl_alipay_wechat.pan, sTmp);

        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//卡属性

		memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//参考号

        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//富友交易类型 微信/支付宝
        strcpy(tbl_alipay_wechat.thr_trans_tp, sTmp);

        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//终端号
		strcpy(tbl_alipay_wechat.equipment_id, sTmp);

        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//商户订单号
        strcpy(tbl_alipay_wechat.order_no, sTmp);

        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//渠道订单号
        strcpy(tbl_alipay_wechat.thr_order_no, sTmp);

        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//交易金额
        strcpy(tbl_alipay_wechat.trans_at, sTmp);
		RightTrim(tbl_alipay_wechat.trans_at);
		dtrans_at = atof(tbl_alipay_wechat.trans_at);
        dtrans_at = floor(dtrans_at * 100 + 0.50001);
        sprintf(tbl_alipay_wechat.trans_at, "%012.0lf", dtrans_at);

        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//交易状态		

        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//交易状态说明
		strcpy(tbl_alipay_wechat.trans_state, sTmp);

        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//交易时间
        strcpy(sDateTime, sTmp);
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sDateTime:[%s].", sDateTime);
        memcpy(tbl_alipay_wechat.trans_date, sDateTime, 4);
        memcpy(tbl_alipay_wechat.trans_date+4, sDateTime+5, 2);
        memcpy(tbl_alipay_wechat.trans_date+6, sDateTime+8, 2);
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "tbl_alipay_wechat.trans_date:[%s].", tbl_alipay_wechat.trans_date);
        memcpy(tbl_alipay_wechat.trans_time, sDateTime+11, 2);
        memcpy(tbl_alipay_wechat.trans_time+2, sDateTime+14, 2);
        memcpy(tbl_alipay_wechat.trans_time+4, sDateTime+17, 2);
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "tbl_alipay_wechat.trans_time:[%s].", tbl_alipay_wechat.trans_time);

        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//清算日期
		strcpy(tbl_alipay_wechat.sett_date, sTmp);

        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim); //商户退款单号
        strcpy(tbl_alipay_wechat.mcht_refund_order, sTmp);     

        memset(sTmp, 0, sizeof(sTmp));        
        getValueByDelim(sFileRecord, sTmp, sDelim);//手续费
        strcpy(tbl_alipay_wechat.wx_fee, sTmp);
		RightTrim(tbl_alipay_wechat.wx_fee);
		dtrans_at = atof(tbl_alipay_wechat.wx_fee);
        dtrans_at = floor(dtrans_at * 100 + 0.50001);
        sprintf(tbl_alipay_wechat.wx_fee, "%012.0lf", dtrans_at);

		sprintf(tbl_alipay_wechat.plat_key, "%8.8s%s", tbl_alipay_wechat.trans_date, tbl_alipay_wechat.order_no);
        //memcpy(tbl_alipay_wechat.plat_key+4, tbl_alipay_wechat.order_no+3, 4);
		CommonRTrim(tbl_alipay_wechat.plat_key);
        //RightTrim(tbl_alipay_wechat.plat_key);


       
        strcpy(tbl_alipay_wechat.trans_tp, "0000");
        //sprintf(tbl_alipay_wechat.bus_tp, "%4.4s%4.4s", BT_BUS_TP_ALIPAY, BT_SETT_TRAN_D);
  
        memcpy(tbl_alipay_wechat.inter_brh_code, ext_inter_brh_code, sizeof(tbl_alipay_wechat.inter_brh_code)-1);
        memcpy(tbl_alipay_wechat.flag_result, BT_FLAG_RESULT_NULL, BT_FLAG_RESULT_LEN);
        memcpy(tbl_alipay_wechat.batch_no,"01", 2);
        
        
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "-----------------test[%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%d]---------", 
        tbl_alipay_wechat.inter_brh_code,   
        tbl_alipay_wechat.sett_date ,       
        tbl_alipay_wechat.trans_date,       
        tbl_alipay_wechat.trans_time ,      
        tbl_alipay_wechat.flag_result,      
        tbl_alipay_wechat.gener_id  ,       
        tbl_alipay_wechat.ext_mcht_no ,     
        tbl_alipay_wechat.ex_ext_mcht_no   ,
        tbl_alipay_wechat.equipment_id  ,   
        tbl_alipay_wechat.order_no  ,       
        tbl_alipay_wechat.user_id  ,        
        tbl_alipay_wechat.plat_key ,        
        tbl_alipay_wechat.batch_no  ,       
        tbl_alipay_wechat.trans_tp  ,       
        tbl_alipay_wechat.trans_state  ,    
        tbl_alipay_wechat.pay_bank   ,      
        tbl_alipay_wechat.thr_trans_tp  ,
        tbl_alipay_wechat.mcht_refund_order,
        tbl_alipay_wechat.refund_type  ,    
        tbl_alipay_wechat.refund_status   , 
        tbl_alipay_wechat.goods_name ,      
        tbl_alipay_wechat.mcht_data ,       
        tbl_alipay_wechat.wx_fee   ,        
        tbl_alipay_wechat.wx_rate  ,        
        tbl_alipay_wechat.acct_type  ,      
        tbl_alipay_wechat.pan    ,          
        tbl_alipay_wechat.ccy    ,          
        tbl_alipay_wechat.trans_at   ,      
        tbl_alipay_wechat.trans_at2   ,     
        tbl_alipay_wechat.trans_at3   ,     
        tbl_alipay_wechat.trans_at4    ,    
        tbl_alipay_wechat.trans_at5  ,     
        tbl_alipay_wechat.seq_num         
        );
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "inter_brh_code[%s]",tbl_alipay_wechat.inter_brh_code); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sett_date[%s]",tbl_alipay_wechat.sett_date); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "trans_date[%s]",tbl_alipay_wechat.trans_date); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "trans_time[%s]",tbl_alipay_wechat.trans_time); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "flag_result[%s]",tbl_alipay_wechat.flag_result); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "gener_id[%s]",tbl_alipay_wechat.gener_id); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "ext_mcht_no[%s]",tbl_alipay_wechat.ext_mcht_no); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "ex_ext_mcht_no[%s]",tbl_alipay_wechat.ex_ext_mcht_no); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "equipment_id[%s]",tbl_alipay_wechat.equipment_id); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "order_no[%s]",tbl_alipay_wechat.order_no); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "user_id[%s]",tbl_alipay_wechat.user_id); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "plat_key[%s]",tbl_alipay_wechat.plat_key); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "batch_no[%s]",tbl_alipay_wechat.batch_no); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "trans_tp[%s]",tbl_alipay_wechat.trans_tp); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "trans_state[%s]",tbl_alipay_wechat.trans_state); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "pay_bank[%s]",tbl_alipay_wechat.pay_bank); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "thr_trans_tp[%s]",tbl_alipay_wechat.thr_trans_tp); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "mcht_refund_order[%s]",tbl_alipay_wechat.mcht_refund_order); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "refund_type[%s]",tbl_alipay_wechat.refund_type); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "refund_status[%s]",tbl_alipay_wechat.refund_status); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "goods_name[%s]",tbl_alipay_wechat.goods_name); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "mcht_data[%s]",tbl_alipay_wechat.mcht_data); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "wx_fee[%s]",tbl_alipay_wechat.wx_fee); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "wx_rate[%s]",tbl_alipay_wechat.wx_rate); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "acct_type[%s]",tbl_alipay_wechat.acct_type); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "pan[%s]",tbl_alipay_wechat.pan); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "ccy[%s]",tbl_alipay_wechat.ccy); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "trans_at[%s]",tbl_alipay_wechat.trans_at); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "trans_at2[%s]",tbl_alipay_wechat.trans_at2); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "trans_at3[%s]",tbl_alipay_wechat.trans_at3); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "trans_at4[%s]",tbl_alipay_wechat.trans_at4); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "trans_at5[%s]",tbl_alipay_wechat.trans_at5); 
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "seq_num[%s]",tbl_alipay_wechat.seq_num); 
   
        iRet = DbsTblAlipayWechatTxn(DBS_INSERT, &tbl_alipay_wechat,0,0);
        if (iRet != 0 && iRet != DBS_KEYDUPLICATE)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAlipayWechatTxn DBS_INSERT error, %d.", iRet);
            fclose (fp);
            return -1;
        }
        else if(iRet == DBS_KEYDUPLICATE)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAlipayWechatTxn DBS_KEYDUPLICATE");
        }
    }
    
    fclose (fp);
    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "total num [%d].", nTotCount);
    HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_0409 Succ .");
    return 0;
}

/****************************** end of Task_0409.c ******************************/
