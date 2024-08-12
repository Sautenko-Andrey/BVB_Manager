#include "addexercise.h"
#include "ui_addexercise.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>

AddExercise::AddExercise(QSqlDatabase &database,
                         QWidget *parent,
                         QListWidget *exercises_list_widget)
    : QDialog(parent)
    , ui(new Ui::AddExercise)
{
    ui->setupUi(this);

    // pointer on database
    db = &database;

    if(exercises_list_widget != nullptr){
        this->exercises_list_widget = exercises_list_widget;
    }

    // filling exercises types list
    ui->typeComboBox->addItems(training_types);

    // signals & slots
    connect(ui->typeComboBox, SIGNAL(currentTextChanged(QString)),
            this, SLOT(trainingTypeChanged()));

}

AddExercise::~AddExercise()
{
    delete ui;
}


void AddExercise::trainingTypeChanged(){

    switch (ui->typeComboBox->currentIndex()) {

    case static_cast<int>(TrainingTypeIndexes::Warm_up):
        current_training_type =
            training_types[static_cast<int>(TrainingTypeIndexes::Warm_up)];
        break;

    case static_cast<int>(TrainingTypeIndexes::Gym):
        current_training_type =
            training_types[static_cast<int>(TrainingTypeIndexes::Gym)];
        break;

    case static_cast<int>(TrainingTypeIndexes::Cardio):
        current_training_type =
            training_types[static_cast<int>(TrainingTypeIndexes::Cardio)];
        break;

    case static_cast<int>(TrainingTypeIndexes::Tactical_technical):
        current_training_type =
            training_types[static_cast<int>(TrainingTypeIndexes::Tactical_technical)];
        break;
    }
}

void AddExercise::on_pushButton_clicked()
{
    // save data in the database
    QSqlQuery query(*db);

    query.prepare("INSERT "
                   "INTO Exercises(title, description, exercise_type)"
                   " VALUES(:ex_title, :ex_description, :ex_type)");

    query.bindValue(":ex_title", ui->titleLine->text());
    query.bindValue(":ex_description", ui->description->toPlainText());
    query.bindValue(":ex_type", current_training_type);

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
    ui->typeComboBox->setCurrentIndex(static_cast<int>(TrainingTypeIndexes::Warm_up));

    // Set focus on the title line
    ui->titleLine->setFocus();

    // updating exercises list
    QSqlQuery update_query(*db);

    if(!update_query.exec("SELECT title, description, exercise_type"
                          " FROM Exercises"
                          " ORDER BY title ASC;"))
    {
        QMessageBox::warning(this, "Exrcise List error", "Couldn't update exrcises list");
        return;
    }
    else{

        //clear and update exercises list
        exercises_list_widget->clear();

        while(update_query.next()){
            exercises_list_widget->addItem(
                update_query.value(0).toString() + " ( " +    // exercise title
                update_query.value(2).toString() + " ) ");      // exercise type
        }
    }
}
