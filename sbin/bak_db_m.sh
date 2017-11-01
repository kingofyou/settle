
if [ $# -gt 0 ]
then
    DATE=$1
else
    DATE=`date +%Y%m%d`
    LastDay=`date -d "30 day ago" +"%Y%m%d"`
fi
echo "参数日期: ${DATE}"

mkdir -p $HOME/backup/data/${DATE}
#按月清理
exp $DBUSER/$DBPWD@$DBNAME tables=HT_TBL_HOST_TXN file=$HOME/backup/data/${DATE}/HT_TBL_HOST_TXN_${DATE}.data grants=y
exp $DBUSER/$DBPWD@$DBNAME tables=HT_TBL_CIB_TXN file=$HOME/backup/data/${DATE}/HT_TBL_CIB_TXN_${DATE}.data grants=y
exp $DBUSER/$DBPWD@$DBNAME tables=HT_TBL_WIFT_TXN file=$HOME/backup/data/${DATE}/HT_TBL_WIFT_TXN_${DATE}.data grants=y
exp $DBUSER/$DBPWD@$DBNAME tables=HT_TBL_SETT_RSLT file=$HOME/backup/data/${DATE}/HT_TBL_SETT_RSLT_${DATE}.data grants=y
exp $DBUSER/$DBPWD@$DBNAME tables=HT_TBL_CMP_FLOW file=$HOME/backup/data/${DATE}/HT_TBL_CMP_FLOW_${DATE}.data grants=y
exp $DBUSER/$DBPWD@$DBNAME tables=HT_TBL_ACCT_RSLT file=$HOME/backup/data/${DATE}/HT_TBL_ACCT_RSLT_${DATE}.data grants=y



echo "start delete database................."
sqlplus $DBUSER/$DBPWD@$DBNAME << EOF
delete from HT_TBL_HOST_TXN where sett_date < ${DATE} and sett_date > ${LastDay};
delete from HT_TBL_CIB_TXN where sett_date < ${DATE} and sett_date > ${LastDay};
delete from HT_TBL_WIFT_TXN where sett_date < ${DATE} and sett_date > ${LastDay};
delete from HT_TBL_WIFT_TXN where sett_date < ${DATE} and sett_date > ${LastDay};
delete from HT_TBL_SETT_RSLT where sett_date < ${DATE} and sett_date > ${LastDay};
delete from HT_TBL_CMP_FLOW where sett_date < ${DATE} and sett_date > ${LastDay};
delete from HT_TBL_ACCT_RSLT where sett_date < ${DATE} and sett_date > ${LastDay};
commit;
EOF
echo "finish delete database................."
