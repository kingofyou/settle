CREATE OR REPLACE TRIGGER "TRI_KEY_CERT"
BEFORE INSERT OR UPDATE OR DELETE ON TP_MERCHANT_CHANNEL_MAPPING
FOR EACH ROW
BEGIN
  IF INSERTING THEN
    insert into TBL_KEY_CERT_INF
    (
       PAY_WAY, MERC_TYPE, MERC_NO, CHANNEL_CODE, PAYING_MERC_CODE, KEY, MERC_CERT, STATUS
    )
    values(
        ' ', ' ', :new.MERC_NO, :new.CHANNEL_CODE, :new.PAYING_MERC_CODE,:new.KEY,:new.PRIVATE_KEY_PATH,:new.STATUS
    );
    
    UPDATE TBL_KEY_CERT_INF a 
    SET a.MERC_TYPE=(SELECT DECODE(SUBSTR(b.CONNECT_SWITCH, 0, 4), 'c001', '1', 'c002', '2') 
                   FROM TP_PAY_CHANNEL b
                   WHERE a.CHANNEL_CODE=b.CHANNEL_CODE),
        a.PAY_WAY=(SELECT CASE WHEN SUBSTR(b.SERVICE_BEAN_ID, 0, 13)='cibPayService' AND SUBSTR(b.PAYWAY_CODE, 0, 6)='alipay' THEN 'cib_alipay'
                               WHEN SUBSTR(b.SERVICE_BEAN_ID, 0, 13)='cibPayService' AND SUBSTR(b.PAYWAY_CODE, 0, 5)='wxpay' THEN 'cib_tenpay' 
                               WHEN SUBSTR(b.SERVICE_BEAN_ID, 0, 13)='cibPayService' AND (SUBSTR(b.PAYWAY_CODE, 0, 8)='quickpay' OR 
                                                                                          SUBSTR(b.PAYWAY_CODE, 0, 8)='gateway') THEN 'cib_xydf'
                               WHEN SUBSTR(b.SERVICE_BEAN_ID, 0, 15)='fuiouPayService' AND SUBSTR(b.PAYWAY_CODE, 0, 6)='alipay' THEN 'fuiou' 
                               WHEN SUBSTR(b.SERVICE_BEAN_ID, 0, 15)='fuiouPayService' AND SUBSTR(b.PAYWAY_CODE, 0, 5)='wxpay' THEN 'fuiou'
                               WHEN SUBSTR(b.SERVICE_BEAN_ID, 0, 13)='ysePayService' AND SUBSTR(b.PAYWAY_CODE, 0, 5)='alipay' THEN 'yinsheng' 
                               WHEN SUBSTR(b.SERVICE_BEAN_ID, 0, 13)='ysePayService' AND SUBSTR(b.PAYWAY_CODE, 0, 5)='wxpay' THEN 'yinsheng'
                               else ' ' end
                   FROM TP_PAY_CHANNEL b
                   WHERE a.CHANNEL_CODE=b.CHANNEL_CODE);
    
  ELSIF UPDATING THEN
      update TBL_KEY_CERT_INF set
            PAYING_MERC_CODE=:new.PAYING_MERC_CODE,
            KEY =:new.KEY,
            MERC_CERT =:new.PRIVATE_KEY_PATH,
            STATUS = :new.STATUS
      where CHANNEL_CODE=:new.CHANNEL_CODE AND
            MERC_NO = :new.MERC_NO;
            
  ELSIF DELETING THEN
      delete from TBL_KEY_CERT_INF 
      where CHANNEL_CODE=:OLD.CHANNEL_CODE AND
            MERC_NO = :OLD.MERC_NO;
  END IF;
END;

/