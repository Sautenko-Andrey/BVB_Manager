#include "bvb_manager.h"
#include "./ui_bvb_manager.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QListWidget>
#include <QDebug>

BVB_Manager::BVB_Manager(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BVB_Manager)
{
    ui->setupUi(this);

    // Calendar settings
    ui->calendarWidget->setLocale(QLocale::English);
    ui->calendarWidget->setGridVisible(true);
    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    ui->calendarWidget->setFirstDayOfWeek(Qt::Monday);

    // Getting today's date
    current_date = "Date: " + ui->calendarWidget->selectedDate().toString("dd.MM.yyyy");

    // Training time settings
    ui->trainingTime->setTime(QTime(12, 01));

    // Work with the calendar
    // get a currnet date and update training date label
    updateDate();

    // get a current time and update training time label
    updateTime();

    // fill players list widget
    ui->playersListWidget->setStyleSheet(
        "QListWidget::item{border-bottom: 1px solid grey;}");

    QSqlQuery players_query(database_manager.getDatabase());

    if(!players_query.exec("SELECT id, first_name, last_name, hometown"
                           " FROM Players"
                           " ORDER BY first_name ASC;")){

        QMessageBox::warning(this, "Database error",
                             "Couldn't load players data from the database");
        return;
    }
    else{
        while(players_query.next()){
            ui->playersListWidget->addItem(
                "ID: " +
                players_query.value(0).toString() + ", " +   // id
                players_query.value(1).toString() + " " +   // first name
                players_query.value(2).toString() + ", ( " +   // second name
                players_query.value(3).toString()           // hometown
                + " )"
            );
        }
    }

    // fill exercises list widget
    ui->exercisesListWidget->setStyleSheet(
        "QListWidget::item{border-bottom: 1px solid grey;}");

    QSqlQuery exercises_query(database_manager.getDatabase());

    if(!exercises_query.exec("SELECT title, description, exercise_type"
                             " FROM Exercises"
                             " ORDER BY exercise_type ASC;")){
        QMessageBox::warning(this, "Database error",
                             "Couldn't load exercises data from the database");
        return;
    }
    else {
        while(exercises_query.next()){
            ui->exercisesListWidget->addItem(
                exercises_query.value(0).toString() + " ( " +    // exercise title
                exercises_query.value(2).toString() + " ) "      // exercise type
            );
        }
    }

    // make all exercises radio button checked by default
    ui->allExercisesRadioButton->setChecked(true);


    // signals & slots
    connect(ui->calendarWidget, SIGNAL(clicked(QDate)),
            this, SLOT(selectedDateChanged()));

    connect(ui->trainingTime, SIGNAL(timeChanged(QTime)),
            this, SLOT(selectedTimeChanged()));

    connect(ui->playersListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(selectedPlayer()));

    connect(ui->exercisesListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(selectedExercise()));

    // exercises list widget's radio buttons signals and slots
    // connect(ui->allExercisesRadioButton, SIGNAL(toggled(bool)),
    //         this, SLOT(exerciseRadioChanged()));                    !!! bug!!!

    connect(ui->warmUpExercisesRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(exerciseRadioChanged()));

    connect(ui->gymExercisesRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(exerciseRadioChanged()));

    connect(ui->cardioExercisesRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(exerciseRadioChanged()));

    connect(ui->tacticalexercisesRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(exerciseRadioChanged()));

}

BVB_Manager::~BVB_Manager()
{
    delete ui;
}


void BVB_Manager::updateDate(){
    // update training date

    selected_date = ui->calendarWidget->selectedDate();
    const QString training_date = "Date: " + selected_date.toString("dd.MM.yyyy");
    ui->dateSelectedLabel->setText(training_date);
}

void BVB_Manager::updateTime(){
    // update training time

    int hour = ui->trainingTime->time().hour();
    int minute = ui->trainingTime->time().minute();

    QString minutes = QString::number(minute);

    if(minute < 10){
        minutes = "0" + QString::number(minute);
    }

    ui->timeSelectedLabel->setText("Time: " +
                                   QString::number(hour) +
                                   ":" +
                                   minutes);
}

void BVB_Manager::selectedDateChanged(){
    // get a new date and update training date label
    updateDate();
}

void BVB_Manager::selectedTimeChanged(){
    updateTime();
}

