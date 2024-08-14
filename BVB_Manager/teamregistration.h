#ifndef TEAMREGISTRATION_H
#define TEAMREGISTRATION_H

#include <QDialog>
#include <QSqlDatabase>
#include <QLineEdit>
#include <QSpinBox>

namespace Ui {
class TeamRegistration;
}

class TeamRegistration : public QDialog
{
    Q_OBJECT

public:
    explicit TeamRegistration(QSqlDatabase &database, QWidget *parent = nullptr);
    ~TeamRegistration();

private slots:
    void on_clearAllButton_clicked();

    void on_clearPlayer_1Button_clicked();

    void on_clearPlayer_2Button_clicked();

    void on_registerButton_clicked();

private:
    Ui::TeamRegistration *ui;

    QSqlDatabase *db{nullptr};

    void dropAllData();

    void dropPlayerData(QLineEdit *player_name,
                        QLineEdit *player_city,
                        QSpinBox *player_height,
                        QSpinBox *player_age,
                        bool focus = true);
};

#endif // TEAMREGISTRATION_H
