/*****************************************************************************/
/*                              TOPLINK 2.0                                  */
/* Copyright (c) 2005 Shanghai Huateng Software Systems Co., Ltd.            */
/* All Rights Reserved                                                       */
/*****************************************************************************/

#ifndef __ERR_CODE_H
#define __ERR_CODE_H

#define ERR_CODE_BASE			0

#define ERR_CODE_CONV_BASE		10000

#define ERR_CODE_CONV_NO_RULE		ERR_CODE_CONV_BASE+2001

#define ERR_CODE_COMMON_BASE	20000

#define ERR_CODE_COMMON_SRV_ID_INV	ERR_CODE_COMMON_BASE+1001

#define ERR_CODE_MSQ_BASE		30000

#define ERR_CODE_MSQ_INIT			ERR_CODE_MSQ_BASE+1001
#define ERR_CODE_MSQ_RCV			ERR_CODE_MSQ_BASE+1002
#define ERR_CODE_MSQ_SND			ERR_CODE_MSQ_BASE+1003
#define ERR_CODE_MSQ_TOO_MANY_MSG	ERR_CODE_MSQ_BASE+1004
#define ERR_CODE_MSQ_NO_MSG			ERR_CODE_MSQ_BASE+1005

#define ERR_CODE_LOG_BASE		40000
#define ERR_CODE_LOG_FILE_OPEN		ERR_CODE_LOG_BASE+1001

#define ERR_CODE_DBS_BASE		50000
#define ERR_CODE_DBS_SELECT			ERR_CODE_DBS_BASE+1001
#define ERR_CODE_DBS_INSERT			ERR_CODE_DBS_BASE+1002
#define ERR_CODE_DBS_UPDATE			ERR_CODE_DBS_BASE+1003
#define ERR_CODE_DBS_DELETE			ERR_CODE_DBS_BASE+1004

#define ERR_CODE_BRG_BASE		60000
#define ERR_CODE_BRD_NO_ROUTE		ERR_CODE_BRG_BASE+1001

#endif
