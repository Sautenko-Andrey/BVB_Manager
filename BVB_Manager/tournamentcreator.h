#ifndef TOURNAMENTCREATOR_H
#define TOURNAMENTCREATOR_H

#include <QDialog>
#include <QSqlDatabase>
#include "calendar.h"
#include <memory>

namespace Ui {
class TournamentCreator;
}

class TournamentCreator : public QDialog
{
    Q_OBJECT

public:
    explicit TournamentCreator(QSqlDatabase *database, QWidget *parent = nullptr);
    ~TournamentCreator();

private slots:
    void on_openCalendarButton_clicked();

    void tournamentDateChanged();

private:
    Ui::TournamentCreator *ui;

    QSqlDatabase *db{nullptr};

    std::unique_ptr<Calendar> calendar{nullptr};
};

#endif // TOURNAMENTCREATOR_H
