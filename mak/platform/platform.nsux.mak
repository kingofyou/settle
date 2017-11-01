#|----------------------------------------------------------------------------|
#|                           TOPMake 0.0.0                                    |
#| Copyright (c) 2000-2003 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME     : platform.nsux.mak                                          |
#|----------------------------------------------------------------------------|

SHELL = /bin/sh

CC    = cc
CXX   =
DLINK = ld
SLINK = ar
DLFLG = -shared
SLFLG = ruc
CCFLG = -shared -KPIC -signed -Wc,-align8
CXXFLG=
EXFLG = -call_shared -KPIC
DBFLG = -g
OPFLG = -O
DLFIX = .so
SLFIX = .a
UXLIBS = -lgen -lm -lc
DLLIBS = -ldl
NETLIBS = -lsocket -lnsl
CURLIBS = -lcurses
CRYLIBS =
THREADLIBS =
XLIBS   = -lXext -lX11
XLIBP   = .
OSDEF = -DNONSTOP_UNIX -DHAVENO_USLEEP -Dconst=""
OSTYPE = nsux
