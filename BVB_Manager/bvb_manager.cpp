#include "bvb_manager.h"
#include "./ui_bvb_manager.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QListWidget>
#include <QFile>
#include <QInputDialog>
#include "choosedate.h"

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
    ui->calendarWidget->setStyleSheet(calendar_style);

    // Language combobox style settings
    ui->langComboBox->addItems({"eng", "rus", "ukr", "swe", "chi"});
    ui->langComboBox->setStyleSheet(combobox_style);

    // Grid checkbox style settings
    ui->gridCheckBox->setStyleSheet(checkbox_style);

    // Weeks checkbox style settings
    ui->weekNumsCheckBox->setStyleSheet(checkbox_style);

    // LCD number style settings
    ui->lcdNumber->setStyleSheet(lcd_number_style);

    // Training time style settings
    ui->trainingTime->setStyleSheet(time_edit_style);

    // Players checkbox style settings
    ui->menCheckBox->setStyleSheet(checkbox_style);
    ui->womenCheckBox->setStyleSheet(checkbox_style);

    // Exercises checkbox settings
    ui->warmUpCheckBox->setStyleSheet(checkbox_style);
    ui->gymCheckBox->setStyleSheet(checkbox_style);
    ui->tacticalCheckBox->setStyleSheet(checkbox_style);
    ui->cardioCheckBox->setStyleSheet(checkbox_style);

    // Push buttons style settings
    ui->removeAllPlayersButton->setStyleSheet(pushbtn_style);
    ui->addAllPlayersButton->setStyleSheet(pushbtn_style);
    ui->removeAllExercisesButton->setStyleSheet(pushbtn_style);
    ui->resetCurrentSettingButton->setStyleSheet(pushbtn_style);
    ui->addToscheduleButton->setStyleSheet(pushbtn_style);
    ui->eraseButton->setStyleSheet(pushbtn_style);
    ui->redoButton->setStyleSheet(pushbtn_style);
    ui->undoButton->setStyleSheet(pushbtn_style);

    // Scroll area style settings
    ui->scrollArea->setStyleSheet(scroll_area_style);

    // Edit area labels style settings
    ui->dateSelectedLabel->setStyleSheet(label_style_hover);
    ui->timeSelectedLabel->setStyleSheet(label_style_hover);
    ui->playerPlayersLabel->setStyleSheet(label_style_hover);
    ui->playerNameLabel->setStyleSheet(label_style_hover);
    ui->exrciseExercisesLabel->setStyleSheet(label_style_hover);
    ui->exerciseLabel->setStyleSheet(label_style_hover);

    // Install event filter on the QLabel
    ui->dateSelectedLabel->installEventFilter(this);
    ui->timeSelectedLabel->installEventFilter(this);
    ui->playerNameLabel->installEventFilter(this);
    ui->exerciseLabel->installEventFilter(this);

    // Notes text edit style settings
    ui->NotesTextEdit->setStyleSheet(text_edit_style);


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

    // list widgets style
    ui->playersListWidget->setStyleSheet(list_widget_style);
    ui->exercisesListWidget->setStyleSheet(list_widget_style);

    // make both players genders checked
    ui->menCheckBox->setChecked(true);
    ui->womenCheckBox->setChecked(true);

    // make all exercises checked by default
    ui->warmUpCheckBox->setChecked(true);
    ui->cardioCheckBox->setChecked(true);
    ui->gymCheckBox->setChecked(true);
    ui->tacticalCheckBox->setChecked(true);

    // get trainings amount for current day
    ui->lcdNumber->display(getTrainings(ui->calendarWidget->selectedDate()));


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

    // double click on the date when user wants to see training on selected day
    // via calendar
    connect(ui->calendarWidget, SIGNAL(activated(QDate)),
            this , SLOT(dateActivated()));

    // one click on the date to show how many trainings scheduled in selected day
    connect(ui->calendarWidget, SIGNAL(clicked(QDate)),
            this, SLOT(dateClicked()));
}

BVB_Manager::~BVB_Manager()
{
    delete ui;
}


/*
    Function shows list of exercises depending on user's choice type
*/
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


/*
    Function shows to user a list of available players
*/
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


/*
    Function updates training date in the schedule area
*/
void BVB_Manager::updateDate(){

    selected_date = ui->calendarWidget->selectedDate();
    const QString training_date = "Date: " + selected_date.toString("dd.MM.yyyy");
    ui->dateSelectedLabel->setText(training_date);
}


