#|----------------------------------------------------------------------------|
#|                           TOPMake 0.0.0                                    |
#| Copyright (c) 2000-2003 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME     : database.oracle81.mak                                      |
#|----------------------------------------------------------------------------|

DBINCP = .
DBLIBP = -L$(ORACLE_HOME)/lib
DBLIBS = -lclntsh -lclient8 `cat $(ORACLE_HOME)/lib/sysliblist`
DBD    = _DB_ORA
DBDEF  = -DORA8 -D$(DBD)
DBFIX  = .pc
DBTYPE = ora
