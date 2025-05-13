#include <iostream>
#include "mainwindow.h"
#include <qchar.h>
#include <qwidget.h>
#include <vector>


#include <QApplication>
#include <QMainWindow>
#include <QGridLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QWidget>
#include <vector>
#include <map>
#include <random>
#include <climits>

using namespace std;



QString MainWindow::getStyle(int init, int sol, int i, int j){
    int top = (i % 3 == 0) ? 5 : 1;
    int left = (j % 3 == 0) ? 5 : 1;
    int right = ((j + 1) % 3 == 0) ? 5 : 1;
    int bottom = ((i + 1) % 3 == 0) ? 5 : 1;
    QString st;
    if (init == sol){
        st = QString(R"(
        QLineEdit {
            background-color:rgb(230, 255, 255);
            color:rgb(0, 0, 0);
            font-weight: bold;
            font-size: 20px;
            border-top: %1px solid black;
            border-left: %2px solid black;
            border-right: %3px solid black;
            border-bottom: %4px solid black;
            qproperty-alignment: AlignCenter;
        }
        )").arg(top).arg(left).arg(right).arg(bottom);

    } else {
        st = QString(R"(
        QLineEdit {
            background-color:rgb(181, 214, 214);
            color:#2e86de;
            font-weight: bold;
            font-size: 20px;
            border-top: %1px solid black;
            border-left: %2px solid black;
            border-right: %3px solid black;
            border-bottom: %4px solid black;
            qproperty-alignment: AlignCenter;
        }
        )").arg(top).arg(left).arg(right).arg(bottom);
    }
    return st;

}


void MainWindow::solveSudoku(){
    std::vector<std::vector<int>> initial (9, std::vector<int>(9));
    std::vector<std::vector<int>> zeros (9, std::vector<int>(9));
        for (int i = 0; i < 9; ++i){
            for (int j = 0; j < 9; ++j){
                initial[i][j] = gridSpins[i][j]->text().toInt(&ok);
                if (!ok) {
                    initial[i][j] = 0;
                }
            }
        }
        // TabuSolver solver(initial);
        // auto solution = solver.solve();

        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j){
                // gridSpins[i][j]->setText(QString::number(solution[i][j]));
                auto style = getStyle(initial[i][j], zeros[i][j], i, j);
                gridSpins[i][j]->setStyleSheet(style);
            }

};

