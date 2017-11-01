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
  is '字典表';
comment on column HT_DICTIONARY.AUTO_ID
  is '主键';
comment on column HT_DICTIONARY.DICTIONARY_ID
  is 'id';
comment on column HT_DICTIONARY.TYPE
  is '种类id';
comment on column HT_DICTIONARY.DICTIONARY_NAME
  is '字典名称';
comment on column HT_DICTIONARY.REMARK
  is '备注（种类名称）';





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
  is ' 下载进度表';
comment on column HT_DOWNLAND_RECORD.ID
  is 'id';
comment on column HT_DOWNLAND_RECORD.SHEET_NAME
  is '下载表格表名';
comment on column HT_DOWNLAND_RECORD.FLAG
  is '成功标示';
comment on column HT_DOWNLAND_RECORD.OPER_START_TIME
  is '操作开始时间';
comment on column HT_DOWNLAND_RECORD.OPER_END_TIME
  is '操作结束时间';
comment on column HT_DOWNLAND_RECORD.USER_NAME
  is '用户账号';
comment on column HT_DOWNLAND_RECORD.TABLE_NAME_C
  is '中文表名';
comment on column HT_DOWNLAND_RECORD.TABLE_NAME_E
  is '英文表名';
comment on column HT_DOWNLAND_RECORD.DOWN_LOAD_TYPE
  is '下载格式';
comment on column HT_DOWNLAND_RECORD.DATE_START
  is '开始/清算日期';
comment on column HT_DOWNLAND_RECORD.DATE_END
  is '结束日期';
comment on column HT_DOWNLAND_RECORD.QUERY_ID
  is '编号';
comment on column HT_DOWNLAND_RECORD.QUERY_NAME
  is '名称';
comment on column HT_DOWNLAND_RECORD.DELETE_FLAG
  is '是否删除';
comment on column HT_DOWNLAND_RECORD.EXPAND_1
  is '中文表名id关联字典表';
comment on column HT_DOWNLAND_RECORD.EXPAND_2
  is '备用字段2';
comment on column HT_DOWNLAND_RECORD.EXPAND_3
  is '备用字段3';
comment on column HT_DOWNLAND_RECORD.EXPAND_4
  is '备用字段4';
comment on column HT_DOWNLAND_RECORD.EXPAND_5
  is '备用字段5';
comment on column HT_DOWNLAND_RECORD.ZHONG_DUAN_HAO
  is '终端号';






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
  is ' 操作记录表';
comment on column HT_OPER_RECORD.ID
  is 'id';
comment on column HT_OPER_RECORD.USER_ID
  is '客户ID';
comment on column HT_OPER_RECORD.USER_NAME
  is '客户姓名';
comment on column HT_OPER_RECORD.IP
  is '请求客户端ip';
comment on column HT_OPER_RECORD.OPER_TIME
  is '操作时间';
comment on column HT_OPER_RECORD.CONTENT
  is '操作内容';
comment on column HT_OPER_RECORD.EXPAND_1
  is '备用字段1';
comment on column HT_OPER_RECORD.EXPAND_2
  is '备用字段2';
comment on column HT_OPER_RECORD.EXPAND_3
  is '备用字段3';
comment on column HT_OPER_RECORD.EXPAND_4
  is '备用字段4';
comment on column HT_OPER_RECORD.EXPAND_5
  is '备用字段5';






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
  is '账号id';
comment on column HT_TBL_ACCT_INF.BANK_CODE
  is '行号';
comment on column HT_TBL_ACCT_INF.ACCT_TYPE
  is '账号类型';
comment on column HT_TBL_ACCT_INF.ACCT_NO
  is '账号';
comment on column HT_TBL_ACCT_INF.ACCT_NAME
  is '账号名称';
comment on column HT_TBL_ACCT_INF.ACCOUNT_BALANCE
  is '账户余额';
comment on column HT_TBL_ACCT_INF.AVAILABLE_BALANCE
  is '可用余额';
comment on column HT_TBL_ACCT_INF.BLOCKED_BALANCE
  is '冻结余额';
comment on column HT_TBL_ACCT_INF.CONTROLLED_BALANCE
  is '控制余额';
comment on column HT_TBL_ACCT_INF.REC_CRT_TS
  is '最新创建时间';
comment on column HT_TBL_ACCT_INF.REC_UPD_TS
  is '最新更新时间';





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
  is '内部机构号';
comment on column HT_TBL_ACCT_RSLT.SETT_DATE
  is '清算日期';
comment on column HT_TBL_ACCT_RSLT.ACCT_DATE
  is '入账日期';
comment on column HT_TBL_ACCT_RSLT.BATCH_NO
  is '平台批次号';
comment on column HT_TBL_ACCT_RSLT.MCHT_RSLT_NO
  is '商户入账流水号';
comment on column HT_TBL_ACCT_RSLT.MCHT_NO
  is '商户号';
comment on column HT_TBL_ACCT_RSLT.MCHT_ROLE
  is '商户角色';
comment on column HT_TBL_ACCT_RSLT.TRANS_NUM
  is '交易笔数';
comment on column HT_TBL_ACCT_RSLT.DEBT_AT
  is '入账金额借';
comment on column HT_TBL_ACCT_RSLT.CRET_AT
  is '入账金额贷';
comment on column HT_TBL_ACCT_RSLT.DEBT_NET_FEE
  is '手续费净额借';
comment on column HT_TBL_ACCT_RSLT.CRET_NET_FEE
  is '手续费净额贷';
comment on column HT_TBL_ACCT_RSLT.SETT_MD
  is '商户收支模式';
comment on column HT_TBL_ACCT_RSLT.MCHT_SETT_BANK
  is '商户结算行';
comment on column HT_TBL_ACCT_RSLT.MCHT_SETT_BANK_NM
  is '商户结算行行名';
comment on column HT_TBL_ACCT_RSLT.MCHT_SETT_ACCT_TYPE
  is '商户结算账户类型';
comment on column HT_TBL_ACCT_RSLT.MCHT_SETT_ACCT
  is '商户结算账户账号';
comment on column HT_TBL_ACCT_RSLT.MCHT_SETT_ACCT_NM
  is '商户结算账户名称';
comment on column HT_TBL_ACCT_RSLT.MCHT_SETT_XINGMIN_ACCT
  is '商户星民易付账户';
comment on column HT_TBL_ACCT_RSLT.PLAT_SETT_BANK
  is '平台结算行';
comment on column HT_TBL_ACCT_RSLT.PLAT_SETT_BANK_NM
  is '平台结算行行名';
comment on column HT_TBL_ACCT_RSLT.PLAT_SETT_ACCT_TYPE
  is '平台结算账户类型';
comment on column HT_TBL_ACCT_RSLT.PLAT_SETT_ACCT
  is '平台结算账户账号';
comment on column HT_TBL_ACCT_RSLT.PLAT_SETT_ACCT_NM
  is '平台结算账户名称';
comment on column HT_TBL_ACCT_RSLT.PLAT_SETT_XINGMIN_ACCT
  is '平台结算星民易付账户';
comment on column HT_TBL_ACCT_RSLT.CRET_CHNL_ID
  is '收入划付渠道编号';
comment on column HT_TBL_ACCT_RSLT.CRET_CHNL_NM
  is '收入划付渠道名称';
comment on column HT_TBL_ACCT_RSLT.CRET_RSLT_NO
  is '收入划付流水号';
comment on column HT_TBL_ACCT_RSLT.MCHT_DEBIT_BANK
  is '商户支出行';
comment on column HT_TBL_ACCT_RSLT.MCHT_DEBIT_BANK_NM
  is '商户支出行行名';
comment on column HT_TBL_ACCT_RSLT.MCHT_DEBIT_ACCT_TYPE
  is '商户支出账户类型';
comment on column HT_TBL_ACCT_RSLT.MCHT_DEBIT_ACCT
  is '商户支出账户账号';
comment on column HT_TBL_ACCT_RSLT.MCHT_DEBIT_ACCT_NM
  is '商户支出账户名称';
comment on column HT_TBL_ACCT_RSLT.PLAT_DEBIT_BANK
  is '平台代扣行';
comment on column HT_TBL_ACCT_RSLT.PLAT_DEBIT_BANK_NM
  is '平台代扣行行名';
comment on column HT_TBL_ACCT_RSLT.PLAT_DEBIT_ACCT_TYPE
  is '平台代扣账户类型';
comment on column HT_TBL_ACCT_RSLT.PLAT_DEBIT_ACCT
  is '平台代扣账户账号';
comment on column HT_TBL_ACCT_RSLT.PLAT_DEBIT_ACCT_NM
  is '平台代扣账户名称';
comment on column HT_TBL_ACCT_RSLT.PLAT_DEBIT_XINGMIN_ACCT
  is '平台代扣星民易付账户';
comment on column HT_TBL_ACCT_RSLT.DEBT_CHNL_ID
  is '支出划付渠道编号';
