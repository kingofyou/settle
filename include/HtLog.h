/*****************************************************************************/
/*                              TOPLINK 2.0                                  */
/* Copyright (c) 2005 Shanghai Huateng Software Systems Co., Ltd.            */
/* All Rights Reserved                                                       */
/*****************************************************************************/

#ifndef __HT_LOG_H
#define __HT_LOG_H

/* ��־�ļ����ĳ��ȵ����ֵ */
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
/* INPUT:  sLogName: ��־��, ����·��                                        */
/*         nLogMode: ��־����,                                               */
/*                   HT_LOG_MODE_ERROR,HT_LOG_MODE_NORMAL,HT_LOG_MODE_DEBUG  */
/*         sFileName: �����Դ������                                         */
/*         nLine: �����Դ������к�                                         */
/*         sFmt: ������Ϣ                                                    */
/* OUTPUT: ��                                                                */
/* RETURN: 0: �ɹ�, ����: ʧ��                                               */
/* DESC:   ����LOG_MODE, ���ü���֮�µ���־��¼����־�ļ���,                 */
/*****************************************************************************/
int HtLogNoDate (char *sLogName, char *sFmt, ...);
int HtLog (char *sLogName, int nLogMode, char *sFileName, int nLine, char *sFmt, ...);
int HtBatServerLog (char *sLogName, int nLogMode, char *sFileName, int nLine, char *sFmt, ...);

/*****************************************************************************/
/* FUNC:   int HtDebugString (char *sLogName, int nLogMode, char *sFileName, */
/*                            int nLine, char *psBuf, int nBufLen);          */
/* INPUT:  sLogName: ��־��, ����·��                                        */
/*         nLogMode: ��־����,                                               */
/*                   HT_LOG_MODE_ERROR,HT_LOG_MODE_NORMAL,HT_LOG_MODE_DEBUG  */
/*         sFileName: �����Դ������                                         */
/*         nLine: �����Դ������к�                                         */
/*         psBuf: �������buffer                                             */
/*         nBufLen: buffer�ĳ���                                             */
/* OUTPUT: ��                                                                */
/* RETURN: 0: �ɹ�, ����: ʧ��                                               */
/* DESC:   ����LOG_MODE, ���ü���֮�µ���־��¼����־�ļ���,                 */
/*         ���������buffer��16����ֵ                                        */
/*****************************************************************************/
int HtDebugString (char *sLogName, int nLogMode, char *sFileName, int nLine, char *psBuf, int nBufLen);

#endif
