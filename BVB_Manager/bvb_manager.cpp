#include "bvb_manager.h"
#include "./ui_bvb_manager.h"
#include <QMessageBox>
#include <QSqlQuery>

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
    // get a new date and update training date label
    updateDate();

    int hour = ui->trainingTime->time().hour();
    int minute = ui->trainingTime->time().minute();

    ui->timeSelectedLabel->setText("Time: " +
                                   QString::number(hour) +
                                   ":" +
                                   QString::number(minute));


    // signals & slots
    connect(ui->calendarWidget, SIGNAL(clicked(QDate)),
            this, SLOT(selectedDateChanged()));

    connect(ui->trainingTime, SIGNAL(timeChanged(QTime)),
            this, SLOT(selectedTimeChanged()));
}

BVB_Manager::~BVB_Manager()
{
    delete ui;
}

void BVB_Manager::updateDate(){
    selected_date = ui->calendarWidget->selectedDate();
    const QString training_date = "Date: " + selected_date.toString("dd.MM.yyyy");
    ui->dateSelectedLabel->setText(training_date);
}

void BVB_Manager::selectedDateChanged(){
    // get a new date and update training date label
    updateDate();
}

void BVB_Manager::selectedTimeChanged(){
    int hour = ui->trainingTime->time().hour();
    int minute = ui->trainingTime->time().minute();

    ui->timeSelectedLabel->setText("Time: " +
                                   QString::number(hour) +
                                   ":" +
                                   QString::number(minute));
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

