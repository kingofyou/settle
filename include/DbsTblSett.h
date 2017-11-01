#ifndef _DBS_TBL_SETT_H
#define _DBS_TBL_SETT_H

#define DBS_CURSOR_SENDFILE 51
#define DBS_CURSOR_DR       52
#define DBS_CURSOR_BNK      53
#define DBS_CURSOR_C        54
#define DBS_CURSOR_D        55
#define DBS_CURSOR_KRNL     56
#define DBS_CURSOR_PAUS     57
#define DBS_CUR_MISS_BNK    59


#define DBS_FETCH_SENDFILE  61
#define DBS_FETCH_DR        62
#define DBS_FETCH_BNK       63
#define DBS_FETCH_C         64
#define DBS_FETCH_D         65
#define DBS_FETCH_KRNL      66
#define DBS_FETCH_PAUS      67
#define DBS_FETCH_MISS_BNK  69


#define DBS_OPEN_SENDFILE   71
#define DBS_OPEN_DR         72
#define DBS_OPEN_BNK        73
#define DBS_OPEN_C          74
#define DBS_OPEN_D          75
#define DBS_OPEN_KRNL       76
#define DBS_OPEN_PAUS       77
#define DBS_OPEN_MISS_BNK   79


#define DBS_CLOSE_SENDFILE  81
#define DBS_CLOSE_DR        82
#define DBS_CLOSE_BNK       83
#define DBS_CLOSE_C         84
#define DBS_CLOSE_D         85
#define DBS_CLOSE_KRNL      86
#define DBS_CLOSE_PAUS      87
#define DBS_CLOSE_MISS_BNK  89


#define DBS_UPD_VAR         41
#define DBS_UPD_SEQ         42
#define DBS_UPD_FL          43
#define DBS_UPD_KEY         44
#define DBS_UPD_BAL         45
#define DBS_UPD_MISS        46
#define DBS_UPD_CRET        47
#define DBS_UPD_DEBT        48


#define DBS_SELECT_FEE      31
#define DBS_SELECT_INTEREST 32
#define DBS_SEL_TAL         33
#define DBS_SEL_BNK         34
#define DBS_SELECT_D        35
#define DBS_SELECT_MD       36
#define DBS_SELECT_ACCT     37
#define DBS_SELECT_MISS     38


#define DBS_TRUNCATE        10


#define DBS_NOTFOUND        1403


#define DBS_CURSOR_VAR      21
#define DBS_FETCH_VAR       22
#define DBS_OPEN_VAR        23
#define DBS_CLOSE_VAR       24


/* 结算明细表结构 */
typedef struct
{
    char        brh_id[10+1];
    char        sett_date[8+1] ;
    char        trans_date[8+1] ;
    char        mcht_no[15+1];
    char        mcht_role[4+1] ;
    char        mcht_rslt_no[32+1];
    char        seq_no[20+1];
    char        plat_sett_bank[12+1] ;
    char        plat_sett_bank_nm[80+1];
    char        plat_sett_acct_type[1+1];
    char        plat_sett_acct[22+1];
    char        plat_sett_acct_nm[80+1] ;
    char        dc_flag[1+1] ;
    char        mcht_sett_xingmin_acct[40+1];
    char        mcht_sett_bank[12+1] ;
    char        mcht_sett_bank_nm[80+1] ;
    char        mcht_sett_acct_type[1+1] ;
    char        mcht_sett_acct[22+1] ;
    char        mcht_sett_acct_nm[80+1] ;
    double      amt;
    double      amt_fee;
    double      plat_amt_fee;
    char        acct_statu[1+1];
    char        status[1+1] ;
    char        chl_id[10+1];
    char        chnl_name[70+1];
    char        file_name[100+1];
    char        ps[80+1] ;
    char        reserve1[8+1];
    char        reserve2[16+1];
    char        reserve3[32+1];
    char        reserve4[64+1];
    char        reserve5[128+1];
} tbl_sett_list_def;

