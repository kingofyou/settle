#ifndef _DBS_DEF_H
#define _DBS_DEF_H

#ifdef _DB_ORA
#define DBS_NOTFOUND            1403
#define DBS_KEYDUPLICATE          -1
#define DBS_FETCHNULL           -1405
#define DBS_ROWMORE          1427
#endif
#ifdef _DB_INFX
#define DBS_NOTFOUND            100
#define DBS_FETCHNULL           -407
#endif
#ifdef _DB_DB2
#define DBS_KEYDUPLICATE            -803
#define DBS_NOTFOUND            100
#define DBS_FETCHNULL           -305
#define DBS_ROWMORE          -811
#endif

#define DBS_IND_NULL		-1

/* tbl_srv_param.param_usage value */
#define SRV_PARAM_USAGE_NAME	"0"
#define SRV_PARAM_USAGE_ENV		"1"
#define SRV_PARAM_USAGE_ARG	"2"
#define DBS_SELECT4              2222
#define DBS_SELECT5              98
#define DBS_SELECT6              99
#define DBS_SELECT7              101
#define DBS_SELECT8              102
#define DBS_SELECT9              103
#define DBS_SELECT10             104
#define DBS_SELECT11             105
#define DBS_SELECT12             106
#define DBS_SELECT13             107
#define DBS_SELECT14             108
#define DBS_SELECT15             109
#define DBS_SELECT16             110
#define DBS_SELECT17             111
/* server env MSG_COMPRESS_FLAG value */
#define FLAG_YES				"Y"
#define FLAG_NO				"N"
#define FLAG_YES_C			'Y'
#define FLAG_NO_C			'N'

/* database user/password */
#define SADBSUserPwdFile "TL_HAHA_FILE"

/* tbl_sta_chg_cfg */
#define STA_CHG_IN_USE_FLAG        '1'
#define STA_CHG_UN_USE_FLAG        '0'
#define STA_CHG_NUM_MAX            200
#define STA_CHG_PRGM_FLAG          '1'

/* dbs operation defines for first argument in dbs function */
#define DBS_INIT       0
#define DBS_SELECT     1
#define DBS_LOCK       2
#define DBS_UPDATE     3
#define DBS_SELECT_H   4
#define DBS_DELETE     4
#define DBS_INSERT     5
#define DBS_INSERT_TDB 32
#define DBS_INSERT_RFS 88
#define DBS_INSERT_CC  46
#define DBS_INSERT_BDT 29
#define DBS_INSERT_FCP 39
#define DBS_INSERT_FCP_ADD 47
#define DBS_INSERT_SUCC 33
#define DBS_INSERT_EPOS_t0 43
#define DBS_INSERT_EPOS 44
#define DBS_INSERT_FAIL 34
#define DBS_INSERT_FRIGN 35
#define DBS_INSERT_BCT 36
#define DBS_INSERT_GM 48
#define DBS_INSERT_BDB 49
#define DBS_INSERT_APAI 50
#define DBS_INSERT_SCORE 51
#define DBS_INSERT_HOST_SCORE_SUCC 52
#define DBS_INSERT_IPAI_PUB 53
#define DBS_INSERT_GS 54
#define DBS_INSERT_PAY 55
#define DBS_INSERT_PAY_RESULT 56
#define DBS_INSERT_CUP 57
#define DBS_INSERT1    6
#define DBS_INSERT2    7
#define DBS_INSERT3    8
#define DBS_INSERT4    9
#define DBS_INSERT_ICS   601

#define DBS_INSERT_ATMP_SUCC  37
#define DBS_INSERT_ATMP_FC_SUCC  39
#define DBS_INSERT_POSP_SUCC  38
#define DBS_INSERT_POSP_SUCC  38
#define DBS_INSERT_EPOSP_SUCC 45

