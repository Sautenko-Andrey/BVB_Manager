#include "changeplayer.h"
#include "ui_changeplayer.h"
#include <QStandardItem>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <algorithm>


ChangePlayer::ChangePlayer(QSqlDatabase &database, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChangePlayer)
{
    ui->setupUi(this);

    // pointer on database
    db = &database;

    // // create a player's data window
    // player_window = std::make_unique<AddPlayer>(database, this);

    ui->listWidget->setMinimumSize(QSize(static_cast<int>(Sizes::ChangePlayerWindowWidth),
                                         static_cast<int>(Sizes::ChangePlayerWindowHeight)));

    ui->listWidget->setStyleSheet(
        "QListWidget::item{border-bottom: 2px solid green;}"
    );

    //--------------------------------------------------------------------------
    // // creating and filling a table of players
    // createPlayersTable(model, ui->tableView, db, all_players, this);

    // connect(ui->tableView,
    //         SIGNAL(cellChanged(int,int)),
    //         this, SLOT(changeData()));     // bag! need a proper signal!
    //--------------------------------------------------------------------------

    // make a query
    QSqlQuery query(database);

    if(!query.exec("SELECT "
                   "id, first_name, last_name, age, sex, height, hometown, phone "
                   "FROM Players ORDER BY first_name ASC, last_name ASC;")){

        QMessageBox::warning(this, "Error", "Couldn't load players data from database."
                                            "Reload the app.");
        return;
    }
    else{
        while(query.next()){

            QString gender = "male";
            query.value(4).toInt() ? gender : gender = "female";

            ui->listWidget->addItem(
                "\nID: " + query.value(0).toString() + ", " +  // ID
                query.value(1).toString() + " " +             // first name
                query.value(2).toString() + " , " +           // last name
                query.value(3).toString() + " y.o, " +        // age
                gender + " , " +                              // gender
                query.value(5).toString() + " cm, " +         // height
                query.value(6).toString() + " , tel: " +      // hometown
                query.value(7).toString()                     // phone
            );
        }
    }


    // signals & slots
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(changeData()));


}

ChangePlayer::~ChangePlayer()
{
    delete ui;
}

void ChangePlayer::changeData()
{
    // when double clicked change row text color to red
    ui->listWidget->currentItem()->setForeground(Qt::red);

    // getting player's id
    QString current_player = ui->listWidget->currentItem()->text();
    auto first_coma_index = current_player.indexOf(",");

    QString pure_index = current_player.left(first_coma_index);
    auto first_space_index = pure_index.indexOf(" ");
    QString index = pure_index.mid(first_space_index + 1, pure_index.size());

    // then we call another wdget same as add player widget
    // with current data in the all fields
    // create a player's data window
    player_window = std::make_unique<AddPlayer>(*db, this, false, index);
    player_window->setWindowTitle("Change player's data");
    player_window->show();
}
