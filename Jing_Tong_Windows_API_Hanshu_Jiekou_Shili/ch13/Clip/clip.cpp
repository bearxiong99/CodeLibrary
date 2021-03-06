/* ************************************
*《精通Windows API》
* 示例代码
* clip.cpp
* 13.8 区域、路径、修剪
**************************************/
/* 头文件 */
#include <Windows.h>
#include <math.h>
#include "resource.h"
/* 全局变量 */
CHOOSEFONT cf; // 选择字体
LOGFONT lf; // 逻辑字体
HFONT hfont; // 字体对象
HFONT hfontOld; // DC的原有字体
HDC hdc; // DC句柄
int nXStart, nYStart; // 用于画直线
RECT rc; // 窗口客户区RECT
double aflSin[90]; // 用于绘制放射直线簇
double aflCos[90]; // 用于绘制放射状直线簇
double flRadius,a; // 半径
int iMode; // 修剪路径组合模式
HRGN hrgn; // 修剪区域
int i;
COLORREF crOld;

// 窗口消息处理函数
LRESULT APIENTRY MainWndProc(
							 HWND hwnd,
							 UINT message,
							 WPARAM wParam,
							 LPARAM lParam)

{

	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;

	case WM_COMMAND: // 菜单命令
		switch (wParam)
		{
		case IDM_VANISH: // 消除客户区（在设置了修剪全其输出范围也会被限制）
			hdc = GetDC(hwnd);
			GetClientRect(hwnd, &rc);
			FillRect(hdc, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
			DeleteDC( hdc);
			break;
			// 根据用户的菜单输入，设置修剪路径的组合模式
		case IDM_AND:
			iMode = RGN_AND;
			break;
		case IDM_COPY:
			iMode = RGN_COPY;
			break;
		case IDM_DIFF:
			iMode = RGN_DIFF;
			break;
		case IDM_OR:
			iMode = RGN_OR;
			break;
		case IDM_XOR:
			iMode = RGN_XOR;
			break;

		case IDM_CLIP:
			{
				// 获取窗口客户区的DC
				hdc = GetDC(hwnd);
				// 创建并为DC选择字体
				// 消息在菜单中点击此项前需先选择字体
				hfont = CreateFontIndirect(cf.lpLogFont);
				hfontOld = (HFONT)SelectObject(hdc, hfont);

				// 五个点，用于创建五角星区域
				POINT point[5]= {{0,200},{600,200},{100,600},{300,0},{500,600}};
				// 创建多边形区域，五角星
				hrgn = CreatePolygonRgn(point, 5, WINDING);
				// 将区域选择为修剪区域
				SelectClipRgn(hdc, hrgn);
				// 输出的文字
				LPSTR szOut = "Lorem ipsum dolor sit amet, consectetur \n"
					"adipisicing elit, sed do eiusmod tempor \n"
					"incididunt ut labore et dolore magna \n"
					"aliqua. Ut enim ad minim veniam, quis \n"
					"nostrud exercitation ullamco laboris nisi \n"
					"ut aliquip ex ea commodo consequat. Duis \n"
					"aute irure dolor in reprehenderit in \n"
					"voluptate velit esse cillum dolore eu \n"
					"fugiat nulla pariatur. Excepteur sint \n"
					"occaecat cupidatat non proident, sunt \n"
					"in culpa qui officia deserunt mollit \n"
					"anim id est laborum.\n";
				// 窗口客户区，用于输出文字
				RECT rect;
				GetClientRect(hwnd,&rect);
				// 设置文字背景为透明
				SetBkMode(hdc, TRANSPARENT);
				// 开始创建路径
				BeginPath(hdc);
				// 路径的形状为输出的文字
				DrawText(hdc, szOut, lstrlen(szOut),&rect , DT_CENTER);
				EndPath(hdc);// 路径已经为DC的当前路径
				// 为DC选择修剪路径，使用用户通过菜单输入的模式
				// 注意在进行此操作前需通过菜单选择组合模式
				SelectClipPath(hdc, iMode);

				// 输出放射状直线，以左上角为原点
				// 计算线的终点
				for (i = 0; i < 90; i++)
				{
					aflSin[i] = sin( (((double)i) / 180.0)
						* 3.14159);
				}
				for (i = 0; i < 90; i++)
				{
					aflCos[i] = cos( (((double)i) / 180.0)
						* 3.14159);
				}
				flRadius = 1000;// 线的长度（窗口大小为600*650）
				// 画线，第一度画一条线
				for (i = 0; i < 90; i++)
				{
					MoveToEx(hdc, nXStart, nYStart,
						(LPPOINT) NULL);
					LineTo(hdc, nXStart + ((int) (flRadius
						* aflCos[i])),
						nYStart + ((int) (flRadius
						* aflSin[i])));
				}
				// 选择回字体，释放
				SelectObject(hdc, hfontOld);
				DeleteObject(hfont);
				DeleteDC( hdc);
				// 刷新窗口
				UpdateWindow(hwnd);

				break;
			}

		case IDM_FONT:
			// 用户选择字体
			cf.lStructSize = sizeof (CHOOSEFONT);
			cf.hwndOwner = hwnd;
			cf.lpLogFont = &lf;
			cf.Flags = CF_SCREENFONTS | CF_EFFECTS;
			cf.rgbColors = RGB(0, 255, 255);
			cf.nFontType = SCREEN_FONTTYPE;

			ChooseFont(&cf);
			break;

		default:
			return DefWindowProc(hwnd, message, wParam,
				lParam);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

// WinMain
int WINAPI WinMain(
				   HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
	MSG msg;
	HWND hWnd;
	WNDCLASS wc;


	wc.style = CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU_CLIP);//菜单
	wc.lpszClassName = "clip";

	if (!RegisterClass(&wc))
	{
		return (FALSE);
	}

	hWnd = CreateWindow(
		"clip",
		"clip",
		WS_OVERLAPPEDWINDOW,
		100, 100, 600, 650,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (!hWnd)
	{
		return (FALSE);
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)(msg.wParam);

	UNREFERENCED_PARAMETER(lpCmdLine);
}