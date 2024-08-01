#ifndef SEARCHPLAYER_H
#define SEARCHPLAYER_H

#include <QDialog>
#include <QSqlDatabase>
#include "addplayer.h"

namespace Ui {
class SearchPlayer;
}

class SearchPlayer : public QDialog
{
    Q_OBJECT

public:
    explicit SearchPlayer(QSqlDatabase &database, QWidget *parent = nullptr);
    ~SearchPlayer();

private slots:
    void on_searchButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::SearchPlayer *ui;

    std::unique_ptr<AddPlayer> player_window{nullptr};

    void clearLineAndSetFocus();

    QSqlDatabase *db{nullptr};
};

#endif // SEARCHPLAYER_H
