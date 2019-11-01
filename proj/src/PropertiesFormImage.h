#ifndef PROPERTIESFORMIMAGE_H
#define PROPERTIESFORMIMAGE_H

#include <QDialog>

namespace Ui {
class PropertiesFormImage;
}

class PropertiesFormImage : public QDialog
{
	Q_OBJECT

public:
	explicit PropertiesFormImage(int w, int h, QWidget *parent = 0);
	~PropertiesFormImage();

private:
	Ui::PropertiesFormImage *ui;
};

#endif // PROPERTIESFORMIMAGE_H