/*
    Function updates training time in the schedule area
*/
void BVB_Manager::updateTime(){
    timeToString();
}


/*
    Function converts time to QString type
*/
QString BVB_Manager::timeToString(){

    int hour = ui->trainingTime->time().hour();
    int minute = ui->trainingTime->time().minute();

    QString minutes = QString::number(minute);

    if(minute < 10){
        minutes = "0" + QString::number(minute);
    }

    QString time_str = "Time: " + QString::number(hour) + ":" + std::move(minutes);

    ui->timeSelectedLabel->setText(time_str);

    return std::move(time_str);
}


/*
    Function gets a new date and updates training date label
*/
void BVB_Manager::selectedDateChanged(){
    updateDate();
}


/*
    Function gets a new time and updates training date label
*/
void BVB_Manager::selectedTimeChanged(){
    updateTime();
}



/*
    Function marks an item when user double clicks on it
*/
void BVB_Manager::markItem(QListWidgetItem *item, const QBrush &color, bool is_bold){

    QFont item_font;

    if(item != nullptr){
        item->setForeground(color);

        // when marking item we apply this
        if (is_bold){
            item_font.setBold(true);
            item_font.setItalic(true);
            item_font.setPointSize(12);
        }

        item->setFont(item_font);
    }
}


/*
    Function allows a user to mark and unmark item (player or exercise)
    when it neccessary.
*/
void BVB_Manager::markUnmarkItem(QListWidget *list_widget, QStringList &container,
                                 QString &text, QLabel *label){

    if(list_widget != nullptr && label != nullptr){

        QString current_item = list_widget->currentItem()->text();

        // unmark
        if(container.contains(current_item)){

            // if this is a gym exercise
            if(current_item.contains("( Gym )")){
                // return the original name of the gymexercise
                list_widget->currentItem()->setText(original_gym_exercise_name);
            }

            // when double clicked change current row text color to black
            markItem(list_widget->currentItem(), Qt::black, false);

            // remove the item from the container
            container.removeAll(current_item);
        }
        // mark
        else{
            int sets{0};   // sets amount
            int reps{0};   // reps amount

            if(current_item.right(6) == "Gym ) "){
                // save an original name of the exercise
                original_gym_exercise_name = current_item;

                bool status_1, status_2;
                // 20 sets maximum
                sets = QInputDialog::getInt(this, "Sets", "How many sets ?",
                                            1, 1, 20, 1, &status_1);
                // 100 reps maximum
                reps = QInputDialog::getInt(this, "Reps", "How many reps ?",
                                            1, 1, 100, 1, &status_2);

                if(status_1 && status_2){
                    // change current_item (adding sets and reps)
                    current_item += (" " + QString::number(sets) +
                                     " x " +QString::number(reps));

                    // changing current item text
                    list_widget->currentItem()->setText(current_item);
                }
            }

            // when double clicked change current row text color to red
            markItem(list_widget->currentItem(), Qt::red);

            // add marked item to the container
            container.append(current_item);
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
}



/*
    Function is called when double clicked on the player in players list widget
*/
void BVB_Manager::selectedPlayer(){

    markUnmarkItem(ui->playersListWidget, marked_players,
                   combined_players, ui->playerNameLabel);
}


/*
    Function is called when double clicked on the exercise in exercises list widget
*/
void BVB_Manager::selectedExercise(){

    markUnmarkItem(ui->exercisesListWidget, marked_exercises,
                   combined_exercises, ui->exerciseLabel);
}


/*
    Function creates the AddPlayerWidget
*/
void BVB_Manager::on_actionAdd_a_new_player_triggered()
{
    // add a new player
    add_player = std::make_unique<AddPlayer>(database_manager.getDatabase(),
                                             this, true, "", "", ui->playersListWidget);

    add_player->setWindowTitle("New player");
    add_player->setFixedSize(685, 342);
    add_player->show();
}


/*
    Function creates the DeleteOnePlayer
*/
void BVB_Manager::on_actionDelete_a_player_triggered()
{
    // delete a desired player
    delete_player = std::make_unique<DeleteOnePlayer>(database_manager.getDatabase(), this);
    delete_player->setWindowTitle("Delete a player");
    delete_player->show();
}


/*
    Function deletes all players from the database
*/
void BVB_Manager::on_actionDelete_all_players_triggered()
{
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

        // const QString dir_name = "/Players_images";
        QString dir_name = "/Players_images";
        QDir dir(QDir::homePath() + dir_name);

        if(dir.exists()){
            // delete the folder and all files
            dir.removeRecursively();

            // create a new empty folder for players pics
            dir.mkpath(QDir::homePath() + std::move(dir_name));
        }
        else{
            QMessageBox::warning(this, "Error", "Folder for pics doesn't exist.");
            return;
        }

        QMessageBox::information(this, "Report", "All players have been removed.");
    }
}


/*
    Function calls the UpdatePlayer widget
*/
void BVB_Manager::on_actionChange_a_player_triggered()
{
    // player editor
    update_player = std::make_unique<UpdatePlayer>(database_manager.getDatabase(), this);
    update_player->setWindowTitle("Change a player/players");
    update_player->setGeometry(0, 0,
                               static_cast<int>(Sizes::ChangePlayerWindowWidth),
                               static_cast<int>(Sizes::ChangePlayerWindowHeight));
    update_player->setMaximumWidth(static_cast<int>(Sizes::ChangePlayerWindowWidth));
    update_player->setMinimumWidth(static_cast<int>(Sizes::ChangePlayerWindowWidth));
    update_player->show();
}


/*
    Function calls the AddExercise widget
*/
void BVB_Manager::on_actionAdd_a_new_exercise_triggered()
{
    // adding a new exercise
    add_exercise = std::make_unique<AddExercise>(database_manager.getDatabase(),
                                                 this, ui->exercisesListWidget);
    add_exercise->setWindowTitle("Add a new exercise");
    add_exercise->show();
}


/*
    Function calls the DeleteExercise widget
*/
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


/*
    Function calls the ChangeExercise widget
*/
void BVB_Manager::on_actionChange_an_exercise_triggered()
{
    // upgrading an exercise
    change_exercise = std::make_unique<ChangeExercise>(database_manager.getDatabase(),
                                                       this);
    change_exercise->setWindowTitle("Change an exercise");
    change_exercise->show();
}


/*
    Function calls the SearchPlayer widget
*/
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



/*
    Function make undo for added to list exercises/players
*/
void BVB_Manager::removeListWidgetItems(QLabel *label, QListWidget *widget,
                                        QStringList &container){

    if(label != nullptr && widget != nullptr){
        // clear label
        label->clear();

        // make all wigets on QListWidget unmarked again
        for(auto i{0}; i < widget->count(); ++i){

            // for all gym exercises return their original names
            if(widget->item(i)->text().contains("( Gym )")){
                // remove sets and reps from exercise name(getting name as original)
                QString correct_name = widget->item(i)->text().left((widget->item(i)->text().indexOf(")")) + 2);
                widget->item(i)->setText(correct_name);
            }
            markItem(widget->item(i),
                     Qt::black, false);
        }

        // clear container
        container.clear();
    }
}


/*
    Function removes all selected players from the schedule
*/
void BVB_Manager::on_removeAllPlayersButton_clicked()
{
    removeListWidgetItems(ui->playerNameLabel, ui->playersListWidget, marked_players);
}


/*
    Function adds all available players into the schedule list
*/
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
                 Qt::red);

        //add player to the marked_players container
        marked_players.append(ui->playersListWidget->item(i)->text());
    }

    // update label
    ui->playerNameLabel->setText(combined_players);
}


