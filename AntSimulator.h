#ifndef ANTSIMULATOR_H
#define ANTSIMULATOR_H
#include "Ant.h"
#include "GameObjects.h"
#include "Nest.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
//#include <atomic>

QT_BEGIN_NAMESPACE
namespace Ui {
class AntSimulator;
}
QT_END_NAMESPACE

class AntSimulator : public QMainWindow
{
    Q_OBJECT

public:
    AntSimulator(QWidget *parent = nullptr);
    void play();
    void stop();
    ~AntSimulator();
    void createMap();
    void toggleUpdates(bool);
private:
    Ui::AntSimulator *ui;
    QGraphicsScene* scene_;
    QGraphicsView* view_;
    QTimer *updateTimer_ = nullptr;

    QList<Wall*> walls_;
    QList<Resource*> resources_;
    QList<Ant*> ants_;
    Nest* nest_;
    bool isPlaying_;
};

struct Config{
    inline static int SCENE_HEIGHT = 500;
    inline static int SCENE_WIDTH = 500;
    inline static int WALL_THICKNESS = 10;
    inline static int RESOURCE_COUNT = 2;
    inline static int NEST_ENTRANCE_RADIUS = 40;
    inline static int FPS = 20;

    inline static int ANT_RADIUS = 5;
    inline static int ANT_COUNT = 40;

};
#endif // ANTSIMULATOR_H
