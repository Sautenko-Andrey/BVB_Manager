#ifndef UTILS_H
#define UTILS_H
#include <QString>
#include <QDialog>
#include <QStandardItemModel>
#include <QTableView>
#include <QSqlDatabase>
#include <QCheckBox>
#include <QPushButton>
#include <QTableWidget>

enum class Sizes {
    MainWindowWidth = 1366,
    MainWindowHeight = 768,
    ChangePlayerWindowWidth = 1000,
    ChangePlayerWindowHeight = 500,
    DeleteExerciseWindowWidth = 350,
    DeleteExerciseWindowHeight = 300,
    DeleteExerciseWindowMaxWidth = 400,
    SearchPlayerWindowWidth = 450,
    SearchPlayerWindowHeight = 280
};

enum class Geometry{
    coordXStartBtn = 10,
    coordYStartBtn = 15,
    BtnWidth = 115,
    BtnHeight = 23,
    BtnResWidth = 45,
    BtnResHeight = 17,
    Step = 44,
    StepX = 121
};

enum class TrainingTypeIndexes {
    Warm_up, Gym, Cardio, Tactical_technical
};

// Styles-----------------------------------
extern const QStringList training_types;

extern const QString BACKGROUND_COLOR;

extern const QString game_result_style;

extern const QString game_result_style_2;

extern const QString game_result_spinbox_style;

extern const QString team_btn_style;

extern const QString team_btn_style_for_none;

extern const QString team_btn_pulsing_style;

extern const QString ok_button_style;

extern const QString team_label_style;

extern const QString calendar_style;

extern const QString combobox_style;

extern const QString checkbox_style;

extern const QString lcd_number_style;

extern const QString time_edit_style;

extern const QString list_widget_style;

extern const QString pushbtn_style;

extern const QString scroll_area_style;

extern const QString label_style;

extern const QString label_style_hover;

extern const QString text_edit_style;


//----------------------------------------

struct Player {
    int id;
    QString first_name;
    QString last_name;
    int age;
    int gender;
    int height;
    QString hometown;
    QString phone;
};

struct Player2 {
    int id;
    QString first_name;
    QString last_name;
    int age;
    QString gender;
    int height;
    QString hometown;
    QString phone;
    QString img;
};

struct Tournament {
    QString date_begin;
    QString date_end;
    QString tour_gender_type;
    QString tour_net_type;
    QString tour_title;
    QVector<QCheckBox *> selected_teams;
};


void createPlayersTable(QStandardItemModel *model, QTableView *tableView,
                        QSqlDatabase *db, QVector<Player> &all_players,
                        QDialog *dialog);

void fontAdapter(QPushButton *btn);

void changeTableItemStyle(QTableWidget *table, const QColor color,
                          bool is_bold = false);


#endif // UTILS_H
