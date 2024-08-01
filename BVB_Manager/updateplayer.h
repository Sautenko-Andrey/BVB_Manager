#ifndef UPDATEPLAYER_H
#define UPDATEPLAYER_H

#include <QDialog>
#include <QSqlDatabase>
#include <QVector>
#include "utils.h"
#include <QImage>
#include "addplayer.h"

namespace Ui {
class UpdatePlayer;
}

enum class Headers {
    FirstName = 1, LastName, Age, Gender,
    Height, Hometown, Phone, Image
};

class UpdatePlayer : public QDialog
{
    Q_OBJECT

public:
    explicit UpdatePlayer(QSqlDatabase &database, QWidget *parent = nullptr);
    ~UpdatePlayer();

private slots:

    void saveID();

    void changeCell();

    void picClicked();

private:
    Ui::UpdatePlayer *ui;

    std::unique_ptr<AddPlayer> player_window{nullptr};

    QVector<Player2> all_players;

    QSqlDatabase *db{nullptr};

    const QStringList header_labels = {
        "ID", "First name", "Last name", "Age", "Gender",
        "Height", "Hometown", "Phone", "Image"
    };

    QString player_id;

    QImage image;
};

#endif // UPDATEPLAYER_H
