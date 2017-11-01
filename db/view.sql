-----------------------------------------------------
-- Export file for user SETTLEMENT                 --
-- Created by Administrator on 2017/9/25, 17:34:31 --
-----------------------------------------------------

spool VIEW.log





CREATE OR REPLACE VIEW CHARGE_ALLOT_REPORT AS
SELECT "SETT_DATE","TRANS_DATE","TRANS_TIME","TERM_NO","PLAT_KEY","PAN","PARTNER","ROLE_ID","PARTNER_NM","TRANS_TP","TRANS_TP_ID","MCHT_RATE","TRANS_AT","MCHT_FEE","MCHT_AT","REMARK"FROM
    (
      SELECT a.SETT_DATE, a.TRANS_DATE, a.TRANS_TIME, '' AS TERM_NO, a.PLAT_KEY, PAN, '结算商户' AS PARTNER, '007' ROLE_ID,b.MCHT_NM AS PARTNER_NM,
            decode(substr(a.TRANS_TP, 0, 4),'1201', '充值','1301', '提现', '5001','退款', '1001', '消费') TRANS_TP,
            substr(a.TRANS_TP, 0, 4) trans_tp_id,
            decode(trim(a.INTER_BRH_CODE), '100001', TO_CHAR(a.MCHT_RATE1*100,'990.99'), '100002', TO_CHAR(a.MCHT_RATE1*100,'990.99'), '100003', TO_CHAR(a.MCHT_RATE1*100,'990.99'), '100004', TO_CHAR(a.MCHT_RATE1*100,'990.99'), '100005', TO_CHAR(a.MCHT_RATE1*100,'990.99'), '100006', TO_CHAR(a.MCHT_RATE1*100,'990.99')) AS MCHT_RATE,
            a.TRANS_AT/100 AS TRANS_AT,
            decode(substr(a.TRANS_TP, 0, 4),'1201', a.CRET_FEE_MCHT_NO1/100, '1301', a.CRET_FEE_MCHT_NO1/100, '5001', a.DEBT_FEE_MCHT_NO1/100, '1001', a.CRET_FEE_MCHT_NO1/100) MCHT_FEE,
            decode(substr(a.TRANS_TP, 0, 4),'1201', a.DEBT_AT_MCHT_NO1/100, '1301', a.CRET_AT_MCHT_NO1/100, '5001', a.CRET_AT_MCHT_NO1/100, '1001', a.DEBT_AT_MCHT_NO1/100) MCHT_AT,
            a.REMARK
      FROM HT_TBL_SETT_FLOW a, HT_TBL_MCHT_INF@PUBLIC_DATA b
      WHERE a.MCHT_NO1 = b.MCHT_NO
      UNION ALL
      SELECT a.SETT_DATE, a.TRANS_DATE, a.TRANS_TIME, '' AS TERM_NO, a.PLAT_KEY, PAN, '分润机构' AS PARTNER,'001' ROLE_ID, c.BRH_NM AS PARTNER_NM,
            decode(substr(a.TRANS_TP, 0, 4),'1201', '充值','1301', '提现', '5001','退款', '1001', '消费') TRANS_TP,
            substr(a.TRANS_TP, 0, 4) trans_tp_id,
            TO_CHAR(a.MCHT_RATE2*100,'990.99') AS MCHT_RATE, a.TRANS_AT/100 AS TRANS_AT,
            0.00 AS MCHT_FEE, a.SHARE_PROFIT_MCHT_NO2/100 AS MCHT_AT,
            a.REMARK
      FROM HT_TBL_SETT_FLOW a, HT_TBL_MCHT_INF@PUBLIC_DATA b, HT_TBL_BRH_INF@PUBLIC_DATA c
      WHERE a.MCHT_NO1 = b.MCHT_NO AND
            TRIM(b.SPL_BRH_ROLE1) = c.BRH_NO
      UNION ALL
      SELECT a.SETT_DATE, a.TRANS_DATE, a.TRANS_TIME, '' AS TERM_NO, a.PLAT_KEY, PAN, '分润机构' AS PARTNER,'001' ROLE_ID, c.BRH_NM AS PARTNER_NM,
            decode(substr(a.TRANS_TP, 0, 4),'1201', '充值','1301', '提现', '5001','退款', '1001', '消费') TRANS_TP,
            substr(a.TRANS_TP, 0, 4) trans_tp_id,
            decode(b.SPLITTING_TYPE, '01', TO_CHAR(a.MCHT_RATE3*100,'990.99'), '02', TO_CHAR(a.MCHT_RATE3/(1-a.MCHT_RATE2)*100,'990.99')) AS MCHT_RATE,
            a.TRANS_AT/100 AS TRANS_AT,
            0.00 AS MCHT_FEE, a.SHARE_PROFIT_MCHT_NO3/100 AS MCHT_AT,
            a.REMARK
      FROM HT_TBL_SETT_FLOW a, HT_TBL_MCHT_INF@PUBLIC_DATA b, HT_TBL_BRH_INF@PUBLIC_DATA c
      WHERE a.MCHT_NO1 = b.MCHT_NO AND
            TRIM(b.SPL_BRH_ROLE2) = c.BRH_NO
      UNION ALL
      SELECT a.SETT_DATE, a.TRANS_DATE, a.TRANS_TIME, '' AS TERM_NO, a.PLAT_KEY, PAN, '分润机构' AS PARTNER,'001' ROLE_ID, c.BRH_NM AS PARTNER_NM,
            decode(substr(a.TRANS_TP, 0, 4),'1201', '充值','1301', '提现', '5001','退款', '1001', '消费') TRANS_TP,
            substr(a.TRANS_TP, 0, 4) trans_tp_id,
            decode(b.SPLITTING_TYPE, '01', TO_CHAR(a.MCHT_RATE4*100,'990.99'), '02', TO_CHAR(a.MCHT_RATE4/(1-a.MCHT_RATE2 - a.MCHT_RATE3)*100,'990.99')) AS MCHT_RATE,
            a.TRANS_AT/100 AS TRANS_AT,
            0.00 AS MCHT_FEE, a.SHARE_PROFIT_MCHT_NO4/100 AS MCHT_AT,
            a.REMARK
      FROM HT_TBL_SETT_FLOW a, HT_TBL_MCHT_INF@PUBLIC_DATA b, HT_TBL_BRH_INF@PUBLIC_DATA c
      WHERE a.MCHT_NO1 = b.MCHT_NO AND
            TRIM(b.SPL_BRH_ROLE3) = c.BRH_NO
      UNION ALL
      SELECT a.SETT_DATE, a.TRANS_DATE, a.TRANS_TIME, '' AS TERM_NO, a.PLAT_KEY, PAN, '分润机构' AS PARTNER,'001' ROLE_ID, c.BRH_NM AS PARTNER_NM,
            decode(substr(a.TRANS_TP, 0, 4),'1201', '充值','1301', '提现', '5001','退款', '1001', '消费') TRANS_TP,
            substr(a.TRANS_TP, 0, 4) trans_tp_id,
            decode(b.SPLITTING_TYPE, '01', TO_CHAR(a.MCHT_RATE5*100,'990.99'), '02', TO_CHAR(a.MCHT_RATE5/(1 - a.MCHT_RATE2 - a.MCHT_RATE3-a.MCHT_RATE4)*100,'990.99')) AS MCHT_RATE,
            a.TRANS_AT/100 AS TRANS_AT,
            0.00 AS MCHT_FEE, a.SHARE_PROFIT_MCHT_NO5/100 AS MCHT_AT,
            a.REMARK
      FROM HT_TBL_SETT_FLOW a, HT_TBL_MCHT_INF@PUBLIC_DATA b, HT_TBL_BRH_INF@PUBLIC_DATA c
      WHERE a.MCHT_NO1 = b.MCHT_NO AND
            TRIM(b.SPL_BRH_ROLE4) = c.BRH_NO
      UNION ALL
      SELECT a.SETT_DATE, a.TRANS_DATE, a.TRANS_TIME, '' AS TERM_NO, a.PLAT_KEY, PAN, '支付机构' AS PARTNER,'004' ROLE_ID, '兴业银行' AS PARTNER_NM,
            decode(substr(a.TRANS_TP, 0, 4),'1201', '充值','1301', '提现', '5001','退款', '1001', '消费') TRANS_TP,
            substr(a.TRANS_TP, 0, 4) trans_tp_id,
            '0.00' AS MCHT_RATE, a.TRANS_AT/100 AS TRANS_AT,
            0.00 AS MCHT_FEE, a.SHARE_PROFIT_MCHT_NO6/100 AS MCHT_AT,
            a.REMARK
      FROM HT_TBL_SETT_FLOW a, HT_TBL_MCHT_INF@PUBLIC_DATA b
      WHERE a.MCHT_NO1 = b.MCHT_NO
      UNION ALL
      SELECT a.SETT_DATE, a.TRANS_DATE, a.TRANS_TIME, '' AS TERM_NO, a.PLAT_KEY, PAN, '运营平台' AS PARTNER,'010' ROLE_ID, '运营平台' AS PARTNER_NM,
            decode(substr(a.TRANS_TP, 0, 4),'1201', '充值','1301', '提现', '5001','退款', '1001', '消费') TRANS_TP,
            substr(a.TRANS_TP, 0, 4) trans_tp_id,
            '0.00' AS MCHT_RATE, a.TRANS_AT/100 AS TRANS_AT,
            0.00 AS MCHT_FEE, a.SHARE_PROFIT_PLAT/100 AS MCHT_AT,
            a.REMARK
      FROM HT_TBL_SETT_FLOW a, HT_TBL_MCHT_INF@PUBLIC_DATA b
      WHERE a.MCHT_NO1 = b.MCHT_NO
     ) c ORDER BY c.PLAT_KEY, c.PARTNER;