/* 头寸信息表结构 */
typedef struct
{
    char        ext_mcht_no[15+1];              /*外部商户编号*/
    char        bank_code[12+1];                /*结算银行*/
    char        bank_name[70+1];                /*结算银行名称*/
    char        acct_no[22+1];                  /*备付金账号*/
    char        acct_name[70+1];                /*结算账户名称*/
    double      interest;                       /*利率*/
    char        sett_date[8+1];                 /*结算日期*/
    double      bal;                            /*更新前账户余额*/
    double      acct_bal;                       /*代付后账户余额*/
    double      resv_bal;                       /*保底余额*/
    double      avai_bal;                       /*可用余额*/
    double      block_bal;                      /*冻结余额*/
    double      ctrl_bal;                       /*控制余额*/
    char        sync_time[14+1];                /*更新时间*/
} tbl_postion_info_def;

/* 头寸试算表结构 */
typedef struct
{
    char        bank_code[12+1];                /*结算银行*/
    char        bank_name[70+1];                /*结算银行名称*/
    char        acct_no[22+1];                  /*备付金账号*/
    char        acct_name[70+1];                /*结算账户名称*/
    double      interest;                       /*利率*/
    char        sett_date[8+1];                 /*结算日期*/
    double      bal;                            /*账户余额*/
    double      resv_bal;                       /*保底余额*/
    double      bal_trial;                      /*试算余额*/
    char        seq_no[20+1];                   /*结算流水*/
    char        sync_time[14+1];                /*更新时间*/
} tbl_postion_trial_def;


/* 归集明细表结构 */
typedef struct
{
    char        brh_id[10+1];                   /*内部机构号*/
    char        sett_date[8+1];                 /*结算日期*/
    char        trans_date[8+1];                /*交易日期*/
    char        seq_no[20+1];                   /*归集流水*/
    char        bank_code[12+1];               /*备付金开户行*/
    char        acct_type[8+1];                 /*结算账户类型*/
    char        acct_no[22+1];                  /*结算账户账号*/
    char        acct_name[70+1];                /*平台结算账户名称*/
    char        dc_flag[1+1];                   /*支出,收入*/
    char        bank_code1[12+1];               /*备付金开户行*/
    char        acct_type1[8+1];                /*对手结算账户类型*/
    char        acct_no1[22+1];                 /*对手结算账户账号*/
    char        acct_name1[70+1];               /*对手结算账户名称*/
    double      amt;                            /*划账金额*/
    char        status[1+1];                    /*结算状态*/
    char        file_name[100+1];               /*划账文件名*/
    char        chnl_id[8+1];                   /*划账渠道*/
    char        ps[80+1];                       /*附言*/
} tbl_mission_info_def;

/* 渠道信息表(银行信息表)结构 */
typedef struct
{
    char        chnl_id[10+1];                  /*渠道编号*/
    char        chnl_name[70+1];                 /*渠道名称*/
    char        status[8+1];                    /*渠道状态*/
    double      min_amt;                        /*金额条件*/
    double      max_amt;                        /*渠道限额*/
    double      bal;                            /*可用余额*/
    char        index_day[6+1];                 /*工作日规则*/
    char        index_time[6+1];                /*时间规则*/
    char        period[8+1];                    /*支付周期*/
    char        index_fee[6+1];                 /*费率编码*/
    char        shadom[22+1];                    /*影子账户*/
} tbl_sett_chan_def;


/* 文件信息表结构 */
typedef struct
{
    char        file_name[100+1];               /*文件名*/
    char        bank_code[12+1];                /*平台银行编号*/
    char        sett_date[8+1];                 /*清算日期*/
    char        step_no[2+1];                   /*步骤号*/
    char        batch_no[2+1];                  /*批次号*/
    char        file_type[1+1];                 /*文件种类*/
    char        file_flag[1+1];                 /*文件类型*/
    char        end_flag[1+1];                  /*当天文件结束标志*/
    char        snd_time[14+1];                 /*发送时间*/
    char        rcv_time[14+1];                 /*接收时间*/
    char        status[1+1];                    /*文件处理状态*/
} tbl_file_info_def;

