/*
                          qmusic

    Copyright (C) 2016 Arthur Benilov,
    arthur.benilov@gmail.com

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.
*/

#ifdef _MSC_VER
#   include <Windows.h>
#   include <DbgHelp.h>
#   include <tchar.h>
#   include "StackWalker.h"
#endif

#include <QFile>
#include <QDate>
#include <QTime>
#include "Application.h"
#include "CrashReporter.h"

CrashReporter* CrashReporter::s_pInstance = nullptr;

// Flag telling whether the exception filter has been installed.
static bool s_unhandledExceptionFilterSet = false;

/**
 * Returns path to the mini-dump crash file.
 * @param timestamp Timestamp to be used in the file name.
 * @return
 */
QString miniDumpFilePath(const QDateTime &timestamp)
{
    QDir appDataDir = Application::dataDirectory();
    QString fileName = QString("crash-%1.dmp")
            .arg(timestamp.toString("yyyyMMdd-HHmmsszzz"));
    return appDataDir.absoluteFilePath(fileName);
}

QString crashLogFilePath(const QDateTime &timestamp)
{
    QDir appDataDir = Application::dataDirectory();
    QString fileName = QString("crash-%1.log")
            .arg(timestamp.toString("yyyyMMdd-HHmmsszzz"));
    return appDataDir.absoluteFilePath(fileName);
}

#ifdef _MSC_VER

/**
 * Specialization of StackWalker class to save stack trace into a string.
 */
class CrashStackWalker : public StackWalker
{
public:
    CrashStackWalker()
        : StackWalker(),
          m_dumpStr()
    {}

    CrashStackWalker(DWORD dwProcessId, HANDLE hProcess)
        : StackWalker(dwProcessId, hProcess),
          m_dumpStr()
    {}

    void OnOutput(LPCSTR szText) override
    {
        m_dumpStr.append(QString::fromLatin1(szText));
    }

    QString dumpStr() const { return m_dumpStr; }

private:
    QString m_dumpStr;
};

#if defined _M_X64 || defined _M_IX86
BOOL preventSetUnhandledExceptionFilter()
{
    HMODULE hKernel32 = LoadLibrary(_T("kernel32.dll"));
    if (hKernel32 == NULL) {
        return FALSE;
    }

    void *pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");
    if (pOrgEntry == NULL) {
        return FALSE;
    }
#ifdef _M_IX86
    unsigned char szExecute[] = {
        0x33, 0xC0,         // xor eax, eax
        0xC2, 0x04, 0x00    // ret 4
    };
#elif _M_X64
    unsigned char szExecute[] = {
        0x33, 0xC0,         // xor eax, eax
        0xC3                // ret
    };
#else
#   error "The following code is applicable only to x86 or x64"
#endif

    DWORD dwOldProtect = 0;
    BOOL bProt = VirtualProtect(pOrgEntry, sizeof(szExecute), PAGE_EXECUTE_READWRITE, &dwOldProtect);
    SIZE_T bytesWritten = 0;
    BOOL bRet = WriteProcessMemory(GetCurrentProcess(),
                                   pOrgEntry, szExecute, sizeof(szExecute), &bytesWritten);
    if ((bProt != FALSE) && (dwOldProtect != PAGE_EXECUTE_READWRITE)) {
        DWORD dwBuf;
        VirtualProtect(pOrgEntry, sizeof(szExecute), dwOldProtect, &dwBuf);
    }
    return bRet;
}

#endif // _M_X64 || _M_IX86

/**
 * Save minidump file.
 * @param pExceptionInfo
 */
