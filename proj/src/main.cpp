#include "MainWindow.h"
#include <QApplication>

#include <QPluginLoader>

#include <iostream>
#include <QImageReader>
#include <QJsonArray>






int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QString path = QCoreApplication::applicationDirPath();

	QApplication::addLibraryPath(path + "/imageformats");

	QList<QByteArray> list = QImageReader::supportedImageFormats();
	for(int i = 0; i < list.size(); ++i)
	{
		std::cout << list.at(i).constData() << std::endl;
	}
	MainWindow w;
	w.show();
	int ret = a.exec();

	return ret;
}
