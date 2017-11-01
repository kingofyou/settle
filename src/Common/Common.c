/*****************************************************************************/
/*               NCUP -- Shanghai Huateng Software System Inc.               */
/*****************************************************************************/
/* PROGRAM NAME: Common.c                                                    */
/* DESCRIPTIONS: The common routines, including                              */
/*               CommonGetCurrentTime -- Get the system time by format       */
/*                                       (YYYYMMDDHHMMSS)                    */
/*               CommonGetCurrentDate -- Get the system date by format       */
/*                                       (YYYYMMDD)                          */
/*****************************************************************************/
/*                             MODIFICATION LOG                              */
/* DATE        PROGRAMMER     DESCRIPTION                                    */
/* 2005-03-29  DONG TIEJUN    Initial Version Creation                       */
/*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include "batch.h"
#include <dlfcn.h>

#include "Common.h"


typedef void(*sighandler_t)(int);

static int  iBrgPort;
static char sBrgIP[20];
static int  Timeout = 0;
static struct stat DataTmpPathStat;
extern  char    gLogFile[LOG_NAME_LEN_MAX];
jmp_buf Stackbuf;
int nReturn;

typedef struct _T_SYS_HEAD
{
    char  DNAD[4];
    char  SCAD[4];
    char  RSVB;
    char  MGEF;
    short MGSQ;
    char  IFCK;
    char  CKVL[8];
} T_SYS_HEAD;

typedef struct _T_PUB_HEAD
{
    char  WSNO[5];
    char  CTNO[4];
    char  SBNO[4];
    char  USID[8];
} T_PUB_HEAD;

typedef struct _T_DATA_HEAD
{
    char  TRCD[4];
    char  TRSD[2];
    char  TRMD;
    char  TRSQ[4];
    short TRLN;
    short OFF1;
    short OFF2;
    char  AUUS[8];
    char  AUPS[8];
} T_DATA_HEAD;

typedef struct _T_RSP_HEAD
{
    char  TRCD[4];
    char  LDTC[4];
    char  TRDT[8];
    char  TRTM[4];
    char  TLSQ[12];
    short ERTR;
    char ERID[7];
} T_RSP_HEAD;

typedef struct _T_ERR_OBJ
{
    char  NAMELEN;
    char  NAME[6];
    char  ERRNOLEN;
    char  ERRNO[2];
    char  ERRIDLEN;
    char  ERRID[7];
    char  ERRTXLEN;
    char  ERRTX[64];
} T_ERR_OBJ;


/*****************************************************************************/
/* FUNC:   void CommonGetCurrentDate (char *sCurrentDate);                   */
/* INPUT:  <none>                                                            */
/* OUTPUT: sCurrentDate   -- the string of current date                      */
/* RETURN: <none>                                                            */
/* DESC:   Get the system date with the format (YYYYMMDD).                   */
/*****************************************************************************/
void  CommonGetCurrentDate(char *sCurrentDate)
{
    time_t current;
    struct tm *tmCurrentTime;

    time(&current);
    tmCurrentTime = localtime(&current);
    HtSprintf(sCurrentDate, "%4d%02d%02d", tmCurrentTime->tm_year + 1900,
              tmCurrentTime->tm_mon + 1, tmCurrentTime->tm_mday);
}

/*****************************************************************************/
/* FUNC:   void CommonGetCurrentTime (char *sCurrentTime);                   */
/* INPUT:  <none>                                                            */
/* OUTPUT: sCurrentTime   -- the string of current time                      */
/* RETURN: <none>                                                            */
/* DESC:   Get the system time with the format (YYYYMMDDhhmmss).             */
/*****************************************************************************/
void CommonGetCurrentTime(char *sCurrentTime)
{
    time_t current;
    struct tm *tmCurrentTime;

    tzset();
    time(&current);
    tmCurrentTime = localtime(&current);
    HtSprintf(sCurrentTime, "%4d%02d%02d%02d%02d%02d",
              tmCurrentTime->tm_year + 1900, tmCurrentTime->tm_mon + 1,
              tmCurrentTime->tm_mday, tmCurrentTime->tm_hour,
              tmCurrentTime->tm_min, tmCurrentTime->tm_sec);
}

void CommonRTrim(char *caDest)
{
    int i;
    for( i = strlen(caDest) - 1 ; i >= 0 ; i-- )
    {
        if( caDest[i] != ' ')
        {
            break;
        }
    }
    caDest[i + 1] = '\0';
}

/**
 *
 */
char *rtrim(char *pstring)
{
    int j;

    j = strlen(pstring);
    if (j <= 0)
        return(pstring);
    while ((j != 0) && (pstring[j - 1] == ' '))
        j--;
    pstring[j] = '\0';
    return(pstring);
}

int CommonLTrim( char *caDest )
{
    int i;
    int j = 0;
    char lsTmp[1024];
    memset(lsTmp, 0, sizeof(lsTmp));

    for( i = 0; i < strlen(caDest) ; i++ )
    {
        if( caDest[i] == ' ')
        {
            continue;
        }
        lsTmp[j++] = caDest[i];
    }
    HtStrcpy(caDest, lsTmp);

    return 0;
}

