#ifndef TOURNAMENTCREATOR_H
#define TOURNAMENTCREATOR_H

#include <QDialog>
#include <QSqlDatabase>
#include "calendar.h"
#include <memory>
#include <QStringList>
#include <QVector>
#include <QCheckBox>

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

    void tourTypeChanged();

    void netModeChanged();

    void on_addButton_clicked();

    void tourChanged();

private:
    Ui::TournamentCreator *ui;

    QSqlDatabase *db{nullptr};

    std::unique_ptr<Calendar> calendar{nullptr};

    const QStringList tour_net_types{"16", "32", "64"};

    const QStringList tour_gender_types{"men", "women", "mix"};

    QString selected_tour_gender_type{tour_gender_types[0]};

    QString selected_tour_net_type{tour_net_types[0]};

    QVector<QCheckBox *> teams;

    QString selected_tournament{"None"};
};

#endif // TOURNAMENTCREATOR_H
