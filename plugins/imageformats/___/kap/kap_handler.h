#ifndef KAP_IMAGE_HANDLER_H
#define KAP_IMAGE_HANDLER_H

#include <QImageIOHandler>

class KAPHandler : public QImageIOHandler
{
public:
	KAPHandler();
	~KAPHandler();
public:
	bool				canRead() const;
	bool				read(QImage* image);
	bool				jumpToNextImage();
	int					currentImageNumber() const;
	int					imageCount();
};

#endif
