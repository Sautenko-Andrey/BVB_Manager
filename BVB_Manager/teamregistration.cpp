#include "teamregistration.h"
#include "ui_teamregistration.h"
#include <QMessageBox>
#include <QSqlQuery>


enum class RegValues {
    MaxRank = 100,
    MinHeight = 150,
    MaxHeight = 230,
    DefaultHeight = 190,
    MinAge = 8,
    MaxAge = 80,
    DefaultAge = 18
};

TeamRegistration::TeamRegistration(QSqlDatabase &database, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TeamRegistration)
{
    ui->setupUi(this);

    // pointer on database
    db = &database;

    // view settings

    // rank is in range (0 - 100)
    ui->teamRankSpinBox->setMaximum(static_cast<int>(RegValues::MaxRank));

    // player's height is in range (150 - 230 cm)
    ui->firstPlayerHeightSpinBox->setMinimum(static_cast<int>(RegValues::MinHeight));
    ui->firstPlayerHeightSpinBox->setMaximum(static_cast<int>(RegValues::MaxHeight));
    ui->firstPlayerHeightSpinBox->setValue(static_cast<int>(RegValues::DefaultHeight));
    ui->secPlayerHeightSpinBox->setMinimum(static_cast<int>(RegValues::MinHeight));
    ui->secPlayerHeightSpinBox->setMaximum(static_cast<int>(RegValues::MaxHeight));
    ui->secPlayerHeightSpinBox->setValue(static_cast<int>(RegValues::DefaultHeight));
    ui->firstPlayerAgeSpinBox->setMinimum(static_cast<int>(RegValues::MinAge));
    ui->firstPlayerAgeSpinBox->setMaximum(static_cast<int>(RegValues::MaxAge));
    ui->firstPlayerAgeSpinBox->setValue(static_cast<int>(RegValues::DefaultAge));
    ui->secPlayerAgeSpinBox->setMinimum(static_cast<int>(RegValues::MinAge));
    ui->secPlayerAgeSpinBox->setMaximum(static_cast<int>(RegValues::MaxAge));
    ui->secPlayerAgeSpinBox->setValue(static_cast<int>(RegValues::DefaultAge));
}

TeamRegistration::~TeamRegistration()
{
    delete ui;
}


/*
    Function removes away all data from the widget
*/
void TeamRegistration::dropAllData(){
    ui->teamNameLine->clear();
    ui->teamRankSpinBox->setValue(0);

    dropPlayerData(ui->firstPlayerNameLine,
                   ui->firstPlayerCityLine,
                   ui->firstPlayerHeightSpinBox,
                   ui->firstPlayerAgeSpinBox,
                   false);

    dropPlayerData(ui->secPlayerNameLine,
                   ui->secPlayerCityLine,
                   ui->secPlayerHeightSpinBox,
                   ui->secPlayerAgeSpinBox,
                   false);

    ui->teamNameLine->setFocus();
}


/*
    Function removes away only player's data
*/
void TeamRegistration::dropPlayerData(QLineEdit *player_name,
                    QLineEdit *player_city,
                    QSpinBox *player_height,
                    QSpinBox *player_age,
                    bool focus){
    player_name->clear();
    player_city->clear();
    player_height->setValue(static_cast<int>(RegValues::DefaultHeight));
    player_age->setValue(static_cast<int>(RegValues::DefaultAge));

    if(focus){
        player_name->setFocus();
    }
}


/*
    Function removes all player's data
    and asks user for confirmation.
*/
void TeamRegistration::on_clearAllButton_clicked()
{
    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, "Dropping all team indo",
                                  "Drop all team's data?",
                                  QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes){

        // make the editor blank
        dropAllData();
    }
}


/*
    Function clears only #1 player data
*/
void TeamRegistration::on_clearPlayer_1Button_clicked()
{
    // clear player #1 info
    dropPlayerData(ui->firstPlayerNameLine,
                   ui->firstPlayerCityLine,
                   ui->firstPlayerHeightSpinBox,
                   ui->firstPlayerAgeSpinBox);
}


/*
    Function clears only #2 player data
*/
void TeamRegistration::on_clearPlayer_2Button_clicked()
{
    // clear player #2 info
    dropPlayerData(ui->secPlayerNameLine,
                   ui->secPlayerCityLine,
                   ui->secPlayerHeightSpinBox,
                   ui->secPlayerAgeSpinBox);
}


/*
    Function registers a team and save it into the database,
    moreover it clears widget and get it ready for the next
    registration.
*/
void TeamRegistration::on_registerButton_clicked()
{
    QSqlQuery reg_query(*db);

    reg_query.prepare("INSERT INTO Teams"
                      " (team_name, rank, first_player_name, second_player_name,"
                      " first_player_height, second_player_height, first_player_age,"
                      " second_player_age, first_player_city, second_player_city)"
                      " VALUES"
                      " (:t_name, :t_rank, :fp_name, :sp_name, :fp_height, :sp_height,"
                      " :fp_age, :sp_age, :fp_city, :sp_city);");

    reg_query.bindValue(":t_name", ui->teamNameLine->text());
    reg_query.bindValue(":t_rank", ui->teamRankSpinBox->value());
    reg_query.bindValue(":fp_name", ui->firstPlayerNameLine->text());
    reg_query.bindValue(":sp_name", ui->secPlayerNameLine->text().trimmed());
    reg_query.bindValue(":fp_height", ui->firstPlayerHeightSpinBox->value());
    reg_query.bindValue(":sp_height", ui->secPlayerHeightSpinBox->value());
    reg_query.bindValue(":fp_age", ui->firstPlayerAgeSpinBox->value());
    reg_query.bindValue(":sp_age", ui->secPlayerAgeSpinBox->value());
    reg_query.bindValue(":fp_city", ui->firstPlayerCityLine->text());
    reg_query.bindValue(":sp_city", ui->secPlayerCityLine->text());

    if(!reg_query.exec()){
        QMessageBox::warning(this, "Databse error", "Couldn't register the team");
        return;
    }
    else{
        QMessageBox::information(this, "Team registration report",
                                 "Team has been successfully registered");
    }

    // clear the editor
    dropAllData();
}

