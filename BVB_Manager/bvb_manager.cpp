#include "bvb_manager.h"
#include "./ui_bvb_manager.h"

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