/*
    Function removes all selected exercises from the schedule
*/
void BVB_Manager::on_removeAllExercisesButton_clicked()
{
    removeListWidgetItems(ui->exerciseLabel, ui->exercisesListWidget, marked_exercises);
}


/*
    Function drops down all settings and selected players/exercises
*/
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


/*
    Function makes the schedule list blank
*/
void BVB_Manager::clearEditor(){

    removeListWidgetItems(ui->playerNameLabel, ui->playersListWidget, marked_players);

    removeListWidgetItems(ui->exerciseLabel, ui->exercisesListWidget, marked_exercises);

    ui->dateSelectedLabel->setText(current_date);

    ui->trainingTime->setTime(QTime(12, 01));

    ui->NotesTextEdit->clear();
}


/*
    Function changes list of exercises depending on user's choice
*/
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


/*
    Function changes list of players depending on user's choice
*/
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


/*
    Function saves all setings into database and moreover create a txt file
    with all selected players, date, time, exercises and coach comments.
*/
void BVB_Manager::on_addToscheduleButton_clicked()
{
    // increase counter's value
    ++trainings_counter;

    // creatin a new folder with txt files
    QString folder_name = "/Trainings";

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
    const QString txt_file(QDir::homePath() +
                           std::move(folder_name) +
                           std::move(file_name) + ".txt");

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

        // adding training info to the database
        QSqlQuery query(database_manager.getDatabase());

        query.prepare("INSERT INTO Trainings (description) VALUES (:descr);");

        query.bindValue(":descr", labels.join("\n"));

        if(!query.exec()){
            QMessageBox::warning(this, "New training saving error",
                                 "Training has not been saving");
        }
        else{
            QMessageBox::information(this, "New training saving",
                                     "Training has been saving");
        }
    }

    // after all clear the editor
    clearEditor();
}


