-- Create table
drop table HT_TBL_BRH_FEE_INF;
create table HT_TBL_BRH_FEE_INF
(
  PAYWAY_CODE    VARCHAR2(50) not null,
  EXT_MCH_NO     VARCHAR2(32),
  TRAN_CODE      VARCHAR2(4),
  CARD_TYPE      VARCHAR2(4),
  FLOOR_AMOUNT   NUMBER(15,2),
  UPPER_AMOUNT   NUMBER(15,2),
  FEE_FLAG       CHAR(1),
  FEE_VALUE      NUMBER(16,3),
  FEE_MIN        NUMBER(15,2),
  FEE_MAX        NUMBER(15,2),
  REC_UPD_USR_ID VARCHAR2(40),
  REC_UPD_TS     CHAR(14),
  REC_CRT_TS     CHAR(14),
  BRH_NM         VARCHAR2(40)
)
tablespace BASE_TBS_1
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 16K
    next 8K
    minextents 1
    maxextents unlimited
  );
-- Add comments to the table 
comment on table HT_TBL_BRH_FEE_INF
  is 'ͨ�����ʱ�';
-- Add comments to the columns 
comment on column HT_TBL_BRH_FEE_INF.PAYWAY_CODE
  is '�������';
comment on column HT_TBL_BRH_FEE_INF.TRAN_CODE
  is '��������';
comment on column HT_TBL_BRH_FEE_INF.CARD_TYPE
  is '����:00:Ĭ��;01:���;02:����;03:����';
comment on column HT_TBL_BRH_FEE_INF.FLOOR_AMOUNT
  is '����շѽ��(�ֶ�)';
comment on column HT_TBL_BRH_FEE_INF.UPPER_AMOUNT
  is '����շѽ��(�ֶ�)';
comment on column HT_TBL_BRH_FEE_INF.FEE_FLAG
  is '��Ӷ����:1:�����շѹ̶�ֵ;2:�������շ�';
comment on column HT_TBL_BRH_FEE_INF.FEE_VALUE
  is '��Ӷֵ:����������FEE_FLAGΪ1:�̶�ֵʱ,��ֵΪ�����ѽ��;����������FLAGΪ 2:������ʱ,��ֵΪ�����ѱ���';
comment on column HT_TBL_BRH_FEE_INF.FEE_MIN
  is '���������:����������շ�';
comment on column HT_TBL_BRH_FEE_INF.FEE_MAX
  is '���������:����������շ�';
comment on column HT_TBL_BRH_FEE_INF.REC_UPD_USR_ID
  is '��¼�޸Ĳ���Ա';
comment on column HT_TBL_BRH_FEE_INF.REC_UPD_TS
  is '��¼�޸�ʱ��';
comment on column HT_TBL_BRH_FEE_INF.REC_CRT_TS
  is '��¼����ʱ��';
comment on column HT_TBL_BRH_FEE_INF.BRH_NM
  is '��������';
-- Create/Recreate primary, unique and foreign key constraints 
alter table HT_TBL_BRH_FEE_INF
  add constraint INDEX_BRH_FEE_INF unique (PAYWAY_CODE, TRAN_CODE, CARD_TYPE)
  using index 
  tablespace BASE_TBS_1
  pctfree 10
  initrans 2
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
-- Create/Recreate indexes 
create index INDEX_BRH_FEE_INF_CARDTYPE on HT_TBL_BRH_FEE_INF (CARD_TYPE)
  tablespace BASE_TBS_1
  pctfree 10
  initrans 2
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );
create index INDEX_BRH_FEE_INF_TRANCODE on HT_TBL_BRH_FEE_INF (TRAN_CODE)
  tablespace BASE_TBS_1
  pctfree 10
  initrans 2
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );


