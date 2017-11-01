#ifndef _DBS_TBL_H
#define _DBS_TBL_H

#define CRiskUsed  49
#define MAXMISNS            15000
#define MAXCHILDS           1000
#define FLD_MISN_NUM_LEN    4
#define LOG_NAME_LEN_MAX    32
#define RECORD_LEN_MAX      2048
#define TXN_CFG_MAX_NUM     400
#define ATW_ISSP_MAX_NUM    1000
#define ATW_IRXP_MAX_NUM    1000

#define WT_STTL_STATUS      0
#define IN_STTL_STATUS      1
#define ERR_OCC_STATUS      2
#define STTL_DONE_STATUS    3
#define WT_RSP_STATUS        4
#define WT_REDO_STATUS        5
#define DW_EMPTY_FILE        9


#define MAXTXNS             150
#define MSG_SRC_ID_ANY      "9999"
#define DATE_LEN            8
#define FILE_NAME_LEN_MAX   200
#define SAMAXCOMMITCNT      1000
#define COMP_KEY_LEN        10
#define BUF_CHG_USAGE_KEY   7
#define FILE_COUNT_MAX      100
#define PATTERN_COUNT_MAX   5
#define PATTERN_LEN_MAX     64
#define INTER_INST_ID_LEN   10
#define INTER_BRH_CODE_LEN  10
#define SQL_BUF_LEN         256
#define NMMaxPartInfN       200
#define NMMaxOprInfN        200
#define NMMaxInstN          200



typedef struct
{
    char inter_brh_code[10 + 1];
    char settlmt_date[8 + 1];
    char mission_index [4 + 1];
    char mission_name[200 + 1];
    int mission_level;
    int mission_status;
    int data_num;
    int child_num;
    int commit_flag;
    int commit_num;
    char start_time[40 + 1];
    char end_time[40 + 1];
} tbl_mission_inf_def;

typedef struct
{
    char    inter_brh_code[10 + 1];
    char    stoday[8 + 1];
	char    stwodaysago[8 + 1];
    char    syesterday[8 + 1];
    char    snextdate[8 + 1];
    char    sstartdate[8 + 1];
    char    sforestartdate[8 + 1];
	int     status;
} tbl_date_inf_def;

typedef struct
{
    char       inter_brh_code[10 + 1];
    char       brh_level[1 + 1];
} stInstInf;

typedef struct
{
    int        inst_num;
    stInstInf  inst_def[NMMaxInstN];
} stInstDef;


typedef struct
{
    int    usage_key;
    int    fld_id;
    int    fld_l;
    int    fld_type;
    char    fld_dsp[257];
} Tbl_fld_dsp_Def;

typedef struct
{
    int    usage_key;
    int    buf_chg_index;
    int    sour_fld_index;
    int    dest_fld_index;
} Tbl_fld_tsf_Def;

typedef struct
{
    int    usage_key;
    char    comp_key[11];
    int    comp_key_len;
    int    ipc_dft_index;
    int    buf_dsp_index;
    char    txn_num[5];
    char    ipc_dft[257];
} Tbl_ipc_dft_dsp_Def;

typedef struct
{
    int    usage_key;
    int    buf_dsp_index;
    int    pos_index;
    int    fld_index;
    int    fld_id;
    int    fld_offset;
} Tbl_buf_dsp_Def;

typedef struct
{
    int    usage_key;
    int    buf_chg_index;
    int    sour_buf_index;
    int    dest_buf_index;
    char    buf_dsp[257];
} Tbl_buf_chg_Def;

typedef struct
{
    int    usage_key;
    int    ipc_dft_index;
    int    pos_index;
    int    fld_index;
    int    fld_len;
    char    fld_val[601];
} Tbl_ipc_dft_Def;


typedef struct 
{
    int usage_key;
    int txn_num;
    int con_index;
    int fld_index;
    int begin_byte_pos;
    int format_chg_need;
    char val [ 36];
} Tbl_con_inf_Def;

typedef struct
{
    char brh_id[10 + 1];
    char brh_level[1 + 1];
} tbl_brh_info_def1;

typedef struct
{
    char bus_tp	       [8 + 1];
    char brh_id[10 + 1];
} tbl_brh_map_def;

typedef struct
{
    char       inter_brh_code[10 + 1];
    char       settlmt_date[8 + 1];
    char   tbl_proc_name[60 + 1];
    int        tbl_proc_flag;
    int        tbl_proc_para1;
    int        tbl_proc_para2;
    int        tbl_proc_para3;
} tbl_proc_ctl_def;

typedef struct
{
    char       inter_brh_code[10 + 1];
    char       settlmt_date[8 + 1];
    char       mission_index[4 + 1];
    int        child_id;
    int        commit_flag;
    int        commit_num;
    int        child_begin_point;
    int        child_end_point;
    int        child_finish_point;
    int        child_err_point;
    int        child_status;
} tbl_child_inf_def;

typedef struct
{
    char        mission_type   [1  + 1];
    char        mission_index  [4  + 1];
    char        mission_name   [200 + 1];
    int         mission_level          ;
    int         data_num               ;
    int         commit_flag            ;
    int         commit_num             ;
} tbl_mission_cfg_def;

typedef struct
{
    char       inter_brh_code[10 + 1];
    char       mission_index[4 + 1];
    char       mission_name[200 + 1];
    char       mission_date[8 + 1];
    char       mission_weekday[1 + 1];
    char       mission_hour[2 + 1];
    int        mission_date_adj;
    int        data_num;
    int        child_num;
    int        commit_flag;
    int        commit_num;
} tbl_mission_time_def;
typedef struct
{
    char       inter_brh_code[10 + 1];
    char       mission_index[4 + 1];
    char       mission_date[8 + 1];
    char       mission_weekday[1 + 1];
    char       mission_hour[2 + 1];
    int        mission_date_adj;
    int        child_id;
    int        commit_flag;
    int        commit_num;
    int        child_begin_point;
    int        child_end_point;
    int        child_finish_point;
    int        child_err_point;
    int        child_status;
} tbl_child_time_def;

