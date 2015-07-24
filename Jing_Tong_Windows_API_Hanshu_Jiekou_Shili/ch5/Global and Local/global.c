/* ************************************
*����ͨWindows API�� 
* ʾ������
* global.c
* 5.3.3  ʹ��ȫ�ֺ;ֲ�����������ͷ��ڴ�
**************************************/

/* ͷ�ļ���*/
#include <windows.h>
#include <stdio.h>

/*************************************
* int main(void)
* ����	��ʾGlobal*������ʹ��
*
* ����	��
*
**************************************/
int main(void)
{	
	LPVOID lpMem;	//�ڴ��ַ
	HGLOBAL hMem;	//�ڴ���
	SIZE_T sizeMem;	//�ڴ��С
	UINT uFlags;	//����

	//�����ڴ�
	lpMem = (LPVOID)GlobalAlloc(GPTR,1000);
	//�����ݸ��Ƶ��ڴ���
	lstrcpy(lpMem,"this is a string");	
	//����ڴ����ԣ���ӡ
	uFlags = GlobalFlags(lpMem);
	printf("�ڴ��е����ݣ�\"%s\"���ڴ��ַ��0x%.8x���ڴ����ԣ�%u\n",
		lpMem,lpMem,uFlags);
	//�ͷ�
	GlobalFree(lpMem);
	//�����ڴ棬��ȡ��Ϣ
	hMem = GlobalAlloc(GMEM_MOVEABLE,1000);
	sizeMem = GlobalSize(hMem);
	uFlags = GlobalFlags(hMem);
	//��ӡ��Ϣ
	printf("�ڴ��С��%d���ڴ�����0x%.8x���ڴ����ԣ�%u\n",
		sizeMem,hMem,uFlags);
	//����
	lpMem = GlobalLock(hMem);
	//�ͷ�
	GlobalFree(hMem);
	return 0;
}