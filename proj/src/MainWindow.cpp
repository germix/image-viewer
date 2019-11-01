#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Settings.h"

#include <QUrl>
#include <QMimeData>
#include <QDropEvent>

#include "AboutDialog.h"
#include "PropertiesDialog.h"

#include <QFileInfo>
#include <QImageReader>
#include <QDirIterator>
#include <QMenu>
#include <QMessageBox>
#include <QClipboard>

#include "utils/shell.h"

#include "SettingsDialog.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	QSettings s(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);

	menuLanguages = new QMenu(tr("Languages"));
	menuLanguages->setIcon(QIcon(":/images/menu-languages.png"));
	languages.init(menuLanguages, "translations", "imageviewer", s.value("Language").toString());

	QList<QByteArray> fmts = QImageReader::supportedImageFormats();
	foreach(QByteArray var, fmts)
	{
		availableFormats << QString("*.") + var.toStdString().c_str();
	}

	imageViewer = new ImageViewer();
	setCentralWidget(imageViewer);
	imageViewer->installEventFilter(this);

	// ...
	connect(ui->actionFileExit, SIGNAL(triggered(bool)), this, SLOT(slotAction()));
	connect(ui->actionFileWallpaper, SIGNAL(triggered(bool)), this, SLOT(slotAction()));
	connect(ui->actionFileCopyPath, SIGNAL(triggered(bool)), this, SLOT(slotAction()));
	connect(ui->actionFileCopyImage, SIGNAL(triggered(bool)), this, SLOT(slotAction()));
	connect(ui->actionFileProperties, SIGNAL(triggered(bool)), this, SLOT(slotAction()));
	connect(ui->actionFileOpenContainerFolder, SIGNAL(triggered(bool)), this, SLOT(slotAction()));
	connect(ui->actionHelpAbout, SIGNAL(triggered(bool)), this, SLOT(slotAction()));
	connect(ui->actionViewNormal, SIGNAL(triggered(bool)), this, SLOT(slotAction()));
	connect(ui->actionViewAspectRatio, SIGNAL(triggered(bool)), this, SLOT(slotAction()));
	connect(ui->actionOptionsSettings, SIGNAL(triggered(bool)), this, SLOT(slotAction()));

	//
	// Recuperar geometría y estado
	//
	restoreGeometry(s.value("WindowGeometry").toByteArray());
	restoreState(s.value("WindowState").toByteArray());

	imageViewer->setBackgroundColor(QColor(s.value("BackgroundColor", "black").toString()));

	//
	// Cargar los el primer archivo pasado por argumentos
	//
	QStringList args = qApp->arguments();
	if(args.size() == 1)
		openFile(QString());
	else
		openFile(args.at(1));
}

