#include "tourdraw.h"
#include "ui_tourdraw.h"
#include <QPushButton>
#include <QPainter>
#include <QTextDocument>
#include <QLine>
//#include <QList>
#include <QVector>
#include <QMessageBox>
#include <QStringList>

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

TourDraw::TourDraw(QSqlDatabase &database,
                   Net net_type,
                   Tournament completed_tournament,
                   QWidget *parent)
    : QDialog(parent)
    , draw_type{net_type}
    , tournament{completed_tournament}
    , ui(new Ui::TourDraw)
{
    ui->setupUi(this);

    // pointer on database
    db = &database;

    loser_btn = new QPushButton(this);
    loser_btn->hide();

    // initialize all W buttons with nullptr
    constexpr int total_w_buttons{28};
    for(int i{0}; i < total_w_buttons; ++i){
        W_buttons.append(nullptr);
    }

    // drawing a tournament schema

    // 16 teams buttons (1st column) on the left side
    QList<int> rank_baskets{
        0, 15, 8, 7, 4, 11, 12, 3, 2, 13, 10, 5, 6, 9, 14, 1
    };
    for(int i{0}, y{static_cast<int>(Geometry::coordYStartBtn)};
        i < static_cast<int>(net_type); ++i){

        // drawing a teams buttons
        QPushButton *btn = new QPushButton(this);

        first_round_team_btns.push_back(btn);

        btn->setGeometry(QRect(static_cast<int>(Geometry::coordXStartBtn), y,
                               static_cast<int>(Geometry::BtnWidth),
                               static_cast<int>(Geometry::BtnHeight)));

        // getting team's name
        auto comma_index =
            tournament.selected_teams[rank_baskets[i]]->text().indexOf("(");

        QString team_name =
            tournament.selected_teams[rank_baskets[i]]->text().left(comma_index - 1);

        btn->setText(team_name);

        btn->setDisabled(true);

        // change button's font if necessary
        fontAdapter(btn);

        y += static_cast<int>(Geometry::Step);
    }

    // 8 teams buttons (2nd column) for the left side and the same for the right
    for(int i{0}, j{8}, y{32}, y2{40};
        i < static_cast<int>(draw_type) / 2; ++i){

        // left side(winners)--------------------------------------------------------------
        QPushButton *btn_left = new QPushButton(this);
        btn_left->setGeometry(QRect(131, y,
                               static_cast<int>(Geometry::BtnWidth),
                               static_cast<int>(Geometry::BtnHeight)));

        btn_left->setText("W " + QString::number(i + 1));

        // change button's font if necessary
        fontAdapter(btn_left);

        // save pointer on the button
        W_buttons[i] = btn_left;
        //------------------------------------------------------------------------

        // right side(loosers)-------------------------------------------------------------
        QPushButton *btn_right = new QPushButton(this);
        btn_right->setGeometry(QRect(1164, y,
                                    static_cast<int>(Geometry::BtnWidth),
                                    static_cast<int>(Geometry::BtnHeight)));

        btn_right->setDisabled(true);
        btn_right->setText("L " + QString::number(j));

        // change button's font if necessary
        fontAdapter(btn_right);

        L1_L12_buttons.push_front(btn_right);
        --j;
        //------------------------------------------------------------------------

        y += static_cast<int>(Geometry::Step) * 2;

        //--------------------------------------------------------------------------
        //drawing 8 game results buttons
        drawGameResultBtn(45, y2);
        y2 += 88;
    }


    // 4 teams buttons (3rd column) for both sides
    const QStringList loosers_labels{"L10", "L9", "L12", "L11"};

    for(int i{0}, y{79}, y2{80}, y3{145}, y4{113};
        i < static_cast<int>(draw_type) / 4; ++i){

        // left side (winners 3rd column)------------------------------------------------
        QPushButton *btn_left = new QPushButton(this);
        btn_left->setGeometry(QRect(252, y,
                               static_cast<int>(Geometry::BtnWidth),
                               static_cast<int>(Geometry::BtnHeight)));

        btn_left->setText("W " + QString::number(i + 9));

        // change button's font if necessary
        fontAdapter(btn_left);

        W_buttons[i + 8] = btn_left;
        y += static_cast<int>(Geometry::Step) * 4;
        //----------------------------------------------------------------------

        // right side (loosers 2nd column winners)----------------------------------------
        QPushButton *btn_right_1 = new QPushButton(this);
        btn_right_1->setGeometry(QRect(1042, y2,
                                 static_cast<int>(Geometry::BtnWidth),
                                 static_cast<int>(Geometry::BtnHeight)));

        btn_right_1->setText("W " + QString::number(i + 13));

        W_buttons[i + 12] = btn_right_1;

        // change button's font if necessary
        fontAdapter(btn_right_1);

        y2 += static_cast<int>(Geometry::Step) * 4;
        //------------------------------------------------------------------------

        // right side (loosers 2nd column loosers)------------------------------------
        QPushButton *btn_right_2 = new QPushButton(this);
        btn_right_2->setGeometry(QRect(1042, y3,
                                       static_cast<int>(Geometry::BtnWidth),
                                       static_cast<int>(Geometry::BtnHeight)));

        btn_right_2->setText(loosers_labels[i]);

        // change button's font if necessary
        fontAdapter(btn_right_2);

        L1_L12_buttons.push_back(btn_right_2);
        y3 += 177;
        //----------------------------------------------------------------------------

        // loosers buttons right side (3 column)-------------------------------------
        QPushButton *looser_btn_3c = new QPushButton(this);
        looser_btn_3c->setGeometry(QRect(919, y4,
                                         static_cast<int>(Geometry::BtnWidth),
                                         static_cast<int>(Geometry::BtnHeight)));

        looser_btn_3c->setText("W " + QString::number(i + 17));

        W_buttons[i + 16] = looser_btn_3c;

        // change button's font if necessary
        fontAdapter(looser_btn_3c);

        y4 += 177;
        //---------------------------------------------------------------------------
    }

    // buttons (final part)
    for(int i{0}, y{225}, y2{190}, y3{113}, y4{150}, y5{560}, y6{162}, y7{293}, j{22};
        i < static_cast<int>(draw_type) / 8; ++i){

        // left side 2 buttons 5 column-------------------------------------------
        QPushButton *btn = new QPushButton(this);
        btn->setGeometry(QRect(static_cast<int>(Geometry::coordXStartBtn) +
                               static_cast<int>(Geometry::StepX) * 4,
                               y,
                               static_cast<int>(Geometry::BtnWidth),
                               static_cast<int>(Geometry::BtnHeight)));

        btn->setText("W " + QString::number(i + 27));

        W_buttons[i + 26] = btn;

        // change button's font if necessary
        fontAdapter(btn);

        y += static_cast<int>(Geometry::Step) * 5;
        //-----------------------------------------------------------------------

        // 3d place battle buttons-----------------------------------------------
        QPushButton *btn_bronze_match = new QPushButton(this);
        btn_bronze_match->setGeometry(QRect(494, y5,
                               static_cast<int>(Geometry::BtnWidth),
                               static_cast<int>(Geometry::BtnHeight)));

        btn_bronze_match->setText("L " + QString::number(i + 27));
        L27_L28_buttons.append(btn_bronze_match);

        // change button's font if necessary
        fontAdapter(btn_bronze_match);

        btn_bronze_match->setDisabled(true);
        y5 += static_cast<int>(Geometry::Step) * 2;
        //-----------------------------------------------------------------------

        // right side 2 buttons (4th column)--------------------------------------
        QPushButton *btn_right = new QPushButton(this);
        btn_right->setGeometry(QRect(796, y2,
                                        static_cast<int>(Geometry::BtnWidth),
                                        static_cast<int>(Geometry::BtnHeight)));

        btn_right->setText("W " + QString::number(i + 23));

        W_buttons[i + 22] = btn_right;

        // change button's font if necessary
        fontAdapter(btn_right);

        y2 += 355;
        //------------------------------------------------------------------------

        // right side 2 buttons (4th column from winners)-------------------------
        QPushButton *btn_right_2 = new QPushButton(this);
        btn_right_2->setGeometry(QRect(796, y3,
                                        static_cast<int>(Geometry::BtnWidth),
                                        static_cast<int>(Geometry::BtnHeight)));

        btn_right_2->setText("L " + QString::number(j));
        btn_right_2->setDisabled(true);

        L22_L21_buttons.append(btn_right_2);

        // change button's font if necessary
        fontAdapter(btn_right_2);


        y3 += 355;
        --j;
        //------------------------------------------------------------------------

        // two loosers team for semifinal--------------------------------------------
        QPushButton *btn_right_3 = new QPushButton(this);
        btn_right_3->setGeometry(QRect(675, y4,
                                       static_cast<int>(Geometry::BtnWidth),
                                       static_cast<int>(Geometry::BtnHeight)));

        btn_right_3->setText("W " + QString::number(i + 25));

        W_buttons[i + 24] = btn_right_3;

        // change button's font if necessary
        fontAdapter(btn_right_3);

        y4 += 360;
        //--------------------------------------------------------------------------

        // 2 win buttons(semifinal)-------------------------------------------------
        QPushButton *win_btn = new QPushButton(this);
        win_btn->setGeometry(QRect(373, y6,
                                   static_cast<int>(Geometry::BtnWidth),
                                   static_cast<int>(Geometry::BtnHeight)));

        win_btn->setText("W " + QString::number(i + 21));

        W_buttons[i + 20] = win_btn;

        // change button's font if necessary
        fontAdapter(win_btn);

        y6 += static_cast<int>(Geometry::Step) * 8;
        //--------------------------------------------------------------------------

        //2 loosers buttons(semifinal)-----------------------------------------------
        QPushButton *looser_btn = new QPushButton(this);
        looser_btn->setGeometry(QRect(373, y7,
                                      static_cast<int>(Geometry::BtnWidth),
                                      static_cast<int>(Geometry::BtnHeight)));

        looser_btn->setText("W " + QString::number(i + 25));
        W25_W26_looser_buttons.append(looser_btn);
        looser_btn->setDisabled(true);

        // change button's font if necessary
        fontAdapter(looser_btn);

        y7 += 95;
        //-----------------------------------------------------------------------------
    }

    // 3 team buttons (1st, 2nd, 3rd places)
    for(int i{0}, y{285}; i < 3; ++i){
        QPushButton *btn = new QPushButton(this);

        btn->setGeometry(QRect(615, y,
                               static_cast<int>(Geometry::BtnWidth),
                               static_cast<int>(Geometry::BtnHeight)));

        if(i == 0){
            btn->setText("1st place");
            final_silver_bronze_buttons.append(btn);
        }
        else if(i == 1){
            btn->setText("2nd place");
            final_silver_bronze_buttons.append(btn);
            btn->setDisabled(true);
        }
        else{
            btn->setText("3rd place");
            final_silver_bronze_buttons.append(btn);
        }

        y += 50;

        // change button's font if necessary
        fontAdapter(btn);
    }

    // Signals & slots

    //W1
    connect(W_buttons[0], &QPushButton::clicked, this, [this](){
        click_game(first_round_team_btns[0], first_round_team_btns[1],
                W_buttons[0], L1_L12_buttons[0]);
    });

    //W2
    connect(W_buttons[1], &QPushButton::clicked, this, [this](){
        click_game(first_round_team_btns[2], first_round_team_btns[3],
                W_buttons[1], L1_L12_buttons[1]);
    });

    //W3
    connect(W_buttons[2], &QPushButton::clicked, this, [this](){
        click_game(first_round_team_btns[4], first_round_team_btns[5],
                W_buttons[2], L1_L12_buttons[2]);
    });

    //W4
    connect(W_buttons[3], &QPushButton::clicked, this, [this](){
        click_game(first_round_team_btns[6], first_round_team_btns[7],
                W_buttons[3], L1_L12_buttons[3]);
    });

    //W5
    connect(W_buttons[4], &QPushButton::clicked, this, [this](){
        click_game(first_round_team_btns[8], first_round_team_btns[9],
                W_buttons[4],L1_L12_buttons[4]);
    });

    //W6
    connect(W_buttons[5], &QPushButton::clicked, this, [this](){
        click_game(first_round_team_btns[10], first_round_team_btns[11],
                W_buttons[5], L1_L12_buttons[5]);
    });

    //W7
    connect(W_buttons[6], &QPushButton::clicked, this, [this](){
        click_game(first_round_team_btns[12], first_round_team_btns[13],
                W_buttons[6], L1_L12_buttons[6]);
    });

    //W8
    connect(W_buttons[7], &QPushButton::clicked, this, [this](){
        click_game(first_round_team_btns[14], first_round_team_btns[15],
                W_buttons[7], L1_L12_buttons[7]);
    });

    //W9
    connect(W_buttons[8], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[0], W_buttons[1],
                W_buttons[8], L1_L12_buttons[9]);
    });

    //W10
    connect(W_buttons[9], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[2], W_buttons[3],
                W_buttons[9], L1_L12_buttons[8]);
    });

    //W11
    connect(W_buttons[10], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[4], W_buttons[5],
                W_buttons[10], L1_L12_buttons[11]);
    });

    //W12
    connect(W_buttons[11], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[6], W_buttons[7],
                W_buttons[11], L1_L12_buttons[10]);
    });

    //W13
    connect(W_buttons[12], &QPushButton::clicked, this, [this](){
        click_game(L1_L12_buttons[7], L1_L12_buttons[6],
                W_buttons[12], loser_btn);
    });

    //W14
    connect(W_buttons[13], &QPushButton::clicked, this, [this](){
        click_game(L1_L12_buttons[5], L1_L12_buttons[4],
                W_buttons[13], loser_btn);
    });

    //W15
    connect(W_buttons[14], &QPushButton::clicked, this, [this](){
        click_game(L1_L12_buttons[3], L1_L12_buttons[2],
                W_buttons[14], loser_btn);
    });

    //W16
    connect(W_buttons[15], &QPushButton::clicked, this, [this](){
        click_game(L1_L12_buttons[1], L1_L12_buttons[0],
                W_buttons[15], loser_btn);
    });

    //W17
    connect(W_buttons[16], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[12], L1_L12_buttons[8],
                W_buttons[16], loser_btn);
    });

    //W18
    connect(W_buttons[17], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[13], L1_L12_buttons[9],
                W_buttons[17], loser_btn);
    });

    //W19
    connect(W_buttons[18], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[14], L1_L12_buttons[10],
                W_buttons[18], loser_btn);
    });

    //W20
    connect(W_buttons[19], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[15], L1_L12_buttons[11],
                W_buttons[19], loser_btn);
    });

    //W21
    connect(W_buttons[20], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[8], W_buttons[9],
                W_buttons[20], L22_L21_buttons[1]);
    });

    //W22
    connect(W_buttons[21], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[10], W_buttons[11],
                W_buttons[21], L22_L21_buttons[0]);
    });

    //W23
    connect(W_buttons[22], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[16], W_buttons[17],
                W_buttons[22], loser_btn);
    });

    //W24
    connect(W_buttons[23], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[18], W_buttons[19],
                W_buttons[23], loser_btn);
    });

    //W25
    connect(W_buttons[24], &QPushButton::clicked, this, [this](){
        click_game(L22_L21_buttons[0], W_buttons[22],
                W25_W26_looser_buttons[0], loser_btn);
    });

    //W26
    connect(W_buttons[25], &QPushButton::clicked, this, [this](){
        click_game(L22_L21_buttons[1], W_buttons[23],
                W25_W26_looser_buttons[1], loser_btn);
    });

    //W27
    connect(W_buttons[26], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[20], W25_W26_looser_buttons[0],
                W_buttons[26], L27_L28_buttons[0]);
    });

    //W28
    connect(W_buttons[27], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[21], W25_W26_looser_buttons[1],
                W_buttons[27], L27_L28_buttons[1]);
    });

    // bronze and golden medals matches
    // 3rd place
    connect(final_silver_bronze_buttons[2], &QPushButton::clicked, this, [this](){
        click_game(L27_L28_buttons[0], L27_L28_buttons[1],
                final_silver_bronze_buttons[2], loser_btn);
    });

    // final
    connect(final_silver_bronze_buttons[0], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[26], W_buttons[27],
                final_silver_bronze_buttons[0], final_silver_bronze_buttons[1]);
    });
}

