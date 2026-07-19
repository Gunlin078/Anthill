#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include "PauseOverlay.hpp"
#include <QGraphicsView>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QKeyEvent>

// Кроме тех, что есть здесь, в AntSimulator лежит обработка esc
class CustomGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CustomGraphicsView(QWidget *parent = nullptr) : QGraphicsView(parent){
        // Отключение стандартной прокрутки колёсиком
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        this->scale(0.5, 0.5);
    }
protected:
    // Зум колёсиком
    double scale_ = 1.0;

    void wheelEvent(QWheelEvent *event) override
    {
        const double scaleFactor = 1.3;
        const double minScale = 0.5;
        const double maxScale = 10.0;

        if (event->angleDelta().y() > 0)
            scale_ = qMin(scale_ * scaleFactor, maxScale);
        else
            scale_ = qMax(scale_ / scaleFactor, minScale);

        resetTransform();
        scale(scale_, scale_);

        event->accept();
    }

    // Перетаскивание сцены мышкой(начало)
    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton) {
            setDragMode(QGraphicsView::ScrollHandDrag);
            QGraphicsView::mousePressEvent(event);
        }
    }
    // Перетаскивание сцены мышкой(конец)
    void mouseReleaseEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton) {
            setDragMode(QGraphicsView::NoDrag);
            QGraphicsView::mouseReleaseEvent(event);
        }
    }

    // Сброс зума по клавише R
    void keyPressEvent(QKeyEvent *event) override
    {
        if (event->key() == Qt::Key_R) {
            resetTransform();
            event->accept();
        } else {
            QGraphicsView::keyPressEvent(event);
        }
    }
};

#endif // CUSTOMGRAPHICSVIEW_H
