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
 *  �� �� ��:  Task_6202.c
 *  ��     �ܣ������̻����׶�����ϸ��
 *  �����Ա:
 *  ����ʱ��:
 *  ��    ע:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include "batch.h"

extern    char    gLogFile[LOG_NAME_LEN_MAX];
extern	  char	  ext_inter_brh_code[10 + 1];
extern    tbl_date_inf_def dbtbl_date_inf;


#define   FNAME   "mcht_check_inf_table"

#define   toCHN(obj) memcmp(obj, "0", 1)?"�Թ�":"����"

/*************************************************
*�������ơ� DownloadFile
*�������ܡ� �����ļ�
*���������
*���������
*�������ء� 0  -- �ɹ�
            -1  -- ʧ��
***************************************************/
static int SendMail(char *mcht_no,const char *filename,char *sMail)
{
    char    mchtno_title[200 + 1];
    char    sh_path[200 + 1];
    char    stlm_dt[8 + 1];
    char    cmd[1024];
    int     nReturnCode;


    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mcht_no[%s] filename: [%s].",mcht_no, filename);
    /* ����ļ��Ƿ���� */
    nReturnCode = access(filename, R_OK);
    if(nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "�ļ���δ����!filename: [%s].", filename);
        return -1;
    }
    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
    
    
    memset(mchtno_title, 0x00, sizeof(mchtno_title));
    sprintf(mchtno_title,"%s_%s",mcht_no,stlm_dt);

    memset(sh_path, 0x00, sizeof(sh_path));
    sprintf(sh_path, "%s/sbin", getenv("BATCHHOME"));

    /* sftp�ϴ��ļ� */
    memset(cmd, 0x00, sizeof(cmd));
    sprintf(cmd, "cd %s; . ./sendmail.sh %s %s %s", sh_path, mchtno_title, filename,sMail);
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "cmd[%s].", cmd);
    nReturnCode = cmdSystem(cmd);
    if(nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "system error: cmd[%s].", cmd);
        return -1;
    }
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "�����̻�[%s]�ʼ��ɹ��ɹ�",mcht_no);
    return 0;
}

/*************************************************
*�������ơ� Total_6202
*�������ܡ�
*���������
*���������
*�������ء� 1
***************************************************/
int Total_6202()
{
    return 1;
}

