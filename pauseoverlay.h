#ifndef PAUSEOVERLAY_H
#define PAUSEOVERLAY_H

#include <QWidget>

class QPushButton;
class QLabel;

class PauseOverlay : public QWidget
{
    Q_OBJECT

public:
    explicit PauseOverlay(QWidget *parent = nullptr);

signals:
    void continueGame();
    void exitToMenu();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel* title_;
    QPushButton* continueButton_;
    QPushButton* menuButton_;
};

#endif
