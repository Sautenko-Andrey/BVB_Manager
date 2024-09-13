#include "utils.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QHeaderView>


const QString BACKGROUND_COLOR = "background-color:#09beb8;";

const QStringList training_types{
    "Warm up", "Gym", "Cardio", "Tactical-technical"
};


void createPlayersTable(QStandardItemModel *model, QTableView *tableView,
                        QSqlDatabase *db, QVector<Player> &all_players,
                        QDialog *dialog)
{
    // adding header to a table
    model = new QStandardItemModel();

    // allow table header occupy full line
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QStringList labels{
        "ID", "First name", "Last name", "Age", "Gender", "Height", "Hometown", "Phone"
    };

    // add header labels for the table
    model->setHorizontalHeaderLabels(labels);
    tableView->setModel(model);

    // adding players data to the table
    QSqlQuery query(*db);

    if(!query.exec("SELECT id, first_name, last_name, age, sex, height, hometown, phone"
                    " FROM Players;")){
        QMessageBox::warning(dialog, "Database error",
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

void fontAdapter(QPushButton *btn){

    constexpr int max_symbols_amount = 24;

    if(btn->text().size() >= max_symbols_amount){
        btn->setFont(QFont("Ubuntu", 6));
    }
    else if(btn->text().size() > max_symbols_amount / 2 + 1 &&
             btn->text().size() < max_symbols_amount){
        btn->setFont(QFont("Ubuntu", 9));
    }
}
