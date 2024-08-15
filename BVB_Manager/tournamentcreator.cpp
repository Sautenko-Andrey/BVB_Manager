#include "tournamentcreator.h"
#include "ui_tournamentcreator.h"

TournamentCreator::TournamentCreator(QSqlDatabase *database, QWidget *parent)
    : QDialog(parent)
    , db{database}
    , ui(new Ui::TournamentCreator)
{
    ui->setupUi(this);
}

TournamentCreator::~TournamentCreator()
{
    delete ui;
}
