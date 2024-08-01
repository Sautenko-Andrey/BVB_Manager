#include "searchplayer.h"
#include "ui_searchplayer.h"
#include <QDebug>
#include <QSqlQuery>
#include <QStringList>
#include <QMessageBox>

SearchPlayer::SearchPlayer(QSqlDatabase &database, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SearchPlayer)
{
    ui->setupUi(this);

    // pointer on database
    db = &database;
}

SearchPlayer::~SearchPlayer()
{
    delete ui;
}

void SearchPlayer::on_searchButton_clicked()
{
    // start searching a player when user pushes this button

    // read data from edit line
    QString user_input = ui->lineEdit->text();

    // removing all necessary whitespaces
    user_input = user_input.trimmed();

    QStringList splitted_name = user_input.split(u' ');

    if(splitted_name.size() != 2){
        QMessageBox::warning(this, "Player's name error",
                    "Please input first name and last name separated by one space");
        return;
    }
    else{
        // // make a query
        // QSqlQuery query(*db);

        // query.prepare("SELECT"
        //               " first_name, last_name, age, sex, height, hometown, phone, picture"
        //               " FROM Players"
        //               " WHERE first_name = :f_name AND last_name = :l_name;");

        // query.bindValue(":f_name", splitted_name[0]);  // first name
        // query.bindValue(":l_name", splitted_name[1]);  // last name

        // if(!query.exec()){
        //     QMessageBox::warning(this, "Database error",
        //                          "Couldn't load player's data from the database");
        //     return;
        // }
        // else{

        //     while(query.next()){
        //         qDebug() << "Name: " << query.value(0).toString();
        //         qDebug() << "Surname: " << query.value(1).toString();
        //         qDebug() << "Age: " << query.value(2).toInt();
        //         qDebug() << "Sex: " << query.value(3).toInt();
        //         qDebug() << "Height: " << query.value(4).toInt();
        //         qDebug() << "Hometown: " << query.value(5).toString();
        //         qDebug() << "Phone: " << query.value(6).toString();
        //         qDebug() << "Image: " << query.value(7).toString();
        //         qDebug() << "------------------------------------------------------";


        //     }
        // }

        player_window = std::make_unique<AddPlayer>(*db, this, false,
                                                    splitted_name[0],
                                                    splitted_name[1]);
        player_window->setWindowTitle(user_input);
        player_window->show();

        // clear edit line and set focus on it
        clearLineAndSetFocus();
    }
}

void SearchPlayer::clearLineAndSetFocus(){
    // clears line edit and sets focus on it
    ui->lineEdit->clear();
    ui->lineEdit->setFocus();
}


void SearchPlayer::on_pushButton_clicked()
{
    // clear edit line and set focus on it
    clearLineAndSetFocus();
}

