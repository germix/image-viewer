#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

#include "ImageViewer.h"
#include "Languages.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
	Ui::MainWindow*	ui;
	ImageViewer*	imageViewer;
	QStringList		availableFormats;
	QStringList		files;
	int				fileIndex;

	Languages		languages;
	QMenu*			menuLanguages;
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
private:
	void openFile(const QString& fname);
private:
	void changeEvent(QEvent* e);
	void dropEvent(QDropEvent* e);
	void dragEnterEvent(QDragEnterEvent* e);
	void contextMenuEvent(QContextMenuEvent* e);
	bool eventFilter(QObject* target, QEvent* event);
private slots:
	void slotAction();
};

#endif // MAINWINDOW_H
