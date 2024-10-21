#ifndef DRAWSCHEMA_H
#define DRAWSCHEMA_H

#include <QDialog>
#include <QObject>
#include <QPropertyAnimation>
#include <QAbstractAnimation>
#include "utils.h"
#include "gameresult.h"
#include <QSqlDatabase>
#include <QPropertyAnimation>

class DrawSchema : public QDialog
{
    Q_OBJECT

public:
    DrawSchema(QWidget* parent = nullptr);

    void moveForward(QPushButton *teamA, QPushButton *teamB,
                     QPushButton *win_btn, QPushButton *los_btn,
                     QPropertyAnimation *animation);

    QPushButton *loser_btn{nullptr};

    QList<QPropertyAnimation *> animations_left_1;

    QList<QPropertyAnimation *> animations_left_2;

    QList<QPropertyAnimation *> animations_right_1;

    QList<QPropertyAnimation *> animations_right_2;

    QList<QPushButton *> loosers_btns;

    void setDB(QSqlDatabase *db);

    QSqlDatabase* getDB(){ return db; }

    void setTournament(Tournament *tournament){
        if(tournament != nullptr){
            this->tournament = tournament;
        }
    }

    Tournament* getTournament(){ return tournament; }

    void fillTeamBtns(Net net, const QList<QPushButton *> &all_teams,
                      const QSize &btn_size);

    void changeBtnStyle(QPushButton *btn, const QSize &size, const QString &css_style);

    void createAnimation(std::pair<int,int> &&range,
                         const QList<QPushButton *> &buttons,
                         QList<QPropertyAnimation *> &animations,
                         int duration);

    QStringList* getAllGamesResults() { return &all_games_results; }

public slots:

    void click_game(QPushButton *team_1, QPushButton *team_2,
                    QPushButton *winner_basket, QPushButton *loser_basket,
                    QPushButton *game_result_btn = nullptr,
                    QPropertyAnimation *animation = nullptr);


    void moveTeams(const QList<QPushButton *> &teams,
                   QList<QPushButton *> &win_btns,
                   QList<QPushButton *> &loosers_btns,
                   QList<QPropertyAnimation *> &animations);

private:

    std::unique_ptr<GameResult> game_result{nullptr};

    QWidget *parent_dialog{nullptr};

    // Overriding method closeEvent for actions before closing a dialog window
    void closeEvent(QCloseEvent *event) override {

        // close parent and sub widgets
        parent_dialog->close();
        event->accept();
    }

private:
    QSqlDatabase *db{nullptr};

    Tournament *tournament{nullptr};

    QStringList all_games_results;
};

#endif // DRAWSCHEMA_H
