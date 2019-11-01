#include "kap_plugin.h"
#include "kap_handler.h"

QStringList			KAPPlugin::keys() const
{
	return QStringList() << "kap";
}
QImageIOPlugin::Capabilities		KAPPlugin::capabilities(QIODevice* device, const QByteArray& format) const
{
	if(format == "kap" || format == "KAP")
		return Capabilities(CanRead | CanWrite);
	if(!format.isEmpty())
		return 0;
	if (!device->isOpen())
		return 0;

	Capabilities cap;
#if 1
	if(device->isReadable())
#else
	if(device->isReadable() && KAPHandler::canRead(device))
#endif
		cap |= CanRead;
#if 0
	if(device->isWritable())
		cap |= CanWrite;
#endif
	return cap;
}
QImageIOHandler*	KAPPlugin::create(QIODevice* device, const QByteArray& format) const
{
	QImageIOHandler* handler = new KAPHandler();
	handler->setDevice(device);
	handler->setFormat(format);
	return handler;
}

Q_EXPORT_STATIC_PLUGIN(KAPPlugin)
Q_EXPORT_PLUGIN2(tga, KAPPlugin)