void createMiniDump(_EXCEPTION_POINTERS *pExceptionInfo, const QDateTime &timestamp)
{
    typedef BOOL (*PDUMPFN) (
                    HANDLE hProcess,
                    DWORD processId,
                    HANDLE hFile,
                    MINIDUMP_TYPE dumpType,
                    PMINIDUMP_EXCEPTION_INFORMATION exceptionParam,
                    PMINIDUMP_USER_STREAM_INFORMATION userStreamParam,
                    PMINIDUMP_CALLBACK_INFORMATION callbackParam
                );

    HMODULE h = ::LoadLibrary(_T("DbgHelp.dll"));
    PDUMPFN pFn = (PDUMPFN)::GetProcAddress(h, "MiniDumpWriteDump");
    if (pFn == NULL) {
        // Unable to find required function.
        return;
    }

    QString miniDumpFile = miniDumpFilePath(timestamp);
    QByteArray baMiniDumpFile = miniDumpFile.toLatin1();

    HANDLE hFile = CreateFile(baMiniDumpFile.data(),
                              GENERIC_READ | GENERIC_WRITE,
                              0,
                              NULL,
                              CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL);

    if (hFile && (hFile != INVALID_HANDLE_VALUE)) {
        MINIDUMP_EXCEPTION_INFORMATION info;
        info.ThreadId = ::GetCurrentThreadId();
        info.ExceptionPointers = pExceptionInfo;
        info.ClientPointers = TRUE;

        MINIDUMP_TYPE dumpType = MiniDumpNormal;

        (*pFn)(::GetCurrentProcess(),
               ::GetCurrentProcessId(),
               hFile,
               dumpType,
               (pExceptionInfo != NULL) ? &info : NULL,
               NULL, NULL);

        CloseHandle(hFile);
    }
}

/**
 * Write a log file containing crash trace
 * @param trace
 */
void createCrashLog(const QString &trace, const QDateTime &timestamp)
{
    QString crashLogFile = crashLogFilePath(timestamp);
    QFile f(crashLogFile);
    if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QByteArray data;
        data.append(QDate::currentDate().toString() + " at ");
        data.append(QTime::currentTime().toString() + "\n");
        data.append(QString("Application: %1\n").arg(qApp->applicationName()));
        data.append(QString("Version: %1\n").arg(qApp->applicationVersion()));
        data.append(QString("------------------------------------------------\n"));
        f.write(data);
        data = trace.toUtf8();
        f.write(data);
        f.close();
    }
}

/**
 * Custom handler for unhandled exceptions.
 * @param pExceptionInfo
 * @return
 */
LONG WINAPI unhandledExceptionFilter(struct _EXCEPTION_POINTERS *pExceptionInfo)
{
    QDateTime timestamp = QDateTime::currentDateTime();

    // Generating minidump
    createMiniDump(pExceptionInfo, timestamp);

    CrashStackWalker stackWalker;
    stackWalker.ShowCallstack(GetCurrentThread(), pExceptionInfo->ContextRecord);

    // Writing log file
    createCrashLog(stackWalker.dumpStr(), timestamp);

    QString message = QString(
                        "Application has encountered an unexpected error and has to be closed.\n"
                        "All unsaved data will be lost.\n"
                        "Crash dump has been created in %1 folder.\n"
                      )
            .arg(Application::dataDirectory().absolutePath());
    QByteArray baMessage = message.toLatin1();

    FatalAppExit(-1, baMessage.constData());
    return EXCEPTION_CONTINUE_SEARCH;
}
#endif // _MSC_VER


//
// CrashReporter implementation
//

CrashReporter::CrashReporter()
{
    installExceptionHandler();
}

CrashReporter* CrashReporter::instance()
{
    if (s_pInstance == nullptr) {
        s_pInstance = new CrashReporter();
    }
    return s_pInstance;
}

void CrashReporter::installExceptionHandler()
{
#ifdef _MSC_VER
    if (!s_unhandledExceptionFilterSet) {
        SetUnhandledExceptionFilter(unhandledExceptionFilter);
#if defined _M_X64 || _M_IX86
        preventSetUnhandledExceptionFilter();
#endif // _M_X64 || _M_IX86
        SetErrorMode(SEM_NOOPENFILEERRORBOX);
        s_unhandledExceptionFilterSet = true;
    }
#endif
}
