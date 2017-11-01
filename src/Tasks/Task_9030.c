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
 *  �� �� ��: Task_9030.c
 *  ��    �ܣ��ؽ����ݿ�������
 *  �����Ա:
 *  ����ʱ��:
 *  ��    ע:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include "batch.h"
#include "DbsTbl.h"

extern  char    			gLogFile[LOG_NAME_LEN_MAX];
extern	int		ext_inter_brh_sta;
tbl_date_inf_def 	dbtbl_date_inf;

static int OprInfLoad(stOperationInfDef * , int);
static int ReBuildSequence_Task( char *inter_brh_9031) ;
static int ReBuildSequence_XXXX_9030(stOperationInfDef *vfripOprInf , char *inter_brh_9032 ) ;
static int ReBuildSequence_MMDD_9030(stOperationInfDef *vfripOprInf) ;

/*************************************************
*�������ơ� Total_9030
*�������ܡ�
*���������
*���������
*�������ء� 1

***************************************************/
int Total_9030()
{
    return 1;
}

/*************************************************
*�������ơ� Task_9030
*�������ܡ��ؽ����ݿ�������
*��������� nBeginOffset��nEndOffset
*���������
*�������ء� 0  -- �ɹ�
            -1 -- ʧ��

***************************************************/
int Task_9030 ( int nBeginOffset, int nEndOffset)
{
    int     nReturnCode ;
    int     nReturnCodeCode ;
    char    inter_brh_9030[10 + 1];
    tbl_CstBrhCupInf_def CstBrhCupInf;

    memset(&CstBrhCupInf, 0x00, sizeof(CstBrhCupInf));

    DbsInstMisn(DBS_CURSOR, &CstBrhCupInf);
    nReturnCodeCode = DbsInstMisn(DBS_OPEN, &CstBrhCupInf);
    if(nReturnCodeCode)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "OPEN tbl_brh_info_cur error!sqlcode[%d]",  nReturnCodeCode);
        return -1;
    }

    while(1)
    {
        nReturnCodeCode = DbsInstMisn(DBS_FETCH, &CstBrhCupInf);
        if(nReturnCodeCode == DBS_NOTFOUND)
        {
            break;
        }
        else if (nReturnCodeCode)
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fetch tbl_brh_info_cur error! sqlcode[%d]", nReturnCodeCode);
            DbsInstMisn(DBS_CLOSE, &CstBrhCupInf);
            return -1;
        }

        memset(inter_brh_9030 , 0 , sizeof(inter_brh_9030) );
        memcpy(inter_brh_9030 , CstBrhCupInf.brh_id, 6);

        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "ReBuildSequence_Task[%s]", inter_brh_9030);
        nReturnCode = ReBuildSequence_Task(inter_brh_9030 );
        if( nReturnCode )
        {
            return -1 ;
        }
    }

    DbsInstMisn(DBS_CLOSE, &CstBrhCupInf);

    return 0 ;
}

static int ReBuildSequence_Task( char *inter_brh_9031)
{
    int     nReturnCode;
    int     iHandleFlag;
    char    settle_date[8 + 1];
    stOperationInfDef    stOprInf;

    memset(&stOprInf, 0x00, sizeof(stOprInf));

    iHandleFlag = 3;

    nReturnCode = OprInfLoad(&stOprInf, iHandleFlag);
    if(nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "OprInfLoad:Err:[%d].", nReturnCode);
        return -1;
    }

    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Begin to ReBuildSequence ...");

    nReturnCode = ReBuildSequence_XXXX_9030(&stOprInf, inter_brh_9031);
    if(nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "ReBuildSequence:Err:[%d].", nReturnCode);
        return -1;
    }


    if( memcmp(inter_brh_9031, "0000", 4) == 0)
    {
        memset(&stOprInf, 0, sizeof(stOperationInfDef));

        iHandleFlag = 4;

        nReturnCode = OprInfLoad(&stOprInf, iHandleFlag);
        if(nReturnCode)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "OprInfLoad:Err:[%d].", nReturnCode);
            return -1;
        }

        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "�ؽ�������ˮ�ļ�sequence");

        nReturnCode = ReBuildSequence_MMDD_9030(&stOprInf);
        if(nReturnCode)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "ReBuildSequence:Err:[%d].", nReturnCode);
            return -1;
        }

    }

    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_9030 Succ.");

    return 0;
}

/*************************************************
*�������ơ� OprInfLoad
*�������ܡ� װ�ر� tbl_opr_ctl �е����ݵ��ṹ��
*���������iHandleFlag
*���������vfripOprInf
*�������ء� 0  -- �ɹ�
            -1 -- ʧ��

***************************************************/
static int OprInfLoad( stOperationInfDef *vfripOprInf, int iHandleFlag)
{
    int          liFldX;
    int          nReturnCode;
    stOprInfDef vtpOprInf;

    HtLog( gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Start to load table ht_tbl_opr_ctl's data ...");

    memset( &vtpOprInf, 0, sizeof(vtpOprInf));
    vtpOprInf.sTblOprFlag = iHandleFlag;

    liFldX = 0;

    /* װ�����ؽ��ұ�־Ϊ3������ */
    DbsOprInf( DBS_CURSOR, &vtpOprInf);
    nReturnCode = DbsOprInf( DBS_OPEN, &vtpOprInf);
    if(nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsOprInf DBS_OPEN err:[%d]", nReturnCode);
        return -1;
    }

    while(1)
    {
        memset( &vtpOprInf, 0, sizeof(vtpOprInf));

        nReturnCode = DbsOprInf( DBS_FETCH, &vtpOprInf);
        if (nReturnCode != 0 && nReturnCode != DBS_NOTFOUND && nReturnCode != DBS_FETCHNULL)
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsOprInf DBS_FETCH err:[%d]", nReturnCode);

            DbsOprInf( DBS_CLOSE, &vtpOprInf);
            return -1;
        }
        else if (nReturnCode == DBS_NOTFOUND)
        {
            break;
        }

        memcpy(vfripOprInf->stOprInf[liFldX].sObjectName, vtpOprInf.sObjectName, 60);

        liFldX++;
    } /* end of while */

    DbsOprInf( DBS_CLOSE, &vtpOprInf);


    vfripOprInf->nOprInfN = liFldX;

    return 0;
} /* end of OprInfLoad */

