#include "tournamentcreator.h"
#include "ui_tournamentcreator.h"
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>
#include <QCheckBox>
#include <QBoxLayout>

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

    // tournament settings
    ui->menRadioButton->setChecked(true);

    ui->netComboBox->addItems(tour_net_types);

    //prepare available tournaments
    QSqlQuery tours_query(*db);

    if(!tours_query.exec("SELECT title, location"
                         " FROM Tournaments"
                         " ORDER BY title ASC;")){
        QMessageBox::warning(this, "Database error",
                             "Couldn't load tournaments from the database");
        return;
    }
    else{
        while(tours_query.next()){
            ui->tourComboBox->addItem(tours_query.value(0).toString() +
                                      ", " +
                                      tours_query.value(1).toString());
        }
    }

    // prepare available teams
    // make a query
    QSqlQuery teams_query(*db);
    if(!teams_query.exec("SELECT"
                          " team_name, rank, first_player_name, second_player_name,"
                          " first_player_city, second_player_city"
                          " FROM Teams"
                          " ORDER BY rank DESC;")){
        QMessageBox::warning(this, "Database error",
                             "Couldn't load teams from the database");
        return;
    }
    else{
        QVBoxLayout *vbox = new QVBoxLayout(ui->teamsScrollArea);

        while(teams_query.next()){

            QCheckBox *checkbox = new QCheckBox(teams_query.value(0).toString() +
                                                " ( " +
                                                teams_query.value(2).toString() +
                                                " / " +
                                                teams_query.value(3).toString() +
                                                " ) , " +
                                                teams_query.value(4).toString() +
                                                " / " +
                                                teams_query.value(5).toString(),
                                                this);

            vbox->addWidget(checkbox);
            teams.push_back(checkbox);
        }

        ui->groupBox->setLayout(vbox);
    }


    // signals & slots
    connect(ui->beginDate, SIGNAL(dateChanged(QDate)),
            this, SLOT(tournamentDateChanged()));

    connect(ui->drationSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(tournamentDateChanged()));

    connect(ui->menRadioButton, SIGNAL(clicked(bool)),
            this, SLOT(tourTypeChanged()));

    connect(ui->womenRadioButton, SIGNAL(clicked(bool)),
            this, SLOT(tourTypeChanged()));

    connect(ui->mixRadioButton, SIGNAL(clicked(bool)),
            this, SLOT(tourTypeChanged()));

    connect(ui->netComboBox, SIGNAL(currentTextChanged(QString)),
            this, SLOT(netModeChanged()));

    connect(ui->tourComboBox, SIGNAL(currentTextChanged(QString)),
            this, SLOT(tourChanged()));
}

TournamentCreator::~TournamentCreator()
{
    delete ui;
}

void TournamentCreator::tournamentDateChanged()
{
    // change last tournament day date

    auto duration = ui->drationSpinBox->value();

    ui->endDate->setDate(ui->beginDate->date().addDays(duration == 0 ? duration : --duration));
}


void TournamentCreator::on_openCalendarButton_clicked()
{
    // open calendar widget
    calendar = std::make_unique<Calendar>(ui->beginDate, ui->endDate,
                                          ui->drationSpinBox, this);
    calendar->show();
}

void TournamentCreator::tourTypeChanged(){
    if(ui->menRadioButton->isChecked()){
        selected_tour_gender_type = tour_gender_types[0];
    }
    else if(ui->womenRadioButton->isChecked()){
        selected_tour_gender_type = tour_gender_types[1];
    }
    else{
        selected_tour_gender_type = tour_gender_types[2];
    }
}

void TournamentCreator::netModeChanged(){
    selected_tour_net_type = ui->netComboBox->currentText();
    // qDebug() << "Net: " << selected_tour_net_type;
}

void TournamentCreator::on_addButton_clicked()
{
    if(teams.size() > selected_tour_net_type.toInt()){
        QMessageBox::warning(this, "Player amount problem",
                             "You added to much players. " +
                             selected_tour_net_type +
                             " less then " +
                             QString::number(teams.size()));
        return;
    }

    for(auto team : teams){
        if(team->isChecked()){
            qDebug() << team->text();
        }
    }
}

void TournamentCreator::tourChanged(){
    selected_tournament = ui->tourComboBox->currentText();
    // qDebug() << "Tour: " << selected_tournament;
}
