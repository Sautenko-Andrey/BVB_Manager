#include "deleteoneplayer.h"
#include "ui_deleteoneplayer.h"
#include <QStandardItem>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QDir>

DeleteOnePlayer::DeleteOnePlayer(QSqlDatabase &database, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DeleteOnePlayer)
{
    ui->setupUi(this);

    db = &database;

    // creating and filling a table of players
    //createPlayersTable(model, ui->tableView, db, all_players, this);

    // adding header to a table
    model = new QStandardItemModel();

    // allow table header occupy full line
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Make headers for the table
    // const QStringList headers = {
    //     "ID", "First name", "Last name", "Age", "Gender", "Height", "Hometown", "Phone"
    // };

    for(int i{0}; i < headers.size(); ++i){
        auto item = new QStandardItem(headers[i]);
        item->setForeground(QBrush(QColor("aqua")));

        // Get the current font
        QFont font = item->font();
        font.setBold(true);
        item->setFont(font);

        // add to the model
        model->setHorizontalHeaderItem(i, item);
    }

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
    int i{0};

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

    // delete mode settings
    ui->deleteModeComboBox->addItems({"delete one", "delete many"});

    // make del button unavailable untill row was marked
    ui->deleteButton->setDisabled(true);

    connect(ui->deleteModeComboBox, SIGNAL(currentTextChanged(QString)),
            this, SLOT(deleteModeChanged()));
}


DeleteOnePlayer::~DeleteOnePlayer()
{
    delete ui;
}

void DeleteOnePlayer::deleteModeChanged(){
    if(ui->deleteModeComboBox->currentText() == "delete one"){
        mode = true;
    }
    else{
        mode = false;
    }
}


void DeleteOnePlayer::on_markButton_clicked()
{
    // make del button available again
    ui->deleteButton->setDisabled(false);

    if(mode){  // deleting one by one mode

        ui->markButton->setDisabled(true);

        ui->deleteModeComboBox->setDisabled(true);

        row_for_del = ui->tableView->currentIndex().row();

        // let's take a look at current index
        db_index_for_del = ui->tableView->currentIndex().data().toInt();

        // Make marked row red
        constexpr int columns_amount = 8;
        for(int i{0}; i < columns_amount; ++i){
            model->setData(model->index(row_for_del, i),
                           QColor(Qt::red), Qt::BackgroundRole);
        }

        // saving marked name for the next purposes
        QSqlQuery save_query(*db);

        save_query.prepare("SELECT first_name, "
                           "last_name FROM Players WHERE id = :id_for_save;");

        save_query.bindValue(":id_for_save", db_index_for_del);

        if(!save_query.exec()){
            QMessageBox::warning(this, "Error", "Couldn't get deleted player's name");
            return;
        }
        else{
            while(save_query.next()){
                name_for_del = save_query.value(0).toString()
                               + "_" + save_query.value(1).toString() + ".jpeg";
            }
        }

    }
    else{    // delete many mode
        auto marked_row = ui->tableView->currentIndex().row();

        // let's take a look at current index
        int db_index = ui->tableView->currentIndex().data().toInt();

        // save db_index in the container
        marked_indexes.push_back(db_index);

        // Make marked row red
        constexpr int columns_amount = 8;

        for(int i{0}; i < columns_amount; ++i){
            model->setData(model->index(marked_row, i),
                           QColor(Qt::red), Qt::BackgroundRole);
        }
        marked_rows.push_back(marked_row);

        // saving marked names for the next purposes
        QSqlQuery save_query(*db);

        save_query.prepare("SELECT first_name, "
                           "last_name FROM Players WHERE id = :id_for_save;");

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
}


void DeleteOnePlayer::on_deleteButton_clicked()
{
    // make mark button available again
    ui->markButton->setDisabled(false);

    // make del mode button available again
    ui->deleteModeComboBox->setDisabled(false);

    if(mode){   // delete one by one

        // delete the row from the table
        model->removeRow(row_for_del);

        //delete data from the database
        QSqlQuery query(*db);

        query.prepare("DELETE FROM Players WHERE id = :id_for_del;");

        query.bindValue(":id_for_del", db_index_for_del);

        if(!query.exec()){
            QMessageBox::warning(this, "Database problem", "Couldn't delete a player.");
            return;
        }

        // delete an image from the directory
        QDir dir(QDir::homePath() + "/Players_images");

        if(!dir.exists()){

            // if folder doesn't exist we create it and warn a user
            dir.mkpath(QDir::homePath() + "/Players_images");
            QMessageBox::warning(this, "Attention!",
            "Folder Players_images was absent. App has made it one more time.");
        }
        else{
            // deleting marked pic from the pics folder
            dir.remove(name_for_del);
        }

        QMessageBox::information(this,"Deleting report",
            "Player has been succesfully deleted from the database.");

    }
    else{  // delete many
        // sort marked rows
        std::sort(marked_rows.begin(), marked_rows.end());

        // delete rows from the tail
        for(auto it = marked_rows.rbegin(); it != marked_rows.rend(); ++it){
            model->removeRow(*it);
        }

        // delete data from database either
        for(auto const &index : marked_indexes){
            QSqlQuery query(*db);
            query.prepare("DELETE FROM Players WHERE id = :id_for_del;");
            query.bindValue(":id_for_del", index);
            if(!query.exec()){
                QMessageBox::warning(this, "Database problem",
                                     "Couldn't delete a player.");
                return;
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
        "Players have been succesfully deleted from the database.");
    }
}

void DeleteOnePlayer::on_helpButton_clicked()
{
    // help info
    QMessageBox::information(this, "Hint",
                "For marking a desired player/row please click on the ID cell "
                "and push button 'Mark ID', then push 'Delete'.");
}