/*****************************************************
*函数名称: CountDate
*函数功能: 计算一个日期加上或者减去一个天数后所得的日期
*输入参数: lStartDate -- 起始日期(YYYYMMDD)
iOffset    -- 偏移量(负数表示减去)
*输出参数: sResultDate -- 计算所得日期
*返回值:   0  -- 成功
-1 -- 失败
*****************************************************/
long CountDate(char *sStartDate, int iOffset, char *sResultDate)
{
    int  step;
    long lStartDate;
    int  iYear, iMonth, iDays;
    long i;
    lStartDate  =  atol(sStartDate);

    iYear  = lStartDate / 10000;
    iMonth = lStartDate % 10000 / 100;
    iDays  = lStartDate % 100;

    step = (iOffset >= 0 ? 1 : -1);

    for(i = 1; i <= abs(iOffset); i ++)
    {
        iDays += step;
        if (iDays <= 0 || iDays > RMONTHDAYS(iMonth, iYear))
        {
            iMonth += step;
            if (iMonth <= 0)
            {
                iYear += step;
                iMonth = 12;
            }
            else if (iMonth > 12)
            {
                iYear += step;
                iMonth = 1;
            }

            if (iDays <= 0)
                iDays = RMONTHDAYS(iMonth, iYear);
            else
                iDays = 1;
        }
    }

    lStartDate = iYear * 10000 + iMonth * 100 + iDays;

    sprintf(sResultDate, "%8ld", lStartDate);

    return 0;
}

/********************************************************************
  Description : convert time from string (YYYYMMDDhhmmss)
                to time_t

  Input   : sTime -- time, YYYYMMDDhhmmss
  Return  : time in time_t
********************************************************************/
time_t GetTimeInLong (char *sTime , int GmtFlag)
{
    char str[20];
    struct tm tmTmp;

    memset (&tmTmp, '\0', sizeof (tmTmp));

    /* get year, year start from 1900 in struct tm */
    memcpy (str, sTime, 4);
    str[4] = '\0';
    tmTmp.tm_year = atoi (str) - 1900;
    /* get month, month start from 0 in struct tm */
    memcpy (str, sTime + 4, 2);
    str[2] = '\0';
    tmTmp.tm_mon = atoi (str) - 1;
    /* get day */
    memcpy (str, sTime + 6, 2);
    str[2] = '\0';
    tmTmp.tm_mday = atoi (str);
    /* get hour */
    memcpy (str, sTime + 8, 2);
    str[2] = '\0';
    tmTmp.tm_hour = atoi (str);
    /* get minute */
    memcpy (str, sTime + 10, 2);
    str[2] = '\0';
    tmTmp.tm_min = atoi (str);
    /* get second */
    memcpy (str, sTime + 12, 2);
    str[2] = '\0';
    tmTmp.tm_sec = atoi (str);
    if (GmtFlag)
        return mktime(&tmTmp) + 8 * 60 * 60;
    else
        return mktime(&tmTmp);
}

void GetDayOfYear(char *insert_time, char *sDayOfYear)
{
    struct tm  *tmCurrent;
    time_t     lTime;

    lTime = GetTimeInLong(insert_time, 0);
    tmCurrent = localtime(&lTime);
    /*sprintf(sDayOfYear, "%03d", tmCurrent->tm_yday + 1 ); */
    sprintf(sDayOfYear, "%03d", tmCurrent->tm_mday + 1 );
    return;
}

void Hex2Str( char *sSrc,  char *sDest, int nSrcLen )
{
    int  i;
    char szTmp[3];

    for( i = 0; i < nSrcLen; i++ )
    {
        HtSprintf( szTmp, "%02X", (unsigned char) sSrc[i] );
        HtMemcpy( &sDest[i * 2], szTmp, 2 );
    }
    return ;
}

void Str2Hex( char *sSrc, char *sDest, int nSrcLen )
{
    int i, nHighBits, nLowBits;

    for( i = 0; i < nSrcLen; i += 2 )
    {
        nHighBits = sSrc[i];
        nLowBits  = sSrc[i + 1];

        if( nHighBits > 0x39 )
            nHighBits -= 0x37;
        else
            nHighBits -= 0x30;

        if( i == nSrcLen - 1 )
            nLowBits = 0;
        else if( nLowBits > 0x39 )
            nLowBits -= 0x37;
        else
            nLowBits -= 0x30;

        sDest[i / 2] = (nHighBits << 4) | (nLowBits & 0x0f);
    }
    return ;
}

/*****************************************************************************/
/* FUNC:   int CalNewDate( char *srcDate, int days, char *desDate )          */
/* INPUT:  srcDate： source date                                             */
/*         days： move date                                                  */
/* OUTPUT: desDate: des date                                                 */
/* DESC:   Get new date.                                                     */
/*****************************************************************************/
int CalNewDate( char *srcDate, int days, char *desDate )
{
    if ( days == 0 )
    {
        HtStrcpy( desDate, srcDate );
    }
    else
    {
        char tmpYear[5];
        char tmpMonth[3];
        char tmpDay[3];

        int year = 0;
        int month = 0;
        int day = 0;

        int monthDays[ 2 ][ 13 ] = { 365, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
                                     366, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
                                   };

        strncpy( tmpYear, srcDate, 4 );
        strncpy( tmpMonth, srcDate + 4, 2 );
        strncpy( tmpDay, srcDate + 4 + 2, 2 );

        tmpYear[ 4 ] = '\0';
        tmpMonth[ 2 ] = '\0';
        tmpDay[ 2 ] = '\0';

        /* 将日期的字符格式转换成整型 */
        year = atoi( tmpYear );
        month = atoi( tmpMonth );
        day = atoi( tmpDay );

        day += days;

        /* 计算日期 */
        if ( days > 0 )/* 向后计算日期 */
        {
            while ( day > monthDays[ isLeapYear( year ) ][ month ] ) /* 超出当月天数就要月份增加 */
            {
                day -= monthDays[ isLeapYear( year ) ][ month ];
                month += 1;

                if ( month > 12 )
                {
                    year += 1;
                    month = 1;
                }
            }
        }
        else/* 向前计算日期 */
        {
            while ( day <= 0 )/*日如果为0或负数，就要减少月份 */
            {
                month -= 1;

                if ( month < 1 )
                {
                    year -= 1;
                    month = 12;
                }

                day += monthDays[ isLeapYear( year ) ][ month ];
            }
        }

        HtSprintf( tmpYear, "%d", year );
        month < 10 ? HtSprintf( tmpMonth, "0%d", month ) : HtSprintf( tmpMonth, "%d", month );
        day < 10 ? HtSprintf( tmpDay, "0%d", day ) : HtSprintf( tmpDay, "%d", day );

        HtStrcpy( desDate, tmpYear );
        HtStrcat( desDate, tmpMonth );
        HtStrcat( desDate, tmpDay );

        desDate[ 8 ] = '\0';
    }

    return 0;
}

