#include "selecttournament.h"
#include "ui_selecttournament.h"

SelectTournament::SelectTournament(QSqlDatabase *database, QWidget *parent)
    : QDialog(parent)
    , db{database}
    , ui(new Ui::SelectTournament)
{
    ui->setupUi(this);
}

SelectTournament::~SelectTournament()
{
    delete ui;
}
