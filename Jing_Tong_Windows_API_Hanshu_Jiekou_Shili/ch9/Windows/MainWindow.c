/* ************************************
*����ͨWindows API�� 
* ʾ������
* MainWindow.c
* 9.3-9.6  ͼ���û�����
**************************************/
/* ͷ�ļ� */
#include <windows.h>
#include <commctrl.h>
#include <Winuser.h>
#include <shellapi.h>
#include "resource.h"
#include "Window.h"
/* �ⲿ���� */
extern HINSTANCE hinst;
extern HWND hwndMain;
extern HWND hwndTreeView;
extern HWND hwndListView;
extern HWND hwndEdit;
extern HMENU hMenuMain;
extern HMENU hPopup;
extern LPSTR lpszLatin; 

/* ************************************
* WinMain
* ����	������ڵ㣬����InitApplication��InitInstance
*			��Ϣѭ��
* ����	hinstance��Ӧ�ó��򱾴�����ʵ��
*			hPrevInstance��Ӧ�ó���֮ǰ��ʵ����ʼ��ΪNULL
*			lpCmdLine�������в���
*			nCmdShow��������ڣ�SW_SHOW
* ����ֵ	ʧ�ܷ���FALSE
**************************************/
int WINAPI WinMain(
				   HINSTANCE hinstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	BOOL fGotMessage;
	//	 InitApplication
	if (!InitApplication(hinstance))
	{
		return FALSE;
	}
	//	 InitInstance
	if (!InitInstance(hinstance, nCmdShow))
	{
		return FALSE;
	}
	// ��Ϣѭ��
	while ((fGotMessage = GetMessage(&msg, (HWND) NULL, 0, 0)) != 0 && fGotMessage != -1)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
	UNREFERENCED_PARAMETER(lpCmdLine);
}
/* ************************************
* BOOL InitApplication(HINSTANCE hinstance)
* ����	ע����������
* ����	hinstance��Ӧ�ó��򱾴�����ʵ��
* ����ֵ	�Ƿ�ɹ�
**************************************/
BOOL InitApplication(HINSTANCE hinstance)
{
	// ʹ��RegisterClassEx
	WNDCLASSEX wcx;
	// ���ṹ
	wcx.cbSize = sizeof(wcx); // WNDCLASSEX�ṹ�Ĵ�С
	wcx.style = CS_HREDRAW | CS_VREDRAW; // �����С�ı����ػ洰��
	wcx.lpfnWndProc = MainWndProc; // ������Ϣ��������
	wcx.cbClsExtra = 0; // �޸��Ӵ������ڴ�
	wcx.cbWndExtra = 0; // �޸��Ӵ����ڴ�
	wcx.hInstance = hinstance; // Ӧ�ó���ʵ��
	wcx.hIcon = LoadIcon(NULL,	MAKEINTRESOURCE(IDI_ICON_MAIN)); // ͼ��
	wcx.hCursor = LoadCursor(NULL,	IDC_ARROW); // ���ָ��
	wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // ������ˢ
	wcx.lpszMenuName = MAKEINTRESOURCE(IDR_MENU_MAIN); // �˵���Դ
	wcx.lpszClassName = "MainWClass"; // ��������
	wcx.hIconSm = (HICON)LoadImage(hinstance, // Сͼ��
		MAKEINTRESOURCE(IDI_ICON_MAIN),
		IMAGE_ICON,
		GetSystemMetrics(SM_CXSMICON),
		GetSystemMetrics(SM_CYSMICON),
		LR_DEFAULTCOLOR);
	// ע�ᴰ���࣬����
	return RegisterClassEx(&wcx);
}

