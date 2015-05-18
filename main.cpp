#include <QDebug>
#include "window.h"
#include "project.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Window wnd;
    wnd.show();
    return a.exec();
}