MainWindow::~MainWindow()
{
	QSettings s(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);
	{
		s.setValue("WindowGeometry", saveGeometry());
		s.setValue("WindowState", saveState());

		s.setValue("Language", languages.language());

		s.setValue("BackgroundColor", imageViewer->getBackgroundColor().name());
	}
	delete ui;
}
void MainWindow::openFile(const QString& fname)
{
	QImage image = QImage(fname);
	if(image.isNull())
	{
		qDebug("The image is null");
		qDebug(fname.toLatin1().constData());

		imageViewer->load(QImage());
	}
	else
	{
		imageViewer->load(image);

		QFileInfo fi(fname);
		QDirIterator iter(fi.absoluteDir().absolutePath(), availableFormats);

		files.clear();
		while(iter.hasNext())
		{
			iter.next();
			files.append(iter.filePath());
		}
		fileIndex = files.indexOf(fi.absoluteFilePath());
		ui->statusBar->showMessage(fi.absoluteFilePath());
	}
	ui->actionFileCopyPath->setEnabled(!image.isNull());
	ui->actionFileCopyImage->setEnabled(!image.isNull());
	ui->actionFileWallpaper->setEnabled(!image.isNull());
	ui->actionFileProperties->setEnabled(!image.isNull());
	ui->actionFileOpenContainerFolder->setEnabled(!image.isNull());
}
void MainWindow::changeEvent(QEvent* e)
{
	if(e != NULL)
	{
		switch(e->type())
		{
#if 1
			case QEvent::LocaleChange:
				{
					QString locale = QLocale::system().name();
					locale.truncate(locale.lastIndexOf('_'));
					languages.load(locale);
				}
				break;
#endif
			case QEvent::LanguageChange:
				ui->retranslateUi(this);
				menuLanguages->setTitle(tr("Languages"));
				break;
			default:
				break;
		}
	}
	QWidget::changeEvent(e);
}
void MainWindow::dropEvent(QDropEvent* e)
{
	QString s;
	const QList<QUrl> urls = e->mimeData()->urls();

	e->acceptProposedAction();
	// ...
	for(int i = 0; i < urls.size(); i++)
	{
		s = urls.at(i).toLocalFile();
		if(!s.isEmpty())
		{
			openFile(s);
		}
	}
}
void MainWindow::dragEnterEvent(QDragEnterEvent* e)
{
	if(e->mimeData()->hasFormat("text/uri-list"))
	{
		e->acceptProposedAction();
	}
}
void MainWindow::contextMenuEvent(QContextMenuEvent* e)
{
	Q_UNUSED(e);
	QMenu menu;

	menu.addAction(ui->actionFileExit);
	menu.addSeparator();
	menu.addAction(ui->actionViewNormal);
	menu.addAction(ui->actionViewAspectRatio);
	menu.addSeparator();
	menu.addAction(ui->actionFileCopyPath);
	menu.addAction(ui->actionFileCopyImage);
	menu.addSeparator();
	menu.addAction(ui->actionFileProperties);
	menu.addSeparator();
	menu.addAction(ui->actionFileOpenContainerFolder);
	menu.addSeparator();
	menu.addAction(ui->actionFileWallpaper);
	menu.addSeparator();
	menu.addMenu(menuLanguages);
	menu.addSeparator();
#ifdef USE_CONFIG
	menu.addAction(ui->actionOptionsSettings);
	menu.addSeparator();
#endif
	menu.addAction(ui->actionHelpAbout);
	menu.exec(QCursor::pos());
}
bool MainWindow::eventFilter(QObject* target, QEvent* event)
{
	if(event->type() == QEvent::KeyPress)
	{
		QKeyEvent* e = (QKeyEvent*)event;
		switch(e->key())
		{
			case Qt::Key_Left:
				if(fileIndex > 0)
				{
					fileIndex--;
					imageViewer->load(QImage(files.at(fileIndex)));
					ui->statusBar->showMessage(files.at(fileIndex));
				}
				break;
			case Qt::Key_Right:
				if(fileIndex < files.length()-1)
				{
					fileIndex++;
					imageViewer->load(QImage(files.at(fileIndex)));
					ui->statusBar->showMessage(files.at(fileIndex), -1);
				}
				break;
		}
	}
	return QMainWindow::eventFilter(target, event);
}
void MainWindow::slotAction()
{
	QAction* action = qobject_cast<QAction*>(sender());

	if(action == ui->actionFileExit)
	{
		close();
	}
	else if(action == ui->actionFileWallpaper)
	{
		if(files.isEmpty())
		{
			QMessageBox::information(0,
									 tr("Properties"),
									 tr("There is no image"),
									 QMessageBox::Ok);
		}
		else
		{
			SetWallpaper(files.at(fileIndex));
		}
	}
	else if(action == ui->actionFileCopyPath)
	{
		if(!files.isEmpty() && fileIndex != -1)
		{
			QApplication::clipboard()->setText("\"" + QDir::toNativeSeparators(files.at(fileIndex)) + "\"");
		}
	}
	else if(action == ui->actionFileCopyImage)
	{
		QApplication::clipboard()->setImage(imageViewer->getImage());
	}
	else if(action == ui->actionFileProperties)
	{
		if(files.isEmpty())
		{
			QMessageBox::information(0,
									 tr("Properties"),
									 tr("There is no image"),
									 QMessageBox::Ok);
		}
		else
		{
			PropertiesDialog(files.at(fileIndex),
							 imageViewer->getImageWidth(),
							 imageViewer->getImageHeight()).exec();
		}
	}
	else if(action == ui->actionFileOpenContainerFolder)
	{
		ShowInContainerFolder(files.at(fileIndex));
	}
	else if(action == ui->actionHelpAbout)
	{
		AboutDialog().exec();
	}
	else if(action == ui->actionViewNormal)
	{
		imageViewer->setSizeMode(ImageViewer::SIZEMODE_NORMAL);
		imageViewer->setScaleFactor(1);
	}
	else if(action == ui->actionViewAspectRatio)
	{
		imageViewer->setSizeMode(ImageViewer::SIZEMODE_ASPECTRATIO);
	}
#ifdef USE_CONFIG
	else if(action == ui->actionOptionsSettings)
	{
		SettingsDialog(availableFormats).exec();
	}
#endif
}


