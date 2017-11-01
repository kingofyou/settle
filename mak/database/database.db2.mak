#|----------------------------------------------------------------------------|
#|                           TOPMake 0.0.0                                    |
#| Copyright (c) 2000-2003 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME     : database.db2.mak                                           |
#|----------------------------------------------------------------------------|

DBINCP = $(DB2_HOME)/include
DBLIBP = -L$(DB2_HOME)/lib
DBLIBS = -ldb2
DBD    = _DB_DB2
DBDEF  = -DDB2 -D$(DBD)
DBFIX  = .sqc
DBTYPE = db2
