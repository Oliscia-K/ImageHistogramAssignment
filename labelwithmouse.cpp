#include "labelwithmouse.h"
#include <QtWidgets>
#include "mainwindow.h"

LabelWithMouse::LabelWithMouse() {
    setMouseTracking(true);
}

void LabelWithMouse::mouseMoveEvent(QMouseEvent *evt){
    QPoint pos = (evt->pos())-QPointF(0.5,0.5).toPoint();
    if (pos.x() < 256 && pos.x() > -1 && pos.y() < 256 && pos.y() > -1)
        emit mouseMoved(pos);
}

