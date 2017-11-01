#include <stdio.h>
#include <string.h>
#include "DbsTbl.h"
#include "Common.h"
#include "HtLog.h"

extern char	gLogFile[LOG_NAME_LEN_MAX];

/*外部支付通道转换为内部定义的支付通道*/
/*输入:pOut 外部支付通道 
  输出:pIn  内部支付通道
*/
int ChnOut2In(char *pOut, char *pIn)
{
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "out[%s].in[%s]", pOut, pIn);
    if(strcmp(pOut, BT_CHN_CIB_ALIPAY) == 0) 
    {
        /*兴业-支付宝*/
        strcpy(pIn, BT_BUS_TP_ALIPAY);
    }  
    else if(strcmp(pOut, BT_CHN_CIB_TENPAY) == 0) 
    {
        /*兴业-微信*/
        strcpy(pIn, BT_BUS_TP_WECHAT);
    } 
    else if(strcmp(pOut, BT_CHN_CIB_QUICKPAY) == 0) 
    {
        /*兴业-快捷支付*/
        strcpy(pIn, BT_BUS_TP_CIB5);
    } 
    else if(strcmp(pOut, BT_CHN_CIB_GATEWAY) == 0) 
    {
        /*兴业-网关支付*/
        strcpy(pIn, BT_BUS_TP_CIB6);
    } 
    else if(!strcmp(pOut, BT_CHN_CIB_XMYF))
    {
        /*核心-星民易付*/
        strcpy(pIn, BT_BUS_TP_CORE);
    }   
	else if(!strcmp(pOut, BT_CHN_FY_ALIPAY))
    {
        /*富友-支付宝支付*/
        strcpy(pIn, BT_BUS_FY_ALIPAY);
    } 
	else if(!strcmp(pOut, BT_CHN_FY_TENPAY))
    {
        /*富友-微信支付*/
        strcpy(pIn, BT_BUS_FY_WECHAT);
    } 
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "out[%s].in[%s]", pOut, pIn);
    return 0;
}

/*外部支付通道转换为内部定义的支付通道*/
/*输入:pOut 外部支付通道 
  输出:pIn  内部支付通道
*/
int ChnIn2Out(char *pOut, char*pIn)
{
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "out[%s].in[%s]", pOut, pIn);
    if(memcmp(pIn, BT_BUS_TP_ALIPAY, BT_BUS_TP_LEN) == 0) 
    {
        /*兴业-支付宝*/
        strcpy(pOut, BT_CHN_CIB_ALIPAY);
    }  
    else if(memcmp(pIn, BT_BUS_TP_WECHAT, BT_BUS_TP_LEN) == 0) 
    {
        /*兴业-微信*/
        strcpy(pOut, BT_CHN_CIB_TENPAY);
    } 
    else if(memcmp(pIn, BT_BUS_TP_CIB5, BT_BUS_TP_LEN) == 0) 
    {
        /*兴业-快捷支付*/
        strcpy(pOut, BT_CHN_CIB_QUICKPAY);
    } 
    else if(memcmp(pIn, BT_BUS_TP_CIB6, BT_BUS_TP_LEN) == 0) 
    {
        /*兴业-网关支付*/
        strcpy(pOut, BT_CHN_CIB_GATEWAY);
    } 
    else 
    {
        /*核心-星民易付*/
        strcpy(pOut, BT_CHN_CIB_XMYF);
    }   
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "out[%s].in[%s]", pOut, pIn);
    return 0;
}
