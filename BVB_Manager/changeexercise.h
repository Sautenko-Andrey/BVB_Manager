#ifndef CHANGEEXERCISE_H
#define CHANGEEXERCISE_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class ChangeExercise;
}

class ChangeExercise : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeExercise(QSqlDatabase &database, QWidget *parent = nullptr);
    ~ChangeExercise();

private slots:
    void on_changeButton_clicked();

    void clickOnItem();

private:
    Ui::ChangeExercise *ui;

    QSqlDatabase *db{nullptr};
};

#endif // CHANGEEXERCISE_H
