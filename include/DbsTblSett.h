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


/* ������ϸ��ṹ */
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

/* ͷ����Ϣ��ṹ */
typedef struct
{
    char        ext_mcht_no[15+1];              /*�ⲿ�̻����*/
    char        bank_code[12+1];                /*��������*/
    char        bank_name[70+1];                /*������������*/
    char        acct_no[22+1];                  /*�������˺�*/
    char        acct_name[70+1];                /*�����˻�����*/
    double      interest;                       /*����*/
    char        sett_date[8+1];                 /*��������*/
    double      bal;                            /*����ǰ�˻����*/
    double      acct_bal;                       /*�������˻����*/
    double      resv_bal;                       /*�������*/
    double      avai_bal;                       /*�������*/
    double      block_bal;                      /*�������*/
    double      ctrl_bal;                       /*�������*/
    char        sync_time[14+1];                /*����ʱ��*/
} tbl_postion_info_def;

/* ͷ�������ṹ */
typedef struct
{
    char        bank_code[12+1];                /*��������*/
    char        bank_name[70+1];                /*������������*/
    char        acct_no[22+1];                  /*�������˺�*/
    char        acct_name[70+1];                /*�����˻�����*/
    double      interest;                       /*����*/
    char        sett_date[8+1];                 /*��������*/
    double      bal;                            /*�˻����*/
    double      resv_bal;                       /*�������*/
    double      bal_trial;                      /*�������*/
    char        seq_no[20+1];                   /*������ˮ*/
    char        sync_time[14+1];                /*����ʱ��*/
} tbl_postion_trial_def;


/* �鼯��ϸ��ṹ */
typedef struct
{
    char        brh_id[10+1];                   /*�ڲ�������*/
    char        sett_date[8+1];                 /*��������*/
    char        trans_date[8+1];                /*��������*/
    char        seq_no[20+1];                   /*�鼯��ˮ*/
    char        bank_code[12+1];               /*�����𿪻���*/
    char        acct_type[8+1];                 /*�����˻�����*/
    char        acct_no[22+1];                  /*�����˻��˺�*/
    char        acct_name[70+1];                /*ƽ̨�����˻�����*/
    char        dc_flag[1+1];                   /*֧��,����*/
    char        bank_code1[12+1];               /*�����𿪻���*/
    char        acct_type1[8+1];                /*���ֽ����˻�����*/
    char        acct_no1[22+1];                 /*���ֽ����˻��˺�*/
    char        acct_name1[70+1];               /*���ֽ����˻�����*/
    double      amt;                            /*���˽��*/
    char        status[1+1];                    /*����״̬*/
    char        file_name[100+1];               /*�����ļ���*/
    char        chnl_id[8+1];                   /*��������*/
    char        ps[80+1];                       /*����*/
} tbl_mission_info_def;

/* ������Ϣ��(������Ϣ��)�ṹ */
typedef struct
{
    char        chnl_id[10+1];                  /*�������*/
    char        chnl_name[70+1];                 /*��������*/
    char        status[8+1];                    /*����״̬*/
    double      min_amt;                        /*�������*/
    double      max_amt;                        /*�����޶�*/
    double      bal;                            /*�������*/
    char        index_day[6+1];                 /*�����չ���*/
    char        index_time[6+1];                /*ʱ�����*/
    char        period[8+1];                    /*֧������*/
    char        index_fee[6+1];                 /*���ʱ���*/
    char        shadom[22+1];                    /*Ӱ���˻�*/
} tbl_sett_chan_def;


/* �ļ���Ϣ��ṹ */
typedef struct
{
    char        file_name[100+1];               /*�ļ���*/
    char        bank_code[12+1];                /*ƽ̨���б��*/
    char        sett_date[8+1];                 /*��������*/
    char        step_no[2+1];                   /*�����*/
    char        batch_no[2+1];                  /*���κ�*/
    char        file_type[1+1];                 /*�ļ�����*/
    char        file_flag[1+1];                 /*�ļ�����*/
    char        end_flag[1+1];                  /*�����ļ�������־*/
    char        snd_time[14+1];                 /*����ʱ��*/
    char        rcv_time[14+1];                 /*����ʱ��*/
    char        status[1+1];                    /*�ļ�����״̬*/
} tbl_file_info_def;

