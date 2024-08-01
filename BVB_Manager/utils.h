#ifndef UTILS_H
#define UTILS_H
#include <QString>
#include <QDialog>
#include <QStandardItemModel>
#include <QTableView>
#include <QSqlDatabase>

enum class Sizes {
    MainWindowWidth = 1366,
    MainWindowHeight = 768,
    ChangePlayerWindowWidth = 1000,
    ChangePlayerWindowHeight = 500,
    DeleteExerciseWindowWidth = 350,
    DeleteExerciseWindowHeight = 300,
    DeleteExerciseWindowMaxWidth = 400,
    SearchPlayerWindowWidth = 450,
    SearchPlayerWindowHeight = 200
};

enum class TrainingTypeIndexes {
    Warm_up, Gym, Cardio, Tactical_technical
};

extern const QStringList training_types;

extern const QString BACKGROUND_COLOR;

struct Player {
    int id;
    QString first_name;
    QString last_name;
    int age;
    int gender;
    int height;
    QString hometown;
    QString phone;
};

struct Player2 {
    int id;
    QString first_name;
    QString last_name;
    int age;
    QString gender;
    int height;
    QString hometown;
    QString phone;
    QString img;
};


void createPlayersTable(QStandardItemModel *model, QTableView *tableView,
                        QSqlDatabase *db, QVector<Player> &all_players,
                        QDialog *dialog);


#endif // UTILS_H
