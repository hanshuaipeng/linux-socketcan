#include "mainwindow.h"
#include <QApplication>
#include "thredcanread.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    thredCanread canRead;
    canRead.start();
    w.show();

    return a.exec();
}
