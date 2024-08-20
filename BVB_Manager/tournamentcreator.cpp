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
        // QVBoxLayout *vbox = new QVBoxLayout(ui->teamsScrollArea);
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

    connect(ui->tabWidget, SIGNAL(currentChanged(int)),
           this, SLOT(tabChanged()));

    // search player by name signals and slots
    // connect(ui->searchByNameLine, SIGNAL(textChanged(QString)),
    //         this, SLOT(searchPlayer()));

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
    // qDebug() << "Net: " << selected_tour_net_type;
}

void TournamentCreator::on_addButton_clicked()
{
    int counter{0};
    for(auto team : teams){
        if(team->isChecked()){
            //qDebug() << team->text();
            ++counter;
        }
    }

    if(counter > selected_tour_net_type.toInt()){
        QMessageBox::warning(this, "Player amount problem",
                             "You added to much players. Net for " +
                             selected_tour_net_type +
                             " , but you selected " +
                             QString::number(teams.size()) + " players.");
        return;
    }
}

void TournamentCreator::tourChanged(){
    selected_tournament = ui->tourComboBox->currentText();
    // qDebug() << "Tour: " << selected_tournament;
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
                // vbox->addWidget(checkbox);

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
    qDebug() << ui->searchByNameLine->text();

    for(auto team : teams){

        auto pos_start = team->text().indexOf("(") + 2;

        auto pos_end = team->text().indexOf(")");

        auto text = team->text().left(pos_end - 1);

        auto clear_names = text.right(text.size() - pos_start);

        if(clear_names.contains(ui->searchByNameLine->text())){
            qDebug() << clear_names;
            team->setFont(QFont("Times", 10, QFont::Bold));
        }
        else{
            team->hide();
        }
    }

    //qDebug() << "-------------------------------";
}
