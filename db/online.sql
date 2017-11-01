----------------------------------------------------
-- Export file for user SETTLEMENT                --
-- Created by Administrator on 2017/9/7, 16:14:29 --
----------------------------------------------------





create table HT_DICTIONARY
(
  AUTO_ID         INTEGER not null,
  DICTIONARY_ID   VARCHAR2(32) not null,
  TYPE            INTEGER,
  DICTIONARY_NAME VARCHAR2(32),
  REMARK          VARCHAR2(100)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on table HT_DICTIONARY
  is '�ֵ��';
comment on column HT_DICTIONARY.AUTO_ID
  is '����';
comment on column HT_DICTIONARY.DICTIONARY_ID
  is 'id';
comment on column HT_DICTIONARY.TYPE
  is '����id';
comment on column HT_DICTIONARY.DICTIONARY_NAME
  is '�ֵ�����';
comment on column HT_DICTIONARY.REMARK
  is '��ע���������ƣ�';





create table HT_DOWNLAND_RECORD
(
  ID              NUMBER(20) not null,
  SHEET_NAME      VARCHAR2(100),
  FLAG            VARCHAR2(1),
  OPER_START_TIME VARCHAR2(20),
  OPER_END_TIME   VARCHAR2(20),
  USER_NAME       VARCHAR2(20),
  TABLE_NAME_C    VARCHAR2(100),
  TABLE_NAME_E    VARCHAR2(40),
  DOWN_LOAD_TYPE  VARCHAR2(5),
  DATE_START      VARCHAR2(20),
  DATE_END        VARCHAR2(20),
  QUERY_ID        VARCHAR2(20),
  QUERY_NAME      VARCHAR2(20),
  DELETE_FLAG     VARCHAR2(1),
  EXPAND_1        VARCHAR2(100),
  EXPAND_2        VARCHAR2(100),
  EXPAND_3        VARCHAR2(100),
  EXPAND_4        VARCHAR2(100),
  EXPAND_5        VARCHAR2(100),
  ZHONG_DUAN_HAO  VARCHAR2(50)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on table HT_DOWNLAND_RECORD
  is ' ���ؽ��ȱ�';
comment on column HT_DOWNLAND_RECORD.ID
  is 'id';
comment on column HT_DOWNLAND_RECORD.SHEET_NAME
  is '���ر�����';
comment on column HT_DOWNLAND_RECORD.FLAG
  is '�ɹ���ʾ';
comment on column HT_DOWNLAND_RECORD.OPER_START_TIME
  is '������ʼʱ��';
comment on column HT_DOWNLAND_RECORD.OPER_END_TIME
  is '��������ʱ��';
comment on column HT_DOWNLAND_RECORD.USER_NAME
  is '�û��˺�';
comment on column HT_DOWNLAND_RECORD.TABLE_NAME_C
  is '���ı���';
comment on column HT_DOWNLAND_RECORD.TABLE_NAME_E
  is 'Ӣ�ı���';
comment on column HT_DOWNLAND_RECORD.DOWN_LOAD_TYPE
  is '���ظ�ʽ';
comment on column HT_DOWNLAND_RECORD.DATE_START
  is '��ʼ/��������';
comment on column HT_DOWNLAND_RECORD.DATE_END
  is '��������';
comment on column HT_DOWNLAND_RECORD.QUERY_ID
  is '���';
comment on column HT_DOWNLAND_RECORD.QUERY_NAME
  is '����';
comment on column HT_DOWNLAND_RECORD.DELETE_FLAG
  is '�Ƿ�ɾ��';
comment on column HT_DOWNLAND_RECORD.EXPAND_1
  is '���ı���id�����ֵ��';
comment on column HT_DOWNLAND_RECORD.EXPAND_2
  is '�����ֶ�2';
comment on column HT_DOWNLAND_RECORD.EXPAND_3
  is '�����ֶ�3';
comment on column HT_DOWNLAND_RECORD.EXPAND_4
  is '�����ֶ�4';
comment on column HT_DOWNLAND_RECORD.EXPAND_5
  is '�����ֶ�5';
comment on column HT_DOWNLAND_RECORD.ZHONG_DUAN_HAO
  is '�ն˺�';






create table HT_OPER_RECORD
(
  ID        NUMBER(20) not null,
  USER_ID   VARCHAR2(30),
  USER_NAME VARCHAR2(60),
  IP        VARCHAR2(60),
  OPER_TIME VARCHAR2(16),
  CONTENT   VARCHAR2(200),
  EXPAND_1  VARCHAR2(100),
  EXPAND_2  VARCHAR2(100),
  EXPAND_3  VARCHAR2(100),
  EXPAND_4  VARCHAR2(100),
  EXPAND_5  VARCHAR2(100)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on table HT_OPER_RECORD
  is ' ������¼��';
comment on column HT_OPER_RECORD.ID
  is 'id';
comment on column HT_OPER_RECORD.USER_ID
  is '�ͻ�ID';
comment on column HT_OPER_RECORD.USER_NAME
  is '�ͻ�����';
comment on column HT_OPER_RECORD.IP
  is '����ͻ���ip';
comment on column HT_OPER_RECORD.OPER_TIME
  is '����ʱ��';
comment on column HT_OPER_RECORD.CONTENT
  is '��������';
comment on column HT_OPER_RECORD.EXPAND_1
  is '�����ֶ�1';
comment on column HT_OPER_RECORD.EXPAND_2
  is '�����ֶ�2';
comment on column HT_OPER_RECORD.EXPAND_3
  is '�����ֶ�3';
comment on column HT_OPER_RECORD.EXPAND_4
  is '�����ֶ�4';
comment on column HT_OPER_RECORD.EXPAND_5
  is '�����ֶ�5';






create table HT_TBL_ACCT_INF
(
  TYPE               VARCHAR2(4) not null,
  BANK_CODE          VARCHAR2(20),
  ACCT_TYPE          VARCHAR2(20),
  ACCT_NO            VARCHAR2(40),
  ACCT_NAME          VARCHAR2(60),
  ACCOUNT_BALANCE    NUMBER(12,2),
  AVAILABLE_BALANCE  NUMBER(12,2),
  BLOCKED_BALANCE    NUMBER(12,2),
  CONTROLLED_BALANCE NUMBER(12,2),
  REC_CRT_TS         VARCHAR2(14),
  REC_UPD_TS         VARCHAR2(14)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_ACCT_INF.TYPE
  is '�˺�id';
comment on column HT_TBL_ACCT_INF.BANK_CODE
  is '�к�';
comment on column HT_TBL_ACCT_INF.ACCT_TYPE
  is '�˺�����';
comment on column HT_TBL_ACCT_INF.ACCT_NO
  is '�˺�';
comment on column HT_TBL_ACCT_INF.ACCT_NAME
  is '�˺�����';
comment on column HT_TBL_ACCT_INF.ACCOUNT_BALANCE
  is '�˻����';
comment on column HT_TBL_ACCT_INF.AVAILABLE_BALANCE
  is '�������';
comment on column HT_TBL_ACCT_INF.BLOCKED_BALANCE
  is '�������';
comment on column HT_TBL_ACCT_INF.CONTROLLED_BALANCE
  is '�������';
comment on column HT_TBL_ACCT_INF.REC_CRT_TS
  is '���´���ʱ��';
comment on column HT_TBL_ACCT_INF.REC_UPD_TS
  is '���¸���ʱ��';





create table HT_TBL_ACCT_RSLT
(
  INTER_BRH_CODE          CHAR(10) not null,
  SETT_DATE               VARCHAR2(8) not null,
  ACCT_DATE               VARCHAR2(8),
  BATCH_NO                VARCHAR2(2),
  MCHT_RSLT_NO            VARCHAR2(32),
  MCHT_NO                 VARCHAR2(15),
  MCHT_ROLE               CHAR(4),
  TRANS_NUM               NUMBER(12),
  DEBT_AT                 NUMBER(12,2),
  CRET_AT                 NUMBER(12,2),
  DEBT_NET_FEE            NUMBER(12,2),
  CRET_NET_FEE            NUMBER(12,2),
  SETT_MD                 VARCHAR2(2),
  MCHT_SETT_BANK          VARCHAR2(12),
  MCHT_SETT_BANK_NM       VARCHAR2(80),
  MCHT_SETT_ACCT_TYPE     CHAR(1),
  MCHT_SETT_ACCT          VARCHAR2(40),
  MCHT_SETT_ACCT_NM       VARCHAR2(80),
  MCHT_SETT_XINGMIN_ACCT  VARCHAR2(40),
  PLAT_SETT_BANK          VARCHAR2(12),
  PLAT_SETT_BANK_NM       VARCHAR2(80),
  PLAT_SETT_ACCT_TYPE     CHAR(1),
  PLAT_SETT_ACCT          VARCHAR2(40),
  PLAT_SETT_ACCT_NM       VARCHAR2(80),
  PLAT_SETT_XINGMIN_ACCT  VARCHAR2(40),
  CRET_CHNL_ID            VARCHAR2(10),
  CRET_CHNL_NM            VARCHAR2(80),
  CRET_RSLT_NO            VARCHAR2(20),
  MCHT_DEBIT_BANK         VARCHAR2(12),
  MCHT_DEBIT_BANK_NM      VARCHAR2(80),
  MCHT_DEBIT_ACCT_TYPE    CHAR(1),
  MCHT_DEBIT_ACCT         VARCHAR2(40),
  MCHT_DEBIT_ACCT_NM      VARCHAR2(80),
  PLAT_DEBIT_BANK         VARCHAR2(12),
  PLAT_DEBIT_BANK_NM      VARCHAR2(80),
  PLAT_DEBIT_ACCT_TYPE    CHAR(1),
  PLAT_DEBIT_ACCT         VARCHAR2(40),
  PLAT_DEBIT_ACCT_NM      VARCHAR2(80),
  PLAT_DEBIT_XINGMIN_ACCT VARCHAR2(40),
  DEBT_CHNL_ID            VARCHAR2(10),
  DEBT_CHNL_NM            VARCHAR2(80),
  DEBT_RSLT_NO            VARCHAR2(20),
  ACCT_STATU              CHAR(1),
  SZ_STATUS               CHAR(1) default 'I',
  SEQ_NUM                 NUMBER(10) not null
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_ACCT_RSLT.INTER_BRH_CODE
  is '�ڲ�������';
comment on column HT_TBL_ACCT_RSLT.SETT_DATE
  is '��������';
comment on column HT_TBL_ACCT_RSLT.ACCT_DATE
  is '��������';
comment on column HT_TBL_ACCT_RSLT.BATCH_NO
  is 'ƽ̨���κ�';
comment on column HT_TBL_ACCT_RSLT.MCHT_RSLT_NO
  is '�̻�������ˮ��';
comment on column HT_TBL_ACCT_RSLT.MCHT_NO
  is '�̻���';
comment on column HT_TBL_ACCT_RSLT.MCHT_ROLE
  is '�̻���ɫ';
comment on column HT_TBL_ACCT_RSLT.TRANS_NUM
  is '���ױ���';
comment on column HT_TBL_ACCT_RSLT.DEBT_AT
  is '���˽���';
comment on column HT_TBL_ACCT_RSLT.CRET_AT
  is '���˽���';
comment on column HT_TBL_ACCT_RSLT.DEBT_NET_FEE
  is '�����Ѿ����';
comment on column HT_TBL_ACCT_RSLT.CRET_NET_FEE
  is '�����Ѿ����';
comment on column HT_TBL_ACCT_RSLT.SETT_MD
  is '�̻���֧ģʽ';
comment on column HT_TBL_ACCT_RSLT.MCHT_SETT_BANK
  is '�̻�������';
comment on column HT_TBL_ACCT_RSLT.MCHT_SETT_BANK_NM
  is '�̻�����������';
comment on column HT_TBL_ACCT_RSLT.MCHT_SETT_ACCT_TYPE
  is '�̻������˻�����';
comment on column HT_TBL_ACCT_RSLT.MCHT_SETT_ACCT
  is '�̻������˻��˺�';
comment on column HT_TBL_ACCT_RSLT.MCHT_SETT_ACCT_NM
  is '�̻������˻�����';
comment on column HT_TBL_ACCT_RSLT.MCHT_SETT_XINGMIN_ACCT
  is '�̻������׸��˻�';
comment on column HT_TBL_ACCT_RSLT.PLAT_SETT_BANK
  is 'ƽ̨������';
comment on column HT_TBL_ACCT_RSLT.PLAT_SETT_BANK_NM
  is 'ƽ̨����������';
comment on column HT_TBL_ACCT_RSLT.PLAT_SETT_ACCT_TYPE
  is 'ƽ̨�����˻�����';
comment on column HT_TBL_ACCT_RSLT.PLAT_SETT_ACCT
  is 'ƽ̨�����˻��˺�';
comment on column HT_TBL_ACCT_RSLT.PLAT_SETT_ACCT_NM
  is 'ƽ̨�����˻�����';
comment on column HT_TBL_ACCT_RSLT.PLAT_SETT_XINGMIN_ACCT
  is 'ƽ̨���������׸��˻�';
comment on column HT_TBL_ACCT_RSLT.CRET_CHNL_ID
  is '���뻮���������';
comment on column HT_TBL_ACCT_RSLT.CRET_CHNL_NM
  is '���뻮����������';
comment on column HT_TBL_ACCT_RSLT.CRET_RSLT_NO
  is '���뻮����ˮ��';
comment on column HT_TBL_ACCT_RSLT.MCHT_DEBIT_BANK
  is '�̻�֧����';
comment on column HT_TBL_ACCT_RSLT.MCHT_DEBIT_BANK_NM
  is '�̻�֧��������';
comment on column HT_TBL_ACCT_RSLT.MCHT_DEBIT_ACCT_TYPE
  is '�̻�֧���˻�����';
comment on column HT_TBL_ACCT_RSLT.MCHT_DEBIT_ACCT
  is '�̻�֧���˻��˺�';
comment on column HT_TBL_ACCT_RSLT.MCHT_DEBIT_ACCT_NM
  is '�̻�֧���˻�����';
comment on column HT_TBL_ACCT_RSLT.PLAT_DEBIT_BANK
  is 'ƽ̨������';
comment on column HT_TBL_ACCT_RSLT.PLAT_DEBIT_BANK_NM
  is 'ƽ̨����������';
comment on column HT_TBL_ACCT_RSLT.PLAT_DEBIT_ACCT_TYPE
  is 'ƽ̨�����˻�����';
comment on column HT_TBL_ACCT_RSLT.PLAT_DEBIT_ACCT
  is 'ƽ̨�����˻��˺�';
comment on column HT_TBL_ACCT_RSLT.PLAT_DEBIT_ACCT_NM
  is 'ƽ̨�����˻�����';
comment on column HT_TBL_ACCT_RSLT.PLAT_DEBIT_XINGMIN_ACCT
  is 'ƽ̨���������׸��˻�';
comment on column HT_TBL_ACCT_RSLT.DEBT_CHNL_ID
  is '֧�������������';
comment on column HT_TBL_ACCT_RSLT.DEBT_CHNL_NM
  is '֧��������������';
comment on column HT_TBL_ACCT_RSLT.DEBT_RSLT_NO
  is '֧��������ˮ��';
comment on column HT_TBL_ACCT_RSLT.ACCT_STATU
  is '����״̬';
comment on column HT_TBL_ACCT_RSLT.SZ_STATUS
  is '��֧״̬';
comment on column HT_TBL_ACCT_RSLT.SEQ_NUM
  is '���';






create table HT_TBL_ACTIVE_MCHT_INF
(
  INTER_BRH_CODE VARCHAR2(10) not null,
  SETT_DATE      VARCHAR2(8) not null,
  MCHT_NO        VARCHAR2(15),
  MCHT_NM        VARCHAR2(60),
  TRANS_NUM      NUMBER(10),
  TRANS_AT       NUMBER(12,2),
  SEQ_NUM        NUMBER(10) not null
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_ACTIVE_MCHT_INF.INTER_BRH_CODE
  is '�ڲ�������';
comment on column HT_TBL_ACTIVE_MCHT_INF.SETT_DATE
  is '��������';
comment on column HT_TBL_ACTIVE_MCHT_INF.MCHT_NO
  is '�̻���';
comment on column HT_TBL_ACTIVE_MCHT_INF.MCHT_NM
  is '�̻�����';
comment on column HT_TBL_ACTIVE_MCHT_INF.TRANS_NUM
  is '���ױ���';
comment on column HT_TBL_ACTIVE_MCHT_INF.TRANS_AT
  is '���׽��';
comment on column HT_TBL_ACTIVE_MCHT_INF.SEQ_NUM
  is '���';






create table HT_TBL_ALGO_CTL
(
  ID          NUMBER(38),
  BUS_TP      CHAR(8) not null,
  TXN_NUM     CHAR(4) not null,
  BUS_NAME    VARCHAR2(80),
  TXN_NAME    VARCHAR2(80),
  C_D_FLAG    CHAR(1),
  STLM_FLAG   VARCHAR2(10),
  REC_UPD_OPR VARCHAR2(11),
  REC_CRT_TS  CHAR(14),
  REC_UPD_TS  CHAR(14)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_ALGO_CTL.ID
  is '���к�';
comment on column HT_TBL_ALGO_CTL.BUS_TP
  is 'ҵ������';
comment on column HT_TBL_ALGO_CTL.TXN_NUM
  is '��������';
comment on column HT_TBL_ALGO_CTL.BUS_NAME
  is 'ҵ������˵��';
comment on column HT_TBL_ALGO_CTL.TXN_NAME
  is '��������˵��';
comment on column HT_TBL_ALGO_CTL.C_D_FLAG
  is '������';
comment on column HT_TBL_ALGO_CTL.STLM_FLAG
  is '�Ƿ������־��1�����㣬0�������㣩';
comment on column HT_TBL_ALGO_CTL.REC_UPD_OPR
  is '���¸��¹�Ա';
comment on column HT_TBL_ALGO_CTL.REC_CRT_TS
  is '���´���ʱ��';
comment on column HT_TBL_ALGO_CTL.REC_UPD_TS
  is '���¸���ʱ��';






create table HT_TBL_BRH_INFO
(
  BRH_ID     VARCHAR2(10) not null,
  CUP_BRH_ID VARCHAR2(10),
  BRH_LEVEL  CHAR(1),
  DESCR      VARCHAR2(40)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_BRH_INFO.BRH_ID
  is '�ڲ�������';
comment on column HT_TBL_BRH_INFO.CUP_BRH_ID
  is '����������';
comment on column HT_TBL_BRH_INFO.BRH_LEVEL
  is '����';
comment on column HT_TBL_BRH_INFO.DESCR
  is '����';





create table HT_TBL_BRH_MAP
(
  BUS_TP CHAR(8) not null,
  BRH_ID VARCHAR2(10) not null,
  DESCR  VARCHAR2(40)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_BRH_MAP.BUS_TP
  is '�ⲿ������';
comment on column HT_TBL_BRH_MAP.BRH_ID
  is '�ڲ�������';
comment on column HT_TBL_BRH_MAP.DESCR
  is '˵��';






create table HT_TBL_BUF_CHG
(
  L_USAGE_KEY      NUMBER(38) not null,
  I_BUF_CHG_INDEX  NUMBER(38) not null,
  I_SOUR_BUF_INDEX NUMBER(38),
  I_DEST_BUF_INDEX NUMBER(38),
  SP_DSP           CHAR(380)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_BUF_CHG.L_USAGE_KEY
  is 'Ӧ�÷�Χ';
comment on column HT_TBL_BUF_CHG.I_BUF_CHG_INDEX
  is '���ݸ�ֵ��ϵ����';
comment on column HT_TBL_BUF_CHG.I_SOUR_BUF_INDEX
  is 'ԴBUF������';
comment on column HT_TBL_BUF_CHG.I_DEST_BUF_INDEX
  is 'Ŀ��BUF������';
comment on column HT_TBL_BUF_CHG.SP_DSP
  is '��������';





create table HT_TBL_BUF_DSP
(
  L_USAGE_KEY     NUMBER(38) not null,
  I_BUF_DSP_INDEX NUMBER(38) not null,
  I_POS_INDEX     NUMBER(38) not null,
  I_FLD_INDEX     NUMBER(38),
  I_FLD_ID        NUMBER(38),
  I_FLD_OFFSET    NUMBER(38)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_BUF_DSP.L_USAGE_KEY
  is 'Ӧ�÷�Χ';
comment on column HT_TBL_BUF_DSP.I_BUF_DSP_INDEX
  is 'BUF�������';
comment on column HT_TBL_BUF_DSP.I_POS_INDEX
  is '˳���';
comment on column HT_TBL_BUF_DSP.I_FLD_INDEX
  is '������';
comment on column HT_TBL_BUF_DSP.I_FLD_ID
  is '���ֶκ�';
comment on column HT_TBL_BUF_DSP.I_FLD_OFFSET
  is 'ƫ����';






create table HT_TBL_CHECK_FLOW
(
  INTER_BRH_CODE CHAR(10) not null,
  SETT_DATE      VARCHAR2(8),
  TRANS_DATE     VARCHAR2(8),
  TRANS_TIME     VARCHAR2(10),
  FLAG_RESULT    CHAR(1),
  ORDER_NO       VARCHAR2(32),
  PLAT_KEY       VARCHAR2(48) not null,
  BATCH_NO       VARCHAR2(2),
  BUS_TP         CHAR(8),
  TRANS_TP       VARCHAR2(4),
  ACCT_TYPE      VARCHAR2(2),
  TRANS_STATE    CHAR(1),
  S_PAN          CHAR(19),
  S_CCY          CHAR(3),
  S_TRANS_NUM    CHAR(12),
  S_TRANS_AT     CHAR(12),
  S_TRANS_FEE    CHAR(12),
  D_PAN          CHAR(19),
  D_CCY          CHAR(3),
  D_TRANS_NUM    CHAR(12),
  D_TRANS_AT     CHAR(12),
  D_TRANS_FEE    CHAR(12),
  FEE_RATE       NUMBER(12,2),
  RESERVE        VARCHAR2(48),
  SEQ_NUM        NUMBER(10)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_CHECK_FLOW.INTER_BRH_CODE
  is '�ڲ�������';
comment on column HT_TBL_CHECK_FLOW.SETT_DATE
  is '��������';
comment on column HT_TBL_CHECK_FLOW.TRANS_DATE
  is '��������';
comment on column HT_TBL_CHECK_FLOW.TRANS_TIME
  is '����ʱ��';
comment on column HT_TBL_CHECK_FLOW.FLAG_RESULT
  is '���Ա�־';
comment on column HT_TBL_CHECK_FLOW.ORDER_NO
  is '������';
comment on column HT_TBL_CHECK_FLOW.PLAT_KEY
  is 'ƽ̨ͳһ��ˮ��';
comment on column HT_TBL_CHECK_FLOW.BATCH_NO
  is 'ƽ̨���κ�';
comment on column HT_TBL_CHECK_FLOW.BUS_TP
  is 'ҵ������';
comment on column HT_TBL_CHECK_FLOW.TRANS_TP
  is '��������';
comment on column HT_TBL_CHECK_FLOW.ACCT_TYPE
  is '�˻�����';
comment on column HT_TBL_CHECK_FLOW.TRANS_STATE
  is '����״̬';
comment on column HT_TBL_CHECK_FLOW.S_PAN
  is 'ƽ̨�����˺�';
comment on column HT_TBL_CHECK_FLOW.S_CCY
  is 'ƽ̨����';
comment on column HT_TBL_CHECK_FLOW.S_TRANS_NUM
  is 'ƽ̨���ױ���';
comment on column HT_TBL_CHECK_FLOW.S_TRANS_AT
  is 'ƽ̨���׽��';
comment on column HT_TBL_CHECK_FLOW.S_TRANS_FEE
  is 'ƽ̨������';
comment on column HT_TBL_CHECK_FLOW.D_PAN
  is '�Է������˺�';
comment on column HT_TBL_CHECK_FLOW.D_CCY
  is '�Է�����';
comment on column HT_TBL_CHECK_FLOW.D_TRANS_NUM
  is '�Է����ױ���';
comment on column HT_TBL_CHECK_FLOW.D_TRANS_AT
  is '�Է����׽��';
comment on column HT_TBL_CHECK_FLOW.D_TRANS_FEE
  is '�Է�������';
comment on column HT_TBL_CHECK_FLOW.FEE_RATE
  is '�������';
comment on column HT_TBL_CHECK_FLOW.RESERVE
  is '��ע';
comment on column HT_TBL_CHECK_FLOW.SEQ_NUM
  is '���';






create table HT_TBL_CHILD_INF
(
  INTER_BRH_CODE     CHAR(10) not null,
  SETTLMT_DATE       CHAR(8) not null,
  MISSION_INDEX      CHAR(4) not null,
  CHILD_ID           NUMBER(15) not null,
  COMMIT_FLAG        NUMBER(15),
  COMMIT_NUM         NUMBER(15),
  CHILD_BEGIN_POINT  NUMBER(15),
  CHILD_END_POINT    NUMBER(15),
  CHILD_FINISH_POINT NUMBER(15),
  CHILD_ERR_POINT    NUMBER(15),
  CHILD_STATUS       NUMBER(15)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );






create table HT_TBL_CHILD_TIME
(
  INTER_BRH_CODE     CHAR(10) not null,
  MISSION_INDEX      CHAR(4) not null,
  MISSION_DATE       CHAR(8) not null,
  MISSION_WEEKDAY    CHAR(1),
  MISSION_HOUR       CHAR(2) not null,
  MISSION_DATE_ADJ   NUMBER(15),
  CHILD_ID           NUMBER(15),
  COMMIT_FLAG        NUMBER(15),
  COMMIT_NUM         NUMBER(15),
  CHILD_BEGIN_POINT  NUMBER(15),
  CHILD_END_POINT    NUMBER(15),
  CHILD_FINISH_POINT NUMBER(15),
  CHILD_ERR_POINT    NUMBER(15),
  CHILD_STATUS       NUMBER(15)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_CHILD_TIME.INTER_BRH_CODE
  is '�ڲ�������';
comment on column HT_TBL_CHILD_TIME.MISSION_INDEX
  is '����������';
comment on column HT_TBL_CHILD_TIME.MISSION_DATE
  is '��������';
comment on column HT_TBL_CHILD_TIME.MISSION_WEEKDAY
  is '��������';
comment on column HT_TBL_CHILD_TIME.MISSION_HOUR
  is '����ʱ��';
comment on column HT_TBL_CHILD_TIME.CHILD_ID
  is '�����к�';
comment on column HT_TBL_CHILD_TIME.COMMIT_FLAG
  is '���ӱ��';
comment on column HT_TBL_CHILD_TIME.COMMIT_NUM
  is '������';
comment on column HT_TBL_CHILD_TIME.CHILD_BEGIN_POINT
  is '��ʼ���';
comment on column HT_TBL_CHILD_TIME.CHILD_END_POINT
  is '�������';
comment on column HT_TBL_CHILD_TIME.CHILD_FINISH_POINT
  is '��ɽ��';
comment on column HT_TBL_CHILD_TIME.CHILD_ERR_POINT
  is '�����';
comment on column HT_TBL_CHILD_TIME.CHILD_STATUS
  is '��״̬';






create table HT_TBL_CIB_TXN
(
  INTER_BRH_CODE CHAR(10) not null,
  SETT_DATE      VARCHAR2(8),
  EPAYTIME       CHAR(14),
  ORDERDATE      CHAR(8),
  TRANSDATE      CHAR(8),
  PLAT_KEY       VARCHAR2(48) not null,
  BATCH_NO       VARCHAR2(2),
  FLAG_RESULT    CHAR(1),
  SNO            CHAR(16),
  MRCHNO         VARCHAR2(8),
  ORDER_NO       VARCHAR2(32),
  ORDERAMT       CHAR(12),
  FEEAMT         CHAR(12),
  BUS_TP         CHAR(8),
  TRANS_TP       VARCHAR2(4) not null,
  TRANS_STATE    CHAR(1),
  ACCT_TYPE      VARCHAR2(2),
  PAN            CHAR(20),
  CCY            CHAR(3),
  SEQ_NUM        NUMBER(10)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_CIB_TXN.INTER_BRH_CODE
  is '�ڲ�������';
comment on column HT_TBL_CIB_TXN.SETT_DATE
  is '��������';
comment on column HT_TBL_CIB_TXN.EPAYTIME
  is 'EPAYϵͳʱ�䣨��Ȼ�գ�';
comment on column HT_TBL_CIB_TXN.ORDERDATE
  is '��������';
comment on column HT_TBL_CIB_TXN.TRANSDATE
  is '�������ڣ���Ȼ�գ�';
comment on column HT_TBL_CIB_TXN.PLAT_KEY
  is 'ƽ̨ͳһ��ˮ��';
comment on column HT_TBL_CIB_TXN.BATCH_NO
  is 'ƽ̨���κ�';
comment on column HT_TBL_CIB_TXN.FLAG_RESULT
  is '���Ա�־';
comment on column HT_TBL_CIB_TXN.SNO
  is '����ϵͳ��ˮ��';
comment on column HT_TBL_CIB_TXN.MRCHNO
  is '�̻���';
comment on column HT_TBL_CIB_TXN.ORDER_NO
  is '�������';
comment on column HT_TBL_CIB_TXN.ORDERAMT
  is '֧�����';
comment on column HT_TBL_CIB_TXN.FEEAMT
  is '������';
comment on column HT_TBL_CIB_TXN.BUS_TP
  is 'ҵ������';
comment on column HT_TBL_CIB_TXN.TRANS_TP
  is '��������';
comment on column HT_TBL_CIB_TXN.TRANS_STATE
  is '����״̬';
comment on column HT_TBL_CIB_TXN.ACCT_TYPE
  is '�˻�����';
comment on column HT_TBL_CIB_TXN.PAN
  is '�˻��˺�';
comment on column HT_TBL_CIB_TXN.CCY
  is '����';
comment on column HT_TBL_CIB_TXN.SEQ_NUM
  is '���к�';





create table HT_TBL_CMP_FLOW
(
  INTER_BRH_CODE CHAR(10) not null,
  SETT_DATE      VARCHAR2(8),
  TRANS_DATE     VARCHAR2(8),
  TRANS_TIME     VARCHAR2(10),
  FLAG_RESULT    CHAR(1),
  ORDER_NO       VARCHAR2(32),
  PLAT_KEY       VARCHAR2(48) not null,
  BATCH_NO       VARCHAR2(2),
  BUS_TP         CHAR(8),
  TRANS_TP       VARCHAR2(4),
  ACCT_TYPE      VARCHAR2(2),
  TRANS_STATE    CHAR(1),
  REVSAL_FLAG    CHAR(1),
  KEY_REVSAL     VARCHAR2(32),
  CANCEL_FLAG    CHAR(1),
  KEY_CANCEL     VARCHAR2(32),
  PAN            CHAR(20),
  ACCT_NAME      CHAR(60),
  PAN1           CHAR(20),
  CCY            CHAR(3),
  TRANS_AT       CHAR(12),
  TRANS_AT2      CHAR(12),
  TRANS_AT3      CHAR(12),
  TRANS_AT4      CHAR(12),
  TRANS_AT5      CHAR(12),
  EXT_MCHT_NO    VARCHAR2(15),
  SETT_TP1       VARCHAR2(3),
  MCHT_NO1       VARCHAR2(15),
  SETT_TP2       VARCHAR2(3),
  MCHT_NO2       VARCHAR2(15),
  SETT_TP3       VARCHAR2(3),
  MCHT_NO3       VARCHAR2(15),
  SETT_TP4       VARCHAR2(3),
  MCHT_NO4       VARCHAR2(15),
  SETT_TP5       VARCHAR2(3),
  MCHT_NO5       VARCHAR2(15),
  SETT_TP6       VARCHAR2(3),
  MCHT_NO6       VARCHAR2(15),
  SETT_TP7       VARCHAR2(3),
  MCHT_NO7       VARCHAR2(15),
  SETT_TP8       VARCHAR2(3),
  MCHT_NO8       VARCHAR2(15),
  SETT_TP9       VARCHAR2(3),
  MCHT_NO9       VARCHAR2(15),
  SEQ_NUM        NUMBER(10)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_CMP_FLOW.INTER_BRH_CODE
  is '�ڲ�������';
comment on column HT_TBL_CMP_FLOW.SETT_DATE
  is '��������';
comment on column HT_TBL_CMP_FLOW.TRANS_DATE
  is '��������';
comment on column HT_TBL_CMP_FLOW.TRANS_TIME
  is '����ʱ��';
comment on column HT_TBL_CMP_FLOW.FLAG_RESULT
  is '���Ա�־';
comment on column HT_TBL_CMP_FLOW.ORDER_NO
  is '������';
comment on column HT_TBL_CMP_FLOW.PLAT_KEY
  is 'ƽ̨ͳһ��ˮ��';
comment on column HT_TBL_CMP_FLOW.BATCH_NO
  is 'ƽ̨���κ�';
comment on column HT_TBL_CMP_FLOW.BUS_TP
  is 'ҵ������';
comment on column HT_TBL_CMP_FLOW.TRANS_TP
  is '��������';
comment on column HT_TBL_CMP_FLOW.ACCT_TYPE
  is '�˻�����';
comment on column HT_TBL_CMP_FLOW.TRANS_STATE
  is '����״̬';
comment on column HT_TBL_CMP_FLOW.REVSAL_FLAG
  is '������־';
comment on column HT_TBL_CMP_FLOW.KEY_REVSAL
  is '������ԭʼ�����ƥ����';
comment on column HT_TBL_CMP_FLOW.CANCEL_FLAG
  is '������־';
comment on column HT_TBL_CMP_FLOW.KEY_CANCEL
  is '������ԭʼ�����ƥ����';
comment on column HT_TBL_CMP_FLOW.PAN
  is '�˻��˺�';
comment on column HT_TBL_CMP_FLOW.ACCT_NAME
  is '�˻�����';
comment on column HT_TBL_CMP_FLOW.PAN1
  is '�����׸��˺�';
comment on column HT_TBL_CMP_FLOW.CCY
  is '����';
comment on column HT_TBL_CMP_FLOW.TRANS_AT
  is '���׽��';
comment on column HT_TBL_CMP_FLOW.TRANS_AT2
  is '���׽��2';
comment on column HT_TBL_CMP_FLOW.TRANS_AT3
  is '���׽��3';
comment on column HT_TBL_CMP_FLOW.TRANS_AT4
  is '���׽��4';
comment on column HT_TBL_CMP_FLOW.TRANS_AT5
  is '���׽��5';
comment on column HT_TBL_CMP_FLOW.EXT_MCHT_NO
  is '�����̻�����';
comment on column HT_TBL_CMP_FLOW.SETT_TP1
  is '����������1';
comment on column HT_TBL_CMP_FLOW.MCHT_NO1
  is '���������1';
comment on column HT_TBL_CMP_FLOW.SETT_TP2
  is '����������2';
comment on column HT_TBL_CMP_FLOW.MCHT_NO2
  is '���������2';
comment on column HT_TBL_CMP_FLOW.SETT_TP3
  is '����������3';
comment on column HT_TBL_CMP_FLOW.MCHT_NO3
  is '���������3';
comment on column HT_TBL_CMP_FLOW.SETT_TP4
  is '����������4';
comment on column HT_TBL_CMP_FLOW.MCHT_NO4
  is '���������4';
comment on column HT_TBL_CMP_FLOW.SETT_TP5
  is '����������5';
comment on column HT_TBL_CMP_FLOW.MCHT_NO5
  is '���������5';
comment on column HT_TBL_CMP_FLOW.SETT_TP6
  is '����������6';
comment on column HT_TBL_CMP_FLOW.MCHT_NO6
  is '���������6';
comment on column HT_TBL_CMP_FLOW.SETT_TP7
  is '����������7';
comment on column HT_TBL_CMP_FLOW.MCHT_NO7
  is '���������7';
comment on column HT_TBL_CMP_FLOW.SETT_TP8
  is '����������8';
comment on column HT_TBL_CMP_FLOW.MCHT_NO8
  is '���������8';
comment on column HT_TBL_CMP_FLOW.SETT_TP9
  is '����������9';
comment on column HT_TBL_CMP_FLOW.MCHT_NO9
  is '���������9';
comment on column HT_TBL_CMP_FLOW.SEQ_NUM
  is '���к�';






create table HT_TBL_COM_CONFIG
(
  KEY1    VARCHAR2(10) not null,
  KEY2    VARCHAR2(10) not null,
  KEY3    VARCHAR2(10) not null,
  VALUE1  VARCHAR2(60),
  VALUE2  VARCHAR2(60),
  VALUE3  VARCHAR2(60),
  REMARK1 VARCHAR2(120),
  REMARK2 VARCHAR2(120),
  REMARK3 VARCHAR2(120)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_COM_CONFIG.KEY1
  is 'KEY1';
comment on column HT_TBL_COM_CONFIG.KEY2
  is 'KEY2';
comment on column HT_TBL_COM_CONFIG.KEY3
  is 'KEY2';
comment on column HT_TBL_COM_CONFIG.VALUE1
  is '����ֵ1';
comment on column HT_TBL_COM_CONFIG.VALUE2
  is '����ֵ2';
comment on column HT_TBL_COM_CONFIG.VALUE3
  is '����ֵ3';
comment on column HT_TBL_COM_CONFIG.REMARK1
  is '˵��1';
comment on column HT_TBL_COM_CONFIG.REMARK2
  is '˵��2';
comment on column HT_TBL_COM_CONFIG.REMARK3
  is '˵��3';





create table HT_TBL_CON_INF
(
  USAGE_KEY       NUMBER(38) not null,
  TXN_NUM         NUMBER(38) not null,
  CON_INDEX       NUMBER(38) not null,
  FLD_INDEX       NUMBER(38),
  BEGIN_BYTE_POS  NUMBER(38),
  FORMAT_CHG_NEED NUMBER(38),
  VAL             VARCHAR2(35)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_CON_INF.USAGE_KEY
  is 'Ӧ�÷�Χ';
comment on column HT_TBL_CON_INF.TXN_NUM
  is '�ڲ����״���';
comment on column HT_TBL_CON_INF.CON_INDEX
  is '�������';
comment on column HT_TBL_CON_INF.FLD_INDEX
  is '����������λ���';
comment on column HT_TBL_CON_INF.BEGIN_BYTE_POS
  is '������λ����ʼλ��';
comment on column HT_TBL_CON_INF.FORMAT_CHG_NEED
  is '������ֵ�Ƿ���Ҫת��';
comment on column HT_TBL_CON_INF.VAL
  is '���������';






create table HT_TBL_COST_CMP_INF
(
  PLAT_KEY   VARCHAR2(48) not null,
  SETT_DATE  VARCHAR2(8),
  TRANS_TP   VARCHAR2(4),
  TRANS_AMT  NUMBER(12,2),
  FEE_TYPE   CHAR(1),
  FEE_RATE   NUMBER(16,2),
  PLAT_COST  NUMBER(16,2),
  FILE_COST  NUMBER(16,2),
  IS_EQUAL   CHAR(1),
  COST_TYPE  CHAR(1),
  REC_CRT_TS VARCHAR2(14)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_COST_CMP_INF.PLAT_KEY
  is '��ˮ��';
comment on column HT_TBL_COST_CMP_INF.SETT_DATE
  is '����/��������';
comment on column HT_TBL_COST_CMP_INF.TRANS_TP
  is '��������';
comment on column HT_TBL_COST_CMP_INF.TRANS_AMT
  is '����/���˽��';
comment on column HT_TBL_COST_CMP_INF.FEE_TYPE
  is 'ƽ̨�Ʒѷ�ʽ';
comment on column HT_TBL_COST_CMP_INF.FEE_RATE
  is 'ƽ̨����';
comment on column HT_TBL_COST_CMP_INF.PLAT_COST
  is 'ƽ̨����ĳɱ�';
comment on column HT_TBL_COST_CMP_INF.FILE_COST
  is '�ļ��ĳɱ�';
comment on column HT_TBL_COST_CMP_INF.IS_EQUAL
  is '�Ƿ��ƽ';
comment on column HT_TBL_COST_CMP_INF.COST_TYPE
  is '�ɱ�����';
comment on column HT_TBL_COST_CMP_INF.REC_CRT_TS
  is '���´���ʱ��';






create table HT_TBL_DATE_INF
(
  INTER_BRH_CODE CHAR(10) not null,
  STODAY         CHAR(8),
  STWODAYSAGO    CHAR(8),
  SYESTERDAY     CHAR(8),
  SNEXTDATE      CHAR(8),
  SSTARTDATE     CHAR(8),
  SFORESTARTDATE CHAR(8),
  STATUS         NUMBER(5)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );






create table HT_TBL_DAT_FLOW
(
  INTER_BRH_CODE CHAR(10) not null,
  SETT_DATE      VARCHAR2(8),
  TRANS_DATE     VARCHAR2(8),
  TRANS_TIME     VARCHAR2(10),
  FLAG_RESULT    CHAR(1),
  STLM_FLAG      CHAR(1),
  ORDER_NO       VARCHAR2(32),
  PLAT_KEY       VARCHAR2(48) not null,
  BATCH_NO       VARCHAR2(2),
  BUS_TP         CHAR(8),
  TRANS_TP       VARCHAR2(4),
  ACCT_TYPE      VARCHAR2(2),
  TRANS_STATE    CHAR(1),
  REVSAL_FLAG    CHAR(1),
  KEY_REVSAL     VARCHAR2(32),
  CANCEL_FLAG    CHAR(1),
  KEY_CANCEL     VARCHAR2(32),
  PAN            CHAR(20),
  ACCT_NAME      CHAR(60),
  PAN1           CHAR(20),
  CCY            CHAR(3),
  TRANS_AT       CHAR(12),
  TRANS_AT2      CHAR(12),
  TRANS_AT3      CHAR(12),
  TRANS_AT4      CHAR(12),
  TRANS_AT5      CHAR(12),
  EXT_MCHT_NO    VARCHAR2(15),
  SETT_TP1       VARCHAR2(3),
  MCHT_NO1       VARCHAR2(15),
  SETT_TP2       VARCHAR2(3),
  MCHT_NO2       VARCHAR2(15),
  SETT_TP3       VARCHAR2(3),
  MCHT_NO3       VARCHAR2(15),
  SETT_TP4       VARCHAR2(3),
  MCHT_NO4       VARCHAR2(15),
  SETT_TP5       VARCHAR2(3),
  MCHT_NO5       VARCHAR2(15),
  SETT_TP6       VARCHAR2(3),
  MCHT_NO6       VARCHAR2(15),
  SETT_TP7       VARCHAR2(3),
  MCHT_NO7       VARCHAR2(15),
  SETT_TP8       VARCHAR2(3),
  MCHT_NO8       VARCHAR2(15),
  SETT_TP9       VARCHAR2(3),
  MCHT_NO9       VARCHAR2(15),
  SEQ_NUM        NUMBER(10)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_DAT_FLOW.INTER_BRH_CODE
  is '�ڲ�������';
comment on column HT_TBL_DAT_FLOW.SETT_DATE
  is '��������';
comment on column HT_TBL_DAT_FLOW.TRANS_DATE
  is '��������';
comment on column HT_TBL_DAT_FLOW.TRANS_TIME
  is '����ʱ��';
comment on column HT_TBL_DAT_FLOW.FLAG_RESULT
  is '���Ա�־';
comment on column HT_TBL_DAT_FLOW.STLM_FLAG
  is '�����ʾ';
comment on column HT_TBL_DAT_FLOW.ORDER_NO
  is '������';
comment on column HT_TBL_DAT_FLOW.PLAT_KEY
  is 'ƽ̨ͳһ��ˮ��';
comment on column HT_TBL_DAT_FLOW.BATCH_NO
  is 'ƽ̨���κ�';
comment on column HT_TBL_DAT_FLOW.BUS_TP
  is 'ҵ������';
comment on column HT_TBL_DAT_FLOW.TRANS_TP
  is '��������';
comment on column HT_TBL_DAT_FLOW.ACCT_TYPE
  is '�˻�����';
comment on column HT_TBL_DAT_FLOW.TRANS_STATE
  is '����״̬';
comment on column HT_TBL_DAT_FLOW.REVSAL_FLAG
  is '������־';
comment on column HT_TBL_DAT_FLOW.KEY_REVSAL
  is '������ԭʼ�����ƥ����';
comment on column HT_TBL_DAT_FLOW.CANCEL_FLAG
  is '������־';
comment on column HT_TBL_DAT_FLOW.KEY_CANCEL
  is '������ԭʼ�����ƥ����';
comment on column HT_TBL_DAT_FLOW.PAN
  is '�˻��˺�';
comment on column HT_TBL_DAT_FLOW.ACCT_NAME
  is '�˻�����';
comment on column HT_TBL_DAT_FLOW.PAN1
  is '�����׸��˺�';
comment on column HT_TBL_DAT_FLOW.CCY
  is '����';
comment on column HT_TBL_DAT_FLOW.TRANS_AT
  is '���׽��';
comment on column HT_TBL_DAT_FLOW.TRANS_AT2
  is '���׽��2';
comment on column HT_TBL_DAT_FLOW.TRANS_AT3
  is '���׽��3';
comment on column HT_TBL_DAT_FLOW.TRANS_AT4
  is '���׽��4';
comment on column HT_TBL_DAT_FLOW.TRANS_AT5
  is '���׽��5';
comment on column HT_TBL_DAT_FLOW.EXT_MCHT_NO
  is '�����̻�����';
comment on column HT_TBL_DAT_FLOW.SETT_TP1
  is '����������1';
comment on column HT_TBL_DAT_FLOW.MCHT_NO1
  is '���������1';
comment on column HT_TBL_DAT_FLOW.SETT_TP2
  is '����������2';
comment on column HT_TBL_DAT_FLOW.MCHT_NO2
  is '���������2';
comment on column HT_TBL_DAT_FLOW.SETT_TP3
  is '����������3';
comment on column HT_TBL_DAT_FLOW.MCHT_NO3
  is '���������3';
comment on column HT_TBL_DAT_FLOW.SETT_TP4
  is '����������4';
comment on column HT_TBL_DAT_FLOW.MCHT_NO4
  is '���������4';
comment on column HT_TBL_DAT_FLOW.SETT_TP5
  is '����������5';
comment on column HT_TBL_DAT_FLOW.MCHT_NO5
  is '���������5';
comment on column HT_TBL_DAT_FLOW.SETT_TP6
  is '����������6';
comment on column HT_TBL_DAT_FLOW.MCHT_NO6
  is '���������6';
comment on column HT_TBL_DAT_FLOW.SETT_TP7
  is '����������7';
comment on column HT_TBL_DAT_FLOW.MCHT_NO7
  is '���������7';
comment on column HT_TBL_DAT_FLOW.SETT_TP8
  is '����������8';
comment on column HT_TBL_DAT_FLOW.MCHT_NO8
  is '���������8';
comment on column HT_TBL_DAT_FLOW.SETT_TP9
  is '����������9';
comment on column HT_TBL_DAT_FLOW.MCHT_NO9
  is '���������9';
comment on column HT_TBL_DAT_FLOW.SEQ_NUM
  is '���к�';






create table HT_TBL_DISC_ALGO
(
  DISC_ID     CHAR(5) not null,
  DISC_STEP   NUMBER(10) not null,
  OPER_RSLT   NUMBER(12,2),
  OPERAND1    NUMBER(12,2),
  OPERATOR1   CHAR(1),
  OPERAND2    NUMBER(12,2),
  OPERATOR2   CHAR(1),
  OPERAND3    NUMBER(12,2),
  REC_OPR_ID  CHAR(1),
  REC_UPD_OPR CHAR(11),
  REC_CRT_TS  CHAR(14),
  REC_UPD_TS  CHAR(14)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_DISC_ALGO.DISC_ID
  is '�㷨����ID';
comment on column HT_TBL_DISC_ALGO.DISC_STEP
  is '�㷨����';
comment on column HT_TBL_DISC_ALGO.OPER_RSLT
  is '���ֵ';
comment on column HT_TBL_DISC_ALGO.OPERAND1
  is '������1';
comment on column HT_TBL_DISC_ALGO.OPERATOR1
  is '������1';
comment on column HT_TBL_DISC_ALGO.OPERAND2
  is '������2';
comment on column HT_TBL_DISC_ALGO.OPERATOR2
  is '������2';
comment on column HT_TBL_DISC_ALGO.OPERAND3
  is '������3';
comment on column HT_TBL_DISC_ALGO.REC_OPR_ID
  is '������ʶ';
comment on column HT_TBL_DISC_ALGO.REC_UPD_OPR
  is '�޸Ĺ�Ա��';
comment on column HT_TBL_DISC_ALGO.REC_CRT_TS
  is '��¼����ʱ��';
comment on column HT_TBL_DISC_ALGO.REC_UPD_TS
  is '��¼�޸�ʱ��';






create table HT_TBL_ERR_FLOW
(
  INTER_BRH_CODE CHAR(10) not null,
  SETT_DATE      VARCHAR2(8) not null,
  TRANS_DATE     VARCHAR2(8),
  TRANS_TIME     VARCHAR2(10),
  FLAG_RESULT    CHAR(1),
  ACCT_STATU     CHAR(1) default 'N',
  ORDER_NO       VARCHAR2(32),
  PLAT_KEY       VARCHAR2(48) not null,
  BATCH_NO       VARCHAR2(2),
  BUS_TP         CHAR(8),
  TRANS_TP       VARCHAR2(4),
  ACCT_TYPE      VARCHAR2(2),
  TRANS_STATE    CHAR(1),
  PAN            CHAR(20),
  PAN1           CHAR(20),
  CCY            CHAR(3),
  TRANS_AT       CHAR(12),
  DEST_PAN       CHAR(20),
  DEST_CCY       CHAR(3),
  DEST_TRANS_AT  CHAR(12),
  HOST_AT        CHAR(12),
  DEBT_AT        CHAR(12),
  TRANS_FEE      CHAR(12),
  RESERVE        VARCHAR2(48)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_ERR_FLOW.INTER_BRH_CODE
  is '�ڲ�������';
comment on column HT_TBL_ERR_FLOW.SETT_DATE
  is '��������';
comment on column HT_TBL_ERR_FLOW.TRANS_DATE
  is '��������';
comment on column HT_TBL_ERR_FLOW.TRANS_TIME
  is '����ʱ��';
comment on column HT_TBL_ERR_FLOW.FLAG_RESULT
  is '���Ա�־';
comment on column HT_TBL_ERR_FLOW.ACCT_STATU
  is '����״̬';
comment on column HT_TBL_ERR_FLOW.ORDER_NO
  is '������';
comment on column HT_TBL_ERR_FLOW.PLAT_KEY
  is 'ƽ̨ͳһ��ˮ��';
comment on column HT_TBL_ERR_FLOW.BATCH_NO
  is 'ƽ̨���κ�';
comment on column HT_TBL_ERR_FLOW.BUS_TP
  is 'ҵ������';
comment on column HT_TBL_ERR_FLOW.TRANS_TP
  is '��������';
comment on column HT_TBL_ERR_FLOW.ACCT_TYPE
  is '�˻�����';
comment on column HT_TBL_ERR_FLOW.TRANS_STATE
  is '����״̬';
comment on column HT_TBL_ERR_FLOW.PAN
  is '�˻��˺�';
comment on column HT_TBL_ERR_FLOW.PAN1
  is '�����׸��˺�';
comment on column HT_TBL_ERR_FLOW.CCY
  is '����';
comment on column HT_TBL_ERR_FLOW.TRANS_AT
  is '���׽��';
comment on column HT_TBL_ERR_FLOW.DEST_PAN
  is '�˻��˺�';
comment on column HT_TBL_ERR_FLOW.DEST_CCY
  is '����';
comment on column HT_TBL_ERR_FLOW.DEST_TRANS_AT
  is '���׽��';
comment on column HT_TBL_ERR_FLOW.DEBT_AT
  is '���˽��';
comment on column HT_TBL_ERR_FLOW.TRANS_FEE
  is '������';
comment on column HT_TBL_ERR_FLOW.RESERVE
  is '��ע';






create table HT_TBL_FLD_DSP
(
  L_USAGE_KEY NUMBER(38) not null,
  I_FLD_ID    NUMBER(38) not null,
  N_FLD_L     NUMBER(38),
  N_FLD_TYPE  NUMBER(38),
  SP_FLD_DSP  VARCHAR2(250)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_FLD_DSP.L_USAGE_KEY
  is '�������';
comment on column HT_TBL_FLD_DSP.I_FLD_ID
  is '��������';
comment on column HT_TBL_FLD_DSP.N_FLD_L
  is '�򳤶�';
comment on column HT_TBL_FLD_DSP.N_FLD_TYPE
  is '������';
comment on column HT_TBL_FLD_DSP.SP_FLD_DSP
  is '����������';






create table HT_TBL_FLD_TSF
(
  L_USAGE_KEY      NUMBER(38) not null,
  I_BUF_CHG_INDEX  NUMBER(38) not null,
  I_SOUR_FLD_INDEX NUMBER(38) not null,
  I_DEST_FLD_INDEX NUMBER(38)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_FLD_TSF.L_USAGE_KEY
  is '���÷�Χ';
comment on column HT_TBL_FLD_TSF.I_BUF_CHG_INDEX
  is 'ת������ID';
comment on column HT_TBL_FLD_TSF.I_SOUR_FLD_INDEX
  is 'ԴBUF��������';
comment on column HT_TBL_FLD_TSF.I_DEST_FLD_INDEX
  is 'Ŀ��BUF��������';






create table HT_TBL_FUNDS_SHORT
(
  BANK_CODE VARCHAR2(12) not null,
  ACCT_NO   VARCHAR2(22) not null,
  ACCT_NAME VARCHAR2(70),
  SHRT_AMT  NUMBER(15,2),
  SETT_DATE VARCHAR2(8) not null
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_FUNDS_SHORT.BANK_CODE
  is 'ƽ̨������';
comment on column HT_TBL_FUNDS_SHORT.ACCT_NO
  is 'ƽ̨�����˻��˺�';
comment on column HT_TBL_FUNDS_SHORT.ACCT_NAME
  is 'ƽ̨�����˻�����';
comment on column HT_TBL_FUNDS_SHORT.SHRT_AMT
  is 'ȱ�ڽ��';
comment on column HT_TBL_FUNDS_SHORT.SETT_DATE
  is '��������';






create table HT_TBL_GATEWAY_TXN
(
  INTER_BRH_CODE CHAR(10) not null,
  SETT_DATE      VARCHAR2(8),
  TRANS_DATE     VARCHAR2(8),
  TRANS_TIME     VARCHAR2(10),
  FLAG_RESULT    CHAR(1),
  ORDER_NO       VARCHAR2(32),
  PLAT_KEY       VARCHAR2(48) not null,
  HOST_KEY       VARCHAR2(48),
  BATCH_NO       VARCHAR2(2),
  BUS_TP         CHAR(8),
  TRANS_TP       VARCHAR2(4),
  ACCT_TYPE      VARCHAR2(2),
  TRANS_STATE    CHAR(1),
  REVSAL_FLAG    CHAR(1),
  KEY_REVSAL     VARCHAR2(32),
  CANCEL_FLAG    CHAR(1),
  KEY_CANCEL     VARCHAR2(32),
  PAN            VARCHAR2(20),
  ACCT_NAME      VARCHAR2(60),
  PAN1           VARCHAR2(20),
  CCY            CHAR(3),
  TRANS_AT       CHAR(12),
  TRANS_AT2      CHAR(12),
  TRANS_AT3      CHAR(12),
  TRANS_AT4      CHAR(12),
  TRANS_AT5      CHAR(12),
  SETT_MCHT_NO   VARCHAR2(15),
  EXT_MCHT_NO    VARCHAR2(15),
  SETT_TP1       VARCHAR2(3),
  MCHT_NO1       VARCHAR2(15),
  SETT_TP2       VARCHAR2(3),
  MCHT_NO2       VARCHAR2(15),
  SETT_TP3       VARCHAR2(3),
  MCHT_NO3       VARCHAR2(15),
  SETT_TP4       VARCHAR2(3),
  MCHT_NO4       VARCHAR2(15),
  SETT_TP5       VARCHAR2(3),
  MCHT_NO5       VARCHAR2(15),
  SETT_TP6       VARCHAR2(3),
  MCHT_NO6       VARCHAR2(15),
  SETT_TP7       VARCHAR2(3),
  MCHT_NO7       VARCHAR2(15),
  SETT_TP8       VARCHAR2(3),
  MCHT_NO8       VARCHAR2(15),
  SETT_TP9       VARCHAR2(3),
  MCHT_NO9       VARCHAR2(15),
  SEQ_NUM        NUMBER(10)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_GATEWAY_TXN.INTER_BRH_CODE
  is '�ڲ�������';
comment on column HT_TBL_GATEWAY_TXN.SETT_DATE
  is '��������';
comment on column HT_TBL_GATEWAY_TXN.TRANS_DATE
  is '��������';
comment on column HT_TBL_GATEWAY_TXN.TRANS_TIME
  is '����ʱ��';
comment on column HT_TBL_GATEWAY_TXN.FLAG_RESULT
  is '���Ա�־';
comment on column HT_TBL_GATEWAY_TXN.ORDER_NO
  is '������';
comment on column HT_TBL_GATEWAY_TXN.PLAT_KEY
  is 'ƽ̨ͳһ��ˮ��';
comment on column HT_TBL_GATEWAY_TXN.HOST_KEY
  is '����Ψһ��';
comment on column HT_TBL_GATEWAY_TXN.BATCH_NO
  is 'ƽ̨���κ�';
comment on column HT_TBL_GATEWAY_TXN.BUS_TP
  is 'ҵ������';
comment on column HT_TBL_GATEWAY_TXN.TRANS_TP
  is '��������';
comment on column HT_TBL_GATEWAY_TXN.ACCT_TYPE
  is '�˻�����';
comment on column HT_TBL_GATEWAY_TXN.TRANS_STATE
  is '����״̬';
comment on column HT_TBL_GATEWAY_TXN.REVSAL_FLAG
  is '������־';
comment on column HT_TBL_GATEWAY_TXN.KEY_REVSAL
  is '������ԭʼ�����ƥ����';
comment on column HT_TBL_GATEWAY_TXN.CANCEL_FLAG
  is '������־';
comment on column HT_TBL_GATEWAY_TXN.KEY_CANCEL
  is '������ԭʼ�����ƥ����';
comment on column HT_TBL_GATEWAY_TXN.PAN
  is '�˻��˺�';
comment on column HT_TBL_GATEWAY_TXN.ACCT_NAME
  is '�˻�����';
comment on column HT_TBL_GATEWAY_TXN.PAN1
  is '�����׸��˺�';
comment on column HT_TBL_GATEWAY_TXN.CCY
  is '����';
comment on column HT_TBL_GATEWAY_TXN.TRANS_AT
  is '���׽��';
comment on column HT_TBL_GATEWAY_TXN.TRANS_AT2
  is '���׽��2';
comment on column HT_TBL_GATEWAY_TXN.TRANS_AT3
  is '���׽��3';
comment on column HT_TBL_GATEWAY_TXN.TRANS_AT4
  is '���׽��4';
comment on column HT_TBL_GATEWAY_TXN.TRANS_AT5
  is '���׽��5';
comment on column HT_TBL_GATEWAY_TXN.EXT_MCHT_NO
  is '�����̻�����';
comment on column HT_TBL_GATEWAY_TXN.SETT_TP1
  is '����������1';
comment on column HT_TBL_GATEWAY_TXN.MCHT_NO1
  is '���������1';
comment on column HT_TBL_GATEWAY_TXN.SETT_TP2
  is '����������2';
comment on column HT_TBL_GATEWAY_TXN.MCHT_NO2
  is '���������2';
comment on column HT_TBL_GATEWAY_TXN.SETT_TP3
  is '����������3';
comment on column HT_TBL_GATEWAY_TXN.MCHT_NO3
  is '���������3';
comment on column HT_TBL_GATEWAY_TXN.SETT_TP4
  is '����������4';
comment on column HT_TBL_GATEWAY_TXN.MCHT_NO4
  is '���������4';
comment on column HT_TBL_GATEWAY_TXN.SETT_TP5
  is '����������5';
comment on column HT_TBL_GATEWAY_TXN.MCHT_NO5
  is '���������5';
comment on column HT_TBL_GATEWAY_TXN.SETT_TP6
  is '����������6';
comment on column HT_TBL_GATEWAY_TXN.MCHT_NO6
  is '���������6';
comment on column HT_TBL_GATEWAY_TXN.SETT_TP7
  is '����������7';
comment on column HT_TBL_GATEWAY_TXN.MCHT_NO7
  is '���������7';
comment on column HT_TBL_GATEWAY_TXN.SETT_TP8
  is '����������8';
comment on column HT_TBL_GATEWAY_TXN.MCHT_NO8
  is '���������8';
comment on column HT_TBL_GATEWAY_TXN.SETT_TP9
  is '����������9';
comment on column HT_TBL_GATEWAY_TXN.MCHT_NO9
  is '���������9';
comment on column HT_TBL_GATEWAY_TXN.SEQ_NUM
  is '���к�';





create table HT_TBL_HOST_TXN
(
  INTER_BRH_CODE CHAR(10) not null,
  SETT_DATE      VARCHAR2(8),
  TRANS_DATE     VARCHAR2(8),
  TRANS_TIME     VARCHAR2(10),
  FLAG_RESULT    CHAR(1),
  EXT_MCHT_NO    VARCHAR2(15),
  ORDER_NO       VARCHAR2(32),
  PLAT_KEY       VARCHAR2(48) not null,
  BATCH_NO       VARCHAR2(2),
  BUS_TP         CHAR(8),
  TRANS_TP       VARCHAR2(4),
  TRANS_STATE    CHAR(1),
  REVSAL_FLAG    CHAR(1),
  KEY_REVSAL     CHAR(32),
  CANCEL_FLAG    CHAR(1),
  KEY_CANCEL     CHAR(32),
  ACCT_TYPE      VARCHAR2(2),
  PAN            CHAR(20),
  CCY            CHAR(3),
  TRANS_AT       CHAR(12),
  TRANS_AT2      CHAR(12),
  TRANS_AT3      CHAR(12),
  TRANS_AT4      CHAR(12),
  TRANS_AT5      CHAR(12),
  SEQ_NUM        NUMBER(10)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_HOST_TXN.INTER_BRH_CODE
  is '�ڲ�������';
comment on column HT_TBL_HOST_TXN.SETT_DATE
  is '��������';
comment on column HT_TBL_HOST_TXN.TRANS_DATE
  is '��������';
comment on column HT_TBL_HOST_TXN.TRANS_TIME
  is '����ʱ��';
comment on column HT_TBL_HOST_TXN.FLAG_RESULT
  is '���Ա�־';
comment on column HT_TBL_HOST_TXN.EXT_MCHT_NO
  is '�����̻�����';
comment on column HT_TBL_HOST_TXN.ORDER_NO
  is '������';
comment on column HT_TBL_HOST_TXN.PLAT_KEY
  is 'ƽ̨ͳһ��ˮ��';
comment on column HT_TBL_HOST_TXN.BATCH_NO
  is 'ƽ̨���κ�';
comment on column HT_TBL_HOST_TXN.BUS_TP
  is 'ҵ������';
comment on column HT_TBL_HOST_TXN.TRANS_TP
  is '��������';
comment on column HT_TBL_HOST_TXN.TRANS_STATE
  is '����״̬';
comment on column HT_TBL_HOST_TXN.REVSAL_FLAG
  is '������־';
comment on column HT_TBL_HOST_TXN.KEY_REVSAL
  is '������ԭʼ�����ƥ����';
comment on column HT_TBL_HOST_TXN.CANCEL_FLAG
  is '������־';
comment on column HT_TBL_HOST_TXN.KEY_CANCEL
  is '������ԭʼ�����ƥ����';
comment on column HT_TBL_HOST_TXN.ACCT_TYPE
  is '�˻�����';
comment on column HT_TBL_HOST_TXN.PAN
  is '�˻��˺�';
comment on column HT_TBL_HOST_TXN.CCY
  is '����';
comment on column HT_TBL_HOST_TXN.TRANS_AT
  is '���׽��';
comment on column HT_TBL_HOST_TXN.TRANS_AT2
  is '���׽��2';
comment on column HT_TBL_HOST_TXN.TRANS_AT3
  is '���׽��3';
comment on column HT_TBL_HOST_TXN.TRANS_AT4
  is '���׽��4';
comment on column HT_TBL_HOST_TXN.TRANS_AT5
  is '���׽��5';
comment on column HT_TBL_HOST_TXN.SEQ_NUM
  is '���к�';






create table HT_TBL_IPC_DFT
(
  USAGE_KEY     NUMBER(38) not null,
  IPC_DFT_INDEX NUMBER(38) not null,
  POS_INDEX     NUMBER(38),
  FLD_INDEX     NUMBER(38),
  FLD_LEN       NUMBER(38),
  FLD_VAL       CHAR(600)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_IPC_DFT.USAGE_KEY
  is '���÷�Χ';
comment on column HT_TBL_IPC_DFT.IPC_DFT_INDEX
  is 'IPC�������';
comment on column HT_TBL_IPC_DFT.POS_INDEX
  is '˳���';
comment on column HT_TBL_IPC_DFT.FLD_INDEX
  is '������';
comment on column HT_TBL_IPC_DFT.FLD_LEN
  is '�򳤶�';
comment on column HT_TBL_IPC_DFT.FLD_VAL
  is '��Ĭ��ֵ';




create table HT_TBL_IPC_DFT_DSP
(
  USAGE_KEY     NUMBER(38) not null,
  COMP_KEY      CHAR(10) not null,
  COMP_KEY_LEN  NUMBER(38),
  IPC_DFT_INDEX NUMBER(38),
  BUF_DSP_INDEX NUMBER(38),
  TXN_NUM       CHAR(4),
  IPC_DFT       CHAR(256)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_IPC_DFT_DSP.USAGE_KEY
  is '�������';
comment on column HT_TBL_IPC_DFT_DSP.COMP_KEY
  is '�Ƚ�����ֵ';
comment on column HT_TBL_IPC_DFT_DSP.COMP_KEY_LEN
  is '����ֵ����';
comment on column HT_TBL_IPC_DFT_DSP.IPC_DFT_INDEX
  is 'Ĭ��IPC����';
comment on column HT_TBL_IPC_DFT_DSP.BUF_DSP_INDEX
  is 'BUFFERת������';
comment on column HT_TBL_IPC_DFT_DSP.TXN_NUM
  is '�ڲ�������';
comment on column HT_TBL_IPC_DFT_DSP.IPC_DFT
  is '����';





create table HT_TBL_MISSION_CFG
(
  MISSION_TYPE  CHAR(1) not null,
  MISSION_INDEX CHAR(4) not null,
  MISSION_NAME  VARCHAR2(200),
  MISSION_LEVEL NUMBER(15),
  DATA_NUM      NUMBER(15),
  COMMIT_FLAG   NUMBER(15),
  COMMIT_NUM    NUMBER(15)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_MISSION_CFG.MISSION_TYPE
  is '�������';
comment on column HT_TBL_MISSION_CFG.MISSION_INDEX
  is '����������';
comment on column HT_TBL_MISSION_CFG.MISSION_NAME
  is '������';
comment on column HT_TBL_MISSION_CFG.MISSION_LEVEL
  is '���񼶱�';
comment on column HT_TBL_MISSION_CFG.DATA_NUM
  is 'ÿ����������ļ�¼��';
comment on column HT_TBL_MISSION_CFG.COMMIT_FLAG
  is 'COMMIT��־';
comment on column HT_TBL_MISSION_CFG.COMMIT_NUM
  is 'ÿ��COMMIT�ļ�¼��';





create table HT_TBL_MISSION_INF
(
  INTER_BRH_CODE CHAR(10) not null,
  SETTLMT_DATE   CHAR(8) not null,
  MISSION_INDEX  CHAR(4) not null,
  MISSION_NAME   VARCHAR2(200),
  MISSION_LEVEL  NUMBER(15),
  MISSION_STATUS NUMBER(15),
  DATA_NUM       NUMBER(15),
  CHILD_NUM      NUMBER(15),
  COMMIT_FLAG    NUMBER(15),
  COMMIT_NUM     NUMBER(15),
  START_TIME     VARCHAR2(40),
  END_TIME       VARCHAR2(40)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_MISSION_INF.INTER_BRH_CODE
  is '�ڲ�������';
comment on column HT_TBL_MISSION_INF.SETTLMT_DATE
  is '��������';
comment on column HT_TBL_MISSION_INF.MISSION_INDEX
  is '����������';
comment on column HT_TBL_MISSION_INF.MISSION_NAME
  is '������';
comment on column HT_TBL_MISSION_INF.MISSION_LEVEL
  is '���񼶱�';
comment on column HT_TBL_MISSION_INF.MISSION_STATUS
  is '����״̬';
comment on column HT_TBL_MISSION_INF.DATA_NUM
  is 'ÿ����������ļ�¼��';
comment on column HT_TBL_MISSION_INF.CHILD_NUM
  is '��������';
comment on column HT_TBL_MISSION_INF.COMMIT_FLAG
  is 'COMMIT��־';
comment on column HT_TBL_MISSION_INF.COMMIT_NUM
  is 'ÿ��COMMIT�ļ�¼��';
comment on column HT_TBL_MISSION_INF.START_TIME
  is '����ʼʱ��';
comment on column HT_TBL_MISSION_INF.END_TIME
  is '�������ʱ��';






create table HT_TBL_MISSION_INFO
(
  BRH_ID     VARCHAR2(10) not null,
  SETT_DATE  VARCHAR2(8) not null,
  TRANS_DATE VARCHAR2(8),
  SEQ_NO     VARCHAR2(20) not null,
  BANK_CODE  VARCHAR2(12),
  ACCT_TYPE  VARCHAR2(8),
  ACCT_NO    VARCHAR2(22),
  ACCT_NAME  VARCHAR2(70),
  DC_FLAG    VARCHAR2(1),
  BANK_CODE1 VARCHAR2(12),
  ACCT_TYPE1 VARCHAR2(8),
  ACCT_NO1   VARCHAR2(22),
  ACCT_NAME1 VARCHAR2(70),
  AMT        NUMBER(15,2),
  STATUS     VARCHAR2(1),
  FILE_NAME  VARCHAR2(100),
  CHNL_ID    VARCHAR2(10),
  PS         VARCHAR2(80)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_MISSION_INFO.BRH_ID
  is '�ڲ�������';
comment on column HT_TBL_MISSION_INFO.SETT_DATE
  is '��������';
comment on column HT_TBL_MISSION_INFO.TRANS_DATE
  is '��������';
comment on column HT_TBL_MISSION_INFO.SEQ_NO
  is '�鼯��ˮ';
comment on column HT_TBL_MISSION_INFO.BANK_CODE
  is '�����𿪻���';
comment on column HT_TBL_MISSION_INFO.ACCT_TYPE
  is '�����˻�����';
comment on column HT_TBL_MISSION_INFO.ACCT_NO
  is '�����˻��˺�';
comment on column HT_TBL_MISSION_INFO.ACCT_NAME
  is 'ƽ̨�����˻�����';
comment on column HT_TBL_MISSION_INFO.DC_FLAG
  is '֧��/����';
comment on column HT_TBL_MISSION_INFO.BANK_CODE1
  is '�����𿪻���';
comment on column HT_TBL_MISSION_INFO.ACCT_TYPE1
  is '���ֽ����˻�����';
comment on column HT_TBL_MISSION_INFO.ACCT_NO1
  is '���ֽ����˻��˺�';
comment on column HT_TBL_MISSION_INFO.ACCT_NAME1
  is '���ֽ����˻�����';
comment on column HT_TBL_MISSION_INFO.AMT
  is '���˽��';
comment on column HT_TBL_MISSION_INFO.STATUS
  is '����״̬';
comment on column HT_TBL_MISSION_INFO.FILE_NAME
  is '�����ļ���';
comment on column HT_TBL_MISSION_INFO.CHNL_ID
  is '��������';
comment on column HT_TBL_MISSION_INFO.PS
  is '����(postscript)';






create table HT_TBL_MISSION_TIME
(
  INTER_BRH_CODE   CHAR(10) not null,
  MISSION_INDEX    CHAR(4) not null,
  MISSION_NAME     VARCHAR2(200),
  MISSION_DATE     CHAR(8),
  MISSION_WEEKDAY  CHAR(1),
  MISSION_HOUR     CHAR(2),
  MISSION_DATE_ADJ NUMBER(15),
  DATA_NUM         NUMBER(15),
  CHILD_NUM        NUMBER(15),
  COMMIT_FLAG      NUMBER(15),
  COMMIT_NUM       NUMBER(15)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_MISSION_TIME.INTER_BRH_CODE
  is '�ڲ�������';
comment on column HT_TBL_MISSION_TIME.MISSION_INDEX
  is '����������';
comment on column HT_TBL_MISSION_TIME.MISSION_NAME
  is '������';
comment on column HT_TBL_MISSION_TIME.MISSION_DATE
  is '��������';
comment on column HT_TBL_MISSION_TIME.MISSION_WEEKDAY
  is '��������';
comment on column HT_TBL_MISSION_TIME.MISSION_HOUR
  is '����ʱ��';
comment on column HT_TBL_MISSION_TIME.DATA_NUM
  is 'ÿ����������ļ�¼��';
comment on column HT_TBL_MISSION_TIME.CHILD_NUM
  is '��������';
comment on column HT_TBL_MISSION_TIME.COMMIT_FLAG
  is 'COMMIT��־';
comment on column HT_TBL_MISSION_TIME.COMMIT_NUM
  is 'ÿ��COMMIT�ļ�¼��';






create table HT_TBL_OPR_CTL
(
  TBL_OPR_FLAG NUMBER(15) not null,
  OBJECT_NAME  CHAR(60) not null,
  PARAM_1      CHAR(60),
  PARAM_2      CHAR(60),
  PARAM_3      CHAR(60),
  PARAM_4      CHAR(60)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_OPR_CTL.TBL_OPR_FLAG
  is '��־λ';
comment on column HT_TBL_OPR_CTL.OBJECT_NAME
  is '������';
comment on column HT_TBL_OPR_CTL.PARAM_1
  is '����1 ';
comment on column HT_TBL_OPR_CTL.PARAM_2
  is '����2';
comment on column HT_TBL_OPR_CTL.PARAM_3
  is '����3';
comment on column HT_TBL_OPR_CTL.PARAM_4
  is '����4';




create table HT_TBL_PART_INF
(
  TBL_NAME         CHAR(60) not null,
  PART_NAME        CHAR(60),
  TBL_SPC_NAME     CHAR(60),
  TBL_INTERVEL_DAY NUMBER(12),
  TBL_OPR_FLAG     NUMBER(12)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_PART_INF.TBL_NAME
  is '�������';
comment on column HT_TBL_PART_INF.PART_NAME
  is 'Ŀ�����ݱ���';
comment on column HT_TBL_PART_INF.TBL_SPC_NAME
  is '��������';
comment on column HT_TBL_PART_INF.TBL_INTERVEL_DAY
  is '����ʱ�䣨�죩';
comment on column HT_TBL_PART_INF.TBL_OPR_FLAG
  is '��������';





create table HT_TBL_POSTION_TRIAL
(
  BANK_CODE VARCHAR2(12) not null,
  BANK_NAME VARCHAR2(70),
  ACCT_NO   VARCHAR2(22) not null,
  ACCT_NAME VARCHAR2(70),
  INTEREST  NUMBER(15,3),
  SETT_DATE VARCHAR2(8) not null,
  BAL       NUMBER(15,2),
  RESV_BAL  NUMBER(15,2),
  BAL_TRIAL NUMBER(15,2),
  SEQ_NO    VARCHAR2(10),
  SYNC_TIME VARCHAR2(14)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_POSTION_TRIAL.BANK_CODE
  is '��������';
comment on column HT_TBL_POSTION_TRIAL.BANK_NAME
  is '������������';
comment on column HT_TBL_POSTION_TRIAL.ACCT_NO
  is '�����˻��˺�';
comment on column HT_TBL_POSTION_TRIAL.ACCT_NAME
  is '�����˻�����';
comment on column HT_TBL_POSTION_TRIAL.INTEREST
  is '����';
comment on column HT_TBL_POSTION_TRIAL.SETT_DATE
  is '�������� ';
comment on column HT_TBL_POSTION_TRIAL.BAL
  is '�˻����';
comment on column HT_TBL_POSTION_TRIAL.RESV_BAL
  is '�������';
comment on column HT_TBL_POSTION_TRIAL.BAL_TRIAL
  is '�������';
comment on column HT_TBL_POSTION_TRIAL.SEQ_NO
  is '������ˮ';
comment on column HT_TBL_POSTION_TRIAL.SYNC_TIME
  is '����ʱ��';





create table HT_TBL_RISK_TXN
(
  INTER_BRH_CODE CHAR(10),
  SETT_DATE      VARCHAR2(8),
  TRANS_DATE     VARCHAR2(8) not null,
  TRANS_TIME     VARCHAR2(10),
  GATEWAY_KEY    VARCHAR2(48) not null,
  ORDER_NO       VARCHAR2(32),
  PAYWAY_TYPE    VARCHAR2(32),
  BUS_TP         CHAR(8),
  TRANS_TP       VARCHAR2(4),
  USER_ID        VARCHAR2(20),
  PAN            VARCHAR2(20),
  TRANS_AT       NUMBER(12,2),
  SETT_MCHT_NO   VARCHAR2(15) not null,
  EXT_MCHT_NO    VARCHAR2(15),
  RISK_REASON    VARCHAR2(255),
  REMARK         VARCHAR2(256)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_RISK_TXN.INTER_BRH_CODE
  is '�ڲ�������';
comment on column HT_TBL_RISK_TXN.SETT_DATE
  is '��������';
comment on column HT_TBL_RISK_TXN.TRANS_DATE
  is '���ؽ�������';
comment on column HT_TBL_RISK_TXN.TRANS_TIME
  is '���ؽ���ʱ��';
comment on column HT_TBL_RISK_TXN.GATEWAY_KEY
  is '������ˮ��';
comment on column HT_TBL_RISK_TXN.ORDER_NO
  is '������';
comment on column HT_TBL_RISK_TXN.PAYWAY_TYPE
  is '֧��ͨ��';
comment on column HT_TBL_RISK_TXN.BUS_TP
  is 'ҵ������';
comment on column HT_TBL_RISK_TXN.TRANS_TP
  is '��������';
comment on column HT_TBL_RISK_TXN.USER_ID
  is '�ͻ�ID';
comment on column HT_TBL_RISK_TXN.PAN
  is '����';
comment on column HT_TBL_RISK_TXN.TRANS_AT
  is '���׽��';
comment on column HT_TBL_RISK_TXN.SETT_MCHT_NO
  is '�����̻�����';
comment on column HT_TBL_RISK_TXN.EXT_MCHT_NO
  is '�����̻�����';
comment on column HT_TBL_RISK_TXN.RISK_REASON
  is '��ش���ԭ��';
comment on column HT_TBL_RISK_TXN.REMARK
  is '����';





create table HT_TBL_SETT_CHAN
(
  CHNL_ID    VARCHAR2(10) not null,
  CHNL_NAME  VARCHAR2(70),
  STATUS     VARCHAR2(8),
  MIN_AMT    NUMBER(15,2),
  MAX_AMT    NUMBER(15,2),
  BAL        NUMBER(15,2),
  INDEX_DAY  VARCHAR2(6),
  INDEX_TIME VARCHAR2(6),
  PERIOD     VARCHAR2(8),
  INDEX_FEE  VARCHAR2(6),
  SHADOM     VARCHAR2(22)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_SETT_CHAN.CHNL_ID
  is '�������';
comment on column HT_TBL_SETT_CHAN.CHNL_NAME
  is '��������';
comment on column HT_TBL_SETT_CHAN.STATUS
  is '����״̬';
comment on column HT_TBL_SETT_CHAN.MIN_AMT
  is '�������';
comment on column HT_TBL_SETT_CHAN.MAX_AMT
  is '�����޶�';
comment on column HT_TBL_SETT_CHAN.BAL
  is '�������';
comment on column HT_TBL_SETT_CHAN.INDEX_DAY
  is '�����չ���';
comment on column HT_TBL_SETT_CHAN.INDEX_TIME
  is 'ʱ�����';
comment on column HT_TBL_SETT_CHAN.PERIOD
  is '֧������';
comment on column HT_TBL_SETT_CHAN.INDEX_FEE
  is '���ʱ���';
comment on column HT_TBL_SETT_CHAN.SHADOM
  is 'Ӱ���˻�';





create table HT_TBL_SETT_FLOW
(
  INTER_BRH_CODE        CHAR(10) not null,
  SETT_DATE             VARCHAR2(8),
  TRANS_DATE            VARCHAR2(8),
  TRANS_TIME            VARCHAR2(10),
  FLAG_RESULT           CHAR(1),
  ACCT_STATU            CHAR(1),
  STATUS                CHAR(1),
  ORDER_NO              VARCHAR2(32),
  PLAT_KEY              VARCHAR2(48) not null,
  BATCH_NO              VARCHAR2(2),
  BUS_TP                CHAR(8),
  TRANS_TP              VARCHAR2(4),
  ACCT_TYPE             VARCHAR2(2),
  TRANS_STATE           CHAR(1),
  REVSAL_FLAG           CHAR(1),
  KEY_REVSAL            VARCHAR2(32),
  CANCEL_FLAG           CHAR(1),
  KEY_CANCEL            VARCHAR2(32),
  PAN                   CHAR(20),
  ACCT_NAME             VARCHAR2(60),
  PAN1                  CHAR(20),
  BANK_ORDER_NO         VARCHAR2(32),
  AMT_FEE               NUMBER(12,2),
  CCY                   CHAR(3),
  TRANS_AT              NUMBER(12,2),
  TRANS_AT2             NUMBER(12,2),
  TRANS_AT3             NUMBER(12,2),
  TRANS_AT4             NUMBER(12,2),
  TRANS_AT5             NUMBER(12,2),
  EXT_MCHT_NO           VARCHAR2(15),
  SETT_TP1              VARCHAR2(3),
  MCHT_NO1              VARCHAR2(15),
  UP_MCHT_NO1           VARCHAR2(15),
  MCHT_ROLE1            VARCHAR2(4),
  DEBT_AT_MCHT_NO1      NUMBER(12,2),
  CRET_AT_MCHT_NO1      NUMBER(12,2),
  DEBT_FEE_MCHT_NO1     NUMBER(12,2),
  CRET_FEE_MCHT_NO1     NUMBER(12,2),
  SHARE_PROFIT_MCHT_NO1 NUMBER(12,2),
  MCHT_RATE1            NUMBER(12,4),
  SETT_TP2              VARCHAR2(3),
  MCHT_NO2              VARCHAR2(15),
  UP_MCHT_NO2           VARCHAR2(15),
  MCHT_ROLE2            VARCHAR2(4),
  DEBT_AT_MCHT_NO2      NUMBER(12,2),
  CRET_AT_MCHT_NO2      NUMBER(12,2),
  DEBT_FEE_MCHT_NO2     NUMBER(12,2),
  CRET_FEE_MCHT_NO2     NUMBER(12,2),
  SHARE_PROFIT_MCHT_NO2 NUMBER(12,2),
  MCHT_RATE2            NUMBER(12,4),
  SETT_TP3              VARCHAR2(3),
  MCHT_NO3              VARCHAR2(15),
  UP_MCHT_NO3           VARCHAR2(15),
  MCHT_ROLE3            VARCHAR2(4),
  DEBT_AT_MCHT_NO3      NUMBER(12,2),
  CRET_AT_MCHT_NO3      NUMBER(12,2),
  DEBT_FEE_MCHT_NO3     NUMBER(12,2),
  CRET_FEE_MCHT_NO3     NUMBER(12,2),
  SHARE_PROFIT_MCHT_NO3 NUMBER(12,2),
  MCHT_RATE3            NUMBER(12,4),
  SETT_TP4              VARCHAR2(3),
  MCHT_NO4              VARCHAR2(15),
  UP_MCHT_NO4           VARCHAR2(15),
  MCHT_ROLE4            VARCHAR2(4),
  DEBT_AT_MCHT_NO4      NUMBER(12,2),
  CRET_AT_MCHT_NO4      NUMBER(12,2),
  DEBT_FEE_MCHT_NO4     NUMBER(12,2),
  CRET_FEE_MCHT_NO4     NUMBER(12,2),
  SHARE_PROFIT_MCHT_NO4 NUMBER(12,2),
  MCHT_RATE4            NUMBER(12,4),
  SETT_TP5              VARCHAR2(3),
  MCHT_NO5              VARCHAR2(15),
  UP_MCHT_NO5           VARCHAR2(15),
  MCHT_ROLE5            VARCHAR2(4),
  DEBT_AT_MCHT_NO5      NUMBER(12,2),
  CRET_AT_MCHT_NO5      NUMBER(12,2),
  DEBT_FEE_MCHT_NO5     NUMBER(12,2),
  CRET_FEE_MCHT_NO5     NUMBER(12,2),
  SHARE_PROFIT_MCHT_NO5 NUMBER(12,2),
  MCHT_RATE5            NUMBER(12,4),
  SETT_TP6              VARCHAR2(3),
  MCHT_NO6              VARCHAR2(15),
  UP_MCHT_NO6           VARCHAR2(15),
  MCHT_ROLE6            VARCHAR2(4),
  DEBT_AT_MCHT_NO6      NUMBER(12,2),
  CRET_AT_MCHT_NO6      NUMBER(12,2),
  DEBT_FEE_MCHT_NO6     NUMBER(12,2),
  CRET_FEE_MCHT_NO6     NUMBER(12,2),
  SHARE_PROFIT_MCHT_NO6 NUMBER(12,2),
  MCHT_RATE6            NUMBER(12,4),
  SETT_TP7              VARCHAR2(3),
  MCHT_NO7              VARCHAR2(15),
  UP_MCHT_NO7           VARCHAR2(15),
  MCHT_ROLE7            VARCHAR2(4),
  DEBT_AT_MCHT_NO7      NUMBER(12,2),
  CRET_AT_MCHT_NO7      NUMBER(12,2),
  DEBT_FEE_MCHT_NO7     NUMBER(12,2),
  CRET_FEE_MCHT_NO7     NUMBER(12,2),
  SHARE_PROFIT_MCHT_NO7 NUMBER(12,2),
  MCHT_RATE7            NUMBER(12,4),
  SETT_TP8              VARCHAR2(3),
  MCHT_NO8              VARCHAR2(15),
  UP_MCHT_NO8           VARCHAR2(15),
  MCHT_ROLE8            VARCHAR2(4),
  DEBT_AT_MCHT_NO8      NUMBER(12,2),
  CRET_AT_MCHT_NO8      NUMBER(12,2),
  DEBT_FEE_MCHT_NO8     NUMBER(12,2),
  CRET_FEE_MCHT_NO8     NUMBER(12,2),
  SHARE_PROFIT_MCHT_NO8 NUMBER(12,2),
  MCHT_RATE8            NUMBER(12,4),
  SETT_TP9              VARCHAR2(3),
  MCHT_NO9              VARCHAR2(15),
  UP_MCHT_NO9           VARCHAR2(15),
  MCHT_ROLE9            VARCHAR2(4),
  DEBT_AT_MCHT_NO9      NUMBER(12,2),
  CRET_AT_MCHT_NO9      NUMBER(12,2),
  DEBT_FEE_MCHT_NO9     NUMBER(12,2),
  CRET_FEE_MCHT_NO9     NUMBER(12,2),
  SHARE_PROFIT_MCHT_NO9 NUMBER(12,2),
  MCHT_RATE9            NUMBER(12,4),
  DEBT_AT_PLAT          NUMBER(12,2),
  CRET_AT_PLAT          NUMBER(12,2),
  DEBT_FEE_PLAT         NUMBER(12,2),
  CRET_FEE_PLAT         NUMBER(12,2),
  SHARE_PROFIT_PLAT     NUMBER(12,2),
  REMARK                VARCHAR2(512),
  SEQ_NUM               NUMBER(10)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_SETT_FLOW.INTER_BRH_CODE
  is '�ڲ�������';
comment on column HT_TBL_SETT_FLOW.SETT_DATE
  is '��������';
comment on column HT_TBL_SETT_FLOW.TRANS_DATE
  is '��������';
comment on column HT_TBL_SETT_FLOW.TRANS_TIME
  is '����ʱ��';
comment on column HT_TBL_SETT_FLOW.FLAG_RESULT
  is '���Ա�־';
comment on column HT_TBL_SETT_FLOW.ACCT_STATU
  is '����״̬';
comment on column HT_TBL_SETT_FLOW.STATUS
  is '����״̬';
comment on column HT_TBL_SETT_FLOW.ORDER_NO
  is '������';
comment on column HT_TBL_SETT_FLOW.PLAT_KEY
  is 'ƽ̨ͳһ��ˮ��';
comment on column HT_TBL_SETT_FLOW.BATCH_NO
  is 'ƽ̨���κ�';
comment on column HT_TBL_SETT_FLOW.BUS_TP
  is 'ҵ������';
comment on column HT_TBL_SETT_FLOW.TRANS_TP
  is '��������';
comment on column HT_TBL_SETT_FLOW.ACCT_TYPE
  is '�˻�����';
comment on column HT_TBL_SETT_FLOW.TRANS_STATE
  is '����״̬';
comment on column HT_TBL_SETT_FLOW.REVSAL_FLAG
  is '������־';
comment on column HT_TBL_SETT_FLOW.KEY_REVSAL
  is '������ԭʼ�����ƥ����';
comment on column HT_TBL_SETT_FLOW.CANCEL_FLAG
  is '������־';
comment on column HT_TBL_SETT_FLOW.KEY_CANCEL
  is '������ԭʼ�����ƥ����';
comment on column HT_TBL_SETT_FLOW.PAN
  is '�˻��˺�';
comment on column HT_TBL_SETT_FLOW.ACCT_NAME
  is '�˻�����';
comment on column HT_TBL_SETT_FLOW.PAN1
  is '�����׸��˺�';
comment on column HT_TBL_SETT_FLOW.BANK_ORDER_NO
  is '����������';
comment on column HT_TBL_SETT_FLOW.AMT_FEE
  is '��ҵ���д���������';
comment on column HT_TBL_SETT_FLOW.CCY
  is '����';
comment on column HT_TBL_SETT_FLOW.TRANS_AT
  is '���׽��';
comment on column HT_TBL_SETT_FLOW.TRANS_AT2
  is '���׽��2';
comment on column HT_TBL_SETT_FLOW.TRANS_AT3
  is '���׽��3';
comment on column HT_TBL_SETT_FLOW.TRANS_AT4
  is '���׽��4';
comment on column HT_TBL_SETT_FLOW.TRANS_AT5
  is '���׽��5';
comment on column HT_TBL_SETT_FLOW.EXT_MCHT_NO
  is '�����̻�����';
comment on column HT_TBL_SETT_FLOW.SETT_TP1
  is '����������1';
comment on column HT_TBL_SETT_FLOW.MCHT_NO1
  is '���������1';
comment on column HT_TBL_SETT_FLOW.UP_MCHT_NO1
  is '�ϼ��̻����';
comment on column HT_TBL_SETT_FLOW.MCHT_ROLE1
  is '�����ɫ1';
comment on column HT_TBL_SETT_FLOW.DEBT_AT_MCHT_NO1
  is '������1��Ǳ���';
comment on column HT_TBL_SETT_FLOW.CRET_AT_MCHT_NO1
  is '������1���Ǳ���';
comment on column HT_TBL_SETT_FLOW.DEBT_FEE_MCHT_NO1
  is '������1���������';
comment on column HT_TBL_SETT_FLOW.CRET_FEE_MCHT_NO1
  is '������1����������';
comment on column HT_TBL_SETT_FLOW.SHARE_PROFIT_MCHT_NO1
  is '������1����';
comment on column HT_TBL_SETT_FLOW.MCHT_RATE1
  is '������1��������';
comment on column HT_TBL_SETT_FLOW.SETT_TP2
  is '����������2';
comment on column HT_TBL_SETT_FLOW.MCHT_NO2
  is '���������2';
comment on column HT_TBL_SETT_FLOW.UP_MCHT_NO2
  is '�ϼ��̻����';
comment on column HT_TBL_SETT_FLOW.MCHT_ROLE2
  is '�����ɫ2';
comment on column HT_TBL_SETT_FLOW.DEBT_AT_MCHT_NO2
  is '������2��Ǳ���';
comment on column HT_TBL_SETT_FLOW.CRET_AT_MCHT_NO2
  is '������2���Ǳ���';
comment on column HT_TBL_SETT_FLOW.DEBT_FEE_MCHT_NO2
  is '������2���������';
comment on column HT_TBL_SETT_FLOW.CRET_FEE_MCHT_NO2
  is '������2����������';
comment on column HT_TBL_SETT_FLOW.SHARE_PROFIT_MCHT_NO2
  is '������2����';
comment on column HT_TBL_SETT_FLOW.MCHT_RATE2
  is '������2��������';
comment on column HT_TBL_SETT_FLOW.SETT_TP3
  is '����������3';
comment on column HT_TBL_SETT_FLOW.MCHT_NO3
  is '���������3';
comment on column HT_TBL_SETT_FLOW.UP_MCHT_NO3
  is '�ϼ��̻����';
comment on column HT_TBL_SETT_FLOW.MCHT_ROLE3
  is '�����ɫ3';
comment on column HT_TBL_SETT_FLOW.DEBT_AT_MCHT_NO3
  is '������3��Ǳ���';
comment on column HT_TBL_SETT_FLOW.CRET_AT_MCHT_NO3
  is '������3���Ǳ���';
comment on column HT_TBL_SETT_FLOW.DEBT_FEE_MCHT_NO3
  is '������3���������';
comment on column HT_TBL_SETT_FLOW.CRET_FEE_MCHT_NO3
  is '������3����������';
comment on column HT_TBL_SETT_FLOW.SHARE_PROFIT_MCHT_NO3
  is '������3����';
comment on column HT_TBL_SETT_FLOW.MCHT_RATE3
  is '������3��������';
comment on column HT_TBL_SETT_FLOW.SETT_TP4
  is '����������4';
comment on column HT_TBL_SETT_FLOW.MCHT_NO4
  is '���������4';
comment on column HT_TBL_SETT_FLOW.UP_MCHT_NO4
  is '�ϼ��̻����';
comment on column HT_TBL_SETT_FLOW.MCHT_ROLE4
  is '�����ɫ4';
comment on column HT_TBL_SETT_FLOW.DEBT_AT_MCHT_NO4
  is '������4��Ǳ���';
comment on column HT_TBL_SETT_FLOW.CRET_AT_MCHT_NO4
  is '������4���Ǳ���';
comment on column HT_TBL_SETT_FLOW.DEBT_FEE_MCHT_NO4
  is '������4���������';
comment on column HT_TBL_SETT_FLOW.CRET_FEE_MCHT_NO4
  is '������4����������';
comment on column HT_TBL_SETT_FLOW.SHARE_PROFIT_MCHT_NO4
  is '������4����';
comment on column HT_TBL_SETT_FLOW.MCHT_RATE4
  is '������4��������';
comment on column HT_TBL_SETT_FLOW.SETT_TP5
  is '����������5';
comment on column HT_TBL_SETT_FLOW.MCHT_NO5
  is '���������5';
comment on column HT_TBL_SETT_FLOW.UP_MCHT_NO5
  is '�ϼ��̻����';
comment on column HT_TBL_SETT_FLOW.MCHT_ROLE5
  is '�����ɫ5';
comment on column HT_TBL_SETT_FLOW.DEBT_AT_MCHT_NO5
  is '������5��Ǳ���';
comment on column HT_TBL_SETT_FLOW.CRET_AT_MCHT_NO5
  is '������5���Ǳ���';
comment on column HT_TBL_SETT_FLOW.DEBT_FEE_MCHT_NO5
  is '������5���������';
comment on column HT_TBL_SETT_FLOW.CRET_FEE_MCHT_NO5
  is '������5����������';
comment on column HT_TBL_SETT_FLOW.SHARE_PROFIT_MCHT_NO5
  is '������5����';
comment on column HT_TBL_SETT_FLOW.MCHT_RATE5
  is '������5��������';
comment on column HT_TBL_SETT_FLOW.SETT_TP6
  is '����������6';
comment on column HT_TBL_SETT_FLOW.MCHT_NO6
  is '���������6';
comment on column HT_TBL_SETT_FLOW.UP_MCHT_NO6
  is '�ϼ��̻����';
comment on column HT_TBL_SETT_FLOW.MCHT_ROLE6
  is '�����ɫ6';
comment on column HT_TBL_SETT_FLOW.DEBT_AT_MCHT_NO6
  is '������6��Ǳ���';
comment on column HT_TBL_SETT_FLOW.CRET_AT_MCHT_NO6
  is '������6���Ǳ���';
comment on column HT_TBL_SETT_FLOW.DEBT_FEE_MCHT_NO6
  is '������6���������';
comment on column HT_TBL_SETT_FLOW.CRET_FEE_MCHT_NO6
  is '������6����������';
comment on column HT_TBL_SETT_FLOW.SHARE_PROFIT_MCHT_NO6
  is '������6����';
comment on column HT_TBL_SETT_FLOW.MCHT_RATE6
  is '������6��������';
comment on column HT_TBL_SETT_FLOW.SETT_TP7
  is '����������7';
comment on column HT_TBL_SETT_FLOW.MCHT_NO7
  is '���������7';
comment on column HT_TBL_SETT_FLOW.UP_MCHT_NO7
  is '�ϼ��̻����';
comment on column HT_TBL_SETT_FLOW.MCHT_ROLE7
  is '�����ɫ7';
comment on column HT_TBL_SETT_FLOW.DEBT_AT_MCHT_NO7
  is '������7��Ǳ���';
comment on column HT_TBL_SETT_FLOW.CRET_AT_MCHT_NO7
  is '������7���Ǳ���';
comment on column HT_TBL_SETT_FLOW.DEBT_FEE_MCHT_NO7
  is '������7���������';
comment on column HT_TBL_SETT_FLOW.CRET_FEE_MCHT_NO7
  is '������7����������';
comment on column HT_TBL_SETT_FLOW.SHARE_PROFIT_MCHT_NO7
  is '������7����';
comment on column HT_TBL_SETT_FLOW.MCHT_RATE7
  is '������7��������';
comment on column HT_TBL_SETT_FLOW.SETT_TP8
  is '����������8';
comment on column HT_TBL_SETT_FLOW.MCHT_NO8
  is '���������8';
comment on column HT_TBL_SETT_FLOW.UP_MCHT_NO8
  is '�ϼ��̻����';
comment on column HT_TBL_SETT_FLOW.MCHT_ROLE8
  is '�����ɫ8';
comment on column HT_TBL_SETT_FLOW.DEBT_AT_MCHT_NO8
  is '������8��Ǳ���';
comment on column HT_TBL_SETT_FLOW.CRET_AT_MCHT_NO8
  is '������8���Ǳ���';
comment on column HT_TBL_SETT_FLOW.DEBT_FEE_MCHT_NO8
  is '������8���������';
comment on column HT_TBL_SETT_FLOW.CRET_FEE_MCHT_NO8
  is '������8����������';
comment on column HT_TBL_SETT_FLOW.SHARE_PROFIT_MCHT_NO8
  is '������8����';
comment on column HT_TBL_SETT_FLOW.MCHT_RATE8
  is '������8��������';
comment on column HT_TBL_SETT_FLOW.SETT_TP9
  is '����������9';
comment on column HT_TBL_SETT_FLOW.MCHT_NO9
  is '���������9';
comment on column HT_TBL_SETT_FLOW.UP_MCHT_NO9
  is '�ϼ��̻����';
comment on column HT_TBL_SETT_FLOW.MCHT_ROLE9
  is '�����ɫ9';
comment on column HT_TBL_SETT_FLOW.DEBT_AT_MCHT_NO9
  is '������9��Ǳ���';
comment on column HT_TBL_SETT_FLOW.CRET_AT_MCHT_NO9
  is '������9���Ǳ���';
comment on column HT_TBL_SETT_FLOW.DEBT_FEE_MCHT_NO9
  is '������9���������';
comment on column HT_TBL_SETT_FLOW.CRET_FEE_MCHT_NO9
  is '������9����������';
comment on column HT_TBL_SETT_FLOW.SHARE_PROFIT_MCHT_NO9
  is '������9����';
comment on column HT_TBL_SETT_FLOW.MCHT_RATE9
  is '������9��������';
comment on column HT_TBL_SETT_FLOW.DEBT_AT_PLAT
  is 'ƽ̨��Ǳ���';
comment on column HT_TBL_SETT_FLOW.CRET_AT_PLAT
  is 'ƽ̨���Ǳ���';
comment on column HT_TBL_SETT_FLOW.DEBT_FEE_PLAT
  is 'ƽ̨���������';
comment on column HT_TBL_SETT_FLOW.CRET_FEE_PLAT
  is 'ƽ̨����������';
comment on column HT_TBL_SETT_FLOW.SHARE_PROFIT_PLAT
  is 'ƽ̨����';
comment on column HT_TBL_SETT_FLOW.REMARK
  is '��ע';
comment on column HT_TBL_SETT_FLOW.SEQ_NUM
  is '���к�';





create table HT_TBL_SETT_LIST
(
  BRH_ID                 VARCHAR2(10) not null,
  SETT_DATE              VARCHAR2(8) not null,
  TRANS_DATE             VARCHAR2(8),
  MCHT_NO                VARCHAR2(15),
  MCHT_ROLE              VARCHAR2(4) not null,
  MCHT_RSLT_NO           VARCHAR2(32) not null,
  SEQ_NO                 VARCHAR2(20),
  PLAT_SETT_BANK         VARCHAR2(12),
  PLAT_SETT_BANK_NM      VARCHAR2(80),
  PLAT_SETT_ACCT_TYPE    VARCHAR2(1),
  PLAT_SETT_ACCT         VARCHAR2(22),
  PLAT_SETT_ACCT_NM      VARCHAR2(80),
  DC_FLAG                VARCHAR2(1) not null,
  MCHT_SETT_XINGMIN_ACCT VARCHAR2(40),
  MCHT_SETT_BANK         VARCHAR2(12),
  MCHT_SETT_BANK_NM      VARCHAR2(80),
  MCHT_SETT_ACCT_TYPE    VARCHAR2(1),
  MCHT_SETT_ACCT         VARCHAR2(22),
  MCHT_SETT_ACCT_NM      VARCHAR2(70),
  AMT                    NUMBER(15,2),
  AMT_FEE                NUMBER(15,2),
  PLAT_AMT_FEE           NUMBER(15,2),
  ACCT_STATU             CHAR(1),
  STATUS                 VARCHAR2(1),
  CHL_ID                 VARCHAR2(32),
  CHNL_NAME              VARCHAR2(70),
  FILE_NAME              VARCHAR2(100),
  PS                     VARCHAR2(80),
  RESERVE1               VARCHAR2(8),
  RESERVE2               VARCHAR2(16),
  RESERVE3               VARCHAR2(32),
  RESERVE4               VARCHAR2(64),
  RESERVE5               VARCHAR2(128)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_SETT_LIST.BRH_ID
  is '�ڲ�������';
comment on column HT_TBL_SETT_LIST.SETT_DATE
  is '��������';
comment on column HT_TBL_SETT_LIST.TRANS_DATE
  is '��������';
comment on column HT_TBL_SETT_LIST.MCHT_NO
  is '�̻���';
comment on column HT_TBL_SETT_LIST.MCHT_ROLE
  is '��ɫ����';
comment on column HT_TBL_SETT_LIST.MCHT_RSLT_NO
  is '�̻�������ˮ��';
comment on column HT_TBL_SETT_LIST.SEQ_NO
  is '������ˮ��';
comment on column HT_TBL_SETT_LIST.PLAT_SETT_BANK
  is 'ƽ̨������';
comment on column HT_TBL_SETT_LIST.PLAT_SETT_BANK_NM
  is 'ƽ̨����������';
comment on column HT_TBL_SETT_LIST.PLAT_SETT_ACCT_TYPE
  is '�����˻�����';
comment on column HT_TBL_SETT_LIST.PLAT_SETT_ACCT
  is '�����˻��˺�';
comment on column HT_TBL_SETT_LIST.PLAT_SETT_ACCT_NM
  is 'ƽ̨�����˻�����';
comment on column HT_TBL_SETT_LIST.DC_FLAG
  is '֧��/����';
comment on column HT_TBL_SETT_LIST.MCHT_SETT_XINGMIN_ACCT
  is '�̻������׸��˻�';
comment on column HT_TBL_SETT_LIST.MCHT_SETT_BANK
  is '�̻�������';
comment on column HT_TBL_SETT_LIST.MCHT_SETT_BANK_NM
  is '�̻�����������';
comment on column HT_TBL_SETT_LIST.MCHT_SETT_ACCT_TYPE
  is '�̻������˻�����';
comment on column HT_TBL_SETT_LIST.MCHT_SETT_ACCT
  is '�̻������˻��˺�';
comment on column HT_TBL_SETT_LIST.MCHT_SETT_ACCT_NM
  is '�̻������˻�����';
comment on column HT_TBL_SETT_LIST.AMT
  is '���˽��';
comment on column HT_TBL_SETT_LIST.AMT_FEE
  is '���˽������';
comment on column HT_TBL_SETT_LIST.PLAT_AMT_FEE
  is 'ƽ̨���㻮������';
comment on column HT_TBL_SETT_LIST.ACCT_STATU
  is '����״̬';
comment on column HT_TBL_SETT_LIST.STATUS
  is '����״̬';
comment on column HT_TBL_SETT_LIST.CHL_ID
  is '��������';
comment on column HT_TBL_SETT_LIST.CHNL_NAME
  is '��������';
comment on column HT_TBL_SETT_LIST.FILE_NAME
  is '�����ļ���';
comment on column HT_TBL_SETT_LIST.PS
  is '����(postscript)';
comment on column HT_TBL_SETT_LIST.RESERVE1
  is '����1';
comment on column HT_TBL_SETT_LIST.RESERVE2
  is '����1';
comment on column HT_TBL_SETT_LIST.RESERVE3
  is '����1';
comment on column HT_TBL_SETT_LIST.RESERVE4
  is '����1';
comment on column HT_TBL_SETT_LIST.RESERVE5
  is '����1';






create table HT_TBL_SETT_RSLT
(
  INTER_BRH_CODE          CHAR(10) not null,
  SETT_DATE               VARCHAR2(8) not null,
  TRANS_DATE              VARCHAR2(8),
  TRANS_TIME              VARCHAR2(10),
  ACCT_DATE               VARCHAR2(8),
  BATCH_NO                VARCHAR2(2),
  BUS_TP                  CHAR(8),
  TRANS_TP                VARCHAR2(4),
  MCHT_ROLE               VARCHAR2(4),
  MCHT_NO                 VARCHAR2(15),
  ACCT_TYPE               VARCHAR2(2),
  SETT_MD                 CHAR(2),
  MCHT_SETT_BANK          VARCHAR2(12),
  MCHT_SETT_BANK_NM       VARCHAR2(80),
  MCHT_SETT_ACCT_TYPE     VARCHAR2(1),
  MCHT_SETT_ACCT          VARCHAR2(40),
  MCHT_SETT_ACCT_NM       VARCHAR2(80),
  MCHT_SETT_XINGMIN_ACCT  VARCHAR2(40),
  MCHT_DEBIT_BANK         VARCHAR2(12),
  MCHT_DEBIT_BANK_NM      VARCHAR2(80),
  MCHT_DEBIT_ACCT_TYPE    CHAR(1),
  MCHT_DEBIT_ACCT         VARCHAR2(40),
  MCHT_DEBIT_ACCT_NM      VARCHAR2(80),
  PLAT_SETT_BANK          VARCHAR2(12),
  PLAT_SETT_BANK_NM       VARCHAR2(80),
  PLAT_SETT_ACCT_TYPE     CHAR(1),
  PLAT_SETT_ACCT          VARCHAR2(40),
  PLAT_SETT_ACCT_NM       VARCHAR2(80),
  PLAT_SETT_XINGMIN_ACCT  VARCHAR2(40),
  PLAT_DEBIT_BANK         VARCHAR2(12),
  PLAT_DEBIT_BANK_NM      VARCHAR2(80),
  PLAT_DEBIT_ACCT_TYPE    VARCHAR2(1),
  PLAT_DEBIT_ACCT         VARCHAR2(40),
  PLAT_DEBIT_ACCT_NM      VARCHAR2(80),
  PLAT_DEBIT_XINGMIN_ACCT VARCHAR2(40),
  DEBT_AT                 NUMBER(12,2),
  CRET_AT                 NUMBER(12,2),
  DEBT_NET_FEE            NUMBER(12,2),
  CRET_NET_FEE            NUMBER(12,2),
  SHARE_PROFIT            NUMBER(12,2),
  TRANS_NUM               NUMBER(10),
  MCHT_RATE               NUMBER(12,3),
  REMARK                  VARCHAR2(512),
  SEQ_NUM                 NUMBER(10) not null
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );





create table HT_TBL_TXN_ALGO_RULE
(
  BUS_TP      CHAR(8) not null,
  TXN_NUM     CHAR(4) not null,
  MATCHSTR    VARCHAR2(25),
  ALGO_ID     CHAR(5),
  PARAM_FLAG  CHAR(32),
  REC_OPR_ID  CHAR(1),
  REC_UPD_OPR CHAR(11),
  REC_CRT_TS  CHAR(14),
  REC_UPD_TS  CHAR(14)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_TXN_ALGO_RULE.BUS_TP
  is 'ҵ������';
comment on column HT_TBL_TXN_ALGO_RULE.TXN_NUM
  is '�ڲ�������';
comment on column HT_TBL_TXN_ALGO_RULE.ALGO_ID
  is '����ID';
comment on column HT_TBL_TXN_ALGO_RULE.PARAM_FLAG
  is '�����ֶ�';
comment on column HT_TBL_TXN_ALGO_RULE.REC_UPD_OPR
  is '�޸Ĺ�Ա��';
comment on column HT_TBL_TXN_ALGO_RULE.REC_CRT_TS
  is '��¼����ʱ��';
comment on column HT_TBL_TXN_ALGO_RULE.REC_UPD_TS
  is '��¼�޸�ʱ��';





create table HT_TBL_WIFT_TXN
(
  INTER_BRH_CODE    CHAR(10) not null,
  SETT_DATE         VARCHAR2(8) not null,
  TRANS_DATE        VARCHAR2(8),
  TRANS_TIME        VARCHAR2(10),
  FLAG_RESULT       CHAR(1),
  GENER_ID          CHAR(20),
  THR_ORDER_NO      VARCHAR2(48),
  THR_MCHT_NO       VARCHAR2(32),
  EXT_MCHT_NO       VARCHAR2(32),
  EX_EXT_MCHT_NO    VARCHAR2(32),
  EQUIPMENT_ID      VARCHAR2(100),
  WEHAT_MCHT_NO     VARCHAR2(32),
  ORDER_NO          VARCHAR2(48),
  USER_ID           VARCHAR2(30),
  PLAT_KEY          VARCHAR2(48) not null,
  BATCH_NO          VARCHAR2(2),
  BUS_TP            VARCHAR2(8),
  TRANS_TP          VARCHAR2(4),
  WIFT_TRANS_TP     CHAR(48),
  TRANS_STATE       CHAR(30),
  PAY_BANK          VARCHAR2(15),
  WX_REFUND_ORDER   VARCHAR2(64) not null,
  MCHT_REFUND_ORDER VARCHAR2(32),
  REFUND_TYPE       VARCHAR2(10),
  REFUND_STATUS     VARCHAR2(40),
  GOODS_NAME        VARCHAR2(400),
  MCHT_DATA         VARCHAR2(500),
  WX_FEE            NUMBER(10,3),
  WX_RATE           VARCHAR2(10),
  ACCT_TYPE         VARCHAR2(2),
  PAN               CHAR(19),
  CCY               CHAR(3),
  TRANS_AT          CHAR(12),
  TRANS_AT2         CHAR(12),
  TRANS_AT3         CHAR(12),
  TRANS_AT4         CHAR(12),
  TRANS_AT5         CHAR(12),
  TRANS_AT6         CHAR(12),
  SEQ_NUM           NUMBER(10)
)
tablespace settle_tbs
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column HT_TBL_WIFT_TXN.INTER_BRH_CODE
  is '�ڲ�������';
comment on column HT_TBL_WIFT_TXN.SETT_DATE
  is '��������';
comment on column HT_TBL_WIFT_TXN.TRANS_DATE
  is '��������';
comment on column HT_TBL_WIFT_TXN.TRANS_TIME
  is '����ʱ��';
comment on column HT_TBL_WIFT_TXN.FLAG_RESULT
  is '���Ա�־';
comment on column HT_TBL_WIFT_TXN.EXT_MCHT_NO
  is '�����̻�����';
comment on column HT_TBL_WIFT_TXN.ORDER_NO
  is '�̻�������';
comment on column HT_TBL_WIFT_TXN.PLAT_KEY
  is 'ƽ̨ͳһ��ˮ��';
comment on column HT_TBL_WIFT_TXN.BATCH_NO
  is 'ƽ̨���κ�';
comment on column HT_TBL_WIFT_TXN.BUS_TP
  is 'ҵ������';
comment on column HT_TBL_WIFT_TXN.TRANS_TP
  is '��������';
comment on column HT_TBL_WIFT_TXN.WIFT_TRANS_TP
  is '΢��ͨ��������';
comment on column HT_TBL_WIFT_TXN.TRANS_STATE
  is '����״̬';
comment on column HT_TBL_WIFT_TXN.ACCT_TYPE
  is '�˻�����';
comment on column HT_TBL_WIFT_TXN.PAN
  is '�˻��˺�';
comment on column HT_TBL_WIFT_TXN.CCY
  is '����';
comment on column HT_TBL_WIFT_TXN.TRANS_AT
  is '���׽��';
comment on column HT_TBL_WIFT_TXN.TRANS_AT2
  is '���׽��2';
comment on column HT_TBL_WIFT_TXN.TRANS_AT3
  is '���׽��3';
comment on column HT_TBL_WIFT_TXN.TRANS_AT4
  is '���׽��4';
comment on column HT_TBL_WIFT_TXN.TRANS_AT5
  is '���׽��5';
comment on column HT_TBL_WIFT_TXN.SEQ_NUM
  is '���к�';


-----------------------------��������-------------------------------

create table TBL_ALIPAY_WECHAT_TXN
(
  INTER_BRH_CODE    CHAR(10) not null,
  SETT_DATE         VARCHAR2(8) not null,
  TRANS_DATE        VARCHAR2(8),
  TRANS_TIME        VARCHAR2(10),
  FLAG_RESULT       CHAR(1),
  GENER_ID          CHAR(20),
  THR_ORDER_NO      VARCHAR2(48),
  THR_MCHT_NO       VARCHAR2(32),
  EXT_MCHT_NO       VARCHAR2(32),
  EX_EXT_MCHT_NO    VARCHAR2(32),
  EQUIPMENT_ID      VARCHAR2(100),
  ORDER_NO          VARCHAR2(48),
  USER_ID           VARCHAR2(30),
  PLAT_KEY          VARCHAR2(48) not null,
  BATCH_NO          VARCHAR2(2),
  BUS_TP            VARCHAR2(8),
  TRANS_TP          VARCHAR2(4),
  THR_TRANS_TP     CHAR(48),
  TRANS_STATE       CHAR(30),
  PAY_BANK          VARCHAR2(15),
  MCHT_REFUND_ORDER VARCHAR2(32),
  REFUND_TYPE       VARCHAR2(10),
  REFUND_STATUS     VARCHAR2(40),
  GOODS_NAME        VARCHAR2(400),
  MCHT_DATA         VARCHAR2(500),
  WX_FEE            NUMBER(10,3),
  WX_RATE           VARCHAR2(10),
  ACCT_TYPE         VARCHAR2(2),
  PAN               CHAR(19),
  CCY               CHAR(3),
  TRANS_AT          CHAR(12),
  TRANS_AT2         CHAR(12),
  TRANS_AT3         CHAR(12),
  TRANS_AT4         CHAR(12),
  TRANS_AT5         CHAR(12),
  TRANS_AT6         CHAR(12),
  SEQ_NUM           NUMBER(10)
)
tablespace SETTLEMENT_SP
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
comment on column TBL_ALIPAY_WECHAT_TXN.INTER_BRH_CODE
  is '�ڲ�������';
comment on column TBL_ALIPAY_WECHAT_TXN.SETT_DATE
  is '��������';
comment on column TBL_ALIPAY_WECHAT_TXN.TRANS_DATE
  is '��������';
comment on column TBL_ALIPAY_WECHAT_TXN.TRANS_TIME
  is '����ʱ��';
comment on column TBL_ALIPAY_WECHAT_TXN.FLAG_RESULT
  is '���Ա�־';
comment on column TBL_ALIPAY_WECHAT_TXN.EXT_MCHT_NO
  is '�����̻�����';
comment on column TBL_ALIPAY_WECHAT_TXN.ORDER_NO
  is '�̻�������';
comment on column TBL_ALIPAY_WECHAT_TXN.PLAT_KEY
  is 'ƽ̨ͳһ��ˮ��';
comment on column TBL_ALIPAY_WECHAT_TXN.BATCH_NO
  is 'ƽ̨���κ�';
comment on column TBL_ALIPAY_WECHAT_TXN.BUS_TP
  is 'ҵ������';
comment on column TBL_ALIPAY_WECHAT_TXN.TRANS_TP
  is '��������';
comment on column TBL_ALIPAY_WECHAT_TXN.THR_TRANS_TP
  is 'ͨ����������';
comment on column TBL_ALIPAY_WECHAT_TXN.TRANS_STATE
  is '����״̬';
comment on column TBL_ALIPAY_WECHAT_TXN.ACCT_TYPE
  is '�˻�����';
comment on column TBL_ALIPAY_WECHAT_TXN.PAN
  is '�˻��˺�';
comment on column TBL_ALIPAY_WECHAT_TXN.CCY
  is '����';
comment on column TBL_ALIPAY_WECHAT_TXN.TRANS_AT
  is '���׽��';
comment on column TBL_ALIPAY_WECHAT_TXN.TRANS_AT2
  is '���׽��2';
comment on column TBL_ALIPAY_WECHAT_TXN.TRANS_AT3
  is '���׽��3';
comment on column TBL_ALIPAY_WECHAT_TXN.TRANS_AT4
  is '���׽��4';
comment on column TBL_ALIPAY_WECHAT_TXN.TRANS_AT5
  is '���׽��5';
comment on column TBL_ALIPAY_WECHAT_TXN.SEQ_NUM
  is '���к�';