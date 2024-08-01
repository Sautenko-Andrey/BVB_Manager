// #ifndef CHANGEPLAYER_H
// #define CHANGEPLAYER_H

// #include <QDialog>
// #include <QSqlDatabase>
// #include <QStandardItemModel>
// #include <QVector>
// #include <memory>
// #include "utils.h"
// #include "addplayer.h"

// namespace Ui {
// class ChangePlayer;
// }


// class ChangePlayer : public QDialog
// {
//     Q_OBJECT

// public:
//     explicit ChangePlayer(QSqlDatabase &database, QWidget *parent = nullptr);
//     ~ChangePlayer();

// private slots:
//     void changeData();

// private:
//     Ui::ChangePlayer *ui;

//     QVector<Player> all_players;

//     QSqlDatabase *db{nullptr};

//     QStandardItemModel *model{nullptr};

//     std::unique_ptr<AddPlayer> player_window{nullptr};
// };

// #endif // CHANGEPLAYER_H
