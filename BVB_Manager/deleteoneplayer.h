#ifndef DELETEONEPLAYER_H
#define DELETEONEPLAYER_H

#include <QDialog>
#include <QSqlDatabase>
#include <QStandardItemModel>
#include <QVector>
#include "utils.h"

namespace Ui {
class DeleteOnePlayer;
}


class DeleteOnePlayer : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteOnePlayer(QSqlDatabase &database, QWidget *parent = nullptr);
    ~DeleteOnePlayer();

private slots:
    void on_markButton_clicked();

    void on_deleteButton_clicked();

    void on_helpButton_clicked();

    void deleteModeChanged();

private:
    Ui::DeleteOnePlayer *ui;

    QVector<Player> all_players;

    QVector<int> marked_rows;   // for view

    QVector<int> marked_indexes;   // for database

    QVector<QString> removed_names;   // for pics folder

    QSqlDatabase *db{nullptr};

    QStandardItemModel *model{nullptr};

    bool mode{true};

    int row_for_del{0};

    int db_index_for_del{0};

    QString name_for_del{"Unknown"};

    inline static const QStringList headers = {
        "ID", "First name", "Last name", "Age", "Gender", "Height", "Hometown", "Phone"
    };
};

#endif // DELETEONEPLAYER_H
