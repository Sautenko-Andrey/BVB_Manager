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


    // signals & slots
    connect(ui->calendarWidget, SIGNAL(clicked(QDate)),
            this, SLOT(selectedDateChanged()));

    connect(ui->trainingTime, SIGNAL(timeChanged(QTime)),
            this, SLOT(selectedTimeChanged()));

    connect(ui->playersListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(selectedPlayer()));

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

// when double clicked on the player in players list widget
void BVB_Manager::selectedPlayer(){

    const QString current_player = ui->playersListWidget->currentItem()->text();

    // unmark player
    if(marked_players.contains(current_player)){
        // when double clicked change row text color to black (unmark a player)
        markItem(ui->playersListWidget->currentItem(),
                 Qt::black, QFont("Helvetica [Cronyx]", 11));

        // remove the player from the marked container
        marked_players.remove(current_player);
    }
    else{
        // when double clicked change row text color to red (mark a player)
        markItem(ui->playersListWidget->currentItem(),
                 Qt::red, QFont("Helvetica [Cronyx]", 13));

        // add marked player to the marked_players container
        marked_players.insert(current_player);
    }

    // clear players string
    combined_players.clear();

    // add players to the label
    for(const auto &player : marked_players){
        combined_players += (player + "\n");
    }

    // update label
    ui->playerNameLabel->setText(combined_players);
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

