#include "hammingnetwork.h"
#include <QDebug>
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HammingNetwork net;

    qDebug() << net.GetFirstLayerSize();
    qDebug() << net.GetZeroLayerSize();
    qDebug() << net.GetCountOfPatterns();

    for(int i = 0; i < 10; i++)
        net.AddPattern("/home/acarus/data/ideal/" + QString::number(i) + ".bmp");

    qDebug() << net.GetCountOfPatterns();

    qDebug() << net.Train();

    qDebug() << "result = " << net.TestPattern(QImage("/home/acarus/data/test/5.bmp"));

    qDebug() << "finished";


    return a.exec();
}
