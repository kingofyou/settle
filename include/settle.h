#ifndef __HT_SETTLE_H
#define __HT_SETTLE_H
#define   MAX_CACL_ITEM         500                   /*算法特殊参数数目最大值*/
#define  TXN_DEBIT                    0                      /*借记*/
#define  TXN_CREDIT                  1                     /*贷记*/
#define  NO_SETTLE                    10000              /*不清算*/
#define  MAX_DISC_ALGO          300                  /*最大算法步骤*/

#define   TRANS_AMT                 0   //交易金额
#define   TRANS_ZERO                1  //该下标空出不被使用，值恒为0

/* 合作方1 */ 
#define   MCHT1_BASE                            49
#define   MCHT1_FLOOR_AMOUNT        50    //最低收费金额(分段)
#define   MCHT1_UPPER_AMOUNT        51  //最大收费金额(分段)
#define   MCHT1_FIX_AMT                     52  //固定值
#define   MCHT1_PCE_AMT                     53   //按比例
#define   MCHT1_FEE_MIN                     54  //最小手续费
#define   MCHT1_FEE_MAX                     55  //最高手续费
#define   MCHT1_PCE_SHARE_ALLOT   56  //分润比例
#define   MCHT1_SETT_AMT                     57  //本金
#define   MCHT1_SETT_FEE                     58  //手续费
#define   MCHT1_SHARE_ALLOT             59  //分润

/* 合作方2 */
#define   MCHT2_BASE                            69
#define   MCHT2_FLOOR_AMOUNT         70    //最低收费金额(分段)
#define   MCHT2_UPPER_AMOUNT         71  //最大收费金额(分段)
#define   MCHT2_FIX_AMT                      72  //固定值
#define   MCHT2_PCE_AMT                     73   //按比例
#define   MCHT2_FEE_MIN                      74  //最小手续费
#define   MCHT2_FEE_MAX                      75  //最高手续费
#define   MCHT2_PCE_SHARE_ALLOT    76  //分润比例
#define   MCHT2_SETT_AMT                    77  //本金
#define   MCHT2_SETT_FEE                     78  //手续费
#define   MCHT2_SHARE_ALLOT          79  //分润
#define   MCHT2_SHARE_AMT            80  //商户手续费-支付通道成本

/* 合作方3 */
#define   MCHT3_BASE                            89
#define   MCHT3_FLOOR_AMOUNT         90    //最低收费金额(分段)
#define   MCHT3_UPPER_AMOUNT         91  //最大收费金额(分段)
#define   MCHT3_FIX_AMT                       92  //固定值
#define   MCHT3_PCE_AMT                      93   //按比例
#define   MCHT3_FEE_MIN                      94  //最小手续费
#define   MCHT3_FEE_MAX                     95  //最高手续费
#define   MCHT3_PCE_SHARE_ALLOT    96  //分润比例
#define   MCHT3_SETT_AMT                    97  //本金
#define   MCHT3_SETT_FEE                    98  //手续费
#define   MCHT3_SHARE_ALLOT           99  //分润

/* 合作方4 */
#define   MCHT4_BASE                            109
#define   MCHT4_FLOOR_AMOUNT         110    //最低收费金额(分段)
#define   MCHT4_UPPER_AMOUNT         111  //最大收费金额(分段)
#define   MCHT4_FIX_AMT                     112  //固定值
#define   MCHT4_PCE_AMT                     113   //按比例
#define   MCHT4_FEE_MIN                     114  //最小手续费
#define   MCHT4_FEE_MAX                     115  //最高手续费
#define   MCHT4_PCE_SHARE_ALLOT   116  //分润比例
#define   MCHT4_SETT_AMT                    117  //本金
#define   MCHT4_SETT_FEE                    118  //手续费
#define   MCHT4_SHARE_ALLOT             119  //分润

/* 合作方5 */
#define   MCHT5_BASE                            129
#define   MCHT5_FLOOR_AMOUNT         130    //最低收费金额(分段)
#define   MCHT5_UPPER_AMOUNT         131  //最大收费金额(分段)
#define   MCHT5_FIX_AMT                     132  //固定值
#define   MCHT5_PCE_AMT                     133   //按比例
#define   MCHT5_FEE_MIN                     134  //最小手续费
#define   MCHT5_FEE_MAX                     135  //最高手续费
#define   MCHT5_PCE_SHARE_ALLOT    136  //分润比例
#define   MCHT5_SETT_AMT                    137  //本金
#define   MCHT5_SETT_FEE                    138  //手续费
#define   MCHT5_SHARE_ALLOT             139  //分润

