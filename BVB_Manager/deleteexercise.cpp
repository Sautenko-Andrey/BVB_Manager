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
                   " title, exercise_type"
                   " FROM Exercises"
                   " ORDER BY exercise_type ASC;")){
        QMessageBox::warning(this, "Database error", "Couldn't load exercises data");
        return;
    }
    else{

        while(query.next()){
            ui->listWidget->addItem(
                query.value(0).toString() + " ( " + query.value(1).toString() + " )"
            );
        }
    }

    // signals & slots
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
