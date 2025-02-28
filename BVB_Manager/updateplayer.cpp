#include "updateplayer.h"
#include "ui_updateplayer.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>

UpdatePlayer::UpdatePlayer(QSqlDatabase &database, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UpdatePlayer)
{
    ui->setupUi(this);

    // pointer on database
    db = &database;

    // reserve memory for all_players container
    constexpr int possible_players_amount{50};
    all_players.reserve(possible_players_amount);

    // filling a table
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setColumnCount(header_labels.size());
    ui->tableWidget->setHorizontalHeaderLabels(header_labels);

    // make  aqua color and bold text of horizontal headers
    changeTableItemStyle(ui->tableWidget, QColor("aqua"), true);

    // getting players data from the database
    QSqlQuery query(database);

    if(!query.exec("SELECT"
                    " id, first_name, last_name, age, sex, height, hometown, phone, picture"
                    " FROM Players"
                    " ORDER BY first_name ASC;")){
        QMessageBox::warning(this, "Database error",
                             "Couldn't load players data from the database");
        return;
    }
    else{
        while(query.next()){

            QString gender = "male";
            query.value(4).toInt() ? gender : gender = "female";

            all_players.emplaceBack(Player2(query.value(0).toInt(), query.value(1).toString(),
                                            query.value(2).toString(),
                                            query.value(3).toInt(), gender,
                                            query.value(5).toInt(), query.value(6).toString(),
                                            query.value(7).toString(), query.value(8).toString()));
        }
    }

    // set row amount
    ui->tableWidget->setRowCount(all_players.size());

    int i = 0;

    for(auto const &player : all_players){
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(player.id)));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(player.first_name));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(player.last_name));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(player.age)));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(player.gender));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(player.height)));
        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(player.hometown));
        ui->tableWidget->setItem(i, 7, new QTableWidgetItem(player.phone));
        ui->tableWidget->setItem(i, 8, new QTableWidgetItem(player.img));

        ++i;
    }

    // hide column with player's ID
    ui->tableWidget->setColumnHidden(0, true);

    // signals & slots
    connect(ui->tableWidget, SIGNAL(cellChanged(int,int)),
            this, SLOT(changeCell()));

    connect(ui->tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
            this, SLOT(saveID()));

    connect(ui->tableWidget, SIGNAL(cellDoubleClicked(int,int)),
            this, SLOT(picClicked()));
}

UpdatePlayer::~UpdatePlayer()
{
    delete ui;
}


/*
    Function triggers when a user double clicks on an image column
*/
void UpdatePlayer::picClicked(){

    if(ui->tableWidget->currentColumn() == static_cast<int>(Headers::Image)){

        const QString player_image_path = QFileDialog::getOpenFileName(this,
                                                           "Select an image");
        if(!image.load(player_image_path)){
            QMessageBox::warning(this, "File error!", "Couldn't load an image!");
        }
        else{

            // save a new image in the players images folder
            // make image's name
            QString name;
            QSqlQuery player_name_query(*db);

            player_name_query.prepare("SELECT first_name, last_name"
                                      " FROM Players"
                                      " WHERE id = :p_id;");

            player_name_query.bindValue(":p_id", player_id);

            if(!player_name_query.exec()){
                QMessageBox::warning(this, "Database error",
                                     "Couldn't load a player's name from the database");
                return;
            }
            else{
                while(player_name_query.next()){
                    name = player_name_query.value(0).toString()
                           + "_"
                           + player_name_query.value(1).toString();
                }

                //removing all possible whitespaces
                name = name.trimmed().replace(" ", "_");

                const QString &image_name = QDir::homePath() +
                                           "/Players_images/" +
                                           std::move(name) + ".jpeg";

                if(!image.save(image_name)){
                    QMessageBox::warning(this, "Saving image error!", "Couldn't save an image!");
                    return;
                }
                else{
                    // save the new image path in the database
                    QSqlQuery img_path_query(*db);

                    img_path_query.prepare("UPDATE Players"
                                  " SET picture = :img_path WHERE id = :p_id;");

                    img_path_query.bindValue(":img_path", image_name);
                    img_path_query.bindValue(":p_id", player_id);

                    if(!img_path_query.exec()){
                        QMessageBox::warning(this, "Database error",
                                        "Couldn't save a new player's image file path");
                    }
                    else{
                        QMessageBox::information(this, "Image saving report",
                                            "A new image has been saved in database");
                    }
                }
            }
        }
    }
}


/*
    Functions saves id in players_id variable for the further purposes
*/
void UpdatePlayer::saveID(){
    player_id = ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->text();
}



/*
    Function allows user to change ant cell in the players table
    and saves changed data in the database.
*/
void UpdatePlayer::changeCell(){

    // getting a new data
    const auto data = ui->tableWidget->item(ui->tableWidget->currentRow(),
                       ui->tableWidget->currentColumn())->text();

    // getting a column number
    auto column = ui->tableWidget->currentColumn();

    // make a query
    QSqlQuery query(*db);

    switch (column) {
    case static_cast<int>(Headers::FirstName):
        query.prepare("UPDATE Players SET first_name = :new_data WHERE id = :p_id;");
        query.bindValue(":new_data", data);
        query.bindValue(":p_id", player_id);
        break;

    case static_cast<int>(Headers::LastName):
        query.prepare("UPDATE Players SET last_name = :new_data WHERE id = :p_id;");
        query.bindValue(":new_data", data);
        query.bindValue(":p_id", player_id);
        break;

    case static_cast<int>(Headers::Age):
        query.prepare("UPDATE Players SET age = :new_data WHERE id = :p_id;");
        query.bindValue(":new_data", data);
        query.bindValue(":p_id", player_id);
        break;

    case static_cast<int>(Headers::Gender):

        int gender;

        if(data == "female"){
            gender = 0;
        }
        else if(data == "male"){
            gender = 1;
        }
        else{
            gender = 1;
        }

        query.prepare("UPDATE Players SET sex = :new_data WHERE id = :p_id;");
        query.bindValue(":new_data", gender);
        query.bindValue(":p_id", player_id);
        break;

    case static_cast<int>(Headers::Height):
        query.prepare("UPDATE Players SET height = :new_data WHERE id = :p_id;");
        query.bindValue(":new_data", data);
        query.bindValue(":p_id", player_id);
        break;

    case static_cast<int>(Headers::Hometown):
        query.prepare("UPDATE Players SET hometown = :new_data WHERE id = :p_id;");
        query.bindValue(":new_data", data);
        query.bindValue(":p_id", player_id);
        break;

    case static_cast<int>(Headers::Phone):
        query.prepare("UPDATE Players SET phone = :new_data WHERE id = :p_id;");
        query.bindValue(":new_data", data);
        query.bindValue(":p_id", player_id);
        break;

    case static_cast<int>(Headers::Image):
        query.prepare("UPDATE Players SET picture = :new_data WHERE id = :p_id;");
        query.bindValue(":new_data", data);
        query.bindValue(":p_id", player_id);
        break;
    }

    if(!query.exec()){
        QMessageBox::warning(this, "Database error", "Couldn't update player's data");
        return;
    }
    else{
        QMessageBox::information(this, "Update report", "Player has been updated");
    }
}
