/**
 *  base64���롢����ʵ��
 *       C����Դ����
 *
 *   ע�⣺��ʹ��gcc����
 *
 *
 *  ʹ��˵����
 *      �����в���˵�������С�-d����������Ϊbase64���룬����Ϊbase64���롣
 *                      ���С�-o������������ļ��������������׼����ļ���
 *      �������Ա�׼����stdin�����Ϊ��׼���stdout�����ض��������������
 *
 *        base64���룺�������������������ȡ���ļ�������Ϊֹ�����������������ļ���β��Ϊֹ����
 *                    ������ı���base64���롣
 *
 *        base64���룺���봿�ı���base64���룬��ȡ���ļ�������Ϊֹ�����������������ļ���β��Ϊֹ����
 *                    ���ԭ���Ķ���������
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
	//д��־���ļ���
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
