#|----------------------------------------------------------------------------|
#|                           TOPMake 2.0                                      |
#| Copyright (c) 2000-2004 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME     : database.ora9.mak                                          |
#|----------------------------------------------------------------------------|

DBINCP = $(ORACLE_HOME)/precomp/public
DBLIBP = -L$(ORACLE_HOME)/
DBLIBS = -lclntsh -lclient11  
DBD    = _DB_ORA
DBDEF  = -DORA10 -D$(DBD)
DBFIX  = .pc
DBTYPE = ora
