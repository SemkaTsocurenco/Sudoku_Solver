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
    QWidget *centralWidget;
    QGridLayout *grid;
    QPushButton *solve;

};

#endif // MAINWINDOW_H
