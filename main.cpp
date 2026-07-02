#include "antsimulator.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AntSimulator w;
    w.show();
    return a.exec();
}
