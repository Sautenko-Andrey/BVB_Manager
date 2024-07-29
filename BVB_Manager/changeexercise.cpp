#include "changeexercise.h"
#include "ui_changeexercise.h"

ChangeExercise::ChangeExercise(QSqlDatabase &database, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChangeExercise)
{
    ui->setupUi(this);

    // pointer on database
    db = &database;

    connect(ui->tableWidget, SIGNAL(itemClicked(QTableWidgetItem*)),
            this, SLOT(clickOnItem()));
}

ChangeExercise::~ChangeExercise()
{
    delete ui;
}

void ChangeExercise::on_changeButton_clicked()
{

}

void ChangeExercise::clickOnItem(){

}

