#ifndef SHELL_H
#define SHELL_H
#include <QPixmap>

class QString;

QPixmap IconFromFile(const QString& fileName);
void ShowInContainerFolder(const QString& fileName);
bool SetWallpaper(const QString& fileName);
bool IsExtensionAssociated(const QString& ext);
void AssociateExtensions(const QString& appPath, const QStringList& extensionList);

#endif // SHELL_H
