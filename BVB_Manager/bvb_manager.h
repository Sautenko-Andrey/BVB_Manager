#ifndef BVB_MANAGER_H
#define BVB_MANAGER_H

#include <QMainWindow>
#include <QDate>
#include <QSet>
#include <QLabel>
#include <memory>
#include "addplayer.h"
#include "databasemanager.h"
#include "deleteoneplayer.h"
#include "changeplayer.h"
#include "addexercise.h"
#include "deleteexercise.h"
#include "changeexercise.h"
#include "updateplayer.h"
#include "searchplayer.h"
#include "doubleeliminationtournament.h"
#include "teamregistration.h"
#include "addtournament.h"
#include "tournamentcreator.h"
#include "tourdraw.h"
#include <QEvent>



QT_BEGIN_NAMESPACE
namespace Ui {
class BVB_Manager;
}
QT_END_NAMESPACE

class BVB_Manager : public QMainWindow
{
    Q_OBJECT

public:
    BVB_Manager(QWidget *parent = nullptr);
    ~BVB_Manager();

private slots:
    void on_actionAdd_a_new_player_triggered();

    void on_actionDelete_a_player_triggered();

    void on_actionDelete_all_players_triggered();

    void on_actionChange_a_player_triggered();

    void on_actionAdd_a_new_exercise_triggered();

    void on_actionDelete_an_exercise_triggered();

    void on_actionChange_an_exercise_triggered();

    void on_actionSearch_a_player_triggered();

    void selectedDateChanged();

    void selectedTimeChanged();

    void selectedPlayer();

    void selectedExercise();

    void exerciseCheckBoxChanged();

    void playersCheckBoxChanged();

    void on_removeAllPlayersButton_clicked();

    void on_addAllPlayersButton_clicked();

    void on_removeAllExercisesButton_clicked();

    void on_resetCurrentSettingButton_clicked();

    void on_addToscheduleButton_clicked();

    void on_eraseButton_clicked();

    void on_redoButton_clicked();

    void on_undoButton_clicked();

    void changeGridCheckBox();

    void changeWeekNumsCheckBox();

    void comboLangChanged();

    void dateActivated();

    void dateClicked();

    void on_actionTournament_16_triggered();

    void on_actionTournament_32_triggered();

    void on_actionTournament_64_triggered();

    void on_actionRegister_a_team_triggered();

    void on_actionAdd_a_new_tournament_triggered();

    void on_actionCreate_a_tournament_triggered();

    //void on_actionDraw_triggered();

private:
    Ui::BVB_Manager *ui;

    //DatabaseManager database_manager;

    DatabaseManager &database_manager = DatabaseManager::getInstance();

    std::unique_ptr<AddPlayer> add_player{nullptr};

    std::unique_ptr<DeleteOnePlayer> delete_player{nullptr};

    //std::unique_ptr<ChangePlayer> change_player{nullptr};

    std::unique_ptr<UpdatePlayer> update_player{nullptr};

    std::unique_ptr<AddExercise> add_exercise{nullptr};

    std::unique_ptr<DeleteExercise> delete_exercise{nullptr};

    std::unique_ptr<ChangeExercise> change_exercise{nullptr};

    std::unique_ptr<SearchPlayer> search_player{nullptr};

    std::unique_ptr<DoubleEliminationTournament> double_elim_tour{nullptr};

    std::unique_ptr<TeamRegistration> team_registration{nullptr};

    std::unique_ptr<AddTournament> add_tournament{nullptr};

    std::unique_ptr<TournamentCreator> tournament_creator{nullptr};

    std::unique_ptr<TourDraw> tour_draw{nullptr};

    QStringList marked_players;

    QStringList marked_exercises;

    QDate selected_date;

    QString combined_players;

    QString combined_exercises;

    QString current_date;

    QString original_gym_exercise_name;

    int trainings_counter{0};

    void updateDate();

    void updateTime();

    void markItem(QListWidgetItem *item, const QBrush &color, bool is_bold = true);

    void markUnmarkItem(QListWidget *list_widget, QStringList &container,
                        QString &text, QLabel *label);

    void removeListWidgetItems(QLabel *label, QListWidget *widget,
                               QStringList &container);

    void getExercises(const QString &type);

    void getPlayers(bool gender);

    QString timeToString();

    void clearEditor();

    int getTrainings(const QDate &date);

    // Event filter to catch hover events
    bool eventFilter(QObject *obj, QEvent *event) override {
        QLabel *label = qobject_cast<QLabel*>(obj);  // Cast the object to QLabel
        if (label) {
            if (event->type() == QEvent::Enter) {
                // Mouse hover event
                QFont font = label->font();
                font.setPointSize(12);  // Increase font size
                label->setFont(font);
                label->setStyleSheet("background-color: #a0a0ff; color: black; font-weight: bold; padding: 10px; border-radius: 5px;");
            } else if (event->type() == QEvent::Leave) {
                // Mouse leave event
                QFont font = label->font();
                font.setPointSize(11);  // Reset font size
                label->setFont(font);
                label->setStyleSheet("background-color: AntiqueWhite; color: black; padding: 10px; border-radius: 5px;");
            }
        }
        // Pass the event on to the base class
        return QWidget::eventFilter(obj, event);
    }
};
#endif // BVB_MANAGER_H