typedef struct
{
    char    inter_brh_code[10 + 1];   /* 唯一键值1 */
    char    sa_stlm_inst_id[11 + 1];
    char    stlm_dt[8 + 1];           /* 唯一键值2 */
    long    bat_id;                   /* 唯一键值3 */
    char    src_file_name[40];        /* 唯一键值4 */
    char    host_name[50 + 1];
    char    acq_ins_id_cd[10 + 1];
    char    sa_trace_log[60 + 1];
    char    file_type[4 + 1];
    char    proc_flg[1 + 1];
    long    rec_num;
} stExtnFileRegDef;
/* ExtnFileReg */

typedef struct
{
    char inter_brh_code[10 + 1];
    char sett_date	   [8 + 1];
    char trans_date	   [8 + 1];
    char trans_time	   [10 + 1];
    char flag_result	 [1 + 1];
    char order_no	     [32 + 1];
    char plat_key	     [48 + 1];
    char host_key	     [48 + 1];
    char batch_no	     [2 + 1];
    char bus_tp	       [8 + 1];
    char trans_tp	     [4 + 1];
    char acct_type	   [2 + 1];
    char trans_state	 [1 + 1];
    char revsal_flag	 [1 + 1];
    char key_revsal	   [32 + 1];
    char cancel_flag	 [1 + 1];
    char key_cancel	   [32 + 1];
    char pan	         [20 + 1];
    char acct_name	     [60 + 1];
    char pan1	     [20 + 1];
    char ccy            [3 + 1];
    char trans_at	     [12 + 1];
    char trans_at2	   [12 + 1];
    char trans_at3	   [12 + 1];
    char trans_at4	   [12 + 1];
    char trans_at5	   [12 + 1];
    char sett_mcht_no	 [15 + 1];    
    char ext_mcht_no	 [15 + 1];
    char sett_tp1	     [3 + 1];
    char mcht_no1	     [15 + 1];
    char sett_tp2	     [3 + 1];
    char mcht_no2	     [15 + 1];
    char sett_tp3	     [3 + 1];
    char mcht_no3	     [15 + 1];
    char sett_tp4	     [3 + 1];
    char mcht_no4	     [15 + 1];
    char sett_tp5	     [3 + 1];
    char mcht_no5	     [15 + 1];
    char sett_tp6	     [3 + 1];
    char mcht_no6	     [15 + 1];
    char sett_tp7	     [3 + 1];
    char mcht_no7	     [15 + 1];
    char sett_tp8	     [3 + 1];
    char mcht_no8	     [15 + 1];
    char sett_tp9	     [3 + 1];
    char mcht_no9	     [15 + 1];
    int seq_num;
}ht_tbl_gateway_txn_def;

typedef struct
{
        char inter_brh_code	 [10+ 1];
	char sett_date	    [8+ 1];
	char trans_date	    [8+ 1];
	char trans_time	    [10+ 1];
	char flag_result	 [1+ 1];
	char ext_mcht_no	 [15+ 1];
	char order_no	     [32+ 1];
	char plat_key	     [48+ 1];
	char batch_no	     [2+ 1];
	char bus_tp	       [8+ 1];
	char trans_tp	     [4+ 1];
	char trans_state	 [1+ 1];
	char revsal_flag	 [1+ 1];
	char key_revsal	   [32+ 1];
	char cancel_flag	 [1+ 1];
	char key_cancel	   [32+ 1];
	char acct_type	   [2+ 1];
	char pan	         [20+ 1];
	char ccy	         [3+ 1];
	char trans_at	   [12+ 1];
	char trans_at2	 [12+ 1];
	char trans_at3	 [12+ 1];
	char trans_at4	 [12+ 1];
	char trans_at5	 [12+ 1];
        int seq_num      ;
}ht_tbl_host_txn_def;


typedef struct
{
char inter_brh_code    [10+ 1];
char sett_date	       [8 + 1];
char trans_date        [8 + 1];
char trans_time        [10+ 1];
char gateway_key       [48 + 1];
char order_no          [32 + 1];
char payway_type       [32 + 1];
char bus_tp            [8 + 1];
char trans_tp          [4 + 1];
char user_id           [20 + 1];
char pan               [20 + 1];
double trans_at        ;
char sett_mcht_no      [15 + 1];
char ext_mcht_no       [15 + 1] ;
char risk_reason       [255 + 1];
char remark            [256 + 1];
}ht_tbl_risk_txn_def;

typedef struct
{
    char    tbl_name[60 + 1];
    char    part_name[60 + 1];
    char    tbl_spc_name[60 + 1];
    int     tbl_interval_days;
    int    tbl_opr_flag;
} Tbl_part_inf_Def;

typedef struct
{
    char    brh_id[10 + 1];
    char       brh_level[1 + 1];
} tbl_CstBrhCupInf_def;


