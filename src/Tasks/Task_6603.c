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
 *  �� �� ��: Task_6603.c
 *  ��    ��: ���Ѵ���
 *  �����Ա: 
 *  ����ʱ��: 2017-03-09
 *  ��    ע:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include <sys/socket.h>
#include "batch.h"
#include "Common.h"
#include "EPay.h"

extern  char    gLogFile[LOG_NAME_LEN_MAX];
extern  char    ext_child_date[8 + 1];
extern  char    ext_inter_brh_code[10 + 1];
extern  int     ext_inter_brh_sta;
extern          tbl_date_inf_def dbtbl_date_inf;

/*************************************************
*�������ơ� Total_6603
*�������ܡ�
*���������
*���������
*�������ء� 1
***************************************************/
int Total_6603()
{
    return 1;
}

/*************************************************
*�������ơ� Task_6603
*�������ܡ� ѭ�����ʴ���
*��������� nBeginOffset��nEndOffset
*���������
*�������ء� 0  -- �ɹ�
            -1 -- ʧ��
***************************************************/
int Task_6603 ( int nBeginOffset, int nEndOffset )
{
    int     nReturnCode;
    char    sDateTime[14+1];
    char    stlm_dt[8+1];
    char    sBbankCode[30+1];
    int		nBankCount = 0; 
    int		nTotalCount = 0;

   
    tbl_sett_list_def   dbtbl_sett_list;
    ht_tbl_com_config_def comconfig;
    
     
    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
     
    
//    memset(sBbankCode, 0, sizeof(sBbankCode));
//    memset(&comconfig, 0x00, sizeof(comconfig));
//    strcpy(comconfig.key1,"200004");
//    strcpy(comconfig.key2,"6603");
//    strcpy(comconfig.key3,"BANKCODE");
//    nReturnCode = DbsbthComConfig (DBS_SELECT, &comconfig);
//    if( nReturnCode )
//    {
//        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsbthComConfig DBS_SELECT[%d]", nReturnCode);
//        return nReturnCode;
//    }
//    CommonRTrim(comconfig.value1); 
//    memcpy(sBbankCode, comconfig.value1,sizeof(sBbankCode)-1);
                
	nReturnCode = DbsTblSettList(DBS_UPDATE3, dbtbl_sett_list);
	if (nReturnCode && nReturnCode != DBS_NOTFOUND)
	{
		HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_UPD_SEQ ht_tbl_sett_list error [%d].", nReturnCode);
		return -1;
	}        
   
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_6603 Succ .");
                    
    return 0;
}



/****************************** end of Task_6603.c ******************************/


