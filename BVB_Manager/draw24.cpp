#include "draw24.h"
#include "ui_draw24.h"
#include <QDebug>

Draw24::Draw24(QSqlDatabase &db, Tournament *tour, QWidget *parent)
    : DrawSchema(parent)
    , ui(new Ui::Draw24)
{
    ui->setupUi(this);

    this->setWindowTitle("24 teams double elimination");

    // connect to the database
    setDB(&db);

    // getting a tournament data
    setTournament(tour);

    // getting teams and filling schema

    // all 24 teams list
    QList<QPushButton *> all_teams{
        ui->team_1_btn, ui->team_2_btn, ui->team_3_btn, ui->team_4_btn,
        ui->team_5_btn, ui->team_6_btn, ui->team_7_btn, ui->team_8_btn,
        ui->team_9_btn, ui->team_10_btn, ui->team_11_btn, ui->team_12_btn,
        ui->team_13_btn, ui->team_14_btn, ui->team_15_btn, ui->team_16_btn,
        ui->team_17_btn, ui->team_18_btn, ui->team_19_btn, ui->team_20_btn,
        ui->team_21_btn, ui->team_22_btn, ui->team_23_btn, ui->team_24_btn
    };

    for(int i{0}; i < 24; ++i){

        // get players names
        auto startIndex = getTournament()->selected_teams[i]->text().indexOf('(');  // Find the first '('
        auto endIndex = getTournament()->selected_teams[i]->text().indexOf(')');

        // Check if both indices were found
        QString names;
        if (startIndex != -1 && endIndex != -1 && endIndex > startIndex) {
            // Extract the substring including parentheses
            names = getTournament()->selected_teams[i]->text().mid(startIndex, endIndex - startIndex + 1);
        }

        auto comma_index =
            getTournament()->selected_teams[i]->text().indexOf("(");

        QString team_name =
            getTournament()->selected_teams[i]->text().left(comma_index - 1);

        all_teams[i]->setFixedSize(static_cast<int>(Geometry::BtnWidth),
                             static_cast<int>(Geometry::BtnHeight));
        all_teams[i]->setDisabled(true);
        all_teams[i]->setText(team_name);
        fontAdapter(all_teams[i]);
        all_teams[i]->setStyleSheet(team_btn_style);
    }
}

Draw24::~Draw24()
{
    delete ui;
}
