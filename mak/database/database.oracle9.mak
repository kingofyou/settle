#|----------------------------------------------------------------------------|
#|                           TOPMake 0.0.0                                    |
#| Copyright (c) 2000-2003 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME     : database.oracle9.mak                                       |
#|----------------------------------------------------------------------------|

DBINCP = .
DBLIBP = -L$(ORACLE_HOME)/lib
DBLIBS = -lclntsh -lclient9 `cat $(ORACLE_HOME)/lib/sysliblist`
DBD    = _DB_ORA
DBDEF  = -DORA9 -D$(DBD)
DBFIX  = .pc
DBTYPE = ora
