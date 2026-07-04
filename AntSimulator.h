#ifndef ANTSIMULATOR_H
#define ANTSIMULATOR_H
#include "Ant.h"
#include "GameObjects.h"
#include "Nest.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>

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
    void createMap();
    ~AntSimulator();

private:
    Ui::AntSimulator *ui;
    QGraphicsScene* m_scene;
    QGraphicsView* m_view;

    QList<Wall*> m_walls;
    QList<Resource*> m_resources;
    QList<Ant*> m_ants;
    Nest* m_nest;
};

struct Config{
    inline static int SCENE_WIDTH = 500;
    inline static int SCENE_HEIGHT = 500;
    inline static int WALL_THICKNESS = 5;
    inline static int NEST_RADIUS = 20;
    inline static int RESOURCE_COUNT = 2;

    inline static int ANT_COUNT = 10;
};
#endif // ANTSIMULATOR_H