int isLeapYear( int year )
{
    if ((( year % 4 == 0 ) && ( year % 100 != 0 )) || ( year % 400 == 0 ) )
        return 1;

    return 0;
}

int tcpClose( sock )
int sock;
{
    if ( fcntl( sock, F_SETFL, FNDELAY ) < 0 )
        return( -1 );
    if ( sock != 0 )
        shutdown( sock, 2 );

    close( sock );
    return( 0 );
}

int WriteSock(socketid, len, buffer)
int socketid;
int len;
char *buffer;
{
    int  num, iWritelen;
    unsigned char Buf_head[5];
    char saSendBuf[2048];

    if (len == 0) return(0);

    memset(saSendBuf, 0, sizeof(saSendBuf));
    len = len + 2;

    memset(&saSendBuf, 0, 6);
    saSendBuf[2] = len / 256;
    saSendBuf[3] = len % 256;
    saSendBuf[4] = len / 256;
    saSendBuf[5] = len % 256;

    HtMemcpy(saSendBuf + 6, buffer, len - 2);
    len = len + 4;

    HtDebugString (gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__,
                   saSendBuf, len);

    iWritelen = 0;
    for(;;)
    {
        while((num = write(socketid, &saSendBuf[iWritelen], len - iWritelen)) <= 0)
        {
            if (errno == EINTR) continue;
            HtLog (gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "write socket error");
            return(-1);
        }
        iWritelen += num;
        if(iWritelen >= len) break;
    }
    return(iWritelen);
}

/*
 *  * Function Name:  IOTimeOut
 *   * Parameter 1  :  no    ???ú??ó?
 *   */
void IOTimeOut(int no )
{
    Timeout = 1;
    longjmp( Stackbuf, Timeout );
}

int ReadSock(socketid, buffer, timeout)
int socketid;
register char *buffer;
int  timeout;
{
    int num, nLen, I = 0, nleft, nread;
    unsigned char tmp_buf[15], Buf_head[15];

    Timeout = 0;
    signal( SIGALRM, IOTimeOut );
    alarm( timeout );

    setjmp( Stackbuf );
    if ( Timeout )
    {
        alarm( 0 );
        return( -1 );
    }

    memset( tmp_buf, 0x00, sizeof(tmp_buf) );

    num = read(socketid, tmp_buf, 6);
    if ( num == 0 )
    {
        HtLog (gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "num = 0  ,read len error = %d", errno);
        alarm(0);
        return (-1);
    }
    else  if ( num < 0 )
    {
        HtLog (gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "read len error = %d", errno);
        alarm(0);
        return (-1);
    }
    else
        HtDebugString( gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, (char *)tmp_buf, (int) 6 ) ;

    memset(Buf_head, 0, sizeof(Buf_head));
    HtMemcpy(Buf_head, tmp_buf + 4, 2);

    nLen = Buf_head[0] * 256 + Buf_head[1] - 2;
    if ( nLen == 0 || nLen > 4096 )
    {
        HtLog (gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "len error = %d", nLen);
        alarm(0);
        return (-1);
    }
    else
        HtLog (gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "data len  = %d", nLen);

    nleft = nLen;
    while ( nleft > 0 )
    {
        nread = read(socketid, buffer, nleft);
        if (nread < 0 && errno == EINTR)
        {
            HtLog (gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "EINTR error");
            break;
        }
        if (nread < 0)
        {
            HtLog (gsLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "read socket error");
            alarm(0);
            return(-1);
        }
        if ( nread == 0 ) break;
        nleft -= nread;
        buffer += nread;
        if ( nleft <= 0 )
            break ;
    }
    alarm(0);
    return (nLen - nleft);

}


int tcpOpen(char *sIpAddr, int nPort, int nRetrys, int nTimeOut)
{
    int    sock;
    struct sockaddr_in remote;

    bzero(&remote, sizeof(remote));
    remote.sin_family = AF_INET;
    remote.sin_addr.s_addr = inet_addr(sIpAddr);
    remote.sin_port = htons(nPort);

    HtLog( gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "ip %s port %d", sIpAddr, nPort);

    Timeout = 0;
    signal(SIGALRM, IOTimeOut);
    alarm(nTimeOut);

    setjmp(Stackbuf);
    if (Timeout)
    {
        alarm(0);
        return -1;
    }

    while (nRetrys)
    {
        HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "retrys %d", nRetrys);
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            if (nRetrys > 0)
            {
                nRetrys--;
                continue;
            }
            return -2;
        }

        HtLog(gsLogFile, HT_LOG_MODE_NORMAL, __FILE__, __LINE__, "socket ok %d", sock);

        if (connect(sock, (struct sockaddr *)&remote, sizeof(remote)) < 0)
        {
            tcpClose(sock);
            if (nRetrys > 0)
            {
                nRetrys--;
                continue;
            }
            return -3;
        }
        alarm(0);
        return sock;
    } /* while */
    return -4;
}

