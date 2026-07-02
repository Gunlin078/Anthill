#include "antsimulator.h"
#include "./ui_antsimulator.h"

AntSimulator::AntSimulator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AntSimulator)
{
    ui->setupUi(this);
}

AntSimulator::~AntSimulator()
{
    delete ui;
}
