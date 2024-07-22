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

AddPlayer::AddPlayer(QSqlDatabase &database, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddPlayer)
{
    ui->setupUi(this);

    // pointer on database
    db = &database;

    // Age box settings
    ui->ageSpinBox->setValue(static_cast<int>(DefaultWidgetValues::DefaultAge));
    ui->ageSpinBox->setMinimum(static_cast<int>(DefaultWidgetValues::MinimumAge));
    ui->ageSpinBox->setMaximum(static_cast<int>(DefaultWidgetValues::MaximumAge));

    // Sex radio buttons settings
    ui->maleRadioButton->setChecked(true);

    // Height box settings
    ui->heightSpinBox->setMinimum(static_cast<int>(DefaultWidgetValues::MinimumHeight));
    ui->heightSpinBox->setMaximum(static_cast<int>(DefaultWidgetValues::MaximumHeight));
    ui->heightSpinBox->setValue(static_cast<int>(DefaultWidgetValues::DefaultHeight));

    // Make focus on the first name line
    ui->firstNameLine->setFocus();
}

AddPlayer::~AddPlayer()
{
    delete ui;
}

void AddPlayer::on_deleteImageButton_clicked()
{
    ui->imageLabel->clear();
}


void AddPlayer::on_addImageButton_clicked()
{
    const QString player_img_path = QFileDialog::getOpenFileName(this, "Select an image");

    if(!image.load(player_img_path)){
        QMessageBox::warning(this, "File error!", "Couldn't load an image!");
    }

    // displaying player's image
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));

    ui->imageLabel->setScaledContents(true);
}

void AddPlayer::clearAllLines(){
    // clear all input data in the form
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


void AddPlayer::on_clearAllButton_clicked()
{
    clearAllLines();
}


void AddPlayer::on_savePlayerButton_clicked()
{
    // save all player's data in database
    // getting data from the dialog
    QString player_first_name = ui->firstNameLine->text();
    QString player_last_name = ui->lastNameLine->text();
    int player_age = ui->ageSpinBox->value();
    int player_height = ui->heightSpinBox->value();
    int player_sex{0};
    ui->femaleRadioButton->isChecked() ? player_sex : player_sex = 1;
    QString player_hometown = ui->hometownLine->text();
    QString player_phone = ui->phoneLine->text();

    // creating a new folder with players images( if doesn't exist )
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

    query.prepare("INSERT INTO Players"
        "(first_name, last_name, age, sex, height, hometown, phone, picture)"
        "VALUES"
        "(:f_name, :l_name, :p_age, :p_sex, :p_height, :p_city, :p_phone, :p_img);");

    query.bindValue(":f_name", player_first_name);
    query.bindValue(":l_name", player_last_name);
    query.bindValue(":p_age", player_age);
    query.bindValue(":p_sex", player_sex);
    query.bindValue(":p_height", player_height);
    query.bindValue(":p_city", player_hometown);
    query.bindValue(":p_phone", player_phone);
    query.bindValue(":p_img", image_name);

    if(!query.exec()){
        QMessageBox::warning(this, "Database problem", "Couldn't save a new player.");
        return;
    }
    else{
        QMessageBox::information(this,"Saving report",
                                 "New player has been succesfully saved in the database.");
        //clear all lines
        clearAllLines();
    }
}

