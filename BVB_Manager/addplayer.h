#ifndef ADDPLAYER_H
#define ADDPLAYER_H

#include <QDialog>
#include <QImage>
#include <QSqlDatabase>


namespace Ui {
class AddPlayer;
}

class AddPlayer : public QDialog
{
    Q_OBJECT

public:
    explicit AddPlayer(QSqlDatabase &database, QWidget *parent = nullptr);
    ~AddPlayer();

private slots:
    void on_deleteImageButton_clicked();

    void on_addImageButton_clicked();

    void on_clearAllButton_clicked();

    void on_savePlayerButton_clicked();

private:

    void clearAllLines();

    Ui::AddPlayer *ui;

    QSqlDatabase *db{nullptr};

    QImage image;
};

#endif // ADDPLAYER_H
