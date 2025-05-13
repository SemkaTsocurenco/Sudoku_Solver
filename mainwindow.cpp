#include "mainwindow.h"
#include <iostream>
#include <qpushbutton.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    setupConnections();


}

MainWindow::~MainWindow() {
}

void MainWindow::setupUi() {

    this->setFixedSize({450,500});

    centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    auto vLay = new QVBoxLayout(centralWidget);

    grid = new QGridLayout(centralWidget);

    solve = new QPushButton("SOLVE",this);
    solve->setStyleSheet(R"(
        QPushButton {
            background-color: #2e86de;
            color: white;
            font-weight: bold;
            font-size: 16px;
            border: none;
            border-radius: 8px;
            padding: 8px 16px;
        }
        QPushButton:hover {
            background-color: #1b4f72;
        }
        QPushButton:pressed {
            background-color: #154360;
        }
    )");

    vLay->addItem(grid);
    vLay->addWidget(solve);
    centralWidget->setLayout(vLay);



    for (int col = 0 ; col < 9 ; col ++){
        for (int row = 0 ; row < 9 ; row ++){
            auto spin = new QLineEdit(this);
            spin->setValidator(new QIntValidator(1, 9, this));
            spin->setFixedSize(50, 50);
            spin->setAlignment(Qt::AlignCenter);

            int top = (col % 3 == 0) ? 5 : 1;
            int left = (row % 3 == 0) ? 5 : 1;
            int right = ((row + 1) % 3 == 0) ? 5 : 1;
            int bottom = ((col + 1) % 3 == 0) ? 5 : 1;

            QString style = QString(R"(
                QLineEdit {
                    background-color:rgb(218, 251, 255);
                    color: #2e86de;
                    font-weight: bold;
                    font-size: 20px;
                    border-top: %1px solid black;
                    border-left: %2px solid black;
                    border-right: %3px solid black;
                    border-bottom: %4px solid black;
                    qproperty-alignment: AlignCenter;
                }
            )").arg(top).arg(left).arg(right).arg(bottom);

            spin->setStyleSheet(style);

            grid->addWidget(spin, col, row);
        }   

    }

    
}


void MainWindow::setupConnections() {
    connect (solve, &QPushButton::clicked, this , &MainWindow::solveSudoku);
}

