#!/bin/ksh
. $HOME/.bash_profile

if [ $# != 4 -a $# != 5 ]; then
    echo "Usage1:"
    echo "  `basename $0` \"FilePath\" \"Delimiter\" \"TableName\" <A/R> "
    echo "Usage2:"
    echo "  `basename $0` \"FilePath\" \"Delimiter\" \"TableName\" \"column1 column2……\" <A/R> "
    exit 1
fi

TBL_NAME=${3}
DELIMITER=${2}
FILE=${1}

if [ $# = 4 ];then

    if [ "${4}" = "A" -o "${4}" = "a" ];then
        LDWAY=APPEND
    else
        if [ "${4}" = "R" -o "${4}" = "r" ];then
            LDWAY=REPLACE
        else
            echo "PARAM 4 MUST BE <A/R>"
            exit 1
        fi
    fi

fi

if [ $# = 5 ];then

    if [ "${5}" = "A" -o "${5}" = "a" ];then
        LDWAY=APPEND
    else
        if [ "${5}" = "R" -o "${5}" = "r" ];then
            LDWAY=REPLACE
        else
            echo "PARAM 5 MUST BE <A/R>"
            exit 1
        fi
    fi
    COLUMNS=${4}
fi

echo "whenever sqlerror exit sql.sqlcode;
set term off;
set echo off;
set timing off;
set heading off;
set feedback off;
set pagesize 0;
set linesize 5000;
set trimspool on;
set trimout on;
set colsep |;
spool ${TBL_NAME}.ctl1;
desc ${TBL_NAME};
spool off;
exit
"|sqlplus $DBUSER/$DBPWD@$DBNAME 1>/dev/null 2>/dev/null

ed ${TBL_NAME}.ctl1 <<!EOF 1>/dev/null 2>/dev/null
1,2d
w
q
!EOF

LINE=" "
REC=" "
if [ $# = 4 ];then
cat ${TBL_NAME}.ctl1 | while read REC
do
    LINE1=`echo $REC|cut  -d' ' -f 1`
    LINE2=`echo $REC|cut  -d' ' -f 2`
    if [ "$LINE2" == "NOT" ]; then
       LINE2=`echo $REC|cut  -d' ' -f 4`
    fi

    LINE="$LINE1"

    TMP=`echo $LINE2|cut -c1-8`
    if [ "$TMP" == "VARCHAR2" ]; then
        TMP1=`echo $LINE2|cut -c9-15`
        LINE="$LINE1  CHAR${TMP1}"
    fi

    TMP=`echo $LINE2|cut -c1-4`
    if [ "$TMP" == "CHAR" ]; then
        LINE="$LINE1    $LINE2"
    fi
    
    TMP=`echo $LINE2|cut -c1-4`
    if [ "$TMP" == "DATE" ]; then
        LINE="$LINE1    $LINE2 \"yyyy-mm-dd HH24:MI:SS\""
    fi    

    echo $LINE >> ${TBL_NAME}.ctl1.tmp1
done
fi

if [ $# = 5 ];then
for COLUMN in $COLUMNS
do
    REC=`cat ${TBL_NAME}.ctl1|grep -i -w $COLUMN`
    LINE1=`echo $REC|cut  -d' ' -f 1`
    LINE2=`echo $REC|cut  -d' ' -f 2`
    if [ "$LINE2" == "NOT" ]; then
       LINE2=`echo $REC|cut  -d' ' -f 4`
    fi

    LINE="$LINE1"

    TMP=`echo $LINE2|cut -c1-8`
    if [ "$TMP" == "VARCHAR2" ]; then
        TMP1=`echo $LINE2|cut -c9-15`
        LINE="$LINE1  CHAR${TMP1}"
    fi

    TMP=`echo $LINE2|cut -c1-4`
    if [ "$TMP" == "CHAR" ]; then
        LINE="$LINE1    $LINE2"
    fi
    
    TMP=`echo $LINE2|cut -c1-4`
    if [ "$TMP" == "DATE" ]; then
        LINE="$LINE1    $LINE2 \"yyyy-mm-dd HH24:MI:SS\""
    fi    

    echo $LINE >> ${TBL_NAME}.ctl1.tmp1
done
echo >> ${TBL_NAME}.ctl1.tmp1    
fi

mv -f ${TBL_NAME}.ctl1.tmp1 ${TBL_NAME}.ctl1
echo "LOAD DATA" > ${TBL_NAME}.ctl
echo "INFILE '${FILE}'" >> ${TBL_NAME}.ctl
echo "${LDWAY}" >> ${TBL_NAME}.ctl
echo "INTO TABLE ${TBL_NAME}" >> ${TBL_NAME}.ctl
echo "FIELDS TERMINATED BY \"${DELIMITER}\"" >> ${TBL_NAME}.ctl
echo "(" >> ${TBL_NAME}.ctl
TMP=""
TMP1=""
cat ${TBL_NAME}.ctl1 | while read TMP
do
    if [ -z "$TMP" ]
    then
        echo $TMP1>>${TBL_NAME}.ctl
        continue
    fi

    if [ -n "$TMP1" ]
    then
        echo $TMP1,>>${TBL_NAME}.ctl
    fi
    TMP1=$TMP
done

echo ")" >> ${TBL_NAME}.ctl
rm -f ${TBL_NAME}.ctl1

sqlldrci errors=999999999 control=${TBL_NAME}.ctl log=${FILE}.log bad=${FILE}.bad
if [ $? -ne 0 ];then
    rm -f ${TBL_NAME}.ctl
    exit 1
fi

rm -f ${TBL_NAME}.ctl

