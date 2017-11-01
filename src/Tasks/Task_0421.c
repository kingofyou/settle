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
 *  �� �� ��: Task_0421.c
 *  ��    ��: ��������ˮ�ļ�
 *  �����Ա: HELU
 *  ����ʱ��: 2016-12-14
 *  ��    ע:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include "batch.h"
#include "Common.h"
#include <iconv.h>

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
//UNICODE��תΪGB2312��
static int u2g(char *inbuf,int inlen,char *outbuf,size_t outlen)
{
	return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,&outlen);
}
//GB2312��תΪUNICODE��
static int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
	return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,&outlen);
}

/*************************************************
*�������ơ� Total_0421
*�������ܡ�
*���������
*���������
*�������ء� 1
***************************************************/
int Total_0421()
{
    int		nTotalNum = 0;
    char      sHostTxnNum[2 + 1];
    char	sFileRecord[RECORD_LEN_MAX];
    char    *pstr;
    char	risk_file_name[200] = {0};
    FILE *fp;
    stExtnFileRegDef StExtnFileReg;

     /* ��ʼ�ļ���Ϣ */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    strcpy(StExtnFileReg.host_name, "RISK");
    strcpy(StExtnFileReg.src_file_name, "RISK_YYYYMMDD_XX.txt");

    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);
    pstr = strstr (StExtnFileReg.src_file_name, "XX");
    if (pstr)
        memcpy (pstr, "01", 2);

    getExtnFileFullName(&StExtnFileReg, risk_file_name);
    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "ext_inter_brh_code[%s]stoday[%s]file[%s].", ext_inter_brh_code, dbtbl_date_inf.stoday, risk_file_name);

    /* ȡ�ļ����� */
    fp = fopen (risk_file_name, "r");
    if (!fp)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen %s error.", risk_file_name);
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
*�������ơ� Task_0421
*�������ܡ�
*��������� nBeginOffset��nEndOffset
*���������
*�������ء� 0  -- �ɹ�
            -1 -- ʧ��
***************************************************/
int Task_0421 ( int nBeginOffset, int nEndOffset )
{
    int		iRet = 0;
    int	        i  = 1, nTotCount=0;
    char    *pstr;
    char	risk_file_name[200] = {0};
    char	sFileRecord[RECORD_LEN_MAX];
    char    sDelim[] = "|";
    char    sTmp[512] = {0};
    FILE   *fp;
    stExtnFileRegDef StExtnFileReg;
    ht_tbl_risk_txn_def ht_tbl_risk_txn;
    tbl_brh_map_def tbl_brh_map_inf;
    
     /* ��ʼ�ļ���Ϣ */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    strcpy(StExtnFileReg.host_name, "RISK");
    strcpy(StExtnFileReg.src_file_name, "RISK_YYYYMMDD_XX.txt");

    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);
    pstr = strstr (StExtnFileReg.src_file_name, "XX");
    if (pstr)
        memcpy (pstr, "01", 2);

    getExtnFileFullName(&StExtnFileReg, risk_file_name);

    fp = fopen (risk_file_name, "r");
    if (!fp)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen %s error", risk_file_name);
        return -1;
    }

    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "file[%s]nBeginOffset=[%d] nEndOffset=[%d].", risk_file_name, nBeginOffset, nEndOffset);

    /* ���ļ���ʼ�� */
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
        memset(&ht_tbl_risk_txn, 0x00, sizeof(ht_tbl_risk_txn));
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
        
        memcpy(ht_tbl_risk_txn.inter_brh_code,ext_inter_brh_code,6);
        memcpy(ht_tbl_risk_txn.sett_date,dbtbl_date_inf.stoday,8);

	    memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, ht_tbl_risk_txn.trans_date, sDelim);//��������
        getValueByDelim(sFileRecord, ht_tbl_risk_txn.trans_time, sDelim);//����ʱ��
        getValueByDelim(sFileRecord, ht_tbl_risk_txn.gateway_key, sDelim);//��ˮ��
        CommonRTrim(ht_tbl_risk_txn.gateway_key);
        getValueByDelim(sFileRecord, ht_tbl_risk_txn.order_no, sDelim);//������  
        getValueByDelim(sFileRecord, ht_tbl_risk_txn.payway_type, sDelim);                    //֧��ͨ��
        
       /*�ⲿ֧��ͨ��ת��Ϊ�ڲ������֧��ͨ��*/
        ChnOut2In(ht_tbl_risk_txn.payway_type, ht_tbl_risk_txn.bus_tp);

        
        getValueByDelim(sFileRecord, ht_tbl_risk_txn.trans_tp, sDelim);//��������
    
        
        getValueByDelim(sFileRecord, ht_tbl_risk_txn.user_id, sDelim);//�ͻ�ID
        getValueByDelim(sFileRecord, ht_tbl_risk_txn.pan, sDelim);//����    
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);//���׽��
 
        CommonRTrim(sTmp);
        ht_tbl_risk_txn.trans_at = floor(atof(sTmp)*100+0.50001);
     
        getValueByDelim(sFileRecord, ht_tbl_risk_txn.sett_mcht_no, sDelim);//�����̻�����
        getValueByDelim(sFileRecord, ht_tbl_risk_txn.ext_mcht_no, sDelim);  //�����̻�����
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);  //��ش���ԭ��    
        u2g(sTmp, strlen(sTmp),ht_tbl_risk_txn.risk_reason, sizeof(ht_tbl_risk_txn.risk_reason));  //��ش���ԭ��    
        getValueByDelim(sFileRecord, ht_tbl_risk_txn.remark, sDelim);       //����1 
         
        iRet = DbsTblRiskTxn(DBS_INSERT, &ht_tbl_risk_txn,0,0);
        if (iRet != 0 && iRet != DBS_KEYDUPLICATE)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblRiskTxn DBS_INSERT error, %d.gateway_key[%s]", iRet, ht_tbl_risk_txn.gateway_key);
            fclose (fp);
            return -1;
        }
        else if(iRet == DBS_KEYDUPLICATE)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblRiskTxn DBS_KEYDUPLICATE");
        }
    }
    fclose (fp);
    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "total num [%d].", nTotCount);
    HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_0421 Succ .");
    return 0;
}

/****************************** end of Task_0421.c ******************************/
