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
    ui->tableWidget->setColumnCount(header_labels.size());
    ui->tableWidget->setHorizontalHeaderLabels(header_labels);

    // change color and make font-size bold
    changeTableItemStyle(ui->tableWidget, QColor("aqua"), true);

    QSqlQuery query(database);

    constexpr int possible_exercises_amount{100};
    all_exercises.reserve(possible_exercises_amount);

    if(!query.exec("SELECT "
                   "title, description, exercise_type"
                   " FROM Exercises ORDER BY exercise_type ASC;")){
        QMessageBox::warning(this, "Database error",
                             "Couldn't load exercises data from the database");
        return;
    }
    else{
        while(query.next()){

            all_exercises.emplace_back(Exercise(query.value(0).toString(),
                                                query.value(1).toString(),
                                                query.value(2).toString()));
        }
    }

    // set row amount
    ui->tableWidget->setRowCount(all_exercises.size());

    int i = 0;
    for(auto const &exercise : all_exercises){
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(exercise.exercise_title));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(exercise.exrcise_description));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(exercise.exercise_type));

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


/*
    Function saves an old title of the exercise
*/
void ChangeExercise::saveOldTitle(){
    original_title = ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->text();
}


/*
    Function allows user to change exercise data through the cell of the table
*/
void ChangeExercise::changeCell(){

    // getting a new data
    const auto &data = ui->tableWidget->item(ui->tableWidget->currentRow(),
                          ui->tableWidget->currentColumn())->text();

    // getting a column number
    const auto &column = ui->tableWidget->currentColumn();

    // make a query
    QSqlQuery query(*db);

    switch (column) {
    case static_cast<int>(HeaderIndexes::Title):
        query.prepare("UPDATE Exercises "
                      "SET title = :new_data WHERE title = :org_title;");
        break;

    case static_cast<int>(HeaderIndexes::Description):
        query.prepare("UPDATE Exercises "
                      "SET description = :new_data WHERE title = :org_title;");
        break;

    case static_cast<int>(HeaderIndexes::Type):
        query.prepare("UPDATE Exercises "
                      "SET exercise_type = :new_data WHERE title = :org_title;");
    }

    query.bindValue(":new_data", data);
    query.bindValue(":org_title", original_title);

    if(!query.exec()){
        QMessageBox::warning(this, "Database error", "Couldn't update exercise's data");
        return;
    }
    else{
        QMessageBox::information(this, "Update report", "Exercise has been updated");
    }
}
