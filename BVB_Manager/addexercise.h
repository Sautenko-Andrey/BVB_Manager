#ifndef ADDEXERCISE_H
#define ADDEXERCISE_H

#include <QDialog>
#include <QSqlDatabase>
#include <QStringList>
#include "utils.h"

namespace Ui {
class AddExercise;
}

class AddExercise : public QDialog
{
    Q_OBJECT

public:
    explicit AddExercise(QSqlDatabase &database, QWidget *parent = nullptr);
    ~AddExercise();

private slots:
    void trainingTypeChanged();

    void on_pushButton_clicked();

private:
    Ui::AddExercise *ui;

    QSqlDatabase *db{nullptr};

    QString current_training_type =
        training_types[static_cast<int>(TrainingTypeIndexes::Warm_up)];
};

#endif // ADDEXERCISE_H