void BVB_Manager::markItem(QListWidgetItem *item,
                           const QBrush &color, const QFont &font){

    item->setForeground(color);
    item->setFont(font);
}

void BVB_Manager::markUnmarkItem(QListWidget *list_widget, QSet<QString> &container,
                                 QString &text, QLabel *label){

    const QString current_item = list_widget->currentItem()->text();

    // unmark
    if(container.contains(current_item)){
        // when double clicked change current row text color to black
        markItem(list_widget->currentItem(),Qt::black, QFont("Helvetica [Cronyx]", 11));
        // remove the item from the container
        container.remove(current_item);
    }
    // mark
    else{
        // when double clicked change current row text color to red
        markItem(list_widget->currentItem(), Qt::red, QFont("Helvetica [Cronyx]", 13));
        // add marked item to the container
        container.insert(current_item);
    }

    // clear items string
    text.clear();

    // add players to the label
    for(const auto &item : container){
        text += (item + "\n");
    }

    // update the label
    label->setText(text);
}

// when double clicked on the player in players list widget
void BVB_Manager::selectedPlayer(){

    markUnmarkItem(ui->playersListWidget, marked_players,
                   combined_players, ui->playerNameLabel);
}

void BVB_Manager::selectedExercise(){

    markUnmarkItem(ui->exercisesListWidget, marked_exercises,
                   combined_exercises, ui->exerciseLabel);

}

void BVB_Manager::on_actionAdd_a_new_player_triggered()
{
    // add a new player
    add_player = std::make_unique<AddPlayer>(database_manager.getDatabase(), this);
    add_player->setWindowTitle("New player");
    add_player->show();
}


void BVB_Manager::on_actionDelete_a_player_triggered()
{
    // delete a desired player
    delete_player = std::make_unique<DeleteOnePlayer>(database_manager.getDatabase(), this);
    delete_player->setWindowTitle("Delete a player");
    delete_player->show();
}


void BVB_Manager::on_actionDelete_all_players_triggered()
{
    // delete all players from the database
    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, "Delete all players",
            "You are going to delete all players data from the database. Are you sure?",
            QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes){

        //deletin data from the database
        QSqlQuery query(database_manager.getDatabase());
        if(!query.exec("DELETE FROM Players")){
            QMessageBox::warning(this, "Error",
                                "Error occured when deleting all players from the database");
        }

        // deleting all pics from pics folder
        //deleting recursevly all images and folder
        // and then create the empty folder again
        QDir dir(QDir::homePath() + "/Players_images");

        if(dir.exists()){
            // delete the folder and all files
            dir.removeRecursively();

            // create a new empty folder for players pics
            dir.mkpath(QDir::homePath() + "/Players_images");
        }
        else{
            QMessageBox::warning(this, "Error", "Folder for pics doesn't exist.");
            return;
        }

        QMessageBox::information(this, "Report", "All players have been removed.");
    }
}

void BVB_Manager::on_actionChange_a_player_triggered()
{
    // player editor
    // change_player = std::make_unique<ChangePlayer>(database_manager.getDatabase(), this);
    // change_player->setWindowTitle("Change a player/players");
    // change_player->setGeometry(0, 0,
    //                           static_cast<int>(Sizes::ChangePlayerWindowWidth),
    //                           static_cast<int>(Sizes::ChangePlayerWindowHeight));
    // change_player->setMaximumWidth(static_cast<int>(Sizes::ChangePlayerWindowWidth));
    // change_player->setMinimumWidth(static_cast<int>(Sizes::ChangePlayerWindowWidth));
    // change_player->show();

    update_player = std::make_unique<UpdatePlayer>(database_manager.getDatabase(), this);
    update_player->setWindowTitle("Change a player/players");
    update_player->setGeometry(0, 0,
                               static_cast<int>(Sizes::ChangePlayerWindowWidth),
                               static_cast<int>(Sizes::ChangePlayerWindowHeight));
    update_player->setMaximumWidth(static_cast<int>(Sizes::ChangePlayerWindowWidth));
    update_player->setMinimumWidth(static_cast<int>(Sizes::ChangePlayerWindowWidth));
    update_player->show();
}


void BVB_Manager::on_actionAdd_a_new_exercise_triggered()
{
    // adding a new exercise
    add_exercise = std::make_unique<AddExercise>(database_manager.getDatabase(), this);
    add_exercise->setWindowTitle("Add a new exercise");
    add_exercise->show();
}


