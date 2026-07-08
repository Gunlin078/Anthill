#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H
#include <QStyleOptionGraphicsItem>
#include <QRandomGenerator>
#include <QGraphicsItem>
#include <QDateTime>
#include <QPainter>
#include "Configs.h"
class ResourceManager;
class Resource;

struct GameObject{
    virtual ~GameObject() = default;
};

//dynamic object
struct Resource : public GameObject, public QGraphicsEllipseItem{
public:
    ~Resource() override = default;
    void remove();
    void restore();

protected:
    Resource(QString name, QColor color, short count, short V)
        : name_(name)
        , color_(color)
        , count_(count)
        , productionSpeed_(V)
        , initialCount_(count)
        , QGraphicsEllipseItem(0, 0, 8, 16)//Случайное, нужен фронтендер
    {
        int x = QRandomGenerator::global()->bounded(Config::NEST_ENTRANCE_RADIUS*3, std::min(Config::SCENE_HEIGHT, Config::SCENE_WIDTH));
        int y = QRandomGenerator::global()->bounded(Config::NEST_ENTRANCE_RADIUS*3, std::min(Config::SCENE_HEIGHT, Config::SCENE_WIDTH));
        setPos(x,y);

        setBrush(QColor(color_));
        setPen(QPen(QColor(color_), 8));
    }

    QString name_;
    QColor color_;
    long long restorationDeadline_;
    short count_;
    short productionSpeed_;
    short initialCount_;
    bool isActive_ = true;

    friend class ResourceFactory;
    friend class ResourceManager;
};

class ResourceFactory{
public:
    static Resource* createByType(const QString& type) {
        if (type == "Branch") return createBranch();
        if (type == "Acorn") return createAcorn();
        if (type == "Leaf") return createLeaf();
        qWarning() << "The resource factory received an unknown type";
        return nullptr;
    }

private:
    static Resource* createBranch() {
        return new Resource({"Branch", QColor(100, 80, 48), 100, 20});
    }

    static Resource* createAcorn() {
        return new Resource({"Acorn", QColor(139, 66, 2), 15, 2});
    }

    static Resource* createLeaf() {
        return new Resource({"Leaf", QColor(66, 164, 61), 20, 5});
    }

};

class ResourceManager {
public:
    static void scheduleRestoration(Resource* res);
    static void checkRestorations(long long currentTimeSec);
    static void clearAll();

private:
    static QList<Resource*> pendingResources_;
};

//static object
struct Wall: public GameObject, public QGraphicsItem{
    ~Wall() override = default;
    Wall(int x,int y,int width,int length)
        : coordinates_(x,y)
        , width_(width)
        , length_(length)
    {
        setPos(x,y);
    }

    QPoint coordinates_;
    int width_;
    int length_;

    QRectF boundingRect() const override{
        return QRectF(0, 0, width_, length_);
    }
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override{
        painter->fillRect(boundingRect(), QColor(80, 70, 60));
        painter->setPen(QPen(Qt::black, 1));
        painter->drawRect(boundingRect());
    }
};

#endif // GAMEOBJECTS_H
