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

private:
    Ui::BVB_Manager *ui;

    DatabaseManager database_manager;

    std::unique_ptr<AddPlayer> add_player{nullptr};

    std::unique_ptr<DeleteOnePlayer> delete_player{nullptr};

    //std::unique_ptr<ChangePlayer> change_player{nullptr};

    std::unique_ptr<UpdatePlayer> update_player{nullptr};

    std::unique_ptr<AddExercise> add_exercise{nullptr};

    std::unique_ptr<DeleteExercise> delete_exercise{nullptr};

    std::unique_ptr<ChangeExercise> change_exercise{nullptr};

    std::unique_ptr<SearchPlayer> search_player{nullptr};

    QSet<QString> marked_players;

    QDate selected_date;

    QString combined_players;

    void updateDate();

    void updateTime();

    void markItem(QListWidgetItem *item, const QBrush &color, const QFont &font);
};
#endif // BVB_MANAGER_H