-- Create table
drop table HT_TBL_BRH_INF;
create table HT_TBL_BRH_INF
(
  BRH_NO               VARCHAR2(12) not null,
  BRH_NM               VARCHAR2(30) not null,
  BRH_ROLE             CHAR(2) not null,
  PAY_BRH              VARCHAR2(2) not null,
  BRH_ACCT_TYPE        CHAR(2),
  BRH_ACCT_NO          VARCHAR2(40),
  BRH_ACCT_NM          VARCHAR2(80),
  BRH_ACCT_BANK        VARCHAR2(12),
  BRH_ACCT_BANKNM      VARCHAR2(80),
  PLAT_DEBIT_BANK      VARCHAR2(12),
  PLAT_DEBIT_ACCT_TYPE CHAR(2),
  PLAT_DEBIT_ACCT      VARCHAR2(40),
  PLAT_DEBIT_ACCT_NM   VARCHAR2(80),
  CONTACT              VARCHAR2(30),
  COMM_MOBIL           VARCHAR2(18),
  COMM_TEL             VARCHAR2(18),
  EMAIL                VARCHAR2(20),
  MANAGER              VARCHAR2(32),
  IDENTITY_NO          VARCHAR2(20),
  MANAGER_TEL          VARCHAR2(24),
  REG_ADDR             VARCHAR2(100),
  REC_UPD_USR_ID       VARCHAR2(40),
  REC_UPD_TS           CHAR(8),
  REC_CRT_TS           CHAR(8),
  FLAG                 CHAR(1) default 0
)
tablespace BASE_TBS_1
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 16K
    next 8K
    minextents 1
    maxextents unlimited
  );
-- Add comments to the table 
comment on table HT_TBL_BRH_INF
  is '������Ϣ��';
-- Add comments to the columns 
comment on column HT_TBL_BRH_INF.BRH_NO
  is '�������';
comment on column HT_TBL_BRH_INF.BRH_NM
  is '��������';
comment on column HT_TBL_BRH_INF.BRH_ROLE
  is '������ɫ��01 ��Ӫ����02 ֧������03 �������04 �����̻���';
comment on column HT_TBL_BRH_INF.PAY_BRH
  is '֧��������00- ΢��01- ֧����02- �������03- ��ҵ����04- �����׸�';
comment on column HT_TBL_BRH_INF.BRH_ACCT_TYPE
  is '���������˻�����';
comment on column HT_TBL_BRH_INF.BRH_ACCT_NO
  is '���������˻��˺�';
comment on column HT_TBL_BRH_INF.BRH_ACCT_NM
  is '���������˻�����';
comment on column HT_TBL_BRH_INF.BRH_ACCT_BANK
  is '���������˻������к�';
comment on column HT_TBL_BRH_INF.BRH_ACCT_BANKNM
  is '���������˻���������';
comment on column HT_TBL_BRH_INF.PLAT_DEBIT_BANK
  is 'ƽ̨������';
comment on column HT_TBL_BRH_INF.PLAT_DEBIT_ACCT_TYPE
  is 'ƽ̨�����˻�����';
comment on column HT_TBL_BRH_INF.PLAT_DEBIT_ACCT
  is 'ƽ̨�����˻��˺�';
comment on column HT_TBL_BRH_INF.PLAT_DEBIT_ACCT_NM
  is 'ƽ̨�����˻�����';
comment on column HT_TBL_BRH_INF.CONTACT
  is '��ϵ������';
comment on column HT_TBL_BRH_INF.COMM_MOBIL
  is '��ϵ���ֻ�';
comment on column HT_TBL_BRH_INF.COMM_TEL
  is '��ϵ�˵绰';
comment on column HT_TBL_BRH_INF.EMAIL
  is '��ϵ��EMAIL';
comment on column HT_TBL_BRH_INF.MANAGER
  is '��������';
comment on column HT_TBL_BRH_INF.IDENTITY_NO
  is '����֤������';
comment on column HT_TBL_BRH_INF.MANAGER_TEL
  is '������ϵ�绰';
comment on column HT_TBL_BRH_INF.REG_ADDR
  is 'ע���ַ';
comment on column HT_TBL_BRH_INF.REC_UPD_USR_ID
  is '��¼�޸Ĳ���Ա';
comment on column HT_TBL_BRH_INF.REC_UPD_TS
  is '��¼�޸�ʱ��';
comment on column HT_TBL_BRH_INF.REC_CRT_TS
  is '��¼����ʱ��';
comment on column HT_TBL_BRH_INF.FLAG
  is 'ɾ����־��0������1ɾ����';


