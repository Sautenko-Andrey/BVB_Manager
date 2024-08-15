#ifndef TOURNAMENTCREATOR_H
#define TOURNAMENTCREATOR_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class TournamentCreator;
}

class TournamentCreator : public QDialog
{
    Q_OBJECT

public:
    explicit TournamentCreator(QSqlDatabase *database, QWidget *parent = nullptr);
    ~TournamentCreator();

private:
    Ui::TournamentCreator *ui;

    QSqlDatabase *db{nullptr};
};

#endif // TOURNAMENTCREATOR_H
