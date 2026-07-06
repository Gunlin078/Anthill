#include "AntSimulator.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AntSimulator game;
    game.play();
    return a.exec();
}