TourDraw::~TourDraw()
{
    delete ui;
}

void TourDraw::fontAdapter(QPushButton *btn){

    constexpr int max_symbols_amount = 24;

    if(btn->text().size() >= max_symbols_amount){
        btn->setFont(QFont("Ubuntu", 6));
    }
    else if(btn->text().size() > max_symbols_amount / 2 + 1 &&
            btn->text().size() < max_symbols_amount){
        btn->setFont(QFont("Ubuntu", 9));
    }
}

void TourDraw::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // drawing a tournament title
    painter.setFont(QFont("Times", 15));
    painter.drawText(QPoint(400, 20), tournament.tour_title +
                                      " ( " +
                                      tournament.date_begin +
                                      " - " +
                                      tournament.date_end +
                                      ") MAIN DRAW ( " +
                                      tournament.tour_gender_type +
                                      " )");

    // final text
    painter.setFont(QFont("Times", 12));
    painter.drawText(QPoint(505, 340), "Final match");
    // bronze medal text
    painter.drawText(QPoint(505, 615), "3rd place match");

    // drawing 16 horizonthal lines ( 1st column )
    for(int i{0}, x{125}, y{26}; i < static_cast<int>(draw_type); ++i){

        painter.drawLine(QPoint(x, y), QPoint(x + 5, y));
        y += static_cast<int>(Geometry::Step);
    }

    // drawing 8 horizontal lines ( 2nd column left side ) and same for the right
    for(int i{0}, x1{245}, x2{1158}, y{47}; i < static_cast<int>(draw_type) / 2; ++i){

        // left side (2nd columnn)--------------------------
        painter.drawLine(QPoint(x1, y), QPoint(x1 + 5, y));
        //-------------------------------------------------

        // right side (2nd column)-------------------------
        painter.drawLine(QPoint(x2, y), QPoint(x2 + 5, y));
        //-------------------------------------------------

        y += static_cast<int>(Geometry::Step) * 2;
    }

    // drawing 4 horizontal lines (winners 3rd column and loosers 2nd column)
    for(int i{0}, x1{365}, x2{1036}, x3{913}, y{90}, y2{92}, y3{157}, y4{126};
        i < static_cast<int>(draw_type) / 4; ++i){

        // left side----------------------------------------
        painter.drawLine(QPoint(x1, y), QPoint(x1 + 5, y));
        //----------------------------------------------------

        // right side----------------------------------------
        painter.drawLine(QPoint(x2, y2), QPoint(x2 + 5, y2));
        painter.drawLine(QPoint(x2, y3), QPoint(x2 + 5, y3));
        painter.drawLine(QPoint(x3, y4), QPoint(x3 + 5, y4));
        //----------------------------------------------------

        y += static_cast<int>(Geometry::Step) * 4;
        y2 += static_cast<int>(Geometry::Step) * 4;
        y3 += static_cast<int>(Geometry::Step) * 4;
        y4 += static_cast<int>(Geometry::Step) * 4;
    }

    // drawing 4 horizontal lines ( 4 column )
    for(int i{0}, x1{487}, x2{608}, x3{607}, x4{790},
        y1{173}, y2{300}, y3{570}, y4{235}, y5{125}, y6{201};
        i < static_cast<int>(draw_type) / 8; ++i){

            painter.drawLine(QPoint(x1, y1), QPoint(x1 + 5, y1));
            y1 += static_cast<int>(Geometry::Step) * 8;

            painter.drawLine(QPoint(x1, y2), QPoint(x1 + 5, y2));
            y2 += 98;
            //---------------------------------------------------

            // 2 horizontal 3rd place lines---------------------
            painter.drawLine(QPoint(x2, y3), QPoint(x2 + 5, y3));
            y3 += 89;
            //---------------------------------------------------

            //left side 2 horizontal lines ( semifinals column )
            painter.drawLine(QPoint(x3, y4), QPoint(x3 + 5, y4));
            y4 += static_cast<int>(Geometry::Step) * 5;
            //-------------------------------------------------

            // right side quarter finals 4 lines--------------
            painter.drawLine(QPoint(x4, y5), QPoint(x4 + 5, y5));
            painter.drawLine(QPoint(x4, y6), QPoint(x4 + 5, y6));
            y5 += 355;
            y6 += 355;
            //-------------------------------------------------
    }

    // drawing 8 vertical lines ( 1st column left side)
    for(int i{0}, x{130}, y1{26}, y2{70}; i < static_cast<int>(draw_type) / 2; ++i){

        painter.drawLine(QPoint(x, y1), QPoint(x, y2));

        y1 += static_cast<int>(Geometry::Step) * 2;
        y2 += static_cast<int>(Geometry::Step) * 2;
    }

    // drawing 4 vertical lines ( 2nd column ) for both sides
    for(int i{0}, x1{250}, x2{1157}, x3{1035}, y1{47}, y2{135}, y3{92}, y4{157};
        i < static_cast<int>(draw_type) / 4; ++i){

        // left side(winners)------------------------------------------------------
        painter.drawLine(QPoint(x1, y1), QPoint(x1, y2));
        //-------------------------------------------------------------------------

        // right side (loosers)-----------------------------------------------------
        painter.drawLine(QPoint(x2, y1), QPoint(x2, y2));
        //--------------------------------------------------------------------------

        //right side (winners 2 column)---------------------------------------------
        painter.drawLine(QPoint(x3, y3), QPoint(x3, y4));
        //--------------------------------------------------------------------------

        y1 += static_cast<int>(Geometry::Step) * 4;
        y2 += static_cast<int>(Geometry::Step) * 4;
        y3 += 176;
        y4 += 176;
    }

    // drawing 4 vertical lines ( 3rd column )
    for(int i{0}, x1{371}, x2{492}, x3{912}, x4{790},
        y1{90}, y2{266}, y3{173}, y4{299}, y5{126}, y6{302}, y7{126}, y8{201};
        i < static_cast<int>(draw_type) / 8; ++i){

            painter.drawLine(QPoint(x1, y1), QPoint(x1, y2));

            painter.drawLine(QPoint(x2, y3), QPoint(x2, y4));

            // 2 vertical lines(loosers 3rd columns)
            painter.drawLine(QPoint(x3, y5), QPoint(x3, y6));
            //-------------------------------------------------

            // two loosers semifinal lines
            painter.drawLine(QPoint(x4, y7), QPoint(x4, y8));

            y1 += static_cast<int>(Geometry::Step) * 8;
            y2 += static_cast<int>(Geometry::Step) * 8;
            y3 += 225;
            y4 += 225;
            y5 += 352;
            y6 += 352;
            y7 += 354;
            y8 += 354;
        }

    constexpr int x1{613};
    constexpr int x2{614};
    // drawing 1 vertical line ( semifinal column )
    painter.drawLine(QPoint(x1, 235), QPoint(x1, 455));

    // 1 vertical line for bronze medal match
    painter.drawLine(QPoint(x2, 570), QPoint(x2, 659));
}

void TourDraw::click_game(QPushButton *team_1, QPushButton *team_2,
                          QPushButton *winner_basket, QPushButton *loser_basket){

    game_result = std::make_unique<GameResult>(team_1, team_2,
                                               winner_basket, loser_basket, this);

    game_result->show();
}

void TourDraw::drawGameResultBtn(const int x, const int y){

    QPushButton *game_result = new QPushButton(this);
    game_result->setGeometry(QRect(x, y,
                                   static_cast<int>(Geometry::BtnResWidth),
                                   static_cast<int>(Geometry::BtnResHeight)));
    game_result->setText("0 : 0");
    game_result->setDisabled(true);

    // add button to the the list of game result buttons
}

void TourDraw::drawTeamBtn(const int x, const int y){

}