void BVB_Manager::on_actionDelete_an_exercise_triggered()
{
    // deleting an exercise
    delete_exercise = std::make_unique<DeleteExercise>(database_manager.getDatabase(),
                                                       this);
    delete_exercise->setWindowTitle("Delete an exercise");
    delete_exercise->setGeometry(0,0,
                                 static_cast<int>(Sizes::DeleteExerciseWindowWidth),
                                 static_cast<int>(Sizes::DeleteExerciseWindowHeight));
    delete_exercise->setMaximumWidth(static_cast<int>(Sizes::DeleteExerciseWindowMaxWidth));
    delete_exercise->show();
}


void BVB_Manager::on_actionChange_an_exercise_triggered()
{
    // upgrading an exercise
    change_exercise = std::make_unique<ChangeExercise>(database_manager.getDatabase(),
                                                       this);
    change_exercise->setWindowTitle("Change an exercise");
    change_exercise->show();

}


void BVB_Manager::on_actionSearch_a_player_triggered()
{
    // Search a player tool
    search_player = std::make_unique<SearchPlayer>(database_manager.getDatabase(),
                                                   this);
    search_player->setWindowTitle("Search a player");
    search_player->setGeometry(0, 0,
                               static_cast<int>(Sizes::SearchPlayerWindowWidth),
                               static_cast<int>(Sizes::SearchPlayerWindowHeight));
    search_player->setMinimumSize(static_cast<int>(Sizes::SearchPlayerWindowWidth),
                                  static_cast<int>(Sizes::SearchPlayerWindowHeight));
    search_player->show();
}


void BVB_Manager::removeListWidgetItems(QLabel *label, QListWidget *widget,
                                        QSet<QString> &container){
    // clear label
    label->clear();

    // make all wigets on QListWidget unmarked again
    for(auto i{0}; i < widget->count(); ++i){
        markItem(widget->item(i),
                 Qt::black, QFont("Helvetica [Cronyx]", 11));
    }

    // clear container
    container.clear();
}

void BVB_Manager::on_removeAllPlayersButton_clicked()
{
    removeListWidgetItems(ui->playerNameLabel, ui->playersListWidget, marked_players);
}


void BVB_Manager::on_addAllPlayersButton_clicked()
{
    // first of all clear current label data
    ui->playerNameLabel->clear();

    // clear general players string
    combined_players.clear();

    // add all players
    for(auto i{0}; i < ui->playersListWidget->count(); ++i){
        // create a players string
        combined_players += (ui->playersListWidget->item(i)->text() + "\n");

        // make each player marked
        markItem(ui->playersListWidget->item(i),
                 Qt::red, QFont("Helvetica [Cronyx]", 13));

        //add player to the marked_players container
        marked_players.insert(ui->playersListWidget->item(i)->text());
    }

    // update label
    ui->playerNameLabel->setText(combined_players);
}


void BVB_Manager::on_removeAllExercisesButton_clicked()
{
    removeListWidgetItems(ui->exerciseLabel, ui->exercisesListWidget, marked_exercises);
}

void BVB_Manager::on_resetCurrentSettingButton_clicked()
{
    // ask the user
    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, "Drop all settings",
                                  "Drop all your settings?",
                                  QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes){

        // make a training list blank

        removeListWidgetItems(ui->playerNameLabel, ui->playersListWidget, marked_players);

        removeListWidgetItems(ui->exerciseLabel, ui->exercisesListWidget, marked_exercises);

        ui->dateSelectedLabel->setText(current_date);

        ui->trainingTime->setTime(QTime(12, 01));
    }
}

void BVB_Manager::exerciseRadioChanged(){
    if(ui->allExercisesRadioButton->isChecked()){
        qDebug() << "All";
    }
    else if(ui->warmUpExercisesRadioButton->isChecked()){
        qDebug() << "Warm up";
    }
    else if(ui->gymExercisesRadioButton->isChecked()){
        qDebug() << "Gym";
    }
    else if(ui->cardioExercisesRadioButton->isChecked()){
        qDebug() << "Cardio";
    }
    else if(ui->tacticalexercisesRadioButton->isChecked()){
        qDebug() << "Tactical";
    }
}
