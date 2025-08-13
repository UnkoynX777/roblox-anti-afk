#include "windowsutils.h"

namespace WindowsUtils
{

    bool isProcessRunning(const QString &processName)
    {
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE)
            return false;
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hSnapshot, &pe32))
        {
            do
            {
                if (_wcsicmp(pe32.szExeFile, (const wchar_t *)processName.utf16()) == 0)
                {
                    CloseHandle(hSnapshot);
                    return true;
                }
            } while (Process32Next(hSnapshot, &pe32));
        }
        CloseHandle(hSnapshot);
        return false;
    }

    bool isWindowFocused(const QString &processName)
    {
        HWND hwnd = GetForegroundWindow();
        if (!hwnd)
            return false;
        DWORD pid = 0;
        GetWindowThreadProcessId(hwnd, &pid);
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE)
            return false;
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);
        bool found = false;
        if (Process32First(hSnapshot, &pe32))
        {
            do
            {
                if (_wcsicmp(pe32.szExeFile, (const wchar_t *)processName.utf16()) == 0 && pe32.th32ProcessID == pid)
                {
                    found = true;
                    break;
                }
            } while (Process32Next(hSnapshot, &pe32));
        }
        CloseHandle(hSnapshot);
        return found;
    }

}