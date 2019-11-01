#include <QtCore>

#if defined(Q_OS_WIN)
#include <windows.h>
#include <QtWinExtras>

bool SetWallpaper(const QString& fileName)
{
	return SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, (LPVOID)fileName.utf16(), SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE);
}
#endif
