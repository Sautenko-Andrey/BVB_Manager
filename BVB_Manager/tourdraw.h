#ifndef TOURDRAW_H
#define TOURDRAW_H

#include <QDialog>
#include <QSqlDatabase>
#include <QList>
#include <QPushButton>
#include <memory>
#include "utils.h"
#include "gameresult.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QVarLengthArray>
#include <QPropertyAnimation>
#include "drawschema.h"
#include "draw24.h"


namespace Ui {
class TourDraw;
}

// enum class Net{ Sixteen = 16, TwentyFour = 24, ThirtyTwo = 32 };

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

    void click_game(QPushButton *team_1, QPushButton *team_2,
                    QPushButton *winner_basket, QPushButton *loser_basket,
                    QPushButton *game_result_btn, QPropertyAnimation *animation = nullptr);



    void moveTeams(const QList<QPushButton *> &teams,
                   QList<QPushButton *> &win_btns,
                   QList<QPushButton *> &loosers_btns,
                   QList<QPropertyAnimation *> &animations);


private:
    Ui::TourDraw *ui;

    QSqlDatabase *db{nullptr};

    Tournament tournament;

    Net draw_type = Net::Sixteen;

    QVector<QPushButton *> first_round_team_btns;

    QList<QPushButton *> L1_L12_buttons;

    QList<QPushButton *> L22_L21_buttons;

    QList<QPushButton *> W25_W26_looser_buttons;

    QList<QPushButton *> L27_L28_buttons;

    QList<QPushButton *> W_buttons;

    QList<QPushButton *> final_silver_bronze_buttons;

    QList<QPushButton *> game_result_buttons;

    QList<QPushButton *> loosers;

    std::unique_ptr<GameResult> game_result{nullptr};

    QPushButton *loser_btn{nullptr};

    QWidget *parent_dialog{nullptr};

    QPropertyAnimation *win_animation{nullptr};
    QPropertyAnimation *los_animation{nullptr};

    QList<QPropertyAnimation *> animations;
    QList<QPropertyAnimation *> animations_right_1;
    QList<QPropertyAnimation *> animations_right_2;

    QPushButton* drawGameResultBtn(const int x, const int y);

    QPushButton* drawTeamBtn(const int x, const int y,
                             const QString &team_name, bool is_disabled = false,
                             const QString &names = "Unknown");

    // Overriding method closeEvent for actions before closing a dialog window
    void closeEvent(QCloseEvent *event) override {

        // close parent and sub widgets
        parent_dialog->close();
        event->accept();
    }

    void moveForward(QPushButton *teamA, QPushButton *teamB,
                     QPushButton *win_btn, QPushButton *los_btn,
                     QPropertyAnimation *animation);

    void createAnimation(std::pair<int,int> &&range,
                         const QList<QPushButton *> &buttons,
                         QList<QPropertyAnimation *> &animations,
                         int duration);

    QPushButton* makeOpsBtn(const QRect &&geom, const QString &&text, const QString &style);

    std::unique_ptr<DrawSchema> draw_schema{nullptr};
};

#endif // TOURDRAW_H
