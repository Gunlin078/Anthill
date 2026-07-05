#ifndef NEST_H
#define NEST_H
#include <QGraphicsEllipseItem>
// Отдельно от остальных, т.к. в будущем появится вид сбоку
class Nest : public QGraphicsEllipseItem
{
public:
    Nest(int x,int y, const int radius)
        : QGraphicsEllipseItem(-radius, -radius, radius*2, radius*2)
        , coordinates_(x,y)
        , radius_(radius)
    {
        setPos(x, y);
        setBrush(QColor(54, 38, 27));
        setPen(QPen(QColor(115, 79, 54), radius*0.6));

        // Ещё одно кольцо вокруг
        int outerRadius = radius + (radius * 0.3);
        QGraphicsEllipseItem* outerRing = new QGraphicsEllipseItem(
            -outerRadius, -outerRadius, outerRadius * 2, outerRadius * 2, this);
        outerRing->setBrush(QColor(0,0,0,0));
        outerRing->setPen(QPen(QColor(149, 102, 70), radius*0.5));
        outerRing->setZValue(-1);
    }
private:
    QPoint coordinates_;
    int radius_;
};
#endif // NEST_H
