#ifndef DELETEEXERCISE_H
#define DELETEEXERCISE_H

#include <QDialog>
#include <QSqlDatabase>
#include <QListWidgetItem>

namespace Ui {
class DeleteExercise;
}

class DeleteExercise : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteExercise(QSqlDatabase &database, QWidget *parent = nullptr);
    ~DeleteExercise();

private slots:

    void clickOnItem();

    void on_deleteButton_clicked();

private:
    Ui::DeleteExercise *ui;

    QSqlDatabase *db{nullptr};

    int current_index{0};

    QListWidgetItem *item{nullptr};

    QString exercise_name{"Unknown"};
};

#endif // DELETEEXERCISE_H
