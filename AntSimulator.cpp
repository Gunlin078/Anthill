#include "AntSimulator.h"
#include "./ui_AntSimulator.h"

AntSimulator::AntSimulator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AntSimulator)
    , m_scene(new QGraphicsScene(this))
    , m_view(new QGraphicsView(m_scene, this))
    , m_nest(nullptr)
{
    ui->setupUi(this);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setScene(m_scene);
    setCentralWidget(m_view);

    createMap();
}

AntSimulator::~AntSimulator()
{
    delete ui;
}

void AntSimulator::createMap() {
    int w = Config::SCENE_WIDTH;
    int h = Config::SCENE_HEIGHT;
    int t = Config::WALL_THICKNESS;
    // 1: Фон (когда то потом)
    // 2: Стены по периметру
    Wall* topWall = new Wall(0, 0, w, t);
    m_scene->addItem(topWall);
    m_walls.append(topWall);

    Wall* bottomWall = new Wall(0, h - t, w, t);
    m_scene->addItem(bottomWall);
    m_walls.append(bottomWall);

    Wall* leftWall = new Wall(0, 0, t, h);
    m_scene->addItem(leftWall);
    m_walls.append(leftWall);

    Wall* rightWall = new Wall(w - t, 0, t, h);
    m_scene->addItem(rightWall);
    m_walls.append(rightWall);

    // 3: Муравейник в центре
    m_nest = new Nest(w/2, h/2, Config::NEST_RADIUS);
    m_scene->addItem(m_nest);

    // 4: Ресурсы (разбросать случайно)
    /*
    for (int i = 0; i < Config::RESOURCE_COUNT; ++i) {
        Resource* r = new Resource(randomX(), randomY(), Resource::Mined);
        m_scene->addItem(r);
        m_resources.append(r);
    }*/

    // 5. Муравьи (стартуют у муравейника)
    /*
    for (int i = 0; i < Config::ANT_COUNT; ++i) {
        Ant* ant = new Ant(w/2, h/2, this);
        m_scene->addItem(ant);
        m_ants.append(ant);
    }*/
}
