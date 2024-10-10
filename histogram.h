#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QWidget>
#include <QtWidgets>
#include "labelwithmouse.h"

class Histogram : public QWidget
{
    Q_OBJECT

    QVector<int> pixelCounts;
    QVector<int> pixelSumCounts;
    QComboBox *contrastSelector;
    QPixmap histoBlock;
    QVector<QPixmap> colorPixmaps;
    LabelWithMouse *picLabel;
    QHBoxLayout *hLayout;
    QPushButton *refresh;
    QCheckBox *useSum;
    QComboBox *rgbSelector;
    QSlider *histoColorSlider;
public:
    Histogram(const QImage image);
    void makeHistoOptions();

signals:
    //void mouseMoved(QPoint pos, QColor color);

public slots:
    void changePixelArray();
    void colorSliderChanged();
    void mouseMoved(QPoint pos);

};


class Waiter{
    static int count;
public:
    Waiter();
    ~Waiter();
};

#endif // HISTOGRAM_H