-- Create table
drop table HT_TBL_EXT_MCHT_INF;
create table HT_TBL_EXT_MCHT_INF
(
  EXT_MCH_NO          VARCHAR2(32) not null,
  EXT_MCH_NM          VARCHAR2(64) not null,
  BRH_NO              CHAR(12) not null,
  BRH_NM              VARCHAR2(30) not null,
  SETTLE_TYPE         VARCHAR2(8) not null,
  EXT_MCH_ACCT_TYPE   CHAR(1),
  EXT_MCH_ACCT_NO     VARCHAR2(40),
  EXT_MCH_ACCT_NM     VARCHAR2(80),
  EXT_MCH_ACCT_BANK   VARCHAR2(12),
  EXT_MCH_ACCT_BANKNM VARCHAR2(80),
  EXT_FEE_ACCT_TYPE   CHAR(1),
  EXT_FEE_ACCT_NO     VARCHAR2(40),
  EXT_FEE_ACCT_NM     VARCHAR2(80),
  EXT_FEE_ACCT_BANK   VARCHAR2(12),
  EXT_FEEACCT_BANKNM  VARCHAR2(80),
  SPL_ACCT_TYPE       CHAR(1),
  SPL_BANK            VARCHAR2(12),
  SPL_ACCT            VARCHAR2(40),
  SPL_ACCT_NM         VARCHAR2(80),
  CONTACT             VARCHAR2(30),
  COMM_MOBIL          VARCHAR2(18),
  COMM_TEL            VARCHAR2(18),
  EMAIL               VARCHAR2(20),
  MANAGER             VARCHAR2(100),
  IDENTITY_NO         VARCHAR2(20),
  MANAGER_TEL         VARCHAR2(40),
  REG_ADDR            VARCHAR2(300),
  REC_UPD_USR_ID      VARCHAR2(40),
  REC_UPD_TS          CHAR(14),
  REC_CRT_TS          VARCHAR2(14),
  ESTATUS             VARCHAR2(8),
  CREATE_TIME         DATE,
  CREATER             VARCHAR2(32),
  UPDATE_TIME         DATE,
  EDITOR              VARCHAR2(32),
  RS_FLD1             VARCHAR2(30),
  RS_FLD2             VARCHAR2(100)
)
tablespace USERS
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
-- Add comments to the table 
comment on table HT_TBL_EXT_MCHT_INF
  is '�ⲿ�̻���Ϣ��';
-- Create/Recreate primary, unique and foreign key constraints 
alter table HT_TBL_EXT_MCHT_INF
  add constraint PK_HT_TBL_EXT_MCHT_INF primary key (EXT_MCH_NO)
  using index 
  tablespace USERS
  pctfree 10
  initrans 2
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );

-- Create table
drop table HT_TBL_FILE_INFO;
create table HT_TBL_FILE_INFO
(
  FILE_NAME VARCHAR2(100),
  BANK_CODE VARCHAR2(12) not null,
  SETT_DATE VARCHAR2(8) not null,
  STEP_NO   VARCHAR2(2) not null,
  BATCH_NO  VARCHAR2(2) not null,
  FILE_TYPE VARCHAR2(1) not null,
  FILE_FLAG VARCHAR2(1) not null,
  END_FLAG  VARCHAR2(1),
  SND_TIME  VARCHAR2(14),
  RCV_TIME  VARCHAR2(14),
  STATUS    VARCHAR2(1)
)
tablespace BASE_TBS_1
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
-- Add comments to the columns 
comment on column HT_TBL_FILE_INFO.FILE_NAME
  is '�ļ���';
comment on column HT_TBL_FILE_INFO.BANK_CODE
  is 'ƽ̨������ ����-HOST ����-�к�';
comment on column HT_TBL_FILE_INFO.SETT_DATE
  is '��������';
comment on column HT_TBL_FILE_INFO.STEP_NO
  is '����� 01-���˲�ƽ�ļ� 02-�����ļ� 03-���н����˺�����ļ� 04-������ļ����ļ� 05-���л����ļ�';
