#!/usr/bin/ksh
#BATCH监控

. $HOME/.bash_profile

#获取当前日期时间
DAY=`date +"%Y%m%d"`
TIME=`date +"%H%M%S"`
SYS_INFO_FILE=$HOME/batch/log/${DAY}/sysinfo.txt

echo "当前时间：$DAY$TIME\n" > $SYS_INFO_FILE

#获取CPU使用率
#cpustr=`vmstat 1 1| sed -n '$p'`
cpustr=`vmstat | sed -n '$p'`
cpuidle=`echo $cpustr | cut -d ' ' -f 16`
CPURATE=`expr 100 - $cpuidle`
echo "CPU使用率：$CPURATE\n" >> $SYS_INFO_FILE

#获取磁盘空间使用率
diskstr=`df -k $HOME | sed -n '$p'`
DISKRATE=`echo $diskstr | cut -d' ' -f4`
DISKRATE=`echo $DISKRATE | cut -d "%" -f 1`
echo "磁盘空间使用率：$DISKRATE\n" >> $SYS_INFO_FILE

#获取内存使用率
#memstr=`lsps -s | sed -n '$p'`
#MEMRATE=`echo $memstr | cut -d ' ' -f 2`
#MEMRATE=`echo $MEMRATE | cut -d "%" -f 1`
#echo "内存使用率：$MEMRATE\n" >> $SYS_INFO_FILE

#uname=`uname`
tmpfile="$HOME/batch/log/${DAY}/dbspace.tmp"
outfile="$SYS_INFO_FILE"

getfs()
{

sqlplus $DBUSER/$DBPWD@$DBNAME <<EOF > /dev/null
    set term off;
    set echo off;
    set timing off;
    set heading off;
    set feedback off;
    set pagesize 0;
    set linesize 1000;
    set trimspool on;
    set trimout on;
	set colsep | ;
    spool $tmpfile.lst;
    select '|',upper(f.tablespace_name) "表空间名",
		  d.tot_grootte_mb "表空间大小(M)",
		  d.tot_grootte_mb - f.total_bytes "已使用空间(M)",
		  f.total_bytes "空闲空间(M)",
		  to_char(round((d.tot_grootte_mb - f.total_bytes) / d.tot_grootte_mb * 100,2),'990.99')  "使用比(%)",
		  to_char(round(f.total_bytes / d.tot_grootte_mb * 100,2),'990.99')  "空闲比(%)"
		from (select tablespace_name,
			  round(sum(bytes) / (1024 * 1024), 2) total_bytes,
			  round(max(bytes) / (1024 * 1024), 2) max_bytes
			  from sys.dba_free_space
			  group by tablespace_name) f,
			(select dd.tablespace_name,
			  round(sum(dd.bytes) / (1024 * 1024), 2) tot_grootte_mb
			  from sys.dba_data_files dd
			  group by dd.tablespace_name) d
		where d.tablespace_name = f.tablespace_name
			and f.tablespace_name in ('TXNTBS','USERS','IDXTBS')
		order by f.tablespace_name;
    spool off;
    exit
EOF
    
    echo "                               数据库表空间监控"  >> $outfile
    echo " "   >> $outfile
    echo " "  >> $outfile
    awk 'BEGIN{printf("%-14.14s|%14.14s|%14.14s|%12.12s|%11.11s|%11.11s \n", "表空间名","表空间大小(M)","已使用空间(M)","空闲空间(M)","使用比(%)","空闲比(%)")}' >> $outfile 2>/dev/null
    echo '--------------------------------------------------------------------------------' >> $outfile

    while read LINE
    do
	    LINEFLAG=`echo $LINE|cut -c 1-1`
	    if [ $LINEFLAG = '|' ]
	    then
	       echo $LINE|awk -F'|' '{printf("%-14.14s|%14.2f|%14.2f|%12.2f|%11.2f|%11.2f\n", $3,$4,$5,$6,$7,$8)}' >> $outfile
	    fi

    done <$tmpfile.lst
    echo '--------------------------------------------------------------------------------' >> $outfile

  rm -rf $tmpfile.lst

}

getfs

