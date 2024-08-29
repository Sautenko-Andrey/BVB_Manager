#ifndef TOURDRAW_H
#define TOURDRAW_H

#include <QDialog>
#include <QSqlDatabase>
#include "utils.h"
#include <QList>
#include <QPushButton>

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

    //void showTeamInfo();
    void W1_clicked();
    void W2_clicked();
    void W3_clicked();
    void W4_clicked();
    void W5_clicked();
    void W6_clicked();
    void W7_clicked();
    void W8_clicked();

private:
    Ui::TourDraw *ui;

    QSqlDatabase *db{nullptr};

    Tournament tournament;

    Net draw_type = Net::Sixteen;

    QVector<QPushButton *> first_round_team_btns;

    QVector<QPushButton *> W1_W8_buttons;

    void addFunction(QVector<void (*)()> &func_vector, void (*func)());
};

#endif // TOURDRAW_H
