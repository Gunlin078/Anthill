#include "Ant.h"
#include "Configs.h"

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


void Ant::moveTo(const QPoint &p)                                              // Пример: currentPos_ = (0;0), а p = (3;4)
{                                                                               // Начало в (0;0), а цель в (3;4)
    if (p == currentPos_.toPoint()) return;
    isMoving_ = true;
    targetPos_ = p;                                                             // targetPos_ = (3;4)
    direction_ = targetPos_ - currentPos_;                                      // direction_ = (3;4) - (0;0) = (3;4)
    distanceToTarget_ = std::hypot(direction_.x(), direction_.y());             // distanceToTarget_ = sqrt( 3*3+4*4 ) = 5
    if (distanceToTarget_ > 0.0001f)
        direction_ /= distanceToTarget_;                                        // direction_ = (3;4) / 5 =  (0.6;0.8) - единичный вектор
}                                                                               // То есть пройдя 1 у.е., к текущим координатам прибавится (0.6;0.8)

void Ant::updatePosition()
{
    if (!isMoving_) return;

    distanceToTarget_ -= step_;                                                 // Осталось пройти: 5 - шаг
    if (distanceToTarget_ <= 0) {
        currentPos_ = targetPos_;
        isMoving_ = false;
        distanceToTarget_ = 0;
    } else {
        currentPos_ += direction_ * step_;                                       // Текущее положение: (0;0) + (0.6;0.8)*шаг
    }
    setPos(currentPos_);
}
