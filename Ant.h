#ifndef ANT_H
#define ANT_H

#include "Configs.hpp"

#include <QGraphicsEllipseItem>
#include <QRandomGenerator>
#include <QPainter>

class Ant : public QGraphicsEllipseItem
{
public:
    virtual ~Ant() = default;
    QPointF getCurrentPos();
    QPoint getSpawnPoint();
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;

    void moveTo(const QPoint&);
    void updatePosition();
protected:
    Ant(short speed):speed_(speed)
    {
        currentPos_ = getSpawnPoint();
        setPos(currentPos_);
        step_ = speed_ * Config::DELTA_TIME;
    }
    QPointF currentPos_;
    QPointF direction_{0,0};    // нормализованное направление
    QPoint targetPos_{0,0};
    float distanceToTarget_{0};
    float step_{0};
    short speed_;
    short angle_{0};
    bool isMoving_{0};
};

class AntWorker: public Ant{
public:
    ~AntWorker() = default;
    AntWorker()
        : Ant(20){}
private:
    QString portableResource_;
    short HP_ = 10;
};

class AntWarrior: public Ant{
public:
    ~AntWarrior() = default;
    AntWarrior()
        : Ant(30){}
private:
    short attack_ = 4;
    short HP_ = 20;
};

class AntScout: public Ant{
public:
    ~AntScout() = default;
    AntScout()
        : Ant(50){}
private:
    short attack_ = 1;
    short HP_ = 15;
};
#endif // ANT_H