typedef struct
{
char inter_brh_code	[10 + 1];
char      sett_date	[8 + 1];
char    trans_date	[8 + 1];
char    trans_time	[10 + 1];
char    flag_result	[1 + 1];
char    acct_statu  [1 + 1];
char      order_no	[32 + 1];
char      plat_key	[48 + 1];
char      batch_no	[2 + 1];
char        bus_tp	[8 + 1];
char      trans_tp	[4 + 1];
char      acct_type	[2 + 1];
char    trans_state	[1 + 1];
char            pan	[20 + 1];
char            pan1	[20 + 1];
char            ccy	[3 + 1];
char      trans_at	[12 + 1];
char      dest_pan	[20 + 1];
char      dest_ccy	[3 + 1];
char  dest_trans_at	[12 + 1];
char  debt_at	[12 + 1];
char  trans_fee	[12 + 1];
char        reserve	[48 + 1];
}ht_tbl_err_flow_def;
typedef struct
{
char inter_brh_code	 [10 + 1];
char      sett_date	 [8 + 1];
char    trans_date	 [8 + 1];
char    trans_time	 [10 + 1];
char    flag_result	 [1 + 1];
char      order_no	 [32 + 1];
char      plat_key	 [48 + 1];
char      batch_no	 [2 + 1];
char        bus_tp	 [8 + 1];
char      trans_tp	 [4 + 1];
char      acct_type	 [2 + 1];
char    trans_state	 [1 + 1];
char    revsal_flag	 [1 + 1];
char    key_revsal	 [32 + 1];
char    cancel_flag	 [1 + 1];
char    key_cancel	 [32 + 1];
char            pan	 [20 + 1];
char acct_name	     [60 + 1];
char            pan1	 [20 + 1];
char            ccy	 [3 + 1];
char      trans_at	 [12 + 1];
char      trans_at2	 [12 + 1];
char      trans_at3	 [12 + 1];
char      trans_at4	 [12 + 1];
char      trans_at5	 [12 + 1];
char    ext_mcht_no	 [15 + 1];
char      sett_tp1	 [3 + 1];
char      mcht_no1	 [15 + 1];
char      sett_tp2	 [3 + 1];
char      mcht_no2	 [15 + 1];
char      sett_tp3	 [3 + 1];
char      mcht_no3	 [15 + 1];
char      sett_tp4	 [3 + 1];
char      mcht_no4	 [15 + 1];
char      sett_tp5	 [3 + 1];
char      mcht_no5	 [15 + 1];
char      sett_tp6	 [3 + 1];
char      mcht_no6	 [15 + 1];
char      sett_tp7	 [3 + 1];
char      mcht_no7	 [15 + 1];
char      sett_tp8	 [3 + 1];
char      mcht_no8	 [15 + 1];
char      sett_tp9	 [3 + 1];
char      mcht_no9	 [15 + 1];
int       seq_num	  ;
}ht_tbl_cmp_flow_def;
typedef struct
{
char inter_brh_code	[10 + 1];
char      sett_date	[8 + 1];
char    trans_date	[8 + 1];
char    trans_time	[10 + 1];
char    flag_result	[1 + 1];
char    stlm_flag[1+1];
char      order_no	[32 + 1];
char      plat_key	[48 + 1];
char      batch_no	[2 + 1];
char        bus_tp	[8 + 1];
char      trans_tp	[4 + 1];
char      acct_type	[2 + 1];
char    trans_state	[1 + 1];
char    revsal_flag	[1 + 1];
char    key_revsal	[32 + 1];
char    cancel_flag	[1 + 1];
char    key_cancel	[32 + 1];
char            pan	[20 + 1];
char acct_name	     [60 + 1];
char            pan1	[20 + 1];
char            ccy	[3 + 1];
char      trans_at	[12 + 1];
char      trans_at2	[12 + 1];
char      trans_at3	[12 + 1];
char      trans_at4	[12 + 1];
char      trans_at5	[12 + 1];
char    ext_mcht_no	[15 + 1];
char      sett_tp1	[3 + 1];
char      mcht_no1	[15 + 1];
char      sett_tp2	[3 + 1];
char      mcht_no2	[15 + 1];
char      sett_tp3	[3 + 1];
char      mcht_no3	[15 + 1];
char      sett_tp4	[3 + 1];
char      mcht_no4	[15 + 1];
char      sett_tp5	[3 + 1];
char      mcht_no5	[15 + 1];
char      sett_tp6	[3 + 1];
char      mcht_no6	[15 + 1];
char      sett_tp7	[3 + 1];
char      mcht_no7	[15 + 1];
char      sett_tp8	[3 + 1];
char      mcht_no8	[15 + 1];
char      sett_tp9	[3 + 1];
char      mcht_no9	[15 + 1];
int seq_num	         ;
}ht_tbl_dat_flow_def;
typedef struct
{
char                  mcht_no	  [15 + 1];
char                  mcht_nm	  [60 + 1];
char    up_mcht_no[15+1]; 
char                  contact	  [30 + 1];
char              comm_mobil	  [18 + 1];
char                comm_tel	  [18 + 1];
char                  manager	  [32 + 1];
char          artif_certif_tp	  [2 + 1];
char              identity_no	  [20 + 1];
char              manager_tel	  [12 + 1];
char                reg_addr	  [60 + 1];
char                  oper_no	  [8 + 1];
char                  oper_nm	  [10 + 1];
char              mcht_status	  [10 + 1];
char                  clear_flag	  [1 + 1];
char                  sett_md	  [2 + 1];
char          mcht_sett_bank	  [12 + 1];
char        mcht_sett_bank_nm	  [80 + 1];
char      mcht_sett_acct_type	  [2 + 1];
char          mcht_sett_acct	  [40 + 1];
char        mcht_sett_acct_nm	  [80 + 1];
char        xingmin_pay_acct	  [40 + 1];
char        businuss_brh_no	      [12 + 1];
char          mcht_debit_bank	  [12 + 1];
char      mcht_debit_bank_nm	  [80 + 1];
char    mcht_debit_acct_type	  [2 + 1];
char          mcht_debit_acct	  [40 + 1];
char      mcht_debit_acct_nm	  [80 + 1];
char      mcht_role	  [2 + 1];
char    js_type[2 + 1];
char    clear_cycle	[3 + 1];
char    splitting_type[2 + 1];
char    spl_acct_type1[1 + 1];
char    spl_acct_no1[40 + 1];
char    spl_acct_nm1[80 + 1];
char    spl_acct_bank1[12 + 1];
char    spl_acct_banknm1[80 + 1];
char  spl_brh_role1	[12+1];
char  spl_brh_nm1	[30+1];
double  spl_acct_rate1;
char    spl_acct_ret_fee_flag1[1+1];
char    spl_acct_type2[1 + 1];
char    spl_acct_no2[40 + 1];
char    spl_acct_nm2[80 + 1];
char    spl_acct_bank2[12 + 1];
char    spl_acct_banknm2[80 + 1];
char spl_brh_role2	[12+1];
char spl_brh_nm2	[30+1];
double  spl_acct_rate2;
char    spl_acct_ret_fee_flag2[1+1];
char    spl_acct_type3[1 + 1];
char    spl_acct_no3[40 + 1];
char    spl_acct_nm3[80 + 1];
char    spl_acct_bank3[12 + 1];
char    spl_acct_banknm3[80 + 1];
char  spl_brh_role3[12+1];
char  spl_brh_nm3	[30+1];
double  spl_acct_rate3;
char    spl_acct_ret_fee_flag3[1+1];
char    spl_acct_type4[1 + 1];
char    spl_acct_no4[40 + 1];
char    spl_acct_nm4[80 + 1];
char    spl_acct_bank4[12 + 1];
char    spl_acct_banknm4[80 + 1]; 
char spl_brh_role4	[12+1];
char  spl_brh_nm4	[30+1];
double  spl_acct_rate4;
char    spl_acct_ret_fee_flag4[1+1];
char              crt_opr_id	  [40 + 1];
char              rec_upd_ts	  [14 + 1];
char              upd_opr_id	  [40 + 1];
char  send_bill_flag[3+1];
char  email[128+1];
char  ret_fee_flag[1+1];
char  recharge_flag[1+1];
}ht_tbl_mcht_inf_def;

