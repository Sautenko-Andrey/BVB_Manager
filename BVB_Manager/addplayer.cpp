#include "addplayer.h"
#include "ui_addplayer.h"
#include <QDir>
#include <QMessageBox>
#include <QSqlQuery>
#include <QFileDialog>


enum class DefaultWidgetValues {
    DefaultAge = 18,
    MinimumAge = 8,
    MaximumAge = 80,
    Male = 1,
    Female = 0,
    DefaultHeight = 190,
    MinimumHeight = 150,
    MaximumHeight = 235
};

AddPlayer::AddPlayer(QSqlDatabase &database,
                     QWidget *parent,
                     bool add_mode,
                     const QString &player_first_name,
                     const QString &player_last_name,
                     QListWidget *players_list_widget)
    : QDialog(parent)
    , ui(new Ui::AddPlayer)
{
    ui->setupUi(this);

    // test players_list_widget
    if(players_list_widget != nullptr){
        this->players_list_widget = players_list_widget;
    }


    // pointer on database
    db = &database;

    // save mode for future purpose
    mode = add_mode;

    // if flag add_mode is false it means this widget will be filling
    // with arguments
    if(!add_mode){

        // make a preview settings for lines and other widgets
        previewSettings(false);

        // fill the widget
        // let's make a query and get all player's data
        QSqlQuery query(database);

        query.prepare("SELECT "
                      "first_name, last_name, age, sex, height, hometown, phone, picture"
                      " FROM Players"
                      " WHERE first_name = :f_name AND last_name = :l_name");

        query.bindValue(":f_name", player_first_name);
        query.bindValue(":l_name", player_last_name);


        if(!query.exec()){
            QMessageBox::warning(this, "Database error", "Couldn't load player's data");
            return;
        }
        else{

            while(query.next()){
                ui->firstNameLine->setText(query.value(0).toString()); // first name
                ui->lastNameLine->setText(query.value(1).toString());  // last name
                ui->ageSpinBox->setValue(query.value(2).toInt());      // age

                if(query.value(3).toInt()){                            // gender
                    ui->maleRadioButton->setChecked(true);
                }
                else{
                    ui->femaleRadioButton->setChecked(true);
                }

                ui->heightSpinBox->setValue(query.value(4).toInt());   // height
                ui->hometownLine->setText(query.value(5).toString());  // hometown
                ui->phoneLine->setText(query.value(6).toString());     // phone

                if(!image.load(query.value(7).toString())){            // image
                    QMessageBox::warning(this, "File error!",
                                         "Couldn't load player's image!");
                }

                // displaying player's image
                ui->imageLabel->setPixmap(QPixmap::fromImage(image));

                ui->imageLabel->setScaledContents(true);

                // change tools interface
                ui->deleteImageButton->hide();
                ui->addImageButton->hide();
                ui->clearAllButton->hide();
                ui->savePlayerButton->hide();
                ui->ageSpinBox->setDisabled(true);
                ui->femaleRadioButton->setDisabled(true);
                ui->heightSpinBox->setDisabled(true);
                ui->hometownLine->setDisabled(true);
                ui->lastNameLine->setDisabled(true);
                ui->maleRadioButton->setDisabled(true);
                ui->phoneLine->setDisabled(true);
                ui->firstNameLine->setDisabled(true);
            }
        }
    // regular adding a new player
    }
    else{
        // make a preview settings for lines and other widgets
        previewSettings();
    }
}

AddPlayer::~AddPlayer()
{
    delete ui;
}


/*
    Function makes all preview settings for all lines in the widget
*/
void AddPlayer::previewSettings(bool regular){

    if(regular){

        ui->ageSpinBox->setValue(static_cast<int>(DefaultWidgetValues::DefaultAge));

        // Sex radio buttons settings
        ui->maleRadioButton->setChecked(true);
        ui->heightSpinBox->setValue(static_cast<int>(DefaultWidgetValues::DefaultHeight));

        // Make focus on the first name line
        ui->firstNameLine->setFocus();
    }

    // Age box settings
    ui->ageSpinBox->setMinimum(static_cast<int>(DefaultWidgetValues::MinimumAge));
    ui->ageSpinBox->setMaximum(static_cast<int>(DefaultWidgetValues::MaximumAge));

    // Height box settings
    ui->heightSpinBox->setMinimum(static_cast<int>(DefaultWidgetValues::MinimumHeight));
    ui->heightSpinBox->setMaximum(static_cast<int>(DefaultWidgetValues::MaximumHeight));
}


/*
    Function delete current loaded image in the image (not from the database)
*/
void AddPlayer::on_deleteImageButton_clicked()
{
    ui->imageLabel->clear();
}


