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

        // make them unclickable
        btn->setDisabled(true);
    });

    // set style for all results button
    ui->allResultsBtn->setStyleSheet(ops_btn_style);

    // set style for final buttons
    ui->firstPlace->setStyleSheet(team_btn_style);
    ui->secondPlace->setStyleSheet(team_btn_style);
    ui->secondPlace->setDisabled(true);
    ui->thirdPlace->setStyleSheet(team_btn_style);
    ui->W59_btn_final->setDisabled(true);
    ui->W60_btn_final->setDisabled(true);
    ui->L59_btn->setDisabled(true);
    ui->L60_btn->setDisabled(true);

    constexpr int duration_mlsc{2000};
    // create animations for buttons W1 - W16
    createAnimation(std::make_pair(0, 16),
                    W_btns, animations_left_1, duration_of_anim);

    // create animations for buttons W17 - W24
    createAnimation(std::make_pair(16, 24),
                    W_btns, animations_left_2, duration_of_anim);

    // create animations for buttons W25 - W32
    createAnimation(std::make_pair(24, 32),
                    W_btns, animations_right_1, duration_of_anim);

    // create animations for buttons W33 - W40
    createAnimation(std::make_pair(32, 40),
                    W_btns, animations_right_2, duration_of_anim);


    // Signals & slots

    // W1 button
    connect(ui->W1_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_1_btn, ui->team_32_btn,
                   ui->W1_btn, ui->L1_btn,
                   nullptr, animations_left_1[0]);
    });

    // W2 button
    connect(ui->W2_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_17_btn, ui->team_16_btn,
                   ui->W2_btn, ui->L2_btn,
                   nullptr, animations_left_1[1]);
    });

    // W3 button
    connect(ui->W3_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_9_btn, ui->team_24_btn,
                   ui->W3_btn, ui->L3_btn,
                   nullptr, animations_left_1[2]);
    });

    // W4 button
    connect(ui->W4_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_25_btn, ui->team_8_btn,
                   ui->W4_btn, ui->L4_btn,
                   nullptr, animations_left_1[3]);
    });

    // W5 button
    connect(ui->W5_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_5_btn, ui->team_28_btn,
                   ui->W5_btn, ui->L5_btn,
                   nullptr, animations_left_1[4]);
    });

    // W6 button
    connect(ui->W6_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_21_btn, ui->team_12_btn,
                   ui->W6_btn, ui->L6_btn,
                   nullptr, animations_left_1[5]);
    });

    // W7 button
    connect(ui->W7_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_13_btn, ui->team_20_btn,
                   ui->W7_btn, ui->L7_btn,
                   nullptr, animations_left_1[6]);
    });

    // W8 button
    connect(ui->W8_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_29_btn, ui->team_4_btn,
                   ui->W8_btn, ui->L8_btn,
                   nullptr, animations_left_1[7]);
    });

    // W9 button
    connect(ui->W9_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_3_btn, ui->team_30_btn,
                   ui->W9_btn, ui->L9_btn,
                   nullptr, animations_left_1[8]);
    });

    // W10 button
    connect(ui->W10_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_19_btn, ui->team_14_btn,
                   ui->W10_btn, ui->L10_btn,
                   nullptr, animations_left_1[9]);
    });

    // W11 button
    connect(ui->W11_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_11_btn, ui->team_22_btn,
                   ui->W11_btn, ui->L11_btn,
                   nullptr, animations_left_1[10]);
    });

    // W12 button
    connect(ui->W12_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_27_btn, ui->team_6_btn,
                   ui->W12_btn, ui->L12_btn,
                   nullptr, animations_left_1[11]);
    });

    // W13 button
    connect(ui->W13_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_7_btn, ui->team_26_btn,
                   ui->W13_btn, ui->L13_btn,
                   nullptr, animations_left_1[12]);
    });

    // W14 button
    connect(ui->W14_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_23_btn, ui->team_10_btn,
                   ui->W14_btn, ui->L14_btn,
                   nullptr, animations_left_1[13]);
    });

    // W15 button
    connect(ui->W15_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_15_btn, ui->team_18_btn,
                   ui->W15_btn, ui->L15_btn,
                   nullptr, animations_left_1[14]);
    });

    // W16 button
    connect(ui->W16_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_31_btn, ui->team_2_btn,
                   ui->W16_btn, ui->L16_btn,
                   nullptr, animations_left_1[15]);
    });

    // W17 button
    connect(ui->W17_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W1_btn, ui->W2_btn,
                   ui->W17_btn, ui->L17_btn,
                   nullptr, animations_left_2[0]);
    });

    // W18 button
    connect(ui->W18_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W3_btn, ui->W4_btn,
                   ui->W18_btn, ui->L18_btn,
                   nullptr, animations_left_2[1]);
    });

    // W19 button
    connect(ui->W19_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W5_btn, ui->W6_btn,
                   ui->W19_btn, ui->L19_btn,
                   nullptr, animations_left_2[2]);
    });

    // W20 button
    connect(ui->W20_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W7_btn, ui->W8_btn,
                   ui->W20_btn, ui->L20_btn,
                   nullptr, animations_left_2[3]);
    });

    // W21 button
    connect(ui->W21_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W9_btn, ui->W10_btn,
                   ui->W21_btn, ui->L21_btn,
                   nullptr, animations_left_2[4]);
    });

    // W22 button
    connect(ui->W22_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W11_btn, ui->W12_btn,
                   ui->W22_btn, ui->L22_btn,
                   nullptr, animations_left_2[5]);
    });

    // W23 button
    connect(ui->W23_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W13_btn, ui->W14_btn,
                   ui->W23_btn, ui->L23_btn,
                   nullptr, animations_left_2[6]);
    });

    // W24 button
    connect(ui->W24_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W15_btn, ui->W16_btn,
                   ui->W24_btn, ui->L24_btn,
                   nullptr, animations_left_2[7]);
    });

    // W41 button
    connect(ui->W41_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W17_btn, ui->W18_btn,
                   ui->W41_btn, ui->L41_btn);
    });

    // W42 button
    connect(ui->W42_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W19_btn, ui->W20_btn,
                   ui->W42_btn, ui->L42_btn);
    });

    // W43 button
    connect(ui->W43_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W21_btn, ui->W22_btn,
                   ui->W43_btn, ui->L43_btn);
    });

    // W44 button
    connect(ui->W44_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W23_btn, ui->W24_btn,
                   ui->W44_btn, ui->L44_btn);
    });

    // W53 button
    connect(ui->W53_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W41_btn, ui->W42_btn,
                   ui->W53_btn, ui->L53_btn);
    });

    // W54 button
    connect(ui->W54_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W43_btn, ui->W44_btn,
                   ui->W54_btn, ui->L54_btn);
    });

    // right side

    // W25 button
    connect(ui->W25_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->L1_btn, ui->L2_btn,
                   ui->W25_btn, loser_btn,
                   nullptr, animations_right_1[0]);
    });

    // W26 button
    connect(ui->W26_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->L3_btn, ui->L4_btn,
                   ui->W26_btn, loser_btn,
                   nullptr, animations_right_1[1]);
    });

    // W27 button
    connect(ui->W27_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->L5_btn, ui->L6_btn,
                   ui->W27_btn, loser_btn,
                   nullptr, animations_right_1[2]);
    });

    // W28 button
    connect(ui->W28_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->L7_btn, ui->L8_btn,
                   ui->W28_btn, loser_btn,
                   nullptr, animations_right_1[3]);
    });

    // W29 button
    connect(ui->W29_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->L9_btn, ui->L10_btn,
                   ui->W29_btn, loser_btn,
                   nullptr, animations_right_1[4]);
    });

    // W30 button
    connect(ui->W30_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->L11_btn, ui->L12_btn,
                   ui->W30_btn, loser_btn,
                   nullptr, animations_right_1[5]);
    });

    // W31 button
    connect(ui->W31_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->L13_btn, ui->L14_btn,
                   ui->W31_btn, loser_btn,
                   nullptr, animations_right_1[6]);
    });

    // W32 button
    connect(ui->W32_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->L15_btn, ui->L16_btn,
                   ui->W32_btn, loser_btn,
                   nullptr, animations_right_1[7]);
    });

    // W33 button
    connect(ui->W33_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W25_btn, ui->L24_btn,
                   ui->W33_btn, loser_btn,
                   nullptr, animations_right_2[0]);
    });

    // W34 button
    connect(ui->W34_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W26_btn, ui->L23_btn,
                   ui->W34_btn, loser_btn,
                   nullptr, animations_right_2[1]);
    });

    // W35 button
    connect(ui->W35_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W27_btn, ui->L22_btn,
                   ui->W35_btn, loser_btn,
                   nullptr, animations_right_2[2]);
    });

    // W36 button
    connect(ui->W36_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W28_btn, ui->L21_btn,
                   ui->W36_btn, loser_btn,
                   nullptr, animations_right_2[3]);
    });

    // W37 button
    connect(ui->W37_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W29_btn, ui->L20_btn,
                   ui->W37_btn, loser_btn,
                   nullptr, animations_right_2[4]);
    });

    // W38 button
    connect(ui->W38_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W30_btn, ui->L19_btn,
                   ui->W38_btn, loser_btn,
                   nullptr, animations_right_2[5]);
    });

    // W39 button
    connect(ui->W39_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W31_btn, ui->L18_btn,
                   ui->W39_btn, loser_btn,
                   nullptr, animations_right_2[6]);
    });

    // W40 button
    connect(ui->W40_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W32_btn, ui->L17_btn,
                   ui->W40_btn, loser_btn,
                   nullptr, animations_right_2[7]);
    });

    // W45
    connect(ui->W45_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W33_btn, ui->W34_btn,
                   ui->W45_btn, loser_btn);
    });

    // W46
    connect(ui->W46_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W35_btn, ui->W36_btn,
                   ui->W46_btn, loser_btn);
    });

    // W47
    connect(ui->W47_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W37_btn, ui->W38_btn,
                   ui->W47_btn, loser_btn);
    });

    // W48
    connect(ui->W48_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W39_btn, ui->W40_btn,
                   ui->W48_btn, loser_btn);
    });

    // W49
    connect(ui->W49_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W45_btn, ui->L42_btn,
                   ui->W49_btn, loser_btn);
    });

    // W50
    connect(ui->W50_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W46_btn, ui->L41_btn,
                   ui->W50_btn, loser_btn);
    });

    // W51
    connect(ui->W51_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W47_btn, ui->L44_btn,
                   ui->W51_btn, loser_btn);
    });

    // W52
    connect(ui->W52_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W48_btn, ui->L43_btn,
                   ui->W52_btn, loser_btn);
    });

    // W55
    connect(ui->W55_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W49_btn, ui->W50_btn,
                   ui->W55_btn, loser_btn);
    });

    // W56
    connect(ui->W56_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W51_btn, ui->W52_btn,
                   ui->W56_btn, loser_btn);
    });

    // W57
    connect(ui->W57_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->L54_btn, ui->W55_btn,
                   ui->W57_btn, loser_btn);
    });

    // W58
    connect(ui->W58_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->L53_btn, ui->W56_btn,
                   ui->W58_btn, loser_btn);
    });

    // W59
    connect(ui->W59_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W53_btn, ui->W57_btn,
                   ui->W59_btn_final, ui->L59_btn);
    });

    // W60
    connect(ui->W60_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W54_btn, ui->W58_btn,
                   ui->W60_btn_final, ui->L60_btn);
    });

    //final buttons
    // golden medal match
    connect(ui->firstPlace, &QPushButton::clicked, this, [this](){
        click_game(ui->W59_btn_final, ui->W60_btn_final,
                   ui->firstPlace, ui->secondPlace);
    });

    // bronze medal match
    connect(ui->thirdPlace, &QPushButton::clicked, this, [this](){
        click_game(ui->L59_btn, ui->L60_btn,
                   ui->thirdPlace, loser_btn);
    });

    // left forward button >
    connect(ui->forward_left_1, &QPushButton::clicked, this, [this, W_btns, L_btns](){

        //make a list of W1 - W16 buttons
        QList<QPushButton *> W1_W16_btns = W_btns.mid(0, 16);

        // make a list of L1 - L16 buttons
        QList<QPushButton *> L1_L16_btns = L_btns.mid(0, 16);

        // make first round team buttons
        QList<QPushButton *> first_round_team_btns{
            ui->team_1_btn, ui->team_32_btn,   // 1st pair
            ui->team_17_btn, ui->team_16_btn,    // 2nd pair
            ui->team_9_btn, ui->team_24_btn,   // 3rd pair
            ui->team_25_btn, ui->team_8_btn,   // 4th pair
            ui->team_5_btn, ui->team_28_btn,   // 5th pair
            ui->team_21_btn, ui->team_12_btn,   // 6th pair
            ui->team_13_btn, ui->team_20_btn,   // 7th pair
            ui->team_29_btn, ui->team_4_btn,    // 8th pair
            ui->team_3_btn, ui->team_30_btn,   // 9th pair
            ui->team_19_btn, ui->team_14_btn,    // 10th pair
            ui->team_11_btn, ui->team_22_btn,   // 11th pair
            ui->team_27_btn, ui->team_6_btn,   // 12th pair
            ui->team_7_btn, ui->team_26_btn,   // 13th pair
            ui->team_23_btn, ui->team_10_btn,   // 14th pair
            ui->team_15_btn, ui->team_18_btn,   // 15th pair
            ui->team_31_btn, ui->team_2_btn    // 16th pair
        };

        moveTeams(first_round_team_btns, W1_W16_btns, L1_L16_btns, animations_left_1);
    });

    // left forward button >>
    connect(ui->forward_left_2, &QPushButton::clicked, this, [this, W_btns, L_btns](){

        //make a list of W17 - W24 buttons
        QList<QPushButton *> W17_W24_btns = W_btns.mid(16, 24);

        // make a list of L17 - L24 buttons
        QList<QPushButton *> L17_L24_btns = L_btns.mid(16, 24);

        // make second round team buttons
        QList<QPushButton *> second_round_team_btns = W_btns.mid(0, 16);

        moveTeams(second_round_team_btns, W17_W24_btns, L17_L24_btns, animations_left_2);
    });

    // right forward button >
    connect(ui->forward_right_1, &QPushButton::clicked, this, [this, W_btns, L_btns](){

        //make a list of W25 - W32 buttons
        QList<QPushButton *> W25_W32_btns = W_btns.mid(24, 32);

        // make first round play-off team buttons
        QList<QPushButton *> first_round_playoff_team_btns = L_btns.mid(0, 16);

        moveTeams(first_round_playoff_team_btns, W25_W32_btns,
                  loosers_btns, animations_right_1);
    });

    // right forward button >>
    connect(ui->forward_right_2, &QPushButton::clicked, this, [this, W_btns, L_btns](){

        //make a list of W33 - W40 buttons
        QList<QPushButton *> W33_W40_btns = W_btns.mid(32, 40);

        // make first round play-off team buttons
        QList<QPushButton *> second_round_playoff_team_btns = {
            ui->W25_btn, ui->L24_btn,
            ui->W26_btn, ui->L23_btn,
            ui->W27_btn, ui->L22_btn,
            ui->W28_btn, ui->L21_btn,
            ui->W29_btn, ui->L20_btn,
            ui->W30_btn, ui->L19_btn,
            ui->W31_btn, ui->L18_btn,
            ui->W32_btn, ui->L17_btn
        };

        moveTeams(second_round_playoff_team_btns, W33_W40_btns,
                  loosers_btns, animations_right_2);
    });

}

Draw32::~Draw32()
{
    delete ui;
}


/*
    Function simply show all games results
*/
void Draw32::on_allResultsBtn_clicked()
{
    showAllGamesResults();
}

