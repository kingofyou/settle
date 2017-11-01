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
 *  文 件 名: Task_6800.c
 *  功    能: 确认结算预处理完成
 *  编程人员: bcw
 *  开发时间: 20170328
 *  备    注:
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
*函数名称∶ Wait_6601
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶
***************************************************/
int Wait_6601()
{
    int nReturnCode;
    char    stlm_dt[8 + 1];
    
    tbl_child_inf_def        dbtbl_child_inf;

    /* 清算日期 */
    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, dbtbl_date_inf.stoday, 8);
    
    while(1)
    {
        memset(&dbtbl_child_inf, 0x00, sizeof(dbtbl_child_inf));
        memcpy(dbtbl_child_inf.inter_brh_code, "200001", BT_BRH_ID_LEN);
        memcpy(dbtbl_child_inf.settlmt_date, stlm_dt, BT_DATE_LEN);
        memcpy(dbtbl_child_inf.mission_index, "6603", BT_MISSION_INDEX_LEN);
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
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "inter_brh_code[%s]settlmt_date[%s],task6601 excuting, please hold 30's.", dbtbl_child_inf.inter_brh_code,dbtbl_child_inf.settlmt_date);
            sleep(2);
        }
    }

//    while(1)
//    {
//        memset(&dbtbl_child_inf, 0x00, sizeof(dbtbl_child_inf));
//        memcpy(dbtbl_child_inf.inter_brh_code, "200004", BT_BRH_ID_LEN);
//        memcpy(dbtbl_child_inf.settlmt_date, stlm_dt, BT_DATE_LEN);
//        memcpy(dbtbl_child_inf.mission_index, "6602", BT_MISSION_INDEX_LEN);
//        nReturnCode = DbsTblchildInf(DBS_SELECT1, &dbtbl_child_inf);
//        if (nReturnCode != 0 && nReturnCode != DBS_NOTFOUND)
//        {
//            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "select task status error[%d]", nReturnCode);
//            return -1;
//        }
//
//        if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_SUSS)
//        {
//            break;
//        }
//        else if (dbtbl_child_inf.child_status == BT_MISSION_STATUS_FAIL)
//        {
//            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "task excuted error, please check.");
//            return -1;
//        }
//        else
//        {
//            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "inter_brh_code[%s]settlmt_date[%s],task6601 excuting, please hold 30's.", dbtbl_child_inf.inter_brh_code,dbtbl_child_inf.settlmt_date);
//            sleep(2);
//        }
//    } 
    
       
    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Check inter_brh_code[%s] Task_6601 Success.",dbtbl_child_inf.inter_brh_code);
    return 0;
}
/*************************************************
*函数名称∶ Total_6800
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_6800()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_6800
*函数功能∶ 交换流水文件检测
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_6800 ( int nBeginOffset, int nEndOffset )
{
    int	nReturnCode;
    /* 等待任务完成 */
    nReturnCode = Wait_6601();
    if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Wait_6601 err.");
        return -1;
    }

    HtLog (gLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "Check[%s] Task_6601 Success.",dbtbl_date_inf.stoday);
    HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "Task_6800 Succ .");
    return 0;
}

/****************************** end of Task_6800.c ******************************/

