#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "can_control.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    Can_Control can1 ;
    struct can_frame frame[2];
    frame[0].can_id = 0x11;
    frame[0].can_dlc = 1;
    frame[0].data[0] = '1';
    frame[1].can_id = 0x22;
    frame[1].can_dlc = 1;
    frame[1].data[0] = '2';
    int test;
    test = can1.socket_connect("can1");
    can1.set_can_filter(test);
    can1.send_frame(test, &frame[0], sizeof(frame[0]));
    can1.socket_close(test);
}

void MainWindow::on_pushButton_2_clicked()
{

}
