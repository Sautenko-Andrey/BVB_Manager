#include "draw32.h"
#include "ui_draw32.h"

Draw32::Draw32(QSqlDatabase &db, Tournament *tour, QWidget *parent)
    : DrawSchema(parent)
    , ui(new Ui::Draw32)
{
    ui->setupUi(this);

    this->setWindowTitle("32 teams double elimination");

    // connect to the database
    setDB(&db);

    // getting a tournament data
    setTournament(tour);
}

Draw32::~Draw32()
{
    delete ui;
}