comment on column HT_TBL_ACCT_RSLT.DEBT_CHNL_NM
  is '支出划付渠道名称';
comment on column HT_TBL_ACCT_RSLT.DEBT_RSLT_NO
  is '支出划付流水号';
comment on column HT_TBL_ACCT_RSLT.ACCT_STATU
  is '入账状态';
comment on column HT_TBL_ACCT_RSLT.SZ_STATUS
  is '收支状态';
comment on column HT_TBL_ACCT_RSLT.SEQ_NUM
  is '序号';






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
  is '内部机构号';
comment on column HT_TBL_ACTIVE_MCHT_INF.SETT_DATE
  is '清算日期';
comment on column HT_TBL_ACTIVE_MCHT_INF.MCHT_NO
  is '商户号';
comment on column HT_TBL_ACTIVE_MCHT_INF.MCHT_NM
  is '商户名称';
comment on column HT_TBL_ACTIVE_MCHT_INF.TRANS_NUM
  is '交易笔数';
comment on column HT_TBL_ACTIVE_MCHT_INF.TRANS_AT
  is '交易金额';
comment on column HT_TBL_ACTIVE_MCHT_INF.SEQ_NUM
  is '序号';






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
  is '序列号';
comment on column HT_TBL_ALGO_CTL.BUS_TP
  is '业务类型';
comment on column HT_TBL_ALGO_CTL.TXN_NUM
  is '交易类型';
comment on column HT_TBL_ALGO_CTL.BUS_NAME
  is '业务类型说明';
comment on column HT_TBL_ALGO_CTL.TXN_NAME
  is '交易类型说明';
comment on column HT_TBL_ALGO_CTL.C_D_FLAG
  is '借贷标记';
comment on column HT_TBL_ALGO_CTL.STLM_FLAG
  is '是否清算标志（1：清算，0：不清算）';
comment on column HT_TBL_ALGO_CTL.REC_UPD_OPR
  is '最新更新柜员';
comment on column HT_TBL_ALGO_CTL.REC_CRT_TS
  is '最新创建时间';
comment on column HT_TBL_ALGO_CTL.REC_UPD_TS
  is '最新更新时间';






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
  is '内部机构号';
comment on column HT_TBL_BRH_INFO.CUP_BRH_ID
  is '银联机构号';
comment on column HT_TBL_BRH_INFO.BRH_LEVEL
  is '级别';
comment on column HT_TBL_BRH_INFO.DESCR
  is '描述';





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
  is '外部机构码';
comment on column HT_TBL_BRH_MAP.BRH_ID
  is '内部机构码';
comment on column HT_TBL_BRH_MAP.DESCR
  is '说明';






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
  is '应用范围';
comment on column HT_TBL_BUF_CHG.I_BUF_CHG_INDEX
  is '数据赋值关系索引';
comment on column HT_TBL_BUF_CHG.I_SOUR_BUF_INDEX
  is '源BUF块索引';
comment on column HT_TBL_BUF_CHG.I_DEST_BUF_INDEX
  is '目的BUF块索引';
comment on column HT_TBL_BUF_CHG.SP_DSP
  is '作用描述';





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
  is '应用范围';
comment on column HT_TBL_BUF_DSP.I_BUF_DSP_INDEX
  is 'BUF块的索引';
comment on column HT_TBL_BUF_DSP.I_POS_INDEX
  is '顺序号';
comment on column HT_TBL_BUF_DSP.I_FLD_INDEX
  is '域索引';
comment on column HT_TBL_BUF_DSP.I_FLD_ID
  is '域字段号';
comment on column HT_TBL_BUF_DSP.I_FLD_OFFSET
  is '偏移量';






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
  is '内部机构号';
comment on column HT_TBL_CHECK_FLOW.SETT_DATE
  is '清算日期';
comment on column HT_TBL_CHECK_FLOW.TRANS_DATE
  is '交易日期';
comment on column HT_TBL_CHECK_FLOW.TRANS_TIME
  is '交易时间';
comment on column HT_TBL_CHECK_FLOW.FLAG_RESULT
  is '勾对标志';
comment on column HT_TBL_CHECK_FLOW.ORDER_NO
  is '订单号';
comment on column HT_TBL_CHECK_FLOW.PLAT_KEY
  is '平台统一流水号';
comment on column HT_TBL_CHECK_FLOW.BATCH_NO
  is '平台批次号';
comment on column HT_TBL_CHECK_FLOW.BUS_TP
  is '业务类型';
comment on column HT_TBL_CHECK_FLOW.TRANS_TP
  is '交易类型';
comment on column HT_TBL_CHECK_FLOW.ACCT_TYPE
  is '账户类型';
comment on column HT_TBL_CHECK_FLOW.TRANS_STATE
  is '交易状态';
comment on column HT_TBL_CHECK_FLOW.S_PAN
  is '平台结算账号';
comment on column HT_TBL_CHECK_FLOW.S_CCY
  is '平台币种';
comment on column HT_TBL_CHECK_FLOW.S_TRANS_NUM
  is '平台交易笔数';
comment on column HT_TBL_CHECK_FLOW.S_TRANS_AT
  is '平台交易金额';
comment on column HT_TBL_CHECK_FLOW.S_TRANS_FEE
  is '平台手续费';
comment on column HT_TBL_CHECK_FLOW.D_PAN
  is '对方结算账号';
comment on column HT_TBL_CHECK_FLOW.D_CCY
  is '对方币种';
comment on column HT_TBL_CHECK_FLOW.D_TRANS_NUM
  is '对方交易笔数';
comment on column HT_TBL_CHECK_FLOW.D_TRANS_AT
  is '对方交易金额';
comment on column HT_TBL_CHECK_FLOW.D_TRANS_FEE
  is '对方手续费';
comment on column HT_TBL_CHECK_FLOW.FEE_RATE
  is '试算费率';
comment on column HT_TBL_CHECK_FLOW.RESERVE
  is '备注';
comment on column HT_TBL_CHECK_FLOW.SEQ_NUM
  is '序号';






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
  is '内部机构号';
comment on column HT_TBL_CHILD_TIME.MISSION_INDEX
  is '任务索引号';
comment on column HT_TBL_CHILD_TIME.MISSION_DATE
  is '任务日期';
comment on column HT_TBL_CHILD_TIME.MISSION_WEEKDAY
  is '任务星期';
comment on column HT_TBL_CHILD_TIME.MISSION_HOUR
  is '任务时间';
comment on column HT_TBL_CHILD_TIME.CHILD_ID
  is '子序列号';
comment on column HT_TBL_CHILD_TIME.COMMIT_FLAG
  is '连接标记';
comment on column HT_TBL_CHILD_TIME.COMMIT_NUM
  is '连接数';
comment on column HT_TBL_CHILD_TIME.CHILD_BEGIN_POINT
  is '开始结点';
comment on column HT_TBL_CHILD_TIME.CHILD_END_POINT
  is '结束结点';
comment on column HT_TBL_CHILD_TIME.CHILD_FINISH_POINT
  is '完成结点';
comment on column HT_TBL_CHILD_TIME.CHILD_ERR_POINT
  is '错误点';
comment on column HT_TBL_CHILD_TIME.CHILD_STATUS
  is '子状态';






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
  is '内部机构号';
comment on column HT_TBL_CIB_TXN.SETT_DATE
  is '清算日期';
comment on column HT_TBL_CIB_TXN.EPAYTIME
  is 'EPAY系统时间（自然日）';
comment on column HT_TBL_CIB_TXN.ORDERDATE
  is '订单日期';
comment on column HT_TBL_CIB_TXN.TRANSDATE
  is '交易日期（自然日）';
comment on column HT_TBL_CIB_TXN.PLAT_KEY
  is '平台统一流水号';
comment on column HT_TBL_CIB_TXN.BATCH_NO
  is '平台批次号';
comment on column HT_TBL_CIB_TXN.FLAG_RESULT
  is '勾对标志';
comment on column HT_TBL_CIB_TXN.SNO
  is '银行系统流水号';
comment on column HT_TBL_CIB_TXN.MRCHNO
  is '商户号';
comment on column HT_TBL_CIB_TXN.ORDER_NO
  is '订单编号';
comment on column HT_TBL_CIB_TXN.ORDERAMT
  is '支付金额';
comment on column HT_TBL_CIB_TXN.FEEAMT
  is '手续费';
comment on column HT_TBL_CIB_TXN.BUS_TP
  is '业务类型';
comment on column HT_TBL_CIB_TXN.TRANS_TP
  is '交易类型';
comment on column HT_TBL_CIB_TXN.TRANS_STATE
  is '交易状态';
comment on column HT_TBL_CIB_TXN.ACCT_TYPE
  is '账户类型';
comment on column HT_TBL_CIB_TXN.PAN
  is '账户账号';
comment on column HT_TBL_CIB_TXN.CCY
  is '币种';
comment on column HT_TBL_CIB_TXN.SEQ_NUM
  is '序列号';





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
  is '内部机构号';
