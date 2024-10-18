#include "draw24.h"
#include "ui_draw24.h"
#include <QDebug>
#include <QMessageBox>


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

    // filling team buttons with names and moreover resize them and set style
    fillTeamBtns(Net::TwentyFour, all_teams,
                 QSize(static_cast<int>(Geometry::BtnWidth),
                       static_cast<int>(Geometry::BtnHeight)));

    //collect and change style for control buttons

    QList<QPushButton *> control_btns{
        ui->forward_left_1, ui->forward_left_2, ui->forward_right_1, ui->forward_right_2
    };

    std::for_each(control_btns.begin(), control_btns.end(), [this](QPushButton *btn)->void{
        changeBtnStyle(btn, QSize(static_cast<int>(Geometry::BtnWidth),
                                  static_cast<int>(Geometry::BtnHeight)), ops_btn_style);
    });

    ui->allResultsBtn->setStyleSheet(ops_btn_style);

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
        ui->W43_btn_final, ui->W44_btn_final
    };

    // set style for W buttons
    std::for_each(W_btns.begin(), W_btns.end(), [this](QPushButton *btn)->void{
        changeBtnStyle(btn, QSize(static_cast<int>(Geometry::BtnWidthShort),
                                  static_cast<int>(Geometry::BtnHeight)), team_btn_style);
    });

    // create animations for butoons W1 - W8
    createAnimation(std::move(std::make_pair(0, 8)), W_btns, animations_left_1, 2000);

    // create animations for buttons W9 - W16
    createAnimation(std::move(std::make_pair(8, 16)), W_btns, animations_left_2, 2000);

    // create animations for buttons W17 - W24
    createAnimation(std::move(std::make_pair(17, 24)), W_btns, animations_right_1, 2000);

    // collect all L buttons
    QList<QPushButton *> L_btns{
        ui->L1_btn, ui->L2_btn, ui->L3_btn, ui->L4_btn, ui->L5_btn, ui->L6_btn,
        ui->L7_btn, ui->L8_btn, ui->L9_btn, ui->L10_btn, ui->L11_btn, ui->L12_btn,
        ui->L13_btn, ui->L14_btn, ui->L15_btn, ui->L16_btn,
        ui->L25_btn, ui->L26_btn, ui->L27_btn, ui->L28_btn,
        ui->L37_btn, ui->L38_btn,
        ui->L43_btn, ui->L44_btn
    };

    // set style for L buttons
    std::for_each(L_btns.begin(), L_btns.end(), [this](QPushButton *btn){
        changeBtnStyle(btn, QSize(static_cast<int>(Geometry::BtnWidthShort),
                                  static_cast<int>(Geometry::BtnHeight)), team_btn_style);
    });

    // Signals & slots

    // W1 button
    connect(ui->W1_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_16_btn, ui->team_17_btn,
                   ui->W1_btn, ui->L1_btn,
                   nullptr, animations_left_1[0]);
    });

    // W2 button
    connect(ui->W2_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_24_btn, ui->team_9_btn,
                   ui->W2_btn, ui->L2_btn,
                   nullptr, animations_left_1[1]);
    });

    // W3 button
    connect(ui->W3_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_12_btn, ui->team_21_btn,
                   ui->W3_btn, ui->L3_btn,
                   nullptr, animations_left_1[2]);
    });

    // W4 button
    connect(ui->W4_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_20_btn, ui->team_13_btn,
                   ui->W4_btn, ui->L4_btn,
                   nullptr, animations_left_1[3]);
    });

    // W5 button
    connect(ui->W5_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_14_btn, ui->team_19_btn,
                   ui->W5_btn, ui->L5_btn,
                   nullptr, animations_left_1[4]);
    });

    // W6 button
    connect(ui->W6_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_22_btn, ui->team_11_btn,
                   ui->W6_btn, ui->L6_btn,
                   nullptr, animations_left_1[5]);
    });

    // W7 button
    connect(ui->W7_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_10_btn, ui->team_23_btn,
                   ui->W7_btn, ui->L7_btn,
                   nullptr, animations_left_1[6]);
    });

    // W8 button
    connect(ui->W8_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_18_btn, ui->team_15_btn,
                   ui->W8_btn, ui->L8_btn,
                   nullptr, animations_left_1[7]);
    });

    // W9 button
    connect(ui->W9_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_1_btn, ui->W1_btn,
                   ui->W9_btn, ui->L9_btn,
                   nullptr, animations_left_2[0]);
    });

    // W10 button
    connect(ui->W10_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W2_btn, ui->team_8_btn,
                   ui->W10_btn, ui->L10_btn,
                   nullptr, animations_left_2[1]);
    });

    // W11 button
    connect(ui->W11_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_5_btn, ui->W3_btn,
                   ui->W11_btn, ui->L11_btn,
                   nullptr, animations_left_2[2]);
    });

    // W12 button
    connect(ui->W12_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W4_btn, ui->team_4_btn,
                   ui->W12_btn, ui->L12_btn,
                   nullptr, animations_left_2[3]);
    });

    // W13 button
    connect(ui->W13_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_3_btn, ui->W5_btn,
                   ui->W13_btn, ui->L13_btn,
                   nullptr, animations_left_2[4]);
    });

    // W14 button
    connect(ui->W14_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W6_btn, ui->team_6_btn,
                   ui->W14_btn, ui->L14_btn,
                   nullptr, animations_left_2[5]);
    });

    // W15 button
    connect(ui->W15_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->team_7_btn, ui->W7_btn,
                   ui->W15_btn, ui->L15_btn,
                   nullptr, animations_left_2[6]);
    });

    // W16 button
    connect(ui->W16_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W8_btn, ui->team_2_btn,
                   ui->W16_btn, ui->L16_btn,
                   nullptr, animations_left_2[7]);
    });

    // W25 button
    connect(ui->W25_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W9_btn, ui->W10_btn,
                   ui->W25_btn, ui->L25_btn);
    });

    // W26 button
    connect(ui->W26_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W11_btn, ui->W12_btn,
                   ui->W26_btn, ui->L26_btn);
    });

    // W27 button
    connect(ui->W27_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W13_btn, ui->W14_btn,
                   ui->W27_btn, ui->L27_btn);
    });

    // W28 button
    connect(ui->W28_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W15_btn, ui->W16_btn,
                   ui->W28_btn, ui->L28_btn);
    });

    // W37 button
    connect(ui->W37_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W25_btn, ui->W26_btn,
                   ui->W37_btn, ui->L37_btn);
    });

    // W38 button
    connect(ui->W38_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->W27_btn, ui->W28_btn,
                   ui->W38_btn, ui->L38_btn);
    });

    // right side (loosers)
    // W24 button
    connect(ui->W24_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->L16_btn, ui->L1_btn,
                   ui->W24_btn, loser_btn, nullptr, animations_right_1[0]);
    });

    // W23 button
    connect(ui->W23_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->L2_btn, ui->L15_btn,
                   ui->W23_btn, loser_btn, nullptr, animations_right_1[1]);
    });

    // W22 button
    connect(ui->W22_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->L14_btn, ui->L3_btn,
                   ui->W22_btn, loser_btn, nullptr, animations_right_1[2]);
    });

    // W21 button
    connect(ui->W21_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->L4_btn, ui->L13_btn,
                   ui->W21_btn, loser_btn, nullptr, animations_right_1[3]);
    });

    // W20 button
    connect(ui->W20_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->L12_btn, ui->L5_btn,
                   ui->W20_btn, loser_btn, nullptr, animations_right_1[4]);
    });

    // W19 button
    connect(ui->W19_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->L6_btn, ui->L11_btn,
                   ui->W19_btn, loser_btn, nullptr, animations_right_1[5]);
    });

    // W18 button
    connect(ui->W18_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->L10_btn, ui->L7_btn,
                   ui->W18_btn, loser_btn, nullptr, animations_right_1[6]);
    });

    // W17 button
    connect(ui->W17_btn, &QPushButton::clicked, this, [this](){
        click_game(ui->L8_btn, ui->L9_btn,
                   ui->W17_btn, loser_btn, nullptr, animations_right_1[7]);
    });


    // left forward button >
    connect(ui->forward_left_1, &QPushButton::clicked, this, [this, W_btns, L_btns](){
        //make a list of W1 - W8 buttons
        QList<QPushButton *> W1_W8_btns = W_btns.mid(0, 8);

        // make a list of L1 - L8 buttons
        QList<QPushButton *> L1_L8_btns = L_btns.mid(0, 8);

        // make first round team buttons
        QList<QPushButton *> first_round_team_btns{
            ui->team_16_btn, ui->team_17_btn,   // 1st pair
            ui->team_24_btn, ui->team_9_btn,    // 2nd pair
            ui->team_12_btn, ui->team_21_btn,   // 3rd pair
            ui->team_20_btn, ui->team_13_btn,   // 4th pair
            ui->team_14_btn, ui->team_19_btn,   // 5th pair
            ui->team_22_btn, ui->team_11_btn,   // 6th pair
            ui->team_10_btn, ui->team_23_btn,   // 7th pair
            ui->team_18_btn, ui->team_15_btn    // 8th pair
        };

        moveTeams(first_round_team_btns, W1_W8_btns, L1_L8_btns, animations_left_1);
    });

    // left forward button >>
    connect(ui->forward_left_2, &QPushButton::clicked, this, [this, W_btns, L_btns](){
        //make a list of W9 - W16 buttons
        QList<QPushButton *> W9_W16_btns = W_btns.mid(8, 16);

        // make a list of L1 - L8 buttons
        QList<QPushButton *> L9_L16_btns = L_btns.mid(8, 16);

        // make second round team buttons
        QList<QPushButton *> sec_round_team_btns{
            ui->team_1_btn, ui->W1_btn,     // 1st pair
            ui->W2_btn, ui->team_8_btn,     // 2nd pair
            ui->team_5_btn, ui->W3_btn,     // 3rd pair
            ui->W4_btn, ui->team_4_btn,     // 4th pair
            ui->team_3_btn, ui->W5_btn,     // 5th pair
            ui->W6_btn, ui->team_6_btn,     // 6th pair
            ui->team_7_btn, ui->W7_btn,     // 7th pair
            ui->W8_btn, ui->team_2_btn      // 8th pair
        };

        moveTeams(sec_round_team_btns, W9_W16_btns, L9_L16_btns, animations_left_2);
    });

    // right forward button >
    connect(ui->forward_right_1, &QPushButton::clicked, this, [this, W_btns, L_btns](){

        //make a list of W17 - W24 buttons
        QList<QPushButton *> W17_W24_btns = W_btns.mid(16, 24);
        qDebug() << "Step 1";

        // make a list of 8 loosers buttons
        QList<QPushButton *> loosers_btns(8, nullptr);
        qDebug() << "Step 2";

        // make first round team buttons
        QList<QPushButton *> play_off_round_btns{
            ui->L16_btn, ui->L1_btn,   // 1st pair
            ui->L2_btn, ui->L15_btn,    // 2nd pair
            ui->L14_btn, ui->L3_btn,   // 3rd pair
            ui->L4_btn, ui->L13_btn,   // 4th pair
            ui->L12_btn, ui->L5_btn,   // 5th pair
            ui->L6_btn, ui->L11_btn,   // 6th pair
            ui->L10_btn, ui->L7_btn,   // 7th pair
            ui->L8_btn, ui->L9_btn,    // 8th pair
        };
        qDebug() << "Step 3";

        // CRASHES HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        moveTeams(play_off_round_btns, W17_W24_btns, loosers_btns, animations_right_1);
        qDebug() << "Step 4";
    });
}


Draw24::~Draw24()
{
    delete ui;
}

void Draw24::on_allResultsBtn_clicked()
{
    QString result;
    for(auto it{getAllGamesResults()->begin()}; it != getAllGamesResults()->end();++it){
        result += *it + "\n";
    }

    QMessageBox::information(this, "Games results", std::move(result));

}

