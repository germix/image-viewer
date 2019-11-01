#ifndef PROPERTIESDIALOG_H
#define PROPERTIESDIALOG_H

#include <QDialog>

namespace Ui {
    class PropertiesDialog;
}

class PropertiesDialog : public QDialog
{
    Q_OBJECT

public:
	explicit PropertiesDialog(const QString& fileName, int w, int h, QWidget *parent = 0);
    ~PropertiesDialog();

private:
	Ui::PropertiesDialog *ui;
private:
	void addTab(QWidget* widget);
};

#endif // PROPERTIESDIALOG_H
