/* ************************************
*����ͨWindows API�� 
* ʾ������
* window.cpp
* 2.2  Windows API�Ĺ��ܷ���
**************************************/

/* Ԥ������*/
/* ͷ�ļ���*/
#include <windows.h>

/* ȫ�ֱ�����*/
HINSTANCE hinst; 

/* ����������*/ 
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int); 
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM); 

/* ************************************
* ����	��ʾһ������
**************************************/
int WINAPI WinMain(HINSTANCE hinstance, 
                   HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine, 
                   int nCmdShow) 
{     
    WNDCLASSEX wcx;         //��������
    HWND hwnd;              //  ���ھ��     
    MSG msg;                //����Ϣ
    BOOL fGotMessage;       //���Ƿ�ɹ���ȡ��Ϣ
    hinst = hinstance;      // Ӧ�ó���ʵ�����������Ϊȫ�ֱ���

    // ��䴰��������ݽṹ
    wcx.cbSize = sizeof(wcx);          // �ṹ��Ĵ�С
    wcx.style = CS_HREDRAW | 
        CS_VREDRAW;                    // ��ʽ����С�ı�ʱ�ػ���� 
    wcx.lpfnWndProc = MainWndProc;     // ������Ϣ�������� 
    wcx.cbClsExtra = 0;                // ��ʹ�����ڴ�
    wcx.cbWndExtra = 0;                // ��ʹ�ô����ڴ� 
    wcx.hInstance = hinstance;         // ������Ӧ�ó���ʵ����� 
    wcx.hIcon = LoadIcon(NULL, 
        IDI_APPLICATION);              // ͼ�꣺Ĭ��
    wcx.hCursor = LoadCursor(NULL, 
        IDC_ARROW);                    // ��꣺Ĭ��
    wcx.hbrBackground = (HBRUSH)GetStockObject( 
        WHITE_BRUSH);                  // ��������ɫ
    wcx.lpszMenuName =  NULL;          // �˵�����ʹ��
    wcx.lpszClassName = "MainWClass";  // �������� 
    wcx.hIconSm = (HICON)LoadImage(hinstance, // Сͼ��
        MAKEINTRESOURCE(5),
        IMAGE_ICON, 
        GetSystemMetrics(SM_CXSMICON), 
        GetSystemMetrics(SM_CYSMICON), 
        LR_DEFAULTCOLOR); 

    // ע�ᴰ����
    if(!RegisterClassEx(&wcx))
    {
        return 1;
    }

    // �������� 
    hwnd = CreateWindow( 
        "MainWClass",        // ������
        "CH 2-3",            // ���ڱ��� 
        WS_OVERLAPPEDWINDOW, // ������ʽ  
        CW_USEDEFAULT,       // ˮƽλ��X��Ĭ�� 
        CW_USEDEFAULT,       // ��ֱλ��Y��Ĭ��
        CW_USEDEFAULT,       // ���ȣ�Ĭ��
        CW_USEDEFAULT,       // �߶ȣ�Ĭ�� 
        (HWND) NULL,         // �����ڣ��� 
        (HMENU) NULL,        // �˵���ʹ�ô�����Ĳ˵� 
        hinstance,           // Ӧ�ó���ʵ����� 
        (LPVOID) NULL);      // ���ڴ���ʱ���ݣ��� 

    if (!hwnd) 
    {
        return 1; 
    }

    // ��ʾ���� 
    ShowWindow(hwnd, nCmdShow); 
    UpdateWindow(hwnd); 
    
    // ��Ϣѭ��
    while (
        (fGotMessage = GetMessage(&msg, (HWND) NULL, 0, 0)) != 0 
        && fGotMessage != -1) 
    { 
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    } 
    return msg.wParam; 

} 

/* ************************************
* MainWndProc
* ����	������Ϣ����������
        �����е���Ϣ��ʹ��Ĭ�ϴ�������
**************************************/
LRESULT CALLBACK MainWndProc(HWND hwnd,
                             UINT uMsg,
                             WPARAM wParam,
                             LPARAM lParam
                             )
{
    switch (uMsg) 
    { 
        case WM_DESTROY: 
            ExitThread(0);
            return 0; 
        default: 
            return DefWindowProc(hwnd, uMsg, wParam, lParam); 
    } 
}