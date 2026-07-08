#ifndef ANT_H
#define ANT_H

#include "Configs.h"

#include <QGraphicsEllipseItem>
#include <QRandomGenerator>
#include <QPainter>

class Ant : public QGraphicsEllipseItem
{
public:
    virtual ~Ant() = default;
    QPoint getSpawnPoint();
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;///нужен фронтендер

    void moveTo(const QPoint&);
    void updatePosition();
protected:
    Ant(int R): radius_(R)
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
    short radius_;
    short angle_{0};
    bool isMoving_{0};
};

class AntWorker: public Ant{
public:
    ~AntWorker() = default;
    AntWorker(int R)
        : Ant(R){}
private:
    QString portableResource_;
    short HP_ = 10;
    short speed_ = 3;
};

class AntWarrior: public Ant{
public:
    ~AntWarrior() = default;
    AntWarrior(int R)
        : Ant(R){}
private:
    short attack_ = 4;
    short HP_ = 20;
    short speed_ = 2;
};

class AntScout: public Ant{
public:
    ~AntScout() = default;
    AntScout(int R)
        : Ant(R){}
private:
    short attack_ = 1;
    short HP_ = 15;
    short speed_ = 4;
};
#endif // ANT_H
