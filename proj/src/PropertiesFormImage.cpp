#include "PropertiesFormImage.h"
#include "ui_PropertiesFormImage.h"

PropertiesFormImage::PropertiesFormImage(int w, int h, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PropertiesFormImage)
{
	ui->setupUi(this);

	ui->lblWidthValue->setText(QString().sprintf("%dpx", w));
	ui->lblHeightValue->setText(QString().sprintf("%dpx", h));
}

PropertiesFormImage::~PropertiesFormImage()
{
	delete ui;
}