void TransPtn(char *ptn, char *sDate)
{
    char *pstr;
    int i = 0;
    char ch = *ptn;
    CommonRTrim(ptn);

    /* replace YYYYMMDD with date */
    pstr = strstr (ptn, "YYYY");
    if (pstr)
        HtMemcpy (pstr, sDate, 4);
    pstr = strstr (ptn, "YY");
    if (pstr)
        HtMemcpy (pstr, sDate + 2, 2);
    pstr = strstr (ptn, "MM");
    if (pstr)
        HtMemcpy (pstr, sDate + 4, 2);
    pstr = strstr (ptn, "DD");
    if (pstr)
        HtMemcpy (pstr, sDate + 6, 2);

    while (ch != '\0')
    {
        if( ch == '_' )
            *(ptn + i) = '?';
        i++;
        ch = *(ptn + i);
    }
    *(ptn + i) = '\0';
    return;
}

/****************************************************************
 * *   函 数 名: StringIsNull
 * *   功    能: 判断字符串是否全为空格或0x00
 * *   编程人员:
 * *   编程时间:
 * *   修改人员:
 * *   修改时间:
 * *   修改原因:
 * *   备    注: 返回值
 *                 1 - 空串
 *                                 0 - 非空串
 ******************************************************************/
int StrIsNull(char *pcaString, int iLen )
{
    int i;

    for( i = 0; i < iLen; i++ )
    {
        if( pcaString[i] != ' ' && pcaString[i] != (char)0x00 )
            return 0;
    }

    return 1;
}
/****************************************************************
 * *   函 数 名: CheckDir
 * *   功    能: 判断文件夹是否存在，如不存在则创建该文件夹
 * *   编程人员:
 * *   编程时间:
 * *   修改人员:
 * *   修改时间:
 * *   修改原因:
 * *   备    注: 返回值
 *                 1 - 创建文件夹失败
 *                 0 - 成功
 ******************************************************************/

int CheckDir(char *sPathSrc)
{
    char sPath[256];
    char sTmpPath[256];
    char cmd[500];
    char *pDir;
    int  nReturn;
    memset(sTmpPath, 0, 256);
    memset(sPath, 0, 256);
    HtMemcpy(sPath, sPathSrc, strlen(sPathSrc));

    pDir = strtok(sPath, "/");
    strcat(sTmpPath, "/");
    strcat(sTmpPath, pDir);
    strcat(sTmpPath, "/");

    memset(&DataTmpPathStat, 0, sizeof(DataTmpPathStat));
    stat(sTmpPath, &DataTmpPathStat);
    if (!S_ISDIR(DataTmpPathStat.st_mode))
    {
        /*nReturn = mkdir(sTmpPath, S_IWUSR|S_IRUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH);*/
        /*nReturn = mkdir(sTmpPath,  S_IRWXU | S_IRWXG | S_IRWXO );*/
        nReturn = mkdir(sTmpPath,  S_IRWXU|S_IRUSR|S_IWUSR|S_IRWXG|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH|S_IXOTH);       
        if(nReturn == -1 && errno != EEXIST)
        {
            HtLog (gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mkdir directory error: [%s].", sTmpPath);
            return -1;
        }
        chmod(sTmpPath , S_IRWXU|S_IRUSR|S_IWUSR|S_IRWXG|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH|S_IXOTH );
    }
    while((pDir = strtok(NULL, "/")) != NULL)
    {
        strcat(sTmpPath, pDir);
        strcat(sTmpPath, "/");

        memset(&DataTmpPathStat, 0, sizeof(DataTmpPathStat));
        stat(sTmpPath, &DataTmpPathStat);
        if (!S_ISDIR(DataTmpPathStat.st_mode))
        {
            /*nReturn = mkdir(sTmpPath, S_IWUSR|S_IRUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH);*/
            /*nReturn = mkdir(sTmpPath,  S_IRWXU | S_IRWXG | S_IRWXO );*/
            nReturn = mkdir(sTmpPath,  S_IRWXU|S_IRUSR|S_IWUSR|S_IRWXG|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH|S_IXOTH); 
            if (nReturn == -1 && errno != EEXIST)
            {
                HtLog (gsLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "mkdir directory error: [%s].", sTmpPath);
                return -1;
            }
        }
        chmod(sTmpPath , S_IRWXU|S_IRUSR|S_IWUSR|S_IRWXG|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH|S_IXOTH );
    }

    /*chmod(sPathSrc , S_IRWXU | S_IRWXG | S_IRWXO );*/
    chmod(sPathSrc , S_IRWXU|S_IRUSR|S_IWUSR|S_IRWXG|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH|S_IXOTH );
    return 0;
}

char *RightTrim ( char *str )
{
    char *s = str;
    s = str + strlen(str) - 1;
    while ( s >= str )
        if ( ( *s == ' ' ) || ( *s == '\t' ) || ( *s == '\r' ) || ( *s == '\n'  ) )
            --s;
        else
            break;
    *( s + 1 ) = 0;
    return str;
}

/*
 *  去除str开始处的空字符
 */
char *LeftTrim(char *str)
{
    char    *s = str;

    while (*s)
        if ((*s == ' ') || (*s == '\t') || (*s == '\r') || (*s == '\n'))
        {
            ++s;
        }
        else
        {
            break;
        }

    if (s > str)
    {
        strcpy((char *)str, (char *)s);
    }

    return str;
}

/*******************************************************
**函数名称: Trim                                      **
**功能描述: 去除str两端的空字符                       **
**作    者:                                           **
**输入参数:                                           **
            str: 源字符串                             **
**输出参数:                                           **
**返回值:   目标的字符串                              **
*******************************************************/
char *Trim( char *str )
{
    if(str == NULL)
    {
        return NULL;
    }
    RightTrim( str );
    LeftTrim( str );
    return str;
}


int nMCalcIn(
    int                         *vlpNumVal,
    void                          *vvpNumStr,
    int                          vnNumStrL)
{

    unsigned char                              *lspNumStr;

    lspNumStr = vvpNumStr;
    *vlpNumVal = 0;
    for(; (vnNumStrL > 0) && (*lspNumStr == 0); vnNumStrL--, lspNumStr++);
    if(vnNumStrL > sizeof(*vlpNumVal))
        return -1;
    for(; vnNumStrL > 0; vnNumStrL--, lspNumStr++)
        *vlpNumVal = *vlpNumVal * 256 + *lspNumStr;
    return 0;
} /* end of nMCalcIn */

int nMCalcOut(
    unsigned short                          vlNumVal,
    void                          *vvpNumStr,
    int                          vnNumStrL)
{

    unsigned char                              *lspNumStr;

    lspNumStr = vvpNumStr;
    memset(
        vvpNumStr,
        0,
        vnNumStrL);
    lspNumStr += (vnNumStrL - 1);
    for(; (vnNumStrL > 0) && (vlNumVal > 0); vnNumStrL--, lspNumStr--)
    {
        *lspNumStr = vlNumVal % 256;
        vlNumVal /= 256;
    } /* end of for */
    if(vlNumVal > 0)
        return -1;
    return 0;
} /* end of nMCalcOut */

/* add by zhangm 20120926 动态加载库调用库函数*/
int funcProc(lib_name, func_name)
char* lib_name;
char* func_name;
{
    void    *handle;
    static int     (* func)() = NULL;
    int ret;

    CommonRTrim( func_name);
    CommonRTrim( lib_name );

    /* if ((handle =dlopen(lib_name,RTLD_LAZY))==NULL) */
    if ((handle = dlopen(lib_name, RTLD_NOW)) == NULL)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "libname[%s]errno[%d][%s]", lib_name, errno, strerror(errno));
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "dlopen error:[%s]", dlerror() );
        /*  dlclose(handle);*/
        return ( -1 );
    }

    if ((func = (int ( *)(char *))dlsym(handle, func_name)) == NULL)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "funcName[%s]", func_name);
        dlclose(handle);
        return ( -1 );
    }

    ret = (*func)();
    dlclose(handle);
    return ret;
}
int funcSubProc(lib_name, func_name, begin, end)
char *lib_name;
char *func_name;
int begin;
int end;
{
    void    *handle;
    static int     (* func)() = NULL;
    int ret;

    CommonRTrim( func_name);
    CommonRTrim( lib_name );

    /* if ((handle =dlopen(lib_name,RTLD_LAZY))) */
    /* if ((handle =dlopen(lib_name,RTLD_NOW)))  */

    if ((handle = dlopen(lib_name, RTLD_NOW)) == NULL)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "libname[%s]errno[%d][%s]", lib_name, errno, strerror(errno) );
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "dlopen error:[%s]", dlerror() );
        /*  dlclose(handle);  */
        return ( -1 );
    }

    if ((func = (int ( *)(char *))dlsym(handle, func_name)) == NULL)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "funcName[%s]", func_name);
        dlclose(handle);
        return ( -1 );
    }

    ret = (*func)(begin, end );
    dlclose(handle);
    return ret;
}

