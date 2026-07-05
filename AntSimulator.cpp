#include "AntSimulator.h"
#include "./ui_AntSimulator.h"

AntSimulator::AntSimulator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AntSimulator)
    , scene_(new QGraphicsScene(this))
    , view_(new QGraphicsView(scene_, this))
    , nest_(nullptr)
{
    ui->setupUi(this);
    view_->setRenderHint(QPainter::Antialiasing);
    view_->setScene(scene_);
    setCentralWidget(view_);

    createMap();
    toggleUpdates();
}
AntSimulator::~AntSimulator()
{
    qDeleteAll(walls_);
    walls_.clear();

    qDeleteAll(resources_);
    resources_.clear();

    qDeleteAll(ants_);
    ants_.clear();

    delete nest_;
    nest_ = nullptr;

    delete ui;
}

void AntSimulator::createMap() {
    int w = Config::SCENE_WIDTH;
    int h = Config::SCENE_HEIGHT;
    int t = Config::WALL_THICKNESS;
    // 1: Фон (когда то потом)
    // 2: Стены по периметру
    Wall* topWall = new Wall(0, 0, w, t);
    scene_->addItem(topWall);
    walls_.append(topWall);

    Wall* bottomWall = new Wall(0, h - t, w, t);
    scene_->addItem(bottomWall);
    walls_.append(bottomWall);

    Wall* leftWall = new Wall(0, 0, t, h);
    scene_->addItem(leftWall);
    walls_.append(leftWall);

    Wall* rightWall = new Wall(w - t, 0, t, h);
    scene_->addItem(rightWall);
    walls_.append(rightWall);

    for (Wall* wall : walls_) {
        wall->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    }

    // 3: Муравейник в центре
    nest_ = new Nest(w/2, h/2, Config::NEST_ENTRANCE_RADIUS);
    scene_->addItem(nest_);

    // 4: Муравьи (стартуют у муравейника на случайном расстоянии от 0 до 2Rw)       Не работает
    for (int i = 0; i < Config::ANT_COUNT; ++i) {
        int r = QRandomGenerator::global()->bounded(static_cast<int>(Config::NEST_ENTRANCE_RADIUS*0.9),
                                                    static_cast<int>(Config::NEST_ENTRANCE_RADIUS*1.5));

        double angle = QRandomGenerator::global()->bounded(360) * M_PI / 180.0;
        int x = w/2 + static_cast<int>(r * qCos(angle));
        int y = h/2 + static_cast<int>(r * qSin(angle));

        Ant* ant = new AntWorker(x, y, Config::ANT_RADIUS);
        scene_->addItem(ant);
        ants_.append(ant);
    }
    // 5: Ресурсы (разбросать случайно)
    /*
    for (int i = 0; i < Config::RESOURCE_COUNT; ++i) {
        Resource* r = new Resource(randomX(), randomY(), Resource::Mined);
        scene_->addItem(r);
        resources_.append(r);
    }*/

}

void AntSimulator::toggleUpdates()
{
    if (!updateTimer_) {
        updateTimer_ = new QTimer(this);
        updateTimer_->setTimerType(Qt::PreciseTimer);
        updateTimer_->setInterval(50);
        connect(updateTimer_, &QTimer::timeout, this, [this]() {
            update();
            //qDebug() << "Updating"; ///отладочное
        });
    }

    if (updateTimer_->isActive()) {
        updateTimer_->stop();
    } else {
        updateTimer_->start();
    }///можно заменить тернарником
}

/* тут я химичил с мютексами, смотреть не надо, это на гиппотетическое будущее
while (true)
{
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this] { return GameRunning; });

    lock.unlock();
}
void AntSimulator::gameOnOff()
{
    std::lock_guard<std::mutex> lock(mtx);
    if (GameRunning == true) {GameRunning = false;}
    else {GameRunning = true;}
}

    std::mutex mtx;
    std::condition_variable cv;
    bool GameRunning{true};

*/