/* ͳ�ƴ��鼯��¼�ṹ�� */
typedef struct
{
    char        sett_date[8+1];                 /*��������*/
    char        status[1+1];                    /*����״̬*/
    char        bank_no[12+1];                  /*��������*/
    char        acct_type[1+1];                 /*ƽ̨�����˻�����*/
    char        acct_no[22+1];                  /*ƽ̨�����˻��˺�*/
    char        acct_name[70+1];                /*ƽ̨�����˻�����*/
    double      total_amt;                      /*�ܽ��*/
    int         total_num;                      /*�ܱ���*/
} sett_miss_info_def;

/* ������Ӧ�ļ��ṹ�� */
typedef struct
{
    char        tran_date[8+1];                 /*��������*/
    char        tran_time[6+1];                 /*����ʱ��*/
    char        seq_no[20+1];                   /*������ˮ*/
    char        tran_no[10+1];                  /*������ˮ*/
    char        tran_type[1+1];                 /*��������*/
    char        tran_status[1+1];               /*����״̬*/
    char        acct_type[2+1];                 /*�˻�����*/
    char        bank_code[12+1];                /*���б��*/
    char        acct_no[22+1];                  /*�˻��˺�*/
    char        tran_amt[12+1];                 /*���׽��*/
    char        tran_amt2[12+1];                /*���׽��2*/
    char        tran_amt3[12+1];                /*���׽��3*/
    char        tran_amt4[12+1];                /*���׽��4*/
    char        tran_amt5[12+1];                /*���׽��5*/
    char        reserve1[10+1];                 /*����1*/
    char        reserve2[20+1];                 /*����2*/
    char        reserve3[20+1];                 /*����3*/
    char        reserve4[50+1];                 /*����4*/
} resp_file_info_def;

/* ��ҵ������Ӧ�ļ��ṹ�� */
typedef struct
{
    char        seq_no[32+1];                   /*������ˮ�򶩵���*/
    char        mcht_no[60+1];                  /*�����̻�*/
    char        tran_date[12+1];                 /*��������*/
    char        tran_time[6+1];                 /*����ʱ��*/
    char        pay_acct_no[22+1];              /*ת���˻�*/
    char        pay_acct_name[70+1];            /*ת���˻�����*/
    char        acct_no[22+1];                  /*ת���˻�*/
    char        acct_name[70+1];                /*ת���˻�����*/
    char        acct_type[30+1];                /*ת���˻�����*/
    char        tran_amt[15+1];                 /*���׽��*/
    char        function[200+1];                /*��;*/
    char        tran_fee[15+1];                 /*������*/
    char        tran_status[1+1];               /*����״̬*/
    char        reserve[200+1];                 /*����*/
} cib_rsp_info_def;

