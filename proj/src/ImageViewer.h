#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QScrollArea>

class QLabel;

class ImageViewer : public QAbstractScrollArea
{
	Q_OBJECT
	QImage image;
	int imageX = 0;
	int imageY = 0;
	bool isDragging;
	QPoint deltaSize;
	int		sizeMode;

	QRect	imageRect;
	QRect	workspaceRect;
	bool	showBorder = false;
	int		spaceSize = 0;
	double	scaleFactor;
	int		drawAlign = 0;
	bool hasTransparency = false;
	QColor	backgroundColor;
public:
	enum
	{
		SIZEMODE_NORMAL,
		SIZEMODE_STRETCH,
		SIZEMODE_ASPECTRATIO,
	};

public:
	explicit ImageViewer(QWidget *parent = 0);
	~ImageViewer();
public:
	void load(const QImage& img);
	void setSizeMode(int mode);
	void setScaleFactor(double factor);
	QColor getBackgroundColor() const
	{
		return backgroundColor;
	}
	void setBackgroundColor(const QColor& color)
	{
		backgroundColor = color;
		viewport()->update();
	}
	QImage getImage() const
	{
		return image;
	}
	int getImageWidth() const
	{
		return image.width();
	}
	int getImageHeight() const
	{
		return image.height();
	}
private:
	void wheelEvent(QWheelEvent* e);
	void paintEvent(QPaintEvent* e);
	void resizeEvent(QResizeEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
	void mousePressEvent(QMouseEvent* e);
	void mouseReleaseEvent(QMouseEvent* e);
private:
	void buildImageRect();
	void buildImageRectNormal();
	void buildImageRectStretch();
	void buildImageRectAspectRatio();
	void updateScrollBars();
signals:
private slots:
	void slotScrollBar_valueChanged(int value);
};

#endif // IMAGEVIEWER_H
