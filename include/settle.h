#ifndef __HT_SETTLE_H
#define __HT_SETTLE_H
#define   MAX_CACL_ITEM         500                   /*�㷨���������Ŀ���ֵ*/
#define  TXN_DEBIT                    0                      /*���*/
#define  TXN_CREDIT                  1                     /*����*/
#define  NO_SETTLE                    10000              /*������*/
#define  MAX_DISC_ALGO          300                  /*����㷨����*/

#define   TRANS_AMT                 0   //���׽��
#define   TRANS_ZERO                1  //���±�ճ�����ʹ�ã�ֵ��Ϊ0

/* ������1 */ 
#define   MCHT1_BASE                            49
#define   MCHT1_FLOOR_AMOUNT        50    //����շѽ��(�ֶ�)
#define   MCHT1_UPPER_AMOUNT        51  //����շѽ��(�ֶ�)
#define   MCHT1_FIX_AMT                     52  //�̶�ֵ
#define   MCHT1_PCE_AMT                     53   //������
#define   MCHT1_FEE_MIN                     54  //��С������
#define   MCHT1_FEE_MAX                     55  //���������
#define   MCHT1_PCE_SHARE_ALLOT   56  //�������
#define   MCHT1_SETT_AMT                     57  //����
#define   MCHT1_SETT_FEE                     58  //������
#define   MCHT1_SHARE_ALLOT             59  //����

/* ������2 */
#define   MCHT2_BASE                            69
#define   MCHT2_FLOOR_AMOUNT         70    //����շѽ��(�ֶ�)
#define   MCHT2_UPPER_AMOUNT         71  //����շѽ��(�ֶ�)
#define   MCHT2_FIX_AMT                      72  //�̶�ֵ
#define   MCHT2_PCE_AMT                     73   //������
#define   MCHT2_FEE_MIN                      74  //��С������
#define   MCHT2_FEE_MAX                      75  //���������
#define   MCHT2_PCE_SHARE_ALLOT    76  //�������
#define   MCHT2_SETT_AMT                    77  //����
#define   MCHT2_SETT_FEE                     78  //������
#define   MCHT2_SHARE_ALLOT          79  //����
#define   MCHT2_SHARE_AMT            80  //�̻�������-֧��ͨ���ɱ�

/* ������3 */
#define   MCHT3_BASE                            89
#define   MCHT3_FLOOR_AMOUNT         90    //����շѽ��(�ֶ�)
#define   MCHT3_UPPER_AMOUNT         91  //����շѽ��(�ֶ�)
#define   MCHT3_FIX_AMT                       92  //�̶�ֵ
#define   MCHT3_PCE_AMT                      93   //������
#define   MCHT3_FEE_MIN                      94  //��С������
#define   MCHT3_FEE_MAX                     95  //���������
#define   MCHT3_PCE_SHARE_ALLOT    96  //�������
#define   MCHT3_SETT_AMT                    97  //����
#define   MCHT3_SETT_FEE                    98  //������
#define   MCHT3_SHARE_ALLOT           99  //����

/* ������4 */
#define   MCHT4_BASE                            109
#define   MCHT4_FLOOR_AMOUNT         110    //����շѽ��(�ֶ�)
#define   MCHT4_UPPER_AMOUNT         111  //����շѽ��(�ֶ�)
#define   MCHT4_FIX_AMT                     112  //�̶�ֵ
#define   MCHT4_PCE_AMT                     113   //������
#define   MCHT4_FEE_MIN                     114  //��С������
#define   MCHT4_FEE_MAX                     115  //���������
#define   MCHT4_PCE_SHARE_ALLOT   116  //�������
#define   MCHT4_SETT_AMT                    117  //����
#define   MCHT4_SETT_FEE                    118  //������
#define   MCHT4_SHARE_ALLOT             119  //����

/* ������5 */
#define   MCHT5_BASE                            129
#define   MCHT5_FLOOR_AMOUNT         130    //����շѽ��(�ֶ�)
#define   MCHT5_UPPER_AMOUNT         131  //����շѽ��(�ֶ�)
#define   MCHT5_FIX_AMT                     132  //�̶�ֵ
#define   MCHT5_PCE_AMT                     133   //������
#define   MCHT5_FEE_MIN                     134  //��С������
#define   MCHT5_FEE_MAX                     135  //���������
#define   MCHT5_PCE_SHARE_ALLOT    136  //�������
#define   MCHT5_SETT_AMT                    137  //����
#define   MCHT5_SETT_FEE                    138  //������
#define   MCHT5_SHARE_ALLOT             139  //����

/* ������6 */
#define   MCHT6_BASE                            149
#define   MCHT6_FLOOR_AMOUNT         150    //����շѽ��(�ֶ�)
#define   MCHT6_UPPER_AMOUNT         151  //����շѽ��(�ֶ�)
#define   MCHT6_FIX_AMT                     152  //�̶�ֵ
#define   MCHT6_PCE_AMT                     153   //������
#define   MCHT6_FEE_MIN                     154  //��С������
#define   MCHT6_FEE_MAX                     155  //���������
#define   MCHT6_PCE_SHARE_ALLOT   156  //�������
#define   MCHT6_SETT_AMT                    157  //����
#define   MCHT6_SETT_FEE                    158  //������
#define   MCHT6_SHARE_ALLOT         159  //����
#define   MCHT6_FILE_FEE                    160  //�����ļ���������
#define   MCHT6_FILE_FLAG                    161  //��ʶȡ�ļ�������:����Ǹ�ֵΪ���׽��+1

/* ������7 */
#define   MCHT7_BASE                            169
#define   MCHT7_FLOOR_AMOUNT         170    //����շѽ��(�ֶ�)
#define   MCHT7_UPPER_AMOUNT         171  //����շѽ��(�ֶ�)
#define   MCHT7_FIX_AMT                     172  //�̶�ֵ
#define   MCHT7_PCE_AMT                     173   //������
#define   MCHT7_FEE_MIN                     174  //��С������
#define   MCHT7_FEE_MAX                     175  //���������
#define   MCHT7_PCE_SHARE_ALLOT   176  //�������
#define   MCHT7_SETT_AMT                    177  //����
#define   MCHT7_SETT_FEE                    178  //������
#define   MCHT7_SHARE_ALLOT             179  //����

/* ������8 */
#define   MCHT8_BASE                            189
#define   MCHT8_FLOOR_AMOUNT         190    //����շѽ��(�ֶ�)
#define   MCHT8_UPPER_AMOUNT         191  //����շѽ��(�ֶ�)
#define   MCHT8_FIX_AMT                     192  //�̶�ֵ
#define   MCHT8_PCE_AMT                     193   //������
#define   MCHT8_FEE_MIN                     194  //��С������
#define   MCHT8_FEE_MAX                      195  //���������
#define   MCHT8_PCE_SHARE_ALLOT      196  //�������
#define   MCHT8_SETT_AMT                  197  //����
#define   MCHT8_SETT_FEE                    198  //������
#define   MCHT8_SHARE_ALLOT            199  //����

/* ������9 */
#define   MCHT9_BASE                            209
#define   MCHT9_FLOOR_AMOUNT         210    //����շѽ��(�ֶ�)
#define   MCHT9_UPPER_AMOUNT         211  //����շѽ��(�ֶ�)
#define   MCHT9_FIX_AMT                     212  //�̶�ֵ
#define   MCHT9_PCE_AMT                     213   //������
#define   MCHT9_FEE_MIN                     214  //��С������
#define   MCHT9_FEE_MAX                     215  //���������
#define   MCHT9_PCE_SHARE_ALLOT   216  //�������
#define   MCHT9_SETT_AMT                    217  //����
#define   MCHT9_SETT_FEE                    218  //������
#define   MCHT9_SHARE_ALLOT             219  //����

/* ƽ̨ */
#define   PLAT_BASE                             429
#define   PLAT_FLOOR_AMOUNT         430    //ƽ̨����շѽ��(�ֶ�)
#define   PLAT_UPPER_AMOUNT         431  //ƽ̨����շѽ��(�ֶ�)
#define   PLAT_FIX_AMT                     432  //ƽ̨�̶�ֵ
#define   PLAT_PCE_AMT                     433   //ƽ̨������
#define   PLAT_FEE_MIN                     434  //ƽ̨��С������
#define   PLAT_FEE_MAX                     435  //ƽ̨���������
#define   PLAT_PCE_SHARE_ALLOT   436  //ƽ̨�������
#define   PLAT_SETT_AMT                    437  //ƽ̨����
#define   PLAT_SETT_FEE                    438  //ƽ̨������
#define   PLAT_SHARE_ALLOT             439  //ƽ̨����
#define   PLAT_SHARE_CAL1             440  //��ż����м�ֵ
#define   PLAT_SHARE_CAL2             441  //��ż����м�ֵ

#endif

