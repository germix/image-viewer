////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ShowInContainerFolder
//
// 03/06/2017
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <QtCore>

#ifdef Q_OS_WIN32
#include <QDir>
#include <QProcess>
#include <QUrl>
#include <QDesktopServices>
#include <windows.h>

void ShowInContainerFolder(const QString& fileName)
{
	//qint64 pid;
	QString args = "explorer.exe /select,\"" + QDir::toNativeSeparators(fileName) + "\"";
	QString workingDir;

	bool success = false;
	PROCESS_INFORMATION pinfo;

#if defined(Q_OS_WINCE)
		QString fullPathProgram = program;
		if (!QDir::isAbsolutePath(fullPathProgram))
			fullPathProgram.prepend(QDir::currentPath().append(QLatin1Char('/')));
		fullPathProgram.replace(QLatin1Char('/'), QLatin1Char('\\'));
		success = CreateProcess((wchar_t*)fullPathProgram.utf16(),
								(wchar_t*)args.utf16(),
								0, 0, false, CREATE_NEW_CONSOLE, 0, 0, 0, &pinfo);
#else
		STARTUPINFOW startupInfo = { sizeof( STARTUPINFO ), 0, 0, 0,
									 (ulong)CW_USEDEFAULT, (ulong)CW_USEDEFAULT,
									 (ulong)CW_USEDEFAULT, (ulong)CW_USEDEFAULT,
									 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
								   };
		success = CreateProcess(0, (wchar_t*)args.utf16(),
								0, 0, FALSE, CREATE_UNICODE_ENVIRONMENT | CREATE_NEW_CONSOLE, 0,
								workingDir.isEmpty() ? 0 : (wchar_t*)workingDir.utf16(),
								&startupInfo, &pinfo);
#endif // Q_OS_WINCE

	if (success) {
		CloseHandle(pinfo.hThread);
		CloseHandle(pinfo.hProcess);
		//if (pid) *pid = pinfo.dwProcessId;
	}


}
#else
void ShowInContainerFolder(const QString& fileName)
{
}

#endif


