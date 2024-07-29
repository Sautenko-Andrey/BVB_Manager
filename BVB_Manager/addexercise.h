#ifndef ADDEXERCISE_H
#define ADDEXERCISE_H

#include <QDialog>
#include <QSqlDatabase>
#include <QStringList>
#include "utils.h"

namespace Ui {
class AddExercise;
}

// enum class TrainingTypeIndexes {
//     Warming_up, Gym, Cardio, Tactical_technical
// };

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

    int current_type_index = static_cast<int>(TrainingTypeIndexes::Warming_up);
};

#endif // ADDEXERCISE_H
