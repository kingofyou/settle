/*****************************************************************************/
/*                              TOPLINK 2.0                                  */
/* Copyright (c) 2005 Shanghai Huateng Software Systems Co., Ltd.            */
/* All Rights Reserved                                                       */
/*****************************************************************************/

#ifndef __HT_LOG_H
#define __HT_LOG_H

/* 日志文件名的长度的最大值 */
#define LOG_NAME_LEN_MAX		32

#define HT_LOG_MODE_OFF				0
#define HT_LOG_MODE_ERROR			1
#define HT_LOG_MODE_NORMAL			2
#define HT_LOG_MODE_DEBUG			3

#define LOG_ERROR  HT_LOG_MODE_ERROR,  __FILE__, __LINE__
#define LOG_NORMAL HT_LOG_MODE_NORMAL, __FILE__, __LINE__
#define LOG_DEBUG  HT_LOG_MODE_DEBUG,  __FILE__, __LINE__

/*****************************************************************************/
/* FUNC:   int HtLog (char *sLogName, int nLogMode, char *sFileName,         */
/*                    int nLine, char *sFmt, ...);                           */
/* INPUT:  sLogName: 日志名, 不带路径                                        */
/*         nLogMode: 日志级别,                                               */
/*                   HT_LOG_MODE_ERROR,HT_LOG_MODE_NORMAL,HT_LOG_MODE_DEBUG  */
/*         sFileName: 报错的源程序名                                         */
/*         nLine: 报错的源程序的行号                                         */
/*         sFmt: 出错信息                                                    */
/* OUTPUT: 无                                                                */
/* RETURN: 0: 成功, 其它: 失败                                               */
/* DESC:   根据LOG_MODE, 将该级别之下的日志记录到日志文件中,                 */
/*****************************************************************************/
int HtLogNoDate (char *sLogName, char *sFmt, ...);
int HtLog (char *sLogName, int nLogMode, char *sFileName, int nLine, char *sFmt, ...);
int HtBatServerLog (char *sLogName, int nLogMode, char *sFileName, int nLine, char *sFmt, ...);

/*****************************************************************************/
/* FUNC:   int HtDebugString (char *sLogName, int nLogMode, char *sFileName, */
/*                            int nLine, char *psBuf, int nBufLen);          */
/* INPUT:  sLogName: 日志名, 不带路径                                        */
/*         nLogMode: 日志级别,                                               */
/*                   HT_LOG_MODE_ERROR,HT_LOG_MODE_NORMAL,HT_LOG_MODE_DEBUG  */
/*         sFileName: 报错的源程序名                                         */
/*         nLine: 报错的源程序的行号                                         */
/*         psBuf: 需输出的buffer                                             */
/*         nBufLen: buffer的长度                                             */
/* OUTPUT: 无                                                                */
/* RETURN: 0: 成功, 其它: 失败                                               */
/* DESC:   根据LOG_MODE, 将该级别之下的日志记录到日志文件中,                 */
/*         输出内容是buffer的16进制值                                        */
/*****************************************************************************/
int HtDebugString (char *sLogName, int nLogMode, char *sFileName, int nLine, char *psBuf, int nBufLen);

#endif
