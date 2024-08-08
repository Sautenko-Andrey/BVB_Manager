#include "bvb_manager.h"
#include "./ui_bvb_manager.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QListWidget>
#include <QFile>
#include <QInputDialog>
#include <QDebug>

BVB_Manager::BVB_Manager(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BVB_Manager)
{
    ui->setupUi(this);

    // prepare database manager
    database_manager.prepareManager(this);

    // Calendar settings
    ui->calendarWidget->setLocale(QLocale::English);
    ui->calendarWidget->setGridVisible(true);
    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    ui->calendarWidget->setFirstDayOfWeek(Qt::Monday);
    ui->langComboBox->addItems({"eng", "rus", "ukr", "swe", "chi"});

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
                           " ORDER BY first_name ASC, sex DESC;")){

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

    // make both players genders checked
    ui->menCheckBox->setChecked(true);
    ui->womenCheckBox->setChecked(true);

    // make all exercises checked by default
    ui->warmUpCheckBox->setChecked(true);
    ui->cardioCheckBox->setChecked(true);
    ui->gymCheckBox->setChecked(true);
    ui->tacticalCheckBox->setChecked(true);


    // signals & slots
    // calendar's area signals & slots
    connect(ui->gridCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(changeGridCheckBox()));

    connect(ui->weekNumsCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(changeWeekNumsCheckBox()));

    connect(ui->langComboBox, SIGNAL(currentTextChanged(QString)),
            this, SLOT(comboLangChanged()));

    connect(ui->calendarWidget, SIGNAL(clicked(QDate)),
            this, SLOT(selectedDateChanged()));

    connect(ui->trainingTime, SIGNAL(timeChanged(QTime)),
            this, SLOT(selectedTimeChanged()));

    connect(ui->playersListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(selectedPlayer()));

    connect(ui->exercisesListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(selectedExercise()));

    // players list widget's check boxes signals and slots
    connect(ui->menCheckBox, SIGNAL(checkStateChanged(Qt::CheckState)),
            this, SLOT(playersCheckBoxChanged()));

    connect(ui->womenCheckBox, SIGNAL(checkStateChanged(Qt::CheckState)),
            this, SLOT(playersCheckBoxChanged()));

    // exercises list widget's check boxes signals and slots
    connect(ui->warmUpCheckBox, SIGNAL(checkStateChanged(Qt::CheckState)),
            this, SLOT(exerciseCheckBoxChanged()));

    connect(ui->cardioCheckBox, SIGNAL(checkStateChanged(Qt::CheckState)),
            this, SLOT(exerciseCheckBoxChanged()));

    connect(ui->gymCheckBox, SIGNAL(checkStateChanged(Qt::CheckState)),
            this, SLOT(exerciseCheckBoxChanged()));

    connect(ui->tacticalCheckBox, SIGNAL(checkStateChanged(Qt::CheckState)),
            this, SLOT(exerciseCheckBoxChanged()));



}

BVB_Manager::~BVB_Manager()
{
    delete ui;
}


void BVB_Manager::getExercises(const QString &type){

    QSqlQuery exercises_query(database_manager.getDatabase());

    exercises_query.prepare("SELECT title, description, exercise_type"
                            " FROM Exercises WHERE exercise_type = :e_type"
                            " ORDER BY title ASC;");

    exercises_query.bindValue(":e_type", type);

    if(!exercises_query.exec()){
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
}


void BVB_Manager::getPlayers(bool gender){

    QSqlQuery query(database_manager.getDatabase());

    query.prepare("SELECT id, first_name, last_name, hometown"
                  " FROM Players"
                  " WHERE sex = :p_gender"
                  " ORDER BY first_name ASC;");

    query.bindValue(":p_gender", gender);

    if(!query.exec()){
        QMessageBox::warning(this, "Database error",
                             "Couldn't load players data from the database");
        return;
    }
    else{
        while(query.next()){
            ui->playersListWidget->addItem(
                "ID: " +
                query.value(0).toString() + ", " +   // id
                query.value(1).toString() + " " +   // first name
                query.value(2).toString() + ", ( " +   // second name
                query.value(3).toString()           // hometown
                + " )"
            );
        }
    }
}

void BVB_Manager::updateDate(){
    // update training date

    selected_date = ui->calendarWidget->selectedDate();
    const QString training_date = "Date: " + selected_date.toString("dd.MM.yyyy");
    ui->dateSelectedLabel->setText(training_date);
}

void BVB_Manager::updateTime(){
    // update training time
    timeToString();
}

QString BVB_Manager::timeToString(){

    int hour = ui->trainingTime->time().hour();
    int minute = ui->trainingTime->time().minute();

    QString minutes = QString::number(minute);

    if(minute < 10){
        minutes = "0" + QString::number(minute);
    }

    QString time_str = "Time: " + QString::number(hour) + ":" + minutes;

    ui->timeSelectedLabel->setText(time_str);

    return time_str;
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

    QString current_item = list_widget->currentItem()->text();

    // unmark

    // BUG! because it compares push_ups vs push_ups 3 x 10!!!!!!

    if(container.contains(current_item)){
        // when double clicked change current row text color to black
        markItem(list_widget->currentItem(),Qt::black, QFont("Ubuntu", 11));
        // remove the item from the container
        container.remove(current_item);
    }
    // mark
    else{

        // new---------------------------------------------------------------------
        qDebug() << current_item.right(6);

        int sets{0};
        int reps{0};

        if(current_item.right(6) == "Gym ) "){
            //QMessageBox::information(this, "Info", "Gym!");

            bool status_1, status_2;
            sets = QInputDialog::getInt(this, "Sets", "How many sets ?",
                                            1, 1, 20, 1, &status_1);
            reps = QInputDialog::getInt(this, "Reps", "How many reps ?",
                                        1, 1, 20, 1, &status_2);

            if(status_1 && status_2){
                qDebug() << sets << " x " << reps;
                // copy current_item
                current_item += (" " + QString::number(sets) +
                                 " x " +QString::number(reps));    // issue here
            }
        }

        //--------------------------------------------------------------------end new

        // when double clicked change current row text color to red
        markItem(list_widget->currentItem(), Qt::red, QFont("Ubuntu", 11));
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

    // test------------------------------------------------------
    for(const auto &item : container){
        qDebug() << item;
    }

    qDebug() << "-------------------------------";
    //-----------------------------------------------------------
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
        // deleting recursevly all images and folder
        // and then create the empty folder again
        const QString dir_name = "/Players_images";
        QDir dir(QDir::homePath() + dir_name);

        if(dir.exists()){
            // delete the folder and all files
            dir.removeRecursively();

            // create a new empty folder for players pics
            dir.mkpath(QDir::homePath() + dir_name);
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
                 Qt::black, QFont("Ubuntu", 11));
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
                 Qt::red, QFont("Ubuntu", 11));

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

        // make the editor blank
        clearEditor();
    }
}

