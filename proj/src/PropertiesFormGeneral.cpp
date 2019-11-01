#include "PropertiesFormGeneral.h"
#include "ui_PropertiesFormGeneral.h"
#include <QFileInfo>
#include <QDateTime>

#include "utils/shell.h"
#ifdef Q_OS_WIN
#include <windows.h>
#else
#endif

PropertiesFormGeneral::PropertiesFormGeneral(const QString& fileName, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::PropertiesFormGeneral)
{
    ui->setupUi(this);

	QFileInfo fi(fileName);
	if(fi.exists())
	{
		ui->label_icon->setPixmap(IconFromFile(fileName));

		ui->label_name->setText(fi.baseName());
		ui->label_size->setText(QString::number(fi.size()));
		ui->label_path->setText(fi.absolutePath());

		ui->label_time_creation->setText(fi.created().toString());
		ui->label_time_modified->setText(fi.lastModified().toString());
		ui->label_time_lastaccess->setText(fi.lastRead().toString());

		ui->checkbox_hidden->setAttribute(Qt::WA_TransparentForMouseEvents);
		ui->checkbox_archive->setAttribute(Qt::WA_TransparentForMouseEvents);
		ui->checkbox_system->setAttribute(Qt::WA_TransparentForMouseEvents);
		ui->checkbox_readonly->setAttribute(Qt::WA_TransparentForMouseEvents);


#ifdef Q_OS_WIN
		SHFILEINFOW shfi;
		DWORD attr = GetFileAttributesW((const WCHAR*)fi.absoluteFilePath().constData());

		SHGetFileInfoW((const WCHAR*)fi.absoluteFilePath().constData(),
					   attr, &shfi, sizeof(shfi), SHGFI_USEFILEATTRIBUTES | SHGFI_TYPENAME | SHGFI_DISPLAYNAME);

		ui->label_type->setText(QString::fromWCharArray(shfi.szTypeName));
		ui->label_display->setText(QString::fromWCharArray(shfi.szDisplayName));

		ui->checkbox_archive->setChecked(attr & FILE_ATTRIBUTE_ARCHIVE);
		ui->checkbox_hidden->setChecked(attr & FILE_ATTRIBUTE_HIDDEN);
		ui->checkbox_readonly->setChecked(attr & FILE_ATTRIBUTE_READONLY);
		ui->checkbox_system->setChecked(attr & FILE_ATTRIBUTE_SYSTEM);
#else
		ui->checkbox_archive->setChecked(fi.isFile());
		ui->checkbox_hidden->setChecked(fi.isHidden());
		ui->checkbox_readonly->setChecked(fi.isWritable());
		ui->checkbox_system->setChecked(false);
#endif
	}
}

PropertiesFormGeneral::~PropertiesFormGeneral()
{
    delete ui;
}
void PropertiesFormGeneral::slotOpenPath()
{
//	ShowInContainerFolder(ui->label_path->text());
	ShowInContainerFolder(ui->label_display->text());
}
