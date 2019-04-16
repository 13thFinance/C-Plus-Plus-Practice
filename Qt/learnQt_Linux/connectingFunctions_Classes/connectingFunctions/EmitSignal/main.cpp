#include "mainwindow.h"
#include <QApplication>

#include "window.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    Window innerWindow(&w);//inner window is child of w (the main window)


    w.show();

    return a.exec();
}
