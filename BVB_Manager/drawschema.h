#ifndef DRAWSCHEMA_H
#define DRAWSCHEMA_H

#include <QDialog>
#include <QObject>
#include <QPropertyAnimation>
#include <QAbstractAnimation>
#include"utils.h"
#include "gameresult.h"

class DrawSchema : public QDialog
{
    Q_OBJECT

public:
    DrawSchema(QWidget* parent = nullptr);

    void moveForward(QPushButton *teamA, QPushButton *teamB,
                     QPushButton *win_btn, QPushButton *los_btn,
                     QPropertyAnimation *animation);

private slots:

    void click_game(QPushButton *team_1, QPushButton *team_2,
                    QPushButton *winner_basket, QPushButton *loser_basket,
                    QPushButton *game_result_btn, QPropertyAnimation *animation = nullptr);



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
};

#endif // DRAWSCHEMA_H
