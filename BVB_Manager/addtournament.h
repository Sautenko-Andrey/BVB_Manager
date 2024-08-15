#ifndef ADDTOURNAMENT_H
#define ADDTOURNAMENT_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class AddTournament;
}

class AddTournament : public QDialog
{
    Q_OBJECT

public:
    explicit AddTournament(QSqlDatabase *database, QWidget *parent = nullptr);
    ~AddTournament();

private slots:
    void on_clearButton_clicked();

    void on_saveButton_clicked();

    void on_clearTitleButton_clicked();

    void on_clearLocationButton_clicked();

    void on_clearDescriptionButton_clicked();

private:
    Ui::AddTournament *ui;

    QSqlDatabase *db{nullptr};

    const QStringList genders{ "men", "women", "all", "mix" };

    void clear_window();
};

#endif // ADDTOURNAMENT_H
