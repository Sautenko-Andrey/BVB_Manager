#ifndef TOURNAMENTCREATOR_H
#define TOURNAMENTCREATOR_H

#include <QDialog>
#include <QSqlDatabase>
#include "calendar.h"
#include <memory>
#include <QStringList>
#include <QVector>
#include <QCheckBox>
#include "teamregistration.h"
#include <QBoxLayout>
#include "utils.h"
#include "doubleeliminationtournament.h"
#include "tourdraw.h"


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

    void tabChanged();

    void searchPlayer();

    void on_clearSearchLineButton_clicked();

    void on_selectAllTeamsButton_clicked();

    void on_unselectAllTeamsButton_clicked();

    void selectedTeamsChanged();

private:
    Ui::TournamentCreator *ui;

    QSqlDatabase *db{nullptr};

    std::unique_ptr<Calendar> calendar{nullptr};

    const QStringList tour_net_types{"16", "24", "32"};

    const QStringList tour_gender_types{"men", "women", "mix"};

    QString selected_tour_gender_type{tour_gender_types[0]};

    QString selected_tour_net_type{tour_net_types[0]};

    QVector<QCheckBox *> teams;

    QString selected_tournament{"None"};

    std::unique_ptr<TeamRegistration> team_registration{nullptr};

    QVBoxLayout *vbox{nullptr};

    QVector<QCheckBox *> selected_teams;

    Tournament completed_tournament;

    std::unique_ptr<TourDraw> tour_draw{nullptr};

};

#endif // TOURNAMENTCREATOR_H
