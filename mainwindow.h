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
    void buildGrid();         
    void refreshSudoku();      
    void solveSudoku();         


private:
    bool ok = false;
    QString style;

    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QSpinBox *baseSelector  = nullptr;

    QPushButton *solveBtn;
    QPushButton *refreshBtn;
    QLineEdit* gridSpins;

    QString getStyle(int initVal,  
                             int row, int col,
                             int base);

    QVector<QVector<QLineEdit*>> cells;  

};



#endif // MAINWINDOW_H




