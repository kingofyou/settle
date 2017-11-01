#!/bin/sh

#加载环境变量
cd ${HOME}
. .bash_profile

HtLog()
{
	DateStr=`date "+%Y-%m-%d %H:%M:%S"`
	if [ $# -eq 0 ]; then
		echo "[ ${DateStr} ] "
	else
		echo "[ ${DateStr} ] $1"
	fi
}

DT=`date "+%Y%m%d"`
destination="duokaronghe@nationz.com.cn,penghongbin@chinasofti.com"

sendMail()
{
    titleName=$1
    mailFile=$2

    MAILLIST=$3
    
    cat ${mailFile} | mail -s "${titleName}" $MAILLIST
    if [ $? -ne 0  ]; then
	    HtLog "mail send error"
	    exit 151
    else
 	    HtLog "mail send succes"
 	    exit 0
    fi    
}

sendMail $1 $2 $3