CREATE OR REPLACE VIEW FUND_CHECK_VIEW AS
select
       k.SETT_DATE, k.TRANS_DATE, k.TRANS_TIME, k.ACCT_TYPE, k.PLAT_KEY, k.TRANS_NAME TRANS_TP, k.PAN s_pan, substr(k.BUS_TP, 0, 4) BUS_TPS,
       TO_CHAR(trim(nvl(k.TRANS_AT,0)/100),'9999999999999.99') s_trans_at,
       decode(substr(k.BUS_TP, 0, 4),'0301','兴业银行-快捷支付','0302','兴业银行-网银支付','0303','兴业银行-微信支付','0304','兴业银行-支付宝支付','0400','星民易付') BUS_TP,
       decode(substr(k.FEE_FLAG, 0, 1), '1', '0.00', TO_CHAR(k.FEE_VALUE, '9999999999999.99')) FEE_RATE,
       decode(substr(k.FEE_FLAG, 0, 1), '1', TO_CHAR(k.FEE_VALUE,'9999999999999.99'), TO_CHAR(round(abs(k.FEE_VALUE/100*(k.TRANS_AT/100)), 2),'9999999999999.99')) s_trans_fee,
       TO_CHAR(round(abs(k.SHARE_PROFIT_MCHT_NO6/100), 2),'9999999999999.99') as d_trans_fee,
       TO_CHAR(round(abs(decode(substr(k.FEE_FLAG, 0, 1), '1', TO_CHAR(k.FEE_VALUE,'9999999999999.99'), TO_CHAR(round(abs(k.FEE_VALUE/100*(k.TRANS_AT/100)), 2),'9999999999999.99'))),2)-round(abs(k.SHARE_PROFIT_MCHT_NO6/100),2),'9999999999999.99') as fee_error,
       --TO_CHAR(k.SHARE_PROFIT_MCHT_NO6/100,'9999999999999.99') d_trans_fee,
       --TO_CHAR(k.TRANS_AT2/100,'9999999999999.99') s_trans_fee,
       --TO_CHAR((k.SHARE_PROFIT_MCHT_NO6-TRANS_AT2)/100,'9999999999999.99') fee_error,
       REMARK reserve
