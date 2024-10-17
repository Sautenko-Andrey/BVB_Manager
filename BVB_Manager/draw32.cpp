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

    // collect all W buttons
    QList<QPushButton *> W_btns{
        ui->W1_btn,ui->W2_btn,ui->W3_btn,ui->W4_btn,
        ui->W5_btn,ui->W6_btn,ui->W7_btn,ui->W8_btn,
        ui->W9_btn,ui->W10_btn,ui->W11_btn,ui->W12_btn,
        ui->W13_btn,ui->W14_btn,ui->W15_btn,ui->W16_btn,
        ui->W17_btn,ui->W18_btn,ui->W19_btn,ui->W20_btn,
        ui->W21_btn,ui->W22_btn,ui->W23_btn,ui->W24_btn,
        ui->W25_btn,ui->W26_btn,ui->W27_btn,ui->W28_btn,
        ui->W29_btn,ui->W30_btn,ui->W31_btn,ui->W32_btn,
        ui->W33_btn,ui->W34_btn,ui->W35_btn,ui->W36_btn,
        ui->W37_btn,ui->W38_btn,ui->W39_btn,ui->W40_btn,
        ui->W41_btn,ui->W42_btn,ui->W43_btn,ui->W44_btn,
        ui->W45_btn,ui->W46_btn,ui->W47_btn,ui->W48_btn,
        ui->W49_btn,ui->W50_btn,ui->W51_btn,ui->W52_btn,
        ui->W53_btn,ui->W54_btn,ui->W55_btn,ui->W56_btn,
        ui->W57_btn,ui->W58_btn,ui->W59_btn,ui->W60_btn,
        ui->W59_btn_final, ui->W60_btn_final
    };

    // set style for W buttons
    std::for_each(W_btns.begin(), W_btns.end(), [this](QPushButton *btn)->void{
        changeBtnStyle(btn, QSize(static_cast<int>(Geometry::BtnWidthShort),
                                  static_cast<int>(Geometry::BtnHeight)), team_btn_style);
    });

    // create animations for butoons W1 - W16
    createAnimation(std::move(std::make_pair(0, 16)), W_btns, animations_left_1, 1200);

    // create animations for buttons W17 - W24
    createAnimation(std::move(std::make_pair(7, 24)), W_btns, animations_left_2, 1200);

    // collect all L buttons
    QList<QPushButton *> L_btns{
        ui->L1_btn, ui->L2_btn, ui->L3_btn, ui->L4_btn, ui->L5_btn, ui->L6_btn,
        ui->L7_btn, ui->L8_btn, ui->L9_btn, ui->L10_btn, ui->L11_btn, ui->L12_btn,
        ui->L13_btn, ui->L14_btn, ui->L15_btn, ui->L16_btn, ui->L24_btn, ui->L23_btn,
        ui->L42_btn, ui->L54_btn, ui->L22_btn, ui->L21_btn, ui->L41_btn, ui->L20_btn,
        ui->L19_btn, ui->L44_btn, ui->L53_btn, ui->L18_btn, ui->L17_btn, ui->L43_btn,
        ui->L59_btn, ui->L60_btn
    };

    // set style for L buttons
    std::for_each(L_btns.begin(), L_btns.end(), [this](QPushButton *btn){
        changeBtnStyle(btn, QSize(static_cast<int>(Geometry::BtnWidthShort),
                                  static_cast<int>(Geometry::BtnHeight)), team_btn_style);
    });
}

Draw32::~Draw32()
{
    delete ui;
}
