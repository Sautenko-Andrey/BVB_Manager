#ifndef TOURDRAW_H
#define TOURDRAW_H

#include <QDialog>
#include <QSqlDatabase>
#include "utils.h"
#include <QList>

namespace Ui {
class TourDraw;
}

enum class Net{ Sixteen = 16, ThirtyTwo = 32, SixtyFour = 64 };

class TourDraw : public QDialog
{
    Q_OBJECT

    //            std::make_unique<TourDraw>(*db, Net::Sixteen,
                                       //completed_tournament, this);

public:
    TourDraw(QSqlDatabase &database,
             Net net_type,
             Tournament completed_tournament,
             QWidget *parent = nullptr);
    ~TourDraw();

    virtual void paintEvent(QPaintEvent *event);

private slots:

    void showTeamInfo();

private:
    Ui::TourDraw *ui;

    QSqlDatabase *db{nullptr};

    Tournament tournament;

    Net draw_type = Net::Sixteen;

    QVector<QPushButton *> first_round_team_btns;

    //QVector<QPushButton *> game_res_1r_btns;

    QList<QPushButton *> game_res_1r_btns;
};

#endif // TOURDRAW_H
