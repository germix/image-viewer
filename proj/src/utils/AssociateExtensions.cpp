#ifdef USE_CONFIG
#include <QStringList>

#include <windows.h>
#include <shobjidl.h>
bool IsWindowsVistaOrHigher() {
   OSVERSIONINFO osvi;
   ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
   GetVersionEx(&osvi);
   return osvi.dwMajorVersion >= 6;
}
/*
bool SetAppAsDefault(const QString& appPath, const QString& extension)
{
	if(IsWindowsVistaOrHigher())
	{
		HKEY hKey;
		if(SUCCEEDED(RegCreateKeyExW(HKEY_CLASSES_ROOT, (LPCWSTR)extension.utf16(), 0, 0, 0, KEY_ALL_ACCESS, 0, &hKey, 0)))
		{
			QString app = "Germix.ImageViewer";
			RegSetValueEx(hKey, L"", 0, REG_SZ, (BYTE*)app.utf16(), app.length()*2);
			RegCloseKey(hKey);
		}

		if(SUCCEEDED(RegCreateKeyExW(HKEY_CLASSES_ROOT, L"Germix.ImageViewer\\shell\\open\\command", 0, 0, 0, KEY_ALL_ACCESS, 0, &hKey, 0)))
		{
			QString app = "\"" + appPath + "\" \"%1\"";
			app.replace("/", "\\");
			RegSetValueExW(hKey, L"", 0, REG_SZ, (BYTE*)app.utf16(), app.length()*2);
			RegCloseKey(hKey);
		}
	}
	return false;
}
*/
int AssociateExtension(const QString& appPath, const QString& desc, const QString& extension)
{
#if 1
	HKEY hkey;

	//string action="Open with my program";
	QString action = "open";

	QString sub="\\shell\\";

	QString path=extension+
				sub+
				action+
				"\\"
				"command\\";


	// 1: Create subkey for extension -> HKEY_CLASSES_ROOT\.002
	if(RegCreateKeyExW(HKEY_CLASSES_ROOT, (LPCWSTR)extension.utf16(),0,0,0,KEY_ALL_ACCESS,0,&hkey,0)!=ERROR_SUCCESS)
	{
		 // cerr <<"Could not create or open a registrty key\n";
		  return 1;
	}
	RegSetValueExW(hkey,L"",0,REG_SZ,(BYTE*)desc.utf16(),desc.size()*2);
	RegCloseKey(hkey);



	// 2: Create Subkeys for action ( "Open with my program" )
	// HKEY_CLASSES_ROOT\.002\Shell\\open with my program\\command
	if(RegCreateKeyExW(HKEY_CLASSES_ROOT,(LPCWSTR)path.utf16(),0,0,0,KEY_ALL_ACCESS,0,&hkey,0)!=ERROR_SUCCESS)
	{
		//  cerr <<"Could not create or open a registrty key\n";
		return 1;
	}
	QString app = "\"" + appPath + "\" \"%1\"";
	RegSetValueExW(hkey, L"", 0, REG_SZ, (BYTE*)app.utf16(), app.size()*2);


	RegCloseKey(hkey);

#else


	HKEY hkey;

	//string action="Open with my program";
	QString action = "open";

	QString sub="\\shell\\";

	QString path=extension+
				sub+
				action+
				"\\"
				"command\\";


	// 1: Create subkey for extension -> HKEY_CLASSES_ROOT\.002
	QString s = "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\" + extension;
	if(RegCreateKeyExW(HKEY_CURRENT_USER, (LPCWSTR)s.utf16(),0,0,0,KEY_ALL_ACCESS,0,&hkey,0)!=ERROR_SUCCESS)
	{
		 // cerr <<"Could not create or open a registrty key\n";
		  return 1;
	}
	RegSetValueExW(hkey,L"",0,REG_SZ,(BYTE*)desc.utf16(),desc.size()*2);
	RegCloseKey(hkey);

/*

	// 2: Create Subkeys for action ( "Open with my program" )
	// HKEY_CLASSES_ROOT\.002\Shell\\open with my program\\command
	if(RegCreateKeyExW(HKEY_CLASSES_ROOT,(LPCWSTR)path.utf16(),0,0,0,KEY_ALL_ACCESS,0,&hkey,0)!=ERROR_SUCCESS)
	{
		//  cerr <<"Could not create or open a registrty key\n";
		return 1;
	}
	QString app = "\"" + appPath + "\" \"%1\"";
	RegSetValueExW(hkey, L"", 0, REG_SZ, (BYTE*)app.utf16(), app.size()*2);


	RegCloseKey(hkey);
*/
#endif
	return 0;
}

