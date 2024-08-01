#ifndef ADDPLAYER_H
#define ADDPLAYER_H

#include <QDialog>
#include <QImage>
#include <QSqlDatabase>
#include <QStringList>


namespace Ui {
class AddPlayer;
}

class AddPlayer : public QDialog
{
    Q_OBJECT

public:

    explicit AddPlayer(QSqlDatabase &database,
                       QWidget *parent = nullptr,
                       bool add_mode = true,
                       const QString &player_first_name = "Unknown",
                       const QString &player_last_name = "Unknown");
    ~AddPlayer();

private slots:
    void on_deleteImageButton_clicked();

    void on_addImageButton_clicked();

    void on_clearAllButton_clicked();

    void on_savePlayerButton_clicked();

private:

    void clearAllLines();

    void previewSettings(bool regular = true);

    Ui::AddPlayer *ui;

    QSqlDatabase *db{nullptr};

    QImage image;

    bool mode{true};

    QString index{"None"};
};

#endif // ADDPLAYER_H
