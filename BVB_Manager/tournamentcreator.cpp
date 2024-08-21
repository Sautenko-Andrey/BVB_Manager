#include "tournamentcreator.h"
#include "ui_tournamentcreator.h"
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>
#include <QCheckBox>
// #include <QBoxLayout>

TournamentCreator::TournamentCreator(QSqlDatabase *database, QWidget *parent)
    : QDialog(parent)
    , db{database}
    , ui(new Ui::TournamentCreator)
{
    ui->setupUi(this);

    // make default starting and finishing date (current)
    ui->beginDate->setDate(QDate::currentDate());
    ui->endDate->setDate(QDate::currentDate());

    //search player window
    ui->searchByNameLine->setPlaceholderText("Search by name");

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

    // make tour by default
    selected_tournament = ui->tourComboBox->currentText();

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
        vbox = new QVBoxLayout(ui->teamsScrollArea);

        while(teams_query.next()){

            QCheckBox *checkbox = new QCheckBox(teams_query.value(0).toString() +
                                                " ( " +
                                                teams_query.value(2).toString() +
                                                " / " +
                                                teams_query.value(3).toString() +
                                                " ) , " +
                                                teams_query.value(4).toString() +
                                                " / " +
                                                teams_query.value(5).toString() +
                                                " , rank: " +
                                                teams_query.value(1).toString(),
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

    connect(ui->tabWidget, SIGNAL(currentChanged(int)),
           this, SLOT(tabChanged()));

    // search player by name signals and slots
    connect(ui->searchByNameLine, SIGNAL(textEdited(QString)),
            this, SLOT(searchPlayer()));
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
}

void TournamentCreator::on_addButton_clicked()
{
    // add teams to the tournament
    int counter{0};  // selected teams counter

    // save selected teams
    for(auto team : teams){
        if(team->isChecked()){
            //qDebug() << team->text();
            selected_teams.push_back(team);
            ++counter;
        }
    }

    // check teams counter
    if(counter > selected_tour_net_type.toInt()){
        QMessageBox::warning(this, "Player amount problem",
                             "You added to much players. Net for " +
                             selected_tour_net_type +
                             " , but you selected " +
                             QString::number(teams.size()) + " players.");
        return;
    }

    // Summary
    // Test
    // qDebug() << "Begin: " << ui->beginDate->text();
    // qDebug() << "End: " << ui->endDate->text();
    // qDebug() << "Gender: " << selected_tour_gender_type;
    // qDebug() << "Net: " << selected_tour_net_type;
    // qDebug() << "Tournament: " << selected_tournament;
    // qDebug() << "Teams: ";
    // for(auto team : selected_teams){
    //     qDebug() << team->text();
    // }

    completed_tournament.date_begin = ui->beginDate->text();
    completed_tournament.date_end = ui->endDate->text();
    completed_tournament.tour_gender_type = selected_tour_gender_type;
    completed_tournament.tour_net_type = selected_tour_net_type;
    completed_tournament.tour_title = selected_tournament;
    completed_tournament.selected_teams = selected_teams;

    // then call widget with draw and net
    // create tournament draw depends on user choice
    if(selected_tour_net_type.toInt() == static_cast<int>(TournamentMode::ofSixteen)){
        tour_draw =
            std::make_unique<DoubleEliminationTournament>(*db, TournamentMode::ofSixteen,
                                                          completed_tournament, this);
    }
    else if(selected_tour_net_type.toInt() == static_cast<int>(TournamentMode::ofThirtyTwo)){
        tour_draw =
            std::make_unique<DoubleEliminationTournament>(*db, TournamentMode::ofThirtyTwo,
                                                          completed_tournament, this);
    }
    else{
        tour_draw =
            std::make_unique<DoubleEliminationTournament>(*db, TournamentMode::ofSixtyFour,
                                                          completed_tournament, this);
    }

    tour_draw->setWindowTitle(completed_tournament.tour_title +
                              " ( " +
                              completed_tournament.date_begin +
                              " - " +
                              completed_tournament.date_end +
                              " ) ");

    tour_draw->show();
}

void TournamentCreator::tourChanged(){
    selected_tournament = ui->tourComboBox->currentText();
}


void TournamentCreator::tabChanged(){
    if(ui->tabWidget->currentIndex() == 0){
        team_registration = std::make_unique<TeamRegistration>(*db, this);
        team_registration->setWindowTitle("Register a new team");
        team_registration->show();
    }
    else{
        // update list of teams. Take the last record from Team table
        QSqlQuery last_team_query(*db);

        if(!last_team_query.exec("SELECT"
                                  " team_name, rank, first_player_name, second_player_name,"
                                  " first_player_city, second_player_city"
                                  " FROM Teams"
                                  " WHERE id = (SELECT MAX(id) FROM Teams);")){

            QMessageBox::warning(this, "Database error",
                                 "Couldn't load the last registered team");
            return;
        }
        else{
            if(last_team_query.next()){

                QCheckBox *checkbox = new QCheckBox(last_team_query.value(0).toString() +
                                                    " ( " +
                                                    last_team_query.value(2).toString() +
                                                    " / " +
                                                    last_team_query.value(3).toString() +
                                                    " ) , " +
                                                    last_team_query.value(4).toString() +
                                                    " / " +
                                                    last_team_query.value(5).toString(),
                                                    this);

                // don't add to the teams widget already existed team
                for(auto team : teams){
                    if(team->text() == checkbox->text()){
                        //qDebug() << "Doubled!";
                        return;
                    }
                }

                vbox->addWidget(checkbox);

                teams.push_back(checkbox);
            }

            ui->groupBox->setLayout(vbox);
        }
    }
}


void TournamentCreator::searchPlayer(){
    // every time print all teams widgets
    for(auto team: teams){
        team->show();
    }

    // every time compare team with the name filter
    for(auto team : teams){

        auto pos_start = team->text().indexOf("(") + 2;

        auto pos_end = team->text().indexOf(")");

        auto text = team->text().left(pos_end - 1);

        auto clear_names = text.right(text.size() - pos_start);

        // looking for match with search template
        if(clear_names.contains(ui->searchByNameLine->text())){
            // if template is blanked string don't italicize team
            if(ui->searchByNameLine->text() == ""){

                team->setFont(QFont("Ubuntu", 11, QFont::Normal));
            }
            else{
                team->setFont(QFont("Ubuntu", 11, QFont::Bold));
            }
        }
        else{
            team->hide();
        }

        // if team is checked make it bold
        if(team->isChecked()){
            team->setFont(QFont("Ubuntu", 11, QFont::Bold));
        }
    }
}

void TournamentCreator::on_clearSearchLineButton_clicked()
{
    ui->searchByNameLine->clear();

    // every time print all teams widgets
    for(auto team: teams){
        if(team->isChecked()){

            team->setFont(QFont("Ubuntu", 11, QFont::Bold));
        }
        else{
            team->setFont(QFont("Ubuntu", 11, QFont::Normal));
        }

        team->show();
    }
}


void TournamentCreator::on_selectAllTeamsButton_clicked()
{
    // select all teams
    for(auto team : teams){
        team->setChecked(true);
        team->setFont(QFont("Ubuntu", 11, QFont::Bold));
    }
}


void TournamentCreator::on_unselectAllTeamsButton_clicked()
{
    // unselect all teams
    for(auto team : teams){
        if(team->isChecked()){
            team->setChecked(false);
            team->setFont(QFont("Ubuntu", 11, QFont::Normal));
        }
    }
}
