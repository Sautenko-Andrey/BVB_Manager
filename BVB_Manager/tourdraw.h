#ifndef TOURDRAW_H
#define TOURDRAW_H

#include <QDialog>
#include <QSqlDatabase>
#include <QList>
#include <QPushButton>
#include <memory>
#include "utils.h"
#include "gameresult.h"



namespace Ui {
class TourDraw;
}

enum class Net{ Sixteen = 16, ThirtyTwo = 32, SixtyFour = 64 };

class TourDraw : public QDialog
{
    Q_OBJECT

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

    // QVector<QPushButton *> W1_W8_buttons;

    // QVector<QPushButton *> W9_W12_buttons;
    QList<QPushButton *> W1_W8_buttons;

    QList<QPushButton *> W9_W12_buttons;

    QList<QPushButton *> L1_L8_buttons;

    void addFunction(QVector<void (*)()> &func_vector, void (*func)());

    std::unique_ptr<GameResult> game_result{nullptr};

    void gameResult(const QString team_1, const QString team_2,
                    QPushButton *result_basket, QPushButton *loser_basket);

    bool result{true};
};

#endif // TOURDRAW_H