comment on column HT_TBL_CMP_FLOW.SETT_DATE
  is '清算日期';
comment on column HT_TBL_CMP_FLOW.TRANS_DATE
  is '交易日期';
comment on column HT_TBL_CMP_FLOW.TRANS_TIME
  is '交易时间';
comment on column HT_TBL_CMP_FLOW.FLAG_RESULT
  is '勾对标志';
comment on column HT_TBL_CMP_FLOW.ORDER_NO
  is '订单号';
comment on column HT_TBL_CMP_FLOW.PLAT_KEY
  is '平台统一流水号';
comment on column HT_TBL_CMP_FLOW.BATCH_NO
  is '平台批次号';
comment on column HT_TBL_CMP_FLOW.BUS_TP
  is '业务类型';
comment on column HT_TBL_CMP_FLOW.TRANS_TP
  is '交易类型';
comment on column HT_TBL_CMP_FLOW.ACCT_TYPE
  is '账户类型';
comment on column HT_TBL_CMP_FLOW.TRANS_STATE
  is '交易状态';
comment on column HT_TBL_CMP_FLOW.REVSAL_FLAG
  is '冲正标志';
comment on column HT_TBL_CMP_FLOW.KEY_REVSAL
  is '冲正与原始请求的匹配域';
comment on column HT_TBL_CMP_FLOW.CANCEL_FLAG
  is '撤销标志';
comment on column HT_TBL_CMP_FLOW.KEY_CANCEL
  is '撤销与原始请求的匹配域';
comment on column HT_TBL_CMP_FLOW.PAN
  is '账户账号';
comment on column HT_TBL_CMP_FLOW.ACCT_NAME
  is '账户名称';
comment on column HT_TBL_CMP_FLOW.PAN1
  is '星民易付账号';
comment on column HT_TBL_CMP_FLOW.CCY
  is '币种';
comment on column HT_TBL_CMP_FLOW.TRANS_AT
  is '交易金额';
comment on column HT_TBL_CMP_FLOW.TRANS_AT2
  is '交易金额2';
comment on column HT_TBL_CMP_FLOW.TRANS_AT3
  is '交易金额3';
comment on column HT_TBL_CMP_FLOW.TRANS_AT4
  is '交易金额4';
comment on column HT_TBL_CMP_FLOW.TRANS_AT5
  is '交易金额5';
comment on column HT_TBL_CMP_FLOW.EXT_MCHT_NO
  is '对外商户编码';
comment on column HT_TBL_CMP_FLOW.SETT_TP1
  is '合作方类型1';
comment on column HT_TBL_CMP_FLOW.MCHT_NO1
  is '合作方编号1';
comment on column HT_TBL_CMP_FLOW.SETT_TP2
  is '合作方类型2';
comment on column HT_TBL_CMP_FLOW.MCHT_NO2
  is '合作方编号2';
comment on column HT_TBL_CMP_FLOW.SETT_TP3
  is '合作方类型3';
comment on column HT_TBL_CMP_FLOW.MCHT_NO3
  is '合作方编号3';
comment on column HT_TBL_CMP_FLOW.SETT_TP4
  is '合作方类型4';
comment on column HT_TBL_CMP_FLOW.MCHT_NO4
  is '合作方编号4';
comment on column HT_TBL_CMP_FLOW.SETT_TP5
  is '合作方类型5';
comment on column HT_TBL_CMP_FLOW.MCHT_NO5
  is '合作方编号5';
comment on column HT_TBL_CMP_FLOW.SETT_TP6
  is '合作方类型6';
comment on column HT_TBL_CMP_FLOW.MCHT_NO6
  is '合作方编号6';
comment on column HT_TBL_CMP_FLOW.SETT_TP7
  is '合作方类型7';
comment on column HT_TBL_CMP_FLOW.MCHT_NO7
  is '合作方编号7';
comment on column HT_TBL_CMP_FLOW.SETT_TP8
  is '合作方类型8';
comment on column HT_TBL_CMP_FLOW.MCHT_NO8
  is '合作方编号8';
comment on column HT_TBL_CMP_FLOW.SETT_TP9
  is '合作方类型9';
comment on column HT_TBL_CMP_FLOW.MCHT_NO9
  is '合作方编号9';
comment on column HT_TBL_CMP_FLOW.SEQ_NUM
  is '序列号';






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
  is '数据值1';
comment on column HT_TBL_COM_CONFIG.VALUE2
  is '数据值2';
comment on column HT_TBL_COM_CONFIG.VALUE3
  is '数据值3';
comment on column HT_TBL_COM_CONFIG.REMARK1
  is '说明1';
comment on column HT_TBL_COM_CONFIG.REMARK2
  is '说明2';
comment on column HT_TBL_COM_CONFIG.REMARK3
  is '说明3';





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
  is '应用范围';
comment on column HT_TBL_CON_INF.TXN_NUM
  is '内部交易代码';
comment on column HT_TBL_CON_INF.CON_INDEX
  is '条件序号';
comment on column HT_TBL_CON_INF.FLD_INDEX
  is '条件发生的位序号';
comment on column HT_TBL_CON_INF.BEGIN_BYTE_POS
  is '条件在位的起始位置';
comment on column HT_TBL_CON_INF.FORMAT_CHG_NEED
  is '条件的值是否需要转换';
comment on column HT_TBL_CON_INF.VAL
  is '具体的条件';






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
  is '流水号';
comment on column HT_TBL_COST_CMP_INF.SETT_DATE
  is '清算/入账日期';
comment on column HT_TBL_COST_CMP_INF.TRANS_TP
  is '交易类型';
comment on column HT_TBL_COST_CMP_INF.TRANS_AMT
  is '交易/入账金额';
comment on column HT_TBL_COST_CMP_INF.FEE_TYPE
  is '平台计费方式';
comment on column HT_TBL_COST_CMP_INF.FEE_RATE
  is '平台费率';
comment on column HT_TBL_COST_CMP_INF.PLAT_COST
  is '平台计算的成本';
comment on column HT_TBL_COST_CMP_INF.FILE_COST
  is '文件的成本';
comment on column HT_TBL_COST_CMP_INF.IS_EQUAL
  is '是否对平';
comment on column HT_TBL_COST_CMP_INF.COST_TYPE
  is '成本类型';
comment on column HT_TBL_COST_CMP_INF.REC_CRT_TS
  is '最新创建时间';






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
  is '内部机构号';
comment on column HT_TBL_DAT_FLOW.SETT_DATE
  is '清算日期';
comment on column HT_TBL_DAT_FLOW.TRANS_DATE
  is '交易日期';
comment on column HT_TBL_DAT_FLOW.TRANS_TIME
  is '交易时间';
comment on column HT_TBL_DAT_FLOW.FLAG_RESULT
  is '勾对标志';
comment on column HT_TBL_DAT_FLOW.STLM_FLAG
  is '清算标示';
comment on column HT_TBL_DAT_FLOW.ORDER_NO
  is '订单号';
comment on column HT_TBL_DAT_FLOW.PLAT_KEY
  is '平台统一流水号';
comment on column HT_TBL_DAT_FLOW.BATCH_NO
  is '平台批次号';
comment on column HT_TBL_DAT_FLOW.BUS_TP
  is '业务类型';
comment on column HT_TBL_DAT_FLOW.TRANS_TP
  is '交易类型';
comment on column HT_TBL_DAT_FLOW.ACCT_TYPE
  is '账户类型';
comment on column HT_TBL_DAT_FLOW.TRANS_STATE
  is '交易状态';
comment on column HT_TBL_DAT_FLOW.REVSAL_FLAG
  is '冲正标志';
comment on column HT_TBL_DAT_FLOW.KEY_REVSAL
  is '冲正与原始请求的匹配域';
comment on column HT_TBL_DAT_FLOW.CANCEL_FLAG
  is '撤销标志';
comment on column HT_TBL_DAT_FLOW.KEY_CANCEL
  is '撤销与原始请求的匹配域';
comment on column HT_TBL_DAT_FLOW.PAN
  is '账户账号';
comment on column HT_TBL_DAT_FLOW.ACCT_NAME
  is '账户名称';
comment on column HT_TBL_DAT_FLOW.PAN1
  is '星民易付账号';
comment on column HT_TBL_DAT_FLOW.CCY
  is '币种';
comment on column HT_TBL_DAT_FLOW.TRANS_AT
  is '交易金额';
comment on column HT_TBL_DAT_FLOW.TRANS_AT2
  is '交易金额2';
comment on column HT_TBL_DAT_FLOW.TRANS_AT3
  is '交易金额3';
comment on column HT_TBL_DAT_FLOW.TRANS_AT4
  is '交易金额4';
comment on column HT_TBL_DAT_FLOW.TRANS_AT5
  is '交易金额5';
comment on column HT_TBL_DAT_FLOW.EXT_MCHT_NO
  is '对外商户编码';
comment on column HT_TBL_DAT_FLOW.SETT_TP1
  is '合作方类型1';
