#include "deleteoneplayer.h"
#include "ui_deleteoneplayer.h"
#include <QStandardItem>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>

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

    model->setHorizontalHeaderItem(0, new QStandardItem(QString("First name")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Last name")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Age")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("Gender")));
    model->setHorizontalHeaderItem(4, new QStandardItem(QString("Height")));
    model->setHorizontalHeaderItem(5, new QStandardItem(QString("Hometown")));
    model->setHorizontalHeaderItem(6, new QStandardItem(QString("Phone")));

    ui->tableView->setModel(model);

    // adding players data to the table
    QSqlQuery query(*db);

    if(!query.exec("SELECT first_name, last_name, age, sex, height, hometown, phone"
                    " FROM Players;")){
        QMessageBox::warning(this, "Database error",
                             "Couldn't load players data. Try one more time.");
        return;
    }

    // filling the container
    while(query.next()){
        Player player(query.value(0).toString(), query.value(1).toString(),
                      query.value(2).toInt(), query.value(3).toInt(),
                      query.value(4).toInt(), query.value(5).toString(),
                      query.value(6).toString());

        all_players.push_back(player);
    }

    // filling the table

    // counter
    int i = 0;

    for(auto const &player : all_players){

        auto name = new QStandardItem(player.first_name);
        auto surname = new QStandardItem(player.last_name);
        auto age = new QStandardItem(QString::number(player.age));
        auto gender = new QStandardItem( player.gender ? "male" : "female" );
        auto height = new QStandardItem(QString::number(player.height));
        auto hometown = new QStandardItem(player.hometown);
        auto phone = new QStandardItem(player.phone);

        model->setItem(i, 0, name);
        model->setItem(i, 1, surname);
        model->setItem(i, 2, age);
        model->setItem(i, 3, gender);
        model->setItem(i, 4, height);
        model->setItem(i, 5, hometown);
        model->setItem(i, 6, phone);

        ++i;
    }

}

DeleteOnePlayer::~DeleteOnePlayer()
{
    delete ui;
}

void DeleteOnePlayer::on_markButton_clicked()
{

}


void DeleteOnePlayer::on_deleteButton_clicked()
{

}

