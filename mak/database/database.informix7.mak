#|----------------------------------------------------------------------------|
#|                           TOPMake 0.0.0                                    |
#| Copyright (c) 2000-2003 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME     : database.informix7.mak                                     |
#|----------------------------------------------------------------------------|

DBINCP = $(INFORMIXDIR)/incl/esql
DBLIBP = -L$(INFORMIXDIR)/lib -L$(INFORMIXDIR)/lib/esql
DBLIBS = `esql -libs`
DBD    = _DB_INFX
DBDEF  = -DINFX7 -D$(DBD) -D__H_LOCALEDEF
DBFIX  = .ec
DBTYPE = infx
