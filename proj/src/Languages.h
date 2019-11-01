#ifndef LANGUAGES_H
#define LANGUAGES_H
#include <QObject>
#include <QTranslator>

class QMenu;
class QAction;

class Languages : public QObject
{
	Q_OBJECT
	QString		baseName;
	QString		languagesPath;
	QString		currentLanguage;
	QTranslator	currentTranslator;
public:
    Languages();
	~Languages();
public:
	void load(QString language);
	void init(QMenu* menu, QString basePath, QString baseName, QString initialLocale);
	QString language() const { return currentLanguage; }
private slots:
	void actionTriggered(QAction* action);
};

#endif // LANGUAGES_H
