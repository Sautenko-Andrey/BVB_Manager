#ifndef GAMERESULT_H
#define GAMERESULT_H

#include <QDialog>
#include <QPushButton>

namespace Ui {
class GameResult;
}

class GameResult : public QDialog
{
    Q_OBJECT

public:
    explicit GameResult(QPushButton *team_1,
                        QPushButton *team_2,
                        QPushButton *winer_basket,
                        QPushButton *loser_basket,
                        QPushButton *game_res_btn,
                        QWidget *parent = nullptr);
    ~GameResult();

private slots:

    void resultTeam1Changed();
    void resultTeam2Changed();

    void on_okButton_clicked();

private:
    Ui::GameResult *ui;

    QPushButton *winner;
    QPushButton *loser;

    QPushButton *first_team;
    QPushButton *second_team;

    QPushButton *game_result;
};

#endif // GAMERESULT_H
