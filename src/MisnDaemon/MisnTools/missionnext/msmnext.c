/*****************************************************************************
 *  Copyright 2012, Shanghai Huateng Software Systems Co., Ltd.
 *  All right reserved.
 *
 *  THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF SHANGHAI HUATENG
 *  SOFTWARE SYSTEMS CO., LTD.  THE CONTENTS OF THIS FILE MAY NOT
 *  BE DISCLOSED TO THIRD PARTIES, COPIED OR DUPLICATED IN ANY FORM,
 *  IN WHOLE OR IN PART, WITHOUT THE PRIOR WRITTEN PERMISSION OF
 *  SHANGHAI HUATENG SOFTWARE SYSTEMS CO., LTD.
 *
 *  文 件 名: msmnext.c
 *  功    能: 强制执行顺序任务下步，手工更新上一级全完成
 *  编程人员: XISTON
 *  开发时间: 2016-11-15
 *  备    注:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/

#include "batch.h"
//#include "Misn.h"
//#include "TMisn.h"

int 		ext_inter_brh_sta;
char    gLogFile[LOG_NAME_LEN_MAX];
char    ext_child_date[8 + 1];
char    ext_inter_brh_code[10 + 1];



int main(int argc, char *argv[])
{
    int  nReturnCode = -1;
    int  dFtno = 0;
    int  dSTATUS = 3;
    int  dLevel = -1;
    int  whSum = 0;
    
    char sLevel[9 + 1];
    char execUpSql[1024 + 1];
    char execDelSql[1024 + 1];
		char vtdate[8 + 1];
		char vtweek[1 + 1];
		char vthour[2 + 1];
		char vtminu[10 + 1];
		struct {
            char    inter_brh_code[10 + 1];
            char    stoday[8 + 1];
            char    syesterday[8 + 1];
            char    snextdate[8 + 1];
            char    sstartdate[8 + 1];
            char    sforestartdate[8 + 1];
       		 } sbtbl_date_inf;		
		
		memset(sLevel, 0x00, sizeof(sLevel));
    memset(execUpSql, 0x00, sizeof(execUpSql));
    memset(execDelSql, 0x00, sizeof(execDelSql));
		memset(&sbtbl_date_inf, 0x00, sizeof(&sbtbl_date_inf));
		memset(vtdate, 0x00, sizeof(vtdate));
		memset(vtweek, 0x00, sizeof(vtweek));
		memset(vthour, 0x00, sizeof(vthour));
		memset(vtminu, 0x00, sizeof(vtminu));
		
    //printf("UP missionnext begin... \n\n");
    
    // 检查传参及传数赋值
    if (argc < 2  || argc > 3)
    {
        //printf("Usage: %s MissionLevel\n", argv[0]);
        //printf("       %s MissionLevel InterBrhCode \n", argv[0]);
        //printf("       %s MissionLevel InterBrhCode TaskCode\n", argv[0]); return 0;
    }
    
    memcpy(sLevel, argv[1], sizeof(sLevel) - 1);
    dLevel = atoi(sLevel);
    
		// 连接数据库
    while(1)
    {
        nReturnCode = DbsConnect();
        if (nReturnCode)
        {
            DbsDisconnect();
            sleep(5);
            whSum++;
            if(whSum > 36)
            {
            	//printf("DbsDisconnect err");
            	return -1;
            }
            continue;
        }
        else
        {
            break;
        }
    }
    
   	// 获取各机构批次101000、101001清算日期
		nReturnCode = DbsTbldateInf(DBS_CURSOR, &sbtbl_date_inf);
		if(nReturnCode != 0)
		{
    	//printf("DbsTbldateInf DBS_CURSOR err [%d]\n\n", nReturnCode);
    	return -1;
    }
		nReturnCode = DbsTbldateInf(DBS_OPEN, &sbtbl_date_inf);
		if(nReturnCode != 0)
		{
    	//printf("DbsTbldateInf DBS_OPEN err [%d]\n\n", nReturnCode);
    	return -1;
    }
    
    while(1)
   	{
   		dFtno++;
   		memset(&sbtbl_date_inf, 0x00, sizeof(&sbtbl_date_inf));
			nReturnCode = DbsTbldateInf(DBS_FETCH, &sbtbl_date_inf);
			if(nReturnCode != 0)
			{
				if(1403 == nReturnCode)
				{
					break;
				}
    		//printf("DbsTbldateInf DBS_FETCH[%d] err [%d]\n\n", dFtno, nReturnCode);
    		return -1;
    	}
    	if(0 == dLevel)
    	{
    		// 顺序批量任务重新开始，删除批量子进程控制表TBL_CHILD_INF
    		sprintf(execDelSql, "delete from TBL_CHILD_INF where INTER_BRH_CODE = '%s' and SETTLMT_DATE = '%s' ", sbtbl_date_inf.inter_brh_code, sbtbl_date_inf.stoday);
				//printf("[%d]1execDelSql[%s] \n", dFtno, execDelSql);
    
    		nReturnCode = DbsRunSQL(execDelSql);
				//printf("exec [%d]1execDelSql[%d] \n\n", dFtno, nReturnCode);
    		if (nReturnCode != 0 && nReturnCode != 1403)
    		{
       	 DbsRollback();
       	 //printf("\n delete TBL_CHILD_TIME exec failed\n\n");
       	 DbsDisconnect();
       	 return -1;
    		}
    		
    		// 顺序批量任务重新开始，删除批量主控表TBL_MISSION_INF
    		sprintf(execDelSql, "delete from TBL_MISSION_INF where INTER_BRH_CODE = '%s' and SETTLMT_DATE = '%s' ", sbtbl_date_inf.inter_brh_code, sbtbl_date_inf.stoday);
				//printf("[%d]2execDelSql[%s] \n", dFtno, execDelSql);
    
    		nReturnCode = DbsRunSQL(execDelSql);
				//printf("exec [%d]2execDelSql[%d] \n\n", dFtno, nReturnCode);
    		if (nReturnCode != 0 && nReturnCode != 1403)
    		{
       	 DbsRollback();
       	 //printf("\n delete TBL_CHILD_TIME exec failed\n\n");
       	 DbsDisconnect();
       	 return -1;
    		}
    	}
    	// 只有2个参数的全更新层级为完成
    	if(2  == argc && 0 != dLevel)
    	{
    		// 更新批量主控表TBL_MISSION_INF状态为完成触发下一层任务
				sprintf(execUpSql, "update TBL_MISSION_INF set MISSION_STATUS=%d where INTER_BRH_CODE='%s' and SETTLMT_DATE='%s' and MISSION_LEVEL=%s ", 
				dSTATUS, sbtbl_date_inf.inter_brh_code, sbtbl_date_inf.stoday, sLevel);
				//printf("execUpSql[%s] \n", execUpSql);
			
	    	nReturnCode = DbsRunSQL(execUpSql);
				//printf("exec execUpSql[%d] \n\n", nReturnCode);
	    	if (nReturnCode != 0)
	    	{
	        DbsRollback();
	        //printf("\nupdate TBL_MISSION_TIME exec failed[%d],\n\nplease sure TaskCode right,\nand try Usage:%s TaskCode InterBrhCode, \nor try other Usage \n\n[uptktm fail!!]\n", nReturnCode, argv[0]);
	        DbsDisconnect();
	        return -1;
	    	}
    	}
    	//3个参数只更新传进来的机构批次的
    	if(0 != dLevel && 3 == argc && !memcmp(sbtbl_date_inf.inter_brh_code, argv[2], 6))
    	{
    		// 更新批量主控表TBL_MISSION_INF状态为完成触发下一层任务
				sprintf(execUpSql, "update TBL_MISSION_INF set MISSION_STATUS=%d where INTER_BRH_CODE='%s' and SETTLMT_DATE='%s' and MISSION_LEVEL=%s ", 
				dSTATUS, sbtbl_date_inf.inter_brh_code, sbtbl_date_inf.stoday, sLevel);
				//printf("execUpSql[%s] \n", execUpSql);
			
	    	nReturnCode = DbsRunSQL(execUpSql);
				//printf("exec execUpSql[%d] \n\n", nReturnCode);
	    	if (nReturnCode != 0)
	    	{
	        DbsRollback();
	        //printf("\nupdate TBL_MISSION_TIME exec failed[%d],\n\nplease sure TaskCode right,\nand try Usage:%s TaskCode InterBrhCode, \nor try other Usage \n\n[uptktm fail!!]\n", nReturnCode, argv[0]);
	        DbsDisconnect();
	        return -1;
	    	}
    	}
    	if(0 != dLevel && 4 == argc && !memcmp(sbtbl_date_inf.inter_brh_code, argv[2], 6))
    	{
    		// 删除批量子进程控制表TBL_CHILD_INF相关记录
				sprintf(execDelSql, "delete TBL_CHILD_INF where INTER_BRH_CODE='%s' and SETTLMT_DATE='%s' and MISSION_INDEX='%s' ", 
				sbtbl_date_inf.inter_brh_code, sbtbl_date_inf.stoday, argv[3]);
				//printf("execDelSql[%s] \n", execDelSql);
			
	    	nReturnCode = DbsRunSQL(execDelSql);
				//printf("exec execDelSql[%d] \n\n", nReturnCode);
	    	if (nReturnCode != 0 && nReturnCode != 1403)
	    	{
	        DbsRollback();
	        //printf("\ndelete TBL_MISSION_INF exec failed[%d],\n\nplease sure TaskCode right,\nand try Usage:%s TaskCode InterBrhCode, \nor try other Usage \n\n[uptktm fail!!]\n", nReturnCode, argv[0]);
	        DbsDisconnect();
	        return -1;
	    	}
	    	
    		// 更新批量主控表TBL_MISSION_INF状态为完成触发下一层任务
				sprintf(execUpSql, "update TBL_MISSION_INF set MISSION_STATUS=0 where INTER_BRH_CODE='%s' and SETTLMT_DATE='%s' and MISSION_INDEX = '%s'", 
				sbtbl_date_inf.inter_brh_code, sbtbl_date_inf.stoday, argv[3]);
				//printf("execUpSql[%s] \n", execUpSql);
			
	    	nReturnCode = DbsRunSQL(execUpSql);
				//printf("exec execUpSql[%d] \n\n", nReturnCode);
	    	if (nReturnCode != 0)
	    	{
	        DbsRollback();
	        //printf("\nupdate TBL_MISSION_TIME exec failed[%d],\n\nplease sure TaskCode right,\nand try Usage:%s TaskCode InterBrhCode, \nor try other Usage \n\n[uptktm fail!!]\n", nReturnCode, argv[0]);
	        DbsDisconnect();
	        return -1;
	    	}
    	}
   	}
   	
   	if(0 == dLevel)
   	{
	   	// 通过数据库获取当前系统时间
			nReturnCode = DbsTimeStamp(DBS_SELECT, vtdate, vtweek, vthour, vtminu);
	    //printf("DbsTimeStamp DBS_SELECT [%d]\n\n", nReturnCode);
	    if (nReturnCode != 0)
	    {
	        DbsRollback();
	        //printf("\nDbsTimeStamp exec failed");
	        DbsDisconnect();
	        return -1;
	    }
	    
	    // 更新定时子进程控制表TBL_CHILD_TIME当天所有进程为完成以触发顺序任务执行
			sprintf(execUpSql, "update TBL_CHILD_TIME set CHILD_STATUS=%d where MISSION_DATE='%s' ", 
			dSTATUS, vtdate);
			//printf("execUpSql[%s] \n", execUpSql);
			
	    nReturnCode = DbsRunSQL(execUpSql);
			//printf("exec execUpSql[%d] \n\n", nReturnCode);
	    if (nReturnCode != 0)
	    {
	        DbsRollback();
	        //printf("\nupdate TBL_MISSION_TIME exec failed[%d],\n\nplease sure TaskCode right,\nand try Usage:%s TaskCode InterBrhCode, \nor try other Usage \n\n[uptktm fail!!]\n", nReturnCode, argv[0]);
	        DbsDisconnect();
	        return -1;
	    }
    }
	  DbsCommit();
    DbsDisconnect();
    if(0 == dLevel)
    {
    	// 当天重新执行顺序任务需要重启服务--修正，不需要
    	//system("restartbatch");
    }
    //printf("\n msmnext succ \n\n");
		
    return 0;
}


/************************************** end of msmnext.c ********************************************/
