/**
 *  base64编码、解码实现
 *       C语言源代码
 *
 *   注意：请使用gcc编译
 *
 *
 *  使用说明：
 *      命令行参数说明：若有“-d”参数，则为base64解码，否则为base64编码。
 *                      若有“-o”参数，后接文件名，则输出到标准输出文件。
 *      输入来自标准输入stdin，输出为标准输出stdout。可重定向输入输出流。
 *
 *        base64编码：输入任意二进制流，读取到文件读完了为止（键盘输入则遇到文件结尾符为止）。
 *                    输出纯文本的base64编码。
 *
 *        base64解码：输入纯文本的base64编码，读取到文件读完了为止（键盘输入则遇到文件结尾符为止）。
 *                    输出原来的二进制流。
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

#define LOG_ALLOC_GRAN			(1024)

char *m_pData;
int m_nAllocLen;
int m_nDataLen;

int IntDate()
{
	m_nAllocLen = 0;
	m_pData = NULL;
	m_nDataLen = 0;
	m_nAllocLen = LOG_ALLOC_GRAN;
	m_pData = malloc (m_nAllocLen);
	m_nDataLen = 0;	  
}

void RawLog(const char *pStr, int nLen)
{
	if (nLen < 0)
		nLen = strlen(pStr);
	if (m_nDataLen + nLen > m_nAllocLen)
	{
		int nNewAllocLen = m_nAllocLen * 2;
		if (nNewAllocLen < m_nDataLen + nLen)
		{
			nNewAllocLen = (m_nDataLen + nLen + LOG_ALLOC_GRAN - 1) 
				/ LOG_ALLOC_GRAN * LOG_ALLOC_GRAN;
		}
		char *pNew = malloc (nNewAllocLen);
		if (m_nDataLen > 0)
			memcpy(pNew, m_pData, m_nDataLen);
		free (m_pData);
		m_pData = pNew;
		m_nAllocLen = nNewAllocLen;
	}
	memcpy(m_pData + m_nDataLen, pStr, nLen);
	m_nDataLen += nLen;
}

void FlushLog()
{
	//写日志到文件中
	int nDataLen = m_nDataLen;
    //printf("nDataLen[%d]\n",nDataLen);
    //printf("m_pData[%s]\n",m_pData);
}

Reset()
{
	if (m_pData)
	{
		free (m_pData);
		m_pData = NULL;
	}       
}


int main(int argc, char * argv[])
{
    char buf[10]={"1234"};
    IntDate();
    RawLog(buf, strlen(buf));
    FlushLog();
    Reset();
    return 0;
}
