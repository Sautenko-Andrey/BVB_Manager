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
    ui->trainingTime->setTime(QTime(12, 0));
}

BVB_Manager::~BVB_Manager()
{
    delete ui;
}

void BVB_Manager::on_actionAdd_a_new_player_triggered()
{
    // add a new player
    addPlayer = std::make_unique<AddPlayer>(dataBaseManager.getDatabase(), this);
    addPlayer->setWindowTitle("New player");
    addPlayer->show();
}


void BVB_Manager::on_actionDelete_a_player_triggered()
{
    // delete a desired player
    deletePlayer = std::make_unique<DeleteOnePlayer>(dataBaseManager.getDatabase(), this);
    deletePlayer->setWindowTitle("Delete a player");
    deletePlayer->show();
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
        QSqlQuery query(dataBaseManager.getDatabase());
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
