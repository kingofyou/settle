#|----------------------------------------------------------------------------|
#|                           TOPMake 0.0.0                                    |
#| Copyright (c) 2000-2003 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME     : database.mysql4.mak                                        |
#|----------------------------------------------------------------------------|

DBINCP = $(MYSQL_HOME)/include
DBLIBP = -L$(MYSQL_HOME)/lib 
DBLIBS = -lmysqlclient
DBD    = _DB_MYSQL
DBDEF  = -DMYSQL4 -D$(DBD)
DBTYPE = mysql
