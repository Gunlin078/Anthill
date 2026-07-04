#ifndef ANT_H
#define ANT_H

#include <QGraphicsEllipseItem>
#include <QPainter>

class Ant : public QGraphicsEllipseItem
{
private:
    int radius_ = 1;
public:
    Ant(int x,int y, int R, QString caste)
        : coordinates_(x,y)
        , radius_(R)
        , caste_(caste){}

    QString caste_;
    QString portableResource_;
    QPoint coordinates_;
    short HP_;
    short angle_;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override {
        // Тело — красный круг
        painter->setBrush(Qt::red);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(-radius_, -radius_, radius_*2, radius_*2);

        // Голова — чёрная точка в направлении угла
        painter->setBrush(Qt::black);
        qreal headX = cos(angle_) * radius_ * 0.8;
        qreal headY = sin(angle_) * radius_ * 0.8;
        painter->drawEllipse(QPointF(headX, headY), 2, 2);
    }
};

#endif // ANT_H
