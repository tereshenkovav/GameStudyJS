#include "mainwindow.h"
#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    if (argc>1) QDir::setCurrent(argv[1]) ;

    return a.exec();
}
