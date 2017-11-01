-----------------------------------------------------
-- Export file for user SETTLEMENT                 --
-- Created by Administrator on 2017/8/23, 13:50:19 --
-----------------------------------------------------

create or replace package P_LIQUIDATION_REPORT is

  procedure LIQUIDATION_REPORT;
  

end P_LIQUIDATION_REPORT;

create or replace procedure delete_ht_downland_record is
begin 
  --删除全部 
  --update  HT_DOWNLAND_RECORD t set t.delete_flag='1' where t.delete_flag='0';
  --删除一个星期（7天）以前的数据
  update  HT_DOWNLAND_RECORD t set t.delete_flag='1' where t.oper_start_time<to_char(sysdate-7,'yyyymmdd');
  commit; 
end;

create or replace package body P_LIQUIDATION_REPORT is
  procedure LIQUIDATION_REPORT is
  SETT_DATE VARCHAR2(8);
  REAL_YES_BALANCE NUMBER(16,2);
  REAL_TOD_BALANCE NUMBER(16,2);
  RECHARGE_INCOME NUMBER(16,2);
  ERQING_INCOME NUMBER(16,2);
  SERVICE_CHARGE_INCOME NUMBER(16,2);
  
  MCHT_OUTCOME NUMBER(16,2);
  FEE_OUTCOME NUMBER(16,2);
  WITHD_OUTCOME NUMBER(16,2);
  COST_OUTCOME NUMBER(16,2);
  begin
    INSERT INTO TBL_OPERATE_ANALYSIS@PUBLIC_DATA (
    SELECT e.INTER_BRH_CODE, 
           decode(trim(e.INTER_BRH_CODE), '100001', '支付宝', '100002', '微信', '100003', '虚拟账户', '100004', '快捷', '100005', '网银', '100006', '富友') as INTER_BRH_NAME,
           e.SETT_DATE, e.TRANS_TP, e.TOTAL_NUM, abs(e.TOTAL_AMT), 
           case when e.EQ_IN_HOST_NUM is null then 0 else e.EQ_IN_HOST_NUM end, 
           case when e.EQ_IN_HOST_AMT is null then 0.00 else abs(e.EQ_IN_HOST_AMT) end, 
           case when f.UEQ_IN_HOST_NUM is null then 0 else f.UEQ_IN_HOST_NUM end, 
           case when f.UEQ_IN_HOST_AMT is null then 0.00 else abs(f.UEQ_IN_HOST_AMT) end, null, null
        FROM
           (SELECT c.INTER_BRH_CODE, c.SETT_DATE, c.TRANS_TP, c.TOTAL_NUM, c.TOTAL_AMT, d.EQ_IN_HOST_NUM, d.EQ_IN_HOST_AMT
            FROM 
                (SELECT INTER_BRH_CODE, SETT_DATE, TRANS_TP, COUNT(*) AS TOTAL_NUM, SUM(TRANS_AT)/100 AS TOTAL_AMT 
                 FROM HT_TBL_GATEWAY_TXN a
                 WHERE SETT_DATE = (SELECT SYESTERDAY FROM HT_TBL_DATE_INF b where a.INTER_BRH_CODE = b.INTER_BRH_CODE)
                 GROUP BY INTER_BRH_CODE, SETT_DATE, TRANS_TP
                ) c
            LEFT JOIN 
                (SELECT INTER_BRH_CODE, TRANS_TP,COUNT(*) AS EQ_IN_HOST_NUM, SUM(TRANS_AT)/100 AS EQ_IN_HOST_AMT
                 FROM HT_TBL_SETT_FLOW a
                 WHERE SETT_DATE = (SELECT SYESTERDAY FROM HT_TBL_DATE_INF b where a.INTER_BRH_CODE = b.INTER_BRH_CODE) AND 
                      (a.INTER_BRH_CODE IN ('100003', '100004', '100005', '100006') OR
                          (a.INTER_BRH_CODE IN ('100001', '100002') AND 
                           a.TRANS_TP = '1201')
                      )
                      GROUP BY INTER_BRH_CODE, TRANS_TP
                ) d
            ON c.INTER_BRH_CODE = d.INTER_BRH_CODE AND 
               c.TRANS_TP = d.TRANS_TP
            ) e 
       LEFT JOIN 
          (SELECT INTER_BRH_CODE, TRANS_TP, COUNT(*) AS UEQ_IN_HOST_NUM, SUM(TRANS_AT)/100 AS UEQ_IN_HOST_AMT
           FROM HT_TBL_ERR_FLOW a
           WHERE SETT_DATE = (SELECT SYESTERDAY FROM HT_TBL_DATE_INF b where a.INTER_BRH_CODE = b.INTER_BRH_CODE) AND
                 (a.INTER_BRH_CODE IN ('100003', '100004', '100005', '100006') OR
                     (a.INTER_BRH_CODE IN ('100001', '100002') AND 
                      a.TRANS_TP = '1201')
                 ) AND
                 a.FLAG_RESULT !='C'
                 GROUP BY INTER_BRH_CODE, TRANS_TP
           ) f
       ON e.INTER_BRH_CODE = f.INTER_BRH_CODE AND 
          e.TRANS_TP = f.TRANS_TP
    UNION ALL
    SELECT  INTER_BRH_CODE, decode(substr(INTER_BRH_CODE, 0, 6), '100012', '直连汇总', '100045', '二清汇总') INTER_BRH_NAME,
            SETT_DATE, TRANS_TP, SUM(TOTAL_NUM) TOTAL_NUM,
            SUM(abs(TOTAL_AMT)) TOTAL_AMT, SUM(EQ_IN_HOST_NUM) EQ_IN_HOST_NUM, SUM(abs(EQ_IN_HOST_AMT)) EQ_IN_HOST_AMT,
            SUM(UEQ_IN_HOST_NUM) UEQ_IN_HOST_NUM, SUM(abs(UEQ_IN_HOST_AMT)) UEQ_IN_HOST_AMT, NULL, NULL
    FROM (
        SELECT decode(substr(INTER_BRH_CODE, 0, 6), '100001', '100012', '100002', '100012', '100004', '100045', '100005', '100045', '100006', '100045') INTER_BRH_CODE,
               decode(substr(INTER_BRH_CODE, 0, 6), '100001', '虚拟账户', '100002', '虚拟账户', '100004', '网银汇总', '100005', '网银汇总', '100006', '富友汇总') INTER_BRH_NAME,
               SETT_DATE, TRANS_TP, SUM(TOTAL_NUM) TOTAL_NUM,
               SUM(TOTAL_AMT) TOTAL_AMT, SUM(EQ_IN_HOST_NUM) EQ_IN_HOST_NUM, SUM(EQ_IN_HOST_AMT) EQ_IN_HOST_AMT,
               SUM(UEQ_IN_HOST_NUM) UEQ_IN_HOST_NUM, SUM(UEQ_IN_HOST_AMT) UEQ_IN_HOST_AMT, NULL, NULL
        FROM (
            SELECT e.INTER_BRH_CODE, 
                   decode(trim(e.INTER_BRH_CODE), '100001', '支付宝', '100002', '微信', '100003', '虚拟账户', '100004', '快捷', '100005', '网银', '100006', '富友') as INTER_BRH_NAME,
                   e.SETT_DATE, e.TRANS_TP, e.TOTAL_NUM, e.TOTAL_AMT, 
                   case when e.EQ_IN_HOST_NUM is null then 0 else e.EQ_IN_HOST_NUM end AS EQ_IN_HOST_NUM, 
                   case when e.EQ_IN_HOST_AMT is null then 0.00 else e.EQ_IN_HOST_AMT end AS EQ_IN_HOST_AMT, 
                   case when f.UEQ_IN_HOST_NUM is null then 0 else f.UEQ_IN_HOST_NUM end AS UEQ_IN_HOST_NUM, 
                   case when f.UEQ_IN_HOST_AMT is null then 0.00 else f.UEQ_IN_HOST_AMT end AS UEQ_IN_HOST_AMT, null, null
            FROM
            (SELECT c.INTER_BRH_CODE, c.SETT_DATE, c.TRANS_TP, c.TOTAL_NUM, c.TOTAL_AMT, d.EQ_IN_HOST_NUM, d.EQ_IN_HOST_AMT
             FROM 
                (SELECT INTER_BRH_CODE, SETT_DATE, TRANS_TP, COUNT(*) AS TOTAL_NUM, SUM(TRANS_AT)/100 AS TOTAL_AMT 
                 FROM HT_TBL_GATEWAY_TXN a
                 WHERE SETT_DATE = (SELECT SYESTERDAY FROM HT_TBL_DATE_INF b where a.INTER_BRH_CODE = b.INTER_BRH_CODE)
                 GROUP BY INTER_BRH_CODE, SETT_DATE, TRANS_TP
                ) c
             LEFT JOIN 
                (SELECT INTER_BRH_CODE, TRANS_TP,COUNT(*) AS EQ_IN_HOST_NUM, SUM(TRANS_AT)/100 AS EQ_IN_HOST_AMT
                 FROM HT_TBL_SETT_FLOW a
                 WHERE SETT_DATE = (SELECT SYESTERDAY FROM HT_TBL_DATE_INF b where a.INTER_BRH_CODE = b.INTER_BRH_CODE) AND 
                      (a.INTER_BRH_CODE IN ('100003', '100004', '100005', '100006') OR
                          (a.INTER_BRH_CODE IN ('100001', '100002') AND 
                           a.TRANS_TP = '1201')
                      )
                      GROUP BY INTER_BRH_CODE, TRANS_TP
                ) d
             ON c.INTER_BRH_CODE = d.INTER_BRH_CODE AND 
               c.TRANS_TP = d.TRANS_TP
            ) e 
          LEFT JOIN 
              (SELECT INTER_BRH_CODE, TRANS_TP, COUNT(*) AS UEQ_IN_HOST_NUM, SUM(TRANS_AT)/100 AS UEQ_IN_HOST_AMT
               FROM HT_TBL_ERR_FLOW a
               WHERE SETT_DATE = (SELECT SYESTERDAY FROM HT_TBL_DATE_INF b where a.INTER_BRH_CODE = b.INTER_BRH_CODE) AND
                     (a.INTER_BRH_CODE IN ('100003', '100004', '100005', '100006') OR
                     (a.INTER_BRH_CODE IN ('100001', '100002') AND 
                      a.TRANS_TP = '1201')
                 ) AND
                 a.FLAG_RESULT !='C'
                 GROUP BY INTER_BRH_CODE, TRANS_TP
           ) f
           ON e.INTER_BRH_CODE = f.INTER_BRH_CODE AND 
              e.TRANS_TP = f.TRANS_TP
        )
    WHERE (TRIM(INTER_BRH_CODE) IN ('100001', '100002') AND
            TRANS_TP = '1201') OR 
          TRIM(INTER_BRH_CODE) IN ('100004', '100005', '100006')     
          GROUP BY INTER_BRH_CODE,SETT_DATE, TRANS_TP)
    GROUP BY INTER_BRH_CODE,SETT_DATE,TRANS_TP);
    
    INSERT INTO TBL_OPERATE_ANALYSIS@PUBLIC_DATA (
    SELECT '100003' INTER_BRH_CODE, '充值汇总' INTER_BRH_NAME, 
            SETT_DATE, TRANS_TP, SUM(TOTAL_NUM) TOTAL_NUM,
            SUM(TOTAL_AMT) TOTAL_AMT, SUM(EQ_IN_HOST_NUM) EQ_IN_HOST_NUM, SUM(EQ_IN_HOST_AMT) EQ_IN_HOST_AMT,
            SUM(UEQ_IN_HOST_NUM) UEQ_IN_HOST_NUM, SUM(UEQ_IN_HOST_AMT) UEQ_IN_HOST_AMT, NULL, NULL
    FROM TBL_OPERATE_ANALYSIS@PUBLIC_DATA 
    WHERE TRANS_TP = '1201' AND
          TRIM(INTER_BRH_CODE) IN ('100012', '100045') AND
          SETT_DATE = (SELECT DISTINCT(SYESTERDAY) FROM HT_TBL_DATE_INF)
          GROUP BY SETT_DATE,TRANS_TP);     
          
    SELECT DISTINCT(SYESTERDAY) INTO SETT_DATE FROM HT_TBL_DATE_INF;
    SELECT nvl(sum(a.BAL), 0) INTO REAL_YES_BALANCE FROM HT_TBL_POSTION_INFO@PUBLIC_DATA a 
         WHERE a.SETT_DATE = (SELECT DISTINCT(STWODAYSAGO) FROM HT_TBL_DATE_INF);
    SELECT nvl(sum(a.BAL), 0) INTO REAL_TOD_BALANCE FROM HT_TBL_POSTION_INFO@PUBLIC_DATA a 
         WHERE a.SETT_DATE = (SELECT DISTINCT(SYESTERDAY) FROM HT_TBL_DATE_INF);
    
          
    SELECT nvl(SUM(a.TRANS_AT)/100, 0) INTO RECHARGE_INCOME 
    FROM HT_TBL_SETT_FLOW a
    WHERE a.SETT_DATE = (SELECT DISTINCT(SYESTERDAY) FROM HT_TBL_DATE_INF) AND
          a.TRANS_TP = '1201';
          
    SELECT nvl(SUM(a.TRANS_AT)/100, 0) INTO ERQING_INCOME 
    FROM HT_TBL_SETT_FLOW a
    WHERE a.SETT_DATE = (SELECT DISTINCT(SYESTERDAY) FROM HT_TBL_DATE_INF) AND
          a.INTER_BRH_CODE IN ('100004', '100005', '100006') AND
          a.TRANS_TP = '1001';
         
    SELECT nvl(SUM(a.SHARE_PROFIT_PLAT)/100, 0) INTO SERVICE_CHARGE_INCOME 
    FROM HT_TBL_SETT_FLOW a
    WHERE a.SETT_DATE = (SELECT DISTINCT(SYESTERDAY) FROM HT_TBL_DATE_INF) AND
          a.MCHT_ROLE1 = '2001' AND
          a.TRANS_TP != '1201';
          
    
    SELECT nvl(SUM(abs(a.AMT)), 0), nvl(SUM(abs(a.AMT_FEE)), 0) INTO MCHT_OUTCOME, FEE_OUTCOME 
    FROM HT_TBL_SETT_LIST a
    WHERE a.SETT_DATE = (SELECT DISTINCT(SYESTERDAY) FROM HT_TBL_DATE_INF) AND
          a.STATUS = '6' AND
          a.AMT > 0;
    
    SELECT nvl(SUM(abs(a.TRANS_AT2))/100, 0) INTO WITHD_OUTCOME 
    FROM HT_TBL_SETT_FLOW a
    WHERE a.SETT_DATE = (SELECT DISTINCT(SYESTERDAY) FROM HT_TBL_DATE_INF) AND
          a.INTER_BRH_CODE IN ('100004', '100005', '100006');
          
    SELECT nvl(SUM(a.SHARE_PROFIT_MCHT_NO2)/100, 0) + 
           nvl(SUM(a.SHARE_PROFIT_MCHT_NO3)/100, 0) + 
           nvl(SUM(a.SHARE_PROFIT_MCHT_NO4)/100, 0) +
           nvl(SUM(a.SHARE_PROFIT_MCHT_NO5)/100, 0) + 
           nvl(SUM(a.SHARE_PROFIT_MCHT_NO6)/100, 0) INTO COST_OUTCOME 
    FROM HT_TBL_SETT_FLOW a
    WHERE a.SETT_DATE = (SELECT DISTINCT(SYESTERDAY) FROM HT_TBL_DATE_INF) AND
          a.INTER_BRH_CODE IN ('100003', '100004', '100005', '100006');
          
    
    
    INSERT INTO TBL_HOST_CHECK_REPORT@PUBLIC_DATA VALUES(
        SETT_DATE, REAL_YES_BALANCE, REAL_TOD_BALANCE, REAL_TOD_BALANCE-REAL_YES_BALANCE,
        RECHARGE_INCOME+ERQING_INCOME+SERVICE_CHARGE_INCOME, MCHT_OUTCOME+FEE_OUTCOME+WITHD_OUTCOME, 
        RECHARGE_INCOME+ERQING_INCOME+SERVICE_CHARGE_INCOME - MCHT_OUTCOME-FEE_OUTCOME-WITHD_OUTCOME, NULL, NULL, NULL);    
         
  end LIQUIDATION_REPORT;
end P_LIQUIDATION_REPORT;