comment on column HT_TBL_FILE_INFO.BATCH_NO
  is '���κ�';
comment on column HT_TBL_FILE_INFO.FILE_TYPE
  is '�ļ����� H-���ļ����ļ� B-���л����ļ� M-���й鼯�ļ�';
comment on column HT_TBL_FILE_INFO.FILE_FLAG
  is '�ļ����� R-��Ӧ�ļ� S-�����ļ�';
comment on column HT_TBL_FILE_INFO.END_FLAG
  is '�����ļ�������־ Y-���ļ���Ҫ���� N-�����ļ��账��';
comment on column HT_TBL_FILE_INFO.SND_TIME
  is '����ʱ��';
comment on column HT_TBL_FILE_INFO.RCV_TIME
  is '����ʱ��';
comment on column HT_TBL_FILE_INFO.STATUS
  is '�ļ�����״̬';
-- Create/Recreate primary, unique and foreign key constraints 
alter table HT_TBL_FILE_INFO
  add constraint PK_HT_TBL_FILE_INFO primary key (SETT_DATE, BANK_CODE, STEP_NO, BATCH_NO, FILE_TYPE, FILE_FLAG)
  using index 
  tablespace BASE_TBS_1
  pctfree 10
  initrans 2
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );


-- Create table
drop table HT_TBL_HIS_DISC_ALGO;
create table HT_TBL_HIS_DISC_ALGO
(
  DISC_ID        VARCHAR2(20) not null,
  TRAN_CODE      VARCHAR2(4),
  MCHT_NO        VARCHAR2(15),
  CARD_TYPE      CHAR(4),
  FLOOR_AMOUNT   NUMBER(15,2),
  UPPER_AMOUNT   NUMBER(15,2),
  FEE_FLAG       CHAR(1),
  FEE_VALUE      NUMBER(16,3),
  FEE_MIN        NUMBER(15,2),
  FEE_MAX        NUMBER(15,2),
  REC_UPD_USR_ID VARCHAR2(40),
  REC_UPD_TS     CHAR(14),
  REC_CRT_TS     CHAR(14)
)
tablespace BASE_TBS_1
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 16K
    next 8K
    minextents 1
    maxextents unlimited
  );
-- Add comments to the table 
comment on table HT_TBL_HIS_DISC_ALGO
  is '�̻����ʱ�';
-- Add comments to the columns 
comment on column HT_TBL_HIS_DISC_ALGO.DISC_ID
  is '�Ʒ��㷨ID';
comment on column HT_TBL_HIS_DISC_ALGO.TRAN_CODE
  is '��������';
comment on column HT_TBL_HIS_DISC_ALGO.MCHT_NO
  is '�̻����';
comment on column HT_TBL_HIS_DISC_ALGO.CARD_TYPE
  is '����:00:Ĭ��;01:���;02:����;03:����';
comment on column HT_TBL_HIS_DISC_ALGO.FLOOR_AMOUNT
  is '����շѽ��(�ֶ�)';
comment on column HT_TBL_HIS_DISC_ALGO.UPPER_AMOUNT
  is '����շѽ��(�ֶ�)';
comment on column HT_TBL_HIS_DISC_ALGO.FEE_FLAG
  is '��Ӷ����:1:�����շѹ̶�ֵ;2:�������շ�';
comment on column HT_TBL_HIS_DISC_ALGO.FEE_VALUE
  is '��Ӷֵ:����������FEE_FLAGΪ1:�̶�ֵʱ,��ֵΪ�����ѽ��;����������FLAGΪ 2:������ʱ,��ֵΪ�����ѱ���';
comment on column HT_TBL_HIS_DISC_ALGO.FEE_MIN
  is '���������:����������շ�';
comment on column HT_TBL_HIS_DISC_ALGO.FEE_MAX
  is '���������:����������շ�';
comment on column HT_TBL_HIS_DISC_ALGO.REC_UPD_USR_ID
  is '��¼�޸Ĳ���Ա';
comment on column HT_TBL_HIS_DISC_ALGO.REC_UPD_TS
  is '��¼�޸�ʱ��';
comment on column HT_TBL_HIS_DISC_ALGO.REC_CRT_TS
  is '��¼����ʱ��';
