#include "Ant.h"

void Ant::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*){
    // Тело — красный овал
    painter->setBrush(Qt::red);
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(-radius_, -radius_, radius_*2, radius_);

    // Голова — чёрная точка в направлении угла
    painter->setBrush(Qt::black);
    int  headX = cos(angle_) * radius_ * 0.8;
    int  headY = sin(angle_) * radius_ * 0.8;
    painter->drawEllipse(QPoint(headX, headY), 2, 2);
}// Пример!!! Из готовых вариантов ещё можно делать как гнездо
