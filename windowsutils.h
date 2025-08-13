#ifndef WINDOWSUTILS_H
#define WINDOWSUTILS_H

#include <QString>
#include <windows.h>
#include <tlhelp32.h>

namespace WindowsUtils
{
    bool isProcessRunning(const QString &processName);
    bool isWindowFocused(const QString &processName);
}

#endif