-- Create/Recreate primary, unique and foreign key constraints 
alter table HT_TBL_HIS_DISC_ALGO
  add constraint INDEX_HIS_DISC_ALGO_PAYWAYCODE unique (DISC_ID)
  using index 
  tablespace BASE_TBS_1
  pctfree 10
  initrans 2
  maxtrans 255
  storage
  (
    initial 64K
    next 1M
    minextents 1
    maxextents unlimited
  );


-- Create table
drop table HT_TBL_MCHT_INF;
create table HT_TBL_MCHT_INF
(
  MCHT_NO              VARCHAR2(15),
  MCHT_NM              VARCHAR2(60),
  UP_MCHT_NO           VARCHAR2(15),
  CONTACT              VARCHAR2(30),
  COMM_MOBIL           VARCHAR2(18),
  COMM_TEL             VARCHAR2(18),
  MANAGER              VARCHAR2(32),
  ARTIF_CERTIF_TP      VARCHAR2(2),
  IDENTITY_NO          VARCHAR2(20),
  MANAGER_TEL          VARCHAR2(12),
  REG_ADDR             VARCHAR2(60),
  OPER_NO              VARCHAR2(8),
  OPER_NM              VARCHAR2(10),
  MCHT_STATUS          VARCHAR2(10),
  CLEAR_FLAG           CHAR(1),
  SETT_MD              VARCHAR2(2),
  MCHT_SETT_BANK       VARCHAR2(12),
  MCHT_SETT_BANK_NM    VARCHAR2(80),
  MCHT_SETT_ACCT_TYPE  CHAR(2),
  MCHT_SETT_ACCT       VARCHAR2(40),
  MCHT_SETT_ACCT_NM    VARCHAR2(80),
  XINGMIN_PAY_ACCT     VARCHAR2(40),
  BUSINUSS_BRH_NO      VARCHAR2(12),
  MCHT_DEBIT_BANK      VARCHAR2(12),
  MCHT_DEBIT_BANK_NM   VARCHAR2(80),
  MCHT_DEBIT_ACCT_TYPE CHAR(2),
  MCHT_DEBIT_ACCT      VARCHAR2(40),
  MCHT_DEBIT_ACCT_NM   VARCHAR2(80),
  MCHT_ROLE            CHAR(2),
  JS_TYPE              CHAR(2),
  CLEAR_CYCLE          CHAR(3),
  SPLITTING_TYPE       CHAR(2),
  SPL_BRH_ROLE1        CHAR(12),
  SPL_BRH_NM1          VARCHAR2(30),
  SPL_ACCT_TYPE1       CHAR(2),
  SPL_ACCT_NO1         VARCHAR2(40),
  SPL_ACCT_NM1         VARCHAR2(80),
  SPL_ACCT_BANK1       VARCHAR2(12),
  SPL_ACCT_BANKNM1     VARCHAR2(80),
  SPL_ACCT_RATE1       NUMBER(16,2),
  SPL_ACCT_RET_FEE_FLAG1 CHAR(1),
  SPL_BRH_ROLE2        CHAR(12),
  SPL_BRH_NM2          VARCHAR2(30),
  SPL_ACCT_TYPE2       CHAR(2),
  SPL_ACCT_NO2         VARCHAR2(40),
  SPL_ACCT_NM2         VARCHAR2(80),
  SPL_ACCT_BANK2       VARCHAR2(12),
  SPL_ACCT_BANKNM2     VARCHAR2(80),
  SPL_ACCT_RATE2       NUMBER(16,2),
  SPL_BRH_ROLE3        CHAR(12),
  SPL_ACCT_RET_FEE_FLAG2 CHAR(1),
  SPL_BRH_NM3          VARCHAR2(30),
  SPL_ACCT_TYPE3       CHAR(2),
  SPL_ACCT_NO3         VARCHAR2(40),
  SPL_ACCT_NM3         VARCHAR2(80),
  SPL_ACCT_BANK3       VARCHAR2(12),
  SPL_ACCT_BANKNM3     VARCHAR2(80),
  SPL_ACCT_RATE3       NUMBER(16,2),
  SPL_ACCT_RET_FEE_FLAG3 CHAR(1),
  SPL_BRH_ROLE4        CHAR(12),
  SPL_BRH_NM4          VARCHAR2(30),
  SPL_ACCT_TYPE4       CHAR(2),
  SPL_ACCT_NO4         VARCHAR2(40),
  SPL_ACCT_NM4         VARCHAR2(80),
  SPL_ACCT_BANK4       VARCHAR2(12),
  SPL_ACCT_BANKNM4     VARCHAR2(80),
  SPL_ACCT_RATE4       NUMBER(16,2),
  SPL_ACCT_RET_FEE_FLAG4 CHAR(1),
  CRT_OPR_ID           VARCHAR2(40),
  REC_UPD_TS           VARCHAR2(14),
  UPD_OPR_ID           VARCHAR2(40),
  SEND_BILL_FLAG       VARCHAR2(3),
  EMAIL                VARCHAR2(128),
  RET_FEE_FLAG         CHAR(1),
  RECHARGE_FLAG        CHAR(1)
)
tablespace USERS
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

