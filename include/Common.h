#ifndef __COMMON_H
#define __COMMON_H

#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <fcntl.h>


#define F002_VAL_LEN            19
#define BUF_SIZE                1800

/*****************************************************************************/
/* FUNC:   void CommonGetCurrentDate (char *sCurrentDate);                   */
/* INPUT:  <none>                                                            */
/* OUTPUT: sCurrentDate   -- the string of current date                      */
/* RETURN: <none>                                                            */
/* DESC:   Get the system date with the format (YYYYMMDD).                   */
/*         NULL is added at the end.                                         */
/*****************************************************************************/
void  CommonGetCurrentDate(char *sCurrentDate);

/*****************************************************************************/
/* FUNC:   void CommonGetCurrentTime (char *sCurrentTime);                   */
/* INPUT:  <none>                                                            */
/* OUTPUT: sCurrentTime   -- the string of current time                      */
/* RETURN: <none>                                                            */
/* DESC:   Get the system time with the format (YYYYMMDDhhmmss).             */
/*         NULL is added at the end.                                         */
/*****************************************************************************/
void CommonGetCurrentTime(char *sCurrentTime);
int sendMobileMsg(char* sms_content);

char                gsLogFile[LOG_NAME_LEN_MAX];

#define BT_FILE_PATH_LEN  256
#define BT_FILE_NAME_LEN  64
#define BT_ITEM_VALUE_LEN 64
#define BT_MAXLINE        1024
#define BT_MINLINE_LEN    10
#define BT_CIB_SUM_LEN    50


#define BT_CRET_XM        "0"
#define BT_DEBT_XM        "1"

#define BT_DATE_LEN                 8           //日期字段长度
#define BT_TIME_LEN                 6           //时间字段长度
#define BT_TRANS_TP_LEN             4           //勾对标志字段长度
#define BT_BUS_TP_HALF_LEN          4           //业务类型-4字节
#define BT_BRH_ID_LEN               6           //内部机构号字段长度
#define BT_MISSION_INDEX_LEN        4           //任务索引号字段长度

#define BT_FLAG_RESULT_LEN          1       
#define BT_FLAG_RESULT_NULL         "-"         //未对账
#define BT_FLAG_RESULT_CMP          "1"         //对平
#define BT_FLAG_RESULT_NOTMATC      "2"         //要素不平
#define BT_FLAG_RESULT_GATEWAYNULL      "3"     //支付网关无
#define BT_FLAG_RESULT_GATEWAY      "4"         //支付网关多
#define BT_FLAG_RESULT_CORENULL     "5"         //虚拟核心无
#define BT_FLAG_RESULT_CORE         "6"         //虚拟核心多
#define BT_FLAG_RESULT_PART_NULL      "7"       //第三方无
#define BT_FLAG_RESULT_PART      "8"            //第三方多
#define BT_FLAG_RESULT_COREFAIL      "9"        //核心失败/冲正/撤销
#define BT_FLAG_RESULT_GATEWAYFAIL      "A"        //支付网关失败/冲正/撤销
#define BT_FLAG_RESULT_PART_ORLESS    "B"             //第三方疑似少
#define BT_FLAG_RESULT_PART_NOTORLESS    "C"             //T+2确定第三方不少
#define BT_FLAG_RESULT_CORE_ORMORE    "D"             //虚拟核心疑似多 待T+2确定是核心多还是网关少

#define BT_FLAG_RESULT_REFUND      "T"          //当天退款(撤单处理)

#define BT_TRANS_STATE_LEN        1            //交易状态字段长度
#define BT_TRANS_STATE_SUCC      "1"            //交易状态-成功
#define BT_TRANS_STATE_FAIL      "0"            //交易状态-失败

#define BT_REVSAL_FLAG_LEN        1            //交易状态字段长度
#define BT_REVSAL_FLAG_Y      "1"            //冲正标志-被冲正
#define BT_REVSAL_FLAG_N      "0"            //冲正标志-未被冲正

#define BT_CANCEL_FLAG_LEN        1            //交易状态字段长度
#define BT_CANCEL_FLAG_Y      "1"            //冲正标志-被冲正
#define BT_CANCEL_FLAG_N      "0"            //冲正标志-未被冲正

/*交易类型*/
#define BT_TRANS_TP_LEN         4               //长度
#define BT_TRANS_TP_CHARGE         "1201"      //充值交易
#define BT_TRANS_TP_WITHDRAW       "1301"      //提现交易
#define BT_TRANS_TP_REFUND         "5001"      //退款交易

#define BT_TRANS_TP_QUICK_IN       "0001"      //快捷支付入账交易
#define BT_TRANS_TP_QUICK_OUT      "1001"      //快捷支付出账交易
#define BT_TRANS_TP_GATEWAY_IN       "0002"      //网银支付入账交易
#define BT_TRANS_TP_GATEWAY_OUT      "1002"      //网银支付出账交易

