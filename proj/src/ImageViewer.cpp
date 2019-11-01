#include "ImageViewer.h"
#include <QLabel>
#include <QScrollBar>
#include <QPainter>
#include <QMouseEvent>


enum DRAWALIGN
{
	DRAWALIGN_HCENTER		 = 0,
	DRAWALIGN_LEFT			 = 0x00000001,
	DRAWALIGN_RIGHT			 = 0x00000002,
	DRAWALIGN_VCENTER		 = 0,
	DRAWALIGN_TOP			 = 0x00000004,
	DRAWALIGN_BOTTOM		 = 0x00000008,
};

QImage background = QImage(":/background.png");

ImageViewer::ImageViewer(QWidget *parent) : QAbstractScrollArea(parent)
{
	spaceSize = 0;
	sizeMode = SIZEMODE_NORMAL;
	isDragging = false;
	scaleFactor = 1;
	connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(slotScrollBar_valueChanged(int)));
	connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(slotScrollBar_valueChanged(int)));

//	setSizeMode(SIZEMODE_NORMAL);
}
ImageViewer::~ImageViewer()
{
}
void ImageViewer::load(const QImage& img)
{
	image = img;
	buildImageRect();
	updateScrollBars();
	viewport()->update();

	hasTransparency = false;
	if(!image.isNull())
	{
		for(int y = 0; y < image.height() && !hasTransparency; y++)
		{
			for(int x = 0; x < image.width() && !hasTransparency; x++)
			{
				if(image.pixelColor(x, y).alpha() != 255)
				{
					hasTransparency = true;
				}
			}
		}
	}

	isDragging = false;
	scaleFactor = 1.0;

	if(!image.isNull() && sizeMode == SIZEMODE_NORMAL)
		setCursor(QCursor(Qt::OpenHandCursor));
	else
		setCursor(QCursor(Qt::ArrowCursor));
}
void ImageViewer::setSizeMode(int mode)
{
	if(sizeMode != mode)
	{
		sizeMode = mode;
		scaleFactor = 1.0;
		buildImageRect();
		updateScrollBars();
		viewport()->update();
	}
}
void ImageViewer::wheelEvent(QWheelEvent* e)
{
	if(!image.isNull())
	{
		if(e->modifiers() & Qt::ControlModifier)
		{
			float f;

			f = scaleFactor + 0.001*e->delta();
			if(f < 32.0/image.width())
				f = 32.0/image.width();

			setScaleFactor(f);
		}
		else
		{
			if(e->modifiers() & Qt::ShiftModifier)
				horizontalScrollBar()->setValue(horizontalScrollBar()->value()-e->delta());
			else
				verticalScrollBar()->setValue(verticalScrollBar()->value()-e->delta());
		}
	}
}
void ImageViewer::paintEvent(QPaintEvent* e)
{
	Q_UNUSED(e);
	QPainter p(viewport());

	p.fillRect(rect(), backgroundColor);
	if(hasTransparency)
	{
		QBrush br(background);
		p.fillRect(imageRect, br);
	}
	p.drawImage(imageRect, image);
}
void ImageViewer::resizeEvent(QResizeEvent* e)
{
	Q_UNUSED(e);
	buildImageRect();
	updateScrollBars();
}
void ImageViewer::mouseMoveEvent(QMouseEvent* e)
{
	if(isDragging)
	{
		verticalScrollBar()->setValue(-(e->globalPos() - deltaSize).y());
		horizontalScrollBar()->setValue(-(e->globalPos() - deltaSize).x());
	}
}
void ImageViewer::mousePressEvent(QMouseEvent* e)
{
	if(e->button() == Qt::LeftButton)
	{
		if(!image.isNull())
		{
			viewport()->grabMouse(QCursor(Qt::ClosedHandCursor));

			deltaSize = e->globalPos() - QPoint(-horizontalScrollBar()->value(), -verticalScrollBar()->value());
			isDragging = true;
		}
	}
}
void ImageViewer::mouseReleaseEvent(QMouseEvent* e)
{
	if(e->button() == Qt::LeftButton)
	{
		if(isDragging)
		{
			viewport()->releaseMouse();
			isDragging = false;
		}
	}
}
void ImageViewer::buildImageRect()
{
	if(!image.isNull())
	{
		switch(sizeMode)
		{
			case SIZEMODE_NORMAL:
				buildImageRectNormal();
				break;
			case SIZEMODE_STRETCH:
				buildImageRectStretch();
				break;
			case SIZEMODE_ASPECTRATIO:
				buildImageRectAspectRatio();
				break;
		}
	}
}
void ImageViewer::buildImageRectNormal()
{
	int x = 0;
	int y = 0;
	int sp = spaceSize + ((showBorder) ? 1 : 0);
	int iImageWidth = image.width()*scaleFactor;
	int iImageHeight = image.height()*scaleFactor;
	int iWindowWidth = viewport()->width();
	int iWindowHeight = viewport()->height();
	int iWorkspaceWidth = iImageWidth;
	int iWorkspaceHeight = iImageHeight;

	iWorkspaceWidth += sp*2;
	iWorkspaceHeight += sp*2;

	if(iWorkspaceWidth < iWindowWidth)
	{
		if(drawAlign & DRAWALIGN_LEFT)
			x = 0;
		else if(drawAlign & DRAWALIGN_RIGHT)
			x = iWindowWidth - iWorkspaceWidth;
		else
			x = (iWindowWidth - iWorkspaceWidth)/2;
	}
	else
	{
		x = -horizontalScrollBar()->value();
	}
	if(iWorkspaceHeight < iWindowHeight)
	{
		if(drawAlign & DRAWALIGN_TOP)
			y = 0;
		else if(drawAlign & DRAWALIGN_BOTTOM)
			y = iWindowHeight - iWorkspaceHeight;
		else
			y = (iWindowHeight - iWorkspaceHeight)/2;
	}
	else
	{
		y = -verticalScrollBar()->value();
	}
	imageRect = QRect(x+sp, y+sp, iImageWidth, iImageHeight);
	workspaceRect = QRect(x, y, iWorkspaceWidth, iWorkspaceHeight);
}
void ImageViewer::buildImageRectStretch()
{
	int x = 0;
	int y = 0;
	int w = viewport()->width();
	int h = viewport()->height();
	int sp = spaceSize + ((showBorder) ? 1 : 0);

	imageRect = QRect(x+sp, y+sp, w-(sp*2), h-(sp*2));
	workspaceRect = QRect(x, y, w, h);
}
void ImageViewer::buildImageRectAspectRatio()
{
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	int sp = spaceSize + ((showBorder) ? 1 : 0);
	int iImageWidth = image.width();
	int iImageHeight = image.height();
	double dWindowWidth = viewport()->width();
	double dWindowHeight = viewport()->height();
	double dWorkspaceWidth = iImageWidth + (double)sp*2.0;
	double dWorkspaceHeight = iImageHeight + (double)sp*2.0;

	double dImageAspectRatio = dWorkspaceWidth / dWorkspaceHeight;
	double dWindowAspectRatio = dWindowWidth / dWindowHeight;

	if(dImageAspectRatio > dWindowAspectRatio)
	{
		long lNewHeight = long(dWindowWidth / dWorkspaceWidth * dWorkspaceHeight);

		x = 0;
		y = (dWindowHeight - lNewHeight) / 2;
		w = dWindowWidth;
		h = lNewHeight;
	}
	else if(dImageAspectRatio < dWindowAspectRatio)
	{
		long lNewWidth = long(dWindowHeight / dWorkspaceHeight * dWorkspaceWidth);

		x = (dWindowWidth - lNewWidth) / 2;
		y = 0;
		w = lNewWidth;
		h = dWindowHeight;
	}
	imageRect = QRect(x+sp, y+sp, w-(sp*2), h-(sp*2));
	workspaceRect = QRect(x, y, w, h);
}
void ImageViewer::updateScrollBars()
{
	if(image.isNull() || sizeMode != SIZEMODE_NORMAL)
	{
		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setVerticalScrollBarPolicy  (Qt::ScrollBarAlwaysOff);
		return;
	}
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setVerticalScrollBarPolicy  (Qt::ScrollBarAsNeeded);

	int hmax;
	int vmax;
	int width = viewport()->width();
	int height = viewport()->height();
	QScrollBar* hbar = horizontalScrollBar();
	QScrollBar* vbar = verticalScrollBar();

	if(spaceSize == 0)
	{
		if(!showBorder)
			hmax = qMax(0, int(image.width() * scaleFactor) - width);
		else
			hmax = qMax(0, int((image.width() * scaleFactor)+2) - width);

		if(!showBorder)
			vmax = qMax(0, int(image.height() * scaleFactor) - height);
		else
			vmax = qMax(0, int((image.height() * scaleFactor)+2) - height);
	}
	else
	{
		if(!showBorder)
			hmax = qMax(0, int((image.width() * scaleFactor)+(spaceSize*2)) - width);
		else
			hmax = qMax(0, int((image.width() * scaleFactor)+2+(spaceSize*2)) - width);

		if(!showBorder)
			vmax = qMax(0, int((image.height() * scaleFactor)+(spaceSize*2)) - height);
		else
			vmax = qMax(0, int((image.height() * scaleFactor)+2+(spaceSize*2)) - height);
	}
	hbar->setRange(0, hmax);
	hbar->setPageStep(width);
	vbar->setRange(0, vmax);
	vbar->setPageStep(height);
}
void ImageViewer::setScaleFactor(double factor)
{
	if(scaleFactor != factor && (factor > 0.001 && factor < 1000))
	{
#if 0
		double zoom = factor;
		double oldZoom = scaleFactor;

		QPoint center = QPoint(viewport()->width() / 2, viewport()->height() / 2);

		int left = qMax( (viewport()->width() - image.width()) / 2, 0);
		int top = qMax( (viewport()->height() - image.height()) / 2, 0);
		QPoint oldOffset = QPoint(left, top);

		QPointF oldScroll = QPointF(horizontalScrollBar()->value(), verticalScrollBar()->value()) - oldOffset;

		QPointF scroll = (zoom / oldZoom) * (oldScroll + center) - center;

		scaleFactor = factor;
		buildImageRect();
		updateScrollBars();
		horizontalScrollBar()->setValue(qAbs(int(scroll.x())));
		verticalScrollBar()->setValue(qAbs(int(scroll.y())));
#else
		scaleFactor = factor;
		buildImageRect();
		updateScrollBars();
#endif
		viewport()->update();
	}
}
void ImageViewer::slotScrollBar_valueChanged(int value)
{
	Q_UNUSED(value);
	buildImageRect();
	viewport()->update();
}

