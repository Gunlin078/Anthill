#include "Ant.h"
#include "Configs.hpp"

#define W Config::SCENE_WIDTH
#define H Config::SCENE_HEIGHT

QPoint Ant::getSpawnPoint()
{
    int r = QRandomGenerator::global()->bounded(static_cast<int>(Config::NEST_ENTRANCE_RADIUS*0.9),
                                                static_cast<int>(Config::NEST_ENTRANCE_RADIUS*1.5));

    double angle = QRandomGenerator::global()->bounded(360) * M_PI / 180.0;
    int x = static_cast<int>(r * qCos(angle));
    int y = static_cast<int>(r * qSin(angle));
    return {x, y};
}

QPointF Ant::getCurrentPos(){return currentPos_;}

void Ant::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*){
    painter->save();

    // ================= ЛАПКИ =================
    QPen legPen(QColor(45, 25, 15));
    legPen.setWidth(2);
    legPen.setCapStyle(Qt::RoundCap);
    painter->setPen(legPen);

    // Левые
    painter->drawLine(-2, -3, -8, -8);
    painter->drawLine(3, 0, -8, 0);
    painter->drawLine(-2, 3, -8, 8);

    // Правые
    painter->drawLine(-2, -3, 8, -8);
    painter->drawLine(3, 0, 8, 0);
    painter->drawLine(-2, 3, 8, 8);

    painter->setPen(Qt::NoPen);

    // ================= БРЮШКО =================
    painter->setBrush(QColor(220, 30, 30));
    painter->drawEllipse(QPointF(-8, 0), 6.5, 5.5);

    // ================= ТАЛИЯ =================
    painter->setBrush(QColor(220, 30, 30));
    painter->drawEllipse(QPointF(0, 0), 2.2, 2.2);

    // ================= ГРУДЬ =================
    painter->setBrush(QColor(220, 30, 30));
    painter->drawEllipse(QPointF(5, 0), 5, 4.5);

    // ================= ГОЛОВА =================
    painter->setBrush(QColor(120, 15, 15));
    painter->drawEllipse(QPointF(12, 0), 4, 4);

    // ================= УСИКИ =================
    QPen antenna(QColor(35, 20, 20));
    antenna.setWidth(2);
    antenna.setCapStyle(Qt::RoundCap);
    painter->setPen(antenna);

    painter->drawLine(14, -2, 18, -6);
    painter->drawLine(14, 2, 18, 6);

    // ================= ГЛАЗА =================
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::white);

    painter->drawEllipse(QPointF(13, -1.5), 1.0, 1.0);
    painter->drawEllipse(QPointF(13, 1.5), 1.0, 1.0);

    painter->setBrush(Qt::black);

    painter->drawEllipse(QPointF(13, -1.5), 0.4, 0.4);
    painter->drawEllipse(QPointF(13, 1.5), 0.4, 0.4);

    painter->restore();
}


void Ant::moveTo(const QPoint &p)                                              // Пример: currentPos_ = (0;0), а p = (3;4)
{                                                                               // Начало в (0;0), а цель в (3;4)
    if (p == currentPos_.toPoint()) return;
    isMoving_ = true;
    targetPos_ = p;                                                             // targetPos_ = (3;4)
    direction_ = targetPos_ - currentPos_;                                      // direction_ = (3;4) - (0;0) = (3;4)
    distanceToTarget_ = std::hypot(direction_.x(), direction_.y());             // distanceToTarget_ = sqrt( 3*3+4*4 ) = 5

    // Поворот в сторону цели
    angle_ = qRadiansToDegrees(qAtan2(direction_.y(), direction_.x()));
    setRotation(angle_);

    if (distanceToTarget_ > 0.0001f)
        direction_ /= distanceToTarget_;                                        // direction_ = (3;4) / 5 =  (0.6;0.8) - единичный вектор
}                                                                               // То есть пройдя 1 у.е., к текущим координатам прибавится (0.6;0.8)

void Ant::updatePosition()
{
    if (!isMoving_) return;

    distanceToTarget_ -= step_;                                                 // Осталось пройти: 5 - шаг
    if (distanceToTarget_ <= step_) {
        currentPos_ = targetPos_;
        isMoving_ = false;
        distanceToTarget_ = 0;
    } else {
        currentPos_ += direction_ * step_;                                       // Текущее положение: (0;0) + (0.6;0.8)*шаг
    }
    setPos(currentPos_);
}

Resource* Ant::resourceNearby()
{
    if (!scene()) return nullptr;

    QPainterPath radiusPath;
    radiusPath.addEllipse(this->pos(), 0.1, 0.1);

    QList<QGraphicsItem*> itemsInRadius = scene()->items(radiusPath);

    for (QGraphicsItem *item : itemsInRadius) {
        if (Resource *res = dynamic_cast<Resource*>(item)){
            return res;
        }}
    return nullptr;
}
