#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H
#include <QStyleOptionGraphicsItem>
#include <QGraphicsItem>
#include <QPainter>

struct GameObject{
    virtual ~GameObject() = default;
};

//dynamic object
struct Resource: public GameObject{
    ~Resource() override = default;
    //будут листья, грибы и т.д.
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