from (
  select f.*, h.txn_name TRANS_NAME from
  (
    select e.FEE_FLAG, e.FEE_VALUE, d.*  FROM
      (SELECT c.PAYWAY_CODE, b.*, c.CHANNEL_CODE FROM
        (select a.*,
          CASE WHEN substr(trim(a.BUS_TP), 0, 4)='0304' THEN 'XY03'
               WHEN substr(trim(a.BUS_TP), 0, 4)='0303' THEN 'XY04'
               WHEN substr(trim(a.BUS_TP), 0, 4)='0301' THEN 'XY01'
               WHEN substr(trim(a.BUS_TP), 0, 4)='0302' THEN 'XY02'
               WHEN substr(trim(a.BUS_TP), 0, 4)='0400' THEN 'XMYF01'
          END as PAYWAY_TYPE
        from HT_TBL_SETT_FLOW a) b, TP_PAY_CHANNEL@PUBLIC_DATA c
       where b.PAYWAY_TYPE=c.CHANNEL_CODE) d, HT_TBL_BRH_FEE_INF@PUBLIC_DATA e
    where d.CHANNEL_CODE=e.PAYWAY_CODE
 ) f
 left join (select t.txn_num,max(t.txn_name) txn_name from HT_TBL_ALGO_CTL t group by t.txn_num)  h on f.trans_tp=h.txn_num
 where substr(f.inter_brh_code, 0, 6) in('100001','100002')
) k
union all
select
       k.SETT_DATE, k.TRANS_DATE, k.TRANS_TIME, k.ACCT_TYPE, k.PLAT_KEY, k.TRANS_NAME TRANS_TP, k.PAN s_pan, substr(k.BUS_TP, 0, 4) BUS_TPS,
       TO_CHAR(trim(nvl(k.TRANS_AT,0)/100),'9999999999999.99') s_trans_at,
       decode(substr(k.BUS_TP, 0, 4),'0301','兴业银行-快捷支付','0302','兴业银行-网银支付','0303','兴业银行-微信支付','0304','兴业银行-支付宝支付','0400','星民易付') BUS_TP,
       decode(substr(k.FEE_FLAG, 0, 1), '1', '0.00', TO_CHAR(k.FEE_VALUE, '9999999999999.99')) FEE_RATE,
       decode(substr(k.FEE_FLAG, 0, 1), '1', TO_CHAR(k.FEE_VALUE,'9999999999999.99'), TO_CHAR(round(abs(k.FEE_VALUE/100*(k.TRANS_AT/100)), 2),'9999999999999.99')) s_trans_fee,
       TO_CHAR(round(abs(k.TRANS_AT2/100), 2),'9999999999999.99') as d_trans_fee,
       TO_CHAR(round(abs(decode(substr(k.FEE_FLAG, 0, 1), '1', TO_CHAR(k.FEE_VALUE,'9999999999999.99'), TO_CHAR(round(abs(k.FEE_VALUE/100*(k.TRANS_AT/100)), 2),'9999999999999.99'))),2)-round(abs(k.TRANS_AT2/100),2),'9999999999999.99') as fee_error,
       REMARK reserve
