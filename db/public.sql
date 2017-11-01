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
  is '通道费率表';
-- Add comments to the columns 
comment on column HT_TBL_BRH_FEE_INF.PAYWAY_CODE
  is '机构编号';
comment on column HT_TBL_BRH_FEE_INF.TRAN_CODE
  is '交易类型';
comment on column HT_TBL_BRH_FEE_INF.CARD_TYPE
  is '卡种:00:默认;01:借记;02:贷记;03:虚拟';
comment on column HT_TBL_BRH_FEE_INF.FLOOR_AMOUNT
  is '最低收费金额(分段)';
comment on column HT_TBL_BRH_FEE_INF.UPPER_AMOUNT
  is '最高收费金额(分段)';
comment on column HT_TBL_BRH_FEE_INF.FEE_FLAG
  is '回佣类型:1:按笔收费固定值;2:按比例收费';
comment on column HT_TBL_BRH_FEE_INF.FEE_VALUE
  is '回佣值:当费率类型FEE_FLAG为1:固定值时,该值为手续费金额;当费率类型FLAG为 2:按比例时,该值为手续费比例';
comment on column HT_TBL_BRH_FEE_INF.FEE_MIN
  is '最低手续费:按比例最低收费';
comment on column HT_TBL_BRH_FEE_INF.FEE_MAX
  is '最高手续费:按比例最高收费';
comment on column HT_TBL_BRH_FEE_INF.REC_UPD_USR_ID
  is '记录修改操作员';
comment on column HT_TBL_BRH_FEE_INF.REC_UPD_TS
  is '记录修改时间';
comment on column HT_TBL_BRH_FEE_INF.REC_CRT_TS
  is '记录创建时间';
comment on column HT_TBL_BRH_FEE_INF.BRH_NM
  is '机构名称';
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
  is '机构信息表';
-- Add comments to the columns 
comment on column HT_TBL_BRH_INF.BRH_NO
  is '机构编号';
comment on column HT_TBL_BRH_INF.BRH_NM
  is '机构名称';
comment on column HT_TBL_BRH_INF.BRH_ROLE
  is '机构角色：01 运营机构02 支付机构03 接入机构04 服务商机构';
comment on column HT_TBL_BRH_INF.PAY_BRH
  is '支付机构：00- 微信01- 支付宝02- 银联快捷03- 兴业银行04- 星民易付';
comment on column HT_TBL_BRH_INF.BRH_ACCT_TYPE
  is '机构结算账户类型';
comment on column HT_TBL_BRH_INF.BRH_ACCT_NO
  is '机构结算账户账号';
comment on column HT_TBL_BRH_INF.BRH_ACCT_NM
  is '机构结算账户户名';
comment on column HT_TBL_BRH_INF.BRH_ACCT_BANK
  is '机构结算账户银行行号';
comment on column HT_TBL_BRH_INF.BRH_ACCT_BANKNM
  is '机构结算账户银行名称';
comment on column HT_TBL_BRH_INF.PLAT_DEBIT_BANK
  is '平台代扣行';
comment on column HT_TBL_BRH_INF.PLAT_DEBIT_ACCT_TYPE
  is '平台代扣账户类型';
comment on column HT_TBL_BRH_INF.PLAT_DEBIT_ACCT
  is '平台代扣账户账号';
comment on column HT_TBL_BRH_INF.PLAT_DEBIT_ACCT_NM
  is '平台代扣账户名称';
comment on column HT_TBL_BRH_INF.CONTACT
  is '联系人姓名';
comment on column HT_TBL_BRH_INF.COMM_MOBIL
  is '联系人手机';
comment on column HT_TBL_BRH_INF.COMM_TEL
  is '联系人电话';
comment on column HT_TBL_BRH_INF.EMAIL
  is '联系人EMAIL';
comment on column HT_TBL_BRH_INF.MANAGER
  is '法人姓名';
comment on column HT_TBL_BRH_INF.IDENTITY_NO
  is '法人证件号码';
comment on column HT_TBL_BRH_INF.MANAGER_TEL
  is '法人联系电话';
comment on column HT_TBL_BRH_INF.REG_ADDR
  is '注册地址';
comment on column HT_TBL_BRH_INF.REC_UPD_USR_ID
  is '记录修改操作员';
comment on column HT_TBL_BRH_INF.REC_UPD_TS
  is '记录修改时间';
comment on column HT_TBL_BRH_INF.REC_CRT_TS
  is '记录创建时间';
comment on column HT_TBL_BRH_INF.FLAG
  is '删除标志（0正常，1删除）';


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
  is '外部商户信息表';
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
  is '文件名';
comment on column HT_TBL_FILE_INFO.BANK_CODE
  is '平台结算行 核心-HOST 其他-行号';
comment on column HT_TBL_FILE_INFO.SETT_DATE
  is '清算日期';
comment on column HT_TBL_FILE_INFO.STEP_NO
  is '步骤号 01-对账不平文件 02-清算文件 03-银行结算账号余额文件 04-结算核心记账文件 05-银行划账文件';
comment on column HT_TBL_FILE_INFO.BATCH_NO
  is '批次号';
comment on column HT_TBL_FILE_INFO.FILE_TYPE
  is '文件种类 H-核心记账文件 B-银行划账文件 M-银行归集文件';
comment on column HT_TBL_FILE_INFO.FILE_FLAG
  is '文件类型 R-响应文件 S-请求文件';
comment on column HT_TBL_FILE_INFO.END_FLAG
  is '当天文件结束标志 Y-无文件需要处理 N-还有文件需处理';
comment on column HT_TBL_FILE_INFO.SND_TIME
  is '发送时间';
