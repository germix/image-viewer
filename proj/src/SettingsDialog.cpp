#ifdef USE_CONFIG
#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

#include "utils/shell.h"
#include <QApplication>

SettingsDialog::SettingsDialog(const QStringList& extensionList, QWidget* parent) :
	QDialog(parent),
	ui(new Ui::SettingsDialog)
{
	ui->setupUi(this);

	for(int i = 0; i < extensionList.size(); i++)
	{
		QString ext = extensionList.at(i);
		QTreeWidgetItem* item = new QTreeWidgetItem(QStringList() << ext.remove(0, 2));

		item->setFlags(item->flags()|Qt::ItemIsUserCheckable);
		if(IsExtensionAssociated("." + ext))
			item->setCheckState(0, Qt::Checked);
		else
			item->setCheckState(0, Qt::Unchecked);

		ui->treeWidget->addTopLevelItem(item);
	}
	connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}
#include <QMessageBox>
void SettingsDialog::accept()
{
	QStringList extensionList;
	for(int i = 0; i < ui->treeWidget->topLevelItemCount(); i++)
	{
		QTreeWidgetItem* item = ui->treeWidget->topLevelItem(i);
		if(item->checkState(0) & Qt::Checked)
		{
			extensionList.append(item->text(0));
		}
	}
	AssociateExtensions(qApp->applicationFilePath(), extensionList);
	QDialog::accept();

}
#endif
