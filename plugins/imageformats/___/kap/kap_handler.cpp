#include "kap_handler.h"
#include "bsb.h"
#include <QImage>
#include <QPixmap>

KAPHandler::KAPHandler()
{
}
KAPHandler::~KAPHandler()
{
}
bool				KAPHandler::canRead() const
{
#if 1
	return true;
#else
	if(canRead(device()))
	{
		setFormat("kap");
		return true;
	}
	return false;
#endif
}
bool				KAPHandler::read(QImage* image)
{
	int x;
	int y;
	BSBImage bsb;
	uint8_t			*buf;
	QIODevice* dev = device();
	QDataStream strm(dev);
//	strm.setByteOrder( QDataStream::BigEndian );
	strm.setByteOrder( QDataStream::LittleEndian);

	if(!bsb_open_header(&strm, &bsb))
	{
		return false;
	}
	buf = (uint8_t *)malloc(bsb.width);

	*image = QImage(bsb.width, bsb.height, QImage::Format_RGB32);
	uint *pixel = (uint *) image->scanLine(0);

	/* Read rows from bsb file and write rows to PPM */
	for (y = 0; y < bsb.height; y++)
	{
		bsb_seek_to_row(&bsb, y);
		bsb_read_row(&bsb, buf);

		/* Each pixel is a triplet of Red,Green,Blue samples */
		for (x = 0; x < bsb.width; x++)
		{
			QRgb c = qRgb(
					bsb.red[(int)buf[x]],
					bsb.green[(int)buf[x]],
					bsb.blue[(int)buf[x]]);

			*pixel++ = c;
		}
	}
	free(buf);

	bsb_close(&bsb);

	return true;
}
bool				KAPHandler::jumpToNextImage()
{
	return false;
}
int					KAPHandler::currentImageNumber() const
{
	return 0;
}
int					KAPHandler::imageCount()
{
	return 0;
}


