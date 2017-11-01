#!/bin/ksh
. $HOME/.bash_profile
if [ $# != 3 ]; then
    echo "Usage:" 
    echo "`basename $0` \"FilePath\" \"Delimiter\" \"SQL\""
    exit 1
fi

#构造sql
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
set colsep ${2};
spool ${1}.$$;
${3};
spool off;
exit
"|sqlplus $DBUSER/$DBPWD@$DBNAME 1>/dev/null 2>/dev/null

#出错处理
[ $? -ne 0 ]&&exit 1

if [ -e "${1}.$$" ];then
sed "s/ *${2}/${2}/g;s/ *$/${2}/g" ${1}.$$ > ${1}
[ $? -ne 0 ]&&exit 1
rm -f ${1}.$$
fi
