/* ************************************
*《精通Windows API》 
* 示例代码
* Fibers.c
* 6.3.4  创建、删除纤程、调度纤程
* 6.3.5  纤程与线程的互相转换
**************************************/
/* 预定义　*/
#define _WIN32_WINNT 0x0501
/* 头文件　*/
#include <windows.h>
#include <stdio.h>
/* 函数声明　*/
VOID WINAPI ReadFiberFunc( LPVOID lpParameter );
VOID WINAPI WriteFiberFunc( LPVOID lpParameter );
/* 结构定义　*/
// 用于向纤程传递参数
// 本实例是使用读、写文件来演示纤程的调度
// 用户可根据实际情况自行定义
typedef struct _FIBERDATASTRUCT
{
	DWORD dwParameter;		// 预留给向纤程传递待定参数
	DWORD dwFiberResultCode;	// GetLastError() 值
	HANDLE hFile;				// 纤程所操作文件的句柄
	DWORD dwBytesProcessed;		// 已经处理了的字节
}FIBERDATASTRUCT, *LPFIBERDATASTRUCT;
/* 常量定义　*/
#define RTN_OK 0			// 返回值 成功
#define RTN_USAGE 1			// 返回值 参数不正确
#define RTN_ERROR 2		// 返回值 错误
#define BUFFER_SIZE 32768	// 缓冲区大小
#define FIBER_COUNT 3		// 主纤程、读纤程、写纤程，共三个
#define PRIMARY_FIBER 0		// 主纤程
#define READ_FIBER 1		// 读纤程
#define WRITE_FIBER 2		// 写纤程

LPVOID g_lpFiber[FIBER_COUNT];	// 纤程地址的数组
LPBYTE g_lpBuffer;				// 缓冲区
DWORD g_dwBytesRead;			// 已读的字节

int main( int argc, char *argv[] )
{
	LPFIBERDATASTRUCT fs;
	// 用法说明
	if (argc != 3)
	{
		printf("Usage: %s <SourceFile> <DestinationFile>\n", argv[0]);
		return RTN_USAGE;
	}
	// 分配FIBERDATASTRUCT空间，FIBER_COUNT个
	fs = (LPFIBERDATASTRUCT)HeapAlloc(
		GetProcessHeap(), 0,
		sizeof(FIBERDATASTRUCT) * FIBER_COUNT);
	if (fs == NULL)
	{
		printf("HeapAlloc error! (rc%=lu)\n", GetLastError());
		return RTN_ERROR;
	}
	//	分配读、写缓冲区
	g_lpBuffer = (LPBYTE)HeapAlloc(GetProcessHeap(), 0, BUFFER_SIZE);
	if (g_lpBuffer == NULL)
	{
		printf("HeapAlloc error! (rc=%lu)\n", GetLastError());
		return RTN_ERROR;
	}
	// 打开源文件，将句柄赋值给fs结构的hFile成员，使纤程可以使用该句柄
	fs[READ_FIBER].hFile = CreateFile(
		argv[1], GENERIC_READ,
		FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN, NULL
		);
	if (fs[READ_FIBER].hFile == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile error! (rc=%lu)\n", GetLastError());
		return RTN_ERROR;
	}
	// 打开目标文件
	fs[WRITE_FIBER].hFile = CreateFile(
		argv[2], GENERIC_WRITE,
		0, NULL, CREATE_NEW,
		FILE_FLAG_SEQUENTIAL_SCAN, NULL
		);
	if (fs[WRITE_FIBER].hFile == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile error! (rc=%lu)\n", GetLastError());
		return RTN_ERROR;
	}

	// 将主线程切换为纤程，为主纤程，只有转换为纤程后才可以切换至其他纤程
	g_lpFiber[PRIMARY_FIBER]=ConvertThreadToFiber(&fs[PRIMARY_FIBER]);
	if (g_lpFiber[PRIMARY_FIBER] == NULL)
	{
		printf("ConvertThreadToFiber failed! rc=%lu\n", GetLastError());
		return RTN_ERROR;
	}
	// 主纤程数据
	fs[PRIMARY_FIBER].dwParameter = 0;
	fs[PRIMARY_FIBER].dwFiberResultCode = 0;
	fs[PRIMARY_FIBER].hFile = INVALID_HANDLE_VALUE;
	// 创建读纤程
	g_lpFiber[READ_FIBER]=CreateFiber(0,ReadFiberFunc,&fs[READ_FIBER]);
	if (g_lpFiber[READ_FIBER] == NULL)
	{
		printf("CreateFiber error! (rc=%lu)\n", GetLastError());
		return RTN_ERROR;
	}
	// 将纤程指针作为参数传给纤程，没有实际意义，为了显示相关信息时区别各纤程
	fs[READ_FIBER].dwParameter = (DWORD)g_lpFiber[READ_FIBER];
	// 创建写纤程
	g_lpFiber[WRITE_FIBER]=CreateFiber(0,WriteFiberFunc,&fs[WRITE_FIBER]);

	if (g_lpFiber[WRITE_FIBER] == NULL)
	{
		printf("CreateFiber error! (rc=%lu)\n", GetLastError());
		return RTN_ERROR;
	}
	fs[WRITE_FIBER].dwParameter = (DWORD)g_lpFiber[WRITE_FIBER];
	// 切换到读程序执行
	SwitchToFiber(g_lpFiber[READ_FIBER]);

	// 由读纤程获写纤程切换回主纤程
	// 显示相关信息
	printf("ReadFiber result == %lu Bytes Processed == %lu\n",
		fs[READ_FIBER].dwFiberResultCode, fs[READ_FIBER].dwBytesProcessed);

	printf("WriteFiber result == %lu Bytes Processed == %lu\n",
		fs[WRITE_FIBER].dwFiberResultCode, fs[WRITE_FIBER].dwBytesProcessed);
	// 删除读写纤程
	DeleteFiber(g_lpFiber[READ_FIBER]);
	DeleteFiber(g_lpFiber[WRITE_FIBER]);
	// 关闭文件句柄、释放内存、返回
	CloseHandle(fs[READ_FIBER].hFile);
	CloseHandle(fs[WRITE_FIBER].hFile);
	HeapFree(GetProcessHeap(), 0, g_lpBuffer);
	HeapFree(GetProcessHeap(), 0, fs);
	return RTN_OK;
}

