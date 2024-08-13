#include "doubleeliminationtournament.h"
#include "ui_doubleeliminationtournament.h"

DoubleEliminationTournament::DoubleEliminationTournament(QSqlDatabase &database,
                                                         TournamentMode mode,
                                                         QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DoubleEliminationTournament)
{
    ui->setupUi(this);

    // pointer on database
    db = &database;
}

DoubleEliminationTournament::~DoubleEliminationTournament()
{
    delete ui;
}

void DoubleEliminationTournament::paintEvent(QPaintEvent *event)
{
    // Drawing a text

}
