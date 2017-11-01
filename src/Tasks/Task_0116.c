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
 *  �� �� ��: Task_0116.c
 *  ��    ��: ȷ��������ˮ�ļ�
 *  �����Ա: HELU
 *  ����ʱ��: 2016-12-13
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
*�������ơ� Wait_100001_100005
*�������ܡ�
*���������
*���������
*�������ء�
***************************************************/
int Wait_100001_100005()
{
    int nReturnCode;
    char    sToday[8 + 1];
    
    tbl_child_inf_def        dbtbl_child_inf;

    /* �������� */
    memset(sToday, 0x00, sizeof(sToday));
    memcpy(sToday,dbtbl_date_inf.stoday,8);
     
    while(1)
    {
        memset(&dbtbl_child_inf, 0x00, sizeof(dbtbl_child_inf));
        memcpy(dbtbl_child_inf.inter_brh_code, "100001", BT_BRH_ID_LEN);
        memcpy(dbtbl_child_inf.settlmt_date, sToday, BT_DATE_LEN);
        memcpy(dbtbl_child_inf.mission_index, "5502", BT_MISSION_INDEX_LEN);
        nReturnCode = DbsTblchildInf(DBS_SELECT1, &dbtbl_child_inf);
        if (nReturnCode != 0 && nReturnCode != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "select task status error[%d]", nReturnCode);
            return -1;
        }

        if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_SUSS)
        {
            break;
        }
        else if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_FAIL)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "task excuted error, please check.");
            return -1;
        }
        else
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "inter_brh_code[%s]settlmt_date[%s],task9901 excuting, please hold 30's.", dbtbl_child_inf.inter_brh_code,dbtbl_child_inf.settlmt_date);
            sleep(2);
        }
    }
    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Check inter_brh_code[%s] Task_5502 Success.",dbtbl_child_inf.inter_brh_code);
    while(1)
    {
        memset(&dbtbl_child_inf, 0x00, sizeof(dbtbl_child_inf));
        memcpy(dbtbl_child_inf.inter_brh_code, "100002", BT_BRH_ID_LEN);
        memcpy(dbtbl_child_inf.settlmt_date, sToday, BT_DATE_LEN);
        memcpy(dbtbl_child_inf.mission_index, "5502", BT_MISSION_INDEX_LEN);
        nReturnCode = DbsTblchildInf(DBS_SELECT1, &dbtbl_child_inf);
        if (nReturnCode != 0 && nReturnCode != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "select task status error[%d]", nReturnCode);
            return -1;
        }

        if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_SUSS)
        {
            break;
        }
        else if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_FAIL)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "task excuted error, please check.");
            return -1;
        }
        else
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "inter_brh_code[%s]settlmt_date[%s],task9901 excuting, please hold 30's.", dbtbl_child_inf.inter_brh_code,dbtbl_child_inf.settlmt_date);
            sleep(2);
        }
    }
    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Check inter_brh_code[%s] Task_5502 Success.",dbtbl_child_inf.inter_brh_code);

    while(1)
    {
        memset(&dbtbl_child_inf, 0x00, sizeof(dbtbl_child_inf));
        memcpy(dbtbl_child_inf.inter_brh_code, "100003", BT_BRH_ID_LEN);
        memcpy(dbtbl_child_inf.settlmt_date, sToday, BT_DATE_LEN);
        memcpy(dbtbl_child_inf.mission_index, "5501", BT_MISSION_INDEX_LEN);
        nReturnCode = DbsTblchildInf(DBS_SELECT1, &dbtbl_child_inf);
        if (nReturnCode != 0 && nReturnCode != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "select task status error[%d]", nReturnCode);
            return -1;
        }

        if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_SUSS)
        {
            break;
        }
        else if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_FAIL)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "task excuted error, please check.");
            return -1;
        }
        else
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "inter_brh_code[%s]settlmt_date[%s],task9901 excuting, please hold 30's.", dbtbl_child_inf.inter_brh_code,dbtbl_child_inf.settlmt_date);
            sleep(2);
        }
    }
    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Check inter_brh_code[%s] Task_5501 Success.",dbtbl_child_inf.inter_brh_code);

    while(1)
    {
        memset(&dbtbl_child_inf, 0x00, sizeof(dbtbl_child_inf));
        memcpy(dbtbl_child_inf.inter_brh_code, "100004", BT_BRH_ID_LEN);
        memcpy(dbtbl_child_inf.settlmt_date, sToday, BT_DATE_LEN);
        memcpy(dbtbl_child_inf.mission_index, "5502", BT_MISSION_INDEX_LEN);
        nReturnCode = DbsTblchildInf(DBS_SELECT1, &dbtbl_child_inf);
        if (nReturnCode != 0 && nReturnCode != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "select task status error[%d]", nReturnCode);
            return -1;
        }

        if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_SUSS)
        {
            break;
        }
        else if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_FAIL)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "task excuted error, please check.");
            return -1;
        }
        else
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "inter_brh_code[%s]settlmt_date[%s],task9901 excuting, please hold 30's.", dbtbl_child_inf.inter_brh_code,dbtbl_child_inf.settlmt_date);
            sleep(2);
        }
    }    

    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Check inter_brh_code[%s] Task_5502 Success.",dbtbl_child_inf.inter_brh_code);

    while(1)
    {
        memset(&dbtbl_child_inf, 0x00, sizeof(dbtbl_child_inf));
        memcpy(dbtbl_child_inf.inter_brh_code, "100005", BT_BRH_ID_LEN);
        memcpy(dbtbl_child_inf.settlmt_date, sToday, BT_DATE_LEN);
        memcpy(dbtbl_child_inf.mission_index, "5502", BT_MISSION_INDEX_LEN);
        nReturnCode = DbsTblchildInf(DBS_SELECT1, &dbtbl_child_inf);
        if (nReturnCode != 0 && nReturnCode != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "select task status error[%d]", nReturnCode);
            return -1;
        }

        if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_SUSS)
        {
            break;
        }
        else if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_FAIL)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "task excuted error, please check.");
            return -1;
        }
        else
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "inter_brh_code[%s]settlmt_date[%s],task9901 excuting, please hold 30's.", dbtbl_child_inf.inter_brh_code,dbtbl_child_inf.settlmt_date);
            sleep(2);
        }
    }    
    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Check inter_brh_code[%s] Task_5502 Success.",dbtbl_child_inf.inter_brh_code);

    while(1)
    {
        memset(&dbtbl_child_inf, 0x00, sizeof(dbtbl_child_inf));
        memcpy(dbtbl_child_inf.inter_brh_code, "100006", BT_BRH_ID_LEN);
        memcpy(dbtbl_child_inf.settlmt_date, sToday, BT_DATE_LEN);
        memcpy(dbtbl_child_inf.mission_index, "5502", BT_MISSION_INDEX_LEN);
        nReturnCode = DbsTblchildInf(DBS_SELECT1, &dbtbl_child_inf);
        if (nReturnCode != 0 && nReturnCode != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "select task status error[%d]", nReturnCode);
            return -1;
        }

        if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_SUSS)
        {
            break;
        }
        else if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_FAIL)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "task excuted error, please check.");
            return -1;
        }
        else
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "inter_brh_code[%s]settlmt_date[%s],task9901 excuting, please hold 30's.", dbtbl_child_inf.inter_brh_code,dbtbl_child_inf.settlmt_date);
            sleep(2);
        }
    }    
    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Check inter_brh_code[%s] Task_5502 Success.",dbtbl_child_inf.inter_brh_code);


    return 0;
}

/*************************************************
*�������ơ� Total_0116
*�������ܡ�
*���������
*���������
*�������ء� 1
***************************************************/
int Total_0116()
{
    return 1;
}

/*************************************************
*�������ơ� Task_0116
*�������ܡ�
*��������� nBeginOffset��nEndOffset
*���������
*�������ء� 0  -- �ɹ�
            -1 -- ʧ��
***************************************************/
int Task_0116 ( int nBeginOffset, int nEndOffset )
{
    int nReturnCode = 0;
    /* �ȴ�������� */
    nReturnCode = Wait_100001_100005();
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Wait_100001_100005 err.");
        return -1;
    }

    HtLog(gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "�����Ѵ������.");
    return 0;
}

/****************************** end of Task_0104.c ******************************/