/*******************************************************
 * 函数名称: IsStrNum
 * 函数描述: 判断输入的字符串是否全为数字
 * 函数参数: str 输入字符串
 * 函数返回:  0 - 源字符串字段都为数字
 *            1 - 源字符串字段不全为数字
 ********************************************************/
int IsStrNum(char *str)
{
    int  i;
    for(i = 0; i < strlen(str); i++)
    {
        if( str[i] < '0' || str[i] > '9')
            return 1;
    }
    return 0;
}

/*******************************************************
 * 函数名称: IsStrChar
 * 函数描述: 判断输入的字符串是否全为数字或字母
 * 函数参数: str 输入字符串
 * 函数返回:  0 - 源字符串字段都为数字或字母
 *            1 - 源字符串字段不全为字母
 ********************************************************/
int IsStrChar(char *str)
{
    int  i;
    for(i = 0; i < strlen(str); i++)
    {
        if( str[i] < '0' ||
                (str[i] > '9' && str[i] < 'A') ||
                (str[i] > 'Z' && str[i] < 'a') ||
                str[i] > 'z')
            return 1;
    }
    return 0;
}


/***************************************************
*函数名称∶ cvtISOTime
*函数功能∶ 日期时间格式转换,将带8位全数字日期、或14为日期时间格式日期时间转换为ISO标准格式
*输入参数∶
			sSDateTime -
*输出参数∶ sTDateTime -
*函数返回∶  0 -- 成功
            -1 -- 失败
***************************************************/
int cvtISOTime(char *sSDateTime, char *sTDateTime)
{
    char sYear[4 + 1], sMonth[2 + 1], sDay[2 + 1], sHour[2 + 1], sMin[2 + 1], sSec[2 + 1];

    memset(sYear, 0x00, sizeof(sYear));
    memset(sMonth, 0x00, sizeof(sMonth));
    memset(sDay, 0x00, sizeof(sDay));
    memset(sHour, 0x00, sizeof(sHour));
    memset(sMin, 0x00, sizeof(sMin));
    memset(sSec, 0x00, sizeof(sSec));

    strncpy(sYear, sSDateTime, 4);
    strncpy(sMonth, sSDateTime + 4, 2);
    strncpy(sDay, sSDateTime + 6, 2);

    if(strlen(sSDateTime) >= 14)
    {
        strncpy(sHour, sSDateTime + 8, 2);
        strncpy(sMin, sSDateTime + 10, 2);
        strncpy(sSec, sSDateTime + 12, 2);

        sprintf(sTDateTime, "%4s-%2s-%2sT%2s:%2s:%2s", sYear, sMonth, sDay, sHour, sMin, sSec);
    }
    else
    {
        sprintf(sTDateTime, "%4s-%2s-%2s", sYear, sMonth, sDay);
    }

    return(0);
}