typedef struct
{
char         disc_id	  [5 + 1];
char         tran_code	  [4 + 1];
char         mcht_no	  [15 + 1];
char       card_type	  [4 + 1];
double floor_amount	 ;
double upper_amount	 ;
char       fee_flag	 [1 + 1];
double     fee_value	;
double       fee_min	;
double       fee_max	;
char  rec_upd_usr_id	   [10 + 1];
char     rec_upd_ts	 [14 + 1];
char     rec_crt_ts	 [14 + 1];
}ht_tbl_his_disc_algo_def;

typedef struct
{
char        inter_brh_code	   [10+1];
char              sett_date	   [8+1] ;
char            trans_date	   [8+1] ;
char            trans_time	   [10+1];
char            flag_result	   [1+1] ;
char            acct_statu     [1+1] ;
char                status     [1+1] ;
char              order_no	   [32+1];
char              plat_key	   [48+1];
char              batch_no	   [2+1] ;
char                bus_tp	   [8+1] ;
char              trans_tp	   [4+1] ;
char              acct_type	   [2+1] ;
char            trans_state	   [1+1] ;
char            revsal_flag	   [1+1] ;
char            key_revsal	   [32+1];
char            cancel_flag	   [1+1] ;
char            key_cancel	   [32+1];
char                    pan	   [20+1];
char              acct_name    [60+1];
char                    pan1	   [20+1];
double        amt_fee;
char          bank_order_no    [32+1];
char                    ccy	   [3+1] ;
double            trans_at	         ;
double            trans_at2	         ;
double            trans_at3	         ;
double            trans_at4	         ;
double            trans_at5	         ;
char            ext_mcht_no	   [15+1];
char              sett_tp1	   [3+1] ;
char              mcht_no1	   [15+1];
char              up_mcht_no1	   [15+1];
char            mcht_role1	   [4+1] ;
double    debt_at_mcht_no1	         ;
double    cret_at_mcht_no1	         ;
double    debt_fee_mcht_no1	         ;
double    cret_fee_mcht_no1	         ;
double   share_profit_mcht_no1	     ;
double  mcht_rate1;
char              sett_tp2	   [3+1] ;
char              mcht_no2	   [15+1];
char              up_mcht_no2	   [15+1];
char            mcht_role2	   [4+1] ;
double    debt_at_mcht_no2	         ;
double    cret_at_mcht_no2	         ;
double    debt_fee_mcht_no2	         ;
double    cret_fee_mcht_no2	         ;
double   share_profit_mcht_no2	     ;
double  mcht_rate2;
char              sett_tp3	   [3+1] ;
char              mcht_no3	   [15+1];
char              up_mcht_no3	   [15+1];
char            mcht_role3	   [4+1] ;
double    debt_at_mcht_no3	         ;
double    cret_at_mcht_no3	         ;
double    debt_fee_mcht_no3	         ;
double    cret_fee_mcht_no3	         ;
double  share_profit_mcht_no3	       ;
double  mcht_rate3;
char              sett_tp4	   [3+1] ;
char              mcht_no4	   [15+1];
char              up_mcht_no4	   [15+1];
char            mcht_role4	   [4+1] ;
double    debt_at_mcht_no4	         ;
double    cret_at_mcht_no4	         ;
double    debt_fee_mcht_no4	         ;
double    cret_fee_mcht_no4	         ;
double  share_profit_mcht_no4	       ;
double  mcht_rate4;
char              sett_tp5	   [3+1] ;
char              mcht_no5	   [15+1];
char              up_mcht_no5	   [15+1];
char            mcht_role5	   [4+1] ;
double    debt_at_mcht_no5	         ;
double    cret_at_mcht_no5	         ;
double    debt_fee_mcht_no5	         ;
double    cret_fee_mcht_no5	         ;
double  share_profit_mcht_no5	       ;
double  mcht_rate5;
char              sett_tp6	   [3+1] ;
char              mcht_no6	   [15+1];
char              up_mcht_no6	   [15+1];
char            mcht_role6	   [4+1] ;
double    debt_at_mcht_no6	         ;
double    cret_at_mcht_no6	         ;
double    debt_fee_mcht_no6	         ;
double    cret_fee_mcht_no6	         ;
double  share_profit_mcht_no6	       ;
double  mcht_rate6;
char              sett_tp7	   [3+1] ;
char              mcht_no7	   [15+1];
char              up_mcht_no7	   [15+1];
char            mcht_role7	   [4+1] ;
double    debt_at_mcht_no7	         ;
double    cret_at_mcht_no7	         ;
double    debt_fee_mcht_no7	         ;
double    cret_fee_mcht_no7	         ;
double  share_profit_mcht_no7	       ;
double  mcht_rate7;
char              sett_tp8	   [3+1] ;
char              mcht_no8	   [15+1];
char              up_mcht_no8	   [15+1];
char            mcht_role8	   [4+1] ;
double    debt_at_mcht_no8	         ;
double    cret_at_mcht_no8	         ;
double    debt_fee_mcht_no8	         ;
double    cret_fee_mcht_no8	         ;
double  share_profit_mcht_no8	       ;
double  mcht_rate8;
char              sett_tp9	   [3+1] ;
char              mcht_no9	   [15+1];
char              up_mcht_no9	   [15+1];
char            mcht_role9	   [4+1] ;
double    debt_at_mcht_no9	         ;
double    cret_at_mcht_no9	         ;
double    debt_fee_mcht_no9	         ;
double    cret_fee_mcht_no9	         ;
double  share_profit_mcht_no9	       ;
double  mcht_rate9;
double        debt_at_plat	         ;
double        cret_at_plat	         ;
double        debt_fee_plat	         ;
double        cret_fee_plat	         ;
double    share_profit_plat	         ;
char    remark[512+1];
int                 seq_num	         ;
} ht_tbl_sett_flow_def;

