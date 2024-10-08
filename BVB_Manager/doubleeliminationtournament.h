#ifndef DOUBLEELIMINATIONTOURNAMENT_H
#define DOUBLEELIMINATIONTOURNAMENT_H

#include <QDialog>
#include <QSqlDatabase>
#include "utils.h"

namespace Ui {
class DoubleEliminationTournament;
}

enum class TournamentMode {
    ofSixteen = 16, ofThirtyTwo = 32, ofSixtyFour = 64
};

class DoubleEliminationTournament : public QDialog
{
    Q_OBJECT

public:
    DoubleEliminationTournament(QSqlDatabase &database,
                                TournamentMode mode,
                                Tournament &tournament,
                                QWidget *parent = nullptr);
    ~DoubleEliminationTournament();

    virtual void paintEvent(QPaintEvent *event) override;

private:
    Ui::DoubleEliminationTournament *ui;

    QSqlDatabase *db{nullptr};

    TournamentMode tournament_mode;

    Tournament current_tournament;
};

#endif // DOUBLEELIMINATIONTOURNAMENT_H