/* 日期时间格式转换函数，将ISO标准格式日期时间转换为带8位全数字日期、或14为日期时间格式 */
int cvtNumTime(char *sSDateTime, char *sTDateTime)
{
    char sYear[4 + 1], sMonth[2 + 1], sDay[2 + 1], sHour[2 + 1], sMin[2 + 1], sSec[2 + 1];

    memset(sYear, 0x00, sizeof(sYear));
    memset(sMonth, 0x00, sizeof(sMonth));
    memset(sDay, 0x00, sizeof(sDay));
    memset(sHour, 0x00, sizeof(sHour));
    memset(sMin, 0x00, sizeof(sMin));
    memset(sSec, 0x00, sizeof(sSec));

    strncpy(sYear, sSDateTime, 4);
    strncpy(sMonth, sSDateTime + 5, 2);
    strncpy(sDay, sSDateTime + 8, 2);

    if(strlen(sSDateTime) >= 19)
    {
        strncpy(sHour, sSDateTime + 11, 2);
        strncpy(sMin, sSDateTime + 14, 2);
        strncpy(sSec, sSDateTime + 17, 2);

        sprintf(sTDateTime, "%4s%2s%2s%2s%2s%2s", sYear, sMonth, sDay, sHour, sMin, sSec);
    }
    else
    {
        sprintf(sTDateTime, "%4s%2s%2s", sYear, sMonth, sDay);
    }

    return(0);
}


/*************************************************
 * 函数名称：getValueByDelim
 * 函数描述：根据分隔符将源字符串sSrc中第一个分隔符sDelim之前的字符串复制到目的串sDest中
 *          并将源字符串sSrc的指针移动到第一个分隔符之后
 *函数参数： sSrc  源字符串
 *           sDest 目的字符串
 *           sDelim 分隔符
 *函数返回：  0 - 成功
 *            1 - 源字符串已到达尾部
 *            2 - 出错
***************************************************/
int getValueByDelim(char *sSrc, char *sDest, char *sDelim )
{
    char *sEnd;

    if( *sSrc == '\0')
    {
        return 1;
    }

    sEnd = strstr(sSrc, sDelim);
    if(sEnd == NULL)
    {
        if(strstr(sSrc, "\n") != NULL)
        {
            sEnd = sSrc + strlen(sSrc) - 1;
        }
        else
        {
            sEnd = sSrc + strlen(sSrc);
        }
    }

    memcpy(sDest, sSrc, sEnd - sSrc);

    memcpy(sSrc, sEnd + (int)strlen(sDelim), strlen(sSrc));
    return 0;
}


/***************************************************
*函数名称∶ getFileName
*函数功能∶ 从路径截取文件名
*输入参数∶
			pathName -含路径文件名

*输出参数∶ fileName -文件名
*函数返回∶  0 -- 成功
            -1 -- 失败
***************************************************/
int getFileName(char *pathName, char *fileName)
{
    int pos;

    pos = strlen(pathName);
    while(pos > 0 && pathName[--pos] != '/') ;
    if(pos > 0)
    {
        memcpy(fileName, pathName + pos + 1, strlen(pathName) - pos);
        return 0 ;
    }
    return -1;
}

/*
 *  Function:  ClearBlank
 *
 *      To remove all comment and blank char from one line
 *
 *  Parameters:
 *
 *      line - point of a string of a line
 *
 *  Return Value:
 */

static void ClearBlank(char *line)
{
    int i = 0, j, k;
    char buf[PFL_LINE_BUFFER_SIZE];

    while (line[i] != 0)
    {
        /*modify by sunyd @20110530/
        if (line[i] == PFL_MEMO_CHARACTER || line[i] == '\n')
        /modify by sunyd @20110530*/
        if (line[i] == '\n')
        {
            line[i] = 0;
            if (i == 0) break;
            j = i - 1;
            do
            {
                if (j < 0) break;
                if ((line[j] != ' ') && (line[j] != '\t')) break;
                line[j--] = 0;
            }
            while(1);
            break;
        }
        i++;
    }

    i = 0;
    memset(buf, 0, PFL_LINE_BUFFER_SIZE);

    while ((line[i] != '=') && (i < (int)strlen(line)))
        i++;

    if (i == (int)strlen(line)) return;

    for (j = i - 1; (line[j] == ' ') || (line[j] == '\t'); j--);
    for (k = i + 1; (line[k] == ' ') || (line[k] == '\t'); k++);

    memcpy(buf, line, j + 1);
    buf[j + 1] = '=';
    strcat(buf + j + 2, line + k);

    strcpy(line, buf);
}

/*
 *  Function:  IsSection
 *
 *      To confirm if a line is a section line
 *
 *  Parameters:
 *
 *      line - point of a string of a line
 *
 *  Return Value:
 *
 *       0 - is not a section line
 *       1 - is a section line
 */

static int IsSection(char *line)
{
    return line[0] == '[';
}


/*
 *  Function:  IsThisSection
 *
 *      To confirm if this line is a spec section
 *
 *  Parameters:
 *
 *      line    - point of a string of a line
 *      section - name of section
 *
 *  Return Value:
 *
 *       0 - is not this section
 *       1 - is this section
 */

static int IsThisSection(const char *line, const char *section)
{
    return !memcmp(line + 1, section, strlen(section));
}


