#ifndef BVB_MANAGER_H
#define BVB_MANAGER_H

#include <QMainWindow>
#include <memory>
#include "addplayer.h"
#include "databasemanager.h"
#include "deleteoneplayer.h"
#include "changeplayer.h"
#include "addexercise.h"

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

private:
    Ui::BVB_Manager *ui;

    DatabaseManager dataBaseManager;

    std::unique_ptr<AddPlayer> addPlayer{nullptr};

    std::unique_ptr<DeleteOnePlayer> deletePlayer{nullptr};

    std::unique_ptr<ChangePlayer> changePlayer{nullptr};

    std::unique_ptr<AddExercise> addExercise{nullptr};
};
#endif // BVB_MANAGER_H
