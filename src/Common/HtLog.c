/*****************************************************************************/
/*                NCUP -- Shanghai Huateng Software System Inc.              */
/*****************************************************************************/
/* PROGRAM NAME: HtLog.c                                                     */
/* DESCRIPTIONS:                                                             */
/*****************************************************************************/
/*                             MODIFICATION LOG                              */
/* DATE        PROGRAMMER     DESCRIPTION                                    */
/* 2005-03-31  YU TONG        Initialize                                     */
/*****************************************************************************/
static char *Id = "@(#)$Header: /home/ctepub/cvs/ctedev/batch/src/Common/HtLog.c,v 1.1.1.1 2012/06/08 07:50:31 ctedev Exp $";

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <memory.h>
#include <unistd.h>
#include "SrvParam.h"
#include "ErrCode.h"
#include "HtLog.h"

/* ��־�ļ�·���ĳ��ȵ����ֵ */
#define LOG_PATH_LEN_MAX		128
/* ��־�ļ����ĳ��ȵ����ֵ */
#define LOG_NAME_LEN_MAX		32

/* ��־�л�ģʽ, ���ļ���С������ */
#define LOG_SWITCH_MODE_SIZE	1
#define LOG_SWITCH_MODE_DATE	2

#define LOG_SIZE_UNIT			1000000

#define LOG_DEFAULT_NAME		"ht.log"
#define SINGLE_LINE				"--------------------------------------------------------------------------------\n"

FILE	*fp;