/*
 *  Function:  IsThisEntry
 *
 *      To confire if this line is a spec entry
 *
 *  Parameters:
 *
 *      line  - point of a string of a line
 *      entry - name of entry
 *
 *  Return Value:
 *
 *       0 - is not this entry
 *       1 - is this entry
 */

static int IsThisEntry(const char *line, const char *entry)
{
    return (!memcmp(line, entry, strlen(entry)) &&
            line[strlen(entry)] == '=') ;
}

/*
 *  Function:  CutContent
 *
 *      To get the value from a line
 *
 *  Parameters:
 *
 *      line  - point of a string of a line
 *      dest  - value of entry to put
 *
 *  Return Value:
 */

static void CutContent(char *line, char *dest)
{
    int i = 0;

    while (line[i++] != '=');
    strcpy(dest, line + i);
}


 int glbPflGetString(const char *section,
                           const char *entry,
                           const char *filename,
                           char       *value)
{
    FILE  *fp;
    char  line[PFL_LINE_BUFFER_SIZE];
    int cbNum = FALSE;
    int InThisSection = FALSE;

    if ((fp = fopen(filename, "r")) == NULL)
        return FAILURE;

    while (NULL != fgets(line, PFL_LINE_BUFFER_SIZE, fp))
    {
        ClearBlank(line);

        if (IsSection(line))
        {
            InThisSection = IsThisSection(line, section);
            continue;
        }

        if (InThisSection == FALSE) continue;

        if (IsThisEntry(line, entry))
        {
            CutContent(line, value);
            cbNum = TRUE;
            break;
        }
    }

    fclose(fp);

    if (cbNum == FALSE)
        return FAILURE;

    return SUCCESS;
}


/**
 *  从配置文件中取值
 *      结果放在第一个参数中
 *
 */
int getPiXmValue(char *buf, char *section, char *item)
{
    int  ret = 0;
    char saValue[BT_ITEM_VALUE_LEN];
    char saFilePath[BT_FILE_PATH_LEN];

    memset(saFilePath,  0, sizeof(saFilePath));
    sprintf(saFilePath, "%s/config/%s", getenv("BATCHHOME"), "xm.cfg");

    memset(saValue, 0, sizeof(saValue));
    ret = glbPflGetString(section, item, saFilePath, saValue);
    if(ret != 0)
    {
        HtLog(gLogFile, LOG_ERROR, "error:glbPflGetString[%s][%s]", section, item);
        return -1;
    }

    strcpy(buf, saValue);

    return 0;
}


/***************************************************
*函数名称∶ getExtnFileFullName
*函数功能∶ 获取文件全名
*输入参数∶
			StExtnFileReg -
*输出参数∶ sFileName -
*函数返回∶  0 -- 成功
            -1 -- 失败
***************************************************/
int getExtnFileFullName(const stExtnFileRegDef *StExtnFileReg, char *sFileName)
{
    int iRet = 0;

    sprintf(sFileName, "%s/%8.8s/%s/%s/",
            getenv("BATCH_FILE_PATH"),
            StExtnFileReg->stlm_dt,
            StExtnFileReg->inter_brh_code,
            StExtnFileReg->host_name);

    iRet = CheckDir(sFileName);
    if (iRet)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "CheckDir err[%d][%s][%s]",
               iRet, strerror(errno), sFileName);
        return -1;
    }

    strcat(sFileName, StExtnFileReg->src_file_name);

    return 0;
}

/***************************************************
*函数名称∶ getExtnFileFullName
*函数功能∶ 获取文件全名
*输入参数∶
			StExtnFileReg -
*输出参数∶ sFileName -
*函数返回∶  0 -- 成功
            -1 -- 失败
***************************************************/
int getShareFileFullName(const stExtnFileRegDef *StExtnFileReg, char *sFileName)
{
    int iRet = 0;

    sprintf(sFileName, "%s/%8.8s/%s/%s/",
            getenv("SHARE_FILE_PATH"),
            StExtnFileReg->stlm_dt,
            StExtnFileReg->inter_brh_code,
            StExtnFileReg->host_name);

    iRet = CheckDir(sFileName);
    if (iRet)
    {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "CheckDir err[%d][%s][%s]",
               iRet, strerror(errno), sFileName);
        return -1;
    }
    
    strcat(sFileName, StExtnFileReg->src_file_name);
    return 0;
}

/***************************************************
*函数名称∶ CheckPanIfEqual
*函数功能∶ 检查两个卡号是否属于同一卡号
*输入参数∶ panA -- 卡号A
            panB -- 卡号B
*输出参数∶
*函数返回∶  0 -- 成功
            -1 -- 失败
***************************************************/
int CheckPanIfEqual(char *panA, char *panB)
{
    int iPanALen = 0;
    int iPanBLen = 0;

    CommonRTrim(panA);
    CommonRTrim(panB);

    iPanALen = strlen(panA);
    iPanBLen = strlen(panB);

    if (iPanALen == iPanBLen)
    {
        if (!memcmp(panA, panB, iPanALen))
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }

    return -1;
}

/*******************************************************
根据分隔符分割并放置到dstBuf中
sSrcBuf 传入分割前的字符串
iSrcLen 源字符串长度
cTab 分隔符
iNum 第几段，从1开始

如果返回值<0，则失败
如果成功，则返回字符串长度

传入的字符串必须是标准以分隔符分割，最后一段需要有分隔符
********************************************************/
int GetInfoToBuf(char *sSrcBuf, int iSrcLen, char cTab, int iNum, char *dstBuf)
{
	int i, j, k, TmpErr;
	char DestBuf[200]={0};
	
	j=1;/* 分割段数 */
	k=0;/* 目标长度 */
	for(i=0;i<iSrcLen;i++){		/* 遍历字符串 */
		if(iNum == j && sSrcBuf[i] != cTab){
			DestBuf[k]=sSrcBuf[i];
			k ++;
		}
		if(sSrcBuf[i] == cTab){ /* 计算分隔符 */
			j++;
		}
		if(j>iNum){
			break;
		}
	}

	if(i==iSrcLen){
		HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "para error! cTab[%c] iNum[%d] sSrcBuf[%d][%s]",
               cTab, iNum, iSrcLen, sSrcBuf);
		return -1;
	}
	else{
		memcpy(dstBuf, DestBuf, k);
		return k;
	}
}

