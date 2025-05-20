#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include <QApplication>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <QSize>
#include <QGridLayout>
#include <QImage>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <qpushbutton.h>
#include <omp.h>       



class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

public slots:
    void buildGrid();          // (re)создание поля
    void refreshSudoku();      // очистка
    void solveSudoku();        // вызов решателя (пустая заглушка)


private:
    bool ok = false;
    QString style;

    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QSpinBox *baseSelector  = nullptr;

    QPushButton *solveBtn;
    QPushButton *refreshBtn;
    QLineEdit* gridSpins[9][9];

    QString getStyle(int initVal,  // было ли число дано изначально
                             int row, int col,
                             int base);

    QVector<QVector<QLineEdit*>> cells;   // [row][col]

};



#endif // MAINWINDOW_H




