#ifndef ADDEXERCISE_H
#define ADDEXERCISE_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class AddExercise;
}

class AddExercise : public QDialog
{
    Q_OBJECT

public:
    explicit AddExercise(QSqlDatabase &database, QWidget *parent = nullptr);
    ~AddExercise();

private:
    Ui::AddExercise *ui;

    QSqlDatabase *db{nullptr};
};

#endif // ADDEXERCISE_H