/*获取系统时间*/
int getSysTime(char* sDateTime)
{
    time_t  lTime;
    struct tm   *tTmLocal;
    char    tDateTime[16];
    
    memset(tDateTime, 0x00, sizeof(tDateTime));
    lTime = time(NULL);
    tTmLocal = localtime(&lTime);
    strftime(tDateTime, sizeof(tDateTime), "%Y%m%d%H%M%S", tTmLocal);
    memcpy(sDateTime, tDateTime, 14);
    return 0;
}

/***************************************************
*函数名称∶ cmdSystem
*函数功能∶ sys执行脚本命令
*输入参数∶
			cmd -脚本命令

*输出参数∶ 无
*函数返回∶  0 -- 成功
            -1 -- 失败
***************************************************/
int cmdSystem(char *cmd)
{
    int nReturnCode = 0;
    sighandler_t old_handler;
    old_handler=signal(SIGCHLD,SIG_DFL);
    nReturnCode = (int)system(cmd);
    signal(SIGCHLD,old_handler);
    return nReturnCode;
}

/* Common.c */

int sendMobileMsg(char* sms_content) {
	return 0;
	tb_sms_send_queue_def tb_sms_send_queue_inf;
	tb_sms_contact_def tb_sms_contact_inf;
	memset(&tb_sms_contact_inf, 0, sizeof(tb_sms_contact_inf));
	// 查询所有运营人员
    DbsSmsSendcontact(DBS_CURSOR, &tb_sms_contact_inf);
    int nReturnCode = DbsSmsSendcontact(DBS_OPEN, &tb_sms_contact_inf);
	if(nReturnCode) {
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "open gmpay_sms_contact err sqlcode[%d].", nReturnCode);
	}

	while(1) {
        nReturnCode = DbsSmsSendcontact(DBS_FETCH, &tb_sms_contact_inf);
		if (nReturnCode && nReturnCode != DBS_NOTFOUND)
        {
		    HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "fetch gmpay_sms_contact err sqlcode[%d].", nReturnCode);
		    DbsSmsSendcontact(DBS_CLOSE, &tb_sms_contact_inf);
            return -1;
        }
		// 所有任务已执行完成
	    else if(nReturnCode == DBS_NOTFOUND) 
	    {
            HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "fetch gmpay_sms_contact null sqlcode[%d].", nReturnCode);
		    DbsSmsSendcontact(DBS_CLOSE, &tb_sms_contact_inf);
			return 0;
		}
		else {
			memset(&tb_sms_send_queue_inf, 0, sizeof(tb_sms_send_queue_inf));
            // 插入短信发送队列表
		    //memcpy(tb_sms_send_queue_inf.term_no,
            //memcpy(tb_sms_send_queue_inf.term_name,
            //memcpy(tb_sms_send_queue_inf.template_no,
            memcpy(tb_sms_send_queue_inf.mob_no_country_code, "86", 2);
	        memcpy(tb_sms_send_queue_inf.mobile_no, tb_sms_contact_inf.mobile, 12);
	        memcpy(tb_sms_send_queue_inf.sms_content, sms_content, strlen(sms_content));
	        //memcpy(tb_sms_send_queue_inf.batch_no,
	        memcpy(tb_sms_send_queue_inf.status, "0", 1);
			nReturnCode = DbsSmsSendQueue(DBS_INSERT, &tb_sms_send_queue_inf);
			if (nReturnCode)
            {
		        HtLog(gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "DbsSmsSendQueue insert err sqlcode[%d].", nReturnCode);
		        DbsSmsSendcontact(DBS_CLOSE, &tb_sms_contact_inf);
                return -1;
            }
		}
    }
    return 0;
}

int GetKeyInf(stKeyInf *pKeyInf, char* payway)
{
    int nReturnCode = 0;
    tbl_key_cert_def tbl_key_cert_inf;
	memset(&tbl_key_cert_inf, 0, sizeof(tbl_key_cert_inf));
	strcpy(tbl_key_cert_inf.pay_way, payway);
    nReturnCode = DbsKeyCertInf(DBS_SELECT, &tbl_key_cert_inf);
	if(nReturnCode) {
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DbsKeyCertInf select err[%d]", nReturnCode);
		return -1;
	}
    CommonRTrim(tbl_key_cert_inf.paying_merc_code); 
    CommonRTrim(tbl_key_cert_inf.key);   
	CommonRTrim(tbl_key_cert_inf.merc_cert);  
    memcpy(pKeyInf->sMrchNo, tbl_key_cert_inf.paying_merc_code,sizeof(tbl_key_cert_inf.paying_merc_code)-1);
    memcpy(pKeyInf->sKey, tbl_key_cert_inf.key,sizeof(pKeyInf->sKey)-1);
	memcpy(pKeyInf->sMrchCert,tbl_key_cert_inf.merc_cert,sizeof(pKeyInf->sMrchCert)-1);

    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "sMrchNo[%s]sKey[%s]sMrchCert[%s]", pKeyInf->sMrchNo,pKeyInf->sKey,pKeyInf->sMrchCert); 
    return 0;
}