/*
    Function clears comment area
*/
void BVB_Manager::on_eraseButton_clicked()
{
    // erase all text in comments area
    ui->NotesTextEdit->clear();
}


/*
    Function do the same thing as redo
*/
void BVB_Manager::on_redoButton_clicked()
{
    ui->NotesTextEdit->redo();
}


/*
    Function do the same thing as undo
*/
void BVB_Manager::on_undoButton_clicked()
{
    ui->NotesTextEdit->undo();
}


/*
    Function add or hide grid on a calendar widget
*/
void BVB_Manager::changeGridCheckBox(){

    if(!ui->gridCheckBox->isChecked()){
        ui->calendarWidget->setGridVisible(false);
    }
    else{
        ui->calendarWidget->setGridVisible(true);
    }
}


/*
    Function adds/removes vertical header for the calendar
*/
void BVB_Manager::changeWeekNumsCheckBox(){

    if(ui->weekNumsCheckBox->isChecked()){
        ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::ISOWeekNumbers);
    }
    else{
        ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    }
}


/*
    Function changes language for the calendar widget
*/
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


/*
    Function is called when user make double click on a date in the calendar widget
*/
void BVB_Manager::dateActivated(){

    // double click / enter /return on calendar widget's date
    auto selected_date = ui->calendarWidget->selectedDate();

    // make a query to getting all trainings on this date
    QSqlQuery query(database_manager.getDatabase());

    query.prepare("SELECT training_date, description"
                  " FROM Trainings"
                  " WHERE DATE(training_date) = DATE(:date);");

    query.bindValue(":date", selected_date);

    if(!query.exec()){
        QMessageBox::warning(this, "Database error", "Couldn't load trainings");
        return;
    }
    else{

        if(!query.next()){
            QMessageBox::information(this,
                                     "Trainig report",
                                     "No training on this day");
            return;
        }
        else{

            query.previous();  // back to the first database record

            while(query.next()){
                QMessageBox::information(this,
                                         query.value(0).toString(),
                                         query.value(1).toString());
            }
        }
    }
}


/*
    Function calculates training amounts for a particular date
*/
int BVB_Manager::getTrainings(const QDate &date) {

    int counter{0};

    // make a query
    QSqlQuery query(database_manager.getDatabase());

    query.prepare("SELECT COUNT(id)"
                  " FROM Trainings"
                  " WHERE DATE(training_date) = DATE(:date);");

    query.bindValue(":date", date);

    if(!query.exec()){
        QMessageBox::warning(this, "Trainings amount report",
                             "Couldn't calculate trainings on this day");
    }
    else{
        if(query.next()){
            return query.value(0).toInt();
        }
    }

    return counter;
}


/*
    Function uses logic above
*/
void BVB_Manager::dateClicked(){

    ui->lcdNumber->display(getTrainings(ui->calendarWidget->selectedDate()));
}


/*
    Function calls the TeamRegistration widget
*/
void BVB_Manager::on_actionRegister_a_team_triggered()
{
    // team registration
    team_registration = std::make_unique<TeamRegistration>(database_manager.getDatabase(),
                                                           this);

    team_registration->setWindowTitle("Team registration");
    team_registration->show();
}


/*
    Function calls the AddTournament widget
*/
void BVB_Manager::on_actionAdd_a_new_tournament_triggered()
{
    // add a new tournament to the database
    add_tournament = std::make_unique<AddTournament>(&database_manager.getDatabase(),
                                                     this);

    add_tournament->setWindowTitle("Add a new tournament");
    add_tournament->show();
}


/*
    Function calls the TournamentCreator widget
*/
void BVB_Manager::on_actionCreate_a_tournament_triggered()
{
    // create a tournament
    tournament_creator = std::make_unique<TournamentCreator>(&database_manager.getDatabase(),
                                                         this);
    tournament_creator->setWindowTitle("Create a new tournament");
    tournament_creator->show();
}
