/* ************************************
*《精通Windows API》 
* 示例代码
* ConsoleIO.c
* 9.2  字符界面程序
**************************************/
/* 预编译声明 */
#define _WIN32_WINNT 0x0501 

#include <windows.h>
#include <stdio.h>
#include <conio.h>
/* 外部变量 */
extern CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 
extern HANDLE hStdout, hStdin; 
/* 函数声明 */
VOID MyErrorExit(LPSTR szErrorText);
void NewLine(void) ;

/* ************************************
* void ScrollScreenBuffer(HANDLE h, INT x)
* 功能	滚动屏幕
* 参数	HANDLE h, 控制台句柄；		INT x， 行
**************************************/
void ScrollScreenBuffer(HANDLE h, INT x)
{
	SMALL_RECT srctScrollRect, srctClipRect;
	CHAR_INFO chiFill;
	COORD coordDest;

	srctScrollRect.Left = 0;
	srctScrollRect.Top = 1;
	srctScrollRect.Right = csbiInfo.dwSize.X - x; 
	srctScrollRect.Bottom = csbiInfo.dwSize.Y - x; 

	// 目的
	coordDest.X = 0; 
	coordDest.Y = 0; 

	// left unchanged. 
	srctClipRect = srctScrollRect; 

	// 设置填充的字符串和属性 
	chiFill.Attributes = FOREGROUND_RED|FOREGROUND_INTENSITY; 
	chiFill.Char.AsciiChar = (char)' '; 

	// 滚动一行
	ScrollConsoleScreenBuffer( 
		h,               // 屏幕缓冲区句柄 
		&srctScrollRect, // scrolling rectangle 
		&srctClipRect,   // clipping rectangle 
		coordDest,       // 右上角目的cell 
		&chiFill);       // 填充的字符的颜色
}

/* ************************************
* VOID GetConsoleInfo(HANDLE hOutput)
* 功能	获取控制台信息
* 参数	HANDLE hOutput，控制台句柄
**************************************/
VOID GetConsoleInfo(HANDLE hOutput)
{
	CONSOLE_FONT_INFO cfi;
	DWORD dwProcessList[32];
	DWORD dwAttachedProcess;
	TCHAR szOutputBuffer[1024];
	HWND hConsoleWindows;
	DWORD dwWritten;
	DWORD i;
	CHAR szConsoleTitle[MAX_PATH];
	// 附属的进程，不考虑多于32个的情况
	dwAttachedProcess = GetConsoleProcessList(dwProcessList,32);

	if(dwAttachedProcess==0)
	{
		MyErrorExit("GetConsoleProcessList");
	}
	// 标题
	if( !GetConsoleTitle(szConsoleTitle, MAX_PATH) )
	{
		MyErrorExit("GetConsoleTitle");
	}
	// 窗口句柄
	hConsoleWindows = GetConsoleWindow();
	// 字体
	GetCurrentConsoleFont(hOutput,FALSE,&cfi);

	wsprintf(szOutputBuffer,"Now %u attached Processes: ",dwAttachedProcess);
	for(i=0;i<dwAttachedProcess;i++)
	{
		wsprintf(szOutputBuffer+lstrlen(szOutputBuffer),"%u, ",dwProcessList[i]);
	}
	// 构造字符串
	wsprintf(szOutputBuffer+lstrlen(szOutputBuffer),
		"\nConsoleTitle is %s,\nWindow Handle is 0x%.8X\n"
		"Font is %u, Font Size X = %u, Y = %u\n",
		szConsoleTitle,hConsoleWindows,
		cfi.nFont,cfi.dwFontSize.X,cfi.dwFontSize.Y);
	// 显示获取的信息
	if(!WriteConsole(hOutput,szOutputBuffer,lstrlen(szOutputBuffer),&dwWritten,NULL))
	{
		MyErrorExit("WriteConsole");
	}
}
/* ************************************
* VOID ChangeBackgroudColor(HANDLE hConsole)
* 功能	改变背景颜色
* 参数	HANDLE hOutput，控制台句柄
**************************************/
VOID ChangeBackgroudColor(HANDLE hConsole)
{
	COORD coord;
	WORD wColor;
	DWORD cWritten;
	BOOL fSuccess;
	coord.X = 0; // 第一个Cell
	coord.Y = 0; // 第一行
	// 背景和前景色
	wColor = BACKGROUND_BLUE | BACKGROUND_RED |
		FOREGROUND_RED |
		FOREGROUND_GREEN |
		FOREGROUND_BLUE;
	// 设置控制台属性
	fSuccess = FillConsoleOutputAttribute(
		hConsole,		// Handle
		wColor,			// 颜色
		80*50,			// 填充的Cell数量
		coord,			// 改变属性的第一个Cell
		&cWritten);	// 实际改变的情况

	if (! fSuccess)
		MyErrorExit("FillConsoleOutputAttribute");
}