/*
    Function loads an player image in the widget (image is not in database yet)
*/
void AddPlayer::on_addImageButton_clicked()
{
    const QString player_img_path = QFileDialog::getOpenFileName(this,
                                                    "Select an image");

    if(!image.load(player_img_path)){
        QMessageBox::warning(this, "File error!", "Couldn't load an image!");
    }

    // displaying player's image
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));

    ui->imageLabel->setScaledContents(true);
}


/*
    Function clears all lines (a user is allowed to
    start filling them from scratch)
*/
void AddPlayer::clearAllLines(){

    ui->firstNameLine->clear();
    ui->lastNameLine->clear();
    ui->ageSpinBox->setValue(static_cast<int>(DefaultWidgetValues::DefaultAge));
    ui->maleRadioButton->setChecked(true);
    ui->heightSpinBox->setValue(static_cast<int>(DefaultWidgetValues::DefaultHeight));
    ui->phoneLine->clear();
    ui->hometownLine->clear();
    ui->imageLabel->clear();

    // focus on the first name line
    ui->firstNameLine->setFocus();
}


/*
    Function clears all data what have been inputed at this moment
    (a user is allowed to start filling the widget from scratch)
*/
void AddPlayer::on_clearAllButton_clicked()
{
    clearAllLines();
}


/*
    Function saves all player data in the database
    and creates folder on user's computer where
    saves player image.
*/
void AddPlayer::on_savePlayerButton_clicked()
{

    QString player_first_name = ui->firstNameLine->text();

    QString player_last_name = ui->lastNameLine->text();

    int player_age = ui->ageSpinBox->value();

    int player_height = ui->heightSpinBox->value();

    int player_sex{0};

    ui->femaleRadioButton->isChecked() ? player_sex : player_sex = 1;

    QString player_hometown = ui->hometownLine->text();

    QString player_phone = ui->phoneLine->text();

    // creating a new folder with players images( if it doesn't exist )
    QDir dir;

    // creating a players images directory
    if(!dir.exists(QDir::homePath() + "/Players_images")){
        dir.mkpath(QDir::homePath() + "/Players_images");
    }

    // saving an input image in the created folder
    // make image's name
    QString name = ui->firstNameLine->text() + "_" + ui->lastNameLine->text();
    //removing all possible whitespaces
    name = name.trimmed().replace(" ", "_");

    const QString image_name = QDir::homePath() + "/Players_images/" + name + ".jpeg";

    if(!image.save(image_name)){
        QMessageBox::warning(this, "Image error!", "Couldn't save an image!");
    }

    // making a query
    QSqlQuery query(*db);

    // depends of mode we do a following logic
    if(mode){
        // simply add a new player
        query.prepare("INSERT INTO Players"
            "(first_name, last_name, age, sex, height, hometown, phone, picture)"
            "VALUES"
            "(:f_name, :l_name, :p_age, :p_sex, :p_height, :p_city, :p_phone, :p_img);");
    }
    else{
        query.prepare("UPDATE Players "
                      "SET "
                      "first_name = :f_name, last_name = :l_name, "
                      "age = :p_age, sex = :p_sex, height = :p_height, "
                      "hometown = :p_city, phone = :p_phone, picture = :p_img"
                      " WHERE id = :player_id;");

        query.bindValue(":player_id", index);

    }

    query.bindValue(":f_name", player_first_name);
    query.bindValue(":l_name", player_last_name);
    query.bindValue(":p_age", player_age);
    query.bindValue(":p_sex", player_sex);
    query.bindValue(":p_height", player_height);
    query.bindValue(":p_city", player_hometown);
    query.bindValue(":p_phone", player_phone);
    query.bindValue(":p_img", image_name);

    if(!query.exec()){
        QMessageBox::warning(this, "Database problem", "Couldn't save player's data.");
        return;
    }
    else{
        QMessageBox::information(this,"Saving report",
                                 "New player has been succesfully saved in the database.");
        //clear all lines
        clearAllLines();
    }

    // update players list widget
    QSqlQuery players_query(*db);

    if(!players_query.exec("SELECT id, first_name, last_name, hometown"
                            " FROM Players"
                            " ORDER BY first_name ASC;")){

        QMessageBox::warning(this, "Updating players list",
                             "Couldn't load players data");
        return;
    }
    else{

        // clear list widget before updating

        if(players_list_widget != nullptr){

            players_list_widget->clear();

            while(players_query.next()){
                players_list_widget->addItem(
                    "ID: " +
                    players_query.value(0).toString() + ", " +   // id
                    players_query.value(1).toString() + " " +   // first name
                    players_query.value(2).toString() + ", ( " +   // second name
                    players_query.value(3).toString()           // hometown
                    + " )"
                );
            }
        }
    }
}
