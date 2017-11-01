#--------------SET PUBLIC ALIAS-------------------------
alias C="export LANG=C";
alias US="export LANG=en_US";
alias vm="view";
alias vo="/usr/bin/vi";
alias ll="ls -l";
alias l="ls -ltr";
alias lf="ls -F";
alias ls="ls -F";
alias lt="ls -lrt";
alias rm="rm -i";
alias cp="cp -i";
alias mv="mv -i";
alias ctags="/usr/local/bin/ctags";
#-------------------------------------------------------

#---------------SET TOOLS ALIAS-------------------------
alias sq='sqlplus $DBUSER/$DBPWD@$DBNAME'
alias cgrep='find . -name "*.*c" | xargs grep'
alias hgrep='find . -name "*.h" | xargs grep'
alias mgrep='find . -name "*mak*" | xargs grep'
alias rmlog='rm -f *log*'
#-------------------------------------------------------

#---------------SET PATH ALIAS--------------------------
#alias cdb='cd $BATCHHOME'
alias cds='cd $BATCHHOME/src'
alias cdi='cd $BATCHHOME/include'
alias cdf='cd $BATCHHOME/file'
alias cdbf='cd $SHARE_FILE_PATH/'
alias cdb='cd $BATCHHOME/bin'
alias cde='cd $BATCHHOME/etc'
alias cdsb='cd $BATCHHOME/sbin'
alias cdlib='cd $BATCHHOME/lib'
alias cdl='if [ -d $BATCHHOME/log/`date +%Y%m%d` ]; then cd $BATCHHOME/log/`date +%Y%m%d`; 
else mkdir -p $BATCHHOME/log/`date +%Y%m%d`; cd $BATCHHOME/log/`date +%Y%m%d`; fi'
alias cdt='cd $BATCHHOME/src/Tasks'
alias cdbM='cd $BATCHHOME/src/Daemon/MisnMain'
alias cdbTM='cd $BATCHHOME/src/Daemon/TMisnMain'
alias cdbS='cd $BATCHHOME/src/Daemon/SubMisnMain'
alias cdbTS='cd $BATCHHOME/src/Daemon/TSubMisnMain'
alias cdbI='cd $BATCHHOME/src/Daemon/InstMisn'
alias cdbD='cd $BATCHHOME/src/Dbs'
alias cdbE='cd $BATCHHOME/src/Daemon/ErrMain'
alias cdbR='cd $BATCHHOME/src/Rfc'

alias c='clear'

#-------------------------------------------------------

