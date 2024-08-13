#ifndef DOUBLEELIMINATIONTOURNAMENT_H
#define DOUBLEELIMINATIONTOURNAMENT_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class DoubleEliminationTournament;
}

enum class TournamentMode {
    ofSixteen, ofThirtyTwo, ofSixtyFour
};

class DoubleEliminationTournament : public QDialog
{
    Q_OBJECT

public:
    DoubleEliminationTournament(QSqlDatabase &database,
                                         TournamentMode mode,
                                         QWidget *parent = nullptr);
    ~DoubleEliminationTournament();

    virtual void paintEvent(QPaintEvent *event) override;

private:
    Ui::DoubleEliminationTournament *ui;

    QSqlDatabase *db{nullptr};
};

#endif // DOUBLEELIMINATIONTOURNAMENT_H
