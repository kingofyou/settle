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
 *  �� �� ��: Task_0109.c
 *  ��    ��: ȷ�ϸ��Ѷ�����ˮ�ļ�
 *  �����Ա: wyl
 *  ����ʱ��: 2016-12-29
 *  ��    ע:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include "batch.h"

extern char	gLogFile[LOG_NAME_LEN_MAX];
extern  tbl_date_inf_def dbtbl_date_inf;
extern	char ext_inter_brh_code[10 + 1];

/*************************************************
*�������ơ� Total_0109
*�������ܡ�
*���������
*���������
*�������ء� 1
***************************************************/
int Total_0109()
{
    return 1;
}

/*************************************************
*�������ơ� Task_0109
*�������ܡ�
*��������� nBeginOffset��nEndOffset
*���������
*�������ء� 0  -- �ɹ�
            -1 -- ʧ��
***************************************************/
int Task_0109 ( int nBeginOffset, int nEndOffset )
{
    int		iRet = 0;
    char 	sFlag[1 + 1] = {0};
    char    *pstr;
    char	local_file_name[256] = {0};
    char	share_file_name[256] = {0};
    char    sCommand[256] = {0};
	char    sCmd[256] = {0};
	char    sFileName[256] = {0};
	int     nReturnCode = 0;
	char    sCurMisnHr[2 + 1];
	char    sms_content[512] = {0};

    stExtnFileRegDef StExtnFileReg;
    struct stat stat_buf_share;
    struct stat stat_buf_alipay;
    
    /* ��ʼ�����ļ���Ϣ */
    memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
    memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
    memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
    //memcpy(StExtnFileReg.host_name, "ALPAY", 5);
	strcpy(StExtnFileReg.src_file_name, "fuiouFile_YYYYMMDD_TXN.txt");
	pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
    if (pstr)
        memcpy (pstr, dbtbl_date_inf.stoday, 8);

	iRet = getExtnFileFullName(&StExtnFileReg, sFileName);
    if (iRet != 0)
    {
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "getExtnFileFullName: error src_file_name[%s]...", StExtnFileReg.src_file_name);
        return -1;
    }

	tbl_key_cert_def KeyInf;
	memset(&KeyInf, 0x00, sizeof(KeyInf));
	strcpy(KeyInf.pay_way, "fuiou");
	strcpy(KeyInf.merc_type, "2");
	nReturnCode = DbsKeyCertInf(DBS_CURSOR, &KeyInf);
	if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsKeyCertInf cursor err. nReturnCode = [%d].", nReturnCode);
        return -1;
    }
    DbsKeyCertInf(DBS_OPEN, &KeyInf);
	while(1) {
        memset(&KeyInf, 0x00, sizeof(KeyInf));
        nReturnCode = DbsKeyCertInf(DBS_FETCH, &KeyInf);
		if(nReturnCode && nReturnCode != DBS_NOTFOUND) {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsKeyCertInf fetch err. nReturnCode = [%d].", nReturnCode);
            return -1;
		}
		else if(nReturnCode == DBS_NOTFOUND) {
		    break;
	    }    
        CommonRTrim(KeyInf.paying_merc_code); 
        /* ��ʼ�����ļ���Ϣ */
		memset(local_file_name, 0, sizeof(local_file_name));
		memset(share_file_name, 0, sizeof(share_file_name));

        memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
        memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
        memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
        //memcpy(StExtnFileReg.host_name, "ALPAY", 5);
        sprintf(StExtnFileReg.src_file_name, "%s_YYYYMMDD_TXN.txt", KeyInf.paying_merc_code);

        pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
        if (pstr)
            memcpy (pstr, dbtbl_date_inf.stoday, 8);

        iRet = getShareFileFullName(&StExtnFileReg, share_file_name);
        if (iRet != 0)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "getShareFileFullName: error src_file_name[%s]...", StExtnFileReg.src_file_name);
            return -1;
        }
    
        /* ��ʼ�ļ���Ϣ */
        memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
        memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
        memcpy(StExtnFileReg.stlm_dt, dbtbl_date_inf.stoday, 8);
        //memcpy(StExtnFileReg.host_name, "ALPAY", 5);
        sprintf(StExtnFileReg.src_file_name, "%s_YYYYMMDD_TXN.txt", KeyInf.paying_merc_code);

        pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
        if (pstr)
             memcpy (pstr, dbtbl_date_inf.stoday, 8);

        iRet = getExtnFileFullName(&StExtnFileReg, local_file_name);
        if (iRet != 0)
        {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "getExtnFileFullName: error src_file_name[%s]...", StExtnFileReg.src_file_name);
            return -1;
        }
   
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "���ڵȴ�֧���������ļ�:[%s]...", local_file_name);

        while (1)
        {
            /* ȷ���ļ��Ƿ���� */
            iRet = access(share_file_name, R_OK);
            if (iRet != 0)
            {
		    	// �ж��Ƿ�ʱδ��ȡ��
		    	/* �ж�ʱ���Ƿ�ﵽ��ʱ��Ҫ�� */
			    // 4���δ���ص������ű���
                memset(sCurMisnHr, 0, 2 + 1);
                int nReturnCode = DbsTimeStamp(DBS_SELECT1, NULL, NULL, sCurMisnHr, NULL);
                if(nReturnCode)
                {
                    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "get time error [%d].", nReturnCode);               
                    return -1;
                }
		    	if(memcmp("04", sCurMisnHr, 2) <= 0) {
                    sleep(2);
			    	continue;
		    	}
		    	else {
                    // ������
				    memset(sms_content, 0, sizeof(sms_content));
				    sprintf(sms_content, "��ʱδ��ȡ��֧���������ļ�[%s]", share_file_name);
                    nReturnCode = sendMobileMsg(sms_content);
				    if(nReturnCode) {
                        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "��������[%s]����ʧ��", sms_content);
				    	sleep(2);
				    	continue;
			    	}

				    sleep(2);
		    	}            
            }
            else
            {
                break;
            }
        }
    
    	int sharefilesize1 = 0;
	    int sharefilesize2 = 0;
	    // ����ļ��Ƿ���������
	    memset(&stat_buf_share, 0x00, sizeof(stat_buf_share));
    	if(stat(share_file_name, &stat_buf_share) == -1)
        {
            HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sleep");
            sharefilesize1 = 0;
        }
    	else {
	    	sharefilesize1 = stat_buf_share.st_size;
    	}
	    sleep(2);
        while (1)
        {
		    memset(&stat_buf_share, 0x00, sizeof(stat_buf_share));
	    	if(stat(share_file_name, &stat_buf_share) == -1)
            {
                HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sleep");
                sleep(2);
                continue;
            }
	    	sharefilesize2 = stat_buf_share.st_size;
		    // �ļ�����ȫ����
		    if(sharefilesize1 == sharefilesize2) {
                // �������ļ��ɹ���Ŀ¼���ص�����·��
		        memset(sCommand, 0, sizeof(sCommand));
		    	sprintf(sCommand, "cp -f %s %s", share_file_name, local_file_name);
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sCommand:[%s]...", sCommand);
                iRet = cmdSystem(sCommand);
                if (iRet != 0)
                {
                    sleep(2);
                    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sCommand:[%s]...", sCommand);
                    continue;
                }

		        /* ȷ���ļ��Ƿ���� */
                iRet = access(local_file_name, R_OK);
                if (iRet != 0)
                {
                    sleep(2);
                    continue;
                }

		        /* ���Ŀ���ļ���СΪ0����ԴĿ���ļ���С��Ϊ0�����¸��� */   
                memset(&stat_buf_alipay, 0x00, sizeof(stat_buf_alipay));
                if(stat(local_file_name, &stat_buf_alipay) == -1)
                {
                    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sleep");
                    sleep(2);
                    continue;
                }

                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, " fileSize[%d][%d]", stat_buf_share.st_size, stat_buf_alipay.st_size);
                if(sharefilesize1 != 0 && stat_buf_alipay.st_size == 0)
                {
                    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, " fileSize[%d][%d]", stat_buf_share.st_size, stat_buf_alipay.st_size);
                    sleep(2);
                    continue;
                }
            }
	    	else {
			    sharefilesize1 = sharefilesize2;
			    sleep(2);
                continue;
		    }        
            break;
        }

		while(1) {
            memset(sCmd, 0, sizeof(sCmd));
            sprintf(sCmd, "cat %s >> %s", local_file_name, sFileName);
		    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "sCmd[%s].",sCmd);
            iRet = cmdSystem(sCmd);
            if (iRet != 0)
            {
                HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sCmd:[%s]...", sCmd);
                continue;
            }
			else break;
		}
		HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "֧�����̻�[%s]�����ļ��ѵ���.", KeyInf.paying_merc_code);
    }

    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "֧���������ļ��ѵ���.");
    return 0;
}

/****************************** end of Task_0104.c ******************************/
