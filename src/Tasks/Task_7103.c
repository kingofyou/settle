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
 *  文 件 名: Task_7103.c
 *  功    能: 富友代付
 *  编程人员: 
 *  开发时间: 2017-03-09
 *  备    注:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include <sys/socket.h>
#include <iconv.h>
#include "batch.h"
#include "Common.h"
#include "EPay.h"

extern  char    gLogFile[LOG_NAME_LEN_MAX];
extern  char    ext_child_date[8 + 1];
extern  char    ext_inter_brh_code[10 + 1];
extern  int     ext_inter_brh_sta;
extern          tbl_date_inf_def dbtbl_date_inf;

/*************************************************
*函数名称∶ Total_7103
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_7103()
{
    return 1;
}

char* rtrim(char* rptr)
{
	if(!rptr) return rptr;
    char* ptrTmp = rptr + strlen(rptr) - 1;
    while(*ptrTmp == ' ') ptrTmp--;
    *++ptrTmp = '\0';
    return rptr;
}

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

static int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
	return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,&outlen);
}

int iSinglePayApi(tbl_sett_list_def* dbtbl_sett_list, char* mcht_no)
{
    int nReturnCode;
    char    sysDate[9];
    char    sysTime[14+1];    
    char    Filename[100];
    char    stlm_dt[8+1];

	char merdt[27];
	char orderno[32+12];
	char bankno[24];
	char cityno[24];
	char branchnm[256];
	char accntno[128];
	char accntnm[128];
	char amt[24];

    char accntnmTmp[128];

	char ret[7] = {};
    char start[128] = {};
	char *sEnd;

	memset(merdt, 0, sizeof(merdt));
	memset(orderno, 0, sizeof(orderno));
	memset(bankno, 0, sizeof(bankno));
	memset(cityno, 0, sizeof(cityno));
	memset(branchnm, 0, sizeof(branchnm));
	memset(accntno, 0, sizeof(accntno));
	memset(accntnm, 0, sizeof(accntnm));
	memset(amt, 0, sizeof(amt));
	memset(accntnmTmp, 0, sizeof(accntnmTmp));

    Fy_Single_Request FyRequest;
    Fy_Single_Response FyResponse; 
    
   //批量资金付款
    memset(&FyRequest,0x0, sizeof(FyRequest));
    memset(&FyResponse,0x0, sizeof(FyResponse));
    memset(sysDate,0x00,sizeof(sysDate));
    memset(sysTime,0x00,sizeof(sysTime));

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
        
    CommonGetCurrentDate(sysDate);
    CommonGetCurrentTime(sysTime);

    memcpy(FyRequest.reqtype, "payforreq", sizeof(FyRequest.reqtype)-1);
	memcpy(FyRequest.merid, "0002900F0345178", sizeof(FyRequest.merid)-1);

	sprintf(merdt, "<merdt>%s</merdt>", rtrim(dbtbl_date_inf.snextdate));
	sprintf(orderno, "<orderno>%s</orderno>", rtrim(dbtbl_sett_list->seq_no));
	sprintf(bankno, "<bankno>%s</bankno>", rtrim(dbtbl_sett_list->mcht_sett_bank));
	sprintf(cityno, "<cityno>%s</cityno>", "5840");
	sprintf(branchnm, "<branchnm>%s</branchnm>", rtrim(dbtbl_sett_list->mcht_sett_bank_nm));
	sprintf(accntno, "<accntno>%s</accntno>", rtrim(dbtbl_sett_list->mcht_sett_acct));

	g2u(rtrim(dbtbl_sett_list->mcht_sett_acct_nm), strlen(rtrim(dbtbl_sett_list->mcht_sett_acct_nm)), accntnmTmp, sizeof(accntnmTmp));
	sprintf(accntnm, "<accntnm>%s</accntnm>", accntnmTmp);
	sprintf(amt, "<amt>%.0f</amt>", (dbtbl_sett_list->amt)*100);

	strcat(FyRequest.xmlMsg, "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>");
    strcat(FyRequest.xmlMsg, "<payforreq>");
	strcat(FyRequest.xmlMsg, "<ver>1.00</ver>");
    strcat(FyRequest.xmlMsg, merdt);
	strcat(FyRequest.xmlMsg, orderno);
	strcat(FyRequest.xmlMsg, bankno);
	strcat(FyRequest.xmlMsg, cityno);
	strcat(FyRequest.xmlMsg, branchnm);
	strcat(FyRequest.xmlMsg, accntno);
	strcat(FyRequest.xmlMsg, accntnm);
	strcat(FyRequest.xmlMsg, amt);
	strcat(FyRequest.xmlMsg, "</payforreq>");
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "FyRequest.xmlMsg:[%s]", FyRequest.xmlMsg);

//    memcpy(FyRequest.merdt, dbtbl_sett_list->sett_date, sizeof(FyRequest.merdt)-1);
//	memcpy(FyRequest.orderno, dbtbl_sett_list->seq_no, sizeof(FyRequest.orderno)-1);
//	memcpy(FyRequest.bankno, dbtbl_sett_list->mcht_sett_bank, sizeof(FyRequest.bankno)-1);
//	memcpy(FyRequest.cityno, "", sizeof(FyRequest.cityno)-1);
//	memcpy(FyRequest.branchnm, dbtbl_sett_list->mcht_sett_bank_nm, sizeof(FyRequest.branchnm)-1);
//	memcpy(FyRequest.accntno, dbtbl_sett_list->mcht_sett_acct, sizeof(FyRequest.accntno)-1);
//	memcpy(FyRequest.accntnm, dbtbl_sett_list->mcht_sett_acct_nm, sizeof(FyRequest.accntnm)-1);
//	FyRequest.amt = dbtbl_sett_list->amt*100;

    nReturnCode = FySinglePayApi(&FyRequest,&FyResponse);
    if(nReturnCode)
    {
        // 代付失败
		dbtbl_sett_list->amt_fee=0;
		memcpy(dbtbl_sett_list->status, "5", 1);
		nReturnCode = DbsTblSettList(DBS_UPDATE2, dbtbl_sett_list);
		if (nReturnCode)
		{
		    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPD_SEQ ht_tbl_sett_list error [%d].", nReturnCode);
		    return -1;
		}
        return nReturnCode;
    }
    else
    {
		HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "FyResponse.xmlMsg:[%s]", FyResponse.xmlMsg);
		sEnd = strstr(FyResponse.xmlMsg, "<ret>");
	    memcpy(ret, sEnd+5, 6);
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "ret:[%s]", ret);

        if(!memcmp("000000", ret, 6)) {
		    dbtbl_sett_list->amt_fee=0;
		    memcpy(dbtbl_sett_list->status, "7", 1);
		    nReturnCode = DbsTblSettList(DBS_UPDATE2, dbtbl_sett_list);
		    if (nReturnCode)
		    {
		        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPD_SEQ ht_tbl_sett_list error [%d].", nReturnCode);
		        return -1;
		    }
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " FySinglePayApi succ [%d]",nReturnCode);   
		}
		else {
            dbtbl_sett_list->amt_fee=0;
		    memcpy(dbtbl_sett_list->status, "5", 1);
		    nReturnCode = DbsTblSettList(DBS_UPDATE2, dbtbl_sett_list);
		    if (nReturnCode)
		    {
		        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPD_SEQ ht_tbl_sett_list error [%d].", nReturnCode);
		        return -1;
		    }
		}
    }
    return 0;    
}

/*************************************************
*函数名称∶ Task_7103
*函数功能∶ 循环单笔代付
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_7103 ( int nBeginOffset, int nEndOffset )
{
    int     nReturnCode;
    char    sDateTime[14+1];
    char    stlm_dt[8+1];
    char    sBbankCode[30+1];
    int		nBankCount = 0; 
    int		nTotalCount = 0;

    char merdt[27] = {};
	char orderno[32+12] = {};
	char bankno[24] = {};
	char cityno[24] = {};
	char branchnm[256] = {};
	char accntno[128] = {};
	char accntnm[128] = {};
	char amt[64] = {};
	char xml[1024] = {};

	char ret[7] = {};
    char* start=NULL;
	char* end = NULL;

	char    sSoapCfgName[32+1] = {0};
	HttpRequest pstHttpRequest;
    HttpResponse pstHttpResponse;
	stKeyInf KeyInf;
    
    memset(&pstHttpRequest, 0x0, sizeof(pstHttpRequest));
    memset(&pstHttpResponse, 0x0, sizeof(pstHttpResponse));
    
    tbl_sett_list_def   dbtbl_sett_list, dbtbl_sett_list_dc;
    ht_tbl_com_config_def comconfig;
    
     
    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
     
	
	memset(&KeyInf, 0x00, sizeof(KeyInf));
//    nReturnCode = GetKeyInf(&KeyInf, "fuiou");
//    if (nReturnCode)
//    {
//        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "GetKeyInf err.");
//        return -1;
//    } 
       
    
//    memset(sBbankCode, 0, sizeof(sBbankCode));
//    memset(&comconfig, 0x00, sizeof(comconfig));
//    strcpy(comconfig.key1,"200004");
//    strcpy(comconfig.key2,"7103");
//    strcpy(comconfig.key3,"BANKCODE");
//    nReturnCode = DbsbthComConfig (DBS_SELECT, &comconfig);
//    if( nReturnCode )
//    {
//        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsbthComConfig DBS_SELECT[%d]", nReturnCode);
//        return nReturnCode;
//    }
//    CommonRTrim(comconfig.value1); 
//    memcpy(sBbankCode, comconfig.value1,sizeof(sBbankCode)-1);
                
        
    /* 游标，根据待划账的行号查询结算明细表 */
    memset(&dbtbl_sett_list_dc, 0x00, sizeof(dbtbl_sett_list_dc));
    memcpy(dbtbl_sett_list_dc.sett_date, stlm_dt, 8);
    //memcpy(dbtbl_sett_list_dc.plat_sett_bank, sBbankCode, sizeof(dbtbl_sett_list_dc.plat_sett_bank)-1);
    memcpy(dbtbl_sett_list_dc.status, "2", 1);
    memcpy(dbtbl_sett_list_dc.dc_flag, "C", 1);
	memcpy(dbtbl_sett_list_dc.chl_id, "FY", 2);
        
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
            return -1;            
        }

		// 富友单笔代付请求
		iSinglePayApi(&dbtbl_sett_list_dc, KeyInf.sMrchNo);
		           
    }
    DbsTblSettList(DBS_CLOSE1, &dbtbl_sett_list_dc);
   
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nTotalCount:[%d]", nTotalCount);
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_7103 Succ .");
                    
    return 0;
}



/****************************** end of Task_7103.c ******************************/