/*************************************************
*�������ơ� ReBuildSequence_XXXX
*�������ܡ� �ؽ�����
*��������� vfripOprInf
*���������
*�������ء� 0  -- �ɹ�
            -1 -- ʧ��
***************************************************/
static int ReBuildSequence_XXXX_9030(stOperationInfDef *vfripOprInf , char *inter_brh_9032 )
{
    int     nReturnCode;
    int     i;
    char    sBuf[256 + 1];

    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "---- Sequence Num [%d] ----", vfripOprInf->nOprInfN);

    for( i = 0; i < vfripOprInf->nOprInfN; i++)
    {
        CommonRTrim(vfripOprInf->stOprInf[i].sObjectName);

        /* ɾ������ */
        memset(sBuf, 0, sizeof(sBuf));

        sprintf(sBuf, "DROP SEQUENCE %s_%s ", vfripOprInf->stOprInf[i].sObjectName, inter_brh_9032);

        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "-------------[%d]----------------", i);
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Drop sequence sBuf:[%s]", sBuf);

        nReturnCode = DbsRunSQL(sBuf);
        if( nReturnCode != 0 && nReturnCode != -2289 )
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsRunSQL: ִ��Sqlʧ��  nReturnCode:[%d], sBuf:[%s]", nReturnCode, sBuf);
            return -1;
        }


        /* �ؽ����� */
        memset(sBuf, 0, sizeof(sBuf));

        sprintf(sBuf, "create SEQUENCE %s_%s INCREMENT BY 1   START WITH 1   MAXvalue 999999999  NOCYCLE  CACHE 1000 ", vfripOprInf->stOprInf[i].sObjectName, inter_brh_9032);

        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Create sequence sBuf:[%s]", sBuf);

        nReturnCode = DbsRunSQL(sBuf);
        if( nReturnCode != 0 )
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsRunSQL: ִ��Sqlʧ��  nReturnCode:[%d], sBuf:[%s]", nReturnCode, sBuf);
            return -1;
        }

        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "ִ��Sql�ɹ� sBuf:[%s]", sBuf);
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "---------------------------------\n");
    }

    return 0;
}

/*************************************************
*�������ơ� ReBuildSequence_MMDD
*�������ܡ� �ؽ�����
*��������� vfripOprInf
*���������
*�������ء� 0  -- �ɹ�
            -1 -- ʧ��
***************************************************/
static int ReBuildSequence_MMDD_9030(stOperationInfDef *vfripOprInf)
{
    int     nReturnCode;
    int     i;
    char    sBuf[256 + 1];
    char    sDateMMDD[4 + 1];

    memset(sDateMMDD, 0, 5);
    memcpy(sDateMMDD, dbtbl_date_inf.stoday + 4, 4);


    for( i = 0; i < vfripOprInf->nOprInfN; i++)
    {
        CommonRTrim(vfripOprInf->stOprInf[i].sObjectName);

        /* ɾ������ */
        memset(sBuf, 0, sizeof(sBuf));

        sprintf(sBuf, "DROP SEQUENCE %s_%s ", vfripOprInf->stOprInf[i].sObjectName, sDateMMDD);

        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Drop SEQUENCE sBuf:[%s]", sBuf);

        nReturnCode = DbsRunSQL(sBuf);
        if( nReturnCode != 0 && nReturnCode != -2289 )
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsRunSQL: ִ��Sqlʧ��  nReturnCode:[%d], sBuf:[%s]", nReturnCode, sBuf);
            return -1;
        }


        /* �ؽ����� */
        memset(sBuf, 0, sizeof(sBuf));

        sprintf(sBuf, "create SEQUENCE %s_%s INCREMENT BY 1   START WITH 1   MAXvalue 999999999  NOCYCLE  CACHE 1000", vfripOprInf->stOprInf[i].sObjectName, sDateMMDD);

        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "CREATE SEQUENCE sBuf:[%s]", sBuf);

        nReturnCode = DbsRunSQL(sBuf);
        if( nReturnCode != 0 )
        {
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__,
                   "DbsRunSQL: ִ��Sqlʧ��  nReturnCode:[%d], sBuf:[%s]", nReturnCode, sBuf);
            return -1;
        }

        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsRunSQL: ִ��Sql�ɹ�  nReturnCode:[%d], sBuf:[%s]\n", nReturnCode, sBuf);
    }

    return 0;
}
/****************************** end of Task_9030.c ******************************/
