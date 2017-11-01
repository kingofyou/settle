#!/bin/ksh
if [ $# -ne 10 ]
then
        echo "usage:loadtable tablename." >>$LOG_FILE_PATH/baktable.${TABLENAME}.log
        exit 1
fi
DB=$1
DBHIS=$2
TABLENAME=$3
TABLENAMEHIS=$4
FIELD=$5
VALUE=$6
COUNT=$7
FLAG=$8
FIELD2=$9
VALUE2=${10}

echo "=============== 备份开始！ ==================" >>$LOG_FILE_PATH/baktable.${TABLENAME}.log

echo "$DB" >$LOG_FILE_PATH/baktable.${TABLENAME}.log
echo "$DBHIS" >>$LOG_FILE_PATH/baktable.${TABLENAME}.log
echo "$TABLENAME" >>$LOG_FILE_PATH/baktable.${TABLENAME}.log
echo "$TABLENAMEHIS" >>$LOG_FILE_PATH/baktable.${TABLENAME}.log
echo "$FIELD" >>$LOG_FILE_PATH/baktable.${TABLENAME}.log
echo "$VALUE" >>$LOG_FILE_PATH/baktable.${TABLENAME}.log
echo "$COUNT" >>$LOG_FILE_PATH/baktable.${TABLENAME}.log
echo "$FLAG" >>$LOG_FILE_PATH/baktable.${TABLENAME}.log
echo "$FIELD2" >>$LOG_FILE_PATH/baktable.${TABLENAME}.log
echo "$VALUE2" >>$LOG_FILE_PATH/baktable.${TABLENAME}.log

if [ $FLAG == "0" ]
then
echo "delete from ${TABLENAMEHIS} where substr(${FIELD},0,8)=${VALUE};" >>$LOG_FILE_PATH/baktable.${TABLENAME}.log
sqlplus -s ${DBHIS}<<!
delete from ${TABLENAMEHIS} where substr(${FIELD},0,8)=${VALUE};
commit;
!

echo "copy from ${DB} to ${DBHIS} insert ${TABLENAMEHIS} using select * from ${TABLENAME} where substr(${FIELD},0,8)=${VALUE};" >>$LOG_FILE_PATH/baktable.${TABLENAME}.log
sqlplus -s ${DB}<<!
copy from ${DB} to ${DBHIS} insert ${TABLENAMEHIS} using select * from ${TABLENAME} where substr(${FIELD},0,8)=${VALUE};
commit;
!

echo "select count(*) from  ${TABLENAMEHIS} where substr(${FIELD},0,8)=${VALUE};" >>$LOG_FILE_PATH/baktable.${TABLENAME}.log
sqlplus -s ${DBHIS}<<!>$LOG_FILE_PATH/baksql.${TABLENAME}.log
select count(*) from  ${TABLENAMEHIS} where substr(${FIELD},0,8)=${VALUE};
!
else
echo "delete from ${TABLENAMEHIS} where substr(${FIELD},0,8)=${VALUE} and ${FIELD2}=${VALUE2};" >>$LOG_FILE_PATH/baktable.${TABLENAME}.log
sqlplus -s ${DBHIS}<<!
delete from ${TABLENAMEHIS} where substr(${FIELD},0,8)=${VALUE} and ${FIELD2}=${VALUE2};
commit;
!

echo "copy from ${DB} to ${DBHIS} insert ${TABLENAMEHIS} using select * from ${TABLENAME} where substr(${FIELD},0,8)=${VALUE} and ${FIELD2}=${VALUE2};" >>$LOG_FILE_PATH/baktable.${TABLENAME}.log
sqlplus -s ${DB}<<!
copy from ${DB} to ${DBHIS} insert ${TABLENAMEHIS} using select * from ${TABLENAME} where substr(${FIELD},0,8)=${VALUE} and ${FIELD2}=${VALUE2};
commit;
!

echo "select count(*) from  ${TABLENAMEHIS} where substr(${FIELD},0,8)=${VALUE} and ${FIELD2}=${VALUE2};" >>$LOG_FILE_PATH/baktable.${TABLENAME}.log
sqlplus -s ${DBHIS}<<!>$LOG_FILE_PATH/baksql.${TABLENAME}.log
select count(*) from  ${TABLENAMEHIS} where substr(${FIELD},0,8)=${VALUE} and ${FIELD2}=${VALUE2};
!
fi


grep -w "$COUNT" $LOG_FILE_PATH/baksql.${TABLENAME}.log
reval=$?
echo "grep return value: $reval" >>$LOG_FILE_PATH/baktable.${TABLENAME}.log
##rm -rf $LOGDIR/baksql.${TABLENAME}.log 
if reval=0
then
    echo "=============== 备份成功！ ==================" >>$LOG_FILE_PATH/baktable.${TABLENAME}.log
    exit 0
else
    echo "=============== 备份失败!" =============== >>$LOG_FILE_PATH/baktable.${TABLENAME}.log
    exit 1
fi

echo "程序不应该执行到这里!" >>$LOG_FILE_PATH/baktable.${TABLENAME}.log
