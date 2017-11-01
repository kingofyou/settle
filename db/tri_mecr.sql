CREATE OR REPLACE TRIGGER TRI_MERCHANT
BEFORE INSERT OR UPDATE OR DELETE ON TP_MERCHANT_INFO
FOR EACH ROW
BEGIN
  IF INSERTING THEN
    insert into HT_TBL_MCHT_INF
    (
       mcht_no,mcht_nm,contact,comm_mobil,comm_tel,manager,artif_certif_tp,identity_no,
       manager_tel,reg_addr,oper_no,oper_nm,mcht_status,clear_flag,sett_md,mcht_sett_bank,
       mcht_sett_bank_nm,mcht_sett_acct_type,mcht_sett_acct,mcht_sett_acct_nm,xingmin_pay_acct,
       businuss_brh_no,mcht_debit_bank,mcht_debit_bank_nm ,mcht_debit_acct_type,mcht_debit_acct,
       mcht_debit_acct_nm,mcht_role,js_type,clear_cycle,splitting_type,spl_brh_role1,spl_brh_nm1,
       spl_acct_type1,spl_acct_no1,spl_acct_nm1,spl_acct_bank1,spl_acct_banknm1,spl_acct_rate1,
       spl_brh_role2,spl_brh_nm2,spl_acct_type2,spl_acct_no2,spl_acct_nm2,spl_acct_bank2,
       spl_acct_banknm2,spl_acct_rate2,spl_brh_role3,spl_brh_nm3,spl_acct_type3,spl_acct_no3,
       spl_acct_nm3,spl_acct_bank3,spl_acct_banknm3,spl_acct_rate3,spl_brh_role4,spl_brh_nm4,
       spl_acct_type4,spl_acct_no4,spl_acct_nm4,spl_acct_bank4,spl_acct_banknm4,spl_acct_rate4,
       crt_opr_id ,rec_upd_ts,upd_opr_id,up_mcht_no,send_bill_flag,email,ret_fee_flag, recharge_flag
    )
    values(
        :new.MERC_NO,:new.MERC_NAME,:new.LINKMAN,:new.TELPHONE,
        :new.TELPHONE,:new.CORP_REPT_NAME,null,:new.CORP_REPT_ID,
        :new.FIXED_PHONE,:new.CORP_ADDR,null,null,:new.STATUS,
        '1',:new.SETTLE_TYPE,:new.MCHT_SETT_BANK,'????',
        :new.MCHT_SETT_ACCT_TYPE,:new.MCHT_SETT_ACCT,:new.MCHT_SETT_ACCT_NM,
        :new.xingmin_pay_acct,:new.BUSINUSS_BRH_NO,:new.MCHT_PAY_BANK,'????',:new.MCHT_PAY_ACCT_TYPE,
        :new.MCHT_PAY_ACCT,:new.MCHT_PAY_ACCT_NM,null,:new.JS_TYPE,
        :new.CLEAR_CYCLE,:new.SPLITTING_TYPE,:new.SPL_BRH_ROLE1,
        :new.SPL_BRH_NM1,:new.spl_acct_type1,:new.spl_acct_no1,:new.spl_acct_nm1,
        :new.spl_acct_bank1,:new.spl_acct_banknm1,:new.spl_acct_rate1,
        :new.SPL_BRH_ROLE2,:new.SPL_BRH_NM2,:new.spl_acct_type2,
        :new.spl_acct_no2,:new.spl_acct_nm2,:new.spl_acct_bank2,
        :new.spl_acct_banknm2,:new.spl_acct_rate2,:new.SPL_BRH_ROLE3,
        :new.SPL_BRH_NM3,:new.spl_acct_type3,:new.spl_acct_no3,
        :new.spl_acct_nm3,:new.spl_acct_bank3,:new.spl_acct_banknm3,
        :new.spl_acct_rate3,:new.SPL_BRH_ROLE4,:new.SPL_BRH_NM4,
        :new.spl_acct_type4,:new.spl_acct_no4,:new.spl_acct_nm4,
        :new.spl_acct_bank4,:new.spl_acct_banknm4,:new.spl_acct_rate4,
        to_char(:new.CREATE_TIME,'yyyyMMdd'),to_char(:new.UPDATE_TIME,'yyyyMMdd'),
        :new.CREATER,:new.UP_MERC_NO,:new.send_bill_flag,:new.email,:new.ret_fee_flag, 'n'
    );
  ELSIF UPDATING THEN
    update HT_TBL_MCHT_INF set
            mcht_nm =:new.MERC_NAME,contact =:new.LINKMAN,
            comm_mobil =:new.TELPHONE,comm_tel  =:new.TELPHONE,
            manager =:new.CORP_REPT_NAME,artif_certif_tp =null,
            identity_no=:new.CORP_REPT_ID,manager_tel=:new.FIXED_PHONE,
            reg_addr=:new.CORP_ADDR,oper_no=null,oper_nm=null,
            mcht_status=:new.STATUS,clear_flag='1',sett_md=:new.SETTLE_TYPE,
            mcht_sett_bank =:new.MCHT_SETT_BANK,mcht_sett_bank_nm ='????',
            mcht_sett_acct_type =:new.MCHT_SETT_ACCT_TYPE,
            mcht_sett_acct=:new.MCHT_SETT_ACCT,mcht_sett_acct_nm  =:new.MCHT_SETT_ACCT_NM,
            xingmin_pay_acct=:new.xingmin_pay_acct,businuss_brh_no=:new.BUSINUSS_BRH_NO,mcht_debit_bank=:new.MCHT_PAY_BANK,
            mcht_debit_bank_nm='????',mcht_debit_acct_type =:new.MCHT_PAY_ACCT_TYPE,
            mcht_debit_acct=:new.MCHT_PAY_ACCT,mcht_debit_acct_nm  =:new.MCHT_PAY_ACCT_NM,
            mcht_role=null,js_type=:new.JS_TYPE,clear_cycle=:new.clear_cycle ,
            splitting_type=:new.splitting_type,SPL_BRH_ROLE1=:new.SPL_BRH_ROLE1,
            SPL_BRH_NM1=:new.SPL_BRH_NM1,spl_acct_type1=:new.spl_acct_type1  ,
            spl_acct_no1  =:new.spl_acct_no1  ,spl_acct_nm1 =:new.spl_acct_nm1  ,
            spl_acct_bank1 =:new.spl_acct_bank1  ,spl_acct_banknm1=:new.spl_acct_banknm1  ,
            spl_acct_rate1     =:new.spl_acct_rate1  ,SPL_BRH_ROLE2=:new.SPL_BRH_ROLE2,
            SPL_BRH_NM2=:new.SPL_BRH_NM2,spl_acct_type2    =:new.spl_acct_type2   ,
            spl_acct_no2       =:new.spl_acct_no2  ,spl_acct_nm2      =:new.spl_acct_nm2   ,
            spl_acct_bank2    =:new.spl_acct_bank2   ,spl_acct_banknm2  =:new.spl_acct_banknm2   ,
            spl_acct_rate2     =:new.spl_acct_rate2  ,SPL_BRH_ROLE3=:new.SPL_BRH_ROLE3,
            SPL_BRH_NM3=:new.SPL_BRH_NM3,spl_acct_type3    =:new.spl_acct_type3   ,
            spl_acct_no3      =:new.spl_acct_no3   ,spl_acct_nm3      =:new.spl_acct_nm3   ,
            spl_acct_bank3    =:new.spl_acct_bank3   ,spl_acct_banknm3   =:new.spl_acct_banknm3  ,
            spl_acct_rate3     =:new.spl_acct_rate3  ,SPL_BRH_ROLE4=:new.SPL_BRH_ROLE4,
            SPL_BRH_NM4=:new.SPL_BRH_NM4,spl_acct_type4    =:new.spl_acct_type4   ,
            spl_acct_no4      =:new.spl_acct_no4   ,spl_acct_nm4      =:new.spl_acct_nm4,
            spl_acct_bank4     =:new.spl_acct_bank4,spl_acct_banknm4  =:new.spl_acct_banknm4,
            spl_acct_rate4    =:new.spl_acct_rate4,crt_opr_id =to_char(:new.CREATE_TIME,'yyyyMMdd'),
            rec_upd_ts  =to_char(:new.UPDATE_TIME,'yyyyMMdd'),upd_opr_id   =:new.CREATER,
            UP_MCHT_NO=:new.UP_MERC_NO,
            send_bill_flag=:new.send_bill_flag,
            email=:new.email,
            ret_fee_flag=:new.ret_fee_flag
  where MCHT_NO=:new.MERC_NO;
  
  ELSIF DELETING THEN
      DELETE FROM HT_TBL_MCHT_INF
      WHERE MCHT_NO=:OLD.MERC_NO;
  END IF;
END;
/