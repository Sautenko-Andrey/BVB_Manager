#include "addtournament.h"
#include "ui_addtournament.h"
#include <QSqlQuery>
#include <QMessageBox>

AddTournament::AddTournament(QSqlDatabase *database, QWidget *parent)
    : QDialog(parent)
    , db{database}
    , ui(new Ui::AddTournament)
{
    ui->setupUi(this);

    // make focis on the title line
    ui->titleLine->setFocus();

    // fill ganders mode
    ui->genderComboBox->addItems(genders);
}

AddTournament::~AddTournament()
{
    delete ui;
}


/*
    Function clears all data in the widget (doesn't delete it from database)
*/
void AddTournament::clear_window(){
    // clear lines
    ui->titleLine->clear();
    ui->locationLine->clear();
    ui->descriptionTextEdit->clear();

    // make gender box as default
    ui->genderComboBox->setCurrentText(genders.first());

    // make focis on the title line
    ui->titleLine->setFocus();
}


void AddTournament::on_clearButton_clicked()
{
    // make all lines blanked
    clear_window();
}


/*
    Function saves all tournament data into database
*/
void AddTournament::on_saveButton_clicked()
{
    // save tournament to the database
    QSqlQuery query(*db);

    query.prepare("INSERT"
                  " INTO Tournaments"
                  " (title, location, description, gender)"
                  " VALUES (:t_title, :t_location, :t_descr, :t_gender);");

    query.bindValue(":t_title", ui->titleLine->text().trimmed());
    query.bindValue(":t_location", ui->locationLine->text().trimmed());
    query.bindValue(":t_descr", ui->descriptionTextEdit->toPlainText().trimmed());
    query.bindValue(":t_gender", ui->genderComboBox->currentText());

    if(!query.exec()){
        QMessageBox::warning(this, "Database error",
                             "Couldn't save the tournament: " + ui->titleLine->text());
        return;
    }
    else{
        QMessageBox::information(this, "Saving " + ui->titleLine->text(),
            ui->titleLine->text() + " has been successfully saved to the database.");
    }

    // make all lines blanked
    clear_window();
}


/*
    Function simply clears title
*/
void AddTournament::on_clearTitleButton_clicked()
{
    // clear title line
    ui->titleLine->clear();

    ui->titleLine->setFocus();
}

/*
    Function simply clears location line
*/
void AddTournament::on_clearLocationButton_clicked()
{
    // clear location line
    ui->locationLine->clear();

    ui->locationLine->setFocus();
}


/*
    Function simply clears description line
*/
void AddTournament::on_clearDescriptionButton_clicked()
{
    // clear description line
    ui->descriptionTextEdit->clear();

    ui->descriptionTextEdit->setFocus();
}