from (
  select f.*, h.txn_name TRANS_NAME from
  (
    select e.FEE_FLAG, e.FEE_VALUE, d.*  FROM
      (SELECT c.PAYWAY_CODE, b.*, c.CHANNEL_CODE FROM
        (select a.*,
          CASE WHEN substr(trim(a.BUS_TP), 0, 4)='0304' THEN 'XY03'
               WHEN substr(trim(a.BUS_TP), 0, 4)='0303' THEN 'XY04'
               WHEN substr(trim(a.BUS_TP), 0, 4)='0301' THEN 'XY01'
               WHEN substr(trim(a.BUS_TP), 0, 4)='0302' THEN 'XY02'
               WHEN substr(trim(a.BUS_TP), 0, 4)='0400' THEN 'XMYF01'
               WHEN substr(trim(a.BUS_TP), 0, 4)='0504' THEN 'FY03'
               WHEN substr(trim(a.BUS_TP), 0, 4)='0503' THEN 'FY04'
          END as PAYWAY_TYPE
        from HT_TBL_SETT_FLOW a) b, TP_PAY_CHANNEL@PUBLIC_DATA c
       where b.PAYWAY_TYPE=c.CHANNEL_CODE) d, HT_TBL_BRH_FEE_INF@PUBLIC_DATA e
    where d.CHANNEL_CODE=e.PAYWAY_CODE
 ) f
 left join (select t.txn_num,max(t.txn_name) txn_name from HT_TBL_ALGO_CTL t group by t.txn_num)  h on f.trans_tp=h.txn_num
 where substr(f.inter_brh_code, 0, 6) in('100004','100005', '100006')
) k
union all
select
       k.SETT_DATE, k.TRANS_DATE, k.TRANS_TIME, k.ACCT_TYPE, k.PLAT_KEY, k.TRANS_TP, k.PAN s_pan, substr(k.BUS_TP, 0, 4) BUS_TPS,
       TO_CHAR(trim(nvl(k.TRANS_AT,0)/100),'9999999999999.99') s_trans_at,
       decode(substr(k.BUS_TP, 0, 4),'0301','兴业银行-快捷支付','0302','兴业银行-网银支付','0303','兴业银行-微信支付','0304','兴业银行-支付宝支付','0400','星民易付') BUS_TP,
       '0.00' FEE_RATE,
       '0.00' s_trans_fee,
       '0.00' d_trans_fee,
       '0.00' fee_error,
       REMARK reserve
from
(
    select
       t.SETT_DATE, t.TRANS_DATE, t.TRANS_TIME, '' ACCT_TYPE, t.PLAT_KEY, h.txn_name TRANS_TP, t.PAN,
       t.TRANS_AT, t.BUS_TP, t.REMARK
    from HT_TBL_SETT_FLOW t
    left join (select t.txn_num,max(t.txn_name) txn_name from HT_TBL_ALGO_CTL t group by t.txn_num)  h on t.trans_tp=h.txn_num
    where substr(t.inter_brh_code, 0, 6) = '100003'
) k;





