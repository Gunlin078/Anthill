#include "Ant.h"
#include "AntSimulator.h"

#define W Config::SCENE_WIDTH
#define H Config::SCENE_HEIGHT

QPoint Ant::getSpawnPoint()
{
    int r = QRandomGenerator::global()->bounded(static_cast<int>(Config::NEST_ENTRANCE_RADIUS*0.9),
                                                static_cast<int>(Config::NEST_ENTRANCE_RADIUS*1.5));

    double angle = QRandomGenerator::global()->bounded(360) * M_PI / 180.0;
    int x = W/2 + static_cast<int>(r * qCos(angle));
    int y = H/2 + static_cast<int>(r * qSin(angle));
    return {x, y};
}

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
}// Пример!!! Из готовых вариантов: ещё можно делать как гнездо
