#ifndef DELETEONEPLAYER_H
#define DELETEONEPLAYER_H

#include <QDialog>
#include <QSqlDatabase>
#include <QStandardItemModel>
#include <QVector>

namespace Ui {
class DeleteOnePlayer;
}

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

class DeleteOnePlayer : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteOnePlayer(QSqlDatabase &database, QWidget *parent = nullptr);
    ~DeleteOnePlayer();

private slots:
    void on_markButton_clicked();

    void on_deleteButton_clicked();

private:
    Ui::DeleteOnePlayer *ui;

    QVector<Player> all_players;

    QVector<int> marked_rows;   // for view

    QVector<int> marked_indexes;   // for database

    QVector<QString> removed_names;   // for pics folder

    QSqlDatabase *db{nullptr};

    QStandardItemModel *model{nullptr};
};

#endif // DELETEONEPLAYER_H
