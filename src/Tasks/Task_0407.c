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
 *  文 件 名: Task_0407.c
 *  功    能: 导入兴业银行流水文件（快捷支付出账文件）
 *  编程人员: wyl
 *  开发时间: 2017-05-04
 *  备    注:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include "batch.h"
#include "Common.h"

extern char	gLogFile[LOG_NAME_LEN_MAX];
extern  tbl_date_inf_def dbtbl_date_inf;
extern	char ext_inter_brh_code[10 + 1];

//int GetKeyInf(stKeyInf *pKeyInf)
//{
//    int nReturnCode;
//    
//    ht_tbl_com_config_def comconfig;
//    tp_payway_ext_def tp_payway_ext;
//    tp_merc_paywayconf_def tp_merc_paywayconf;
//        
//    memset(&comconfig, 0x00, sizeof(comconfig));
//    strcpy(comconfig.key1,"200001");
//    strcpy(comconfig.key2,"CIBGATEWAY");
//    strcpy(comconfig.key3,"PAYWAYTYPE");
//    nReturnCode = DbsbthComConfig (DBS_SELECT, &comconfig);
//     if( nReturnCode )
//    {
//        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsbthComConfig DBS_SELECT[%d]", nReturnCode);
//        return nReturnCode;
//    }
//
//    CommonRTrim(comconfig.value1);  
//    CommonRTrim(comconfig.value2);  
//    CommonRTrim(comconfig.value3);
//    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "[%s][%s][%s]", comconfig.value1,comconfig.value2,comconfig.value3); 
//    
//    memset(&tp_payway_ext, 0x00, sizeof(tp_payway_ext));
//    memcpy(tp_payway_ext.payway_type,comconfig.value1,sizeof(tp_payway_ext.payway_type)-1);    
//    nReturnCode = DbsTpPaywayExt (DBS_SELECT, &tp_payway_ext);
//	if (nReturnCode)
//	{
//	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTpPaywayExt error [%d]", nReturnCode);
//	    return -1;
//	}	
//	CommonRTrim(tp_payway_ext.payway_code);
//	memset(&tp_merc_paywayconf,0x0,sizeof(tp_merc_paywayconf));
//	memcpy(tp_merc_paywayconf.payway_code,tp_payway_ext.payway_code,sizeof(tp_merc_paywayconf.payway_code)-1);
//	HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "payway_code[%s]", tp_merc_paywayconf.payway_code);
//	nReturnCode = DbsTpMercPaywayConf (DBS_SELECT, &tp_merc_paywayconf);    
//	if (nReturnCode)
//	{
//	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTpMercPaywayConf error [%d]", nReturnCode);
//	    return -1;
//	}
//	CommonRTrim(tp_merc_paywayconf.paying_merc_code);
//	HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "paying_merc_code[%s]", tp_merc_paywayconf.paying_merc_code);
//	
//    CommonRTrim(tp_merc_paywayconf.paying_merc_code);  
//    memcpy(pKeyInf->sMrchNo,tp_merc_paywayconf.paying_merc_code,sizeof(pKeyInf->sMrchNo)-1);
//    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "sMrchNo[%s]", pKeyInf->sMrchNo); 
//    return 0;
//}
/*************************************************
*函数名称∶ Total_0407
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_0407()
{
    int		nTotalNum = 0;
    int iRet = 0;
    char      sHostTxnNum[2 + 1];
    char	sFileRecord[RECORD_LEN_MAX] = {0};
    char    *pstr;
    char	cib_file_name[200] = {0};
    FILE *fp;
    glob_t globbuf;
    stExtnFileRegDef StExtnFileReg;
    stKeyInf KeyInf;

    memset(&KeyInf, 0x00, sizeof(KeyInf));
    iRet = GetKeyInf(&KeyInf, "cib_xydf");
    if (iRet)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "GetKeyInf err.");
        return -1;
    } 
   /* 初始文件信息 */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    memcpy(StExtnFileReg.host_name, "CIB", 3);
    /*文件名：商户号_交易日期(yyyymmdd)_回单类型(0/1)_文件生成时间(yyyymmddhhmmss).txt*/
    sprintf(StExtnFileReg.src_file_name, "%s_YYYYMMDD_1.txt",KeyInf.sMrchNo);
    
    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);

    getExtnFileFullName(&StExtnFileReg, cib_file_name);

    iRet = glob (cib_file_name, GLOB_NOSORT, NULL, &globbuf);
    if(iRet)
    {
       HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "glob error [%d %s]", iRet, cib_file_name);
       globfree (&globbuf);
       return -1;
    }
    else if(globbuf.gl_pathc != 1)
    {
       HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "file num err [%d] [%d]", iRet, globbuf.gl_pathc);
       globfree (&globbuf);
       return -1;
    }
    
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,"gl_pathc[%d] [%s]", globbuf.gl_pathc, globbuf.gl_pathv[0]);
        
    /* 取文件行数 */
    fp = fopen (globbuf.gl_pathv[0], "r");
    if (!fp)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen [%s] error.", globbuf.gl_pathv[0]);
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
*函数名称∶ Task_0407
*函数功能∶
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_0407 ( int nBeginOffset, int nEndOffset )
{
    int		iRet = 0;
    int	    i  = 1, nTotCount=0, nFileCount = 0;
    char    *pstr;
    char    sCupBrhId[8 + 1] = {0};
    char	cib_file_name[200] = {0};
    char	sFileRecord[RECORD_LEN_MAX] = {0};
    char    sTmp[512] = {0};
    char    sDelim[] = "|";
    FILE   *fp;
    glob_t globbuf;

    stExtnFileRegDef StExtnFileReg;
    ht_tbl_cib_txn_def ht_tbl_cib_txn;
    tbl_brh_map_def tbl_brh_map_inf;
    stKeyInf KeyInf;

    memset(&KeyInf, 0x00, sizeof(KeyInf));
    iRet = GetKeyInf(&KeyInf, "cib_xydf");
    if (iRet)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "GetKeyInf err.");
        return -1;
    }
   /* 初始文件信息 */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    memcpy(StExtnFileReg.host_name, "CIB", 3);
    /*文件名：商户号_交易日期(yyyymmdd)_回单类型(0/1)_文件生成时间(yyyymmddhhmmss).txt*/
    sprintf(StExtnFileReg.src_file_name, "%s_YYYYMMDD_1.txt",KeyInf.sMrchNo);
    
    pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);

    getExtnFileFullName(&StExtnFileReg, cib_file_name);

    iRet = glob (cib_file_name, GLOB_NOSORT, NULL, &globbuf);
    if(iRet)
    {
       HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "glob error [%d %s]", iRet, cib_file_name);
       globfree (&globbuf);
       return -1;
    }
    else if(globbuf.gl_pathc != 1)
    {
       HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "file num err [%d] [%d]", iRet, globbuf.gl_pathc);
       globfree (&globbuf);
       return -1;
    }
    
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,"gl_pathc[%d] [%s]", globbuf.gl_pathc, globbuf.gl_pathv[0]);
    
    fp = fopen (globbuf.gl_pathv[0], "r");
    if (!fp)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen [%s] error,", globbuf.gl_pathv[0]);
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
        memset(&ht_tbl_cib_txn, 0x00, sizeof(ht_tbl_cib_txn));
        if (fgets(sFileRecord, RECORD_LEN_MAX, fp) == NULL)
        {
            break;
        }
        if(strlen(sFileRecord) < BT_MINLINE_LEN)
        {
            HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sFileRecord [%s].", sFileRecord);
            continue;    
        }
        else if(strlen(sFileRecord) <= BT_CIB_SUM_LEN)
        {
            memset(sTmp, 0, sizeof(sTmp)); 
            getValueByDelim(sFileRecord, sTmp, sDelim);
            nFileCount = atoi(sTmp);
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "nFileCount[%d].", nFileCount);
            continue;    
        }
        i++;
        nTotCount++;
	    memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, ht_tbl_cib_txn.epaytime, sDelim);
        getValueByDelim(sFileRecord, ht_tbl_cib_txn.orderdate, sDelim);
        getValueByDelim(sFileRecord, ht_tbl_cib_txn.sno, sDelim);
        
        getValueByDelim(sFileRecord, ht_tbl_cib_txn.mrchno, sDelim);
        getValueByDelim(sFileRecord, ht_tbl_cib_txn.order_no, sDelim);
        HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "test num [%s][%s][%s][%s][%s].", ht_tbl_cib_txn.epaytime, ht_tbl_cib_txn.orderdate, ht_tbl_cib_txn.sno, ht_tbl_cib_txn.mrchno, ht_tbl_cib_txn.order_no);
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);
        CommonRTrim(sTmp);
        sprintf(ht_tbl_cib_txn.orderamt, "%012.0lf", floor(atof(sTmp)*100+0.50001));
        CommonRTrim(ht_tbl_cib_txn.orderamt);
        CommonLTrim(ht_tbl_cib_txn.orderamt);
        
        memset(sTmp, 0, sizeof(sTmp));
        getValueByDelim(sFileRecord, sTmp, sDelim);
        CommonRTrim(sTmp);
        sprintf(ht_tbl_cib_txn.feeamt, "%012.0lf", floor(atof(sTmp)*100+0.50001));        
        CommonRTrim(ht_tbl_cib_txn.feeamt);
        CommonLTrim(ht_tbl_cib_txn.feeamt);
        
        getValueByDelim(sFileRecord, ht_tbl_cib_txn.transdate, sDelim);
        
        memcpy(ht_tbl_cib_txn.inter_brh_code, ext_inter_brh_code, sizeof(ht_tbl_cib_txn.inter_brh_code)-1);
        
        memcpy(ht_tbl_cib_txn.sett_date, dbtbl_date_inf.stoday, sizeof(ht_tbl_cib_txn.sett_date)-1);
        
        
        RightTrim(ht_tbl_cib_txn.transdate);        
        CommonRTrim(ht_tbl_cib_txn.orderdate);
        CommonRTrim(ht_tbl_cib_txn.sno);
        CommonRTrim(ht_tbl_cib_txn.order_no);
        strcpy(ht_tbl_cib_txn.plat_key, ht_tbl_cib_txn.orderdate);
        strcat(ht_tbl_cib_txn.plat_key, ht_tbl_cib_txn.order_no);
        CommonRTrim(ht_tbl_cib_txn.plat_key);
        
        strcpy(ht_tbl_cib_txn.batch_no, "01");
        memcpy(ht_tbl_cib_txn.flag_result, BT_FLAG_RESULT_NULL, BT_FLAG_RESULT_LEN);
        sprintf(ht_tbl_cib_txn.bus_tp,"%s%s", BT_BUS_TP_CIB5, BT_BUS_TP_CIB5);
        memcpy(ht_tbl_cib_txn.trans_tp, BT_TRANS_TP_QUICK_OUT, BT_TRANS_TP_LEN);      
        strcpy(ht_tbl_cib_txn.trans_state, "1");   
        strcpy(ht_tbl_cib_txn.acct_type, "00");  
        strcpy(ht_tbl_cib_txn.pan, "0000000000000000000"); 
        strcpy(ht_tbl_cib_txn.ccy, "156"); 
                                
        iRet = DbsTblCIBTxn(DBS_INSERT, &ht_tbl_cib_txn, 0, 0);
        if (iRet != 0 && iRet != DBS_KEYDUPLICATE)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblCIBTxn DBS_INSERT error, %d.", iRet);
            fclose (fp);
            return -1;
        }
        else if(iRet == DBS_KEYDUPLICATE)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblCIBTxn DBS_KEYDUPLICATE");
        }
    }    

    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "total num [%d].", nTotCount);
    if (nFileCount != nTotCount)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "file num error, nFileCount[%d].nFileCount[%d]", nFileCount, nTotCount);
        fclose (fp);
        return -1;
    }
    
    fclose (fp);
        
    HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_0407 Succ .");
    return 0;
}
/****************************** end of Task_0407.c ******************************/