comment on column HT_TBL_DAT_FLOW.MCHT_NO1
  is '合作方编号1';
comment on column HT_TBL_DAT_FLOW.SETT_TP2
  is '合作方类型2';
comment on column HT_TBL_DAT_FLOW.MCHT_NO2
  is '合作方编号2';
comment on column HT_TBL_DAT_FLOW.SETT_TP3
  is '合作方类型3';
comment on column HT_TBL_DAT_FLOW.MCHT_NO3
  is '合作方编号3';
comment on column HT_TBL_DAT_FLOW.SETT_TP4
  is '合作方类型4';
comment on column HT_TBL_DAT_FLOW.MCHT_NO4
  is '合作方编号4';
comment on column HT_TBL_DAT_FLOW.SETT_TP5
  is '合作方类型5';
comment on column HT_TBL_DAT_FLOW.MCHT_NO5
  is '合作方编号5';
comment on column HT_TBL_DAT_FLOW.SETT_TP6
  is '合作方类型6';
comment on column HT_TBL_DAT_FLOW.MCHT_NO6
  is '合作方编号6';
comment on column HT_TBL_DAT_FLOW.SETT_TP7
  is '合作方类型7';
comment on column HT_TBL_DAT_FLOW.MCHT_NO7
  is '合作方编号7';
comment on column HT_TBL_DAT_FLOW.SETT_TP8
  is '合作方类型8';
comment on column HT_TBL_DAT_FLOW.MCHT_NO8
  is '合作方编号8';
comment on column HT_TBL_DAT_FLOW.SETT_TP9
  is '合作方类型9';
comment on column HT_TBL_DAT_FLOW.MCHT_NO9
  is '合作方编号9';
comment on column HT_TBL_DAT_FLOW.SEQ_NUM
  is '序列号';






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
  is '算法规则ID';
comment on column HT_TBL_DISC_ALGO.DISC_STEP
  is '算法步骤';
comment on column HT_TBL_DISC_ALGO.OPER_RSLT
  is '结果值';
comment on column HT_TBL_DISC_ALGO.OPERAND1
  is '操作数1';
comment on column HT_TBL_DISC_ALGO.OPERATOR1
  is '操作符1';
comment on column HT_TBL_DISC_ALGO.OPERAND2
  is '操作数2';
comment on column HT_TBL_DISC_ALGO.OPERATOR2
  is '操作符2';
comment on column HT_TBL_DISC_ALGO.OPERAND3
  is '操作数3';
comment on column HT_TBL_DISC_ALGO.REC_OPR_ID
  is '操作标识';
comment on column HT_TBL_DISC_ALGO.REC_UPD_OPR
  is '修改柜员号';
comment on column HT_TBL_DISC_ALGO.REC_CRT_TS
  is '记录创建时间';
comment on column HT_TBL_DISC_ALGO.REC_UPD_TS
  is '记录修改时间';






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
  is '内部机构号';
comment on column HT_TBL_ERR_FLOW.SETT_DATE
  is '清算日期';
comment on column HT_TBL_ERR_FLOW.TRANS_DATE
  is '交易日期';
comment on column HT_TBL_ERR_FLOW.TRANS_TIME
  is '交易时间';
comment on column HT_TBL_ERR_FLOW.FLAG_RESULT
  is '勾对标志';
comment on column HT_TBL_ERR_FLOW.ACCT_STATU
  is '入账状态';
comment on column HT_TBL_ERR_FLOW.ORDER_NO
  is '订单号';
comment on column HT_TBL_ERR_FLOW.PLAT_KEY
  is '平台统一流水号';
comment on column HT_TBL_ERR_FLOW.BATCH_NO
  is '平台批次号';
comment on column HT_TBL_ERR_FLOW.BUS_TP
  is '业务类型';
comment on column HT_TBL_ERR_FLOW.TRANS_TP
  is '交易类型';
comment on column HT_TBL_ERR_FLOW.ACCT_TYPE
  is '账户类型';
comment on column HT_TBL_ERR_FLOW.TRANS_STATE
  is '交易状态';
comment on column HT_TBL_ERR_FLOW.PAN
  is '账户账号';
comment on column HT_TBL_ERR_FLOW.PAN1
  is '星民易付账号';
comment on column HT_TBL_ERR_FLOW.CCY
  is '币种';
comment on column HT_TBL_ERR_FLOW.TRANS_AT
  is '交易金额';
comment on column HT_TBL_ERR_FLOW.DEST_PAN
  is '账户账号';
comment on column HT_TBL_ERR_FLOW.DEST_CCY
  is '币种';
comment on column HT_TBL_ERR_FLOW.DEST_TRANS_AT
  is '交易金额';
comment on column HT_TBL_ERR_FLOW.DEBT_AT
  is '入账金额';
comment on column HT_TBL_ERR_FLOW.TRANS_FEE
  is '手续费';
comment on column HT_TBL_ERR_FLOW.RESERVE
  is '备注';






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
  is '作用类别';
comment on column HT_TBL_FLD_DSP.I_FLD_ID
  is '域索引号';
comment on column HT_TBL_FLD_DSP.N_FLD_L
  is '域长度';
comment on column HT_TBL_FLD_DSP.N_FLD_TYPE
  is '域类型';
comment on column HT_TBL_FLD_DSP.SP_FLD_DSP
  is '域作用描述';






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
  is '作用范围';
comment on column HT_TBL_FLD_TSF.I_BUF_CHG_INDEX
  is '转换索引ID';
comment on column HT_TBL_FLD_TSF.I_SOUR_FLD_INDEX
  is '源BUF的索引域';
comment on column HT_TBL_FLD_TSF.I_DEST_FLD_INDEX
  is '目的BUF的索引域';






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
  is '平台结算行';
comment on column HT_TBL_FUNDS_SHORT.ACCT_NO
  is '平台结算账户账号';
comment on column HT_TBL_FUNDS_SHORT.ACCT_NAME
  is '平台结算账户名称';
comment on column HT_TBL_FUNDS_SHORT.SHRT_AMT
  is '缺口金额';
comment on column HT_TBL_FUNDS_SHORT.SETT_DATE
  is '结算日期';






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
  is '内部机构号';
comment on column HT_TBL_GATEWAY_TXN.SETT_DATE
  is '清算日期';
comment on column HT_TBL_GATEWAY_TXN.TRANS_DATE
  is '交易日期';
comment on column HT_TBL_GATEWAY_TXN.TRANS_TIME
  is '交易时间';
comment on column HT_TBL_GATEWAY_TXN.FLAG_RESULT
  is '勾对标志';
comment on column HT_TBL_GATEWAY_TXN.ORDER_NO
  is '订单号';
comment on column HT_TBL_GATEWAY_TXN.PLAT_KEY
  is '平台统一流水号';
comment on column HT_TBL_GATEWAY_TXN.HOST_KEY
  is '额外唯一键';
comment on column HT_TBL_GATEWAY_TXN.BATCH_NO
  is '平台批次号';
comment on column HT_TBL_GATEWAY_TXN.BUS_TP
  is '业务类型';
comment on column HT_TBL_GATEWAY_TXN.TRANS_TP
  is '交易类型';
comment on column HT_TBL_GATEWAY_TXN.ACCT_TYPE
  is '账户类型';
comment on column HT_TBL_GATEWAY_TXN.TRANS_STATE
  is '交易状态';
comment on column HT_TBL_GATEWAY_TXN.REVSAL_FLAG
  is '冲正标志';
comment on column HT_TBL_GATEWAY_TXN.KEY_REVSAL
  is '冲正与原始请求的匹配域';
comment on column HT_TBL_GATEWAY_TXN.CANCEL_FLAG
  is '撤销标志';
comment on column HT_TBL_GATEWAY_TXN.KEY_CANCEL
  is '撤销与原始请求的匹配域';
comment on column HT_TBL_GATEWAY_TXN.PAN
  is '账户账号';
comment on column HT_TBL_GATEWAY_TXN.ACCT_NAME
  is '账户名称';
comment on column HT_TBL_GATEWAY_TXN.PAN1
  is '星民易付账号';
comment on column HT_TBL_GATEWAY_TXN.CCY
  is '币种';
comment on column HT_TBL_GATEWAY_TXN.TRANS_AT
  is '交易金额';
comment on column HT_TBL_GATEWAY_TXN.TRANS_AT2
  is '交易金额2';
comment on column HT_TBL_GATEWAY_TXN.TRANS_AT3
  is '交易金额3';
comment on column HT_TBL_GATEWAY_TXN.TRANS_AT4
  is '交易金额4';
comment on column HT_TBL_GATEWAY_TXN.TRANS_AT5
  is '交易金额5';
comment on column HT_TBL_GATEWAY_TXN.EXT_MCHT_NO
  is '对外商户编码';
comment on column HT_TBL_GATEWAY_TXN.SETT_TP1
  is '合作方类型1';
comment on column HT_TBL_GATEWAY_TXN.MCHT_NO1
  is '合作方编号1';
