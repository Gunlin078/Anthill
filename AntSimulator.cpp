#include "AntSimulator.h"
#include "./ui_AntSimulator.h"

#define W Config::SCENE_WIDTH
#define H Config::SCENE_HEIGHT
#define T Config::WALL_THICKNESS

AntSimulator::AntSimulator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AntSimulator)
    , scene_(new QGraphicsScene(this))
    , view_(new QGraphicsView(scene_, this))
    , nest_(nullptr)
    , isPlaying_(false)
{
    ui->setupUi(this);
    view_->setRenderHint(QPainter::Antialiasing);
    view_->setScene(scene_);
    setCentralWidget(view_);

    createMap();
    pauseOverlay_ = new PauseOverlay(this);
    pauseOverlay_->setGeometry(rect());
    connect(
        pauseOverlay_,
        &PauseOverlay::continueGame,
        this,
        [this]()
        {
            pauseOverlay_->hide();
            play();
        }
        );
    connect(
        pauseOverlay_,
        &PauseOverlay::exitToMenu,
        this,
        [this]()
        {
            close();
        }
        );
}

void AntSimulator::play()
{
    if (isPlaying_ == false){
        this->show();
        isPlaying_ = true;
        toggleUpdates(true);
    }
}

void AntSimulator::stop()
{
    if (isPlaying_ == true){
        isPlaying_ = false;
        toggleUpdates(false);
    }
}

AntSimulator::~AntSimulator()
{
    qDeleteAll(walls_);
    walls_.clear();

    ResourceManager::clearAll();
    qDeleteAll(resources_);
    resources_.clear();

    qDeleteAll(ants_);
    ants_.clear();

    delete nest_;
    nest_ = nullptr;

    delete ui;
}

void AntSimulator::createMap() {
    // 1: Фон (когда то сильно потом)
    // 2: Стены по периметру
    Wall* topWall = new Wall(0, 0, W, T);
    scene_->addItem(topWall);
    walls_.append(topWall);

    Wall* bottomWall = new Wall(0, H - T, W, T);
    scene_->addItem(bottomWall);
    walls_.append(bottomWall);

    Wall* leftWall = new Wall(0, 0, T, H);
    scene_->addItem(leftWall);
    walls_.append(leftWall);

    Wall* rightWall = new Wall(W - T, 0, T, H);
    scene_->addItem(rightWall);
    walls_.append(rightWall);

    for (Wall* wall : walls_) {
        wall->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    }

    // 3: Муравейник в центре
    nest_ = new Nest(W/2, H/2, Config::NEST_ENTRANCE_RADIUS);
    scene_->addItem(nest_);

    // 4: Муравьи (стартуют у муравейника рядом со входом)
    for (int i = 0; i < Config::ANT_COUNT; ++i) {
        Ant* ant = new AntWorker(Config::ANT_RADIUS);
        scene_->addItem(ant);
        ants_.append(ant);
    }
    // 5: Ресурсы (разбросать случайно)
    for (int i = 0; i < Config::RESOURCE_COUNT; ++i) {
        Resource* branch = ResourceFactory::createByType("Branch");
        Resource* acorn = ResourceFactory::createByType("Acorn");
        Resource* leaf = ResourceFactory::createByType("Leaf");
        scene_->addItem(branch);    resources_.append(branch);
        scene_->addItem(acorn);     resources_.append(acorn);
        scene_->addItem(leaf);      resources_.append(leaf);
    }
}

void AntSimulator::toggleUpdates(bool starting)
{
    if (starting) {
        if (!updateTimer_) {
            updateTimer_ = new QTimer(this);
            updateTimer_->setTimerType(Qt::PreciseTimer);
            updateTimer_->setInterval(1000 / Config::FPS);
            connect(updateTimer_, &QTimer::timeout, this, [this]() {
                update();
                for (Ant* ant: ants_) {ant->updatePosition();}
                //LOG_MSG("Updating");///отладочное
                // Проверка восстановления ресурсов (не чаще чем раз в секунду)
                long long now = QDateTime::currentSecsSinceEpoch();
                if (now - lastRestorationCheckTime_ >= CHECK_INTERVAL_SEC) {
                    ResourceManager::checkRestorations(now);
                    lastRestorationCheckTime_ = now;
                }
            });
        }

        if (!updateTimer_->isActive()) {
            updateTimer_->start();
        }
        return;
    }

    if (!updateTimer_) {
        qDebug() << "The game loop hasn't yet been launched";
        return;
    }

    if (updateTimer_->isActive()) {
        updateTimer_->stop();
    }
}

void AntSimulator::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Escape)
    {
        if(isPlaying_)
        {
            stop();

            pauseOverlay_->show();

            pauseOverlay_->raise();

            pauseOverlay_->setFocus();
        }
    }
}

void AntSimulator::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    pauseOverlay_->setGeometry(rect());
}

