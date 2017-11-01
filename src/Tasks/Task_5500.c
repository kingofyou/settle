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
 *  �� �� ��: Task_5500.c
 *  ��    ��: ���ȷ��
 *  �����Ա: wyl
 *  ����ʱ��: 20170504
 *  ��    ע:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include <sys/socket.h>
#include "batch.h"
#include "Common.h"

extern  char    gLogFile[LOG_NAME_LEN_MAX];
extern  char    ext_child_date[8 + 1];
extern  char    ext_inter_brh_code[10 + 1];
extern  int     ext_inter_brh_sta;
extern          tbl_date_inf_def dbtbl_date_inf;

/*************************************************
*�������ơ� Total_5500
*�������ܡ�
*���������
*���������
*�������ء� 1
***************************************************/
int Total_5500()
{
    return 1;
}

/*************************************************
*�������ơ� Task_5500
*�������ܡ� ������ˮ�ļ����
*��������� nBeginOffset��nEndOffset
*���������
*�������ء� 0  -- �ɹ�
            -1 -- ʧ��
***************************************************/
int Task_5500 ( int nBeginOffset, int nEndOffset )
{
    int	nReturn;

    tbl_mission_inf_def      dbtbl_mission_inf;

    while(1)
    {
        /*�ȴ�֧��������������*/
        memset(&dbtbl_mission_inf, 0x00, sizeof(dbtbl_mission_inf));
        memcpy(dbtbl_mission_inf.inter_brh_code, BT_BRH_ID_ALIPAY, BT_BRH_ID_LEN);
        memcpy(dbtbl_mission_inf.settlmt_date, dbtbl_date_inf.stoday, BT_DATE_LEN);
        memcpy(dbtbl_mission_inf.mission_index, BT_MISSION_INDEX_5101, BT_MISSION_INDEX_LEN);
        nReturn = DbsTblmissionInf(DBS_SELECT4, &dbtbl_mission_inf);
        if (nReturn && nReturn != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblmissionInf error[%d]", nReturn);
            return -1;
        }
        else if (nReturn == DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "not found! settlmt_date[%s],task9031 excuting, please hold 30's.", dbtbl_mission_inf.settlmt_date);
            sleep(2);
            continue;
        }

        if (dbtbl_mission_inf.mission_status == BT_MISSION_STATUS_SUSS)
        {
            ;
        }
        else if (dbtbl_mission_inf.mission_status == BT_MISSION_STATUS_FAIL)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DbsTblmissionInf error, please check.");
            return -1;
        }
        else
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "settlmt_date[%s],task9031 excuting, please hold 30's.", dbtbl_mission_inf.settlmt_date);
            sleep(2);
            continue;
        }
        
        /*�ȴ�΢������������*/
        memset(&dbtbl_mission_inf, 0x00, sizeof(dbtbl_mission_inf));
        memcpy(dbtbl_mission_inf.inter_brh_code, BT_BRH_ID_WECHAT, BT_BRH_ID_LEN);
        memcpy(dbtbl_mission_inf.settlmt_date, dbtbl_date_inf.stoday, BT_DATE_LEN);
        memcpy(dbtbl_mission_inf.mission_index, BT_MISSION_INDEX_5101, BT_MISSION_INDEX_LEN);
        nReturn = DbsTblmissionInf(DBS_SELECT4, &dbtbl_mission_inf);
        if (nReturn && nReturn != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblmissionInf error[%d]", nReturn);
            return -1;
        }
        else if (nReturn == DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "not found! settlmt_date[%s],task9031 excuting, please hold 30's.", dbtbl_mission_inf.settlmt_date);
            sleep(2);
            continue;
        }

        if (dbtbl_mission_inf.mission_status == BT_MISSION_STATUS_SUSS)
        {
            ;
        }
        else if (dbtbl_mission_inf.mission_status == BT_MISSION_STATUS_FAIL)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DbsTblmissionInf error, please check.");
            return -1;
        }
        else
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "settlmt_date[%s],task9031 excuting, please hold 30's.", dbtbl_mission_inf.settlmt_date);
            sleep(2);
            continue;
        }
        
        /*�ȴ��������������*/
        memset(&dbtbl_mission_inf, 0x00, sizeof(dbtbl_mission_inf));
        memcpy(dbtbl_mission_inf.inter_brh_code, BT_BRH_ID_QUICK, BT_BRH_ID_LEN);
        memcpy(dbtbl_mission_inf.settlmt_date, dbtbl_date_inf.stoday, BT_DATE_LEN);
        memcpy(dbtbl_mission_inf.mission_index, BT_MISSION_INDEX_5101, BT_MISSION_INDEX_LEN);
        nReturn = DbsTblmissionInf(DBS_SELECT4, &dbtbl_mission_inf);
        if (nReturn && nReturn != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblmissionInf error[%d]", nReturn);
            return -1;
        }
        else if (nReturn == DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "not found! settlmt_date[%s],task9031 excuting, please hold 30's.", dbtbl_mission_inf.settlmt_date);
            sleep(2);
            continue;
        }

        if (dbtbl_mission_inf.mission_status == BT_MISSION_STATUS_SUSS)
        {
            ;
        }
        else if (dbtbl_mission_inf.mission_status == BT_MISSION_STATUS_FAIL)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DbsTblmissionInf error, please check.");
            return -1;
        }
        else
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "settlmt_date[%s],task9031 excuting, please hold 30's.", dbtbl_mission_inf.settlmt_date);
            sleep(2);
            continue;
        }
        
        /*�ȴ���������������*/
        memset(&dbtbl_mission_inf, 0x00, sizeof(dbtbl_mission_inf));
        memcpy(dbtbl_mission_inf.inter_brh_code, BT_BRH_ID_GATEWAY, BT_BRH_ID_LEN);
        memcpy(dbtbl_mission_inf.settlmt_date, dbtbl_date_inf.stoday, BT_DATE_LEN);
        memcpy(dbtbl_mission_inf.mission_index, BT_MISSION_INDEX_5101, BT_MISSION_INDEX_LEN);
        nReturn = DbsTblmissionInf(DBS_SELECT4, &dbtbl_mission_inf);
        if (nReturn && nReturn != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "select task status error[%d]", nReturn);
            return -1;
        }
        else if (nReturn == DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "not found! settlmt_date[%s],task9031 excuting, please hold 30's.", dbtbl_mission_inf.settlmt_date);
            sleep(2);
            continue;
        }

        if (dbtbl_mission_inf.mission_status == BT_MISSION_STATUS_SUSS)
        {
            break;
        }
        else if (dbtbl_mission_inf.mission_status == BT_MISSION_STATUS_FAIL)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "task excuted error, please check.");
            return -1;
        }
        else
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "settlmt_date[%s],task9031 excuting, please hold 30's.", dbtbl_mission_inf.settlmt_date);
            sleep(2);
            continue;
        }

		/*�ȴ���������������*/
        memset(&dbtbl_mission_inf, 0x00, sizeof(dbtbl_mission_inf));
        memcpy(dbtbl_mission_inf.inter_brh_code, BT_BRH_ID_FUIOU, BT_BRH_ID_LEN);
        memcpy(dbtbl_mission_inf.settlmt_date, dbtbl_date_inf.stoday, BT_DATE_LEN);
        memcpy(dbtbl_mission_inf.mission_index, BT_MISSION_INDEX_5101, BT_MISSION_INDEX_LEN);
        nReturn = DbsTblmissionInf(DBS_SELECT4, &dbtbl_mission_inf);
        if (nReturn && nReturn != DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsTblmissionInf error[%d]", nReturn);
            return -1;
        }
        else if (nReturn == DBS_NOTFOUND)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "not found! settlmt_date[%s],task9031 excuting, please hold 30's.", dbtbl_mission_inf.settlmt_date);
            sleep(2);
            continue;
        }

        if (dbtbl_mission_inf.mission_status == BT_MISSION_STATUS_SUSS)
        {
            ;
        }
        else if (dbtbl_mission_inf.mission_status == BT_MISSION_STATUS_FAIL)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DbsTblmissionInf error, please check.");
            return -1;
        }
        else
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "settlmt_date[%s],task9031 excuting, please hold 30's.", dbtbl_mission_inf.settlmt_date);
            sleep(2);
            continue;
        }
    }

    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Check Task_5500 Success.");

    return 0;
}

/****************************** end of Task_5500.c ******************************/