extern char ext_inter_brh_code[10 + 1];
int	 ConvertEnv(char *str);
int ReplaceEnvVar(char *str);

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
int HtLog (char *sLogName, int nLogMode, char *sFileName, int nLine, char *sFmt, ...)
{
    char	sLogTime[128];
    char	sDateTime[16];
    char    sUsecTime[10];
    char	sParamLogFilePath[LOG_PATH_LEN_MAX];
    int		nParamLogMode;
    int		nParamLogSwitchMode;
    int		nParamLogSize; /* in megabytes */
    int		nReturnCode;
    va_list	ap;
    time_t	lTime;
    struct tm	*tTmLocal;
    char    *pPoint;
    char    *pPointTmp;
    char    sCommand[40];
    struct stat stFilePathStat;

    /* get config parameter, LOG_MODE, LOG_FILE_PATH, LOG_SWITCH_MODE, LOG_SIZE */
    nParamLogMode = atoi (getenv (LOG_MODE));

    if (nParamLogMode == 0)
        nParamLogMode = HT_LOG_MODE_OFF;
    nParamLogSwitchMode = atoi (getenv (LOG_SWITCH_MODE));
    if (nParamLogSwitchMode == 0)
        nParamLogSwitchMode = LOG_SWITCH_MODE_SIZE;
    nParamLogSize = atoi (getenv (LOG_SIZE));
    if (nParamLogSize == 0)
        nParamLogSize = LOG_SIZE_DEFAULT;
    memset (sParamLogFilePath, 0, sizeof (sParamLogFilePath));
    if (getenv (LOG_FILE_PATH))
        HtStrcpy (sParamLogFilePath, (char *)getenv (LOG_FILE_PATH));
    if (strlen (sParamLogFilePath) == 0)
        HtStrcpy (sParamLogFilePath, ".");

    /* check whether the msg should be saved in log file */
    if (nParamLogMode < nLogMode)
        return 0;

    /* get current time */
    memset (sLogTime, 0x00, sizeof(sLogTime));
    memset (sDateTime, 0x00, sizeof(sDateTime));
    memset (sUsecTime, 0x00, sizeof(sUsecTime));
		     
    lTime = time (NULL);
    tTmLocal = localtime (&lTime);
    strftime (sLogTime, sizeof(sLogTime), "%Y%m%d %H%M%S", tTmLocal);
    strftime (sDateTime, sizeof(sDateTime), "%Y%m%d%H%M%S", tTmLocal);

	struct timeval tv;
    gettimeofday(&tv,0);   
    sprintf(sUsecTime,"%03d",tv.tv_usec/1000);
    
    /** added by jxz 20090413*/
    /* append the current date to the file path */
    HtStrcat(sParamLogFilePath, "/");
    strncat(sParamLogFilePath, sDateTime, 8);

    /* check if the file path exsit. if not, create it */
    memset(&stFilePathStat, 0x00, sizeof(stFilePathStat));
    stat(sParamLogFilePath, &stFilePathStat);
    if (!S_ISDIR(stFilePathStat.st_mode))
    {
        memset(sCommand, 0, sizeof(sCommand));
        HtStrcpy(sCommand, "mkdir ");
        HtStrcat(sCommand, sParamLogFilePath);
        HtStrcat(sCommand, " > /dev/null 2>/dev/null");
        system(sCommand);
    }
    HtStrcat(sParamLogFilePath, "/");

    /*modify by sunyudong*/
    strcat(sParamLogFilePath, ext_inter_brh_code);

    /* check if the file path exsit. if not, create it */
    memset(&stFilePathStat, 0x00, sizeof(stFilePathStat));
    stat(sParamLogFilePath, &stFilePathStat);
    if (!S_ISDIR(stFilePathStat.st_mode))
    {
        memset(sCommand, 0, sizeof(sCommand));
        HtStrcpy(sCommand, "mkdir ");
        HtStrcat(sCommand, sParamLogFilePath);
        HtStrcat(sCommand, " > /dev/null 2>/dev/null");
        system(sCommand);
    }

    /* check if the file path exsit. if not, create it
    HtStrcat(sParamLogFilePath, ext_inter_brh_code);
    memset(&stFilePathStat, 0x00, sizeof(stFilePathStat));
    stat(sParamLogFilePath, &stFilePathStat);
    if (!S_ISDIR(stFilePathStat.st_mode))
    {
        memset(sCommand, 0, sizeof(sCommand));
        HtStrcpy(sCommand, "mkdir ");
        HtStrcat(sCommand, sParamLogFilePath);
        HtStrcat(sCommand, " > /dev/null 2>/dev/null");
        system(sCommand);
    }
    */

    /* open log file */
    nReturnCode = OpenLogFile (sParamLogFilePath, sLogName, nParamLogSwitchMode, nParamLogSize, sDateTime);
    if (nReturnCode)
        return (nReturnCode);

    /*
    pPointTmp = sFileName;
    while ((pPointTmp = memchr(pPointTmp, '/', strlen(pPointTmp))) != 0)
    {
    	pPointTmp = pPointTmp + 1;
    	pPoint = pPointTmp;
    }
    */

    /* save log msg in file */
    //fprintf(fp, "[%10d][%s][% 12s:%04d]", getpid(),sLogTime,pPoint,nLine);
    fprintf(fp, "[%06ld][%s][%3s][%12s][%05d]->", (long)getpid(), sLogTime,sUsecTime, sFileName, nLine);

    va_start(ap, sFmt);
    vfprintf(fp, sFmt, ap);
    va_end(ap);

    fprintf(fp, "\n");
    fflush(fp);

    /* close file */
    fclose (fp);

    return (0);
}

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
#if 0
int HtDebugString (char *sLogName, int nLogMode, char *sFileName, int nLine, char *psBuf, int nBufLen)
{
    register int i, j = 0;
    char 	sLine[100], sTemp[6];
    char	sLogTime[128];
    char	sDateTime[16];
    char	sParamLogFilePath[LOG_PATH_LEN_MAX];
    int		nParamLogMode;
    int		nParamLogSwitchMode;
    int		nParamLogSize; /* in megabytes */
    int		nReturnCode;
    va_list	ap;
    time_t	lTime;
    struct tm	*tTmLocal;

    /* get config parameter, LOG_MODE, LOG_FILE_PATH, LOG_SWITCH_MODE, LOG_SIZE */
    nParamLogMode = atoi (getenv (LOG_MODE));
    if (nParamLogMode == 0)
        nParamLogMode = HT_LOG_MODE_OFF;
    nParamLogSwitchMode = atoi (getenv (LOG_SWITCH_MODE));
    if (nParamLogSwitchMode == 0)
        nParamLogSwitchMode = LOG_SWITCH_MODE_SIZE;
    nParamLogSize = atoi (getenv (LOG_SIZE));
    if (nParamLogSize == 0)
        nParamLogSize = LOG_SIZE_DEFAULT;
    memset (sParamLogFilePath, 0, sizeof (sParamLogFilePath));
    if (getenv (LOG_FILE_PATH))
        HtStrcpy (sParamLogFilePath, (char *)getenv (LOG_FILE_PATH));
    if (strlen (sParamLogFilePath) == 0)
        HtStrcpy (sParamLogFilePath, ".");

    /* check whether the msg should be saved in log file */
    if (nParamLogMode < nLogMode)
        return 0;

    /* get current time */
    memset (sLogTime, 0x00, sizeof(sLogTime));
    memset (sDateTime, 0x00, sizeof(sDateTime));
    lTime = time (NULL);
    tTmLocal = localtime (&lTime);
    strftime (sLogTime, sizeof(sLogTime), "%Y-%m-%d %H:%M:%S", tTmLocal);
    strftime (sDateTime, sizeof(sDateTime), "%Y%m%d%H%M%S", tTmLocal);

    /* open log file */
    nReturnCode = OpenLogFile (sParamLogFilePath, sLogName, nParamLogSwitchMode, nParamLogSize, sDateTime);
    if (nReturnCode)
        return (nReturnCode);

    fprintf(fp, "[%s][%s][%d]len=%d\n", sLogTime, sFileName, nLine, nBufLen);
    fprintf(fp, "%80.80s\n", SINGLE_LINE);

    /* save log msg in file */
    for	(i = 0; i < nBufLen; i++)
    {
        /* initialize a new line */
        if (j == 0)
        {
            memset ( sLine,	' ', sizeof(sLine));
            HtSprintf (sTemp,	"%04d:", i );
            HtMemcpy (sLine, sTemp, 5);
            HtSprintf (sTemp, ":%04d", i + 15 );
            HtMemcpy (sLine + 72, sTemp, 5);
        }

        /* output psBuf value in hex */
        HtSprintf( sTemp, "%02X ", (unsigned	char)psBuf[i]);
        HtMemcpy( &sLine[j * 3 + 5 + (j > 7)], sTemp, 3);

        /* output psBuf in ascii */
        if ( isprint (psBuf[i]))
        {
            sLine[j + 55 + (j > 7)] = psBuf[i];
        }
        else
        {
            sLine[j + 55 + (j > 7)] = '.';
        }
        j++;

        /* output the line to file */
        if (j == 16)
        {
            sLine[77] = 0;
            fprintf(fp, "%s\n", sLine);
            j = 0;
        }
    }

    /* last line */
    if (j)
    {
        sLine[77] = 0;
        fprintf(fp, "%s\n",	sLine);
    }
    fprintf(fp, "%80.80s\n", SINGLE_LINE);

    fflush(fp);

    /* close file */
    fclose (fp);

    return (0);
}
#endif
int HtDebugString (char *sLogName, int nLogMode, char *sFileName, int nLine, char *psBuf, int nBufLen)
{
    register int i, j = 0;
    char    sLine[100], sTemp[6];
    char    sLogTime[128];
    char    sDateTime[16];
    char    sUsecTime[10];
    char    sParamLogFilePath[LOG_PATH_LEN_MAX];
    int     nParamLogMode;
    int     nParamLogSwitchMode;
    int     nParamLogSize; /* in megabytes */
    int     nReturnCode;
    time_t  lTime;
    struct tm   *tTmLocal;
    struct stat stFilePathStat;

    /* get config parameter, LOG_MODE, LOG_FILE_PATH, LOG_SWITCH_MODE, LOG_SIZE */
    nParamLogMode = atoi (getenv (LOG_MODE));
    if (nParamLogMode == 0)
        nParamLogMode = HT_LOG_MODE_OFF;
    nParamLogSwitchMode = atoi (getenv (LOG_SWITCH_MODE));
    if (nParamLogSwitchMode == 0)
        nParamLogSwitchMode = LOG_SWITCH_MODE_SIZE;
    nParamLogSize = atoi (getenv (LOG_SIZE));
    if (nParamLogSize == 0)
        nParamLogSize = LOG_SIZE_DEFAULT;
    memset (sParamLogFilePath, 0, sizeof (sParamLogFilePath));
    if (getenv (LOG_FILE_PATH))
        strcpy (sParamLogFilePath, (char *)getenv (LOG_FILE_PATH));
    if (strlen (sParamLogFilePath) == 0)
        strcpy (sParamLogFilePath, ".");

    /* check whether the msg should be saved in log file */
    if (nParamLogMode < nLogMode)
        return 0;

    /* get current time */
    memset (sLogTime, 0x00, sizeof(sLogTime));
    memset (sDateTime, 0x00, sizeof(sDateTime));
    memset (sUsecTime, 0x00, sizeof(sUsecTime));
    lTime = time (NULL);
    tTmLocal = localtime (&lTime);
    strftime (sLogTime, sizeof(sLogTime), "%m%d %H%M%S", tTmLocal);
    strftime (sDateTime, sizeof(sDateTime), "%Y%m%d%H%M%S", tTmLocal);

	struct timeval tv;
    gettimeofday(&tv,0);   
    sprintf(sUsecTime,"%03d",tv.tv_usec/1000);

    /** added by jxz 20090413*/
    /* append the current date to the file path */
    strcat(sParamLogFilePath, "/");
    strncat(sParamLogFilePath, sDateTime, 8);

    /* check if the file path exsit. if not, create it */
    memset(&stFilePathStat, 0x00, sizeof(stFilePathStat));
    stat(sParamLogFilePath, &stFilePathStat);
    if (!S_ISDIR(stFilePathStat.st_mode))
    {
        nReturnCode = mkdir( sParamLogFilePath, S_IRWXU | S_IRWXG );
        if(nReturnCode != 0)
        {
            return nReturnCode;
        }
    }
    /* end of added by jxz 20090413*/

    /* open log file */
    nReturnCode = OpenLogFile (sParamLogFilePath, sLogName, nParamLogSwitchMode, nParamLogSize, sDateTime);
    if (nReturnCode)
        return (nReturnCode);

    fprintf(fp, "[%06ld][%s][%3s][%12s][%05d][nLen: %d]\n", (long)getpid(), sLogTime,sUsecTime, sFileName, nLine, nBufLen);
    fprintf(fp, "%80.80s\n", SINGLE_LINE);
    /* save log msg in file */
    for (i = 0; i < nBufLen; i++)
    {
        /* initialize a new line */
        if (j == 0)
        {
            memset ( sLine, ' ', sizeof(sLine));
            sprintf (sTemp, "%04d:", i );
            memcpy (sLine, sTemp, 5);
            sprintf (sTemp, ":%04d", i + 15 );
            memcpy (sLine + 72, sTemp, 5);
        }

        /* output psBuf value in hex */
        sprintf( sTemp, "%02X ", (unsigned  char)psBuf[i]);
        memcpy( &sLine[j * 3 + 5 + (j > 7)], sTemp, 3);

        /* output psBuf in ascii */
        if ( isprint (psBuf[i]))
        {
            sLine[j + 55 + (j > 7)] = psBuf[i];
        }
        else
        {
            sLine[j + 55 + (j > 7)] = '.';
        }
        j++;

        /* output the line to file */
        if (j == 16)
        {
            sLine[77] = 0;
            fprintf(fp, "%s\n", sLine);
            j = 0;
        }
    }
    /* last line */
    if (j)
    {
        sLine[77] = 0;
        fprintf(fp, "%s\n", sLine);
    }
    fprintf(fp, "%80.80s\n", SINGLE_LINE);

    fflush(fp);

    /* close file */
    fclose (fp);

    return (0);
}

