#include "changeexercise.h"
#include "ui_changeexercise.h"
#include <QSqlQuery>
#include <QMessageBox>
#include "utils.h"
#include <QItemSelectionModel>

ChangeExercise::ChangeExercise(QSqlDatabase &database, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChangeExercise)
{
    ui->setupUi(this);

    // pointer on database
    db = &database;

    // filling a table
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(header_labels);

    QSqlQuery query(database);

    if(!query.exec("SELECT title, description, type FROM Exercises ORDER BY type ASC;")){
        QMessageBox::warning(this, "Database error",
                             "Couldn't load exercises data from the database");
        return;
    }
    else{
        while(query.next()){
            Exercise exercise(query.value(0).toString(),
                              query.value(1).toString(),
                              query.value(2).toInt());

            all_exercises.push_back(exercise);
        }
    }

    // set row amount
    ui->tableWidget->setRowCount(all_exercises.size());

    int i = 0;
    for(auto const &exercise : all_exercises){
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(exercise.exercise_title));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(exercise.exrcise_description));

        QString type;
        switch (exercise.exercise_type) {
            case static_cast<int>(TrainingTypeIndexes::Warming_up):
                type = "Warming up";
                break;

            case static_cast<int>(TrainingTypeIndexes::Gym):
                type = "Gym";
                break;

            case static_cast<int>(TrainingTypeIndexes::Cardio):
                type = "Cardio";
                break;

            case static_cast<int>(TrainingTypeIndexes::Tactical_technical):
                type = "Tactical-technical";
                break;
        }

        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(type));

        ++i;
    }

    // signals & slots
    connect(ui->tableWidget, SIGNAL(cellChanged(int,int)),
            this, SLOT(changeCell()));

    connect(ui->tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
            this, SLOT(saveOldTitle()));

}

ChangeExercise::~ChangeExercise()
{
    delete ui;
}

void ChangeExercise::saveOldTitle(){
    original_title = ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->text();
}

void ChangeExercise::changeCell(){

    // getting a new data
    auto data = ui->tableWidget->item(ui->tableWidget->currentRow(),
                          ui->tableWidget->currentColumn())->text();

    // getting a column number
    auto column = ui->tableWidget->currentColumn();

    // make a query
    QSqlQuery query(*db);

    switch (column) {
    case static_cast<int>(HeaderIndexes::Title):
        query.prepare("UPDATE Exercises SET title = :new_data WHERE title = :org_title;");
        query.bindValue(":new_data", data);
        query.bindValue(":org_title", original_title);
        break;

    case static_cast<int>(HeaderIndexes::Description):
        query.prepare("UPDATE Exercises "
                      "SET description = :new_data WHERE title = :org_title;");
        query.bindValue(":new_data", data);
        query.bindValue(":org_title", original_title);
        break;

    case static_cast<int>(HeaderIndexes::Type):
        int type{0};

        if(data == training_types[0]){
            type = static_cast<int>(TrainingTypeIndexes::Warming_up);
        }
        else if(data == training_types[1]){
            type = static_cast<int>(TrainingTypeIndexes::Gym);
        }
        else if(data == training_types[2]){
            type = static_cast<int>(TrainingTypeIndexes::Cardio);
        }
        else if(data == training_types[3]){
            type = static_cast<int>(TrainingTypeIndexes::Tactical_technical);
        }
        else{
            type = static_cast<int>(TrainingTypeIndexes::Warming_up);
        }

        query.prepare("UPDATE Exercises SET type = :new_data WHERE title = :org_title;");
        query.bindValue(":new_data", type);
        query.bindValue(":org_title", original_title);
    }

    if(!query.exec()){
        QMessageBox::warning(this, "Database error", "Couldn't update exercise's data");
        return;
    }
    else{
        QMessageBox::information(this, "Update report", "Exercise has been updated");
    }
}
