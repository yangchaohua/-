#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QTimer>
#include <QTime>
#include <QString>
#include <time.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private slots:
    void showImage();
    void on_pushButton_clicked();
    void send_d();
private:
    Ui::MainWindow *ui;
    QTimer    *timer;
    QTimer    *timer1;
};

#endif // MAINWINDOW_H
