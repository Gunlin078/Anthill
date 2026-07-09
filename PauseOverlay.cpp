#include "PauseOverlay.h"

#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>

PauseOverlay::PauseOverlay(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground);

    title_ = new QLabel("ПАУЗА");

    continueButton_ = new QPushButton("Продолжить");

    menuButton_ = new QPushButton("Выход");

    title_->setAlignment(Qt::AlignCenter);

    title_->setStyleSheet(
        "font-size:32px;"
        "font-weight:bold;"
        "color:white;"
        );

    continueButton_->setFixedSize(220,55);
    menuButton_->setFixedSize(220,55);

    continueButton_->setStyleSheet(
        "QPushButton{"
        "background:#4d4d4d;"
        "color:white;"
        "border:2px solid white;"
        "border-radius:10px;"
        "font-size:18px;"
        "}"
        "QPushButton:hover{"
        "background:#666666;"
        "}"
        );

    menuButton_->setStyleSheet(
        continueButton_->styleSheet()
        );

    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addStretch();

    layout->addWidget(title_);

    layout->addSpacing(30);

    layout->addWidget(continueButton_,0,Qt::AlignCenter);

    layout->addSpacing(15);

    layout->addWidget(menuButton_,0,Qt::AlignCenter);

    layout->addStretch();

    connect(continueButton_,
            &QPushButton::clicked,
            this,
            &PauseOverlay::continueGame);

    connect(menuButton_,
            &QPushButton::clicked,
            this,
            &PauseOverlay::exitToMenu);

    hide();
}

void PauseOverlay::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.fillRect(rect(),
                     QColor(0,0,0,150));
}