/* ************************************
* VOID ChangeUpCaseTextBackgroudColor(HANDLE hStdin, HANDLE hStdout)
* 功能	改变文字背景颜色
* 参数	HANDLE hStdin, HANDLE hStdout，控制台输入输出句柄
**************************************/
VOID ChangeUpCaseTextBackgroudColor(HANDLE hStdin, HANDLE hStdout)
{
	DWORD dwLen, i, dwRead, dwWritten;
	WORD wColors[3];
	BOOL fSuccess;
	COORD coord;
	HANDLE hSreenHandle;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	PCHAR lpCharacter = HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,2048);
	coord.X = 0;	// 起始Cell
	coord.Y = 0;
	// 读取OutputCharacter
	if(!ReadConsoleOutputCharacter(hStdout,lpCharacter,2047,coord,&dwRead))
	{
		MyErrorExit("ReadConsoleOutputCharacter");
	}
	// 获取ScreenBufferInfo
	GetConsoleScreenBufferInfo(hStdout,&csbi);

	dwLen = lstrlen(lpCharacter);
	// 颜色数组
	wColors[0] = BACKGROUND_RED;
	wColors[1] = BACKGROUND_RED | BACKGROUND_GREEN;
	wColors[2] = BACKGROUND_GREEN;

	for (i=0 ;i<dwLen; i++)
	{
		// 查找大写字母
		if(lpCharacter[i]>='A'&& lpCharacter[i]<='Z')
		{
			// 如果是大写字母，则从大写字母开始位置的三个Cell的
			// 文字背景改变成三种不同的颜色
			coord.Y = i/csbi.dwSize.X;
			coord.X = i%csbi.dwSize.X;
			fSuccess = WriteConsoleOutputAttribute(
				hStdout, // handle
				wColors, 3, 
				coord, // 起始位置
				&dwWritten);
			if(!fSuccess)
			{
				MyErrorExit("WriteConsoleOutputAttribute");
			}
		}
	}
}

/* ************************************
* VOID ChangeTextColor(HANDLE hStdout,WORD wColor)
* 功能	改变文字颜色
* 参数	HANDLE hStdout,句柄； WORD wColor，新的颜色
**************************************/
VOID ChangeTextColor(HANDLE hStdout, WORD wColor)
{
	// 设置文字属性
	if (! SetConsoleTextAttribute(hStdout, wColor))
	{
		MessageBox(NULL, "SetConsoleTextAttribute", "Console Error",
			MB_OK);
		return;
	}
}

/* ************************************
* VOID ChangeConsoleTitle(LPSTR szNewTitle)
* 功能	改变控制台窗口的标题
* 参数	LPSTR szNewTitle，新标题
**************************************/
VOID ChangeConsoleTitle(LPSTR szNewTitle)
{

	if( !SetConsoleTitle(szNewTitle) )
		MyErrorExit("SetConsoleTitle failed\n");
}

/* ************************************
* void cls( HANDLE hConsole )
* 功能	清屏
* 参数	HANDLE hConsole，句柄
**************************************/
void cls( HANDLE hConsole )
{
	COORD coordScreen = { 0, 0 }; // 鼠标指针位置
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;

	// 当前Buffer中的文字Cell数目
	if( !GetConsoleScreenBufferInfo( hConsole, &csbi ))
		return;
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	// 将整个屏幕填充
	if( !FillConsoleOutputCharacter( hConsole, (TCHAR) ' ',
		dwConSize, coordScreen, &cCharsWritten ))
		return;

	// 获取文字属性
	if( !GetConsoleScreenBufferInfo( hConsole, &csbi ))
		return;

	// 设置buffer属性
	if( !FillConsoleOutputAttribute( hConsole, csbi.wAttributes,
		dwConSize, coordScreen, &cCharsWritten ))
		return;

	// 将鼠标指针移动起始位置
	SetConsoleCursorPosition( hConsole, coordScreen );
}

/* ************************************
* VOID ChangeMode( HANDLE hStdin, HANDLE hStdout)
* 功能	改变控制台的模式，关闭ENABLE_LINE_INPUT和ENABLE_ECHO_INPUT
* 参数	HANDLE hStdin, HANDLE hStdout，句柄
**************************************/
VOID ChangeMode( HANDLE hStdin, HANDLE hStdout)
{
		LPSTR lpszPrompt = "Mode changeed Type any key, or q to quit: ";
		CHAR chBuffer[256];

	DWORD fdwMode, fdwOldMode;
	DWORD cRead,cWritten;
	// 获取当前模式
	if (! GetConsoleMode(hStdin, &fdwOldMode)) 
	{
		MessageBox(NULL, "GetConsoleMode", "Console Error", MB_OK); 
		return;
	}
	// 修改模式并重新设置
	fdwMode = fdwOldMode & 
		~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT); 
	if (! SetConsoleMode(hStdin, fdwMode)) 
	{
		MessageBox(NULL, "SetConsoleMode", "Console Error", MB_OK); 
		return;
	}
	// 换行
	NewLine();
	// 循环等待输入并处理
	while (1) 
	{ 
		if (! WriteFile( hStdout, 
			lpszPrompt,  lstrlen(lpszPrompt), 
			&cWritten, NULL) )
		{
			MyErrorExit("WriteFile");
			return;
		}

		if (! ReadFile(hStdin, chBuffer, 1, &cRead, NULL)) 
			break; 
		if (chBuffer[0] == '\r')
			NewLine();
		else if (! WriteFile(hStdout, chBuffer, cRead, 
			&cWritten, NULL)) break;
		else
			NewLine();
		if (chBuffer[0] == 'q') break; // 输入q，退出 
	} 
	// 恢复模式
	SetConsoleMode(hStdin, fdwOldMode);
}

