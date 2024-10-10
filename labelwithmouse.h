#ifndef LABELWITHMOUSE_H
#define LABELWITHMOUSE_H

#include <QLabel>

class LabelWithMouse : public QLabel
{
    Q_OBJECT
public:
    LabelWithMouse();

signals:
    void mouseMoved(QPoint pos);

public slots:
    void mouseMoveEvent(QMouseEvent *evt) override;
};

#endif // LABELWITHMOUSE_H
