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




class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void setupUi();
    void setupConnections();

public slots:
    void solveSudoku();


private:
    bool ok = false;

    QWidget *centralWidget;
    QGridLayout *grid;
    QPushButton *solve;
    QLineEdit* gridSpins[9][9];

    QString getStyle(int init, int sol, int i, int j);

};

#endif // MAINWINDOW_H