/*****************************************************************************/
/* FUNC:   int OpenLogFile (char *sLogFilePath, char *sLogName,              */
/*                          int nLogSwitchMode, int nLogSize,                */
/*                          char *sDate, FILE *fp)                           */
/* INPUT:  sLogFilePath: ��־·��                                            */
/*         sLogName: ��־�ļ���                                              */
/*         nLogSwitchMode: ��־�л�ģʽ                                      */
/*                   LOG_SWITCH_MODE_SIZE, LOG_SWITCH_MODE_DATE              */
/*         nLogSize: LOG_SWITCH_MODE_SIZEģʽ���ļ���С                      */
/*         sDateTime: ��ǰʱ��, YYYYMMDDhhmmss                               */
/* OUTPUT: fp: �򿪵���־�ļ���ָ��                                          */
/* RETURN: 0: �ɹ�, ����: ʧ��                                               */
/* DESC:   ����nLogSwitchMode, ����־�ļ�                                  */
/*         LOG_SWITCH_MODE_SIZE: ���ļ���С(M)�ﵽnLogSize, �л������ļ�,    */
/*                               ԭ�ļ�����Ϊ�ļ����д���ʱ��                */
/*                               xx.log.YYYYMMDDhhmmss                       */
/*         LOG_SWITCH_MODE_DATE: ��־�ļ�����������, xx.log.YYYYMMDD         */
/*****************************************************************************/
int OpenLogFile (char *sLogFilePath, char *sLogName, int nLogSwitchMode, int nLogSize, char *sDateTime )
{
    char		sExpFilePath[LOG_PATH_LEN_MAX];
    char		sFullLogName[LOG_PATH_LEN_MAX + LOG_NAME_LEN_MAX];
    char		sFullBakLogName[LOG_PATH_LEN_MAX + LOG_NAME_LEN_MAX];
    int			nReturnCode;
    struct stat	statbuf;

    memset (sFullLogName, 0x00, sizeof (sFullLogName));


    HtStrcpy (sExpFilePath, sLogFilePath);
    nReturnCode = ReplaceEnvVar (sExpFilePath);

    /* set log file name */
    if (!sLogName || strlen(sLogName) == 0)
        HtSprintf (sFullLogName, "%s/%s", sExpFilePath, LOG_DEFAULT_NAME);
    else
        HtSprintf (sFullLogName, "%s/%s", sExpFilePath, sLogName);


    if (nLogSwitchMode == LOG_SWITCH_MODE_DATE)
    {
        /* append date in log file name */
        strncat (sFullLogName, sDateTime, 8);
    }
    else
    {
        /* this is LOG_SWITCH_MODE_SIZE */
        /* check file size */
        memset (&statbuf, 0x00, sizeof(statbuf));
        nReturnCode = stat (sFullLogName, &statbuf);
        if ( nReturnCode == 0 && statbuf.st_size >= LOG_SIZE_UNIT * nLogSize )
        {
            memset (sFullBakLogName, 0x00, sizeof(sFullBakLogName));
            HtSprintf (sFullBakLogName, "%s.%s", sFullLogName, sDateTime);
            rename (sFullLogName, sFullBakLogName);
        }
    }

    /* open log file */

    fp = fopen (sFullLogName, "a+");
    if (fp == NULL )
        return (ERR_CODE_LOG_BASE + errno);

    return 0;
}

