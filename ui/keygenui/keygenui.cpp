// keygenui.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "keygenui.h"
#include <string>

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������:
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void keygen(HWND hwnd);
std::string ExeCmd(const char *pszCmd);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), 0, DlgProc);
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg) {
        case WM_INITDIALOG:
			// ��ʼ���Ի������
			SetDlgItemText(hwnd,IDC_KEY,"0xFFFFFFFF");
			SetDlgItemText(hwnd,IDC_TYPE,"0x3F");
			SetDlgItemText(hwnd,IDC_DATA,"0xFF");
			break;
		// ����������¼�����ť�ȣ�
        case WM_COMMAND:
			// ��wParam���ֽڽ����жϣ����������ǵ�ID���μ�MSDN��
			switch(LOWORD(wParam)) {
				case IDC_GEN:
					keygen(hwnd);
					break;
			}
		break;
        case WM_CLOSE:
			PostQuitMessage(0);
			break;
	}
	return 0;
}

void keygen(HWND hwnd)
{
	char Key[11]={0};
	char Type[5]={0};
	char Data[5]={0};
	char cmd[60]={0};
	GetDlgItemText(hwnd,IDC_KEY,Key,11);
	GetDlgItemText(hwnd,IDC_TYPE,Type,5);
	GetDlgItemText(hwnd,IDC_DATA,Data,5);

	memcpy(&cmd,"keygen.exe ",11);
	cmd[10] = 0x20;
	memcpy(&cmd[11],&Key,10); // KEY
	cmd[21]=0x20;
	memcpy(&cmd[22],&Type,4); // TYPE
	cmd[26]=0x20;
	memcpy(&cmd[27],&Data,4); // DATA

	// �����ⲿ����֮ǰ����ж� �ⲿ�����Ƿ����
	std::string str = ExeCmd((char*)&cmd);
	SetDlgItemText(hwnd,IDC_CDKEY,str.c_str());

	memset(&cmd,0,60);

	// ������֤ CD-KEY
	char CdKey[30]={0};
	memcpy(&CdKey,str.c_str(),29);

	memcpy(&cmd,"keyver.exe ",10);
	cmd[10] = 0x20;
	memcpy(&cmd[11],&Key,10);
	cmd[21] = 0x20;
	memcpy(&cmd[22],&CdKey,29);

	// �����ⲿ����֮ǰ����ж� �ⲿ�����Ƿ����
	str = ExeCmd((char*)&cmd);
	char delims[2] = {0x0d,0};
	char *result = NULL;
	result = strtok( (char*)str.c_str(), delims );
	int pos=0;
	if(memcmp(result,"OK",2)==0)
	{
		result = strtok( NULL, delims );
		pos=strcspn(result,"0x");
		SetDlgItemText(hwnd,IDC_SIGN,result+pos);
		
		result = strtok( NULL, delims );
		pos=strcspn(result,"0x");
		SetDlgItemText(hwnd,IDC_TYPE,result+pos);

		result = strtok( NULL, delims );
		pos=strcspn(result,"0x");
		SetDlgItemText(hwnd,IDC_DATA,result+pos);

		result = strtok( NULL, delims );
		pos=strcspn(result,"0x");
		SetDlgItemText(hwnd,IDC_HASH,result+pos);

	}
	else
	{
		SetDlgItemText(hwnd,IDC_SIGN,"");
		SetDlgItemText(hwnd,IDC_TYPE,"");
		SetDlgItemText(hwnd,IDC_DATA,"");
		SetDlgItemText(hwnd,IDC_HASH,"");
		MessageBox(hwnd, "��֤ʧ��", "��Ʒ��Կ��֤", 16);
	}
	//MessageBox(hwnd, "Hello World!", "keygen", 0);
}

// ���������г��򣬷��ؽ��
std::string ExeCmd(const char *pszCmd)
{
	// ���������ܵ�
	SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES),NULL,TRUE};
	HANDLE hRead,hWrite;	// �ܵ��Ķ���д���
	if(!CreatePipe(&hRead, &hWrite, &sa, 0))	// �����ܵ�
	{        
		return "";
	}
   
	// ���������н���������Ϣ(�����ط�ʽ���������λ�������hWrite)
	STARTUPINFO si = {sizeof(STARTUPINFO)};
	GetStartupInfo(&si);
	si.dwFlags = STARTF_USESHOWWINDOW |STARTF_USESTDHANDLES;
	si.wShowWindow =SW_HIDE;// ���ش���
	si.hStdError =hWrite;	// ������Ϣд���ܵ�
	si.hStdOutput =hWrite;	// ��׼���д���ܵ�
  
	// ������������������
	PROCESS_INFORMATION pi;
	if(!CreateProcess(NULL, (char*)pszCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{        
		return "";
	}

	// �����ر�hWrite
	CloseHandle(hWrite);

	// ��ȡ�����з���ֵ
	std::string strRet;
	char buff[1024] = {0};
	DWORD dwRead = 0;

	// ѭ���ӹܵ���ȡ�����з��ص���Ϣ
	while(ReadFile(hRead, buff, 1024, &dwRead, NULL))
	{
		strRet.append(buff, dwRead);	// ׷��
	}
	// �ر�hRead
	CloseHandle(hRead);
	return strRet; 
}
