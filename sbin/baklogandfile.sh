#!/bin/sh

#���ػ�������
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
	#ȡǰһ�������
	LastDay=`date -d "1 day ago" +"%Y%m%d"`
fi

#�����ļ�Ŀ¼
fileBackupPath=${HOME}/backup/file
LogBackupPath=${HOME}/backup/log

mkdir -p ${fileBackupPath}
mkdir -p ${LogBackupPath}

#�ļ�����־Ŀ¼
filePath=${BATCH_FILE_PATH}
BatLogPath=${B_LOG_FILE_PATH}

#����ļ�����Ŀ¼�Ƿ����
if [ ! -e ${fileBackupPath} -o ! -d ${fileBackupPath} ] ; then
	HtLog "�ļ�����Ŀ¼[${fileBackupPath}]������"
exit 1
fi

#����ļ�
cd ${filePath}
if [ $? -ne 0 ];then 
	HtLog "����[${filePath}]Ŀ¼ʧ��"
	exit 1
else
	HtLog "����[${filePath}]Ŀ¼�ɹ�"
fi

if [ ! -e ${LastDay} -o ! -d ${LastDay} ] ; then
	HtLog "�ļ�Ŀ¼[${LastDay}]������"
exit 1
fi

tar -cf file_${LastDay}.tar ./${LastDay}
if [ $? -ne 0 ];then  
	HtLog "����ļ�ʧ��"
	exit 1
else
	HtLog "����ļ��ɹ� �����ļ�[file_${LastDay}.tar]"
fi

#ѹ���ļ�
gzip file_${LastDay}.tar
if [ $? -ne 0 ];then  
	HtLog "ѹ���ļ�ʧ��"
	exit 1
else
	HtLog "ѹ���ļ��ɹ� �����ļ�[file_${LastDay}.tar.gz]"
fi

#��ѹ���õ��ļ��ļ��Ƶ������ļ�Ŀ¼
mv file_${LastDay}.tar.gz ${fileBackupPath}
if [ $? -ne 0 ];then  
	HtLog "�ƶ������ļ�ʧ��"
	exit 1
else
	HtLog "�ѽ������ļ��ɹ��Ƶ�Ŀ¼[${fileBackupPath}]"
fi

#ɾ���ļ�
rm -rf ./${LastDay}
if [ $? -ne 0 ];then  
	HtLog "ɾ���ļ�ʧ��"
	exit 1
else
	HtLog "�ѽ��ļ�[${filePath}/${LastDay}]�ɹ�ɾ��"
fi


#���������־
cd ${BatLogPath}
if [ $? -ne 0 ];then 
        HtLog "����[${BatLogPath}]Ŀ¼ʧ��"
        exit 1
else
        HtLog "����[${BatLogPath}]Ŀ¼�ɹ�"
fi

if [ ! -e ${LastDay} -o ! -d ${LastDay} ] ; then
        HtLog "��־Ŀ¼[${LastDay}]������"
exit 1
fi

tar -cf bat_logfile_${LastDay}.tar ./${LastDay}
if [ $? -ne 0 ];then  
        HtLog "�����־ʧ��"
        exit 1
else
        HtLog "�����־�ɹ� �����ļ�[bat_logfile_${LastDay}.tar]"
fi

#ѹ����־
gzip bat_logfile_${LastDay}.tar
if [ $? -ne 0 ];then  
        HtLog "ѹ����־ʧ��"
        exit 1
else
        HtLog "ѹ����־�ɹ� �����ļ�[bat_logfile_${LastDay}.tar.gz]"
fi

#��ѹ���õ���־�ļ��Ƶ�������־Ŀ¼
mv bat_logfile_${LastDay}.tar.gz ${LogBackupPath}
if [ $? -ne 0 ];then  
        HtLog "�ƶ�������־ʧ��"
        exit 1
else
        HtLog "�ѽ�������־�ɹ��Ƶ�Ŀ¼[${LogBackupPath}]"
fi

#ɾ����־
rm -rf ./${LastDay}
if [ $? -ne 0 ];then  
        HtLog "ɾ����־ʧ��"
        exit 1
else
        HtLog "�ѽ���־[${BatLogPath}/${LastDay}]�ɹ�ɾ��"
fi