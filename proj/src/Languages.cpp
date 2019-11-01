#include "Languages.h"
#include <QLocale>
#include <QDir>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QApplication>

Languages::Languages()
{
}
Languages::~Languages()
{
}
void Languages::load(QString language)
{
	if(currentLanguage != language)
	{
		currentLanguage = language;

		QLocale locale = QLocale(language);
		QLocale::setDefault(locale);
		//QString languageName = QLocale::languageToString(locale.language());

		QString fileName = languagesPath + "/" + QString(baseName + "_%1.qm").arg(language);

		// Remover el viejo traductor
		qApp->removeTranslator(&currentTranslator);

		// Cargar el nuevo traductor
		if(currentTranslator.load(fileName))
		{
			qApp->installTranslator(&currentTranslator);
		}
		else
		{
			// TODO: ¿Notificar error?
		}
	}
}
void Languages::init(QMenu* menu, QString basePath, QString baseName, QString initialLocale)
{
	this->baseName = baseName;
	// ...
	QActionGroup* langGroup = new QActionGroup(this);
	langGroup->setExclusive(true);

	connect(langGroup, SIGNAL(triggered(QAction*)), this, SLOT(actionTriggered(QAction*)));

	//
	// Si no hay un lenguaje inicial, usar el del sistema
	//
	if(initialLocale.isEmpty())
	{
		QString defaultLocale = QLocale::system().name();
		defaultLocale.truncate(defaultLocale.lastIndexOf('_'));

		initialLocale = defaultLocale;
	}

	//
	// Obtener la carpeta de donde están los lenguajes
	//
	languagesPath = QApplication::applicationDirPath();
	if(!basePath.isEmpty() && (basePath.at(0) != '/' && basePath.at(0) != '\\'))
	{
		languagesPath += '/';
	}
	languagesPath.append(basePath);

	//
	// Obtener el listado de los archivos de lenguaje
	//
	QDir dir(languagesPath);
	QStringList fileNames = dir.entryList(QStringList(baseName + "_*.qm"));

	//
	// Enumarar cada uno e ir creando una acción para ir agregandola al menú
	//
	for(int i = 0; i < fileNames.size(); i++)
	{
		QString locale;

		locale = fileNames[i];
		locale.truncate(locale.lastIndexOf('.'));
		locale.remove(0, locale.indexOf('_') + 1);

		QString lang = QLocale::languageToString(QLocale(locale).language());
		QIcon ico(QString("%1/%2.png").arg(languagesPath).arg(locale));

		QAction* action = new QAction(ico, lang, this);
		action->setCheckable(true);
		action->setData(locale);

		menu->addAction(action);
		langGroup->addAction(action);

		if(initialLocale == locale)
		{
			action->setChecked(true);
		}
	}
	//
	// Cargar lenguaje inicial
	//
	load(initialLocale);
}
void Languages::actionTriggered(QAction* action)
{
	if(action != NULL)
	{
		load(action->data().toString());
	}
}

