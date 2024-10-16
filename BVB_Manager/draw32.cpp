#include "draw32.h"
#include "ui_draw32.h"

Draw32::Draw32(QSqlDatabase &db, Tournament *tour, QWidget *parent)
    : DrawSchema(parent)
    , ui(new Ui::Draw32)
{
    ui->setupUi(this);

    this->setWindowTitle("32 teams double elimination");

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
        ui->team_21_btn, ui->team_22_btn, ui->team_23_btn, ui->team_24_btn,
        ui->team_25_btn, ui->team_26_btn, ui->team_27_btn, ui->team_28_btn,
        ui->team_29_btn, ui->team_30_btn, ui->team_31_btn, ui->team_32_btn
    };

    // filling team buttons with names and moreover resize them and set style
    fillTeamBtns(Net::ThirtyTwo, all_teams,
                 QSize(static_cast<int>(Geometry::BtnWidth),
                       static_cast<int>(Geometry::BtnHeight)));

    //collect and change style for control buttons

    QList<QPushButton *> control_btns{
        ui->forward_left_1, ui->forward_left_2, ui->forward_left_3,
        ui->forward_right_1, ui->forward_right_2, ui->forward_right_3
    };

    std::for_each(control_btns.begin(), control_btns.end(), [this](QPushButton *btn)->void{
        changeBtnStyle(btn, QSize(static_cast<int>(Geometry::BtnWidth),
                                  static_cast<int>(Geometry::BtnHeight)), ops_btn_style);
    });
}

Draw32::~Draw32()
{
    delete ui;
}