comment on column HT_TBL_GATEWAY_TXN.SETT_TP2
  is '合作方类型2';
comment on column HT_TBL_GATEWAY_TXN.MCHT_NO2
  is '合作方编号2';
comment on column HT_TBL_GATEWAY_TXN.SETT_TP3
  is '合作方类型3';
comment on column HT_TBL_GATEWAY_TXN.MCHT_NO3
  is '合作方编号3';
comment on column HT_TBL_GATEWAY_TXN.SETT_TP4
  is '合作方类型4';
comment on column HT_TBL_GATEWAY_TXN.MCHT_NO4
  is '合作方编号4';
comment on column HT_TBL_GATEWAY_TXN.SETT_TP5
  is '合作方类型5';
comment on column HT_TBL_GATEWAY_TXN.MCHT_NO5
  is '合作方编号5';
comment on column HT_TBL_GATEWAY_TXN.SETT_TP6
  is '合作方类型6';
comment on column HT_TBL_GATEWAY_TXN.MCHT_NO6
  is '合作方编号6';
comment on column HT_TBL_GATEWAY_TXN.SETT_TP7
  is '合作方类型7';
comment on column HT_TBL_GATEWAY_TXN.MCHT_NO7
  is '合作方编号7';
comment on column HT_TBL_GATEWAY_TXN.SETT_TP8
  is '合作方类型8';
comment on column HT_TBL_GATEWAY_TXN.MCHT_NO8
  is '合作方编号8';
comment on column HT_TBL_GATEWAY_TXN.SETT_TP9
  is '合作方类型9';
comment on column HT_TBL_GATEWAY_TXN.MCHT_NO9
  is '合作方编号9';
comment on column HT_TBL_GATEWAY_TXN.SEQ_NUM
  is '序列号';





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
  is '内部机构号';
comment on column HT_TBL_HOST_TXN.SETT_DATE
  is '清算日期';
comment on column HT_TBL_HOST_TXN.TRANS_DATE
  is '交易日期';
comment on column HT_TBL_HOST_TXN.TRANS_TIME
  is '交易时间';
comment on column HT_TBL_HOST_TXN.FLAG_RESULT
  is '勾对标志';
comment on column HT_TBL_HOST_TXN.EXT_MCHT_NO
  is '对外商户编码';
comment on column HT_TBL_HOST_TXN.ORDER_NO
  is '订单号';
comment on column HT_TBL_HOST_TXN.PLAT_KEY
  is '平台统一流水号';
comment on column HT_TBL_HOST_TXN.BATCH_NO
  is '平台批次号';
comment on column HT_TBL_HOST_TXN.BUS_TP
  is '业务类型';
comment on column HT_TBL_HOST_TXN.TRANS_TP
  is '交易类型';
comment on column HT_TBL_HOST_TXN.TRANS_STATE
  is '交易状态';
comment on column HT_TBL_HOST_TXN.REVSAL_FLAG
  is '冲正标志';
comment on column HT_TBL_HOST_TXN.KEY_REVSAL
  is '冲正与原始请求的匹配域';
comment on column HT_TBL_HOST_TXN.CANCEL_FLAG
  is '撤销标志';
comment on column HT_TBL_HOST_TXN.KEY_CANCEL
  is '撤销与原始请求的匹配域';
comment on column HT_TBL_HOST_TXN.ACCT_TYPE
  is '账户类型';
comment on column HT_TBL_HOST_TXN.PAN
  is '账户账号';
comment on column HT_TBL_HOST_TXN.CCY
  is '币种';
comment on column HT_TBL_HOST_TXN.TRANS_AT
  is '交易金额';
comment on column HT_TBL_HOST_TXN.TRANS_AT2
  is '交易金额2';
comment on column HT_TBL_HOST_TXN.TRANS_AT3
  is '交易金额3';
comment on column HT_TBL_HOST_TXN.TRANS_AT4
  is '交易金额4';
comment on column HT_TBL_HOST_TXN.TRANS_AT5
  is '交易金额5';
comment on column HT_TBL_HOST_TXN.SEQ_NUM
  is '序列号';






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
  is '作用范围';
comment on column HT_TBL_IPC_DFT.IPC_DFT_INDEX
  is 'IPC块的索引';
comment on column HT_TBL_IPC_DFT.POS_INDEX
  is '顺序号';
comment on column HT_TBL_IPC_DFT.FLD_INDEX
  is '域索引';
comment on column HT_TBL_IPC_DFT.FLD_LEN
  is '域长度';
comment on column HT_TBL_IPC_DFT.FLD_VAL
  is '域默认值';




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
  is '作用类别';
comment on column HT_TBL_IPC_DFT_DSP.COMP_KEY
  is '比较特征值';
comment on column HT_TBL_IPC_DFT_DSP.COMP_KEY_LEN
  is '特征值长度';
comment on column HT_TBL_IPC_DFT_DSP.IPC_DFT_INDEX
  is '默认IPC索引';
comment on column HT_TBL_IPC_DFT_DSP.BUF_DSP_INDEX
  is 'BUFFER转换索引';
comment on column HT_TBL_IPC_DFT_DSP.TXN_NUM
  is '内部交易码';
comment on column HT_TBL_IPC_DFT_DSP.IPC_DFT
  is '描述';





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
  is '任务类别';
comment on column HT_TBL_MISSION_CFG.MISSION_INDEX
  is '任务索引号';
comment on column HT_TBL_MISSION_CFG.MISSION_NAME
  is '任务名';
comment on column HT_TBL_MISSION_CFG.MISSION_LEVEL
  is '任务级别';
comment on column HT_TBL_MISSION_CFG.DATA_NUM
  is '每个子任务处理的记录数';
comment on column HT_TBL_MISSION_CFG.COMMIT_FLAG
  is 'COMMIT标志';
comment on column HT_TBL_MISSION_CFG.COMMIT_NUM
  is '每次COMMIT的记录数';





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
  is '内部机构号';
comment on column HT_TBL_MISSION_INF.SETTLMT_DATE
  is '清算日期';
comment on column HT_TBL_MISSION_INF.MISSION_INDEX
  is '任务索引号';
comment on column HT_TBL_MISSION_INF.MISSION_NAME
  is '任务名';
comment on column HT_TBL_MISSION_INF.MISSION_LEVEL
  is '任务级别';
comment on column HT_TBL_MISSION_INF.MISSION_STATUS
  is '任务状态';
comment on column HT_TBL_MISSION_INF.DATA_NUM
  is '每个子任务处理的记录数';
comment on column HT_TBL_MISSION_INF.CHILD_NUM
  is '子任务数';
comment on column HT_TBL_MISSION_INF.COMMIT_FLAG
  is 'COMMIT标志';
comment on column HT_TBL_MISSION_INF.COMMIT_NUM
  is '每次COMMIT的记录数';
comment on column HT_TBL_MISSION_INF.START_TIME
  is '任务开始时间';
comment on column HT_TBL_MISSION_INF.END_TIME
  is '任务结束时间';






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
  is '内部机构号';
comment on column HT_TBL_MISSION_INFO.SETT_DATE
  is '结算日期';
comment on column HT_TBL_MISSION_INFO.TRANS_DATE
  is '交易日期';
comment on column HT_TBL_MISSION_INFO.SEQ_NO
  is '归集流水';
comment on column HT_TBL_MISSION_INFO.BANK_CODE
  is '备付金开户行';
comment on column HT_TBL_MISSION_INFO.ACCT_TYPE
  is '结算账户类型';
comment on column HT_TBL_MISSION_INFO.ACCT_NO
  is '结算账户账号';
comment on column HT_TBL_MISSION_INFO.ACCT_NAME
  is '平台结算账户名称';
comment on column HT_TBL_MISSION_INFO.DC_FLAG
  is '支出/收入';
comment on column HT_TBL_MISSION_INFO.BANK_CODE1
  is '备付金开户行';
comment on column HT_TBL_MISSION_INFO.ACCT_TYPE1
  is '对手结算账户类型';
comment on column HT_TBL_MISSION_INFO.ACCT_NO1
  is '对手结算账户账号';
comment on column HT_TBL_MISSION_INFO.ACCT_NAME1
  is '对手结算账户名称';
comment on column HT_TBL_MISSION_INFO.AMT
  is '划账金额';
comment on column HT_TBL_MISSION_INFO.STATUS
  is '结算状态';
comment on column HT_TBL_MISSION_INFO.FILE_NAME
  is '划账文件名';
comment on column HT_TBL_MISSION_INFO.CHNL_ID
  is '划账渠道';
comment on column HT_TBL_MISSION_INFO.PS
  is '附言(postscript)';






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
  is '内部机构号';
comment on column HT_TBL_MISSION_TIME.MISSION_INDEX
  is '任务索引号';
comment on column HT_TBL_MISSION_TIME.MISSION_NAME
  is '任务名';
comment on column HT_TBL_MISSION_TIME.MISSION_DATE
  is '任务日期';