comment on column HT_TBL_FILE_INFO.RCV_TIME
  is '接收时间';
comment on column HT_TBL_FILE_INFO.STATUS
  is '文件处理状态';
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
  is '商户扣率表';
-- Add comments to the columns 
comment on column HT_TBL_HIS_DISC_ALGO.DISC_ID
  is '计费算法ID';
comment on column HT_TBL_HIS_DISC_ALGO.TRAN_CODE
  is '交易类型';
comment on column HT_TBL_HIS_DISC_ALGO.MCHT_NO
  is '商户编号';
comment on column HT_TBL_HIS_DISC_ALGO.CARD_TYPE
  is '卡种:00:默认;01:借记;02:贷记;03:虚拟';
comment on column HT_TBL_HIS_DISC_ALGO.FLOOR_AMOUNT
  is '最低收费金额(分段)';
comment on column HT_TBL_HIS_DISC_ALGO.UPPER_AMOUNT
  is '最高收费金额(分段)';
comment on column HT_TBL_HIS_DISC_ALGO.FEE_FLAG
  is '回佣类型:1:按笔收费固定值;2:按比例收费';
comment on column HT_TBL_HIS_DISC_ALGO.FEE_VALUE
  is '回佣值:当费率类型FEE_FLAG为1:固定值时,该值为手续费金额;当费率类型FLAG为 2:按比例时,该值为手续费比例';
comment on column HT_TBL_HIS_DISC_ALGO.FEE_MIN
  is '最低手续费:按比例最低收费';
comment on column HT_TBL_HIS_DISC_ALGO.FEE_MAX
  is '最高手续费:按比例最高收费';
comment on column HT_TBL_HIS_DISC_ALGO.REC_UPD_USR_ID
  is '记录修改操作员';
comment on column HT_TBL_HIS_DISC_ALGO.REC_UPD_TS
  is '记录修改时间';
comment on column HT_TBL_HIS_DISC_ALGO.REC_CRT_TS
  is '记录创建时间';
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
  is '头寸信息表';
-- Add comments to the columns 
comment on column HT_TBL_POSTION_INFO.EXT_MCHT_NO
  is '外部商户编号';
comment on column HT_TBL_POSTION_INFO.BANK_CODE
  is '结算银行';
comment on column HT_TBL_POSTION_INFO.BANK_NAME
  is '结算银行名称';
comment on column HT_TBL_POSTION_INFO.ACCT_NO
  is '备付金账号';
comment on column HT_TBL_POSTION_INFO.ACCT_NAME
  is '结算账户名称';
comment on column HT_TBL_POSTION_INFO.INTEREST
  is '利率';
comment on column HT_TBL_POSTION_INFO.SETT_DATE
  is '结算日期';
comment on column HT_TBL_POSTION_INFO.ACCT_BAL
  is '账户余额';
comment on column HT_TBL_POSTION_INFO.RESV_BAL
  is '保底余额';
comment on column HT_TBL_POSTION_INFO.AVAI_BAL
  is '可用余额';
comment on column HT_TBL_POSTION_INFO.BLOCK_BAL
  is '冻结余额';
comment on column HT_TBL_POSTION_INFO.CTRL_BAL
  is '控制余额';
comment on column HT_TBL_POSTION_INFO.SYNC_TIME
  is '更新时间';
comment on column HT_TBL_POSTION_INFO.BAL
  is '代付后余额';

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
'支付通道编码';

comment on column TBL_OPERATE_ANALYSIS.INTER_BRH_NAME is
'支付通道名';

comment on column TBL_OPERATE_ANALYSIS.SETT_DATE is
'清算日期';

comment on column TBL_OPERATE_ANALYSIS.TRANS_TP is
'交易类型';

comment on column TBL_OPERATE_ANALYSIS.TOTAL_NUM is
'交易总笔数';

comment on column TBL_OPERATE_ANALYSIS.TOTAL_AMT is
'交易总金额';

comment on column TBL_OPERATE_ANALYSIS.EQ_IN_HOST_NUM is
'对平送核心笔数';

comment on column TBL_OPERATE_ANALYSIS.EQ_IN_HOST_AMT is
'对平送核心金额';

comment on column TBL_OPERATE_ANALYSIS.UEQ_IN_HOST_NUM is
'不平送核心笔数';

comment on column TBL_OPERATE_ANALYSIS.UEQ_IN_HOST_AMT is
'不平送核心金额';

comment on column TBL_OPERATE_ANALYSIS.UN_HOST_NUM is
'未送核心笔数';

comment on column TBL_OPERATE_ANALYSIS.UN_HOST_AMT is
'未送核心金额';

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
'清算日期';

comment on column TBL_HOST_CHECK_REPORT.REAL_YES_BALANCE is
'实体账户昨日余额';

comment on column TBL_HOST_CHECK_REPORT.REAL_TOD_BALANCE is
'实体账户今日余额';

comment on column TBL_HOST_CHECK_REPORT.REAL_CHANGE is
'实体账户差额';

comment on column TBL_HOST_CHECK_REPORT.CASH_INCOME is
'头寸收入';

comment on column TBL_HOST_CHECK_REPORT.CASH_OUTCOME is
'头寸支出';

comment on column TBL_HOST_CHECK_REPORT.FUND_CHANGE is
'资金差额';

comment on column TBL_HOST_CHECK_REPORT.HOST_YES_BALANCE is
'虚拟账户昨日余额';

comment on column TBL_HOST_CHECK_REPORT.HOST_TOD_BALANCE is
'虚拟账户今日余额';

comment on column TBL_HOST_CHECK_REPORT.HOST_CHANGE is
'虚拟账户差额';


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