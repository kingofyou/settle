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

if [ $# -ge 1 ]; then
	LastDay=$1
else
	#取前一天的日期
	LastDay=`date -d "1 day ago" +"%Y%m%d"`
fi

#备份文件目录
fileBackupPath=${HOME}/backup/file
LogBackupPath=${HOME}/backup/log

mkdir -p ${fileBackupPath}
mkdir -p ${LogBackupPath}

#文件、日志目录
filePath=${BATCH_FILE_PATH}
BatLogPath=${B_LOG_FILE_PATH}

#检查文件备份目录是否存在
if [ ! -e ${fileBackupPath} -o ! -d ${fileBackupPath} ] ; then
	HtLog "文件备份目录[${fileBackupPath}]不存在"
exit 1
fi

#打包文件
cd ${filePath}
if [ $? -ne 0 ];then 
	HtLog "进入[${filePath}]目录失败"
	exit 1
else
	HtLog "进入[${filePath}]目录成功"
fi

if [ ! -e ${LastDay} -o ! -d ${LastDay} ] ; then
	HtLog "文件目录[${LastDay}]不存在"
exit 1
fi

tar -cf file_${LastDay}.tar ./${LastDay}
if [ $? -ne 0 ];then  
	HtLog "打包文件失败"
	exit 1
else
	HtLog "打包文件成功 生成文件[file_${LastDay}.tar]"
fi

#压缩文件
gzip file_${LastDay}.tar
if [ $? -ne 0 ];then  
	HtLog "压缩文件失败"
	exit 1
else
	HtLog "压缩文件成功 生成文件[file_${LastDay}.tar.gz]"
fi

#将压缩好的文件文件移到备份文件目录
mv file_${LastDay}.tar.gz ${fileBackupPath}
if [ $? -ne 0 ];then  
	HtLog "移动备份文件失败"
	exit 1
else
	HtLog "已将备份文件成功移到目录[${fileBackupPath}]"
fi

#删除文件
rm -rf ./${LastDay}
if [ $? -ne 0 ];then  
	HtLog "删除文件失败"
	exit 1
else
	HtLog "已将文件[${filePath}/${LastDay}]成功删除"
fi


#打包批量日志
cd ${BatLogPath}
if [ $? -ne 0 ];then 
        HtLog "进入[${BatLogPath}]目录失败"
        exit 1
else
        HtLog "进入[${BatLogPath}]目录成功"
fi

if [ ! -e ${LastDay} -o ! -d ${LastDay} ] ; then
        HtLog "日志目录[${LastDay}]不存在"
exit 1
fi

tar -cf bat_logfile_${LastDay}.tar ./${LastDay}
if [ $? -ne 0 ];then  
        HtLog "打包日志失败"
        exit 1
else
        HtLog "打包日志成功 生成文件[bat_logfile_${LastDay}.tar]"
fi

#压缩日志
gzip bat_logfile_${LastDay}.tar
if [ $? -ne 0 ];then  
        HtLog "压缩日志失败"
        exit 1
else
        HtLog "压缩日志成功 生成文件[bat_logfile_${LastDay}.tar.gz]"
fi

#将压缩好的日志文件移到备份日志目录
mv bat_logfile_${LastDay}.tar.gz ${LogBackupPath}
if [ $? -ne 0 ];then  
        HtLog "移动备份日志失败"
        exit 1
else
        HtLog "已将备份日志成功移到目录[${LogBackupPath}]"
fi

#删除日志
rm -rf ./${LastDay}
if [ $? -ne 0 ];then  
        HtLog "删除日志失败"
        exit 1
else
        HtLog "已将日志[${BatLogPath}/${LastDay}]成功删除"
fi