typedef struct
{
char bus_tp	    [8+1];
char txn_num	    [8+1];
char bus_name	    [80+1];
char txn_name	    [80+1];
char c_d_flag	  [1+1];
char stlm_flag	  [10+1];
char rec_upd_opr  [11+1];
char rec_crt_ts	[14+1];
char rec_upd_ts	[14+1];
}ht_tbl_algo_ctl_def;
typedef struct
{
char	bus_tp	    [8+1];
char	txn_num	    [4+1];
char	algo_id	    [5+1];
char	param_flag	[32+1];
char	rec_upd_opr	[11+1];
char	rec_crt_ts	[14+1];
char	rec_upd_ts	[14+1];
}ht_tbl_txn_algo_rule_def;

typedef struct
{
char disc_id	[5+1 ];
int disc_step	;
double oper_rslt	;
double operand1	;
char operator1	[1+1 ];
double operand2	;
char operator2	[1+1 ];
double operand3	;
char rec_opr_id	[1+1 ];
char rec_upd_opr	[11+1 ];
char rec_crt_ts	[14+1 ];
char rec_upd_ts	[14+1 ];
}ht_tbl_disc_algo_def;

typedef struct
{
char   inter_brh_code	  [10 + 1];
char    sett_date	[8 + 1];
char trans_date       	   [8 + 1];       
char trans_time        [10 + 1]; 
char    acct_date	[8+ 1] ;
char    batch_no	  [2 + 1];
char    bus_tp	  [8 + 1];
char    trans_tp	  [4 + 1];
char    mcht_role[4 + 1];
char    mcht_no	[15 + 1];
char    acct_type[2+1];
char    sett_md[2+1];
char    mcht_sett_bank	[12 + 1];
char    mcht_sett_bank_nm	[80 + 1];
char    mcht_sett_acct_type	[1 + 1];
char    mcht_sett_acct	  [40 + 1];
char    mcht_sett_acct_nm	[80 + 1];
char    mcht_sett_xingmin_acct	[40 + 1];
char         mcht_debit_bank	              [12+ 1];
char     mcht_debit_bank_nm	              [80+ 1]  ;
char    mcht_debit_acct_type	               [1+ 1];
char         mcht_debit_acct	              [40+ 1];
char     mcht_debit_acct_nm	              [80+ 1]  ;
char    plat_sett_bank	              [12+ 1]  ;
char    plat_sett_bank_nm	              [80+ 1];
char    plat_sett_acct_type	              [1+ 1] ;
char    plat_sett_acct	              [40+ 1]  ;
char    plat_sett_acct_nm	              [80+ 1];
char    plat_sett_xingmin_acct	              [40+ 1];    
char    plat_debit_bank	[12 + 1];
char    plat_debit_bank_nm	[80 + 1];
char    plat_debit_acct_type	[1 + 1];
char    plat_debit_acct	[40 + 1];
char    plat_debit_acct_nm	[80 + 1];
char    plat_debit_xingmin_acct	[40 + 1];
double  debt_at;
double  cret_at	;
double  debt_net_fee	;
double  cret_net_fee;
double  share_profit;
int trans_num ;
double  mcht_rate;
char    remark[512+1];
int     seq_num 	;
}ht_tbl_sett_rslt_def;

typedef struct
{
    char         inter_brh_code	              [10+ 1]  ;
    char               sett_date	              [8+ 1] ;
    char               acct_date	              [8+ 1] ;
    char               batch_no	              [2+ 1]   ;
    char                 mcht_rslt_no	              [32+ 1];
    char                 mcht_no	              [15+ 1];
    char               mcht_role	              [4+ 1] ;
    int                trans_num	                     ;
    double               debt_at	                     ;
    double               cret_at	                     ;
    double         debt_net_fee	                       ;
    double         cret_net_fee	                       ;
    char                 sett_md	              [2+ 1] ;
    char         mcht_sett_bank	              [12+ 1]  ;
    char       mcht_sett_bank_nm	              [80+ 1];
    char     mcht_sett_acct_type	              [1+ 1] ;
    char         mcht_sett_acct	              [40+ 1]  ;
    char       mcht_sett_acct_nm	              [80+ 1];
    char    mcht_sett_xingmin_acct	[40 + 1];
    char         plat_sett_bank	              [12+ 1]  ;
    char       plat_sett_bank_nm	              [80+ 1];
    char     plat_sett_acct_type	              [1+ 1] ;
    char         plat_sett_acct	              [40+ 1]  ;
    char       plat_sett_acct_nm	              [80+ 1];
    char       plat_sett_xingmin_acct	              [40+ 1];    
    char         mcht_debit_bank	              [12+ 1];
    char     mcht_debit_bank_nm	              [80+ 1]  ;
    char    mcht_debit_acct_type	               [1+ 1];
    char         mcht_debit_acct	              [40+ 1];
    char     mcht_debit_acct_nm	              [80+ 1]  ;
    char         plat_debit_bank	              [12+ 1];
    char     plat_debit_bank_nm	              [80+ 1]  ;
    char    plat_debit_acct_type	               [1+ 1];
    char         plat_debit_acct	              [40+ 1];
    char     plat_debit_acct_nm	              [80+ 1]  ;
    char       plat_debit_xingmin_acct	              [40+ 1];    
    char             acct_statu	              [1+ 1]   ;
    char             sz_status                [1+1];
    int              seq_num 	                         ;
}ht_tbl_acct_rslt_def;

