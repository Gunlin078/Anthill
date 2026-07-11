#ifndef ANTSIMULATOR_H
#define ANTSIMULATOR_H

#include "Ant.h"
#include "Configs.hpp"
#include "CustomGraphicsView.hpp"
#include "GameObjects.h"
#include "Nest.hpp"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class AntSimulator;
}
QT_END_NAMESPACE

class AntSimulator : public QMainWindow
{
    Q_OBJECT

public:
    AntSimulator(QWidget *parent = nullptr);
    void play();
    void stop();
    ~AntSimulator();
    void createMap();
    void toggleUpdates(bool);
private:
    Ui::AntSimulator *ui;
    QGraphicsScene* scene_;
    CustomGraphicsView* view_;
    QTimer *updateTimer_ = nullptr;

    QList<Wall*> walls_;
    QList<Resource*> resources_;
    QList<Ant*> ants_;
    Nest* nest_;

    PauseOverlay* pauseOverlay_;

    long long lastRestorationCheckTime_ = 0;
    static constexpr long long CHECK_INTERVAL_SEC = 1;

    bool isPlaying_;
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};
#endif // ANTSIMULATOR_H