CREATE OR REPLACE VIEW MCHT_FUND_REPORT AS
SELECT a.TRANS_DATE, a.TRANS_TIME, '' AS TERM_NO, a.PLAT_KEY,
       decode(substr(a.INTER_BRH_CODE, 0, 6),'100001', '兴业银行-支付宝','100002', '兴业银行-微信',
                                       '100003','虚拟卡支付', '100004', '兴业银行-快捷支付', '100005', '兴业银行-网银支付', '100006', '富友支付') INTER_BRH_CODE,
       decode(substr(a.TRANS_TP, 0, 4),'1201', '充值','1301', '提现', '5001','退款', '1001', '消费') TRANS_TP,
       a.PAN, a.TRANS_AT/100 AS TRANS_AT, abs(a.MCHT_RATE1*100) AS MCHT_RATE,
       decode(substr(a.TRANS_TP, 0, 4),'1201', a.CRET_FEE_MCHT_NO1/100, '1301', a.CRET_FEE_MCHT_NO1/100, '5001', a.DEBT_FEE_MCHT_NO1/100, '1001', a.CRET_FEE_MCHT_NO1/100) MCHT_FEE,
       decode(substr(a.TRANS_TP, 0, 4),'1201', a.DEBT_AT_MCHT_NO1/100, '1301', a.CRET_AT_MCHT_NO1/100, '5001', a.CRET_AT_MCHT_NO1/100, '1001', a.DEBT_AT_MCHT_NO1/100) MCHT_AT,
       a.REMARK, a.MCHT_NO1, a.SETT_DATE
      FROM HT_TBL_SETT_FLOW a;





CREATE OR REPLACE VIEW MCHT_PAY_REPORT AS
SELECT SETT_DATE, MCHT_NO, MCHT_NM, MCHT_ROLE, MCHT_SETT_ACCT, MCHT_SETT_ACCT_NM, MCHT_SETT_BANK_NM, DC_FLAG,
       PLAT_SETT_ACCT, PLAT_SETT_ACCT_NM, PLAT_SETT_BANK_NM, PAY_WAY, SEQ_NO, AMT, AMT_FEE, REMARK
FROM (
  SELECT a.SETT_DATE, a.MCHT_NO, b.MCHT_NM,
       DECODE(SUBSTR(a.MCHT_ROLE, 0, 4), '0011', '个人提现', '0031', '商户提现', '1001', '结算商户') MCHT_ROLE,
       a.MCHT_SETT_ACCT, a.MCHT_SETT_ACCT_NM, a.MCHT_SETT_BANK_NM,
       DECODE(SUBSTR(a.DC_FLAG, 0, 1), 'C', '支出', 'D', '收入') DC_FLAG,
       a.PLAT_SETT_ACCT, a.PLAT_SETT_ACCT_NM, a.PLAT_SETT_BANK_NM, '兴业代付' PAY_WAY, a.SEQ_NO, a.AMT, a.AMT_FEE, '' REMARK
  FROM HT_TBL_SETT_LIST a, HT_TBL_MCHT_INF@PUBLIC_DATA b
  WHERE a.MCHT_NO = b.MCHT_NO AND
      a.MCHT_ROLE IN ('0011', '0031', '1001')
UNION ALL
  SELECT a.SETT_DATE, a.MCHT_NO, b.BRH_NM,
       DECODE(SUBSTR(a.MCHT_ROLE, 0, 4), '1002', '分润机构(二清)', '2002', '分润机构(直连)') MCHT_ROLE,
       a.MCHT_SETT_ACCT, a.MCHT_SETT_ACCT_NM, a.MCHT_SETT_BANK_NM,
       DECODE(SUBSTR(a.DC_FLAG, 0, 1), 'C', '支出', 'D', '收入') DC_FLAG,
       a.PLAT_SETT_ACCT, a.PLAT_SETT_ACCT_NM, a.PLAT_SETT_BANK_NM, '兴业代付' PAY_WAY, a.SEQ_NO, a.AMT, a.AMT_FEE, '' REMARK
  FROM HT_TBL_SETT_LIST a, HT_TBL_BRH_INF@PUBLIC_DATA b
  WHERE a.MCHT_NO = b.BRH_NO AND
      a.MCHT_ROLE IN ('1002', '2002'));





