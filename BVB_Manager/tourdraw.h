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

    void W1_clicked();
    void W2_clicked();
    void W3_clicked();
    void W4_clicked();
    void W5_clicked();
    void W6_clicked();
    void W7_clicked();
    void W8_clicked();
    void W9_clicked();
    void W10_clicked();
    void W11_clicked();
    void W12_clicked();
    void W13_clicked();
    void W14_clicked();
    void W15_clicked();
    void W16_clicked();
    void W17_clicked();
    void W18_clicked();
    void W19_clicked();
    void W20_clicked();
    void W21_clicked();
    void W22_clicked();
    void W23_clicked();
    void W24_clicked();
    void W25_clicked();
    void W26_clicked();
    void W27_clicked();
    void W28_clicked();
    void bronze_match_clicked();
    void final_match_clicked();

private:
    Ui::TourDraw *ui;

    QSqlDatabase *db{nullptr};

    Tournament tournament;

    Net draw_type = Net::Sixteen;

    QVector<QPushButton *> first_round_team_btns;

    QList<QPushButton *> W1_W8_buttons;

    QList<QPushButton *> W13_W14_W15_W16_buttons;

    QList<QPushButton *> W9_W12_buttons;

    QList<QPushButton *> L1_L8_buttons;

    QList<QPushButton *> L9_L12_buttons;

    QList<QPushButton *> W21_W22_buttons;

    QList<QPushButton *> L22_L21_buttons;

    QList<QPushButton *> W25_W26_buttons;

    QList<QPushButton *> W25_W26_looser_buttons;

    QList<QPushButton *> W27_W28_buttons;

    QList<QPushButton *> L27_L28_buttons;

    QList<QPushButton *> W17_W18_W19_W20_buttons;

    QList<QPushButton *> W23_W24_buttons;

    QList<QPushButton *> W_buttons;

    QList<QPushButton *> final_silver_bronze_buttons;

    std::unique_ptr<GameResult> game_result{nullptr};

    void gameResult(QPushButton *team_1, QPushButton *team_2,
                    QPushButton *result_basket, QPushButton *loser_basket);

    void fontAdapter(QPushButton *btn);

    QPushButton *loser_btn{nullptr};
};

#endif // TOURDRAW_H
