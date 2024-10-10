#include "imageviewer.h"
#include <QtWidgets>

//must construct inner thins first
ImageViewer::ImageViewer(const QImage &_image):image(_image),scale(1.0) {
    setScene(&scene);
    giPixmap = scene.addPixmap(QPixmap::fromImage(image));

    setBackgroundBrush(Qt::lightGray);
    setSceneRect(QRectF(QPointF(0,0), image.size()));
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
}