create or replace view negative_pay_report as
select a.SETT_DATE, a.ACCT_DATE TRANS_DATE, a.MCHT_NO, b.MCHT_NM, a.MCHT_SETT_ACCT, a.MCHT_SETT_ACCT_NM, a.DEBT_AT-a.CRET_AT AMT,
         decode(substr(a.ACCT_STATU, 0, 1), 'Y', '已入账', 'N', '未入账', '9', '处理中','1', '失败挂账','2', '挂账恢复','3', '挂账恢复成功','P', '暂缓挂账','R', '暂缓恢复','4', '核心记账失败','5', '收入已入账','6', '支出已入账') ACCT_STATU
    from ht_tbl_acct_rslt a, HT_TBL_MCHT_INF@PUBLIC_DATA b
    where a.MCHT_NO = b.MCHT_NO AND
          A.MCHT_SETT_ACCT_TYPE != '3';





CREATE OR REPLACE VIEW PROFIT_LOSS_REPORT AS
SELECT b.SETT_DATE, b.MCHT_NO1, c.MCHT_NM, '' TERM_NO, b.SHARE_PROFIT_PLAT, '0.00' PAY_PROFIT_PLAT,
         b.SHARE_PROFIT_PLAT AS PROFIT_PROFIT_PLAT, '' REMARK
  FROM (
      SELECT a.SETT_DATE, a.MCHT_NO1, SUM(a.SHARE_PROFIT_PLAT/100) AS SHARE_PROFIT_PLAT, '0.00' as PAY_PROFIT_PLAT
      FROM HT_TBL_SETT_FLOW a
      GROUP BY a.SETT_DATE, a.MCHT_NO1
      ) b, HT_TBL_MCHT_INF@PUBLIC_DATA c
  WHERE b.MCHT_NO1 = c.MCHT_NO;





CREATE OR REPLACE VIEW SERVICE_CHARGE_REPORT AS
SELECT a.SETT_DATE, a.TRANS_DATE, a.TRANS_TIME, a.MCHT_NO1, b.MCHT_NM, '' TERM_NO,  a.PLAT_KEY,
            decode(substr(a.INTER_BRH_CODE, 0, 6),'100001', '兴业银行-支付宝','100002', '兴业银行-微信',
                 '100003','虚拟卡支付', '100004', '兴业银行-快捷支付', '100005', '兴业银行-网银支付', '100006', '富友支付') INTER_BRH_CODE,
            decode(substr(a.TRANS_TP, 0, 4),'1201', '充值','1301', '提现', '5001','退款', '1001', '消费') TRANS_TP,
      a.pan, a.TRANS_AT/100 AS TRANS_AT,
            decode(substr(a.TRANS_TP, 0, 4),'1201', a.CRET_FEE_MCHT_NO1/100, '1301', a.CRET_FEE_MCHT_NO1/100, '5001', a.DEBT_FEE_MCHT_NO1/100, '1001', a.CRET_FEE_MCHT_NO1/100) MCHT_FEE,
            a.SHARE_PROFIT_MCHT_NO2/100 + a.SHARE_PROFIT_MCHT_NO3/100 + a.SHARE_PROFIT_MCHT_NO4/100 + a.SHARE_PROFIT_MCHT_NO5/100 AS SHARE_PROFIT,
      a.SHARE_PROFIT_MCHT_NO6/100 AS SHARE_PROFIT_PAY, a.SHARE_PROFIT_PLAT/100 AS SHARE_PROFIT_PLAT,
      a.REMARK
      FROM HT_TBL_SETT_FLOW a, HT_TBL_MCHT_INF@PUBLIC_DATA b
      WHERE a.MCHT_NO1 = b.MCHT_NO;





create or replace view sett_flow_view as
select t.sett_date,t.trans_date,t.trans_time,'' zhong_duan_hao,t.plat_key,t.trans_tp trans_tp_id,c.txn_name trans_tp,
       t.pan,t.trans_at,'结算商户' role,t.mcht_no1 mcht_no,m.mcht_nm worker,t.MCHT_RATE1 shou_xu_feilv,to_number(null) fen_pen_jin_er,
       (t.CRET_FEE_MCHT_NO1-t.DEBT_FEE_MCHT_NO1) shou_xu_fei,t.remark bak
 from HT_TBL_SETT_FLOW t left join multicard.HT_TBL_MCHT_INF@public_data m on trim(t.mcht_no1)=trim(m.mcht_no)
 left join (select m.txn_num , max(m.txn_name) txn_name
 from HT_TBL_ALGO_CTL m group by m.txn_num) c on t.trans_tp=c.txn_num where t.sett_tp1='007'
