# set environment variables for NCUP
# database
# for ORACLE
ORACLE_BASE=/home/oracle/app;export ORACLE_BASE
ORACLE_HOME=$ORACLE_BASE;export ORACLE_HOME
ORACLE_SID=orcl;export ORACLE_SID
NLS_LANG=AMERICAN_AMERICA.ZHS16GBK;export NLS_LANG
NLS_DATE_FORMAT=YYYYMMDDHH24MISS;export NLS_DATE_FORMAT
export LD_LIBRARY_PATH=$ORACLE_HOME:/usr/lib
export TNS_ADMIN=$ORACLE_HOME
LIBPATH=$LIBPATH:$ORACLE_HOME/:$ORACLE_HOME/sdk/;export LIBPATH


DBNAME=sett;export DBNAME
DBUSER=settlement;export DBUSER
DBPWD=settlement;export DBPWD

#¹«¹²¿â
DBPUBNAME=multicard_pdb;export DBPUBNAME
DBPUBUSER=multicard;export DBPUBUSER
DBPUBPWD=multicard;export DBPUBPWD

# path
export PATH=$PATH:$HOME/bin:$ORACLE_HOME/bin:$ORACLE_HOME/sdk

