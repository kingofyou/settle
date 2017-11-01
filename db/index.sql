alter table HT_DOWNLAND_RECORD
  add constraint PK_DOWNLAND_RECORD primary key (ID)
  using index 
  tablespace settle_idx_tbs
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

  alter table HT_OPER_RECORD
  add constraint PK_OPER_RECORD primary key (ID)
  using index 
  tablespace settle_idx_tbs
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

  alter table HT_TBL_ACCT_INF
  add constraint PK_HT_TBL_ACCT_INF primary key (TYPE)
  using index 
  tablespace settle_idx_tbs
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

alter table HT_TBL_ACCT_RSLT
  add constraint PK_HT_TBL_ACCT_RSLT primary key (INTER_BRH_CODE, SETT_DATE, SEQ_NUM)
  using index 
  tablespace settle_idx_tbs
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

  alter table HT_TBL_ACTIVE_MCHT_INF
  add constraint PK_HT_TBL_ACTIVE_MCHT_INF primary key (SETT_DATE, INTER_BRH_CODE, SEQ_NUM)
  using index 
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_ACTIVE_MCHT_INF on HT_TBL_ACTIVE_MCHT_INF (SETT_DATE)
  tablespace settle_idx_tbs
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

  alter table HT_TBL_ALGO_CTL
  add constraint PK_HT_TBL_ALGO_CTL primary key (BUS_TP, TXN_NUM)
  using index 
  tablespace settle_idx_tbs
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

  alter table HT_TBL_BRH_INFO
  add constraint PK_HT_TBL_BRH_INFO primary key (BRH_ID)
  using index 
  tablespace settle_idx_tbs
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

alter table HT_TBL_BRH_MAP
  add constraint PK_HT_TBL_BRH_MAP primary key (BUS_TP)
  using index 
  tablespace settle_idx_tbs
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

alter table HT_TBL_BUF_CHG
  add constraint PK_HT_TBL_BUF_CHG primary key (L_USAGE_KEY, I_BUF_CHG_INDEX)
  using index 
  tablespace settle_idx_tbs
  pctfree 10
  initrans 2
  maxtrans 255;

alter table HT_TBL_BUF_DSP
  add constraint PK_HT_TBL_BUF_DSP primary key (L_USAGE_KEY, I_BUF_DSP_INDEX, I_POS_INDEX)
  using index 
  tablespace settle_idx_tbs
  pctfree 10
  initrans 2
  maxtrans 255;

  alter table HT_TBL_CHECK_FLOW
  add constraint PK_HT_TBL_CHECK_FLOW primary key (INTER_BRH_CODE, PLAT_KEY)
  using index 
  tablespace settle_idx_tbs
  pctfree 10
  initrans 2
  maxtrans 255;

  alter table HT_TBL_CHILD_INF
  add constraint PK_HT_TBL_CHILD_INF primary key (INTER_BRH_CODE, SETTLMT_DATE, MISSION_INDEX, CHILD_ID)
  using index 
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_CHILD_INF on HT_TBL_CHILD_INF (INTER_BRH_CODE, SETTLMT_DATE)
  tablespace settle_idx_tbs
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

  alter table HT_TBL_CHILD_TIME
  add constraint PK_HT_TBL_CHILD_TIME primary key (INTER_BRH_CODE, MISSION_INDEX, MISSION_DATE, MISSION_HOUR)
  using index 
  tablespace settle_idx_tbs
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

  alter table HT_TBL_CIB_TXN
  add constraint PK_HT_TBL_CIB_TXN primary key (INTER_BRH_CODE, PLAT_KEY, TRANS_TP)
  using index 
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_CIB_TXN on HT_TBL_CIB_TXN (INTER_BRH_CODE)
  tablespace settle_idx_tbs
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

alter table HT_TBL_CMP_FLOW
  add constraint PK_HT_TBL_CMP_FLOW primary key (INTER_BRH_CODE, PLAT_KEY)
  using index 
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_CMP_FLOW on HT_TBL_CMP_FLOW (INTER_BRH_CODE, SETT_DATE)
  tablespace settle_idx_tbs
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

  alter table HT_TBL_COM_CONFIG
  add constraint PK_HT_TBL_COM_CONFIG primary key (KEY1, KEY2, KEY3)
  using index 
  tablespace settle_idx_tbs
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

