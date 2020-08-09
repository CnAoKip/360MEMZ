#include "360MEMZ.h"
#define  WM_SHOWTASK (WM_USER + 1)

const wchar_t *effectnames[] = {
L"�� �� ��", 
L"��עup��",
L"�� �� �� С �� ��",
L"ɾ���ļ�",
L"360MEMZ�ι���",
L"Hilbert����",
L"Loading...", 
L"Cmd_MEMZ",
L"�� ��",
L"winver��ɧ��" 

};

int lasteffect = 1;



BOOL firsttotray = true;
void ToTray(HWND hwnd, BOOL bDelete)
{
	if(firsttotray) {
		firsttotray = false;
		MessageBox(hwnd, L"360MEMZ����̨ ����С�������̡�", L"360MEMZ", MB_ICONINFORMATION);
	}
	
	HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_360MEMZ));
    NOTIFYICONDATA nid;
// {ECB18593-7D73-4B77-B786-251E2FDC935F}
static const GUID ic_guid = 
{ 0xecb18593, 0x7d73, 0x4b77, { 0xb7, 0x86, 0x25, 0x1e, 0x2f, 0xdc, 0x93, 0x5f } };

    nid.guidItem = ic_guid;
    nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uFlags = NIF_GUID|NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.uCallbackMessage = WM_SHOWTASK;
    nid.hIcon = hIcon;
    nid.uID = 0;
    lstrcpyW(nid.szTip, L"360MEMZ ����̨");//��Ϣ��ʾ��
    Shell_NotifyIcon(NIM_SETVERSION, &nid);
    Shell_NotifyIcon(bDelete?NIM_DELETE:NIM_ADD, &nid);//�����������ͼ��
    /*AllocConsole();
    Sleep(500);
    char a[999];
    sprintf(a, "%u\n", GetLastError());
    MessageBoxA(NULL, a, "", 0);
    Sleep(2000);*/
    if(!bDelete)ShowWindow(hwnd, SW_HIDE);//����������
    else {
    	ShowWindow(hwnd, SW_SHOW);
    	ShowWindow(hwnd, SW_RESTORE); 
	}
}

LPWSTR Text;
int iTimeout = 240;


