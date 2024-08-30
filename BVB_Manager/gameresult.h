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
    explicit GameResult(const QString team_1,
                        const QString team_2,
                        QPushButton *winer_basket,
                        QPushButton *loser_basket,
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

    QString first_team;
    QString second_team;
};

#endif // GAMERESULT_H