/* ************************************
* BOOL InitInstance(HINSTANCE hinstance, int nCmdShow)
* ����	���������ںͿؼ�
* ����	hinstance��Ӧ�ó��򱾴�����ʵ��
*			nCmdShow�������ʾ
* ����ֵ	�Ƿ�ɹ�
**************************************/
BOOL InitInstance(HINSTANCE hinstance, int nCmdShow)
{
	HWND hwnd;
	RECT rect;
	// ����Ӧ�ó���ʵ�����
	hinst = hinstance;
	hwnd = CreateWindow(
		"MainWClass", // ����������ʹ��֮ǰע�����������
		"Let's Smile", // ����������ʾ�ڴ��ڱ������ϵ��ַ���
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS |
		WS_POPUPWINDOW, 	// ������ʽ
		CW_USEDEFAULT, // ˮƽλ�ã�Ĭ�ϣ�
		CW_USEDEFAULT, // ��ֱλ�ã�Ĭ�ϣ�
		800, // ��
		600, // ��
		(HWND) NULL, // �޸�����
		(HMENU) LoadMenu(hinst,MAKEINTRESOURCE(IDR_MENU_MAIN)), // �˵�
		hinstance, //Ӧ�ó���ʵ��
		(LPVOID) NULL); // �޴��ڴ�������
	// �����Ƿ񴴽��ɹ�
	if (!hwnd)
		return FALSE;
	// �������ھ��
	hwndMain = hwnd;
	// ��֤ͨ�ÿؼ���̬���ӿ��Ѿ�����
	InitCommonControls();
	// �������ֿؼ����Ӵ���
	hwndTreeView = CreateTreeView(hwndMain, "files");
	hwndListView = CreateListView(hwndMain,"processing");
	hwndEdit = CreateEdit(hwndMain,"texta");
	// ��ȡ�����ڿͻ�����RECT�����η�����ĸ��߽�㣩
	GetClientRect(hwndMain,&rect);
	// �����Ӵ��ڵĴ�С��λ��
	SetWindows(&rect);
	// ��EDIT�ؼ�����ʾ�ı�
	ShowTextOnEdit(lpszLatin);
	// ��ʾ�����ڣ�ʹ��WinMain�������õĴ�����ʾ��ʽ
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	return TRUE;
}
/* ************************************
* MainWndProc
* ����	��������Ϣ��������
**************************************/
LRESULT CALLBACK MainWndProc(
							 HWND hwnd, // ���ھ��
							 UINT uMsg, // ��Ϣ��ʶ��
							 WPARAM wParam, // ��Ϣ�ĵ�һ������
							 LPARAM lParam)// ��Ϣ�ĵڶ�������
{

	switch (uMsg)
	{
	case WM_CREATE:	// ���ڴ���ʱ�յ�����Ϣ
		break;

	case WM_PAINT:		// ���ڱ�����ʱ�յ�����Ϣ
		break;

	case WM_SIZE:		// ���ڴ�С�ı�ʱ
		OnWindowResize();
		break;
		// NOTIFY��ͨ���ɿؼ����͸������ڣ�˵���ؼ����ڽ���ĳ���
	case WM_NOTIFY:
		OnChildWindowsNotify((PVOID)lParam);
		break;
		// �������٣�������ϽǵĹرհ�ť�ᴥ������Ϣ
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
		// �������룬�˵��ѡ�У���ť�����
	case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				// �ڡ��������˵���ѡ�񡰹��ڡ�
			case ID_HELP_ABOUT:
				{
					DialogBox(
						hinst, (LPCTSTR)IDD_DIALOG_ABOUT, hwnd, (DLGPROC)About
						);
					return 0;
				}
			default:
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}
		}
		// �����ڴ˴���������Ϣ
	default:
		break;
	}
	// �кܶ���Ϣδ����������Ҫ��Ĭ�ϴ�����Ϣ��������������
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
/* ************************************
* BOOL OnWindowResize()
* ����	�������ڴ�С�ı���Ϣ
*			�ı��Ӵ��ڴ�С��λ��
**************************************/
BOOL OnWindowResize()
{
	RECT rcClient;
	GetClientRect(hwndMain, &rcClient);
	SetWindows(&rcClient);
	return TRUE;
}

/* ************************************
* BOOL OnChildWindowsNotify(PVOID pParam)
* ����	���������Ӵ����򸸴��ڷ��͵�NOTIFY��Ϣ
* ����	pParam��WM_NOTIFY��Ϣ����
* ����ֵ 	�Ƿ���Tree View���͵�
**************************************/
BOOL OnChildWindowsNotify(PVOID pParam)
{
	LPNMHDR phdr = (LPNMHDR)pParam;
	// ֻ����Tree View���͵�Notify������������
	if(phdr->hwndFrom != hwndTreeView)
	{
		return FALSE;
	}
	switch (((LPNMHDR)pParam)->code)
	{
	// ������Ҽ������������OnRclickTree���������Ҽ��˵�
	case NM_RCLICK :
		OnRclickTree((LPNMHDR)pParam);
		break;

	default:
		break;
	}
	return TRUE;
}


/* ************************************
* About
* ����	�����ڡ��Ի�����Ϣ��������
**************************************/
LRESULT CALLBACK About(
					   HWND hDlg,		// �Ի��򴰿ھ��
					   UINT message,		// ��Ϣ
					   WPARAM wParam,	// ��Ϣ����
					   LPARAM lParam)		// ��Ϣ����
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		// ���OK�������Ի���
		if (LOWORD(wParam) == IDOK) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		// ������˳�������������
		if(LOWORD(wParam) == IDCANCEL)
		{
			ExitProcess(0);
		}
		break;
	}
	return FALSE;
}

/* ************************************
* DWORD SetWindows(LPRECT lpRect)
* ����	�������д����������ؼ����������Ӵ���
*			�����������Ӵ��ڵ�λ�úʹ�С			
* ����	lpRect��ָ���ʾ�����ڿͻ�����RECT
**************************************/
DWORD SetWindows(LPRECT lpRect)
{
	// Tree View
	SetWindowPos(hwndTreeView, HWND_TOP,
		lpRect->left, lpRect->top,
		lpRect->right *0.3 ,lpRect->bottom,
		SWP_SHOWWINDOW);
	// List View
	SetWindowPos(hwndListView, HWND_TOP,
		lpRect->right * 0.3, lpRect->bottom * 0.7,
		lpRect->right * 0.7, lpRect->bottom * 0.3,
		SWP_SHOWWINDOW);
	// Edit
	SetWindowPos(hwndEdit, HWND_TOP,
		lpRect->right * 0.3, lpRect->top,
		lpRect->right * 0.7, lpRect->bottom * 0.7,
		SWP_SHOWWINDOW);
	return 0;
}