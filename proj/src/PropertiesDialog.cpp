#include "PropertiesDialog.h"
#include "ui_PropertiesDialog.h"

#include "PropertiesFormGeneral.h"
#include "PropertiesFormImage.h"

PropertiesDialog::PropertiesDialog(const QString& fileName, int w, int h, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PropertiesDialog)
{
    ui->setupUi(this);
	// ...
	addTab(new PropertiesFormGeneral(fileName));
	addTab(new PropertiesFormImage(w, h));
}
PropertiesDialog::~PropertiesDialog()
{
    delete ui;
}
void PropertiesDialog::addTab(QWidget* widget)
{
	ui->tabWidget->addTab(widget, widget->windowTitle());
}
