#ifdef USE_CONFIG
#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
	Q_OBJECT
	Ui::SettingsDialog* ui;
public:
	explicit SettingsDialog(const QStringList& extensionList, QWidget* parent = 0);
	~SettingsDialog();
public:
	void accept();
};

#endif // SETTINGSDIALOG_H
#endif
