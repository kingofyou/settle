/*****************************************************************************
 *
 *  文 件 名: Task_9003.c
 *  功    能: 获取富友支付宝对账文件
 *  编程人员: 
 *  开发时间: 20170912
 *  备    注:
******************************************************************************/

/*****************************************************************************
 * DATE        PROGRAMMER     MODIFY DESCRIPTION

*****************************************************************************/
#include <sys/socket.h>
#include "batch.h"
#include "Common.h"
#include "cJSON.h"
#include "EPay.h"

extern  char    gLogFile[LOG_NAME_LEN_MAX];
extern  char    ext_child_date[8 + 1];
extern  char    ext_inter_brh_code[10 + 1];
extern  int     ext_inter_brh_sta;

#define REDO_NUM		            100


/*************************************************
*函数名称∶ Total_9007
*函数功能∶
*输入参数∶
*输出参数∶
*函数返回∶ 1
***************************************************/
int Total_9007()
{
    return 1;
}

/*************************************************
*函数名称∶ Task_9007
*函数功能∶ 获取富友支付宝对账文件
*输入参数∶ nBeginOffset，nEndOffset
*输出参数∶
*函数返回∶ 0  -- 成功
            -1 -- 失败
***************************************************/
int Task_9007 ( int nBeginOffset, int nEndOffset )
{
    int     nReturnCode;
    char    sDateTime[14+1];
    char    stlm_dt[8 + 1];
    char    sysDate[9];
    char    sysTime[14+1];
	char    fileName[64];
	char    sCmd[128];
	char    host_file_name[128];
	char    homePath[32];

    char    *pstr;
    tbl_key_cert_def KeyInf;
	stExtnFileRegDef StExtnFileReg;
  
	int flag = 0;

    memset(stlm_dt, 0x00, sizeof(stlm_dt));
    memcpy(stlm_dt, ext_child_date , 8);
 
    memset(&KeyInf, 0x00, sizeof(KeyInf));
	strcpy(KeyInf.pay_way, "fuiou");
	strcpy(KeyInf.merc_type, "1");
	nReturnCode = DbsKeyCertInf(DBS_CURSOR, &KeyInf);
	if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsKeyCertInf cursor err. nReturnCode = [%d].", nReturnCode);
        return -1;
    }
    nReturnCode = DbsKeyCertInf(DBS_OPEN, &KeyInf);
	if (nReturnCode)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsKeyCertInf open err. nReturnCode = [%d].", nReturnCode);
        return -1;
    }
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
        CommonRTrim(KeyInf.key);   
	    CommonRTrim(KeyInf.merc_cert); 
		
        memset(fileName, 0, sizeof(fileName));
		memset(sCmd, 0, sizeof(sCmd));
		memset(host_file_name, 0, sizeof(host_file_name));
       
        sprintf(fileName, "%s_YYYYMMDD_TXN.txt", KeyInf.paying_merc_code);
        pstr = strstr (fileName, "YYYYMMDD");
        if (pstr)
            memcpy (pstr, stlm_dt, 8);

		memset(&StExtnFileReg, 0x00, sizeof(StExtnFileReg));
        memcpy(StExtnFileReg.inter_brh_code, ext_inter_brh_code, 6);
        memcpy(StExtnFileReg.stlm_dt, stlm_dt, 8);
        //memcpy(StExtnFileReg.host_name, "ALPAY", 5);
        sprintf(StExtnFileReg.src_file_name, "%s_YYYYMMDD_TXN.txt", KeyInf.paying_merc_code);

        pstr = strstr (StExtnFileReg.src_file_name, "YYYYMMDD");
        if (pstr)
            memcpy (pstr, stlm_dt, 8);
		getShareFileFullName(&StExtnFileReg, host_file_name);

	    memset(homePath, 0, sizeof(homePath));
        strcpy(homePath, getenv("HOME"));
	    sprintf(sCmd, "cd %s/batch/sbin; ./downFyfile.sh %s", homePath, fileName);    
        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "sCmd[%s].", sCmd);
        nReturnCode = cmdSystem(sCmd);
        if(nReturnCode)
        {
            HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "system error:nReturnCode[%s].", nReturnCode);
            return -1;
        }   
        else
        {
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, " system success:[%d]", nReturnCode); 
            /* 检查文件是否已下载 */
             nReturnCode = access(host_file_name, R_OK);
            if(nReturnCode)
            {
                HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "下载流水处理结果文件失败!filepath: [%s].", host_file_name);
                flag = 1;
            }          
        } 
	}
    DbsKeyCertInf(DBS_CLOSE, &KeyInf);
	if(flag) return -1;
    return 0;
}

/****************************** end of Task_9003.c ******************************/


