/* ************************************
*����ͨWindows API��
* ʾ������
* handle_path.cpp
* 4.4.3	ͨ���ļ������ȡ�ļ�·��
**************************************/
/* ͷ�ļ�*/
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include <psapi.h>
/* Ԥ��������*/
#pragma comment (lib, "Psapi.lib")
#define BUFSIZE 512
/* ��������*/
BOOL GetFileNameFromHandle(HANDLE hFile) ;

/* ************************************
* BOOL GetFileNameFromHandle(HANDLE hFile) 
* ����	���ļ������ȡ�ļ�·��
*
* ����	ANDLE hFile����Ҫ���·�����ļ����
*
* ����ֵBOOL �Ƿ�ɹ���
**************************************/
BOOL GetFileNameFromHandle(HANDLE hFile) 
{
	TCHAR pszFilename[MAX_PATH+1];
	HANDLE hFileMap;
	PVOID pMem;

	// ����ļ���С,�������Ƿ�Ϊ
	DWORD dwFileSizeHigh = 0;
	DWORD dwFileSizeLow = GetFileSize(hFile, &dwFileSizeHigh); 
	if( dwFileSizeLow == 0 && dwFileSizeHigh == 0 )
	{
		printf("����map�ļ���СΪ���ļ�.\n");
		return FALSE;
	}
	// ����mapping����
	hFileMap = CreateFileMapping(hFile, 
		NULL, 
		PAGE_READONLY,
		0, 
		1,
		NULL);
	if (!hFileMap) 
	{
		printf("CreateFileMapping error: %d",GetLastError());
		return FALSE;
	}

	pMem = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 1);
	if (!pMem) 
	{
		printf("MapViewOfFile error: %d",GetLastError());
		return FALSE;
	}
	// ��mapping�������ļ���
	if (0 == GetMappedFileName (GetCurrentProcess(), 
		pMem, 
		pszFilename,
		MAX_PATH))
	{
		printf("GetMappedFileName error: %d",GetLastError());
		return FALSE;
	}
	// ���豸��ת��Ϊ·��
	TCHAR szTemp[BUFSIZE] = {0};
	if (0 == GetLogicalDriveStrings(BUFSIZE-1, szTemp)) 
	{
		printf("GetLogicalDriveStrings error: %d",GetLastError());
		return FALSE;
	}
	TCHAR szName[MAX_PATH];
	TCHAR szDrive[3] = {0};
	BOOL bFound = FALSE;
	PTCHAR p = szTemp;
	do 
	{
		CopyMemory(szDrive,p,2*sizeof(TCHAR));
		// ͨ��·�������豸��
		if (!QueryDosDevice(szDrive, szName, BUFSIZE))
		{
			printf("QueryDosDevice error: %d",GetLastError());
			return FALSE;
		}
		UINT uNameLen = lstrlen(szName);
		if (uNameLen < MAX_PATH) 
		{
			//�Ƚ����������豸�����ļ��豸���Ƿ�ƥ��
			bFound = strncmp(pszFilename, szName,uNameLen) == 0;
			if (bFound) 
			{
				//���ƥ�䣬˵���Ѿ��ҵ�������·����
				TCHAR szTempFile[MAX_PATH];
				wsprintf(szTempFile,
					TEXT("%s%s"),
					szDrive,
					pszFilename+uNameLen);
				lstrcpy(pszFilename, szTempFile);
			}
		}
		// ѭ������һ��NULL
		while (*p++);
	} while (!bFound && *p);

	UnmapViewOfFile(pMem);
	CloseHandle(hFileMap);
	printf("File path is %s\n", pszFilename);
	return TRUE;
}

/* ************************************
* int main()
* ����	���ҵ�һ��Ŀ¼�е�һ��txt�ļ�
*		���ļ����������ļ��������ļ�·����
*
* ����	δʹ��
*
* ����ֵ0��ʾ�ɹ�����ʾʧ�ܡ�
**************************************/
int main()
{
	HANDLE hFile;
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	hFind = FindFirstFile("*.txt",&wfd);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		printf("can not find a file");
		return 1;
	}
	//CloseHandle(hFind);
	printf("find %s at current dir\n",wfd.cFileName);
	hFile = CreateFile(wfd.cFileName, 
		GENERIC_READ | GENERIC_WRITE,
		0, 
		NULL,
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		printf("create file error, %d",GetLastError());
	}
	else
	{
		GetFileNameFromHandle(hFile) ;
	}
	CloseHandle(hFile);
	return 0;
}