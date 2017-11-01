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

#define BT_DATE_LEN                 8           //�����ֶγ���
#define BT_TIME_LEN                 6           //ʱ���ֶγ���
#define BT_TRANS_TP_LEN             4           //���Ա�־�ֶγ���
#define BT_BUS_TP_HALF_LEN          4           //ҵ������-4�ֽ�
#define BT_BRH_ID_LEN               6           //�ڲ��������ֶγ���
#define BT_MISSION_INDEX_LEN        4           //�����������ֶγ���

#define BT_FLAG_RESULT_LEN          1       
#define BT_FLAG_RESULT_NULL         "-"         //δ����
#define BT_FLAG_RESULT_CMP          "1"         //��ƽ
#define BT_FLAG_RESULT_NOTMATC      "2"         //Ҫ�ز�ƽ
#define BT_FLAG_RESULT_GATEWAYNULL      "3"     //֧��������
#define BT_FLAG_RESULT_GATEWAY      "4"         //֧�����ض�
#define BT_FLAG_RESULT_CORENULL     "5"         //���������
#define BT_FLAG_RESULT_CORE         "6"         //������Ķ�
#define BT_FLAG_RESULT_PART_NULL      "7"       //��������
#define BT_FLAG_RESULT_PART      "8"            //��������
#define BT_FLAG_RESULT_COREFAIL      "9"        //����ʧ��/����/����
#define BT_FLAG_RESULT_GATEWAYFAIL      "A"        //֧������ʧ��/����/����
#define BT_FLAG_RESULT_PART_ORLESS    "B"             //������������
#define BT_FLAG_RESULT_PART_NOTORLESS    "C"             //T+2ȷ������������
#define BT_FLAG_RESULT_CORE_ORMORE    "D"             //����������ƶ� ��T+2ȷ���Ǻ��Ķ໹��������

#define BT_FLAG_RESULT_REFUND      "T"          //�����˿�(��������)

#define BT_TRANS_STATE_LEN        1            //����״̬�ֶγ���
#define BT_TRANS_STATE_SUCC      "1"            //����״̬-�ɹ�
#define BT_TRANS_STATE_FAIL      "0"            //����״̬-ʧ��

#define BT_REVSAL_FLAG_LEN        1            //����״̬�ֶγ���
#define BT_REVSAL_FLAG_Y      "1"            //������־-������
#define BT_REVSAL_FLAG_N      "0"            //������־-δ������

#define BT_CANCEL_FLAG_LEN        1            //����״̬�ֶγ���
#define BT_CANCEL_FLAG_Y      "1"            //������־-������
#define BT_CANCEL_FLAG_N      "0"            //������־-δ������

/*��������*/
#define BT_TRANS_TP_LEN         4               //����
#define BT_TRANS_TP_CHARGE         "1201"      //��ֵ����
#define BT_TRANS_TP_WITHDRAW       "1301"      //���ֽ���
#define BT_TRANS_TP_REFUND         "5001"      //�˿��

#define BT_TRANS_TP_QUICK_IN       "0001"      //���֧�����˽���
#define BT_TRANS_TP_QUICK_OUT      "1001"      //���֧�����˽���
#define BT_TRANS_TP_GATEWAY_IN       "0002"      //����֧�����˽���
#define BT_TRANS_TP_GATEWAY_OUT      "1002"      //����֧�����˽���

#define BT_MISSION_STATUS_SUSS      3        //����״̬-�ɹ�
#define BT_MISSION_STATUS_FAIL      2        //����״̬-ʧ��

/*�ڲ�������*/
#define BT_BRH_ID_ALIPAY        "100001"      //��ҵ-֧����֧��
#define BT_BRH_ID_WECHAT          "100002"      //��ҵ-΢��֧��
#define BT_BRH_ID_CORE          "100003"      //�����׸�
#define BT_BRH_ID_QUICK          "100004"      //��ҵ-���֧��
#define BT_BRH_ID_GATEWAY          "100005"      //��ҵ-���֧��
#define BT_BRH_ID_FUIOU          "100006"      //����

#define BT_BRH_PAY_CORE         "200001"    //�ڲ�������-����

/*����������*/
#define BT_MISSION_INDEX_0401   "0401"      //����������ˮ�ļ�
#define BT_MISSION_INDEX_0402   "0402"      //���������ˮ�ļ�
#define BT_MISSION_INDEX_9901   "9901"      //�ж���������
#define BT_MISSION_INDEX_9070   "9070"      //�жϽ���Ԥ����
#define BT_MISSION_INDEX_5101   "5101"      //���

#define BT_MISSION_INDEX_5501   "5501"      //�̻���������׼��


#define BT_SLEEP_TIME_30      30            //����ʱ��-30

/*��ɫ����*/
#define BT_MCHT_ROLE_LEN           4    //��ɫ���ͳ���
#define BT_MCHT_ROLE_SETT_IND          "1001"    //�����̻������壩
#define BT_MCHT_ROLE_SETT_D          "2001"    //�����̻�(ֱ��)
#define BT_MCHT_ROLE_PROF_BRH_IND       "1002"    //�������(����)
#define BT_MCHT_ROLE_PROF_BRH_D       "2002"    //�������(ֱ��)
#define BT_MCHT_ROLE_PAY_BRH_IND       "1003"    //֧������(����)
#define BT_MCHT_ROLE_PAY_BRH_D       "2003"    //֧������(ֱ��)
#define BT_MCHT_ROLE_PLAT_IND          "1004"    //ƽ̨����(����)
#define BT_MCHT_ROLE_PLAT_D          "2004"    //ƽ̨����(ֱ��)

/*����ģʽ*/
#define BT_SETT_TRAN_LEN       4    //����ģʽ����
#define BT_SETT_TRAN_D      "2000"    //ֱ��ģʽ
#define BT_SETT_TRAN_IND       "1000"    //����ģʽ


/*���������*/
#define BT_MCHT_NO_PLAT            "mcht00000000005"    //ƽ̨
#define BT_MCHT_NO_SPL1            "mcht00000000001"    //�����̻�1
#define BT_MCHT_NO_SPL2            "mcht00000000002"    //�����̻�2
#define BT_MCHT_NO_SPL3            "mcht00000000003"    //�����̻�3
#define BT_MCHT_NO_SPL4            "mcht00000000004"    //�����̻�4

/*�����־*/
#define BT_DAT_STLM_FLAG_LEN  1      //�����־�ֶγ���
#define BT_DAT_STLM_FLAG_N   "N"      //������
#define BT_DAT_STLM_FLAG_C   "C"      //����ɹ�

#define BT_SETT_TP_LEN     3        //���������ͳ���

/*����������*/
#define BT_SETT_TP_EXT    "001"      //�������
#define BT_SETT_TP_BRH   "004"      //֧������
#define BT_SETT_TP_SETT   "007"      //�����̻�
#define BT_SETT_TP_PLAT    "010"      //ƽ̨

/*֧��ͨ��*/
#define BT_CHN_CIB_ALIPAY   "XY03"      //��ҵ-֧����
#define BT_CHN_CIB_TENPAY   "XY04"      //��ҵ-΢��
#define BT_CHN_CIB_QUICKPAY   "XY01"      //��ҵ-���֧��
#define BT_CHN_CIB_GATEWAY   "XY02"      //��ҵ-����֧��
#define BT_CHN_CIB_XMYF   "XMYF01"      //����-�����׸�

#define BT_CHN_FY_ALIPAY "FY03"        // ����-֧����
#define BT_CHN_FY_TENPAY "FY04"        // ����-΢��

/*ҵ������*/
#define BT_BUS_TP_LEN           4
#define BT_BUS_TP_DEF         "0000"      //Ĭ��
#define BT_BUS_TP_ALIPAY         "0304"      //��ҵ-֧����
#define BT_BUS_TP_WECHAT         "0303"      //��ҵ-΢��
#define BT_BUS_TP_CIB5            "0301"      //��ҵ-���֧��
#define BT_BUS_TP_CIB6            "0302"      //��ҵ-����֧��
#define BT_BUS_TP_CORE           "0400"      //����-�����׸�

#define BT_BUS_FY_ALIPAY          "0504"     // ����-֧����
#define BT_BUS_FY_WECHAT          "0503"     // ����-΢��

/*���ĵ��˻�����*/
#define BT_ACC_TYPE_PLAT_LEN           4
#define BT_ACC_TYPE_PLAT_DEP           "0022"      //��Ӫ�̴���˻�


#endif