const QString nppDoc    = "Germix.ImageViewer-doc";
const QString nppName   = "Germix.ImageViewer-file";
const QString nppBackup = "Germix.ImageViewer-backup";

bool IsExtensionAssociated(const QString& ext)
{
	bool ok = false;
	HKEY hKey;
	if(SUCCEEDED(RegOpenKeyEx(HKEY_CLASSES_ROOT, (LPCWSTR)ext.utf16(), 0, KEY_ALL_ACCESS, &hKey)))
	{
		DWORD valType = 0;
		WCHAR valData[256];
		DWORD valDataLen = sizeof(valData);

		RegQueryValueExW(hKey, L"", NULL, &valType, (LPBYTE)(valData), &valDataLen);

		if((valType == REG_SZ) && !wcscmp(valData, (const wchar_t*)nppName.utf16()))
		{
			ok = true;
		}
		RegCloseKey(hKey);
	}
	return ok;
}

void addExtension(const QString& ext)
{
	HKEY  hKey = 0;
	DWORD dwDisp = 0;
	HRESULT nn;

	if(SUCCEEDED(RegCreateKeyExW(HKEY_CLASSES_ROOT, (LPCWSTR)ext.utf16(), 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &dwDisp)))
//	if(SUCCEEDED(RegCreateKeyExW(HKEY_CLASSES_ROOT, (LPCWSTR)ext.utf16(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisp)))
	{
		if(dwDisp == REG_OPENED_EXISTING_KEY)
		{
			WCHAR valData[MAX_PATH];
			DWORD valDataLen = MAX_PATH * sizeof(WCHAR);

			if(SUCCEEDED(RegQueryValueExW(hKey, L"", NULL, NULL, (LPBYTE)(valData), &valDataLen)))
			{
				RegSetValueExW(hKey, (LPCWSTR)nppBackup.utf16(), 0, REG_SZ, (LPBYTE)(valData), valDataLen);
			}
		}
		RegSetValueExW(hKey, NULL, 0, REG_SZ, (const BYTE *)(nppName.utf16()), (nppName.size()+1)*2);
		RegCloseKey(hKey);
	}
}

void AssociateExtensions(const QString& appPath, const QStringList& extensionList)
{
	{
		HKEY hKey;
		DWORD dwDisp;
		QString cmd;

		cmd = nppName + "\\shell\\open\\command";
		if(SUCCEEDED(RegCreateKeyExW(HKEY_CLASSES_ROOT, (LPCWSTR)cmd.utf16(), 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &dwDisp)))
		{
			HKEY hRootKey;
			if(SUCCEEDED(RegOpenKeyExW(HKEY_CLASSES_ROOT, (LPCWSTR)nppName.utf16(), 0, KEY_ALL_ACCESS, &hRootKey)))
			{
				RegSetValueExW(hRootKey, NULL, 0, REG_SZ, (LPBYTE)nppDoc.utf16(), (nppDoc.size()+1)*2);
				RegCloseKey(hRootKey);
			}
			QString app = "\"" + appPath + "\" \"%1\"";

			app.replace("/", "\\");
			RegSetValueExW(hKey, NULL, 0, REG_SZ, (LPBYTE)app.utf16(), (app.size()+1)*2);
			RegCloseKey(hKey);
		}
	}
	for(int i = 0; i < extensionList.size(); i++)
	{
		addExtension("." + extensionList.at(i));
	//	AssociateExtension(appPath, "Germix.ImageViewer", "." + extensionList.at(i));
	}
}

#endif