typedef struct
{
    char inter_brh_code   [10 + 1];           
    char sett_date        	   [8 + 1];        
    char trans_date       	   [8 + 1];       
    char trans_time        [10 + 1];           
    char flag_result      	 [1 + 1];         
    char gener_id         	     [20 + 1];  
    char thr_mcht_no         	     [32 + 1];        
    char ext_mcht_no      	     [32 + 1];     
    char ex_ext_mcht_no      [32 + 1];         
    char equipment_id            [100 + 1];    
    char wehat_mcht_no    	     [32 + 1]; 
    char thr_order_no         	     [48 + 1];     
    char order_no         	   [48 + 1];      
    char user_id           [30 + 1];          
    char plat_key          [48 + 1];          
    char batch_no            [2 + 1];    
    char bus_tp         	 [8 + 1];       
    char trans_tp         	 [4 + 1];  
    char wift_trans_tp         	 [48 + 1];          
    char trans_state      	   [30 + 1];        
    char pay_bank                [15 + 1];     
    char wx_refund_order        [32 + 1];       
    char mcht_refund_order     [32 + 1];      
    char refund_type      	   [10 + 1];       
    char refund_status    	   [40 + 1];       
    char goods_name       	   [400 + 1];       
    char mcht_data        	   [500 + 1];      
    char wx_fee                 [12 + 1];     
    char wx_rate            [10 + 1];         
    char acct_type        [2 + 1];            
    char pan              [19 + 1];           
    char ccy              [3 + 1];            
    char trans_at         [12 + 1];           
    char trans_at2        [12 + 1];           
    char trans_at3        [12 + 1];           
    char trans_at4        [12 + 1];           
    char trans_at5        [12 + 1]; 
    char trans_at6        [12 + 1];        
    int  seq_num;                                       
}ht_tbl_wift_txn_def;

typedef struct
{
    char inter_brh_code [10+1];
    char sett_date [8+1];
    char mcht_no	[15+1];
    char mcht_nm	[60+1];
    int trans_num;
    double trans_at;                         
}ht_tbl_active_mcht_inf_def;

typedef struct
{
    char type[4+1];
    char bank_code[20+1];
    char acct_type[20+1];
    char acct_no[40+1];
    char acct_name[60+1];
    double account_balance;
    double available_balance;
    double blocked_balance;
    double controlled_balance;
    char rec_crt_ts[14+1];
    char rec_upd_ts[14+1];                           
}ht_tbl_acct_inf_def;

typedef struct
{
    char inter_brh_code	[10+1];
    char sett_date	[8+1];
    char epaytime	[14+1];
    char orderdate   [8+1];
    char transdate   [8+1];
    char plat_key	[48+1];
    char batch_no	[2+1];
    char flag_result	[1+1];
    char sno         [16+1];
    char mrchno      [8+1]; 
    char order_no    [32+1];
    char orderamt    [12+1];
    char feeamt      [12+1];
    char bus_tp	[8+1];
    char trans_tp	[4+1];
    char trans_state	[1+1];
    char acct_type	[2+1];
    char pan	[20+1];
    char ccy  [3+1];
    int seq_num;
}ht_tbl_cib_txn_def;

typedef struct
{
    char payway_code	[32+1];
    char payway_name	[64+1];
    char connect_switch	[8+1];    
    char sett_type[2+1];
    char estatus	[8+1];
    double cost_rate;
    char open_type   [2+1];
    char sett_md	[2+1];
    char plat_sett_bank	[11+1];
    char plat_sett_acct_type	[1+1];
    char plat_sett_acct         [40+1];
    char plat_sett_acct_nm      [80+1]; 
    char plat_debit_bank    [11+1];
    char plat_debit_acct_type    [1+1];
    char plat_debit_acct      [40+1];
    char plat_debit_acct_nm	[80+1];
    char brh_role	[1+1];
    char pay_chan	[4+1];
}ht_tbl_payway_ext_def;

