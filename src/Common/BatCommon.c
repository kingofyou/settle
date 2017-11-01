#include <stdio.h>
#include <string.h>
#include "DbsTbl.h"
#include "Common.h"
#include "HtLog.h"

extern char	gLogFile[LOG_NAME_LEN_MAX];

/*�ⲿ֧��ͨ��ת��Ϊ�ڲ������֧��ͨ��*/
/*����:pOut �ⲿ֧��ͨ�� 
  ���:pIn  �ڲ�֧��ͨ��
*/
int ChnOut2In(char *pOut, char *pIn)
{
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "out[%s].in[%s]", pOut, pIn);
    if(strcmp(pOut, BT_CHN_CIB_ALIPAY) == 0) 
    {
        /*��ҵ-֧����*/
        strcpy(pIn, BT_BUS_TP_ALIPAY);
    }  
    else if(strcmp(pOut, BT_CHN_CIB_TENPAY) == 0) 
    {
        /*��ҵ-΢��*/
        strcpy(pIn, BT_BUS_TP_WECHAT);
    } 
    else if(strcmp(pOut, BT_CHN_CIB_QUICKPAY) == 0) 
    {
        /*��ҵ-���֧��*/
        strcpy(pIn, BT_BUS_TP_CIB5);
    } 
    else if(strcmp(pOut, BT_CHN_CIB_GATEWAY) == 0) 
    {
        /*��ҵ-����֧��*/
        strcpy(pIn, BT_BUS_TP_CIB6);
    } 
    else if(!strcmp(pOut, BT_CHN_CIB_XMYF))
    {
        /*����-�����׸�*/
        strcpy(pIn, BT_BUS_TP_CORE);
    }   
	else if(!strcmp(pOut, BT_CHN_FY_ALIPAY))
    {
        /*����-֧����֧��*/
        strcpy(pIn, BT_BUS_FY_ALIPAY);
    } 
	else if(!strcmp(pOut, BT_CHN_FY_TENPAY))
    {
        /*����-΢��֧��*/
        strcpy(pIn, BT_BUS_FY_WECHAT);
    } 
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "out[%s].in[%s]", pOut, pIn);
    return 0;
}

/*�ⲿ֧��ͨ��ת��Ϊ�ڲ������֧��ͨ��*/
/*����:pOut �ⲿ֧��ͨ�� 
  ���:pIn  �ڲ�֧��ͨ��
*/
int ChnIn2Out(char *pOut, char*pIn)
{
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "out[%s].in[%s]", pOut, pIn);
    if(memcmp(pIn, BT_BUS_TP_ALIPAY, BT_BUS_TP_LEN) == 0) 
    {
        /*��ҵ-֧����*/
        strcpy(pOut, BT_CHN_CIB_ALIPAY);
    }  
    else if(memcmp(pIn, BT_BUS_TP_WECHAT, BT_BUS_TP_LEN) == 0) 
    {
        /*��ҵ-΢��*/
        strcpy(pOut, BT_CHN_CIB_TENPAY);
    } 
    else if(memcmp(pIn, BT_BUS_TP_CIB5, BT_BUS_TP_LEN) == 0) 
    {
        /*��ҵ-���֧��*/
        strcpy(pOut, BT_CHN_CIB_QUICKPAY);
    } 
    else if(memcmp(pIn, BT_BUS_TP_CIB6, BT_BUS_TP_LEN) == 0) 
    {
        /*��ҵ-����֧��*/
        strcpy(pOut, BT_CHN_CIB_GATEWAY);
    } 
    else 
    {
        /*����-�����׸�*/
        strcpy(pOut, BT_CHN_CIB_XMYF);
    }   
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "out[%s].in[%s]", pOut, pIn);
    return 0;
}