union all
select t.sett_date,t.trans_date,t.trans_time,'' zhong_duan_hao,t.plat_key,t.trans_tp trans_tp_id,c.txn_name trans_tp,
      t.pan,t.trans_at,'分润机构' role,t.mcht_no2 mcht_no,m.brh_nm worker,t.MCHT_RATE2 shou_xu_feilv,
      t.SHARE_PROFIT_MCHT_NO2 fen_pen_jin_er,to_number(null) shou_xu_fei,t.remark bak
 from HT_TBL_SETT_FLOW t left join multicard.HT_TBL_BRH_INF@public_data m on trim(t.mcht_no2)=trim(m.brh_no)
 left join (select m.txn_num , max(m.txn_name) txn_name
 from HT_TBL_ALGO_CTL m group by m.txn_num) c on t.trans_tp=c.txn_num where t.sett_tp2='001'
union all
select t.sett_date,t.trans_date,t.trans_time,'' zhong_duan_hao,t.plat_key,t.trans_tp trans_tp_id,c.txn_name trans_tp,
        t.pan,t.trans_at,'分润机构' role,t.mcht_no3 mcht_no,m.brh_nm worker,t.MCHT_RATE3 shou_xu_feilv,
        t.SHARE_PROFIT_MCHT_NO3 fen_pen_jin_er,to_number(null) shou_xu_fei,t.remark bak
 from HT_TBL_SETT_FLOW t left join multicard.HT_TBL_BRH_INF@public_data m on trim(t.mcht_no3)=trim(m.brh_no)
 left join (select m.txn_num , max(m.txn_name) txn_name
 from HT_TBL_ALGO_CTL m group by m.txn_num) c on t.trans_tp=c.txn_num where t.sett_tp3='001'
union all
select t.sett_date,t.trans_date,t.trans_time,'' zhong_duan_hao,t.plat_key,t.trans_tp trans_tp_id,c.txn_name trans_tp,
        t.pan,t.trans_at,'分润机构' role,t.mcht_no4 mcht_no,m.brh_nm worker,t.MCHT_RATE4 shou_xu_feilv,
        t.SHARE_PROFIT_MCHT_NO4 fen_pen_jin_er,to_number(null) shou_xu_fei,t.remark bak
 from HT_TBL_SETT_FLOW t left join multicard.HT_TBL_BRH_INF@public_data m on trim(t.mcht_no4)=trim(m.brh_no)
 left join (select m.txn_num , max(m.txn_name) txn_name
 from HT_TBL_ALGO_CTL m group by m.txn_num) c on t.trans_tp=c.txn_num where t.sett_tp4='001'
union all
select t.sett_date,t.trans_date,t.trans_time,'' zhong_duan_hao,t.plat_key,t.trans_tp trans_tp_id,c.txn_name trans_tp,
        t.pan,t.trans_at,'分润机构' role,t.mcht_no5 mcht_no,m.brh_nm worker,t.MCHT_RATE5 shou_xu_feilv,
        t.SHARE_PROFIT_MCHT_NO5 fen_pen_jin_er,to_number(null) shou_xu_fei,t.remark bak
 from HT_TBL_SETT_FLOW t left join multicard.HT_TBL_BRH_INF@public_data m on trim(t.mcht_no5)=trim(m.brh_no)
 left join (select m.txn_num , max(m.txn_name) txn_name
 from HT_TBL_ALGO_CTL m group by m.txn_num) c on t.trans_tp=c.txn_num where t.sett_tp5='001'
 union all
select t.sett_date,t.trans_date,t.trans_time,'' zhong_duan_hao,t.plat_key,t.trans_tp trans_tp_id,c.txn_name trans_tp,
      t.pan,t.trans_at,'支付机构' role,t.mcht_no6 mcht_no,m.brh_nm worker,t.MCHT_RATE6 shou_xu_feilv,
      SHARE_PROFIT_MCHT_NO6 fen_pen_jin_er,to_number(null) shou_xu_fei,t.remark bak
 from HT_TBL_SETT_FLOW t left join multicard.HT_TBL_BRH_INF@public_data m on trim(t.mcht_no6)=trim(m.brh_no)
 left join (select m.txn_num , max(m.txn_name) txn_name
 from HT_TBL_ALGO_CTL m group by m.txn_num) c on t.trans_tp=c.txn_num where t.sett_tp6='004'
