#ifndef KAP_IMAGE_PLUGIN_H
#define KAP_IMAGE_PLUGIN_H

#include <QImageIOPlugin>

class KAPPlugin : public QImageIOPlugin
{
public:
	QStringList			keys() const;
	Capabilities		capabilities(QIODevice* device, const QByteArray& format) const;
	QImageIOHandler*	create(QIODevice* device, const QByteArray& format) const;
};

#endif