/* 统计待归集记录结构体 */
typedef struct
{
    char        sett_date[8+1];                 /*结算日期*/
    char        status[1+1];                    /*结算状态*/
    char        bank_no[12+1];                  /*结算银行*/
    char        acct_type[1+1];                 /*平台结算账户类型*/
    char        acct_no[22+1];                  /*平台结算账户账号*/
    char        acct_name[70+1];                /*平台结算账户名称*/
    double      total_amt;                      /*总金额*/
    int         total_num;                      /*总笔数*/
} sett_miss_info_def;

/* 银行响应文件结构体 */
typedef struct
{
    char        tran_date[8+1];                 /*交易日期*/
    char        tran_time[6+1];                 /*交易时间*/
    char        seq_no[20+1];                   /*结算流水*/
    char        tran_no[10+1];                  /*核心流水*/
    char        tran_type[1+1];                 /*交易类型*/
    char        tran_status[1+1];               /*交易状态*/
    char        acct_type[2+1];                 /*账户类型*/
    char        bank_code[12+1];                /*银行编号*/
    char        acct_no[22+1];                  /*账户账号*/
    char        tran_amt[12+1];                 /*交易金额*/
    char        tran_amt2[12+1];                /*交易金额2*/
    char        tran_amt3[12+1];                /*交易金额3*/
    char        tran_amt4[12+1];                /*交易金额4*/
    char        tran_amt5[12+1];                /*交易金额5*/
    char        reserve1[10+1];                 /*备用1*/
    char        reserve2[20+1];                 /*备用2*/
    char        reserve3[20+1];                 /*备用3*/
    char        reserve4[50+1];                 /*备用4*/
} resp_file_info_def;

/* 兴业银行响应文件结构体 */
typedef struct
{
    char        seq_no[32+1];                   /*结算流水或订单号*/
    char        mcht_no[60+1];                  /*二级商户*/
    char        tran_date[12+1];                 /*付款日期*/
    char        tran_time[6+1];                 /*付款时间*/
    char        pay_acct_no[22+1];              /*转出账户*/
    char        pay_acct_name[70+1];            /*转出账户名称*/
    char        acct_no[22+1];                  /*转入账户*/
    char        acct_name[70+1];                /*转入账户名称*/
    char        acct_type[30+1];                /*转入账户类型*/
    char        tran_amt[15+1];                 /*交易金额*/
    char        function[200+1];                /*用途*/
    char        tran_fee[15+1];                 /*手续费*/
    char        tran_status[1+1];               /*交易状态*/
    char        reserve[200+1];                 /*描述*/
} cib_rsp_info_def;