-- Create table
drop table HT_TBL_POSTION_INFO;
create table HT_TBL_POSTION_INFO
(
  EXT_MCHT_NO VARCHAR2(15),
  BANK_CODE   VARCHAR2(12),
  BANK_NAME   VARCHAR2(70),
  ACCT_NO     VARCHAR2(22),
  ACCT_NAME   VARCHAR2(70),
  INTEREST    NUMBER(15,3),
  SETT_DATE   VARCHAR2(8),
  ACCT_BAL    NUMBER(15,2),
  RESV_BAL    NUMBER(15,2),
  AVAI_BAL    NUMBER(15,2),
  BLOCK_BAL   NUMBER(15,2),
  CTRL_BAL    NUMBER(15,2),
  SYNC_TIME   VARCHAR2(14),
  BAL         NUMBER(15,2)
)
tablespace BASE_TBS_1
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
-- Add comments to the table 
comment on table HT_TBL_POSTION_INFO
  is 'ͷ����Ϣ��';
-- Add comments to the columns 
comment on column HT_TBL_POSTION_INFO.EXT_MCHT_NO
  is '�ⲿ�̻����';
comment on column HT_TBL_POSTION_INFO.BANK_CODE
  is '��������';
comment on column HT_TBL_POSTION_INFO.BANK_NAME
  is '������������';
comment on column HT_TBL_POSTION_INFO.ACCT_NO
  is '�������˺�';
comment on column HT_TBL_POSTION_INFO.ACCT_NAME
  is '�����˻�����';
comment on column HT_TBL_POSTION_INFO.INTEREST
  is '����';
comment on column HT_TBL_POSTION_INFO.SETT_DATE
  is '��������';
comment on column HT_TBL_POSTION_INFO.ACCT_BAL
  is '�˻����';
comment on column HT_TBL_POSTION_INFO.RESV_BAL
  is '�������';
comment on column HT_TBL_POSTION_INFO.AVAI_BAL
  is '�������';
comment on column HT_TBL_POSTION_INFO.BLOCK_BAL
  is '�������';
comment on column HT_TBL_POSTION_INFO.CTRL_BAL
  is '�������';
comment on column HT_TBL_POSTION_INFO.SYNC_TIME
  is '����ʱ��';
comment on column HT_TBL_POSTION_INFO.BAL
  is '���������';

drop table TBL_OPERATE_ANALYSIS;
create table TBL_OPERATE_ANALYSIS 
(
   INTER_BRH_CODE       VARCHAR2(10),
   INTER_BRH_NAME       VARCHAR2(32),
   SETT_DATE            VARCHAR2(8),
   TRANS_TP             VARCHAR2(8),
   TOTAL_NUM            NUMBER(12),
   TOTAL_AMT            NUMBER(16,2),
   EQ_IN_HOST_NUM       NUMBER(12),
   EQ_IN_HOST_AMT       NUMBER(16,2),
   UEQ_IN_HOST_NUM      NUMBER(12),
   UEQ_IN_HOST_AMT      NUMBER(16,2),
   UN_HOST_NUM          NUMBER(12),
   UN_HOST_AMT          NUMBER(16,2)
)
tablespace BASE_TBS_1
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
  comment on column TBL_OPERATE_ANALYSIS.INTER_BRH_CODE is
