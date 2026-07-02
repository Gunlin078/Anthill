#ifndef ANTSIMULATOR_H
#define ANTSIMULATOR_H

#include <QMainWindow>

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
    ~AntSimulator();

private:
    Ui::AntSimulator *ui;
};
#endif // ANTSIMULATOR_H