union all
select t.sett_date,t.trans_date,t.trans_time,'' zhong_duan_hao,t.plat_key,t.trans_tp trans_tp_id,c.txn_name trans_tp,
      t.pan,t.trans_at,'星民易付平台' role,t.EXT_MCHT_NO mcht_no,m.EXT_MCH_NM worker,to_number(null) shou_xu_feilv,
      t.SHARE_PROFIT_PLAT fen_pen_jin_er,to_number(null) shou_xu_fei,t.remark bak
 from HT_TBL_SETT_FLOW t left join multicard.HT_TBL_EXT_MCHT_INF@public_data m on trim(t.EXT_MCHT_NO)=trim(m.EXT_MCH_NO)
 left join (select m.txn_num , max(m.txn_name) txn_name
 from HT_TBL_ALGO_CTL m group by m.txn_num) c on t.trans_tp=c.txn_num;





create or replace view sett_list_view as
select
      t.SETT_DATE,t.MCHT_NO,m.MCHT_NM,t.MCHT_SETT_ACCT,t.MCHT_SETT_ACCT_NM,t.MCHT_SETT_BANK_NM,
      decode(t.dc_flag,'C','支出','D','收入') flag ,t.AMT_FEE,
      decode(t.mcht_role,'1001','商户结算') mcht_role,
      t.PLAT_SETT_ACCT , t.PLAT_SETT_ACCT_NM, t.PLAT_SETT_BANK_NM , t.chnl_name CHNL_NM, t.seq_no RSLT_NO,
      t.amt transAt
from HT_TBL_SETT_LIST t
left join multicard.HT_TBL_MCHT_INF@public_data m on trim(t.MCHT_NO)=trim(m.mcht_no) where STATUS!=5 and  t.mcht_role='1001'
UNION ALL
select
      t.SETT_DATE,t.MCHT_NO,m.BRH_NM,t.MCHT_SETT_ACCT,t.MCHT_SETT_ACCT_NM,t.MCHT_SETT_BANK_NM,
      decode(t.dc_flag,'C','支出','D','收入') flag ,t.AMT_FEE,
      decode(t.mcht_role,'1002','机构分润','2002','机构分润') mcht_role,
      t.PLAT_SETT_ACCT , t.PLAT_SETT_ACCT_NM, t.PLAT_SETT_BANK_NM , t.chnl_name CHNL_NM, t.seq_no RSLT_NO,
      t.amt transAt
from HT_TBL_SETT_LIST t
left join multicard.HT_TBL_BRH_INF@public_data m on trim(t.MCHT_NO)=trim(m.BRH_NO) where STATUS!=5 and t.mcht_role IN ('1002','2002');





create or replace view shgzch_view as
select t.MCHT_RSLT_NO,t.SETT_DATE,t.acct_date,t.MCHT_NO,
       trim(t.MCHT_SETT_BANK) MCHT_SETT_BANK,trim(t.MCHT_SETT_BANK_NM) MCHT_SETT_BANK_NM,
       trim(t.MCHT_SETT_ACCT) MCHT_SETT_ACCT,trim(t.MCHT_SETT_ACCT_NM) MCHT_SETT_ACCT_NM,
       ((t.DEBT_AT+t.DEBT_NET_FEE)-(t.CRET_AT+CRET_NET_FEE))||'' ACCT_MONEY,  t.acct_statu,
        decode(t.acct_statu,'1','失败挂账','2','挂账恢复','3','挂账恢复成功') acct_statu_name
from HT_TBL_ACCT_RSLT t  where t.SETT_MD='01' and t.acct_statu ='1' or t.acct_statu ='2' or t.acct_statu ='3'
union all
select t.MCHT_RSLT_NO,t.SETT_DATE,t.acct_date,t.MCHT_NO,
       trim(t.MCHT_SETT_BANK) MCHT_SETT_BANK,trim(t.MCHT_SETT_BANK_NM) MCHT_SETT_BANK_NM,
       trim(t.MCHT_SETT_ACCT) MCHT_SETT_ACCT,trim(t.MCHT_SETT_ACCT_NM) MCHT_SETT_ACCT_NM,
        decode(t.SZ_STATUS,1,(t.CRET_AT+t.CRET_NET_FEE)||'',2,(t.DEBT_AT+t.DEBT_NET_FEE)||'',
        3,'收:'||(t.CRET_AT+CRET_NET_FEE) || '支:'||(t.DEBT_AT+t.DEBT_NET_FEE)
         ) ACCT_MONEY,t.acct_statu,
        decode(t.acct_statu,'1','失败挂账','2','挂账恢复','3','挂账恢复成功') acct_statu_name
from HT_TBL_ACCT_RSLT t  where t.SETT_MD='02' and t.sz_status <>0;


spool off
