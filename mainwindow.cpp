#include "mainwindow.h"
#include <iostream>
#include <qpushbutton.h>





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize({450, 550});
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto *vLay = new QVBoxLayout(centralWidget);

    baseSelector = new QSpinBox(this);
    baseSelector->setRange(2, 10);  
    baseSelector->setValue(3);      
    baseSelector->setPrefix("base = ");
    baseSelector->setToolTip("base² = размер стороны поля");

    gridLayout = new QGridLayout();
    vLay->addWidget(baseSelector);
    vLay->addSpacing(10);
    vLay->addLayout(gridLayout);

    solveBtn   = new QPushButton("SOLVE",   this);
    refreshBtn = new QPushButton("REFRESH", this);

    solveBtn  ->setStyleSheet("QPushButton{background:#2e86de;color:#fff;font:16px bold;border-radius:8px;padding:8px;} "
                              "QPushButton:hover{background:#256e9e;} "
                              "QPushButton:pressed{background:#154360;}");
    refreshBtn->setStyleSheet("QPushButton{background:#db2020;color:#fff;font:16px bold;border-radius:8px;padding:8px;} "
                              "QPushButton:hover{background:#a12424;} "
                              "QPushButton:pressed{background:#471f1f;}");

    int sizespace = 20 ;

    vLay->addSpacing(sizespace);
    vLay->addWidget(solveBtn);
    vLay->addWidget(refreshBtn);

    // --- сигналы ---
    connect(baseSelector, qOverload<int>(&QSpinBox::valueChanged),
            this, &MainWindow::buildGrid);
    connect(refreshBtn, &QPushButton::clicked,
            this, &MainWindow::refreshSudoku);
    connect(solveBtn,   &QPushButton::clicked,
            this, &MainWindow::solveSudoku);
    // // resize mainwindow if baseSelector is changed 
    // connect(baseSelector, qOverload<int>(&QSpinBox::valueChanged),
    //         this, &MainWindow::buildGrid);

    buildGrid();   // первичное построение
}

//---------------------------------------------------------
//  (re)создаём поле соответствующего размера
//---------------------------------------------------------
void MainWindow::buildGrid()
{
    QLayoutItem *item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    cells.clear();
    const int base = baseSelector->value();
    const int N    = base * base;

    setFixedSize({50 * N,  (N * 50) + (33 * base)});


    cells = QVector<QVector<QLineEdit*>>(N, QVector<QLineEdit*>(N, nullptr));

    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c) {
            auto *edit = new QLineEdit(this);
            edit->setFixedSize(50, 50);
            edit->setAlignment(Qt::AlignCenter);
            edit->setValidator(new QIntValidator(1, N, edit));

            // рамки блоков
            int top    = (r % base == 0)          ? 3 : 1;
            int left   = (c % base == 0)          ? 3 : 1;
            int right  = ((c + 1) % base == 0)    ? 3 : 1;
            int bottom = ((r + 1) % base == 0)    ? 3 : 1;

            edit->setStyleSheet(QString(
                "QLineEdit{background:#dafbff;color:#2e86de;"
                "font-size:20px;font-weight:bold;"
                "border-top:%1px solid black;border-left:%2px solid black;"
                "border-right:%3px solid black;border-bottom:%4px solid black;}")
                .arg(top).arg(left).arg(right).arg(bottom));

            gridLayout->addWidget(edit, r, c);
            cells[r][c] = edit;
        }
}



void MainWindow::refreshSudoku()
{
    for (auto &row : cells)
        for (auto *e : row)
            e->clear();
}