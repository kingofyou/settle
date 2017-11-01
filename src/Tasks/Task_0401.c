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
 *  文 件 名: Task_0401.c
 *  功    能: 导入网关对账流水
 *  编程人员: HELU
 *  开发时间: 2016-12-14
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
*函数名称∶ Total_0401
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_0401()
{
    int		nTotalNum = 0;
    char      sHostTxnNum[2 + 1];
    char	sFileRecord[RECORD_LEN_MAX];
    char    *pstr;
    char	onlinepay_file_name[200] = {0};
    FILE *fp;
    stExtnFileRegDef StExtnFileReg;

     /* 初始文件信息 */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    memcpy(StExtnFileReg.host_name, "ONLINEPAY", 9);
    memcpy(StExtnFileReg.src_file_name, "ONLINEPAY_YYYYMMDD_XX.txt", 25);

    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);
    pstr = strstr (StExtnFileReg.src_file_name, "XX");
    if (pstr)
        memcpy (pstr, "99", 2);

    getExtnFileFullName(&StExtnFileReg, onlinepay_file_name);
    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "ext_inter_brh_code[%s]stoday[%s]file[%s].", ext_inter_brh_code, dbtbl_date_inf.stoday, onlinepay_file_name);

    /* 取文件行数 */
    fp = fopen (onlinepay_file_name, "r");
    if (!fp)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen %s error.", onlinepay_file_name);
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
*函数名称∶ Task_0401
*函数功能∶
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_0401 ( int nBeginOffset, int nEndOffset )
{
    int		iRet = 0;
    int	        i  = 1, nTotCount=0;
    char    *pstr;
    char	onlinepay_file_name[200] = {0};
    char	sFileRecord[RECORD_LEN_MAX];
    char    sDelim[] = "|";
    char    sTmp[512] = {0};
    FILE   *fp;
    stExtnFileRegDef StExtnFileReg;
    ht_tbl_gateway_txn_def ht_tbl_gateway_txn;
    tbl_brh_map_def tbl_brh_map_inf;
	ht_tbl_ext_mcht_inf_def  tbl_ext_mcht_inf;    
    
     /* 初始文件信息 */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    memcpy(StExtnFileReg.host_name, "ONLINEPAY", 9);
    memcpy(StExtnFileReg.src_file_name, "ONLINEPAY_YYYYMMDD_XX.txt", 25);

    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);
    pstr = strstr (StExtnFileReg.src_file_name, "XX");
    if (pstr)
        memcpy (pstr, "99", 2);

    getExtnFileFullName(&StExtnFileReg, onlinepay_file_name);

    fp = fopen (onlinepay_file_name, "r");
    if (!fp)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen %s error", onlinepay_file_name);
        return -1;
    }

    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "file[%s]nBeginOffset=[%d] nEndOffset=[%d].", onlinepay_file_name, nBeginOffset, nEndOffset);

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
        memset(&ht_tbl_gateway_txn, 0x00, sizeof(ht_tbl_gateway_txn));
        if (fgets(sFileRecord, RECORD_LEN_MAX, fp) == NULL)
        {
            break;
        }
        if(strlen(sFileRecord) < BT_MINLINE_LEN)
        {
            HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sFileRecord [%s].", sFileRecord);
            continue;    
        }
        
        i++;
        nTotCount++;

	    memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, ht_tbl_gateway_txn.trans_date, sDelim);
        getValueByDelim(sFileRecord, ht_tbl_gateway_txn.trans_time, sDelim);
        getValueByDelim(sFileRecord, sTmp, sDelim);
        getValueByDelim(sFileRecord, ht_tbl_gateway_txn.plat_key, sDelim);
        CommonRTrim(ht_tbl_gateway_txn.plat_key);
        getValueByDelim(sFileRecord, ht_tbl_gateway_txn.host_key, sDelim);
        CommonRTrim(ht_tbl_gateway_txn.host_key);
        getValueByDelim(sFileRecord, ht_tbl_gateway_txn.batch_no, sDelim);
        getValueByDelim(sFileRecord, ht_tbl_gateway_txn.order_no, sDelim);
        
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);
        CommonRTrim(sTmp);        
        
       /*外部支付通道转换为内部定义的支付通道*/
        ChnOut2In(sTmp, ht_tbl_gateway_txn.bus_tp);
                  
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);
        CommonRTrim(sTmp);
     
        strcat(ht_tbl_gateway_txn.bus_tp, sTmp);   
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "ht_tbl_gateway_txn.bus_tp [%s].", ht_tbl_gateway_txn.bus_tp);     
        
        getValueByDelim(sFileRecord, ht_tbl_gateway_txn.trans_tp, sDelim);
        getValueByDelim(sFileRecord, ht_tbl_gateway_txn.acct_type, sDelim);
        getValueByDelim(sFileRecord, ht_tbl_gateway_txn.trans_state, sDelim);
           
        getValueByDelim(sFileRecord, ht_tbl_gateway_txn.revsal_flag, sDelim);
        getValueByDelim(sFileRecord, ht_tbl_gateway_txn.cancel_flag, sDelim);  
        CommonRTrim(ht_tbl_gateway_txn.revsal_flag);
        CommonRTrim(ht_tbl_gateway_txn.cancel_flag);   
        if(strlen(ht_tbl_gateway_txn.revsal_flag) <= 0)
        {
            strcpy(ht_tbl_gateway_txn.revsal_flag, "0");    
        }
        if(strlen(ht_tbl_gateway_txn.cancel_flag) <= 0)
        {
            strcpy(ht_tbl_gateway_txn.cancel_flag, "0");    
        }
        getValueByDelim(sFileRecord, ht_tbl_gateway_txn.key_revsal, sDelim);      
        getValueByDelim(sFileRecord, ht_tbl_gateway_txn.key_cancel, sDelim);        
        getValueByDelim(sFileRecord, ht_tbl_gateway_txn.pan, sDelim);
        CommonRTrim(ht_tbl_gateway_txn.pan);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "ht_tbl_gateway_txn.pan [%s].", ht_tbl_gateway_txn.pan);
        
        getValueByDelim(sFileRecord, ht_tbl_gateway_txn.acct_name, sDelim);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "ht_tbl_gateway_txn.acct_name [%s].", ht_tbl_gateway_txn.acct_name); 

        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);
          
        getValueByDelim(sFileRecord, ht_tbl_gateway_txn.ccy, sDelim);
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);
        CommonRTrim(sTmp);
        sprintf(ht_tbl_gateway_txn.trans_at, "%012.0lf", floor(atof(sTmp)*100+0.50001)); 
          
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);
        CommonRTrim(sTmp);
        sprintf(ht_tbl_gateway_txn.trans_at2, "%012.0lf", floor(atof(sTmp)*100+0.50001)); 
        
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);
        CommonRTrim(sTmp);
        sprintf(ht_tbl_gateway_txn.trans_at3, "%012.0lf", floor(atof(sTmp)*100+0.50001));
        
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);
        CommonRTrim(sTmp);
        sprintf(ht_tbl_gateway_txn.trans_at4, "%012.0lf", floor(atof(sTmp)*100+0.50001));
        
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);
        CommonRTrim(sTmp);
        sprintf(ht_tbl_gateway_txn.trans_at5, "%012.0lf", floor(atof(sTmp)*100+0.50001));

        getValueByDelim(sFileRecord, ht_tbl_gateway_txn.sett_mcht_no, sDelim);
        CommonRTrim(ht_tbl_gateway_txn.sett_mcht_no);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "test [%s].", ht_tbl_gateway_txn.sett_mcht_no);
        getValueByDelim(sFileRecord, ht_tbl_gateway_txn.ext_mcht_no, sDelim);        
        CommonRTrim(ht_tbl_gateway_txn.ext_mcht_no);        
        RightTrim(ht_tbl_gateway_txn.ext_mcht_no);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "test [%s].", ht_tbl_gateway_txn.ext_mcht_no);
        
        memcpy(ht_tbl_gateway_txn.sett_tp1, BT_SETT_TP_SETT, BT_SETT_TP_LEN);
        strcpy(ht_tbl_gateway_txn.mcht_no1, ht_tbl_gateway_txn.sett_mcht_no);

        /*memcpy(ht_tbl_gateway_txn.inter_brh_code, ext_inter_brh_code, sizeof(ht_tbl_gateway_txn.inter_brh_code)-1);*/
        
        memset(&tbl_brh_map_inf, 0, sizeof(tbl_brh_map_inf));
        strcpy(tbl_brh_map_inf.bus_tp, ht_tbl_gateway_txn.bus_tp);
        
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "test [%s].", tbl_brh_map_inf.bus_tp);

		// 获取外部商户号
		if(!memcmp("1301", ht_tbl_gateway_txn.trans_tp,4)) {
			
             memset(&tbl_ext_mcht_inf, 0, sizeof(tbl_ext_mcht_inf));
             iRet = DbsTblExtMchtInfTxn(DBS_SELECT1, &tbl_ext_mcht_inf, 0, 0);
             if (iRet != 0)
             {
                 HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblExtMchtInfTxn DBS_SELECT error, %d,mcht_no[%s].", iRet, tbl_ext_mcht_inf.ext_mch_no);
                 fclose (fp);
				 return -1;
             }
			 CommonRTrim(tbl_ext_mcht_inf.ext_mch_no); 
			 strcpy(ht_tbl_gateway_txn.ext_mcht_no, tbl_ext_mcht_inf.ext_mch_no);
			 HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "ext_mcht_no [%s].", ht_tbl_gateway_txn.ext_mcht_no);
		}
        
        iRet = DbsTblbrhMap(DBS_SELECT, &tbl_brh_map_inf,0,0);
        if (iRet != 0)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblbrhMap DBS_SELECT error, %d.", iRet);
            fclose (fp);
            return -1;
        }
        
        CommonRTrim(tbl_brh_map_inf.brh_id);
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "test [%s][%s].", tbl_brh_map_inf.brh_id, ht_tbl_gateway_txn.plat_key);
        strcpy(ht_tbl_gateway_txn.inter_brh_code, tbl_brh_map_inf.brh_id);
        
        memcpy(ht_tbl_gateway_txn.sett_date, dbtbl_date_inf.stoday, sizeof(ht_tbl_gateway_txn.sett_date)-1);
        memcpy(ht_tbl_gateway_txn.flag_result, BT_FLAG_RESULT_NULL, BT_FLAG_RESULT_LEN);
        
        iRet = DbsTblGatewayTxn(DBS_INSERT, &ht_tbl_gateway_txn,0,0);
        if (iRet != 0 && iRet != DBS_KEYDUPLICATE)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblGatewayTxn DBS_INSERT error, %d.plat_key[%s]", iRet, ht_tbl_gateway_txn.plat_key);
            fclose (fp);
            return -1;
        }
        else if(iRet == DBS_KEYDUPLICATE)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblGatewayTxn DBS_KEYDUPLICATE");
        }
    }
    fclose (fp);
    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "total num [%d].", nTotCount);
    HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_0401 Succ .");
    return 0;
}

/****************************** end of Task_0401.c ******************************/
