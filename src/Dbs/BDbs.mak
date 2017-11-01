#|----------------------------------------------------------------------------|
#|                           TOPMake 2.0                                      |
#| Copyright (c) 2000-2003 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME    : sample.dynamiclib.mak                                       |
#| DESCRIPTIONS : Sample makefile for dynamic library                         |
#|----------------------------------------------------------------------------|

include $(BATCHHOME)/mak/platform.mak
include $(BATCHHOME)/mak/database.mak
include $(BATCHHOME)/mak/tuxedo.mak

PRGOBJS = dbscommon.o \
		001DbsTbldateInf.o  \
		002DbsInstMisn.o    \
		003DbsTblmissionInf.o   \
		004DbsInsertMisn.o   \
		005DbsFldDsp.o   \
		006DbsIpcDft.o   \
		007DbsIpcDftDsp.o  \
		008DbsConInf.o    \
		009DbsBufDsp.o   \
		010DbsFldTsf.o  \
		011DbsBufChg.o  \
		012XbsMisnTime.o  \
		013DbsTimeStamp.o \
		014DbsTblGatewayTxn.o \
		015DbsTblHostTxn.o  \
		016DbsTblRiskTxn.o \
		017DbsPartInf.o   \
		018DbsOprInf.o \
		019DbsTblErrFlow.o \
		020DbsTblCmpFlow.o \
		021DbsTblDatFlow.o   \
		022DbsTblSettFlow.o \
		023DbsTblSettRslt.o \
		024DbsTblAcctRslt.o  \
		025DbsTblCheckFlow.o \
		026DbsTblMchtInf.o   \
		027DbsTblHisDiscAlgo.o \
		030DbsSeqOpr.o \
		031DbsTblAlgoCtl.o \
		032DbsTblTxnAlgoRule.o \
		033DbsTblDiscAlgo.o \
		036DbsTblActiveMchtInf.o \
		037DbsTblSett.o \
		038DbsTblAcctInf.o \
		039DbsTblCIBTxn.o \
		040DbsTblPayWayExtTxn.o \
		041DbsTblBrhTxn.o \
		042DbsTblBrhFeeTxn.o\
		043DbsTpCertKey.o\
		044DbsTpBankCardsBin.o \
		045DbsTblExtMchtTxn.o\
		046DbsTblComConfig.o\
		047DbsTblWfitTxn.o \
		048DbsTpPaywayExt.o \
		049DbsTblCostCmpInf.o \
		050DbsTpOperaterAccInf.o\
		051DbsTpMercPaywayConf.o\
		052DbsTblSettList.o \
		053DbsSmsSendQueue.o \
		054DbsLiquidationReport.o \
		055DbsKeyCertInf.o \
		056DbsTblAlipayWechatTxn.o
		
PRGTARG = BDbs
PRGLIBS = $(DBLIBS) $(NETLIBS) $(UXLIBS)
#PRGDEFS = -lBPlat

# used for db2 database
PRGDBNM =

debug all: debugdynamic
release: releasedynamic

# DO NOT modify any code below!!!

releasedynamic debugdynamic releasestatic debugstatic releaseexec debugexec clean:
	@make -f $(BATCHHOME)/mak/mkstand.mak $@ TARGET="$(PRGTARG)" OBJS="$(PRGOBJS)" LIBS="$(PRGLIBS)" DEFS="$(PRGDEFS)" DBNM="$(PRGDBNM)"
