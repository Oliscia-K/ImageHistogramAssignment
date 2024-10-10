#include "mainwindow.h"
#include <QtWidgets>

#include "histogram.h"
MainWindow *mainWindow;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent){
    mainWindow = this;
    QAction *openFileAct = new QAction("Open image file");
    connect(openFileAct, &QAction::triggered, this, &MainWindow::openFileSlot);
    openFileAct->setShortcut(Qt::CTRL | Qt::Key_O);

    QMenu *fileMenu = new QMenu("File");
    fileMenu->addAction(openFileAct);
    menuBar()->addMenu(fileMenu);

    QSettings settings("Oliscia's Systems", "Graphics");
    lastDir = settings.value("lastDir", "").toString();
}

MainWindow::~MainWindow() {
    QSettings settings("Oliscia's Systems", "Graphics");
    settings.setValue("lastDir", lastDir);
}

void MainWindow::openFileSlot(){
    QString fName = QFileDialog::getOpenFileName(this, "Select image file", lastDir, "Image files (*.png *.jpg *.bmp)");
    if (fName.isEmpty()) return;
    QImage image(fName);
    if (image.isNull()) return;
    lastDir = QFileInfo(fName).absolutePath();

    Histogram *fullHisto = new Histogram(image);
    setCentralWidget(fullHisto);
}
