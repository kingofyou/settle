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
 *  �� �� ��: Task_9029.c
 *  ��    ��: �������ݱ��������
 *  �����Ա: HELU
 *  ����ʱ��: 2016-12-17
 *  ��    ע:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include "batch.h"
#include "DbsTbl.h"

extern  char    gLogFile[LOG_NAME_LEN_MAX];
extern  char    ext_child_date[8 + 1];
extern	char	ext_inter_brh_code[10 + 1];
extern	int 	ext_inter_brh_sta;

static int PartInfTblLoad( stPartitionInfDef *);
static int ClearCurrPartition(char *, stPartitionInfDef *);

/*************************************************
*�������ơ� Total_9029
*�������ܡ�
*���������
*���������
*�������ء�
***************************************************/
int Total_9029()
{
    return 1;
}

/*************************************************
*�������ơ� Task_9029
*�������ܡ�
*��������� nBeginOffset��nEndOffset
*���������
*�������ء� 0  -- �ɹ�
            -1 -- ʧ��
***************************************************/
int Task_9029 ( int nBeginOffset, int nEndOffset)
{
    int     nReturnCode;
    int     iHandleFlag;
    char    settle_date[8 + 1];
    stPartitionInfDef    stPartInf;

    memset(&stPartInf, 0, sizeof(stPartitionInfDef));

    memset(settle_date, 0x00, sizeof(settle_date));
	CalNewDate(ext_child_date, 1, settle_date);

    nReturnCode = PartInfTblLoad(&stPartInf);
    if(nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "PartInfTblLoad:Err:[%d].", nReturnCode);
        return -1;
    }

    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "---------------����ʼ----------------\n");

    nReturnCode = ClearCurrPartition(settle_date, &stPartInf);
    if(nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DropPartition:Err:[%d].", nReturnCode);
        return -1;
    }

    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_9029 Succ.\n");

    return 0;
}

/*************************************************
*�������ơ� PartInfTblLoad
*�������ܡ� װ�ر� tbl_part_inf �е����ݵ��ṹ����
*��������� vfripPartInf->stPartInf[0].iTblOprFlag
*��������� vfripPartInf
*�������ء� 0  -- �ɹ�
            -1 -- ʧ��
***************************************************/
int PartInfTblLoad( stPartitionInfDef *vfripPartInf)
{
    int          liFldX;
    int          nReturnCode;
    Tbl_part_inf_Def vtpPartInf;

    liFldX = 0;

    HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Start to load table tbl_part_inf's data ...");

    memset(&vtpPartInf, 0, sizeof( Tbl_part_inf_Def));

    DbsPartInf(DBS_CURSOR, &vtpPartInf);
    nReturnCode = DbsPartInf( DBS_OPEN, &vtpPartInf);
    if(nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsOprInf DBS_OPEN err:[%d]", nReturnCode);
        return -1;
    }

    while(1)
    {
        memset(&vtpPartInf, 0x00, sizeof(vtpPartInf));

        nReturnCode = DbsPartInf( DBS_FETCH, &vtpPartInf);
        if (nReturnCode != 0 && nReturnCode != DBS_NOTFOUND && nReturnCode != DBS_FETCHNULL)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsOprInf DBS_FETCH err:[%d]", nReturnCode);
            DbsPartInf( DBS_CLOSE, &vtpPartInf);
            return -1;
        }
        else if (nReturnCode == DBS_NOTFOUND)
        {
            break;
        }

        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Load Table: [%s]", vtpPartInf.tbl_name);

        memcpy( vfripPartInf->stPartInf[liFldX].sTblName, vtpPartInf.tbl_name, 60);
        memcpy( vfripPartInf->stPartInf[liFldX].sPartName, vtpPartInf.part_name, 60);
        memcpy( vfripPartInf->stPartInf[liFldX].sTblSpcName, vtpPartInf.tbl_spc_name, 60);
        vfripPartInf->stPartInf[liFldX].iIntervalDays = vtpPartInf.tbl_interval_days;
        vfripPartInf->stPartInf[liFldX].iOprFlag = vtpPartInf.tbl_opr_flag;

        liFldX++;
    }

    DbsPartInf( DBS_CLOSE, &vtpPartInf);

    vfripPartInf->nPartInfN = liFldX;

    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "liFldX:[%d]", liFldX);

    return 0;
}

/*************************************************
*�������ơ� ClearCurrPartition
*�������ܡ�
*���������
*���������
*�������ء� 0  -- �ɹ�
            -1 -- ʧ��

***************************************************/
int ClearCurrPartition(char *sStlmDate, stPartitionInfDef *vfripPartInf)
{
    int     nReturnCode;
    int     iHandleFlag;
    int     i;
    long    count;
    char    sBuf[256 + 1];
    char    sKeepDate[8 + 1];
    char    *pStrRpl;

    iHandleFlag = 1;

    for( i = 0; i < vfripPartInf->nPartInfN; i++)
    {
        CommonRTrim(vfripPartInf->stPartInf[i].sTblName);
        CommonRTrim(vfripPartInf->stPartInf[i].sTblSpcName);

        /* �������� */
        if (pStrRpl = strstr(vfripPartInf->stPartInf[i].sTblSpcName, "$STTDATE"))
        {
            memcpy(pStrRpl, sStlmDate, 8);
        }

        /* �������� */
        if (pStrRpl = strstr(vfripPartInf->stPartInf[i].sTblSpcName, "$KEPDATE"))
        {
            memset(sKeepDate, 0x00, sizeof(sKeepDate));
            CountDate(sStlmDate, vfripPartInf->stPartInf[i].iIntervalDays * (-1), sKeepDate);
            memcpy(pStrRpl, sKeepDate, 8);
        }

        memset(sBuf, 0, sizeof(sBuf));

        /* ���� */
        if (1 == vfripPartInf->stPartInf[i].iOprFlag)
        {
            sprintf(sBuf, "delete from  %s where %s and rownum <= 5000",
                    vfripPartInf->stPartInf[i].sTblName, vfripPartInf->stPartInf[i].sTblSpcName);
        }
        else
        {
            continue;
        }

        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "����[%s]��...", vfripPartInf->stPartInf[i].sTblName);
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "sBuf[%s]", sBuf);

        count = 0;

        while(1)
        {
            nReturnCode = DbsRunSQL(sBuf);
            if( nReturnCode && nReturnCode != DBS_NOTFOUND)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "ExecuteSql: ִ��Sqlʧ��  nReturnCode:[%d], sBuf:[%s]", nReturnCode, sBuf);
                return -1;
            }
            else if( nReturnCode && nReturnCode == DBS_NOTFOUND)
            {
                HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "ִ��SQL����ɹ�.");
                break;
            }

            DbsCommit();

            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "deleting[%d]", count += 5000);
        }
    }

    return 0;
}

/****************************** end of Task_9029.c ******************************/

