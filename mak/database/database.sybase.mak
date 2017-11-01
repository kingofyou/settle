#|----------------------------------------------------------------------------|
#|                           TOPMake 0.0.0                                    |
#| Copyright (c) 2000-2003 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME     : database.sybase.mak                                        |
#|----------------------------------------------------------------------------|

DBINCP = $(SYBASE)/$(SYBASE_OCS)/include
DBLIBP = -L$(SYBASE)/$(SYBASE_OCS)/lib
DBLIBS = -lct -lcs -ltcl -lcomn -lintl
DBD    = _DB_SYBASE
DBDEF  = -DDSYBASE -D$(DBD)
DBFIX  = .cp
DBTYPE = sybase