/* ���˽����ṹ */
typedef struct
{
    char        inter_brh_code[10+1];           /*�ڲ�������*/
    char        sett_date[8+1];                 /*��������*/
    char        acct_date[8+1];                 /*��������*/
    char        batch_no[2+1];                  /*ƽ̨���κ�*/
    char        mcht_rslt_no[25+1];             /*�̻�������ˮ��*/
    char        mcht_no[15+1];                  /*�̻���*/
    char        mcht_role[1+1];                 /*�̻���ɫ*/
    int         trans_num;                      /*���ױ���*/
    double      debt_at;                        /*���˽���*/
    double      cret_at;                        /*���˽���*/
    double      debt_net_fee;                   /*�����Ѿ����*/
    double      cret_net_fee;                   /*�����Ѿ����*/
    char        sett_md[2+1];                   /*�̻���֧ģʽ*/
    char        mcht_sett_bank[11+1];           /*�̻�������*/
    char        mcht_sett_bank_nm[80+1];        /*�̻�����������*/
    char        mcht_sett_acct_type[1+1];       /*�̻������˻�����*/
    char        mcht_sett_acct[40+1];           /*�̻������˻��˺�*/
    char        mcht_sett_acct_nm[80+1];        /*�̻������˻�����*/
    char        plat_sett_bank[11+1];           /*ƽ̨������*/
    char        plat_sett_bank_nm[80+1];        /*ƽ̨����������*/
    char        plat_sett_acct_type[1+1];       /*ƽ̨�����˻�����*/
    char        plat_sett_acct[40+1];           /*ƽ̨�����˻��˺�*/
    char        plat_sett_acct_nm[80+1];        /*ƽ̨�����˻�����*/
    char        cret_chnl_id[10+1];             /*���뻮���������*/
    char        cret_chnl_nm[80+1];             /*���뻮����������*/
    char        cret_rslt_no[20+1];             /*���뻮����ˮ��*/
    char        mcht_debit_bank[11+1];          /*�̻�֧����*/
    char        mcht_debit_bank_nm[80+1];       /*�̻�֧��������*/
    char        mcht_debit_acct_type[1+1];      /*�̻�֧���˻�����*/
    char        mcht_debit_acct[40+1];          /*�̻�֧���˻��˺�*/
    char        mcht_debit_acct_nm[80+1];      /*�̻�֧���˻�����*/
    char        plat_debit_bank[11+1];          /*ƽ̨������*/
    char        plat_debit_bank_nm[80+1];       /*ƽ̨����������*/
    char        plat_debit_acct_type[1+1];      /*ƽ̨�����˻�����*/
    char        plat_debit_acct[40+1];          /*ƽ̨�����˻��˺�*/
    char        plat_debit_acct_nm[80+1];       /*ƽ̨�����˻�����*/
    char        debt_chnl_id[10+1];             /*֧�������������*/
    char        debt_chnl_nm[80+1];             /*֧��������������*/
    char        debt_rslt_no[20+1];             /*֧��������ˮ��*/
    char        acct_statu[1+1];                /*����״̬*/
    char        sz_status[1+1];                 /*��֧״̬*/
    int        seq_num;                         /*���*/
} tbl_acct_rslt_def;

typedef struct
{
    char        inter_brh_code[10+1];           /*�ڲ�������*/
    char        sett_date[8+1];                 /*��������*/
    char        mcht_rslt_no[25+1];             /*�̻�������ˮ��*/
    char        trans_num[12+1];                /*���ױ���*/
    double      debt_at;                        /*���˽���*/
    double      cret_at;                        /*���˽���*/
    double      debt_net_fee;                   /*�����Ѿ����*/
    double      cret_net_fee;                   /*�����Ѿ����*/
    char        sett_md[2+1];                   /*�̻���֧ģʽ*/
    char        mcht_sett_bank[12+1];           /*�̻�������*/
    char        mcht_sett_bank_nm[80+1];        /*�̻�����������*/
    char        mcht_sett_acct_type[1+1];       /*�̻������˻�����*/
    char        mcht_sett_acct[40+1];           /*�̻������˻��˺�*/
    char        mcht_sett_acct_nm[80+1];        /*�̻������˻�����*/
    char        plat_sett_bank[12+1];           /*ƽ̨������*/
    char        plat_sett_bank_nm[80+1];        /*ƽ̨����������*/
    char        plat_sett_acct_type[1+1];       /*ƽ̨�����˻�����*/
    char        plat_sett_acct[40+1];           /*ƽ̨�����˻��˺�*/
    char        plat_sett_acct_nm[80+1];        /*ƽ̨�����˻�����*/
    int         seq_num;                        /*���*/
} acct_rslt_cinfo_def;

typedef struct
{
    char        bank_code[12+1];                /*ƽ̨������*/
    char        acct_no[22+1];                  /*ƽ̨�����˻��˺�*/
    char        acct_name[70+1];                /*ƽ̨�����˻�����*/
    double      shrt_amt;                       /*ȱ�ڽ��*/
    char        sett_date[8+1];                 /*��������*/
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
