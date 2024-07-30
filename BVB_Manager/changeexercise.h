#ifndef CHANGEEXERCISE_H
#define CHANGEEXERCISE_H

#include <QDialog>
#include <QSqlDatabase>
#include <QStandardItemModel>
#include <QVector>

namespace Ui {
class ChangeExercise;
}

struct Exercise {

    QString exercise_title;
    QString exrcise_description;
    int exercise_type;
};

enum class HeaderIndexes {
    Title, Description, Type
};

class ChangeExercise : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeExercise(QSqlDatabase &database, QWidget *parent = nullptr);
    ~ChangeExercise();

private slots:

    void changeCell();

    void saveOldTitle();

private:
    Ui::ChangeExercise *ui;

    QVector<Exercise> all_exercises;

    QSqlDatabase *db{nullptr};

    QStandardItemModel *model{nullptr};

    QString original_title;

    const QStringList header_labels = {"Title", "Description", "Type"};
};

#endif // CHANGEEXERCISE_H