#define DBS_COUNT      1024
#define DBS_CURSOR     11
#define DBS_OPEN       12
#define DBS_CLOSE      13
#define DBS_FETCH      14
#define DBS_CURSOR1    1000
#define DBS_FETCH1     1001
#define DBS_OPEN1      1002
#define DBS_CLOSE1     1003
#define DBS_CURSOR2    1004
#define DBS_CURSOR3    1005
#define DBS_CURSOR4    1006
#define DBS_CURSOR5    1016
#define DBS_FETCH2     1007
#define DBS_OPEN2      1008
#define DBS_CLOSE2     1009
#define DBS_FETCH3     1010
#define DBS_OPEN3      1011
#define DBS_CLOSE3     1012
#define DBS_FETCH4     1013
#define DBS_OPEN4      1014
#define DBS_CLOSE4     1015
#define DBS_FETCH5     1017
#define DBS_OPEN5      1018
#define DBS_CLOSE5     1019
#define DBS_FETCH6     1020
#define DBS_OPEN6      1021
#define DBS_CLOSE6     1022
#define DBS_CURSOR6    1023
#define DBS_CURSOR7    2001
#define DBS_OPEN7      2002
#define DBS_FETCH7     2003
#define DBS_CLOSE7     2004
#define DBS_CURSOR8    2005
#define DBS_OPEN8      2006
#define DBS_FETCH8     2007
#define DBS_CLOSE8     2008
#define DBS_CURSOR9    2009
#define DBS_OPEN9      2010
#define DBS_FETCH9     2011
#define DBS_CLOSE9     2012
#define DBS_CURSOR10    2013
#define DBS_OPEN10      2014
#define DBS_FETCH10     2015
#define DBS_CLOSE10     2016
#define DBS_CURSOR11    2017
#define DBS_OPEN11      2018
#define DBS_FETCH11     2019
#define DBS_CLOSE11     2020
#define DBS_CURSOR12    2021
#define DBS_OPEN12      2022
#define DBS_FETCH12    2023
#define DBS_CLOSE12     2024
#define DBS_CURSOR13    2025
#define DBS_OPEN13      2026
#define DBS_FETCH13    2027
#define DBS_CLOSE13     2028
#define DBS_CURSOR14    2029
#define DBS_OPEN14      2030
#define DBS_FETCH14    2031
#define DBS_CLOSE14     2032
#define DBS_CURSOR15    2033
#define DBS_OPEN15      2034
#define DBS_FETCH15    2035
#define DBS_CLOSE15     2036

#define DBS_SELECT1    20
#define DBS_LOCK1      21
#define DBS_UPDATE1    22
#define DBS_DELETE1	   30
#define DBS_DELETE2	   61
#define DBS_SELECT2    23
#define DBS_LOCK2      24
#define DBS_UPDATE2    25
#define DBS_SELECT3    26
#define DBS_LOCK3      27
#define DBS_UPDATE3    28
#define DBS_UPDATE4    902
#define DBS_UPDATE5    901
#define DBS_UPDATE6    903
#define DBS_UPDATE7    904
#define DBS_UPDATE8    905
#define DBS_UPDATE9    906
#define DBS_UPDATE10   907
#define DBS_UPDATE11   908
#define DBS_UPDATE12   909
#define DBS_UPDATE13   910
#define DBS_UPDATE14   911
#define DBS_UPDATE15   912

#define DBS_INSERT_ERR_MISN 40
#define DBS_INSERT_CUPS_FLOW 41
#define DBS_INSERT_CUPS_LOGOFEE 42

/* DBS_SELECT21: key_rsp, txn_num */
#define DBS_SELECT21   121
/* DBS_SELECT22: key_revsal, txn_num */
#define DBS_SELECT22   122
/* DBS_SELECT23: key_cancel, txn_num */
#define DBS_SELECT23   123
/* DBS_SELECT24: key_revsal */
#define DBS_SELECT24   124

#define DBS_GET_NEXT_VAL	31

#define MAX_CON_VAL_LEN		35

#define MAX_FLD_NUM		200
#define MAX_MSG_NUM		500
#define MAX_IPC_NUM		50
#define MAX_BMP_NUM		500
#define MAX_CON_NUM		500
#define MAX_CON_VAL_LEN		35
#define MAX_BMP_VAL_LEN		100
#define F000_MSG_TYPE_LEN       4
#define F129_TXN_CODE_LEN       3
#define F130_TXN_NUMBER_LEN     4
#define F131_NEW_HEAD_LEN		46

/* 第二组附加操作 */
#define OPEN2                      206
#define CLOSE2                     207
#define FETCH2                     208

/* 第四组附加操作 */
#define OPEN4                      209
#define CLOSE4                     210
#define FETCH4                     211

#define LUsrErrNull                     0
#define LUsrErrMEncInit                 578
#define LUsrErrMMsqInit                 541
#define LUsrErrMNumOut                  507
#define LUsrErrMConnectDb               502
#define LUsrErrMEncOpr                  561
#define LUsrErrEBuildEncKey			    219
#define LUsrErrQTblLineInfOpr           625
#define LUsrErrQTblFileTransOpr         663
#define LSqlErrorUnknownOpr             2

#define NO_SWITCH             -10000

/* Dbs functions */
int DbsConnect ();
int DbsBegin ();
int DbsCommit ();
int DbsRollback ();
int DbsDelete (char *sType, char *sDate);
#endif