alter table HT_TBL_CON_INF
  add constraint PK_HT_TBL_CON_INF primary key (USAGE_KEY, TXN_NUM, CON_INDEX)
  using index 
  tablespace settle_idx_tbs
  pctfree 10
  initrans 2
  maxtrans 255;

  alter table HT_TBL_COST_CMP_INF
  add constraint PK_HT_TBL_COST_CMP_INF primary key (PLAT_KEY)
  using index 
  tablespace settle_idx_tbs
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

  alter table HT_TBL_DATE_INF
  add constraint PK_HT_TBL_DATE_INF primary key (INTER_BRH_CODE)
  using index 
  tablespace settle_idx_tbs
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

  alter table HT_TBL_DAT_FLOW
  add constraint PK_HT_TBL_DAT_FLOW primary key (INTER_BRH_CODE, PLAT_KEY)
  using index 
  tablespace settle_idx_tbs
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

  alter table HT_TBL_DISC_ALGO
  add constraint PK_HT_TBL_DISC_ALGO primary key (DISC_ID, DISC_STEP)
  using index 
  tablespace settle_idx_tbs
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

  alter table HT_TBL_ERR_FLOW
  add constraint PK_HT_TBL_ERR_FLOW primary key (INTER_BRH_CODE, PLAT_KEY, SETT_DATE)
  using index 
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_ERR_FLOW on HT_TBL_ERR_FLOW (SETT_DATE)
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_ERR_FLOW1 on HT_TBL_ERR_FLOW (SUBSTR(BUS_TP,0,4))
  tablespace settle_idx_tbs
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

  alter table HT_TBL_FLD_DSP
  add constraint PK_HT_TBL_FLD_DSP primary key (L_USAGE_KEY, I_FLD_ID)
  using index 
  tablespace settle_idx_tbs
  pctfree 10
  initrans 2
  maxtrans 255;

  alter table HT_TBL_FLD_TSF
  add constraint PK_HT_TBL_FLD_TSF primary key (L_USAGE_KEY, I_BUF_CHG_INDEX, I_SOUR_FLD_INDEX)
  using index 
  tablespace settle_idx_tbs
  pctfree 10
  initrans 2
  maxtrans 255;

  alter table HT_TBL_FUNDS_SHORT
  add constraint PK_HT_TBL_FUNDS_SHORT primary key (BANK_CODE, ACCT_NO, SETT_DATE)
  using index 
  tablespace settle_idx_tbs
  pctfree 10
  initrans 2
  maxtrans 255;

  alter table HT_TBL_GATEWAY_TXN
  add constraint PK_HT_TBL_GATEWAY_TXN primary key (INTER_BRH_CODE, PLAT_KEY)
  using index 
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_GATEWAY_TXN on HT_TBL_GATEWAY_TXN (TRANS_STATE, INTER_BRH_CODE)
  tablespace settle_idx_tbs
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

alter table HT_TBL_HOST_TXN
  add constraint PK_HT_TBL_HOST_TXN primary key (INTER_BRH_CODE, PLAT_KEY)
  using index 
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_HOST_TXN1 on HT_TBL_HOST_TXN (INTER_BRH_CODE, SETT_DATE)
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_HOST_TXN2 on HT_TBL_HOST_TXN (PLAT_KEY)
  tablespace settle_idx_tbs
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

  alter table HT_TBL_IPC_DFT
  add constraint PK_HT_TBL_IPC_DFT primary key (USAGE_KEY, IPC_DFT_INDEX)
  using index 
  tablespace settle_idx_tbs
  pctfree 10
  initrans 2
  maxtrans 255;

alter table HT_TBL_IPC_DFT_DSP
  add constraint PK_HT_TBL_IPC_DFT_DSP primary key (USAGE_KEY, COMP_KEY)
  using index 
  tablespace settle_idx_tbs
  pctfree 10
  initrans 2
  maxtrans 255;

alter table HT_TBL_MISSION_CFG
  add constraint PK_HT_TBL_MISSION_CFG primary key (MISSION_TYPE, MISSION_INDEX)
  using index 
  tablespace settle_idx_tbs
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

alter table HT_TBL_MISSION_INF
  add constraint PK_HT_TBL_MISSION_INF primary key (INTER_BRH_CODE, SETTLMT_DATE, MISSION_INDEX)
  using index 
  tablespace settle_idx_tbs
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

  alter table HT_TBL_MISSION_INFO
  add constraint PK_TBL_MISSION_INFO primary key (BRH_ID, SETT_DATE, SEQ_NO)
  using index 
  tablespace settle_idx_tbs
  pctfree 10
  initrans 2
  maxtrans 255;

  alter table HT_TBL_MISSION_TIME
  add constraint PK_HT_TBL_MISSION_TIME primary key (INTER_BRH_CODE, MISSION_INDEX)
  using index 
  tablespace settle_idx_tbs
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

  alter table HT_TBL_OPR_CTL
  add constraint PK_HT_TBL_OPR_CTL primary key (TBL_OPR_FLAG, OBJECT_NAME)
  using index 
  tablespace settle_idx_tbs
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

alter table HT_TBL_PART_INF
  add constraint PK_HT_TBL_PART_INF primary key (TBL_NAME)
  using index 
  tablespace settle_idx_tbs
  pctfree 10
  initrans 2
  maxtrans 255;

alter table HT_TBL_POSTION_TRIAL
  add constraint PK_HT_TBL_POSTION_TRIAL primary key (BANK_CODE, ACCT_NO, SETT_DATE)
  using index 
  tablespace settle_idx_tbs
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