void DrawGlowingText(HDC hDC, LPWSTR szText, RECT *rcArea, 
	DWORD dwTextFlags, int iGlowSize, HFONT hFont)
{
	//��ȡ������
	HTHEME hThm = OpenThemeData(GetDesktopWindow(), L"TextStyle");
	//����DIB
	HDC hMemDC = CreateCompatibleDC(hDC);
	BITMAPINFO bmpinfo = {0};
	bmpinfo.bmiHeader.biSize = sizeof(bmpinfo.bmiHeader);
	bmpinfo.bmiHeader.biBitCount = 32;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biWidth = rcArea->right - rcArea->left;
	bmpinfo.bmiHeader.biHeight = -(rcArea->bottom - rcArea->top);
	HBITMAP hBmp = CreateDIBSection(hMemDC, &bmpinfo, DIB_RGB_COLORS, 0, NULL, 0);
	if (hBmp == NULL) return;
	HGDIOBJ hBmpOld = SelectObject(hMemDC, hBmp);
	//����ѡ��
	DTTOPTS dttopts = {0};
	dttopts.dwSize = sizeof(DTTOPTS);
	dttopts.dwFlags = DTT_GLOWSIZE | DTT_COMPOSITED;
	dttopts.iGlowSize = iGlowSize;	//����ķ�Χ��С
	//�����ı�
	SetTextColor(hMemDC, RGB(192, 0, 255));
	if(hFont) SelectObject(hMemDC, hFont);
	RECT rc = {0, 0, rcArea->right - rcArea->left, rcArea->bottom - rcArea->top};
	HRESULT hr = DrawThemeTextEx(hThm, hMemDC, 0, 0, szText, -1, dwTextFlags , &rc, &dttopts);
	if(FAILED(hr)) return;
	BitBlt(hDC, rcArea->left, rcArea->top, rcArea->right - rcArea->left, 
		rcArea->bottom - rcArea->top, hMemDC, 0, 0, SRCCOPY | CAPTUREBLT);
	//Clear
	SelectObject(hMemDC, hBmpOld);
	DeleteObject(hBmp);
	DeleteDC(hMemDC);
	CloseThemeData(hThm);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		
		case WM_CLOSE: {
			ToTray(hwnd, FALSE); 
			return 0;
		}
		case WM_ACTIVATE: {
			
			SetTimer(hwnd, 1, 1000, NULL);
			SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
			MARGINS margins = {-1, -1, -1, -1};
			DwmExtendFrameIntoClientArea(hwnd, &margins);
			break;
		}
		
		case WM_TIMER:{
			iTimeout--;
			if(iTimeout==-1) Kill();
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		}
		
		case WM_PAINT:{
			InvalidateRect(hwnd, NULL, FALSE);
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			RECT rc;
			GetClientRect(hwnd, &rc);
			LOGFONTW LogFont = {0 };
			LogFont.lfHeight = -48;
			LogFont.lfWeight = FW_BOLD;
			LogFont.lfWidth = -24;
			LogFont.lfUnderline = TRUE;
			lstrcpyW(LogFont.lfFaceName, L"΢���ź�");
			HFONT hFontNew = CreateFontIndirectW(&LogFont);
			SelectObject(hdc, hFontNew);
			
			
			swprintf(Text, L"�Ѿ����е���%d��Ч��:\n%s\nԤ�ƽ���%d��������", lasteffect, effectnames[lasteffect-1], iTimeout);
			
			DrawGlowingText(hdc, Text, &rc, DT_CENTER, 15, hFontNew);
			EndPaint(hwnd, &ps);
			break;
		}
		case WM_SHOWTASK:{
			if(lParam == WM_LBUTTONDOWN)ToTray(hwnd, TRUE);
			return 0;
		}
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

DWORD WINAPI CreateTerminater(LPVOID lpParameter) {
	Text = (LPWSTR)LocalAlloc(LMEM_ZEROINIT, 4096);
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* insert window procedure function here */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(COLOR_WINDOW);;
	wc.lpszClassName = L"WindowClass";
	wc.hIcon		 = LoadIcon(hInstance, MAKEINTRESOURCE(IDR_360MEMZ)); /* use "A" as icon name when you want to use the project icon */
	wc.hIconSm		 = LoadIcon(hInstance, MAKEINTRESOURCE(IDR_360MEMZ)); /* as above */

	if(!RegisterClassEx(&wc)) {
		MessageBoxA(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	
	hwnd = CreateWindowExA(WS_EX_CLIENTEDGE|WS_EX_TOPMOST,"WindowClass","360MEMZ ����̨",WS_VISIBLE|WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME,CW_USEDEFAULT,CW_USEDEFAULT,800,480,NULL,NULL,hInstance,NULL);
	if(hwnd == NULL) {
		MessageBoxA(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	
	HFONT font = CreateFont(
                20, // nHeight
                0, // nWidth
                0, // nEscapement
                0, // nOrientation
                FW_NORMAL, // nWeight
                FALSE, // bItalic
                FALSE, // bUnderline
                0, // cStrikeOut
                ANSI_CHARSET, // nCharSet
                OUT_DEFAULT_PRECIS, // nOutPrecision
                CLIP_DEFAULT_PRECIS, // nClipPrecision
                DEFAULT_QUALITY, // nQuality
                DEFAULT_PITCH | FF_SWISS,
                L"΢���ź�" // nPitchAndFamily Arial
                );



        HWND b1 = CreateWindow(
                L"BUTTON",   // predefined class
                L"��ͣ��Ч������",       // button text
                WS_VISIBLE | WS_CHILD,  //values for buttons.
                100,         // starting x position
                300,         // starting y position
                150,        // button width
                40,        // button height
                hwnd,       // parent window
                0,       // No menu
                (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                NULL);

        HWND b2 = CreateWindow(
                L"BUTTON",   // predefined class
                L"��(��)��(��)",       // button text
                WS_VISIBLE | WS_CHILD,  //values for buttons.
                290,         // starting x position
                300,         // starting y position
                150,        // button width
                40,        // button height
                hwnd,       // parent window
                0,       // No menu
                (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                NULL);
		
		HWND b3 = CreateWindow(
                L"BUTTON",   // predefined class
                L"��ֹ����",       // button text
                WS_VISIBLE | WS_CHILD,  //values for buttons.
                480,         // starting x position
                300,         // starting y position
                150,        // button width
                40,        // button height
                hwnd,       // parent window
                0,       // No menu
                (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                NULL);
        SendMessage(b1, WM_SETFONT, (WPARAM)font, 1);
        SendMessage(b2, WM_SETFONT, (WPARAM)font, 1);
		SendMessage(b3, WM_SETFONT, (WPARAM)font, 1);
		
	bool paused = false;
	DWORD tid = (DWORD)lpParameter;
	while(GetMessage(&Msg, NULL, 0, 0) > 0) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
		if (Msg.hwnd==b1 && Msg.message== WM_LBUTTONDOWN) {
			if(!paused){
				SetWindowTextW(b1, L"������Ч������");
				
				
            	SuspendThread(OpenThread(THREAD_ALL_ACCESS, FALSE, tid));
            	KillTimer(hwnd, 1);
            	paused = true;
			}
			else{
				SetWindowTextW(b1, L"��ͣ��Ч������");
				tid = (DWORD)lpParameter;
				
            	ResumeThread(OpenThread(THREAD_ALL_ACCESS, FALSE, tid));
            	SetTimer(hwnd, 1, 1000, NULL);
            	paused = false;
			}
        }

        if (Msg.hwnd == b2 && Msg.message == WM_LBUTTONDOWN) {
            Kill();
        }
        
        if (Msg.hwnd == b3 && Msg.message == WM_LBUTTONDOWN) {
            system("taskkill /f /im rundll32.exe");
            SuspendThread(OpenThread(THREAD_ALL_ACCESS, FALSE, tid));
            MessageBox(hwnd, L"��������ֹ!��ȷ���˳���", L"360MEMZ", MB_ICONINFORMATION);
            TerminateProcess(GetCurrentProcess(), 0);
        }
	}
	return Msg.wParam;
}