int	 ConvertEnv(char *str)
{
    char envbuf[LOG_PATH_LEN_MAX], buf[LOG_PATH_LEN_MAX];
    char *ptr = NULL, *ptr1 = NULL;
    int  len, choice = 0;

    memset(buf, 0, sizeof(buf));

    ptr = (char *)strpbrk((char *)str, "./");
    if ( ptr != NULL )
    {
        choice = 1;
        memset(envbuf, 0, sizeof(envbuf));
        HtMemcpy(envbuf, &str[1], ptr - str - 1);
    }
    else
    {
        choice = 2;
        memset(envbuf, 0, sizeof(envbuf));
        HtStrcpy(envbuf, &str[1]);
    }

    ptr1 = (char *)getenv(envbuf);
    if ( ptr1 == (char *)NULL )
    {
        return -1;
    }
    else
    {
        if ( choice == 1 )
        {
            HtStrcat(buf, ptr1);
            HtStrcat(buf, ptr);
        }
        else if ( choice == 2 )
            HtStrcat(buf, ptr1);
    }

    len = strlen(buf);

    memset(str, 0, sizeof(str));
    HtStrcpy(str, buf);

    return 0;
}

/*****************************************************************************/
/* FUNC:   int ReplaceEnvVar(char *str)                                      */
/* INPUT:  str: �ַ���, ���ܰ�����������                                     */
/*              ��: $(HOME)/src/$TERM                                        */
/* OUTPUT: str: �ַ���, ���������Ѿ��ö�Ӧ��ֵ�滻                           */
/* RETURN: 0: �ɹ�, ����: ʧ��                                               */
/* DESC:   ��������ַ���, �����еĻ��������ö�Ӧ��ֵ�滻                    */
/*         ��: $(HOME)/src/$TERM, ���е�$(HOME)��$TERM�ᱻ�滻               */
/*         ����ʱ�뱣֤str��Ӧ��buffer�㹻��, �������滻���Խ��             */
/*****************************************************************************/
int ReplaceEnvVar(char *str)
{
    char *ptr = NULL;
    char buf[LOG_PATH_LEN_MAX], field[LOG_PATH_LEN_MAX];
    int	 len = 0, nRet, flag;
    int  count = 1, i, j;

    /********************************************************************/
    /* 		delete 	characters ' ','(',')','\t' from string "str"		*/
    /********************************************************************/
    len = strlen(str);
    memset(buf, 0, sizeof(buf));

    for(i = 0, j = 0; i < len; i++)
        if ( str[i] != ' ' && str[i] != '\t' && str[i] != '(' && str[i] != ')' )
            buf[j++] = str[i];

    memset(str, 0, len);
    HtStrcpy(str, buf);

    /********************************************************/
    /* 		distinguish if first character is '$' or not	*/
    /********************************************************/
    if ( str[0] == '$' )
        flag = 1;
    else
        flag = 2;

    memset(buf, 0, sizeof(buf));
    if ( flag == 1 )
    {
        ptr = (char *)strtok((char *)str, "$");
        while ( ptr != NULL )
        {
            memset(field, 0, sizeof(field));
            HtSprintf(field, "%c%s", '$', ptr);
            nRet = ConvertEnv(field);
            if ( nRet != 0 )
                return -1;

            HtStrcat(buf, field);

            ptr = (char *)strtok((char *)NULL, "$");
        }
    }
    else if ( flag == 2 )
    {
        ptr = (char *)strtok((char *)str, "$");
        while ( ptr != NULL )
        {
            count++;
            if ( count != 2)
            {
                memset(field, 0, sizeof(field));
                HtSprintf(field, "%c%s", '$', ptr);
                nRet = ConvertEnv(field);
                if ( nRet != 0 )
                    return -1;
            }
            else
            {
                memset(field, 0, sizeof(field));
                HtSprintf(field, "%s", ptr);
            }

            HtStrcat(buf, field);

            ptr = (char *)strtok((char *)NULL, "$");
        }
    }

    len = strlen(buf);
    HtMemcpy(str, buf, len + 1);

    return 0;
}


