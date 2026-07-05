#ifndef ANT_H
#define ANT_H

#include <QGraphicsEllipseItem>
#include <QRandomGenerator>
#include <QPainter>

class Ant : public QGraphicsEllipseItem
{
public:
    virtual ~Ant() = default;
    QPoint getSpawnPoint();
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;///нужен фронтендер
protected:
    Ant(int R)
        : radius_(R)
    {
        setPos(getSpawnPoint());
    }
    QPoint coordinates_;
    int angle_ = 0;
    short radius_;
};

class AntWorker: public Ant{
public:
    ~AntWorker() = default;
    AntWorker(int R)
        : Ant(R){}
private:
    QString portableResource_;
    short HP_ = 10;
    short speed = 3;
};

class AntWarrior: public Ant{
public:
    ~AntWarrior() = default;
    AntWarrior(int R)
        : Ant(R){}
private:
    short attack = 4;
    short HP_ = 20;
    short speed = 2;
};

class AntScout: public Ant{
public:
    ~AntScout() = default;
    AntScout(int R)
        : Ant(R){}
private:
    short attack = 1;
    short HP_ = 15;
    short speed = 4;
};
#endif // ANT_H