'֧��ͨ������';

comment on column TBL_OPERATE_ANALYSIS.INTER_BRH_NAME is
'֧��ͨ����';

comment on column TBL_OPERATE_ANALYSIS.SETT_DATE is
'��������';

comment on column TBL_OPERATE_ANALYSIS.TRANS_TP is
'��������';

comment on column TBL_OPERATE_ANALYSIS.TOTAL_NUM is
'�����ܱ���';

comment on column TBL_OPERATE_ANALYSIS.TOTAL_AMT is
'�����ܽ��';

comment on column TBL_OPERATE_ANALYSIS.EQ_IN_HOST_NUM is
'��ƽ�ͺ��ı���';

comment on column TBL_OPERATE_ANALYSIS.EQ_IN_HOST_AMT is
'��ƽ�ͺ��Ľ��';

comment on column TBL_OPERATE_ANALYSIS.UEQ_IN_HOST_NUM is
'��ƽ�ͺ��ı���';

comment on column TBL_OPERATE_ANALYSIS.UEQ_IN_HOST_AMT is
'��ƽ�ͺ��Ľ��';

comment on column TBL_OPERATE_ANALYSIS.UN_HOST_NUM is
'δ�ͺ��ı���';

comment on column TBL_OPERATE_ANALYSIS.UN_HOST_AMT is
'δ�ͺ��Ľ��';

drop table TBL_HOST_CHECK_REPORT;
  create table TBL_HOST_CHECK_REPORT 
(
   SETT_DATE            VARCHAR2(8),
   REAL_YES_BALANCE     NUMBER(16,2),
   REAL_TOD_BALANCE     NUMBER(16,2),
   REAL_CHANGE          NUMBER(16,2),
   CASH_INCOME          NUMBER(16,2),
   CASH_OUTCOME         NUMBER(16,2),
   FUND_CHANGE          NUMBER(16,2),
   HOST_YES_BALANCE     NUMBER(16,2),
   HOST_TOD_BALANCE     NUMBER(16,2),
   HOST_CHANGE          NUMBER(16,2)
)
tablespace BASE_TBS_1
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
  comment on column TBL_HOST_CHECK_REPORT.SETT_DATE is
'��������';

comment on column TBL_HOST_CHECK_REPORT.REAL_YES_BALANCE is
'ʵ���˻��������';

comment on column TBL_HOST_CHECK_REPORT.REAL_TOD_BALANCE is
'ʵ���˻��������';

comment on column TBL_HOST_CHECK_REPORT.REAL_CHANGE is
'ʵ���˻����';

comment on column TBL_HOST_CHECK_REPORT.CASH_INCOME is
'ͷ������';

comment on column TBL_HOST_CHECK_REPORT.CASH_OUTCOME is
'ͷ��֧��';

comment on column TBL_HOST_CHECK_REPORT.FUND_CHANGE is
'�ʽ���';

comment on column TBL_HOST_CHECK_REPORT.HOST_YES_BALANCE is
'�����˻��������';

comment on column TBL_HOST_CHECK_REPORT.HOST_TOD_BALANCE is
'�����˻��������';

comment on column TBL_HOST_CHECK_REPORT.HOST_CHANGE is
'�����˻����';


drop table TBL_KEY_CERT_INF;
create table TBL_KEY_CERT_INF 
(
   PAY_WAY              VARCHAR2(32)         not null,
   MERC_TYPE            VARCHAR2(1),
   MERC_NO              VARCHAR2(32),
   CHANNEL_CODE         VARCHAR2(32),
   PAYING_MERC_CODE     VARCHAR2(32),
   KEY                  VARCHAR2(128),
   MERC_CERT            VARCHAR2(128),
   STATUS               VARCHAR2(2),
   constraint PK_TBL_KEY_CERT_INF primary key (MERC_NO, CHANNEL_CODE,PAYING_MERC_CODE)
)
tablespace BASE_TBS_1
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