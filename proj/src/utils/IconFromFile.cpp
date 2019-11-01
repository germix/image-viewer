////////////////////////////////////////////////////////////////////////////////////////////////////
//
// IconFromFile
//
// ?/?/2017
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <QtCore>

#if defined(Q_OS_WIN)
#include <windows.h>
#include <QtWinExtras>
QPixmap IconFromFile(const QString& fileName)
{
	HICON hIcon;
	QPixmap pixmap;

#if 0
	hIcon = ExtractIconW(NULL, (const WCHAR*)fileName.constData(), 0);
#else
	SHFILEINFO shfi;

	memset(&shfi, 0, sizeof(shfi));
	SHGetFileInfoW((const WCHAR*)fileName.constData(),
				   FILE_ATTRIBUTE_NORMAL,
				   &shfi,
				   sizeof(SHFILEINFO),
				   SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_LARGEICON);
	hIcon = shfi.hIcon;
#endif
	if(hIcon)
	{
		pixmap = QtWin::fromHICON(hIcon);
		DestroyIcon(hIcon);
	}
	return pixmap;
}
#elif defined(Q_OS_LINUX)

QPixmap IconFromFile(const QString& fileName)
{
	return QPixmap();
}

#endif