/* 合作方6 */
#define   MCHT6_BASE                            149
#define   MCHT6_FLOOR_AMOUNT         150    //最低收费金额(分段)
#define   MCHT6_UPPER_AMOUNT         151  //最大收费金额(分段)
#define   MCHT6_FIX_AMT                     152  //固定值
#define   MCHT6_PCE_AMT                     153   //按比例
#define   MCHT6_FEE_MIN                     154  //最小手续费
#define   MCHT6_FEE_MAX                     155  //最高手续费
#define   MCHT6_PCE_SHARE_ALLOT   156  //分润比例
#define   MCHT6_SETT_AMT                    157  //本金
#define   MCHT6_SETT_FEE                    158  //手续费
#define   MCHT6_SHARE_ALLOT         159  //分润
#define   MCHT6_FILE_FEE                    160  //对账文件的手续费
#define   MCHT6_FILE_FLAG                    161  //标识取文件手续费:如果是该值为交易金额+1

/* 合作方7 */
#define   MCHT7_BASE                            169
#define   MCHT7_FLOOR_AMOUNT         170    //最低收费金额(分段)
#define   MCHT7_UPPER_AMOUNT         171  //最大收费金额(分段)
#define   MCHT7_FIX_AMT                     172  //固定值
#define   MCHT7_PCE_AMT                     173   //按比例
#define   MCHT7_FEE_MIN                     174  //最小手续费
#define   MCHT7_FEE_MAX                     175  //最高手续费
#define   MCHT7_PCE_SHARE_ALLOT   176  //分润比例
#define   MCHT7_SETT_AMT                    177  //本金
#define   MCHT7_SETT_FEE                    178  //手续费
#define   MCHT7_SHARE_ALLOT             179  //分润

/* 合作方8 */
#define   MCHT8_BASE                            189
#define   MCHT8_FLOOR_AMOUNT         190    //最低收费金额(分段)
#define   MCHT8_UPPER_AMOUNT         191  //最大收费金额(分段)
#define   MCHT8_FIX_AMT                     192  //固定值
#define   MCHT8_PCE_AMT                     193   //按比例
#define   MCHT8_FEE_MIN                     194  //最小手续费
#define   MCHT8_FEE_MAX                      195  //最高手续费
#define   MCHT8_PCE_SHARE_ALLOT      196  //分润比例
#define   MCHT8_SETT_AMT                  197  //本金
#define   MCHT8_SETT_FEE                    198  //手续费
#define   MCHT8_SHARE_ALLOT            199  //分润

/* 合作方9 */
#define   MCHT9_BASE                            209
#define   MCHT9_FLOOR_AMOUNT         210    //最低收费金额(分段)
#define   MCHT9_UPPER_AMOUNT         211  //最大收费金额(分段)
#define   MCHT9_FIX_AMT                     212  //固定值
#define   MCHT9_PCE_AMT                     213   //按比例
#define   MCHT9_FEE_MIN                     214  //最小手续费
#define   MCHT9_FEE_MAX                     215  //最高手续费
#define   MCHT9_PCE_SHARE_ALLOT   216  //分润比例
#define   MCHT9_SETT_AMT                    217  //本金
#define   MCHT9_SETT_FEE                    218  //手续费
#define   MCHT9_SHARE_ALLOT             219  //分润

/* 平台 */
#define   PLAT_BASE                             429
#define   PLAT_FLOOR_AMOUNT         430    //平台最低收费金额(分段)
#define   PLAT_UPPER_AMOUNT         431  //平台最大收费金额(分段)
#define   PLAT_FIX_AMT                     432  //平台固定值
#define   PLAT_PCE_AMT                     433   //平台按比例
#define   PLAT_FEE_MIN                     434  //平台最小手续费
#define   PLAT_FEE_MAX                     435  //平台最高手续费
#define   PLAT_PCE_SHARE_ALLOT   436  //平台分润比例
#define   PLAT_SETT_AMT                    437  //平台本金
#define   PLAT_SETT_FEE                    438  //平台手续费
#define   PLAT_SHARE_ALLOT             439  //平台分润
#define   PLAT_SHARE_CAL1             440  //存放计算中间值
#define   PLAT_SHARE_CAL2             441  //存放计算中间值

#endif

