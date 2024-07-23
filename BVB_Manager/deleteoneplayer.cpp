#include "deleteoneplayer.h"
#include "ui_deleteoneplayer.h"
#include <QStandardItem>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QMap>

DeleteOnePlayer::DeleteOnePlayer(QSqlDatabase &database, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DeleteOnePlayer)
{
    ui->setupUi(this);

    db = &database;

    // adding header to a table
    model = new QStandardItemModel();

    // allow table header occupy full line
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    model->setHorizontalHeaderItem(0, new QStandardItem(QString("ID")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("First name")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Last name")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("Age")));
    model->setHorizontalHeaderItem(4, new QStandardItem(QString("Gender")));
    model->setHorizontalHeaderItem(5, new QStandardItem(QString("Height")));
    model->setHorizontalHeaderItem(6, new QStandardItem(QString("Hometown")));
    model->setHorizontalHeaderItem(7, new QStandardItem(QString("Phone")));

    ui->tableView->setModel(model);

    // adding players data to the table
    QSqlQuery query(*db);

    if(!query.exec("SELECT id, first_name, last_name, age, sex, height, hometown, phone"
                    " FROM Players;")){
        QMessageBox::warning(this, "Database error",
                             "Couldn't load players data. Try one more time.");
        return;
    }

    // filling the container
    while(query.next()){
        Player player(query.value(0).toInt(), query.value(1).toString(),
                      query.value(2).toString(),query.value(3).toInt(),
                      query.value(4).toInt(),query.value(5).toInt(),
                      query.value(6).toString(),query.value(7).toString());


        all_players.push_back(player);
    }

    // filling the table

    // counter
    int i = 0;

    for(auto const &player : all_players){
        auto id = new QStandardItem(QString::number(player.id));
        auto name = new QStandardItem(player.first_name);
        auto surname = new QStandardItem(player.last_name);
        auto age = new QStandardItem(QString::number(player.age));
        auto gender = new QStandardItem( player.gender ? "male" : "female" );
        auto height = new QStandardItem(QString::number(player.height));
        auto hometown = new QStandardItem(player.hometown);
        auto phone = new QStandardItem(player.phone);

        model->setItem(i, 0, id);
        model->setItem(i, 1, name);
        model->setItem(i, 2, surname);
        model->setItem(i, 3, age);
        model->setItem(i, 4, gender);
        model->setItem(i, 5, height);
        model->setItem(i, 6, hometown);
        model->setItem(i, 7, phone);

        ++i;
    }
}


DeleteOnePlayer::~DeleteOnePlayer()
{
    delete ui;
}


void DeleteOnePlayer::on_markButton_clicked()
{
    auto marked_row = ui->tableView->currentIndex().row();

    // let's take a look at current index
    int db_index = ui->tableView->currentIndex().data().toInt();

    // save db_index in the container
    marked_indexes.push_back(db_index);

    // Make marked row red
    constexpr int columns_amount = 8;

    for(int i{0}; i < columns_amount; ++i){
        model->setData(model->index(marked_row, i), QColor(Qt::red), Qt::BackgroundRole);
    }

    marked_rows.push_back(marked_row);

    // saving marked names for the next purposes
    QSqlQuery save_query(*db);

    save_query.prepare("SELECT first_name, last_name FROM Players WHERE id = :id_for_save;");

    save_query.bindValue(":id_for_save", db_index);

    if(!save_query.exec()){
        QMessageBox::warning(this, "Error", "Couldn't get deleted player/s name/s");
        return;
    }
    else{
        while(save_query.next()){
            QString name = save_query.value(0).toString()
                           + "_" + save_query.value(1).toString() + ".jpeg";

            removed_names.push_back(name);
        }
    }
}


void DeleteOnePlayer::on_deleteButton_clicked()
{
    // sort marked rows
    std::sort(marked_rows.begin(), marked_rows.end());

    // container where we save deleted players names

    // delete rows from the tail
    for(auto it = marked_rows.rbegin(); it != marked_rows.rend(); ++it){

        // deleting a row from view
        model->removeRow(*it);

        // deleting data from database either
        for(auto const &index : marked_indexes){
            QSqlQuery query(*db);
            query.prepare("DELETE FROM Players WHERE id = :id_for_del;");
            query.bindValue(":id_for_del", index);
            if(!query.exec()){
                QMessageBox::warning(this, "Database problem", "Couldn't delete a player.");
                return;
            }
        }
    }

    // delete image/images from the directory
    QDir dir(QDir::homePath() + "/Players_images");

    if(!dir.exists()){

        // if folder doesn't exist we create it and warn a user
        dir.mkpath(QDir::homePath() + "/Players_images");
        QMessageBox::warning(this, "Attention!",
        "Folder Players_images was absent. App has made it one more time.");
    }
    else{

        // deleting marked pics from the pics folder
        for(auto const &name : removed_names){
            dir.remove(name);
        }
    }

    QMessageBox::information(this,"Deleting report",
    "Player/players has/have been succesfully deleted from the database.");
}
