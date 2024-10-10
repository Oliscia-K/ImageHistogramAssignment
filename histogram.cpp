#include "histogram.h"
#include "imageviewer.h"
#include <QtWidgets>
#include "mainwindow.h"
#include "labelwithmouse.h"

Histogram::Histogram(const QImage image) {
    ImageViewer *imageViewer = new ImageViewer(image);

    hLayout = new QHBoxLayout();
    QVBoxLayout *layout = new QVBoxLayout();

    histoBlock = QPixmap(256, 256);
    picLabel = new LabelWithMouse();
    picLabel->setPixmap(histoBlock);

    connect(picLabel, &LabelWithMouse::mouseMoved, this, &Histogram::mouseMoved);

    histoColorSlider = new QSlider(Qt::Horizontal);
    histoColorSlider->setMinimum(0);
    histoColorSlider->setMaximum(255);

    rgbSelector = new QComboBox();
    rgbSelector->addItem("Red");
    rgbSelector->addItem("Green");
    rgbSelector->addItem("Blue");

    contrastSelector = new QComboBox();
    for (int i = 0; i < 8; ++i){
        contrastSelector->addItem(QString::number(1<<i));
    }

    useSum = new QCheckBox("Display by sum");
    refresh = new QPushButton("Click me after changing the color,\n contrast,\n and/or display by sum\n option to see new histograms!");

    connect(histoColorSlider, &QSlider::valueChanged, this, &Histogram::colorSliderChanged);
    connect(refresh, &QPushButton::clicked, this, &Histogram::changePixelArray);

    layout->addWidget(picLabel);
    layout->addSpacing(5);
    layout->addWidget(useSum);
    layout->addSpacing(5);
    layout->addWidget(histoColorSlider);
    layout->addSpacing(5);
    layout->addWidget(rgbSelector);
    layout->addSpacing(5);
    layout->addWidget(contrastSelector);
    layout->addSpacing(5);
    layout->addWidget(refresh);
    layout->addStretch(1);

    hLayout->addWidget(imageViewer, 1);
    hLayout->addLayout(layout);

    setLayout(hLayout);

    pixelCounts = QVector<int>((1<<24), 0);
    for (int row = 0; row < image.height(); ++row){
        for (int col =0; col < image.width(); ++col){
            QRgb colorValues =  image.pixel(col, row);
            ++pixelCounts[colorValues & 0xffffff];
        }
    }

    makeHistoOptions();
    setMouseTracking(true);
}

int Waiter::count = 0;

Waiter::Waiter(){
    if (++count == 1){
        mainWindow->statusBar()->setStyleSheet("color:red");
        mainWindow->statusBar()->showMessage("Just a moment, please :)");
        mainWindow->setEnabled(false);
        mainWindow->statusBar()->show();
        QApplication::processEvents();
    }
}
Waiter::~Waiter(){
    if (--count == 0){
        mainWindow->statusBar()->setStyleSheet("color:black");
        mainWindow->statusBar()->clearMessage();
        mainWindow->setEnabled(true);
    }
}

