/* ************************************
*《精通Windows API》 
* 示例代码
* mlclt.c
* 11.1 通过mailslot进程间通信
**************************************/
/* 头文件 */
#include <windows.h>
#include <stdio.h>
/* 全局变量 */
HANDLE hSlot;
LPTSTR lpszSlotName = TEXT("\\\\.\\mailslot\\sample_mailslot");		// mailslot名
LPTSTR lpszMessage = TEXT("Test Message for mailslot "); // 通信的内容

/* ************************************
* void main()
* 功能	进程间mailslot通信客户端
**************************************/
void main()
{ 
	BOOL fResult; 
	HANDLE hFile; 
	DWORD cbWritten; 

	DWORD cbMessage;
	// 打开mailslot
	hFile = CreateFile(lpszSlotName, 
		GENERIC_WRITE,		// 可写
		FILE_SHARE_READ,
		(LPSECURITY_ATTRIBUTES) NULL, 
		OPEN_EXISTING,		// 打开一个已经存在的mailslot，应该由服务端已经创建
		FILE_ATTRIBUTE_NORMAL, 
		(HANDLE) NULL); 

	if (hFile == INVALID_HANDLE_VALUE) 
	{ 
		printf("CreateFile failed with %d.\n", GetLastError()); 
		return ; 
	}
	// 向mailslot写入
	fResult = WriteFile(hFile, 
		lpszMessage, 
		(DWORD) (lstrlen(lpszMessage)+1)*sizeof(TCHAR),  
		&cbWritten, 
		(LPOVERLAPPED) NULL); 

	if (!fResult) 
	{ 
		printf("WriteFile failed with %d.\n", GetLastError()); 
		return ; 
	} 
	// 结束
	printf("Slot written to successfully.\n"); 
	CloseHandle(hFile); 
	return ;
}