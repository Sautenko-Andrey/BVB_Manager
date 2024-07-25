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
    ChangePlayerWindowWidth = 600,
    ChangePlayerWindowHeight = 400
};


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


void createPlayersTable(QStandardItemModel *model, QTableView *tableView,
                        QSqlDatabase *db, QVector<Player> &all_players,
                        QDialog *dialog);


#endif // UTILS_H
