#include "tournamentcreator.h"
#include "ui_tournamentcreator.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QCheckBox>
#include <algorithm>


TournamentCreator::TournamentCreator(QSqlDatabase *database, QWidget *parent)
    : QDialog(parent)
    , db{database}
    , ui(new Ui::TournamentCreator)
{
    ui->setupUi(this);

    // Reserve memory for 16 teams at least
    constexpr int possible_teams_amount{16};
    teams.reserve(possible_teams_amount);
    selected_teams.reserve(possible_teams_amount);

    // make save button disabled till user select at least 8 teams
    ui->addButton->setDisabled(true);

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

    // for selected teams counter
    for(auto team : teams){
        connect(team, &QCheckBox::toggled,
                this, &TournamentCreator::selectedTeamsChanged);
    }
}

TournamentCreator::~TournamentCreator()
{
    delete ui;
}


/*
    Function triggers when user changes tournament date
*/
void TournamentCreator::tournamentDateChanged()
{
    auto duration = ui->drationSpinBox->value();

    ui->endDate->setDate(ui->beginDate->date().addDays(duration == 0 ? duration : --duration));
}


/*
    Function opens calendar widget
*/
void TournamentCreator::on_openCalendarButton_clicked()
{
    // open calendar widget
    calendar = std::make_unique<Calendar>(ui->beginDate, ui->endDate,
                                          ui->drationSpinBox, this);
    calendar->show();
}


/*
    Function triggers when user changes tournament type(gender)
*/
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


/*
    Function triggers when user changes tournament type(16, 24, 32 net)
*/
void TournamentCreator::netModeChanged(){

    selected_tour_net_type = ui->netComboBox->currentText();

    // change teams counter value
    ui->teamsCounterLabel->setText("Selected teams total: 0 / " + selected_tour_net_type);

    // drop all previous actions
    std::for_each(teams.begin(), teams.end(), [](QCheckBox *box){
        box->setChecked(false);
    });
}


/*
    Function adds a team to the tournament
*/
void TournamentCreator::on_addButton_clicked()
{
    // add teams to the tournament
    // save selected teams
    for(auto team : teams){
        if(team->isChecked()){
            selected_teams.push_back(team);
        }
    }

    while(selected_teams.size() != 32){
        QCheckBox * empty_team = new QCheckBox("None, None, None", this);
        selected_teams.push_back(empty_team);
    }

    completed_tournament.date_begin = ui->beginDate->text();
    completed_tournament.date_end = ui->endDate->text();
    completed_tournament.tour_gender_type = selected_tour_gender_type;
    completed_tournament.tour_net_type = selected_tour_net_type;
    completed_tournament.tour_title = selected_tournament;
    completed_tournament.selected_teams = selected_teams;

    if(selected_tour_net_type.toInt() == static_cast<int>(Net::Sixteen)){

        tour_draw =
            std::make_unique<TourDraw>(*db, Net::Sixteen,
                                       completed_tournament, this);

        tour_draw->setWindowTitle(completed_tournament.tour_title +
                                  " ( " +
                                  completed_tournament.date_begin +
                                  " - " +
                                  completed_tournament.date_end +
                                  " ) ");

        tour_draw->show();

        // make all child widget disabled for security purpose
        // Get all child widgets of the parent
        const QObjectList &children = this->children();

        for (QObject *child : children)
        {
            // Convert QObject to QWidget to access widget-specific functions
            QWidget *widget = qobject_cast<QWidget *>(child);

            // Skip null objects and the exception widget (managed by shared_ptr)
            if (widget && widget != tour_draw.get())
            {
                widget->setEnabled(false);  // Disable the widget
            }
        }
    }
    else if(selected_tour_net_type.toInt() == static_cast<int>(Net::TwentyFour)){

        // drawing a schema for 24 teams
        draw_schema = std::make_unique<Draw24>(*db,
                                               &completed_tournament, this);
        draw_schema->show();

    }
    else{
        // drawing a schema for 32 teams
        draw_schema = std::make_unique<Draw32>(*db,
                                               &completed_tournament, this);
        draw_schema->show();
    }
}


void TournamentCreator::tourChanged(){
    selected_tournament = ui->tourComboBox->currentText();
}


/*
    Function triggers when user change pages on the tab
*/
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
                        delete checkbox;
                        return;
                    }
                }

                teams.push_back(checkbox);
                vbox->addWidget(checkbox);
            }

            if (ui->groupBox->layout() != vbox) {
                ui->groupBox->setLayout(vbox);
            }
        }
    }
}


/*
    Function for searhing player in database
*/
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

        const auto clear_names = text.right(text.size() - pos_start);

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


/*
    Function simply clears player searching line
*/
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

/*
    Function selects all teams when in neccessary
*/
void TournamentCreator::on_selectAllTeamsButton_clicked()
{
    // select all teams
    for(auto team : teams){
        team->setChecked(true);
        team->setFont(QFont("Ubuntu", 11, QFont::Bold));
    }
}

/*
    Function unselects all teams when in neccessary
*/
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


/*
    Function counts selected teams, compare it to the limit
    and if everything is ok it allows user to push confirm button
*/
void TournamentCreator::selectedTeamsChanged(){

    auto total = std::count_if(teams.cbegin(), teams.cend(), [](QCheckBox *box){
        return box->isChecked();
    });

    ui->teamsCounterLabel->setText(
        "Selected teams total: " +
        QString::number(total) + " / " +
        selected_tour_net_type
    );

    // check teams amount limits

    if(constexpr int min_teams_amount{8}; total == min_teams_amount){
        // make save button available when user selected 8 teams (minimum)
        ui->addButton->setDisabled(false);
    }
    // when user achieves teams amount limit make all unchecked checkboxes disabled
    else if(total >= selected_tour_net_type.toInt()){
        std::for_each(teams.begin(), teams.end(), [](QCheckBox *box){
            if(!box->isChecked()){
                box->setDisabled(true);
            }

        });
    }
    else{
        // if user is in range and doesn't cross limits
        // he is allowed to add teams
        std::for_each(teams.begin(), teams.end(), [](QCheckBox *box){
            if(!box->isEnabled())
            box->setEnabled(true);
        });
    }
}


/*
    Function updates teams list
*/
void TournamentCreator::on_updateListButton_clicked()
{
    // clean vbox
    QLayoutItem* item{nullptr};
    while ((item = vbox->takeAt(0)) != nullptr) {
        // Remove the widget associated with the item
        if (item->widget()) {
            delete item->widget(); // This deletes the widget
        }
        // Delete the layout item itself
        delete item;
    }

    // Clear teams container
    for (auto team : teams) {
        delete team;  // Delete every check box
    }

    //clean teams container
    teams.clear();

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

        // Remove memory leack
        if (ui->groupBox->layout() != vbox) {
            if (ui->groupBox->layout()) {
                delete ui->groupBox->layout();  // Delete old group box
            }
            ui->groupBox->setLayout(vbox);  // set new group box
        }
    }
}