#define BT_MISSION_STATUS_SUSS      3        //任务状态-成功
#define BT_MISSION_STATUS_FAIL      2        //任务状态-失败

/*内部机构号*/
#define BT_BRH_ID_ALIPAY        "100001"      //兴业-支付宝支付
#define BT_BRH_ID_WECHAT          "100002"      //兴业-微信支付
#define BT_BRH_ID_CORE          "100003"      //星民易付
#define BT_BRH_ID_QUICK          "100004"      //兴业-快捷支付
#define BT_BRH_ID_GATEWAY          "100005"      //兴业-快捷支付
#define BT_BRH_ID_FUIOU          "100006"      //富友

#define BT_BRH_PAY_CORE         "200001"    //内部机构号-结算

/*任务索引号*/
#define BT_MISSION_INDEX_0401   "0401"      //导入网关流水文件
#define BT_MISSION_INDEX_0402   "0402"      //导入核心流水文件
#define BT_MISSION_INDEX_9901   "9901"      //判断清算日切
#define BT_MISSION_INDEX_9070   "9070"      //判断结算预处理
#define BT_MISSION_INDEX_5101   "5101"      //清分

#define BT_MISSION_INDEX_5501   "5501"      //商户入账数据准备


#define BT_SLEEP_TIME_30      30            //休眠时间-30

/*角色类型*/
#define BT_MCHT_ROLE_LEN           4    //角色类型长度
#define BT_MCHT_ROLE_SETT_IND          "1001"    //结算商户（二清）
#define BT_MCHT_ROLE_SETT_D          "2001"    //结算商户(直连)
#define BT_MCHT_ROLE_PROF_BRH_IND       "1002"    //分润机构(二清)
#define BT_MCHT_ROLE_PROF_BRH_D       "2002"    //分润机构(直连)
#define BT_MCHT_ROLE_PAY_BRH_IND       "1003"    //支付机构(二清)
#define BT_MCHT_ROLE_PAY_BRH_D       "2003"    //支付机构(直连)
#define BT_MCHT_ROLE_PLAT_IND          "1004"    //平台机构(二清)
#define BT_MCHT_ROLE_PLAT_D          "2004"    //平台机构(直连)

/*清算模式*/
#define BT_SETT_TRAN_LEN       4    //清算模式长度
#define BT_SETT_TRAN_D      "2000"    //直联模式
#define BT_SETT_TRAN_IND       "1000"    //二清模式


/*合作方编号*/
#define BT_MCHT_NO_PLAT            "mcht00000000005"    //平台
#define BT_MCHT_NO_SPL1            "mcht00000000001"    //分润商户1
#define BT_MCHT_NO_SPL2            "mcht00000000002"    //分润商户2
#define BT_MCHT_NO_SPL3            "mcht00000000003"    //分润商户3
#define BT_MCHT_NO_SPL4            "mcht00000000004"    //分润商户4

/*清算标志*/
#define BT_DAT_STLM_FLAG_LEN  1      //清算标志字段长度
#define BT_DAT_STLM_FLAG_N   "N"      //待清算
#define BT_DAT_STLM_FLAG_C   "C"      //清算成功

#define BT_SETT_TP_LEN     3        //合作方类型长度

/*合作方类型*/
#define BT_SETT_TP_EXT    "001"      //分润机构
#define BT_SETT_TP_BRH   "004"      //支付机构
#define BT_SETT_TP_SETT   "007"      //结算商户
#define BT_SETT_TP_PLAT    "010"      //平台

/*支付通道*/
#define BT_CHN_CIB_ALIPAY   "XY03"      //兴业-支付宝
#define BT_CHN_CIB_TENPAY   "XY04"      //兴业-微信
#define BT_CHN_CIB_QUICKPAY   "XY01"      //兴业-快捷支付
#define BT_CHN_CIB_GATEWAY   "XY02"      //兴业-网关支付
#define BT_CHN_CIB_XMYF   "XMYF01"      //核心-星民易付

#define BT_CHN_FY_ALIPAY "FY03"        // 富友-支付宝
#define BT_CHN_FY_TENPAY "FY04"        // 富友-微信

/*业务类型*/
#define BT_BUS_TP_LEN           4
#define BT_BUS_TP_DEF         "0000"      //默认
#define BT_BUS_TP_ALIPAY         "0304"      //兴业-支付宝
#define BT_BUS_TP_WECHAT         "0303"      //兴业-微信
#define BT_BUS_TP_CIB5            "0301"      //兴业-快捷支付
#define BT_BUS_TP_CIB6            "0302"      //兴业-网关支付
#define BT_BUS_TP_CORE           "0400"      //核心-星民易付

#define BT_BUS_FY_ALIPAY          "0504"     // 富友-支付宝
#define BT_BUS_FY_WECHAT          "0503"     // 富友-微信

/*核心的账户类型*/
#define BT_ACC_TYPE_PLAT_LEN           4
#define BT_ACC_TYPE_PLAT_DEP           "0022"      //运营商存款账户


#endif

