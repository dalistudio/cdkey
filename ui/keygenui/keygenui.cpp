// keygenui.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "keygenui.h"
#include <string>

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
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

// “关于”框的消息处理程序。
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
			// 初始化对话框代码
			SetDlgItemText(hwnd,IDC_KEY,"0xFFFFFFFF");
			SetDlgItemText(hwnd,IDC_TYPE,"0x3F");
			SetDlgItemText(hwnd,IDC_DATA,"0xFF");
			break;
		// 如果是命令事件（按钮等）
        case WM_COMMAND:
			// 对wParam低字节进行判断，其中是我们的ID（参见MSDN）
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

	// 调用外部命令之前最好判断 外部程序是否存在
	std::string str = ExeCmd((char*)&cmd);
	SetDlgItemText(hwnd,IDC_CDKEY,str.c_str());

	memset(&cmd,0,60);

	// 下面验证 CD-KEY
	char CdKey[30]={0};
	memcpy(&CdKey,str.c_str(),29);

	memcpy(&cmd,"keyver.exe ",10);
	cmd[10] = 0x20;
	memcpy(&cmd[11],&Key,10);
	cmd[21] = 0x20;
	memcpy(&cmd[22],&CdKey,29);

	// 调用外部命令之前最好判断 外部程序是否存在
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
		MessageBox(hwnd, "验证失败", "产品密钥验证", 16);
	}
	//MessageBox(hwnd, "Hello World!", "keygen", 0);
}

// 调用命令行程序，返回结果
std::string ExeCmd(const char *pszCmd)
{
	// 创建匿名管道
	SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES),NULL,TRUE};
	HANDLE hRead,hWrite;	// 管道的读和写句柄
	if(!CreatePipe(&hRead, &hWrite, &sa, 0))	// 创建管道
	{        
		return "";
	}
   
	// 设置命令行进程启动信息(以隐藏方式启动命令并定位其输出到hWrite)
	STARTUPINFO si = {sizeof(STARTUPINFO)};
	GetStartupInfo(&si);
	si.dwFlags = STARTF_USESHOWWINDOW |STARTF_USESTDHANDLES;
	si.wShowWindow =SW_HIDE;// 隐藏窗体
	si.hStdError =hWrite;	// 错误信息写到管道
	si.hStdOutput =hWrite;	// 标准输出写到管道
  
	// 启动进程运行命令行
	PROCESS_INFORMATION pi;
	if(!CreateProcess(NULL, (char*)pszCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{        
		return "";
	}

	// 立即关闭hWrite
	CloseHandle(hWrite);

	// 读取命令行返回值
	std::string strRet;
	char buff[1024] = {0};
	DWORD dwRead = 0;

	// 循环从管道读取命令行返回的信息
	while(ReadFile(hRead, buff, 1024, &dwRead, NULL))
	{
		strRet.append(buff, dwRead);	// 追加
	}
	// 关闭hRead
	CloseHandle(hRead);
	return strRet; 
}