void BVB_Manager::clearEditor(){
    // make a training list blank

    removeListWidgetItems(ui->playerNameLabel, ui->playersListWidget, marked_players);

    removeListWidgetItems(ui->exerciseLabel, ui->exercisesListWidget, marked_exercises);

    ui->dateSelectedLabel->setText(current_date);

    ui->trainingTime->setTime(QTime(12, 01));

    ui->NotesTextEdit->clear();
}

void BVB_Manager::exerciseCheckBoxChanged(){

    // first of all clear data
    ui->exercisesListWidget->clear();

    // then filling widget in case what exercises user wanna see

    if(ui->warmUpCheckBox->isChecked()){
        getExercises("Warm up");
    }

    if(ui->cardioCheckBox->isChecked()){
        getExercises("Cardio");
    }

    if(ui->gymCheckBox->isChecked()){
        getExercises("Gym");
    }

    if(ui->tacticalCheckBox->isChecked()){
        getExercises("Tactical-technical");
    }
}


void BVB_Manager::playersCheckBoxChanged(){

    // first of all take away data from widget
    ui->playersListWidget->clear();

    // then refill widget
    if(ui->menCheckBox->isChecked()){
        getPlayers(true);
    }

    if(ui->womenCheckBox->isChecked()){
        getPlayers(false);
    }
}

void BVB_Manager::on_addToscheduleButton_clicked()
{
    // increase counter's value
    ++trainings_counter;

    // creatin a new folder with txt files
    const QString folder_name = "/Trainings";

    QDir dir;

    if(!dir.exists(QDir::homePath() + folder_name)){
        dir.mkpath(QDir::homePath() + folder_name);
    }

    // make file name
    QString file_name = "/" + QString::number(trainings_counter) +
                              "_" + current_date.right(10);

    // clearence
    file_name = file_name.trimmed().replace(" ", "_").replace(".", "_");

    // make a txt file with a training schema
    const QString txt_file(QDir::homePath() + folder_name +file_name + ".txt");

    // create a text for the pdf file

    QStringList labels;
    labels << current_date << timeToString()
           << "\nPlayers: "
           << ui->playerNameLabel->text()
           << "Exercises:"
           << ui->exerciseLabel->text()
           << "Comments:"
           << ui->NotesTextEdit->toPlainText();

    QFile file(txt_file);
    if(file.open(QFile::WriteOnly | QIODevice::Text)){
        QTextStream stream(&file);
        stream << labels.join("\n");
    }

    // after all clear the editor
    clearEditor();
}

void BVB_Manager::on_eraseButton_clicked()
{
    // erase all text in comments area
    ui->NotesTextEdit->clear();
}


void BVB_Manager::on_redoButton_clicked()
{
    ui->NotesTextEdit->redo();
}


void BVB_Manager::on_undoButton_clicked()
{
    ui->NotesTextEdit->undo();
}

void BVB_Manager::changeGridCheckBox(){

    if(!ui->gridCheckBox->isChecked()){
        ui->calendarWidget->setGridVisible(false);
    }
    else{
        ui->calendarWidget->setGridVisible(true);
    }
}

void BVB_Manager::changeWeekNumsCheckBox(){

    if(ui->weekNumsCheckBox->isChecked()){
        ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::ISOWeekNumbers);
    }
    else{
        ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    }
}

void BVB_Manager::comboLangChanged(){

    if(ui->langComboBox->currentText() == "eng"){
        ui->calendarWidget->setLocale(QLocale::English);
    }
    else if(ui->langComboBox->currentText() == "rus"){
        ui->calendarWidget->setLocale(QLocale::Russian);
    }
    else if(ui->langComboBox->currentText() == "ukr"){
        ui->calendarWidget->setLocale(QLocale::Ukrainian);
    }
    else if(ui->langComboBox->currentText() == "swe"){
        ui->calendarWidget->setLocale(QLocale::Swedish);
    }
    else{
        ui->calendarWidget->setLocale(QLocale::Chinese);
    }
}