comment on column HT_TBL_MISSION_TIME.MISSION_WEEKDAY
  is '任务星期';
comment on column HT_TBL_MISSION_TIME.MISSION_HOUR
  is '任务时间';
comment on column HT_TBL_MISSION_TIME.DATA_NUM
  is '每个子任务处理的记录数';
comment on column HT_TBL_MISSION_TIME.CHILD_NUM
  is '子任务数';
comment on column HT_TBL_MISSION_TIME.COMMIT_FLAG
  is 'COMMIT标志';
comment on column HT_TBL_MISSION_TIME.COMMIT_NUM
  is '每次COMMIT的记录数';






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
  is '标志位';
comment on column HT_TBL_OPR_CTL.OBJECT_NAME
  is '序列名';
comment on column HT_TBL_OPR_CTL.PARAM_1
  is '保留1 ';
comment on column HT_TBL_OPR_CTL.PARAM_2
  is '保留2';
comment on column HT_TBL_OPR_CTL.PARAM_3
  is '保留3';
comment on column HT_TBL_OPR_CTL.PARAM_4
  is '保留4';




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
  is '清理表名';
comment on column HT_TBL_PART_INF.PART_NAME
  is '目标数据表名';
comment on column HT_TBL_PART_INF.TBL_SPC_NAME
  is '清理条件';
comment on column HT_TBL_PART_INF.TBL_INTERVEL_DAY
  is '保留时间（天）';
comment on column HT_TBL_PART_INF.TBL_OPR_FLAG
  is '操作类型';





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
  is '结算银行';
comment on column HT_TBL_POSTION_TRIAL.BANK_NAME
  is '结算银行名称';
comment on column HT_TBL_POSTION_TRIAL.ACCT_NO
  is '结算账户账号';
comment on column HT_TBL_POSTION_TRIAL.ACCT_NAME
  is '结算账户名称';
comment on column HT_TBL_POSTION_TRIAL.INTEREST
  is '利率';
comment on column HT_TBL_POSTION_TRIAL.SETT_DATE
  is '结算日期 ';
comment on column HT_TBL_POSTION_TRIAL.BAL
  is '账户余额';
comment on column HT_TBL_POSTION_TRIAL.RESV_BAL
  is '保底余额';
comment on column HT_TBL_POSTION_TRIAL.BAL_TRIAL
  is '试算余额';
comment on column HT_TBL_POSTION_TRIAL.SEQ_NO
  is '结算流水';
comment on column HT_TBL_POSTION_TRIAL.SYNC_TIME
  is '更新时间';





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
  is '内部机构号';
comment on column HT_TBL_RISK_TXN.SETT_DATE
  is '清算日期';
comment on column HT_TBL_RISK_TXN.TRANS_DATE
  is '网关交易日期';
comment on column HT_TBL_RISK_TXN.TRANS_TIME
  is '网关交易时间';
comment on column HT_TBL_RISK_TXN.GATEWAY_KEY
  is '网关流水号';
comment on column HT_TBL_RISK_TXN.ORDER_NO
  is '订单号';
comment on column HT_TBL_RISK_TXN.PAYWAY_TYPE
  is '支付通道';
comment on column HT_TBL_RISK_TXN.BUS_TP
  is '业务类型';
comment on column HT_TBL_RISK_TXN.TRANS_TP
  is '交易类型';
comment on column HT_TBL_RISK_TXN.USER_ID
  is '客户ID';
comment on column HT_TBL_RISK_TXN.PAN
  is '卡号';
comment on column HT_TBL_RISK_TXN.TRANS_AT
  is '交易金额';
comment on column HT_TBL_RISK_TXN.SETT_MCHT_NO
  is '结算商户编码';
comment on column HT_TBL_RISK_TXN.EXT_MCHT_NO
  is '对外商户编码';
comment on column HT_TBL_RISK_TXN.RISK_REASON
  is '风控触发原因';
comment on column HT_TBL_RISK_TXN.REMARK
  is '备用';





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
  is '渠道编号';
comment on column HT_TBL_SETT_CHAN.CHNL_NAME
  is '渠道名称';
comment on column HT_TBL_SETT_CHAN.STATUS
  is '渠道状态';
comment on column HT_TBL_SETT_CHAN.MIN_AMT
  is '金额条件';
comment on column HT_TBL_SETT_CHAN.MAX_AMT
  is '渠道限额';
comment on column HT_TBL_SETT_CHAN.BAL
  is '可用余额';
comment on column HT_TBL_SETT_CHAN.INDEX_DAY
  is '工作日规则';
comment on column HT_TBL_SETT_CHAN.INDEX_TIME
  is '时间规则';
comment on column HT_TBL_SETT_CHAN.PERIOD
  is '支付周期';
comment on column HT_TBL_SETT_CHAN.INDEX_FEE
  is '费率编码';
comment on column HT_TBL_SETT_CHAN.SHADOM
  is '影子账户';





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
  is '内部机构号';
comment on column HT_TBL_SETT_FLOW.SETT_DATE
  is '清算日期';
comment on column HT_TBL_SETT_FLOW.TRANS_DATE
  is '交易日期';
comment on column HT_TBL_SETT_FLOW.TRANS_TIME
  is '交易时间';
comment on column HT_TBL_SETT_FLOW.FLAG_RESULT
  is '勾对标志';
comment on column HT_TBL_SETT_FLOW.ACCT_STATU
  is '入账状态';
comment on column HT_TBL_SETT_FLOW.STATUS
  is '结算状态';
comment on column HT_TBL_SETT_FLOW.ORDER_NO
  is '订单号';
comment on column HT_TBL_SETT_FLOW.PLAT_KEY
  is '平台统一流水号';
comment on column HT_TBL_SETT_FLOW.BATCH_NO
  is '平台批次号';
comment on column HT_TBL_SETT_FLOW.BUS_TP
  is '业务类型';
comment on column HT_TBL_SETT_FLOW.TRANS_TP
  is '交易类型';
comment on column HT_TBL_SETT_FLOW.ACCT_TYPE
  is '账户类型';
comment on column HT_TBL_SETT_FLOW.TRANS_STATE
  is '交易状态';
comment on column HT_TBL_SETT_FLOW.REVSAL_FLAG
  is '冲正标志';
comment on column HT_TBL_SETT_FLOW.KEY_REVSAL
  is '冲正与原始请求的匹配域';
comment on column HT_TBL_SETT_FLOW.CANCEL_FLAG
  is '撤销标志';
comment on column HT_TBL_SETT_FLOW.KEY_CANCEL
  is '撤销与原始请求的匹配域';
comment on column HT_TBL_SETT_FLOW.PAN
  is '账户账号';
comment on column HT_TBL_SETT_FLOW.ACCT_NAME
  is '账户名称';
comment on column HT_TBL_SETT_FLOW.PAN1
  is '星民易付账号';
comment on column HT_TBL_SETT_FLOW.BANK_ORDER_NO
  is '代付订单号';
comment on column HT_TBL_SETT_FLOW.AMT_FEE
  is '兴业银行代付手续费';
comment on column HT_TBL_SETT_FLOW.CCY
  is '币种';
comment on column HT_TBL_SETT_FLOW.TRANS_AT
  is '交易金额';
comment on column HT_TBL_SETT_FLOW.TRANS_AT2
  is '交易金额2';
comment on column HT_TBL_SETT_FLOW.TRANS_AT3
  is '交易金额3';
comment on column HT_TBL_SETT_FLOW.TRANS_AT4
  is '交易金额4';
comment on column HT_TBL_SETT_FLOW.TRANS_AT5
  is '交易金额5';
comment on column HT_TBL_SETT_FLOW.EXT_MCHT_NO
  is '对外商户编码';
comment on column HT_TBL_SETT_FLOW.SETT_TP1
  is '合作方类型1';
comment on column HT_TBL_SETT_FLOW.MCHT_NO1
  is '合作方编号1';
comment on column HT_TBL_SETT_FLOW.UP_MCHT_NO1
  is '上级商户编号';
comment on column HT_TBL_SETT_FLOW.MCHT_ROLE1
  is '账务角色1';
comment on column HT_TBL_SETT_FLOW.DEBT_AT_MCHT_NO1
  is '合作方1借记本金';
comment on column HT_TBL_SETT_FLOW.CRET_AT_MCHT_NO1
  is '合作方1贷记本金';
comment on column HT_TBL_SETT_FLOW.DEBT_FEE_MCHT_NO1
  is '合作方1借记手续费';
comment on column HT_TBL_SETT_FLOW.CRET_FEE_MCHT_NO1
  is '合作方1贷记手续费';
comment on column HT_TBL_SETT_FLOW.SHARE_PROFIT_MCHT_NO1
  is '合作方1分润';
comment on column HT_TBL_SETT_FLOW.MCHT_RATE1
  is '合作方1手续费率';
comment on column HT_TBL_SETT_FLOW.SETT_TP2
  is '合作方类型2';
comment on column HT_TBL_SETT_FLOW.MCHT_NO2
  is '合作方编号2';
