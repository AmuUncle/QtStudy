#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyle(QStyleFactory::create("fusion"));
    a.setFont(QFont("Microsoft Yahei", 9));

    MainWindow w;
    w.show();

    return a.exec();
}
