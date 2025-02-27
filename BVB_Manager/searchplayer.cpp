#include "searchplayer.h"
#include "ui_searchplayer.h"
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

    if(constexpr int num{2}; splitted_name.size() != num)
    {
        QMessageBox::warning(this, "Player's name error",
                    "Please input first name and last name separated by one space");
        return;
    }
    else{
        player_window = std::make_unique<AddPlayer>(*db, this, false,
                                                    splitted_name[0],
                                                    splitted_name[1]);
        player_window->setWindowTitle(std::move(user_input));
        player_window->show();

        // clear edit line and set focus on it
        clearLineAndSetFocus();
    }
}


/*
    Function clears line edit and return focus on it again
*/
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