/*************************************************
*�������ơ� Task_6202
*�������ܡ�
*��������� nBeginOffset��nEndOffset
*���������
*�������ء� 0  -- �ɹ�
            -1 -- ʧ��
***************************************************/
int Task_6202 ( int nBeginOffset, int nEndOffset )
{
    int     nReturn;
    int     Count = 0;
    double  amt_tran = 0.00;
    double  fee_business = 0.00;
    double  amt_send = 0.00;
    double  mcht_fee = 0.00;
    double  mrms_amt = 0.00;
    char    file_path[100];
    char    file_name[200];
    char    today[8 + 1];
    char    *pstr;
    char    txn_num[3 + 1];
    char    sMail[60+1];

    FILE    *fp;
    
    ht_tbl_mcht_inf_def ht_tbl_mcht_inf;
    ht_tbl_sett_flow_def ht_tbl_sett_flow;
    ht_tbl_algo_ctl_def ht_tbl_algo_ctl;
    
    memset(today, 0x00, sizeof(today));
    memcpy(today, dbtbl_date_inf.stoday, 8);

    /* �ļ����·�� */
    memset(file_path, 0x00, sizeof(file_path));
    sprintf(file_path, "%s/YYYYMMDD/%6s/FE/%s", getenv("SHARE_FILE_PATH"), ext_inter_brh_code, FNAME);
    pstr = strstr (file_path, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, today, 8);

    nReturn = CheckDir(file_path);
    if(nReturn)
    {
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "Create directory error: [%s].", file_name);
        return -1;
    }
    
    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "file_path=[%s].", file_path);

    /* �����̻� */
    memset(&ht_tbl_mcht_inf, 0x00, sizeof(ht_tbl_mcht_inf));
    DbsTblMchtInf(DBS_CURSOR, &ht_tbl_mcht_inf, nBeginOffset,nEndOffset);   
    nReturn = DbsTblMchtInf(DBS_OPEN, &ht_tbl_mcht_inf, nBeginOffset,nEndOffset);   
    if( nReturn )
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblMchtInf DBS_OPEN ERROR, sqlcode=[%d].", nReturn);
        return -1;
    }
    while(1)
    {
        memset(&ht_tbl_mcht_inf, 0x00, sizeof(ht_tbl_mcht_inf));
        nReturn = DbsTblMchtInf(DBS_FETCH, &ht_tbl_mcht_inf, nBeginOffset,nEndOffset);   
        if(nReturn != 0 && nReturn != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_FETCH DbsTblMchtInf  error sqlcode[%d]", nReturn);
            DbsTblMchtInf(DBS_CLOSE, &ht_tbl_mcht_inf, nBeginOffset,nEndOffset);  
            return -1;
        }
        else if(nReturn == DBS_NOTFOUND)
        {
            break;
        }

        /* �ļ��� */
        memset(file_name, 0x00, sizeof(file_name));
        Trim(ht_tbl_mcht_inf.mcht_no);
        sprintf(file_name, "%s/%s-%s.txt", file_path, ht_tbl_mcht_inf.mcht_no, FNAME);

        fp = fopen (file_name, "w");
        if (!fp)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fopen %s error.", file_name);
            DbsTblMchtInf(DBS_CLOSE, &ht_tbl_mcht_inf, nBeginOffset,nEndOffset);  
            return -1;
        }

        fprintf(fp, "%57.57s\n", "�̻����˵�����");
        fprintf(fp, "�������ڣ�%-77.77s��ӡ����:%s\n", today, today);
        fprintf(fp, "�̻���ţ�%-77.77s�̻�����:%s\n", ht_tbl_mcht_inf.mcht_no, ht_tbl_mcht_inf.mcht_nm);
        fprintf(fp, "%-8.8s|%-8.8s|%-8.8s|%-08.08s|%-27.27s|%-19.19s|%-12.12s|%-15.15s|%-12.12s\n",
                "��������",
                "��������",
                "����ʱ��",
                "��������",
                "��ˮ��",
                "����",
                "���׽��",
                "�̻����",
                "�̻�����");        

        /* ���ù�0 */
        Count = 0;
        amt_tran = 0.00;
        fee_business = 0.00;
        amt_send = 0.00;

        /* �������ա��̻��� */
        memset(&ht_tbl_sett_flow, 0x00, sizeof(ht_tbl_sett_flow));
        memcpy(ht_tbl_sett_flow.sett_date, today, 8);
        memcpy(ht_tbl_sett_flow.mcht_no1, ht_tbl_mcht_inf.mcht_no, sizeof(ht_tbl_sett_flow.mcht_no1));
        DbsTblSettFlow(DBS_CURSOR2, &ht_tbl_sett_flow,nBeginOffset,nEndOffset);
        nReturn = DbsTblSettFlow(DBS_OPEN2, &ht_tbl_sett_flow,nBeginOffset,nEndOffset);
        if(nReturn)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "OPEN2 ht_tbl_sett_flow error!sqlcode[%d]", nReturn);
            fclose(fp);
            DbsTblMchtInf(DBS_CLOSE, &ht_tbl_mcht_inf, nBeginOffset,nEndOffset);  
            return -1;
        }

        while(1)
        {
            memset(&ht_tbl_sett_flow, 0x00, sizeof(ht_tbl_sett_flow));
            nReturn = DbsTblSettFlow(DBS_FETCH2, &ht_tbl_sett_flow,nBeginOffset,nEndOffset);
            if(nReturn != 0 && nReturn != DBS_NOTFOUND)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_FDBS_FETCH2ETCH1 ht_tbl_sett_flow  error sqlcode[%d]", nReturn);
                fclose(fp);
                DbsTblSettFlow(DBS_CLOSE2, &ht_tbl_sett_flow,nBeginOffset,nEndOffset);
                DbsTblMchtInf(DBS_CLOSE, &ht_tbl_mcht_inf, nBeginOffset,nEndOffset);  
                return -1;
            }
            else if(nReturn == DBS_NOTFOUND)
            {
                break;
            }
                       
            memset(&ht_tbl_algo_ctl, 0x00, sizeof(ht_tbl_algo_ctl));
            memcpy(ht_tbl_algo_ctl.bus_tp, ht_tbl_sett_flow.bus_tp, sizeof(ht_tbl_algo_ctl.bus_tp)-1);
            memcpy(ht_tbl_algo_ctl.txn_num, ht_tbl_sett_flow.trans_tp, sizeof(ht_tbl_algo_ctl.txn_num)-1);           
            nReturn = DbsTblAlgoCtl(DBS_SELECT, &ht_tbl_algo_ctl,nBeginOffset, nEndOffset);
            if(nReturn)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_SELECT DbsTblAlgoCtl  error[%d],bus_tp[%s] trans_tp[%s]", nReturn, ht_tbl_sett_flow.bus_tp,ht_tbl_sett_flow.trans_tp);
                fclose(fp);
                DbsTblSettFlow(DBS_CLOSE2, &ht_tbl_sett_flow,nBeginOffset,nEndOffset);
                DbsTblMchtInf(DBS_CLOSE, &ht_tbl_mcht_inf, nBeginOffset,nEndOffset);  
                return -1;
            }
           
            fprintf(fp, "%-8.8s|%-8.8s|%-8.8s|%-8.8s|%-27.27s|%-19.19s|%12.2f|%-15.15s|%-60.60s|\n",
                    ht_tbl_sett_flow.sett_date,
                    ht_tbl_sett_flow.trans_date,
                    ht_tbl_sett_flow.trans_time,
                    ht_tbl_algo_ctl.txn_name,
                    ht_tbl_sett_flow.plat_key,
                    ht_tbl_sett_flow.pan,
                    ht_tbl_sett_flow.trans_at/100,
                    ht_tbl_mcht_inf.mcht_no,
                    ht_tbl_mcht_inf.mcht_nm);
            /* ͳ�� */
            Count++;
            amt_tran = amt_tran + ht_tbl_sett_flow.trans_at/100;
        }
        fprintf(fp, "�ܼƱ�����%-d\t���ױ���%-21.2f\n",
                Count,
                amt_tran);
        fclose(fp);
        DbsTblSettFlow(DBS_CLOSE2, &ht_tbl_sett_flow,nBeginOffset,nEndOffset);
        
        if(ht_tbl_mcht_inf.send_bill_flag[0]== 'y')
        {
            memset(sMail,0x0,sizeof(sMail));
            memcpy(sMail,ht_tbl_mcht_inf.email,sizeof(sMail)-1);
            Trim(sMail);
            nReturn = SendMail(ht_tbl_mcht_inf.mcht_no,file_name,sMail);
            if(nReturn)
            {
                HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "SendMail error!nReturn[%d]", nReturn);
                DbsTblMchtInf(DBS_CLOSE, &ht_tbl_mcht_inf, nBeginOffset,nEndOffset);  
                return -1;
            } 
        }       
    }
    DbsTblMchtInf(DBS_CLOSE, &ht_tbl_mcht_inf, nBeginOffset,nEndOffset); 

    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Task_6202 succ.");

    return 0;

}
/****************************** end of Task_6202.c ******************************/