/* ************************************
* void NewLine(void) 
* 功能	新的一行，滚动屏幕
**************************************/
void NewLine(void) 
{ 
	if (! GetConsoleScreenBufferInfo(hStdout, &csbiInfo)) 
	{
		MessageBox(NULL, "GetConsoleScreenBufferInfo", 
			"Console Error", MB_OK); 
		return;
	}

	csbiInfo.dwCursorPosition.X = 0; 

	if ((csbiInfo.dwSize.Y-1) == csbiInfo.dwCursorPosition.Y) 
	{ 
		ScrollScreenBuffer(hStdout, 1); 
	} 

	else csbiInfo.dwCursorPosition.Y += 1; 

	if (! SetConsoleCursorPosition(hStdout, 
		csbiInfo.dwCursorPosition)) 
	{
		MessageBox(NULL, "SetConsoleCursorPosition", "Console Error", 
			MB_OK); 
		return;
	}
} 

/* ************************************
* VOID UseBlockIO()
* 功能	使用Block IO
**************************************/
VOID UseBlockIO()
{
	HANDLE hStdout, hNewScreenBuffer;
	SMALL_RECT srctReadRect;
	SMALL_RECT srctWriteRect;
	CHAR_INFO chiBuffer[160]; // [2][80];
	COORD coordBufSize;
	COORD coordBufCoord;
	BOOL fSuccess;

	// 获得句柄
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	// 新建buffer
	hNewScreenBuffer = CreateConsoleScreenBuffer(
		GENERIC_READ | 	GENERIC_WRITE,// 可读可写
		0, // 不共享
		NULL, // 默认安全属性
		CONSOLE_TEXTMODE_BUFFER, // must be TEXTMODE
		NULL); 
	if (hStdout == INVALID_HANDLE_VALUE ||
		hNewScreenBuffer == INVALID_HANDLE_VALUE)
	{
		printf("CreateConsoleScreenBuffer (%d)\n", GetLastError());
		return;
	}

	// 设置屏幕的Buffer
	if (! SetConsoleActiveScreenBuffer(hNewScreenBuffer) )
	{
		printf("SetConsoleActiveScreenBuffer (%d)\n", GetLastError());
		return;
	}

	// 设置源Cell矩阵
	srctReadRect.Top = 0; // top left: row 0, col 0
	srctReadRect.Left = 0;
	srctReadRect.Bottom = 1; // bot. right: row 1, col 79
	srctReadRect.Right = 79;

	// 临时buffer大小 2 rows x 80 columns.
	coordBufSize.Y = 2;
	coordBufSize.X = 80;
	// 临时buffer的起始位置
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;

	// 从屏幕buffer复制内容到临时buffer
	fSuccess = ReadConsoleOutput(
		hStdout, // 源
		chiBuffer, // 目的
		coordBufSize, // 目的buffer大小
		coordBufCoord, // 目的起始位置
		&srctReadRect); // 源起始位置
	if (! fSuccess)
	{
		printf("ReadConsoleOutput (%d)\n", GetLastError());
		return;
	}

	// 设置目的Cells矩阵
	srctWriteRect.Top = 10; // top lt: row 10, col 0
	srctWriteRect.Left = 0;
	srctWriteRect.Bottom = 11; // bot. rt: row 11, col 79
	srctWriteRect.Right = 79;

	// 从临时buffer向新的屏幕buffer复制
	fSuccess = WriteConsoleOutput(
		hNewScreenBuffer, 
		chiBuffer,
		coordBufSize, 
		coordBufCoord, 
		&srctWriteRect); 
	if (! fSuccess)
	{
		printf("WriteConsoleOutput (%d)\n", GetLastError());
		return;
	}
	// 等等一段时间
	Sleep(10000);

	// 恢复
	if (! SetConsoleActiveScreenBuffer(hStdout))
	{
		printf("SetConsoleActiveScreenBuffer (%d)\n", GetLastError());
		return;
	}
}


VOID MyErrorExit(LPSTR szErrorText)
{
	LPVOID szShow = HeapAlloc(GetProcessHeap(),
		HEAP_NO_SERIALIZE,lstrlen(szErrorText)+32);
	wsprintf(szShow,"ERROR; %s\nLastErrorCode (%u)",szErrorText,GetLastError());
	MessageBox(NULL,szShow,"ERROR",MB_OK);
	HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, szShow);
	ExitProcess(0);
}
