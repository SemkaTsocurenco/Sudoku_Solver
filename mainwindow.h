#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include <QApplication>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include <QImage>
#include <QMessageBox>


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setupUi();
    void setupConnections();

private:
    QWidget *centralWidget;
    

};

#endif // MAINWINDOW_H
