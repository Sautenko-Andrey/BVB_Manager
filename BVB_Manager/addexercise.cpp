#include "addexercise.h"
#include "ui_addexercise.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>

AddExercise::AddExercise(QSqlDatabase &database, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddExercise)
{
    ui->setupUi(this);

    // pointer on database
    db = &database;

    // filling exercises types list
    ui->typeComboBox->addItems(training_types);


    connect(ui->typeComboBox, SIGNAL(currentTextChanged(QString)),
            this, SLOT(trainingTypeChanged()));

}

AddExercise::~AddExercise()
{
    delete ui;
}


void AddExercise::trainingTypeChanged(){

    switch (ui->typeComboBox->currentIndex()) {

    case static_cast<int>(TrainingTypeIndexes::Warming_up):
        current_type_index = static_cast<int>(TrainingTypeIndexes::Warming_up);
        break;

    case static_cast<int>(TrainingTypeIndexes::Gym):
        current_type_index = static_cast<int>(TrainingTypeIndexes::Gym);
        break;

    case static_cast<int>(TrainingTypeIndexes::Cardio):
        current_type_index = static_cast<int>(TrainingTypeIndexes::Cardio);
        break;

    case static_cast<int>(TrainingTypeIndexes::Tactical_technical):
        current_type_index = static_cast<int>(TrainingTypeIndexes::Tactical_technical);
        break;
    }
}

void AddExercise::on_pushButton_clicked()
{
    // save data in the database
    QSqlQuery query(*db);

    query.prepare("INSERT "
                   "INTO Exercises(title, description, type)"
                   " VALUES(:ex_title, :ex_description, :ex_type)");

    query.bindValue(":ex_title", ui->titleLine->text());
    query.bindValue(":ex_description", ui->description->toPlainText());
    query.bindValue(":ex_type", current_type_index);

    if(!query.exec()){
        QMessageBox::warning(this, "Database error",
                             "Couldn't save a new exercise. Reload app");
        return;
    }
    else{
        QMessageBox::information(this, "New exercise adding report",
                                 "A new exercise has been added!");
    }

    // clean lines
    ui->titleLine->clear();
    ui->description->clear();
    ui->typeComboBox->setCurrentIndex(static_cast<int>(TrainingTypeIndexes::Warming_up));

    // Set focus on the title line
    ui->titleLine->setFocus();
}
