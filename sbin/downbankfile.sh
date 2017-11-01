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
DATE_DT=$1
FILENAME=$2
FILETYPE=$3
LOCALPATH1=${SHARE_FILE_PATH}"/${DATE_DT}/100004/CIB"
LOCALPATH2=${SHARE_FILE_PATH}"/${DATE_DT}/100005/CIB"
LOCALPATH3=${SHARE_FILE_PATH}"/${DATE_DT}"
DESTPATH="/shareDir/cib"
mkdir -p ${LOCALPATH1}
mkdir -p ${LOCALPATH2}
mkdir -p ${DESTPATH}/"${DATE_DT}_${FILETYPE}"
chmod 777 ${LOCALPATH3}
chmod 777 ${DESTPATH}/"${DATE_DT}_${FILETYPE}"
cd $DESTPATH

unzip $FILENAME -d "${DATE_DT}_${FILETYPE}"
if [ $# -eq 0 ]; then
    HtLog "unzip $FILENAME error"
    exit 151
else
    HtLog "unzip $FILENAME succ"
fi

if [ $FILETYPE -eq "0" ] || [ $FILETYPE -eq "1" ]; then
    mv -f $DESTPATH/"${DATE_DT}_${FILETYPE}"/* $LOCALPATH1/
else
    mv -f $DESTPATH/"${DATE_DT}_${FILETYPE}"/* $LOCALPATH2/
fi