typedef struct
{
char id  [50 + 1];
int version;
char create_time [8 + 1];
char update_time [8 + 1];
char creater [32 + 1];
char editor [32 + 1];
char merc_type [32 + 1];
char up_mcht_no [15 + 1];
char merc_no [32 + 1];
char merc_name [64 + 1];
char estatus [8 + 1];
char private_key  	 [512 + 1];
char public_key 	 [512 + 1];
char key_type  	 [8 + 1];
char return_url 	 [255 + 1];
char notify_url 	 [255 + 1];
char org_code  	 [32 + 1];
char linkman 	 [32 + 1];
char telphone  	 [32 + 1];
char email  	 [128 + 1];
char address 	 [128 + 1];
char passwd 	 [128 + 1];
char settle_bank_code	 [32 + 1];
char settle_account  	 [32 + 1];
char bank_account 	 [32 + 1];
char merc_busi_lic 	 [40 + 1];
char lic_vali_date[8 + 1];
char busi_lic_file_name [60 + 1];
char busi_lic_path	 [200 + 1];
char merc_org_code[40 + 1];
char org_code_file_name [60 + 1];
char org_code_path	 [200 + 1];
char merc_tax_code 	 [40 + 1];
char tax_code_file_name [60 + 1];
char tax_code_path	 [200 + 1];
char  default_flag	 [1 + 1];
char corp_rept_name  	 [100 + 1];
char corp_rept_id 	 [24 + 1];
char post_code 	 [6 + 1];
char fixed_phone  	 [32 + 1];
char corp_addr 	 [200 + 1];
char shop_name 	 [200 + 1];
char shop_addr 	 [200 + 1];
char  sub_acc_type 	 [4 + 1];
char  open_type 	 [2 + 1];
char  open_chn 	 [2 + 1];
char open_creator 	  [100 + 1];
char  open_time 	  [18 + 1];
char  open_sts 	 [2 + 1];
char  res_code 	 [4 + 1];
char res_msg  	  [40 + 1];
char res_sys_seq 	  [20 + 1];
char merc_public_key	  [512 + 1];
char ori_passwd  	  [128 + 1];
char  passwd_flag 	 [1 + 1];
int wrong_counts ;
char locked_time [6 + 1];
char send_bill_flag 	  [3 + 1];
char settle_type  	 [8 + 1];
char businuss_brh_no	  [11 + 1];
char mcht_pay_bank	 [11 + 1];
char mcht_pay_acct_type [1 + 1];
char mcht_pay_acct	 [40 + 1];
char mcht_pay_acct_nm	 [80 + 1];
char mcht_sett_bank	  [11 + 1];
char mcht_sett_acct_type	 [1 + 1];
char mcht_sett_acct	  [40 + 1];
char mcht_sett_acct_nm	 [80 + 1];
char xingmin_pay_acct	 [40 + 1];
char  js_type	 [2 + 1];
char  clear_cycle	  [3 + 1];
char clear_date	 [8 + 1];
char sett_way	  [40 + 1];
char  splitting_type	 [2 + 1];
char  spl_brh_role1	 [12 + 1];
char spl_brh_nm1	 [30 + 1];
char  spl_acct_type1	 [1 + 1];
char spl_acct_no1	 [40 + 1];
char spl_acct_nm1	 [80 + 1];
char spl_acct_bank1	  [11 + 1];
char spl_acct_banknm1 [80 + 1];
double spl_acct_rate1;
char  spl_brh_role2	 [12 + 1];
char spl_brh_nm2	 [30 + 1];
char  spl_acct_type2	 [1 + 1];
char spl_acct_no2	 [40 + 1];
char spl_acct_nm2	 [80 + 1];
char spl_acct_bank2	  [11 + 1];
char spl_acct_banknm2	 [80 + 1];
double spl_acct_rate2;
char  spl_brh_role3	 [12 + 1];
char spl_brh_nm3	 [30 + 1];
char  spl_acct_type3	 [1 + 1];
char spl_acct_no3	 [40 + 1];
char spl_acct_nm3	 [80 + 1];
char spl_acct_bank3	  [11 + 1];
char spl_acct_banknm3	 [80 + 1];
double spl_acct_rate3;
char  spl_brh_role4	 [12 + 1];
char spl_brh_nm4	 [30 + 1];
char  spl_acct_type4	 [1 + 1];
char spl_acct_no4	 [40 + 1];
char spl_acct_nm4	 [80 + 1];
char spl_acct_bank4	  [11 + 1];
char spl_acct_banknm4	 [80 + 1];
double  spl_acct_rate4;
char bundle_return_url	 [255 + 1];
char bundle_notify_url  [255 + 1];
}tp_merchant_ext_def;

typedef struct
{
char brh_no   [12 + 1];                     
char  brh_nm  [30 + 1];                     
char brh_role [2 + 1];                      
char pay_brh [2 + 1];                       
char  brh_acct_type [2 + 1];                
char   brh_acct_no  [40 + 1];               
char   brh_acct_nm  [80 + 1];               
char brh_acct_bank  [12 + 1];               
char  brh_acct_banknm  [80 + 1];            
char contact  [30 + 1];                     
char comm_mobil  [18 + 1];                  
char   comm_tel  [18 + 1];                  
char  email   [20 + 1];                     
char manager  [32 + 1];                     
char   identity_no  [20 + 1];               
char   manager_tel  [12 + 1];               
char   reg_addr  [60 + 1];                  
char  plat_debit_bank  [12 + 1];            
char plat_debit_acct_type [2 + 1];          
char  plat_debit_acct  [40 + 1];            
char  plat_debit_acct_nm  [80 + 1];         
char rec_upd_usr_id [10 + 1];               
char  rec_upd_ts [14 + 1];                  
char  rec_crt_ts [14 + 1];                  
}ht_tbl_brh_inf_def;                        

typedef struct
{
        char payway_code         [50 + 1];
		char ext_mch_no     [15 + 1];
        char tran_code      [4 + 1];
        char card_type      [4 + 1];
        double  floor_amount      ;
        double  upper_amount     ;
        char  fee_flag   [1 + 1];
        double  fee_value         ;
        double fee_min    ; 
        double fee_max    ;
        char  rec_upd_usr_id  [10 + 1];
        char rec_upd_ts  [14 + 1]; 
        char rec_crt_ts  [14 + 1];
}ht_tbl_brh_fee_inf_def;


typedef struct
{
      char id[50+1];
      char payway_code[32+1];
      char cert_key_type[1+1];
      char key[128+1];
      char value[4000+1];
      char title[256+1];
      char demo[600+1];
}tp_cert_key_def;


typedef struct
{
    char bank_card_bin[32+1];
    char bank_card_name[128+1];
    char card_length[3+1];
    char bank_code[32+1];
    char bank_name[128+1]; 
    char account_type[32+1];
    char field1[32+1];
    char field2[32+1];
    char field3[32+1];
}tp_bankcards_bin_def;

typedef struct
{
char    ext_mch_no[15+1];
char    ext_mch_nm[30+1];                      
char    brh_no[12+1];                          
char    brh_nm[30+1];
char    settle_type     [8+1];                 
char    ext_mch_acct_type[1+1];                
char    ext_mch_acct_no[40+1];                 
char    ext_mch_acct_nm[80+1];                 
char    ext_mch_acct_bank[12+1];               
char    ext_mch_acct_banknm[80+1];             
char    ext_fee_acct_type[1+1];                
char    ext_fee_acct_no[40+1];                 
char    ext_fee_acct_nm[80+1];                 
char    ext_fee_acct_bank[12+1];               
char    ext_feeacct_banknm[80+1];              
char    spl_acct_type[1+1];                    
 char   spl_bank[12+1];                        
char    spl_acct[40+1];                        
char    spl_acct_nm[80+1];                     
char    contact[30+1];                         
char    comm_mobil[18+1];                      
char    comm_tel[18+1];                        
char    email[20+1];                           
char    manager[10+1];                         
char    identity_no[20+1];                     
char    manager_tel[12+1];                     
char    reg_addr[60+1];                        
char    rec_upd_usr_id[10+1];                  
char    rec_upd_ts[14+1];                      
char    rec_crt_ts[14+1];   
}ht_tbl_ext_mcht_inf_def;