comment on column HT_TBL_SETT_FLOW.UP_MCHT_NO2
  is '上级商户编号';
comment on column HT_TBL_SETT_FLOW.MCHT_ROLE2
  is '账务角色2';
comment on column HT_TBL_SETT_FLOW.DEBT_AT_MCHT_NO2
  is '合作方2借记本金';
comment on column HT_TBL_SETT_FLOW.CRET_AT_MCHT_NO2
  is '合作方2贷记本金';
comment on column HT_TBL_SETT_FLOW.DEBT_FEE_MCHT_NO2
  is '合作方2借记手续费';
comment on column HT_TBL_SETT_FLOW.CRET_FEE_MCHT_NO2
  is '合作方2贷记手续费';
comment on column HT_TBL_SETT_FLOW.SHARE_PROFIT_MCHT_NO2
  is '合作方2分润';
comment on column HT_TBL_SETT_FLOW.MCHT_RATE2
  is '合作方2手续费率';
comment on column HT_TBL_SETT_FLOW.SETT_TP3
  is '合作方类型3';
comment on column HT_TBL_SETT_FLOW.MCHT_NO3
  is '合作方编号3';
comment on column HT_TBL_SETT_FLOW.UP_MCHT_NO3
  is '上级商户编号';
comment on column HT_TBL_SETT_FLOW.MCHT_ROLE3
  is '账务角色3';
comment on column HT_TBL_SETT_FLOW.DEBT_AT_MCHT_NO3
  is '合作方3借记本金';
comment on column HT_TBL_SETT_FLOW.CRET_AT_MCHT_NO3
  is '合作方3贷记本金';
comment on column HT_TBL_SETT_FLOW.DEBT_FEE_MCHT_NO3
  is '合作方3借记手续费';
comment on column HT_TBL_SETT_FLOW.CRET_FEE_MCHT_NO3
  is '合作方3贷记手续费';
comment on column HT_TBL_SETT_FLOW.SHARE_PROFIT_MCHT_NO3
  is '合作方3分润';
comment on column HT_TBL_SETT_FLOW.MCHT_RATE3
  is '合作方3手续费率';
comment on column HT_TBL_SETT_FLOW.SETT_TP4
  is '合作方类型4';
comment on column HT_TBL_SETT_FLOW.MCHT_NO4
  is '合作方编号4';
comment on column HT_TBL_SETT_FLOW.UP_MCHT_NO4
  is '上级商户编号';
comment on column HT_TBL_SETT_FLOW.MCHT_ROLE4
  is '账务角色4';
comment on column HT_TBL_SETT_FLOW.DEBT_AT_MCHT_NO4
  is '合作方4借记本金';
comment on column HT_TBL_SETT_FLOW.CRET_AT_MCHT_NO4
  is '合作方4贷记本金';
comment on column HT_TBL_SETT_FLOW.DEBT_FEE_MCHT_NO4
  is '合作方4借记手续费';
comment on column HT_TBL_SETT_FLOW.CRET_FEE_MCHT_NO4
  is '合作方4贷记手续费';
comment on column HT_TBL_SETT_FLOW.SHARE_PROFIT_MCHT_NO4
  is '合作方4分润';
comment on column HT_TBL_SETT_FLOW.MCHT_RATE4
  is '合作方4手续费率';
comment on column HT_TBL_SETT_FLOW.SETT_TP5
  is '合作方类型5';
comment on column HT_TBL_SETT_FLOW.MCHT_NO5
  is '合作方编号5';
comment on column HT_TBL_SETT_FLOW.UP_MCHT_NO5
  is '上级商户编号';
comment on column HT_TBL_SETT_FLOW.MCHT_ROLE5
  is '账务角色5';
comment on column HT_TBL_SETT_FLOW.DEBT_AT_MCHT_NO5
  is '合作方5借记本金';
comment on column HT_TBL_SETT_FLOW.CRET_AT_MCHT_NO5
  is '合作方5贷记本金';
comment on column HT_TBL_SETT_FLOW.DEBT_FEE_MCHT_NO5
  is '合作方5借记手续费';
comment on column HT_TBL_SETT_FLOW.CRET_FEE_MCHT_NO5
  is '合作方5贷记手续费';
comment on column HT_TBL_SETT_FLOW.SHARE_PROFIT_MCHT_NO5
  is '合作方5分润';
comment on column HT_TBL_SETT_FLOW.MCHT_RATE5
  is '合作方5手续费率';
comment on column HT_TBL_SETT_FLOW.SETT_TP6
  is '合作方类型6';
comment on column HT_TBL_SETT_FLOW.MCHT_NO6
  is '合作方编号6';
comment on column HT_TBL_SETT_FLOW.UP_MCHT_NO6
  is '上级商户编号';
comment on column HT_TBL_SETT_FLOW.MCHT_ROLE6
  is '账务角色6';
comment on column HT_TBL_SETT_FLOW.DEBT_AT_MCHT_NO6
  is '合作方6借记本金';
comment on column HT_TBL_SETT_FLOW.CRET_AT_MCHT_NO6
  is '合作方6贷记本金';
comment on column HT_TBL_SETT_FLOW.DEBT_FEE_MCHT_NO6
  is '合作方6借记手续费';
comment on column HT_TBL_SETT_FLOW.CRET_FEE_MCHT_NO6
  is '合作方6贷记手续费';
comment on column HT_TBL_SETT_FLOW.SHARE_PROFIT_MCHT_NO6
  is '合作方6分润';
comment on column HT_TBL_SETT_FLOW.MCHT_RATE6
  is '合作方6手续费率';
comment on column HT_TBL_SETT_FLOW.SETT_TP7
  is '合作方类型7';
comment on column HT_TBL_SETT_FLOW.MCHT_NO7
  is '合作方编号7';
comment on column HT_TBL_SETT_FLOW.UP_MCHT_NO7
  is '上级商户编号';
comment on column HT_TBL_SETT_FLOW.MCHT_ROLE7
  is '账务角色7';
comment on column HT_TBL_SETT_FLOW.DEBT_AT_MCHT_NO7
  is '合作方7借记本金';
comment on column HT_TBL_SETT_FLOW.CRET_AT_MCHT_NO7
  is '合作方7贷记本金';
comment on column HT_TBL_SETT_FLOW.DEBT_FEE_MCHT_NO7
  is '合作方7借记手续费';
comment on column HT_TBL_SETT_FLOW.CRET_FEE_MCHT_NO7
  is '合作方7贷记手续费';
comment on column HT_TBL_SETT_FLOW.SHARE_PROFIT_MCHT_NO7
  is '合作方7分润';
comment on column HT_TBL_SETT_FLOW.MCHT_RATE7
  is '合作方7手续费率';
comment on column HT_TBL_SETT_FLOW.SETT_TP8
  is '合作方类型8';
comment on column HT_TBL_SETT_FLOW.MCHT_NO8
  is '合作方编号8';
comment on column HT_TBL_SETT_FLOW.UP_MCHT_NO8
  is '上级商户编号';
comment on column HT_TBL_SETT_FLOW.MCHT_ROLE8
  is '账务角色8';
comment on column HT_TBL_SETT_FLOW.DEBT_AT_MCHT_NO8
  is '合作方8借记本金';
comment on column HT_TBL_SETT_FLOW.CRET_AT_MCHT_NO8
  is '合作方8贷记本金';
comment on column HT_TBL_SETT_FLOW.DEBT_FEE_MCHT_NO8
  is '合作方8借记手续费';
comment on column HT_TBL_SETT_FLOW.CRET_FEE_MCHT_NO8
  is '合作方8贷记手续费';
comment on column HT_TBL_SETT_FLOW.SHARE_PROFIT_MCHT_NO8
  is '合作方8分润';
comment on column HT_TBL_SETT_FLOW.MCHT_RATE8
  is '合作方8手续费率';
comment on column HT_TBL_SETT_FLOW.SETT_TP9
  is '合作方类型9';
comment on column HT_TBL_SETT_FLOW.MCHT_NO9
  is '合作方编号9';
comment on column HT_TBL_SETT_FLOW.UP_MCHT_NO9
  is '上级商户编号';
comment on column HT_TBL_SETT_FLOW.MCHT_ROLE9
  is '账务角色9';
comment on column HT_TBL_SETT_FLOW.DEBT_AT_MCHT_NO9
  is '合作方9借记本金';
comment on column HT_TBL_SETT_FLOW.CRET_AT_MCHT_NO9
  is '合作方9贷记本金';
comment on column HT_TBL_SETT_FLOW.DEBT_FEE_MCHT_NO9
  is '合作方9借记手续费';
comment on column HT_TBL_SETT_FLOW.CRET_FEE_MCHT_NO9
  is '合作方9贷记手续费';
comment on column HT_TBL_SETT_FLOW.SHARE_PROFIT_MCHT_NO9
  is '合作方9分润';
comment on column HT_TBL_SETT_FLOW.MCHT_RATE9
  is '合作方9手续费率';
