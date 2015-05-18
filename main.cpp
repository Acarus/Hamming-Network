#include <QDebug>
#include "mainwindow.h"
#include "project.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow wnd;
    wnd.show();
    return a.exec();
}