typedef struct
{
    char     key1[10 + 1];
    char     key2[10 + 1];
    char     key3[10 + 1];
    char     value1[60 + 1];
    char     value2[60 + 1];
    char     value3[60 + 1];
    char     remark1[120 + 1];
    char     remark2[120 + 1];
    char     remark3[120 + 1];
} ht_tbl_com_config_def;

typedef struct
{
	char  plat_key                 [48 + 1];
	char sett_date                 [8 + 1];
	char  trans_tp                 [4 + 1];
	double   trans_amt          ;
	char      fee_type             [1 + 1];
	double    fee_rate            ;
	double   plat_cost            ;
	double   file_cost             ;
	char      is_equal             [1 + 1];
	char     cost_type           [1 + 1];
	char rec_crt_ts               [14 + 1];
}ht_tbl_cost_cmp_inf_def;

typedef struct
{
	char	channel_code[32];
char	channel_name[60];
char	external_org_no[32];
char	payway_code[32];
char	status[2];
char	sett_md[2];
char	plat_sett_bank[12];
char	plat_sett_acct_type[1];
char	plat_sett_acct[40];
char	plat_sett_acct_nm[80];
char	plat_debit_bank[12];
char	plat_debit_acct_type[1];
char	plat_debit_acct[40];
char	plat_debit_acct_nm[80];
char	brh_role[2];
char	pay_chan[4];
char	connect_switch[8];
char	sett_type[2];
char	compt_fee_flag[2];
char	create_time[32];
char	service_bean_id[64];
char	creater[40];
char	update_time[32];
char	updater[40];
char	brh_no[12];
char	brm_nm[30];
}tp_payway_ext_def;

typedef struct
{
    char    id             [20+ 1];                           
    char    opr_code       [20+ 1];                           
    char    opr_name       [100+ 1];                          
    char    opr_type       [4+ 1];                            
    char    bk             [12+ 1];                           
    char    br             [12+ 1];                           
    char    opr_acc_no     [40+ 1];                           
    char    opr_acc_name   [300+ 1];                          
    char    core_acc_type  [4+ 1];                            
    char    opr_acc_status [2+ 1];                            
    char    opr_acc_type   [2+ 1];                            
    char    ccy            [3+ 1];                            
    char    platform_id    [20+ 1];                           
    char    system_id      [20+ 1];                           
    char    createor       [100+ 1];                          
    char    create_time    [6+ 1];                            
    char    updater        [100+ 1];                          
    char    update_time    [6+ 1];                            
    char    userid         [40+ 1];                           
    char    remark         [300+ 1];                          
    char    rs_fld1        [40+ 1];                           
    char    rs_fld2        [100+ 1];                          
    char    rs_fld3        [40+ 1];                           
    char    rs_fld4        [100+ 1];                          
    char    flag           [4+ 1];                            
    char    open_sts       [2+ 1];                            
    char    res_code       [4+ 1];                            
    char    res_msg        [40+ 1];                           
    char    res_sys_seq    [20+ 1];   
}tp_operater_acc_def;


typedef struct
{
    char    payway_code             [32+ 1];                           
    char    paying_merc_code        [32+ 1];       
}tp_merc_paywayconf_def;

typedef struct
{
    char    sMrchNo[32+1];
    char    sKey[60+1];
	char    sMrchCert[100+1];
} stKeyInf;

 typedef struct
 {
	char term_no[32 + 1];
    char term_name[32 + 1];
    char template_no[10 + 1];
    char mob_no_country_code[10 + 1];
	char mobile_no[20 + 1];
	char sms_content[512 + 1];
	char batch_no[32 + 1];
	char status[2 + 1];
} tb_sms_send_queue_def;

 typedef struct
 {
     char name[32];
	 char mobile[32];
 } tb_sms_contact_def;

typedef struct
{
     char    pay_way[32 + 1];
	 char    merc_type[32 + 1];
     char    paying_merc_code[32 + 1];
	 char    key[128 + 1];
     char    merc_cert[128 + 1];
} tbl_key_cert_def;

typedef struct
{
    char inter_brh_code   [10 + 1];           
    char sett_date        	   [8 + 1];        
    char trans_date       	   [8 + 1];       
    char trans_time        [10 + 1];           
    char flag_result      	 [1 + 1];         
    char gener_id         	     [20 + 1];  
    char thr_mcht_no         	     [32 + 1];        
    char ext_mcht_no      	     [32 + 1];     
    char ex_ext_mcht_no      [32 + 1];         
    char equipment_id            [100 + 1];    
    char thr_order_no         	     [48 + 1];     
    char order_no         	   [48 + 1];      
    char user_id           [30 + 1];          
    char plat_key          [48 + 1];          
    char batch_no            [2 + 1];    
    char bus_tp         	 [8 + 1];       
    char trans_tp         	 [4 + 1];  
    char thr_trans_tp         	 [48 + 1];          
    char trans_state      	   [30 + 1];        
    char pay_bank                [15 + 1];         
    char mcht_refund_order     [32 + 1];      
    char refund_type      	   [10 + 1];       
    char refund_status    	   [40 + 1];       
    char goods_name       	   [400 + 1];       
    char mcht_data        	   [500 + 1];      
    char wx_fee                 [12 + 1];     
    char wx_rate            [10 + 1];         
    char acct_type        [2 + 1];            
    char pan              [19 + 1];           
    char ccy              [3 + 1];            
    char trans_at         [12 + 1];           
    char trans_at2        [12 + 1];           
    char trans_at3        [12 + 1];           
    char trans_at4        [12 + 1];           
    char trans_at5        [12 + 1]; 
    char trans_at6        [12 + 1];        
    int  seq_num;                                       
}tbl_alipay_wechat_def;

#endif

