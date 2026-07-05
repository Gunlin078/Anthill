#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H
#include <QStyleOptionGraphicsItem>
#include <QGraphicsItem>
#include <QPainter>

// Если определен флаг релиза, то макрос станет заглушкой
#ifdef NDEBUG
#define LOG_MSG(x)
#else
#define LOG_MSG(x) qDebug() << x
#endif

struct GameObject{
    virtual ~GameObject() = default;
};

//dynamic object
struct Resource: public GameObject{
public:
    ~Resource() override = default;
    QString name_;
    QColor color_;
    short count_;
    short productionSpeed_;
protected:
    Resource(QString name, QColor color, short count, short V)
        : name_(name)
        , color_(color)
        , count_(count)
        , productionSpeed_(V){}
    friend class ResourceFactory;
};

class ResourceFactory{
public:
    static Resource* createByType(const QString& type) {
        if (type == "Branch") return createLeaf();
        if (type == "Acorn") return createAcorn();
        if (type == "Leaf") return createLeaf();
        LOG_MSG("The resource factory received an unknown type");
        return nullptr;
    }

private:
    static Resource* createBranch() {
        return new Resource({"Branch", QColor(133, 99, 48), 100, 20});
    }

    static Resource* createAcorn() {
        return new Resource({"Acorn", QColor(139, 66, 2), 15, 2});
    }

    static Resource* createLeaf() {
        return new Resource({"Leaf", QColor(66, 164, 61), 20, 5});
    }

};
// Использование:
//auto* acorn = ResourceFactory::createAcorn();
//auto* leaf = ResourceFactory::createLeaf();


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
