#ifndef TOURDRAW_H
#define TOURDRAW_H

#include <QDialog>

namespace Ui {
class TourDraw;
}

enum class Net{ Sixteen = 16, ThirtyTwo = 32, SixtyFour = 64 };

class TourDraw : public QDialog
{
    Q_OBJECT

public:
    TourDraw(Net net_type, QWidget *parent = nullptr);
    ~TourDraw();

    virtual void paintEvent(QPaintEvent *event);

private slots:

    void showTeamInfo();

private:
    Ui::TourDraw *ui;

    Net draw_type = Net::Sixteen;

    QVector<QPushButton *> first_round_team_btns;
};

#endif // TOURDRAW_H
