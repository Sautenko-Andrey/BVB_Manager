#include "tournamentcreator.h"
#include "ui_tournamentcreator.h"
#include <QDebug>

TournamentCreator::TournamentCreator(QSqlDatabase *database, QWidget *parent)
    : QDialog(parent)
    , db{database}
    , ui(new Ui::TournamentCreator)
{
    ui->setupUi(this);

    // make default starting and finishing date (current)
    ui->beginDate->setDate(QDate::currentDate());
    ui->endDate->setDate(QDate::currentDate());

    // make last day widget unclickable
    ui->endDate->setDisabled(true);



    // signals & slots
    connect(ui->beginDate, SIGNAL(dateChanged(QDate)),
            this, SLOT(tournamentDateChanged()));

    connect(ui->drationSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(tournamentDateChanged()));
}

TournamentCreator::~TournamentCreator()
{
    delete ui;
}

void TournamentCreator::tournamentDateChanged()
{
    // change last tournament day date
    ui->endDate->setDate(ui->beginDate->date().addDays(ui->drationSpinBox->value() - 1));
}


void TournamentCreator::on_openCalendarButton_clicked()
{
    // open calendar widget
    calendar = std::make_unique<Calendar>(this);
    calendar->show();

    auto date = calendar->getDate();   // doesn't work properly

    qDebug() << date;
}

