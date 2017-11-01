######################################################
######## 本脚本用于生成crontab信息，settle用户  ########
######################################################

#!/usr/bin/ksh
. $HOME/.bash_profile

crontab -l > $HOME/batch/sbin/timetable.bak
rm -f $HOME/batch/sbin/timetable

echo "22 01 * * * $HOME/batch/sbin/baklogandfile.sh" >> $HOME/batch/sbin/timetable
crontab $HOME/batch/sbin/timetable

rm -f $HOME/batch/sbin/timetable