void Histogram::makeHistoOptions(){
    Waiter waiter;
    if (useSum->checkState()==0){
        histoColorSlider->setMaximum(255);
        colorPixmaps = QVector<QPixmap>(256);
        int contrastIndex = contrastSelector->currentIndex();
        for (int maps = 0; maps < 256; ++maps){
            QImage currentImage = histoBlock.toImage();
            if (useSum->checkState() == 0){
                for (int row = 0; row < 256; ++row){
                    for (int col = 0; col < 256; ++col){
                        int scaled = 64 + (1 << contrastIndex) * pixelCounts[qRgb(maps, col, row) & 0xffffff];
                        if (scaled > 255) scaled = 255;
                        if (pixelCounts[qRgb(maps, col, row) & 0xffffff] > 0){
                            currentImage.setPixelColor(col, row, qRgb(scaled, scaled, scaled));
                        }else{
                            currentImage.setPixelColor(col, row, qRgb(64, 64, 64));
                        }
                    }
                }
            }
            colorPixmaps.insert(maps, QPixmap::fromImage(currentImage));
        }
        picLabel->setPixmap(colorPixmaps[histoColorSlider->value()]);
    }
    else{
        histoColorSlider->setMaximum(3*255);
        colorPixmaps.resize(3*255+1);
        int colorIndex = rgbSelector->currentIndex();
        int contrastIndex = contrastSelector->currentIndex();
        for (int maps = 0; maps <= 3*255; ++maps){
            QImage currentImage = histoBlock.toImage();
            for (int row = 0; row < 256; ++row){
                for(int col = 0; col < 256; ++col){
                    int sum = maps - col - row;
                    if (sum >= 0){
                        if (colorIndex == 0){
                            int scaled = 64 + (1 << contrastIndex) * pixelCounts[qRgb(sum, col, row) & 0xffffff];
                            if (scaled > 255) scaled = 255;
                            if ( pixelCounts[qRgb(sum, col, row) & 0xffffff] > 0){
                                currentImage.setPixelColor(col, row, qRgb(scaled, scaled, scaled));
                            }else{
                                currentImage.setPixelColor(col, row, qRgb(64, 64, 64));
                            }
                        }
                        if (colorIndex == 1){
                            int scaled = 64 + (1 << contrastIndex) * pixelCounts[qRgb(col, sum, row) & 0xffffff];
                            if (scaled > 255) scaled = 255;
                            if ( pixelCounts[qRgb(col, sum, row) & 0xffffff] > 0){
                                currentImage.setPixelColor(col, row, qRgb(scaled, scaled, scaled));
                            }else{
                                currentImage.setPixelColor(col, row, qRgb(64, 64, 64));
                            }
                        }
                        if (colorIndex == 2){
                            int scaled = 64 + (1 << contrastIndex) * pixelCounts[qRgb(col, row, sum) & 0xffffff];
                            if (scaled > 255) scaled = 255;
                            if ( pixelCounts[qRgb(col, row, sum) & 0xffffff] > 0){
                                currentImage.setPixelColor(col, row, qRgb(scaled, scaled, scaled));
                            }else{
                                currentImage.setPixelColor(col, row, qRgb(64, 64, 64));
                            }
                        }
                    }
                }
            }
            colorPixmaps.insert(maps, QPixmap::fromImage(currentImage));
        }
        picLabel->setPixmap(colorPixmaps[histoColorSlider->value()]);
    }
}

void Histogram::changePixelArray(){
    makeHistoOptions();
}

void Histogram::colorSliderChanged(){
    picLabel->setPixmap(colorPixmaps[histoColorSlider->value()]);
}

void Histogram::mouseMoved(QPoint pos){
    int colorFiller = histoColorSlider->value();
    if (useSum->checkState() == 2){
        colorFiller = histoColorSlider->value() - pos.x() - pos.y();
        if (colorFiller < 0) colorFiller = 0;
    }
    int colorIndex = rgbSelector->currentIndex();
    if (colorIndex == 0){
        QRgb color = qRgb(colorFiller, pos.x(), pos.y());
        mainWindow->statusBar()->showMessage(QString("Position (%1,%2), Color (R,G,B) = (%3,%4,%5)").arg(pos.x()).arg(pos.y()).arg(qRed(color)).arg(qGreen(color)).arg(qBlue(color)));
    }
    if (colorIndex == 1){
        QRgb color = qRgb(pos.x(), colorFiller, pos.y());
        mainWindow->statusBar()->showMessage(QString("Position (%1,%2), Color (R,G,B) = (%3,%4,%5)").arg(pos.x()).arg(pos.y()).arg(qRed(color)).arg(qGreen(color)).arg(qBlue(color)));
    }
    if (colorIndex == 2){
        QRgb color = qRgb(pos.x(), pos.y(), colorFiller);
        mainWindow->statusBar()->showMessage(QString("Position (%1,%2), Color (R,G,B) = (%3,%4,%5)").arg(pos.x()).arg(pos.y()).arg(qRed(color)).arg(qGreen(color)).arg(qBlue(color)));
    }

}
