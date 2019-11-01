#ifndef PROPERTIESFORMGENERAL_H
#define PROPERTIESFORMGENERAL_H

#include <QWidget>

namespace Ui {
    class PropertiesFormGeneral;
}

class PropertiesFormGeneral : public QWidget
{
    Q_OBJECT

public:
	explicit PropertiesFormGeneral(const QString& fileName, QWidget* parent = 0);
    ~PropertiesFormGeneral();

private:
    Ui::PropertiesFormGeneral *ui;
public slots:
	void slotOpenPath();
};

#endif // PROPERTIESFORMGENERAL_H
