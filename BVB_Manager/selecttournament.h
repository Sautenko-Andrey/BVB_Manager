#ifndef SELECTTOURNAMENT_H
#define SELECTTOURNAMENT_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class SelectTournament;
}

class SelectTournament : public QDialog
{
    Q_OBJECT

public:
    explicit SelectTournament(QSqlDatabase *database, QWidget *parent = nullptr);
    ~SelectTournament();

private:
    Ui::SelectTournament *ui;

    QSqlDatabase *db{nullptr};
};

#endif // SELECTTOURNAMENT_H
