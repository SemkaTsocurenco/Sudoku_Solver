#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    setupConnections();


}

MainWindow::~MainWindow() {
}

void MainWindow::setupUi() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

}



void MainWindow::setupConnections() {

}