VOID WINAPI ReadFiberFunc( LPVOID lpParameter )
{
	LPFIBERDATASTRUCT fds = (LPFIBERDATASTRUCT)lpParameter;

	// 判断参数
	if (fds == NULL)
	{
		printf("Passed NULL fiber data. Exiting current thread.\n");
		return;
	}
	// 显示纤程信息
	printf("Read Fiber (dwParameter == 0x%lx)\n", fds->dwParameter);
	// 初始化处理的字节数为0
	fds->dwBytesProcessed = 0;
	// 循环读
	while (1)
	{
		if (!ReadFile(fds->hFile, g_lpBuffer, BUFFER_SIZE,
			&g_dwBytesRead, NULL))
		{
			break;
		}
		// 判断文件是否已经读完
		if (g_dwBytesRead == 0) break;

		// 已经处理的字节，累加
		fds->dwBytesProcessed += g_dwBytesRead;

		// 读一次后切换到写纤程，将读出的数据写入到目标文件
		printf("Switch To Write");
		SwitchToFiber(g_lpFiber[WRITE_FIBER]);
	}
	// 读操作完成，准备交出执行，返回到主纤程中
	fds->dwFiberResultCode = GetLastError();
	SwitchToFiber(g_lpFiber[PRIMARY_FIBER]);
}

VOID WINAPI WriteFiberFunc( LPVOID lpParameter )
{
	LPFIBERDATASTRUCT fds = (LPFIBERDATASTRUCT)lpParameter;
	DWORD dwBytesWritten;

	// 判断参数
	if (fds == NULL)
	{
		printf("Passed NULL fiber data. Exiting current thread.\n");
		return;
	}
	// 显示纤程信息
	printf("Write Fiber (dwParameter == 0x%lx)\n", fds->dwParameter);
	// 初始化，注意和读纤程的不同
	fds->dwBytesProcessed = 0;
	fds->dwFiberResultCode = ERROR_SUCCESS;

	while (1)
	{
		// 写入数据
		if (!WriteFile(fds->hFile, g_lpBuffer, g_dwBytesRead,
			&dwBytesWritten, NULL))
		{
			// 如果发生错误，退出循环
			break;
		}
		fds->dwBytesProcessed += dwBytesWritten;

		// 写入完成，切换到读纤程
		printf("Switch To Write");
		SwitchToFiber(g_lpFiber[READ_FIBER]);
	}
	// 出错，切换到主纤程
	// 如果写操作不出错，是不可能由写纤程切换回主纤程的
	fds->dwFiberResultCode = GetLastError();
	SwitchToFiber(g_lpFiber[PRIMARY_FIBER]);
}