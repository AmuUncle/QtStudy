#include "mainwnd.h"
#include <QApplication>
#include "appinit.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AppInit::Instance()->start();

    MainWnd w;
    w.show();

    return a.exec();
}