alter table HT_TBL_RISK_TXN
  add constraint PK_HT_TBL_RISK_TXN primary key (TRANS_DATE, GATEWAY_KEY, SETT_MCHT_NO)
  using index 
  tablespace settle_idx_tbs
  pctfree 10
  initrans 2
  maxtrans 255;
create index IDX_HT_TBL_RISK_TXN on HT_TBL_RISK_TXN (SETT_DATE)
  tablespace settle_idx_tbs
  pctfree 10
  initrans 2
  maxtrans 255;
create index IDX_HT_TBL_RISK_TXN2 on HT_TBL_RISK_TXN (EXT_MCHT_NO)
  tablespace settle_idx_tbs
  pctfree 10
  initrans 2
  maxtrans 255;

alter table HT_TBL_SETT_CHAN
  add constraint PK_HT_TBL_SETT_CHAN primary key (CHNL_ID)
  using index 
  tablespace settle_idx_tbs
  pctfree 10
  initrans 2
  maxtrans 255;


alter table HT_TBL_SETT_FLOW
  add constraint PK_HT_TBL_SETT_FLOW primary key (INTER_BRH_CODE, PLAT_KEY)
  using index 
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_SETT_FLOW1 on HT_TBL_SETT_FLOW (SETT_DATE)
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_SETT_FLOW10 on HT_TBL_SETT_FLOW (MCHT_NO5)
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_SETT_FLOW11 on HT_TBL_SETT_FLOW (MCHT_NO6)
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_SETT_FLOW12 on HT_TBL_SETT_FLOW (SETT_TP1)
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_SETT_FLOW13 on HT_TBL_SETT_FLOW (SETT_TP2)
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_SETT_FLOW14 on HT_TBL_SETT_FLOW (SETT_TP3)
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_SETT_FLOW15 on HT_TBL_SETT_FLOW (SETT_TP4)
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_SETT_FLOW16 on HT_TBL_SETT_FLOW (SETT_TP5)
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_SETT_FLOW17 on HT_TBL_SETT_FLOW (SETT_TP6)
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_SETT_FLOW2 on HT_TBL_SETT_FLOW (SETT_DATE, INTER_BRH_CODE)
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_SETT_FLOW3 on HT_TBL_SETT_FLOW (SETT_DATE, TRANS_TP)
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_SETT_FLOW4 on HT_TBL_SETT_FLOW (TRANS_TP)
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_SETT_FLOW5 on HT_TBL_SETT_FLOW (SUBSTR(BUS_TP,0,4))
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_SETT_FLOW6 on HT_TBL_SETT_FLOW (MCHT_NO1)
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_SETT_FLOW7 on HT_TBL_SETT_FLOW (MCHT_NO2)
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_SETT_FLOW8 on HT_TBL_SETT_FLOW (MCHT_NO3)
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_SETT_FLOW9 on HT_TBL_SETT_FLOW (MCHT_NO4)
  tablespace settle_idx_tbs
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

alter table HT_TBL_SETT_LIST
  add constraint PK_HT_TBL_SETT_LIST primary key (BRH_ID, SETT_DATE, MCHT_RSLT_NO, MCHT_ROLE, DC_FLAG)
  using index 
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_SETT_LIST on HT_TBL_SETT_LIST (MCHT_NO)
  tablespace settle_idx_tbs
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

  alter table HT_TBL_SETT_RSLT
  add constraint PK_HT_TBL_SETT_RSLT primary key (INTER_BRH_CODE, SETT_DATE, SEQ_NUM)
  using index 
  tablespace settle_idx_tbs
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

alter table HT_TBL_TXN_ALGO_RULE
  add constraint PK_HT_TBL_TXN_ALGO_RULE primary key (BUS_TP, TXN_NUM)
  using index 
  tablespace settle_idx_tbs
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

alter table HT_TBL_WIFT_TXN
  add constraint PK_HT_TBL_WIFT_TXN primary key (INTER_BRH_CODE, SETT_DATE, PLAT_KEY, WX_REFUND_ORDER)
  using index 
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_WIFT_TXN on HT_TBL_WIFT_TXN (INTER_BRH_CODE)
  tablespace settle_idx_tbs
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
create index IDX_HT_TBL_WIFT_TXN1 on HT_TBL_WIFT_TXN (INTER_BRH_CODE, SETT_DATE)
  tablespace settle_idx_tbs
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

  alter table TBL_ALIPAY_WECHAT_TXN
  add constraint PK_TBL_ALIPAY_WECHAT_TXN primary key (INTER_BRH_CODE, SETT_DATE, PLAT_KEY)
  using index 
  tablespace SETTLEMENT_SP
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
create index IDX_TBL_ALIPAY_WECHAT_TXN1 on TBL_ALIPAY_WECHAT_TXN (INTER_BRH_CODE, SETT_DATE)
  tablespace SETTLEMENT_SP
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