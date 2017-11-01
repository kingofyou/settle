
if [ $# -gt 0 ]
then
    DATE=$1
else
    DATE=`date +%Y%m%d`
    LastDay=`date -d "90 day ago" +"%Y%m%d"`
fi
echo "参数日期: ${DATE}"

mkdir -p $HOME/backup/data/${DATE}
#按月清理
exp $DBUSER/$DBPWD@$DBNAME tables=HT_TBL_SETT_FLOW file=$HOME/backup/data/${DATE}/HT_TBL_SETT_FLOW_${DATE}.data grants=y
exp $DBUSER/$DBPWD@$DBNAME tables=HT_TBL_ERR_FLOW file=$HOME/backup/data/${DATE}/HT_TBL_ERR_FLOW_${DATE}.data grants=y
exp $DBUSER/$DBPWD@$DBNAME tables=HT_TBL_SETT_LIST file=$HOME/backup/data/${DATE}/HT_TBL_SETT_LIST_${DATE}.data grants=y
exp $DBUSER/$DBPWD@$DBNAME tables=HT_TBL_GATEWAY_TXN file=$HOME/backup/data/${DATE}/HT_TBL_GATEWAY_TXN_${DATE}.data grants=y



echo "start delete database................."
sqlplus $DBUSER/$DBPWD@$DBNAME << EOF
delete from HT_TBL_SETT_FLOW where sett_date < ${DATE} and sett_date > ${LastDay};
delete from HT_TBL_ERR_FLOW where sett_date < ${DATE} and sett_date > ${LastDay};
delete from HT_TBL_SETT_LIST where sett_date < ${DATE} and sett_date > ${LastDay};
delete from HT_TBL_GATEWAY_TXN where sett_date < ${DATE} and sett_date > ${LastDay};
commit;
EOF
echo "finish delete database................."
