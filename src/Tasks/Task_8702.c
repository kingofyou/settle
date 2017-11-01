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
 *  �� �� ��: Task_8702.c
 *  ��    ��: �������ˡ�����-�������Ϊ������Ϊ�㴦��
 *  �����Ա: Jason Yu
 *  ����ʱ��: 20170707
 *  ��    ע:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include <sys/socket.h>
#include "batch.h"
#include "Common.h"


extern char	gLogFile[LOG_NAME_LEN_MAX];
extern  tbl_date_inf_def dbtbl_date_inf;
extern	char ext_inter_brh_code[10 + 1];


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
*�������ơ� WriteFile
*�������ܡ�
*��������� ���ɼ�����ˮ�ļ�
*���������
*�������ء� 0  -- �ɹ�
            -1 -- ʧ��
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

    return  ;
}



/*���������Ҫ�����Ľ��й���*/
int NegaHand(FILE* fp, int nBeginOffset, int nEndOffset)
{
    int     nReturnCode;
    char    stlm_dt[8+1]; 
    int     iTotolCount = 0;   
    tbl_acct_rslt_def   dbtbl_acct_rslt;      
    ht_tbl_acct_inf_def  tbl_acct_inf0041;
    ht_tbl_acct_inf_def  tbl_acct_infG026;
	char* sDateTime;
	
    stHost_Acct_def  sthost_acct;
    memset(&sthost_acct,0x00,sizeof(sthost_acct));

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
	
    /*0041-��ȡ���������˻���Ϣ*/
    memset(&tbl_acct_inf0041,0x0,sizeof(tbl_acct_inf0041));
    memcpy(tbl_acct_inf0041.type,"10",2);
    nReturnCode = DbsTblAcctInf(DBS_SELECT, &tbl_acct_inf0041);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAcctInf [%s] err[%d]",tbl_acct_inf0041.type,nReturnCode);
        return -1;
    }       

    /*G026-��Ӫ�̷����˻�(�������)*/
    memset(&tbl_acct_infG026,0x0,sizeof(tbl_acct_infG026));
    memcpy(tbl_acct_infG026.type,"13",2);
    nReturnCode = DbsTblAcctInf(DBS_SELECT, &tbl_acct_infG026);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAcctInf [%s] err[%d]",tbl_acct_infG026.type,nReturnCode);
        return -1;
    }           
           
    memset(&dbtbl_acct_rslt, 0x00, sizeof(dbtbl_acct_rslt));    
    memcpy(dbtbl_acct_rslt.acct_date, stlm_dt, 8);
    memcpy(dbtbl_acct_rslt.acct_statu , "N", 1);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "acct_date:[%s], acct_statu:[%s]", dbtbl_acct_rslt.acct_date, dbtbl_acct_rslt.acct_statu);
    /* �α꣬���˽����*/
    nReturnCode = DbsTblAcctRslt(DBS_CURSOR1, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);	
    nReturnCode = DbsTblAcctRslt(DBS_OPEN1, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
    if (nReturnCode)
    {
	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN1 tbl_rslt_sett_cur1 error [%d]", nReturnCode);
	    return -1;
    }
    
    while(1)
    {
        /* fetch ÿ��δ���˵ļ�¼ */
        memset(&dbtbl_acct_rslt, 0x0, sizeof(dbtbl_acct_rslt));
	    nReturnCode = DbsTblAcctRslt(DBS_FETCH1, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);   
        if (nReturnCode && nReturnCode != DBS_FETCHNULL)
        {
            if (nReturnCode == DBS_NOTFOUND)
            {
                break;
            }

	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_FETCH1 tbl_rslt_sett_cur_1 error [%d]", nReturnCode);
	        DbsTblAcctRslt(DBS_CLOSE1, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
            return -1;
        }	    
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "inter_brh_code:[%s].", dbtbl_acct_rslt.inter_brh_code);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_role:[%s].", dbtbl_acct_rslt.mcht_role);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_rslt_no:[%s].", dbtbl_acct_rslt.mcht_rslt_no);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "trans_num:[%d].", dbtbl_acct_rslt.trans_num);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "debt_at:[%.2f].", dbtbl_acct_rslt.debt_at);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "cret_at:[%.2f].", dbtbl_acct_rslt.cret_at);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "debt_net_fee:[%.2f].", dbtbl_acct_rslt.debt_net_fee);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "cret_net_fee:[%.2f].", dbtbl_acct_rslt.cret_net_fee);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_md:[%s].", dbtbl_acct_rslt.sett_md);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_bank:[%s].", dbtbl_acct_rslt.mcht_sett_bank);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_bank_nm:[%s].", dbtbl_acct_rslt.mcht_sett_bank_nm);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_acct_type:[%s].", dbtbl_acct_rslt.mcht_sett_acct_type);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_acct:[%s].", dbtbl_acct_rslt.mcht_sett_acct);            
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_acct_nm:[%s].", dbtbl_acct_rslt.mcht_sett_acct_nm);      
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_sett_bank:[%s].", dbtbl_acct_rslt.plat_sett_bank);            
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_sett_bank_nm:[%s].", dbtbl_acct_rslt.plat_sett_bank_nm);      
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_sett_acct_type:[%s].", dbtbl_acct_rslt.plat_sett_acct_type);  
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_sett_acct:[%s].", dbtbl_acct_rslt.plat_sett_acct);            
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_sett_acct_nm:[%s].", dbtbl_acct_rslt.plat_sett_acct_nm);      
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "acct_statu:[%s].", dbtbl_acct_rslt.acct_statu);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sz_status:[%s].", dbtbl_acct_rslt.sz_status);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "seq_num:[%d].", dbtbl_acct_rslt.seq_num);      
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "========================="); 

	    if(dbtbl_acct_rslt.debt_at <= 0.0000001 && 
            dbtbl_acct_rslt.debt_at >= -0.0000001 &&
            dbtbl_acct_rslt.cret_at <= 0.0000001 && 
            dbtbl_acct_rslt.cret_at >= -0.0000001)
        {
            HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "debt_at 0 [%f] cret_at 0[%f] ������Ϊ��",dbtbl_acct_rslt.debt_at ,dbtbl_acct_rslt.cret_at);

	        RightTrim(dbtbl_acct_rslt.mcht_rslt_no);
	        RightTrim(dbtbl_acct_rslt.mcht_role);
	        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_rslt_no:[%s].", dbtbl_acct_rslt.mcht_rslt_no);
	        memcpy(dbtbl_acct_rslt.acct_statu, "8", 1);
	        nReturnCode = DbsTblAcctRslt(DBS_UPDATE, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
	        if (nReturnCode == DBS_NOTFOUND)
	        {
	            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPDATE ht_tbl_acct_rslt error [%d]", nReturnCode);
	            DbsTblAcctRslt(DBS_CLOSE1, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
	            fclose (fp);
	            return -1;
	        }
        }
	
	    if(dbtbl_acct_rslt.debt_at -dbtbl_acct_rslt.cret_at < 0.0000001 )
        {
            HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "debt_at 0 [%f]�cret_at 0[%f] ,�������Ϊ���������˴���",dbtbl_acct_rslt.debt_at ,dbtbl_acct_rslt.cret_at);
            iTotolCount++;       
            /* ����¼��Ϣд���ļ��� */
	        memset(sDateTime,0x00,sizeof(sDateTime));
	        getSysTime(sDateTime);

	        if(strncmp(dbtbl_acct_rslt.mcht_role, "1001",4) == 0)
	        {
	        /*���������˻�0041 -->��Ӫ�̷����˻�(�������) G026*/
	        memset(&sthost_acct,0x00,sizeof(sthost_acct));
	        rtrim(dbtbl_acct_rslt.mcht_rslt_no);
	        rtrim(dbtbl_acct_rslt.mcht_role);
	        sprintf(sthost_acct.seq_no,     "%s%s",    dbtbl_acct_rslt.mcht_rslt_no,dbtbl_acct_rslt.mcht_role);      /*������ˮ��*/
	        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*���÷�ϵͳID*/
	        memcpy(sthost_acct.chl_id,     "����",    sizeof(sthost_acct.chl_id)-1);      /* ���� */
	        memcpy(sthost_acct.trans_date, dbtbl_acct_rslt.sett_date,  sizeof(sthost_acct.trans_date)-1);  /*��������*/
	        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*����ʱ��*/
	        memcpy(sthost_acct.bank_code,  tbl_acct_inf0041.bank_code, sizeof(sthost_acct.bank_code)-1);   /*ת�������к� */
	        memcpy(sthost_acct.acct_type,  tbl_acct_inf0041.acct_type, sizeof(sthost_acct.acct_type)-1);   /* ת���˺����� */
	        memcpy(sthost_acct.acct_no,    tbl_acct_inf0041.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* ת���˺� */
	        memcpy(sthost_acct.acct_name,  tbl_acct_inf0041.acct_name, sizeof(sthost_acct.acct_name)-1);   /* ת�������� */
	        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* ���� */  
	        sthost_acct.amt = (dbtbl_acct_rslt.debt_at - dbtbl_acct_rslt.cret_at)/100;                                                   /*���׽��*/
	        memcpy(sthost_acct.bank_code1, tbl_acct_infG026.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* ת�뽻���к� */        
	        memcpy(sthost_acct.acct_type1, tbl_acct_infG026.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* ת���˺����� */
	        memcpy(sthost_acct.acct_no1,   tbl_acct_infG026.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* ת���˺� */
	        memcpy(sthost_acct.acct_name1, tbl_acct_infG026.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* ת�뷽���� */     
	        memcpy(sthost_acct.dc_flag,    "11",                    sizeof(sthost_acct.dc_flag)-1); 
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
	        else if(strncmp(dbtbl_acct_rslt.mcht_role, "1002",4) == 0)
	        {
	         /*���������˻�0041 -->��Ӫ�̷����˻�(�������) G026*/
	        memset(&sthost_acct,0x00,sizeof(sthost_acct));
	        rtrim(dbtbl_acct_rslt.mcht_rslt_no);
	        rtrim(dbtbl_acct_rslt.mcht_role);
	        sprintf(sthost_acct.seq_no,     "%s%s",    dbtbl_acct_rslt.mcht_rslt_no,dbtbl_acct_rslt.mcht_role);      /*������ˮ��*/
	        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*���÷�ϵͳID*/
	        memcpy(sthost_acct.chl_id,     "ֱ��",    sizeof(sthost_acct.chl_id)-1);      /* ���� */
	        memcpy(sthost_acct.trans_date, dbtbl_acct_rslt.sett_date,  sizeof(sthost_acct.trans_date)-1);  /*��������*/
	        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*����ʱ��*/
	        memcpy(sthost_acct.bank_code,  tbl_acct_inf0041.bank_code, sizeof(sthost_acct.bank_code)-1);   /*ת�������к� */
	        memcpy(sthost_acct.acct_type,  tbl_acct_inf0041.acct_type, sizeof(sthost_acct.acct_type)-1);   /* ת���˺����� */
	        memcpy(sthost_acct.acct_no,    tbl_acct_inf0041.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* ת���˺� */
	        memcpy(sthost_acct.acct_name,  tbl_acct_inf0041.acct_name, sizeof(sthost_acct.acct_name)-1);   /* ת�������� */
	        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* ���� */  
	        sthost_acct.amt = (dbtbl_acct_rslt.debt_at - dbtbl_acct_rslt.cret_at)/100;                                                   /*���׽��*/
	        memcpy(sthost_acct.bank_code1, tbl_acct_infG026.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* ת�뽻���к� */        
	        memcpy(sthost_acct.acct_type1, tbl_acct_infG026.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* ת���˺����� */
	        memcpy(sthost_acct.acct_no1,   tbl_acct_infG026.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* ת���˺� */
	        memcpy(sthost_acct.acct_name1, tbl_acct_infG026.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* ת�뷽���� */     
	        memcpy(sthost_acct.dc_flag,    "11",                    sizeof(sthost_acct.dc_flag)-1); 
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
	        else if(strncmp(dbtbl_acct_rslt.mcht_role, "2002",4) == 0)
	        {
	         /*���������˻�0041 -->��Ӫ�̷����˻�(�������) G026*/
	        memset(&sthost_acct,0x00,sizeof(sthost_acct));
	        rtrim(dbtbl_acct_rslt.mcht_rslt_no);
	        rtrim(dbtbl_acct_rslt.mcht_role);
	        sprintf(sthost_acct.seq_no,     "%s%s",    dbtbl_acct_rslt.mcht_rslt_no,dbtbl_acct_rslt.mcht_role);      /*������ˮ��*/
	        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*���÷�ϵͳID*/
	        memcpy(sthost_acct.chl_id,     "ֱ��",    sizeof(sthost_acct.chl_id)-1);      /* ���� */
	        memcpy(sthost_acct.trans_date, dbtbl_acct_rslt.sett_date,  sizeof(sthost_acct.trans_date)-1);  /*��������*/
	        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*����ʱ��*/
	        memcpy(sthost_acct.bank_code,  tbl_acct_inf0041.bank_code, sizeof(sthost_acct.bank_code)-1);   /*ת�������к� */
	        memcpy(sthost_acct.acct_type,  tbl_acct_inf0041.acct_type, sizeof(sthost_acct.acct_type)-1);   /* ת���˺����� */
	        memcpy(sthost_acct.acct_no,    tbl_acct_inf0041.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* ת���˺� */
	        memcpy(sthost_acct.acct_name,  tbl_acct_inf0041.acct_name, sizeof(sthost_acct.acct_name)-1);   /* ת�������� */
	        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* ���� */  
	        sthost_acct.amt = (dbtbl_acct_rslt.debt_at - dbtbl_acct_rslt.cret_at)/100;                                                   /*���׽��*/
	        memcpy(sthost_acct.bank_code1, tbl_acct_infG026.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* ת�뽻���к� */        
	        memcpy(sthost_acct.acct_type1, tbl_acct_infG026.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* ת���˺����� */
	        memcpy(sthost_acct.acct_no1,   tbl_acct_infG026.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* ת���˺� */
	        memcpy(sthost_acct.acct_name1, tbl_acct_infG026.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* ת�뷽���� */     
	        memcpy(sthost_acct.dc_flag,    "11",                    sizeof(sthost_acct.dc_flag)-1); 
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
	    
        
        }

	    RightTrim(dbtbl_acct_rslt.mcht_rslt_no);
	    RightTrim(dbtbl_acct_rslt.mcht_role);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_rslt_no:[%s].", dbtbl_acct_rslt.mcht_rslt_no);
	    memcpy(dbtbl_acct_rslt.acct_statu, "7", 1);
	    nReturnCode = DbsTblAcctRslt(DBS_UPDATE, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
	    if (nReturnCode == DBS_NOTFOUND)
	    {
	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPDATE ht_tbl_acct_rslt error [%d]", nReturnCode);
	        DbsTblAcctRslt(DBS_CLOSE1, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
	        fclose (fp);
	        return -1;
	    }
     
}

	DbsTblAcctRslt(DBS_CLOSE1, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
	return 0;
}

int NegaCancel(FILE* fp, int nBeginOffset, int nEndOffset)
{
    int     nReturnCode;
    char    stlm_dt[8+1]; 
    int     iTotolCount = 0;   
    tbl_acct_rslt_def   dbtbl_acct_rslt;      
    ht_tbl_acct_inf_def  tbl_acct_inf0026;
    ht_tbl_acct_inf_def  tbl_acct_infG026;
	char* sDateTime;
	
    stHost_Acct_def  sthost_acct;
    memset(&sthost_acct,0x00,sizeof(sthost_acct));

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
    /*0026-��ȡ��Ӫ�̷����˻���Ϣ*/
    memset(&tbl_acct_inf0026,0x0,sizeof(tbl_acct_inf0026));
    memcpy(tbl_acct_inf0026.type,"7",2);
    nReturnCode = DbsTblAcctInf(DBS_SELECT, &tbl_acct_inf0026);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAcctInf [%s] err[%d]",tbl_acct_inf0026.type,nReturnCode);
        return -1;
    }       

    /*G026-��Ӫ�̷����˻�(�������)*/
    memset(&tbl_acct_infG026,0x0,sizeof(tbl_acct_infG026));
    memcpy(tbl_acct_infG026.type,"13",2);
    nReturnCode = DbsTblAcctInf(DBS_SELECT, &tbl_acct_infG026);
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblAcctInf [%s] err[%d]",tbl_acct_infG026.type,nReturnCode);
        return -1;
    }           
           
    memset(&dbtbl_acct_rslt, 0x00, sizeof(dbtbl_acct_rslt));    
    memcpy(dbtbl_acct_rslt.acct_date, stlm_dt, 8);
    memcpy(dbtbl_acct_rslt.acct_statu , "7", 1);
    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "acct_date:[%s], acct_statu:[%s]", dbtbl_acct_rslt.acct_date, dbtbl_acct_rslt.acct_statu);
    /* �α꣬���˽����*/
    nReturnCode = DbsTblAcctRslt(DBS_CURSOR2, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);	
    nReturnCode = DbsTblAcctRslt(DBS_OPEN2, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
    if (nReturnCode)
    {
	    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN2 tbl_acct_rslt_cur2 error [%d]", nReturnCode);
	    return -1;
    }
    
    while(1)
    {
        /* fetch ÿ��δ���˵ļ�¼ */
        memset(&dbtbl_acct_rslt, 0x0, sizeof(dbtbl_acct_rslt));
	    nReturnCode = DbsTblAcctRslt(DBS_FETCH2, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);   
        if (nReturnCode && nReturnCode != DBS_FETCHNULL)
        {
            if (nReturnCode == DBS_NOTFOUND)
            {
                break;
            }

	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_FETCH2 tbl_acct_rslt_cur2 error [%d]", nReturnCode);
	        DbsTblAcctRslt(DBS_CLOSE2, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
            return -1;
        }	    
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "inter_brh_code:[%s].", dbtbl_acct_rslt.inter_brh_code);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_role:[%s].", dbtbl_acct_rslt.mcht_role);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_rslt_no:[%s].", dbtbl_acct_rslt.mcht_rslt_no);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "trans_num:[%d].", dbtbl_acct_rslt.trans_num);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "debt_at:[%.2f].", dbtbl_acct_rslt.debt_at);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "cret_at:[%.2f].", dbtbl_acct_rslt.cret_at);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "debt_net_fee:[%.2f].", dbtbl_acct_rslt.debt_net_fee);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "cret_net_fee:[%.2f].", dbtbl_acct_rslt.cret_net_fee);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sett_md:[%s].", dbtbl_acct_rslt.sett_md);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_bank:[%s].", dbtbl_acct_rslt.mcht_sett_bank);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_bank_nm:[%s].", dbtbl_acct_rslt.mcht_sett_bank_nm);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_acct_type:[%s].", dbtbl_acct_rslt.mcht_sett_acct_type);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_acct:[%s].", dbtbl_acct_rslt.mcht_sett_acct);            
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_sett_acct_nm:[%s].", dbtbl_acct_rslt.mcht_sett_acct_nm);      
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_sett_bank:[%s].", dbtbl_acct_rslt.plat_sett_bank);            
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_sett_bank_nm:[%s].", dbtbl_acct_rslt.plat_sett_bank_nm);      
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_sett_acct_type:[%s].", dbtbl_acct_rslt.plat_sett_acct_type);  
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_sett_acct:[%s].", dbtbl_acct_rslt.plat_sett_acct);            
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "plat_sett_acct_nm:[%s].", dbtbl_acct_rslt.plat_sett_acct_nm);      
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "acct_statu:[%s].", dbtbl_acct_rslt.acct_statu);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sz_status:[%s].", dbtbl_acct_rslt.sz_status);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "seq_num:[%d].", dbtbl_acct_rslt.seq_num);      
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "========================="); 

	
	    if(dbtbl_acct_rslt.debt_at -dbtbl_acct_rslt.cret_at < 0.0000001 )
        {
            HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "debt_at 0 [%f]�cret_at 0[%f] ,�������Ϊ���������˴���",dbtbl_acct_rslt.debt_at ,dbtbl_acct_rslt.cret_at);
            iTotolCount++;       
            /* ����¼��Ϣд���ļ��� */
	        memset(sDateTime,0x00,sizeof(sDateTime));
	        getSysTime(sDateTime);

	        if(strncmp(dbtbl_acct_rslt.mcht_role, "1001",4) == 0)
	        {
	        /*��Ӫ�̷����˻�(�������) G026 --> ��Ӫ�̷����˻�0026*/
	        memset(&sthost_acct,0x00,sizeof(sthost_acct));
	        rtrim(dbtbl_acct_rslt.mcht_rslt_no);
	        rtrim(dbtbl_acct_rslt.mcht_role);
	        sprintf(sthost_acct.seq_no,     "%s%s",    dbtbl_acct_rslt.mcht_rslt_no,dbtbl_acct_rslt.mcht_role);      /*������ˮ��*/
	        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*���÷�ϵͳID*/
	        memcpy(sthost_acct.chl_id,     "����",    sizeof(sthost_acct.chl_id)-1);      /* ���� */
	        memcpy(sthost_acct.trans_date, dbtbl_acct_rslt.sett_date,  sizeof(sthost_acct.trans_date)-1);  /*��������*/
	        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*����ʱ��*/
	        memcpy(sthost_acct.bank_code,  tbl_acct_infG026.bank_code, sizeof(sthost_acct.bank_code)-1);   /*ת�������к� */
	        memcpy(sthost_acct.acct_type,  tbl_acct_infG026.acct_type, sizeof(sthost_acct.acct_type)-1);   /* ת���˺����� */
	        memcpy(sthost_acct.acct_no,    tbl_acct_infG026.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* ת���˺� */
	        memcpy(sthost_acct.acct_name,  tbl_acct_infG026.acct_name, sizeof(sthost_acct.acct_name)-1);   /* ת�������� */
	        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* ���� */  
	        sthost_acct.amt = (dbtbl_acct_rslt.debt_at - dbtbl_acct_rslt.cret_at)/100;                                                   /*���׽��*/
	        memcpy(sthost_acct.bank_code1, tbl_acct_inf0026.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* ת�뽻���к� */        
	        memcpy(sthost_acct.acct_type1, tbl_acct_inf0026.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* ת���˺����� */
	        memcpy(sthost_acct.acct_no1,   tbl_acct_inf0026.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* ת���˺� */
	        memcpy(sthost_acct.acct_name1, tbl_acct_inf0026.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* ת�뷽���� */     
	        memcpy(sthost_acct.dc_flag,    "11",                    sizeof(sthost_acct.dc_flag)-1); 
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
	        else if(strncmp(dbtbl_acct_rslt.mcht_role, "1002",4) == 0)
	        {
	             /*��Ӫ�̷����˻�(�������) G026 --> ��Ӫ�̷����˻�0026*/
	        memset(&sthost_acct,0x00,sizeof(sthost_acct));
	        rtrim(dbtbl_acct_rslt.mcht_rslt_no);
	        rtrim(dbtbl_acct_rslt.mcht_role);
	        sprintf(sthost_acct.seq_no,     "%s%s",    dbtbl_acct_rslt.mcht_rslt_no,dbtbl_acct_rslt.mcht_role);      /*������ˮ��*/
	        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*���÷�ϵͳID*/
	        memcpy(sthost_acct.chl_id,     "ֱ��",    sizeof(sthost_acct.chl_id)-1);      /* ���� */
	        memcpy(sthost_acct.trans_date, dbtbl_acct_rslt.sett_date,  sizeof(sthost_acct.trans_date)-1);  /*��������*/
	        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*����ʱ��*/
	        memcpy(sthost_acct.bank_code,  tbl_acct_infG026.bank_code, sizeof(sthost_acct.bank_code)-1);   /*ת�������к� */
	        memcpy(sthost_acct.acct_type,  tbl_acct_infG026.acct_type, sizeof(sthost_acct.acct_type)-1);   /* ת���˺����� */
	        memcpy(sthost_acct.acct_no,    tbl_acct_infG026.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* ת���˺� */
	        memcpy(sthost_acct.acct_name,  tbl_acct_infG026.acct_name, sizeof(sthost_acct.acct_name)-1);   /* ת�������� */
	        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* ���� */  
	        sthost_acct.amt = (dbtbl_acct_rslt.debt_at - dbtbl_acct_rslt.cret_at)/100;                                                   /*���׽��*/
	        memcpy(sthost_acct.bank_code1, tbl_acct_inf0026.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* ת�뽻���к� */        
	        memcpy(sthost_acct.acct_type1, tbl_acct_inf0026.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* ת���˺����� */
	        memcpy(sthost_acct.acct_no1,   tbl_acct_inf0026.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* ת���˺� */
	        memcpy(sthost_acct.acct_name1, tbl_acct_inf0026.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* ת�뷽���� */     
	        memcpy(sthost_acct.dc_flag,    "11",                    sizeof(sthost_acct.dc_flag)-1); 
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
	        else if(strncmp(dbtbl_acct_rslt.mcht_role, "2002",4) == 0)
	        {
	             /*��Ӫ�̷����˻�(�������) G026 --> ��Ӫ�̷����˻�0026*/
	        memset(&sthost_acct,0x00,sizeof(sthost_acct));
	        rtrim(dbtbl_acct_rslt.mcht_rslt_no);
	        rtrim(dbtbl_acct_rslt.mcht_role);
	        sprintf(sthost_acct.seq_no,     "%s%s",    dbtbl_acct_rslt.mcht_rslt_no,dbtbl_acct_rslt.mcht_role);      /*������ˮ��*/
	        memcpy(sthost_acct.top_cpg,    "TOPCPG" ,                  sizeof(sthost_acct.top_cpg)-1);     /*���÷�ϵͳID*/
	        memcpy(sthost_acct.chl_id,     "ֱ��",    sizeof(sthost_acct.chl_id)-1);      /* ���� */
	        memcpy(sthost_acct.trans_date, dbtbl_acct_rslt.sett_date,  sizeof(sthost_acct.trans_date)-1);  /*��������*/
	        memcpy(sthost_acct.trans_time, sDateTime+8,                sizeof(sthost_acct.trans_time)-1);  /*����ʱ��*/
	        memcpy(sthost_acct.bank_code,  tbl_acct_infG026.bank_code, sizeof(sthost_acct.bank_code)-1);   /*ת�������к� */
	        memcpy(sthost_acct.acct_type,  tbl_acct_infG026.acct_type, sizeof(sthost_acct.acct_type)-1);   /* ת���˺����� */
	        memcpy(sthost_acct.acct_no,    tbl_acct_infG026.acct_no,   sizeof(sthost_acct.acct_no)-1);     /* ת���˺� */
	        memcpy(sthost_acct.acct_name,  tbl_acct_infG026.acct_name, sizeof(sthost_acct.acct_name)-1);   /* ת�������� */
	        memcpy(sthost_acct.ccy,        "156",                  sizeof(sthost_acct.ccy)-1);         /* ���� */  
	        sthost_acct.amt = (dbtbl_acct_rslt.debt_at - dbtbl_acct_rslt.cret_at)/100;                                                   /*���׽��*/
	        memcpy(sthost_acct.bank_code1, tbl_acct_inf0026.bank_code, sizeof(sthost_acct.bank_code1)-1);  /* ת�뽻���к� */        
	        memcpy(sthost_acct.acct_type1, tbl_acct_inf0026.acct_type, sizeof(sthost_acct.acct_type1)-1);  /* ת���˺����� */
	        memcpy(sthost_acct.acct_no1,   tbl_acct_inf0026.acct_no,   sizeof(sthost_acct.acct_no1)-1);    /* ת���˺� */
	        memcpy(sthost_acct.acct_name1, tbl_acct_inf0026.acct_name, sizeof(sthost_acct.acct_name1)-1);  /* ת�뷽���� */     
	        memcpy(sthost_acct.dc_flag,    "11",                    sizeof(sthost_acct.dc_flag)-1); 
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
	    
        
        }
		   
        RightTrim(dbtbl_acct_rslt.mcht_rslt_no);
	    RightTrim(dbtbl_acct_rslt.mcht_role);
	    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "==mcht_rslt_no:[%s].", dbtbl_acct_rslt.mcht_rslt_no);
	    memcpy(dbtbl_acct_rslt.acct_statu, "Z", 1);
	    nReturnCode = DbsTblAcctRslt(DBS_UPDATE, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
	    if (nReturnCode == DBS_NOTFOUND)
	    {
	        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPDATE ht_tbl_acct_rslt error [%d]", nReturnCode);
	        DbsTblAcctRslt(DBS_CLOSE2, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
	        fclose (fp);
	        return -1;
	    }
  

}
	
	DbsTblAcctRslt(DBS_CLOSE2, &dbtbl_acct_rslt,nBeginOffset,nEndOffset);
	return 0;

}


/*************************************************
*�������ơ� Total_8702
*�������ܡ�
*���������
*���������
*�������ء� 1
***************************************************/
int Total_8702()
{
    return 1;
}

/*************************************************
*�������ơ� Task_8702
*�������ܡ� ����������¼���ɺ��Ĺ����ļ�
*��������� nBeginOffset��nEndOffset
*���������
*�������ء� 0  -- �ɹ�
            -1 -- ʧ��
***************************************************/
int Task_8702 ( int nBeginOffset, int nEndOffset )
{
    int     nReturnCode;
    char    sDateTime[14+1];
    char    stlm_dt[8 + 1];

    char    *pstr;
    FILE    *fp;
    char    sFileFullName[200];
    char    sTmpFileName[100];  
    tbl_file_info_def       dbtbl_file_info;

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);    
   
    memset(sTmpFileName, 0x00, sizeof(sTmpFileName));
    strcpy(sTmpFileName, "HOST_YYYYMMDD_XX_ACCT.txt");
    pstr = strstr(sTmpFileName, "YYYYMMDD");
    if (pstr)
        memcpy(pstr, stlm_dt, 8);
    pstr = strstr(sTmpFileName, "XX");
    if (pstr)
        memcpy (pstr, "11", 2);
                
    /* ��ʼ�ļ���Ϣ */
    memset(sDateTime, 0x00, sizeof(sDateTime));
    getSysTime(sDateTime);
    memset(&dbtbl_file_info, 0x00, sizeof(dbtbl_file_info));
    memcpy(dbtbl_file_info.sett_date, stlm_dt, 8);
    memcpy(dbtbl_file_info.snd_time, sDateTime, 14);
    memcpy(dbtbl_file_info.step_no, "04", 2);
    memcpy(dbtbl_file_info.batch_no, "11", 2);
    memcpy(dbtbl_file_info.file_type, "H", 1);
    memcpy(dbtbl_file_info.file_flag, "S", 1);
    memcpy(dbtbl_file_info.end_flag, "Y", 1);
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

    nReturnCode = NegaHand(fp, nBeginOffset, nEndOffset);
    if(nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "NegaHand err[%d]", nReturnCode);
        fclose(fp);
        return -1;
    }

    nReturnCode = NegaCancel(fp, nBeginOffset, nEndOffset);
    if(nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "NegaCancel err[%d]", nReturnCode);
        fclose(fp);
        return -1;
    }

    fclose(fp);
    memcpy(dbtbl_file_info.status, "0", 1);   
    /* �����ļ���Ϣ�� */
    nReturnCode = iInsertTblFileInfo(dbtbl_file_info);
    if (nReturnCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "iInsertTblFileInfo error [%d]", nReturnCode);
        return -1;
    }

    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_8702 Succ stlm_dt[%s].",stlm_dt);
 
    return 0;
}





/****************************** end of Task_8701.c ******************************/