int HtLogNoDate (char *sLogName, char *sFmt, ...)
{
    char    sLogTime[128];
    char    sDateTime[16];
    char    sParamLogFilePath[LOG_PATH_LEN_MAX];
    int     nParamLogMode;
    int     nParamLogSwitchMode;
    int     nParamLogSize; /* in megabytes */
    int     nReturnCode;
    long    lTimeTick;
    va_list ap;
    time_t  lTime;
    struct tm   *tTmLocal;
    struct tms  tTMS;
    struct stat stFilePathStat;

    /* get config parameter, LOG_MODE, LOG_FILE_PATH, LOG_SWITCH_MODE, LOG_SIZE */
    nParamLogMode = atoi (getenv ("LOG_8583"));
    if (nParamLogMode == 0)
        return(0);
    /*nParamLogMode = HT_LOG_MODE_OFF; */
    nParamLogSwitchMode = atoi (getenv (LOG_SWITCH_MODE));
    if (nParamLogSwitchMode == 0)
        nParamLogSwitchMode = LOG_SWITCH_MODE_SIZE;
    nParamLogSize = atoi (getenv (LOG_SIZE));
    if (nParamLogSize == 0)
        nParamLogSize = LOG_SIZE_DEFAULT;
    memset (sParamLogFilePath, 0, sizeof (sParamLogFilePath));
    if (getenv (LOG_FILE_PATH))
        strcpy (sParamLogFilePath, (char *)getenv (LOG_FILE_PATH));
    if (strlen (sParamLogFilePath) == 0)
        strcpy (sParamLogFilePath, ".");

    /* get current time */
    memset (sLogTime, 0x00, sizeof(sLogTime));
    memset (sDateTime, 0x00, sizeof(sDateTime));
    lTime = time (NULL);
    tTmLocal = localtime (&lTime);
    strftime (sLogTime, sizeof(sLogTime), "%Y-%m-%d %H:%M:%S", tTmLocal);
    strftime (sDateTime, sizeof(sDateTime), "%Y%m%d", tTmLocal);

    /** added by jxz 20090413*/
    /* append the current date to the file path */
    strcat(sParamLogFilePath, "/");
    strncat(sParamLogFilePath, sDateTime, 8);

    /* check if the file path exsit. if not, create it */
    memset(&stFilePathStat, 0x00, sizeof(stFilePathStat));
    stat(sParamLogFilePath, &stFilePathStat);
    if (!S_ISDIR(stFilePathStat.st_mode))
    {
        nReturnCode = mkdir( sParamLogFilePath, S_IRWXU | S_IRWXG );
        if(nReturnCode != 0)
        {
            return nReturnCode;
        }
    }
    /* end of added by jxz 20090413*/

    /* get current time in tick */
    lTimeTick = times( &tTMS);

    /* open log file */
    nReturnCode = OpenLogFile (sParamLogFilePath, sLogName, nParamLogSwitchMode, nParamLogSize, sLogTime);
    if (nReturnCode)
        return (nReturnCode);

    /* save log msg in file */
    va_start(ap, sFmt);
    vfprintf(fp, sFmt, ap);
    va_end(ap);

    fflush(fp);

    /* close file */
    fclose (fp);

    return (0);
}