/* 入账结果表结构 */
typedef struct
{
    char        inter_brh_code[10+1];           /*内部机构号*/
    char        sett_date[8+1];                 /*清算日期*/
    char        acct_date[8+1];                 /*入账日期*/
    char        batch_no[2+1];                  /*平台批次号*/
    char        mcht_rslt_no[25+1];             /*商户入账流水号*/
    char        mcht_no[15+1];                  /*商户号*/
    char        mcht_role[1+1];                 /*商户角色*/
    int         trans_num;                      /*交易笔数*/
    double      debt_at;                        /*入账金额借*/
    double      cret_at;                        /*入账金额贷*/
    double      debt_net_fee;                   /*手续费净额借*/
    double      cret_net_fee;                   /*手续费净额贷*/
    char        sett_md[2+1];                   /*商户收支模式*/
    char        mcht_sett_bank[11+1];           /*商户结算行*/
    char        mcht_sett_bank_nm[80+1];        /*商户结算行行名*/
    char        mcht_sett_acct_type[1+1];       /*商户结算账户类型*/
    char        mcht_sett_acct[40+1];           /*商户结算账户账号*/
    char        mcht_sett_acct_nm[80+1];        /*商户结算账户名称*/
    char        plat_sett_bank[11+1];           /*平台结算行*/
    char        plat_sett_bank_nm[80+1];        /*平台结算行行名*/
    char        plat_sett_acct_type[1+1];       /*平台结算账户类型*/
    char        plat_sett_acct[40+1];           /*平台结算账户账号*/
    char        plat_sett_acct_nm[80+1];        /*平台结算账户名称*/
    char        cret_chnl_id[10+1];             /*收入划付渠道编号*/
    char        cret_chnl_nm[80+1];             /*收入划付渠道名称*/
    char        cret_rslt_no[20+1];             /*收入划付流水号*/
    char        mcht_debit_bank[11+1];          /*商户支出行*/
    char        mcht_debit_bank_nm[80+1];       /*商户支出行行名*/
    char        mcht_debit_acct_type[1+1];      /*商户支出账户类型*/
    char        mcht_debit_acct[40+1];          /*商户支出账户账号*/
    char        mcht_debit_acct_nm[80+1];      /*商户支出账户名称*/
    char        plat_debit_bank[11+1];          /*平台代扣行*/
    char        plat_debit_bank_nm[80+1];       /*平台代扣行行名*/
    char        plat_debit_acct_type[1+1];      /*平台代扣账户类型*/
    char        plat_debit_acct[40+1];          /*平台代扣账户账号*/
    char        plat_debit_acct_nm[80+1];       /*平台代扣账户名称*/
    char        debt_chnl_id[10+1];             /*支出划付渠道编号*/
    char        debt_chnl_nm[80+1];             /*支出划付渠道名称*/
    char        debt_rslt_no[20+1];             /*支出划付流水号*/
    char        acct_statu[1+1];                /*入账状态*/
    char        sz_status[1+1];                 /*收支状态*/
    int        seq_num;                         /*序号*/
} tbl_acct_rslt_def;

typedef struct
{
    char        inter_brh_code[10+1];           /*内部机构号*/
    char        sett_date[8+1];                 /*清算日期*/
    char        mcht_rslt_no[25+1];             /*商户入账流水号*/
    char        trans_num[12+1];                /*交易笔数*/
    double      debt_at;                        /*入账金额借*/
    double      cret_at;                        /*入账金额贷*/
    double      debt_net_fee;                   /*手续费净额借*/
    double      cret_net_fee;                   /*手续费净额贷*/
    char        sett_md[2+1];                   /*商户收支模式*/
    char        mcht_sett_bank[12+1];           /*商户结算行*/
    char        mcht_sett_bank_nm[80+1];        /*商户结算行行名*/
    char        mcht_sett_acct_type[1+1];       /*商户结算账户类型*/
    char        mcht_sett_acct[40+1];           /*商户结算账户账号*/
    char        mcht_sett_acct_nm[80+1];        /*商户结算账户名称*/
    char        plat_sett_bank[12+1];           /*平台结算行*/
    char        plat_sett_bank_nm[80+1];        /*平台结算行行名*/
    char        plat_sett_acct_type[1+1];       /*平台结算账户类型*/
    char        plat_sett_acct[40+1];           /*平台结算账户账号*/
    char        plat_sett_acct_nm[80+1];        /*平台结算账户名称*/
    int         seq_num;                        /*序号*/
} acct_rslt_cinfo_def;

typedef struct
{
    char        bank_code[12+1];                /*平台结算行*/
    char        acct_no[22+1];                  /*平台结算账户账号*/
    char        acct_name[70+1];                /*平台结算账户名称*/
    double      shrt_amt;                       /*缺口金额*/
    char        sett_date[8+1];                 /*结算日期*/
} tbl_funds_shrt_def;


typedef struct
{
    char seq_no[48+1]; 
    char top_cpg[10+1];
    char chl_id[10+1];
    char trans_date[8+1];
    char trans_time[6+1];
    char bank_code[20+1];
    char acct_type[10+1];
    char acct_no[40+1];
    char acct_name[60+1];
    char ccy[3+1];
    double amt;
    char bank_code1[20+1];
    char acct_type1[10+1];
    char acct_no1[40+1];
    char acct_name1[60+1];
    char dc_flag[2+1];
    char param_1[255+1];
} stHost_Acct_def;

#endif
