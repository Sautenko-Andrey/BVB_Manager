#include "deleteexercise.h"
#include "ui_deleteexercise.h"
#include "utils.h"
#include <QSqlQuery>
#include <QMessageBox>

DeleteExercise::DeleteExercise(QSqlDatabase &database, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DeleteExercise)
{
    ui->setupUi(this);

    // pointer on the database
    db = &database;

    ui->listWidget->setMinimumSize(
        QSize(static_cast<int>(Sizes::DeleteExerciseWindowWidth),
              static_cast<int>(Sizes::DeleteExerciseWindowHeight))
    );

    ui->listWidget->setMaximumWidth(static_cast<int>(Sizes::DeleteExerciseWindowMaxWidth));

    ui->listWidget->setStyleSheet(
        "QListWidget::item{border-bottom: 2px solid green;}"
    );

    // get data from the database
    QSqlQuery query(*db);

    if(!query.exec("SELECT"
                   " title, type"
                   " FROM Exercises"
                   " ORDER BY type ASC;")){
        QMessageBox::warning(this, "Database error", "Couldn't load exercises data");
        return;
    }
    else{

        while(query.next()){

            QString type_name{"Unknown"};

            switch (query.value(1).toInt()) {
            case static_cast<int>(TrainingTypeIndexes::Warming_up):
                type_name = training_types[static_cast<int>(TrainingTypeIndexes::Warming_up)];
                break;
            case static_cast<int>(TrainingTypeIndexes::Gym):
                type_name = training_types[static_cast<int>(TrainingTypeIndexes::Gym)];
                break;
            case static_cast<int>(TrainingTypeIndexes::Cardio):
                type_name = training_types[static_cast<int>(TrainingTypeIndexes::Cardio)];
                break;
            case static_cast<int>(TrainingTypeIndexes::Tactical_technical):
                type_name = training_types[static_cast<int>(TrainingTypeIndexes::Tactical_technical)];
                break;
            }

            ui->listWidget->addItem(
                query.value(0).toString() + " ( " + type_name + " )"
            );
        }
    }

    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(clickOnItem()));
}

DeleteExercise::~DeleteExercise()
{
    delete ui;
}

void DeleteExercise::clickOnItem(){

    // getting exercise name for further purposes
    current_index = ui->listWidget->currentRow();
    auto current_name = ui->listWidget->currentItem()->text();

    auto first_brace = current_name.indexOf("(");
    exercise_name = current_name.left(first_brace);
    exercise_name.chop(1);
}

void DeleteExercise::on_deleteButton_clicked()
{
    // deleting item from list widget
    item = ui->listWidget->takeItem(current_index);
    delete item;

    //deleting data from the database
    QSqlQuery query(*db);

    query.prepare("DELETE FROM Exercises WHERE title = :e_name;");
    query.bindValue(":e_name", exercise_name);

    if(!query.exec()){
        QMessageBox::warning(this, "Database error",
                             "Couldn't delete the exercise from the database");
    }
    else{
        QMessageBox::information(this, "Deleting exercise report",
                                 "The exrcise has been deleted");
    }
}
