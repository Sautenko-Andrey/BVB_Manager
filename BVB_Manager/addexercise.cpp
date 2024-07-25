#include "addexercise.h"
#include "ui_addexercise.h"

AddExercise::AddExercise(QSqlDatabase &database, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddExercise)
{
    ui->setupUi(this);

    // pointer on database
    db = &database;
}

AddExercise::~AddExercise()
{
    delete ui;
}