comment on column HT_TBL_SETT_FLOW.DEBT_AT_PLAT
  is '平台借记本金';
comment on column HT_TBL_SETT_FLOW.CRET_AT_PLAT
  is '平台贷记本金';
comment on column HT_TBL_SETT_FLOW.DEBT_FEE_PLAT
  is '平台借记手续费';
comment on column HT_TBL_SETT_FLOW.CRET_FEE_PLAT
  is '平台贷记手续费';
comment on column HT_TBL_SETT_FLOW.SHARE_PROFIT_PLAT
  is '平台分润';
comment on column HT_TBL_SETT_FLOW.REMARK
  is '备注';
comment on column HT_TBL_SETT_FLOW.SEQ_NUM
  is '序列号';





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
  is '内部机构号';
comment on column HT_TBL_SETT_LIST.SETT_DATE
  is '结算日期';
comment on column HT_TBL_SETT_LIST.TRANS_DATE
  is '交易日期';
comment on column HT_TBL_SETT_LIST.MCHT_NO
  is '商户号';
comment on column HT_TBL_SETT_LIST.MCHT_ROLE
  is '角色类型';
comment on column HT_TBL_SETT_LIST.MCHT_RSLT_NO
  is '商户入账流水号';
comment on column HT_TBL_SETT_LIST.SEQ_NO
  is '结算流水号';
comment on column HT_TBL_SETT_LIST.PLAT_SETT_BANK
  is '平台结算行';
comment on column HT_TBL_SETT_LIST.PLAT_SETT_BANK_NM
  is '平台结算行行名';
comment on column HT_TBL_SETT_LIST.PLAT_SETT_ACCT_TYPE
  is '结算账户类型';
comment on column HT_TBL_SETT_LIST.PLAT_SETT_ACCT
  is '结算账户账号';
comment on column HT_TBL_SETT_LIST.PLAT_SETT_ACCT_NM
  is '平台结算账户名称';
comment on column HT_TBL_SETT_LIST.DC_FLAG
  is '支出/收入';
comment on column HT_TBL_SETT_LIST.MCHT_SETT_XINGMIN_ACCT
  is '商户星民易付账户';
comment on column HT_TBL_SETT_LIST.MCHT_SETT_BANK
  is '商户结算行';
comment on column HT_TBL_SETT_LIST.MCHT_SETT_BANK_NM
  is '商户结算行行名';
comment on column HT_TBL_SETT_LIST.MCHT_SETT_ACCT_TYPE
  is '商户结算账户类型';
comment on column HT_TBL_SETT_LIST.MCHT_SETT_ACCT
  is '商户结算账户账号';
comment on column HT_TBL_SETT_LIST.MCHT_SETT_ACCT_NM
  is '商户结算账户名称';
comment on column HT_TBL_SETT_LIST.AMT
  is '划账金额';
comment on column HT_TBL_SETT_LIST.AMT_FEE
  is '划账金额手续';
comment on column HT_TBL_SETT_LIST.PLAT_AMT_FEE
  is '平台计算划付手续';
comment on column HT_TBL_SETT_LIST.ACCT_STATU
  is '入账状态';
comment on column HT_TBL_SETT_LIST.STATUS
  is '结算状态';
comment on column HT_TBL_SETT_LIST.CHL_ID
  is '划账渠道';
comment on column HT_TBL_SETT_LIST.CHNL_NAME
  is '渠道名称';
comment on column HT_TBL_SETT_LIST.FILE_NAME
  is '划账文件名';
comment on column HT_TBL_SETT_LIST.PS
  is '附言(postscript)';
comment on column HT_TBL_SETT_LIST.RESERVE1
  is '保留1';
comment on column HT_TBL_SETT_LIST.RESERVE2
  is '保留1';
comment on column HT_TBL_SETT_LIST.RESERVE3
  is '保留1';
comment on column HT_TBL_SETT_LIST.RESERVE4
  is '保留1';
comment on column HT_TBL_SETT_LIST.RESERVE5
  is '保留1';






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
  is '业务类型';
comment on column HT_TBL_TXN_ALGO_RULE.TXN_NUM
  is '内部交易码';
comment on column HT_TBL_TXN_ALGO_RULE.ALGO_ID
  is '规则ID';
comment on column HT_TBL_TXN_ALGO_RULE.PARAM_FLAG
  is '补充字段';
comment on column HT_TBL_TXN_ALGO_RULE.REC_UPD_OPR
  is '修改柜员号';
comment on column HT_TBL_TXN_ALGO_RULE.REC_CRT_TS
  is '记录创建时间';
comment on column HT_TBL_TXN_ALGO_RULE.REC_UPD_TS
  is '记录修改时间';





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
  is '内部机构号';
comment on column HT_TBL_WIFT_TXN.SETT_DATE
  is '清算日期';
comment on column HT_TBL_WIFT_TXN.TRANS_DATE
  is '交易日期';
comment on column HT_TBL_WIFT_TXN.TRANS_TIME
  is '交易时间';
comment on column HT_TBL_WIFT_TXN.FLAG_RESULT
  is '勾对标志';
comment on column HT_TBL_WIFT_TXN.EXT_MCHT_NO
  is '对外商户编码';
comment on column HT_TBL_WIFT_TXN.ORDER_NO
  is '商户订单号';
comment on column HT_TBL_WIFT_TXN.PLAT_KEY
  is '平台统一流水号';
comment on column HT_TBL_WIFT_TXN.BATCH_NO
  is '平台批次号';
comment on column HT_TBL_WIFT_TXN.BUS_TP
  is '业务类型';
comment on column HT_TBL_WIFT_TXN.TRANS_TP
  is '交易类型';
comment on column HT_TBL_WIFT_TXN.WIFT_TRANS_TP
  is '微富通交易类型';
comment on column HT_TBL_WIFT_TXN.TRANS_STATE
  is '交易状态';
comment on column HT_TBL_WIFT_TXN.ACCT_TYPE
  is '账户类型';
comment on column HT_TBL_WIFT_TXN.PAN
  is '账户账号';
comment on column HT_TBL_WIFT_TXN.CCY
  is '币种';
comment on column HT_TBL_WIFT_TXN.TRANS_AT
  is '交易金额';
comment on column HT_TBL_WIFT_TXN.TRANS_AT2
  is '交易金额2';
comment on column HT_TBL_WIFT_TXN.TRANS_AT3
  is '交易金额3';
comment on column HT_TBL_WIFT_TXN.TRANS_AT4
  is '交易金额4';
comment on column HT_TBL_WIFT_TXN.TRANS_AT5
  is '交易金额5';
comment on column HT_TBL_WIFT_TXN.SEQ_NUM
  is '序列号';


-----------------------------富友新增-------------------------------

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
  is '内部机构号';
comment on column TBL_ALIPAY_WECHAT_TXN.SETT_DATE
  is '清算日期';
comment on column TBL_ALIPAY_WECHAT_TXN.TRANS_DATE
  is '交易日期';
comment on column TBL_ALIPAY_WECHAT_TXN.TRANS_TIME
  is '交易时间';
comment on column TBL_ALIPAY_WECHAT_TXN.FLAG_RESULT
  is '勾对标志';
comment on column TBL_ALIPAY_WECHAT_TXN.EXT_MCHT_NO
  is '对外商户编码';
comment on column TBL_ALIPAY_WECHAT_TXN.ORDER_NO
  is '商户订单号';
comment on column TBL_ALIPAY_WECHAT_TXN.PLAT_KEY
  is '平台统一流水号';
comment on column TBL_ALIPAY_WECHAT_TXN.BATCH_NO
  is '平台批次号';
comment on column TBL_ALIPAY_WECHAT_TXN.BUS_TP
  is '业务类型';
comment on column TBL_ALIPAY_WECHAT_TXN.TRANS_TP
  is '交易类型';
comment on column TBL_ALIPAY_WECHAT_TXN.THR_TRANS_TP
  is '通道交易类型';
comment on column TBL_ALIPAY_WECHAT_TXN.TRANS_STATE
  is '交易状态';
comment on column TBL_ALIPAY_WECHAT_TXN.ACCT_TYPE
  is '账户类型';
comment on column TBL_ALIPAY_WECHAT_TXN.PAN
  is '账户账号';
comment on column TBL_ALIPAY_WECHAT_TXN.CCY
  is '币种';
comment on column TBL_ALIPAY_WECHAT_TXN.TRANS_AT
  is '交易金额';
comment on column TBL_ALIPAY_WECHAT_TXN.TRANS_AT2
  is '交易金额2';
comment on column TBL_ALIPAY_WECHAT_TXN.TRANS_AT3
  is '交易金额3';
comment on column TBL_ALIPAY_WECHAT_TXN.TRANS_AT4
  is '交易金额4';
comment on column TBL_ALIPAY_WECHAT_TXN.TRANS_AT5
  is '交易金额5';
comment on column TBL_ALIPAY_WECHAT_TXN.SEQ_NUM
  is '序列号';