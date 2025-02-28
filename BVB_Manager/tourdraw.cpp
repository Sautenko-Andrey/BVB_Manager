#include "tourdraw.h"
#include "ui_tourdraw.h"
#include <QPushButton>
#include <QPainter>
#include <QTextDocument>
#include <QLine>
#include <QVector>
#include <QMessageBox>
#include <QStringList>
#include <QRegularExpression>


TourDraw::TourDraw(QSqlDatabase &database,
                   Net net_type,
                   Tournament completed_tournament,
                   QWidget *parent)
    : QDialog(parent)
    , draw_type{net_type}
    , tournament{completed_tournament}
    , parent_dialog{parent}
    , ui(new Ui::TourDraw)
{
    ui->setupUi(this);

    // reserve memory for buttons containers
    L1_L12_buttons.reserve(12);
    L22_L21_buttons.reserve(2);
    W25_W26_looser_buttons.reserve(2);
    L27_L28_buttons.reserve(2);
    W_buttons.reserve(28);
    final_silver_bronze_buttons.reserve(2);
    game_result_buttons.reserve(30);
    loosers.reserve(4);

    // change dialog's background color
    this->setStyleSheet("background-color: azure; color : black");

    // pointer on database
    db = &database;

    // pointer on nowhere for out team
    loser_btn = new QPushButton(this);
    loser_btn->hide();

    // initialize all W buttons with nullptr
    for(int i{0}, total_w_buttons{28}; i < total_w_buttons; ++i){
        W_buttons.append(nullptr);
    }

    // initialize all game result buttons
    for(int i{0}, total_game_res_btns = 30; i < total_game_res_btns; ++i){
        game_result_buttons.append(nullptr);
    }

    // initialize loosers buttons
    for(int i{0}; i < 4; ++i){
        auto looser_btn = new QPushButton(this);
        // dummy sizes
        looser_btn->setGeometry(5000, 5000, 1, 1);
        looser_btn->hide();
        loosers.append(looser_btn);
    }

    // drawing a tournament schema
    // 16 teams buttons (1st column) on the left side
    const QList<int> rank_baskets{
        0, 15, 8, 7, 4, 11, 12, 3, 2, 13, 10, 5, 6, 9, 14, 1
    };

    for(int i{0}, y{static_cast<int>(Geometry::coordYStartBtn)};
        i < static_cast<int>(net_type); ++i){

        // drawing 16 teams buttons

        // get players names
        auto startIndex = tournament.selected_teams[rank_baskets[i]]->text().indexOf('(');  // Find the first '('
        auto endIndex = tournament.selected_teams[rank_baskets[i]]->text().indexOf(')');    // Find the first ')'

        // Check if both indices were found
        QString names;
        if (startIndex != -1 && endIndex != -1 && endIndex > startIndex) {
            // Extract the substring including parentheses
            names = tournament.selected_teams[rank_baskets[i]]->text().mid(startIndex, endIndex - startIndex + 1);
        }

        auto comma_index =
            tournament.selected_teams[rank_baskets[i]]->text().indexOf("(");

        const QString team_name =
            tournament.selected_teams[rank_baskets[i]]->text().left(comma_index - 1);

        auto btn = drawTeamBtn(static_cast<int>(Geometry::coordXStartBtn), y,
                               team_name, true, names);

        // add button to the list
        first_round_team_btns.push_back(btn);

        y += static_cast<int>(Geometry::Step);
    }

    // 8 teams buttons (2nd column) for the left side and the same for the right
    for(int i{0}, j{8}, y{32}, y2{40};
        i < static_cast<int>(draw_type) / 2; ++i){

        // left side(winners)

        auto btn_left = drawTeamBtn(131, y, "W " + QString::number(i + 1));

        // save pointer on the button
        W_buttons[i] = btn_left;

        // right side(loosers)
        auto btn_right = drawTeamBtn(1164, y, "L " + QString::number(j), true);

        // save pointer
        L1_L12_buttons.push_front(btn_right);

        --j;
        y += static_cast<int>(Geometry::Step) * 2;

        //drawing 8 game results buttons (1st column left side) and add to the list
        game_result_buttons[i] = drawGameResultBtn(45, y2);
        y2 += 88;
    }

    // 4 teams buttons (3rd column) for both sides
    const QStringList loosers_labels{"L10", "L9", "L12", "L11"};

    for(int i{0}, y{79}, y2{80}, y3{145}, y4{113}, y5{75}, y6{112};
        i < static_cast<int>(draw_type) / 4; ++i){

        // left side (winners 3rd column)
        auto btn_left = drawTeamBtn(252, y, "W " + QString::number(i + 9));

        W_buttons[i + 8] = btn_left;

        y += static_cast<int>(Geometry::Step) * 4;

        // right side (loosers 2nd column winners)
        auto btn_right_1 = drawTeamBtn(1042, y2, "W " + QString::number(i + 13));

        W_buttons[i + 12] = btn_right_1;

        y2 += static_cast<int>(Geometry::Step) * 4;

        auto btn_right_2 = drawTeamBtn(1042, y3, loosers_labels[i]);

        L1_L12_buttons.push_back(btn_right_2);

        y3 += 177;

        // loosers buttons right side (3 column)
        auto looser_btn_3c = drawTeamBtn(919, y4, "W " + QString::number(i + 17));

        W_buttons[i + 16] = looser_btn_3c;

        y4 += 177;

        // drawing 4 game result buttons (2nd column left side)
        game_result_buttons[i + 8] = drawGameResultBtn(160, y5);
        //y5 += 178;

        // drawing 4 game result buttons (1st column right side)
        game_result_buttons[i + 12] = drawGameResultBtn(1200, y5);

        // drawing 4 game result buttons (2nd column right side)
        game_result_buttons[i + 16] = drawGameResultBtn(1075, y6);

        y5 += 178;
        y6 += 178;
    }

    // buttons (final part)
    for(int i{0}, y{225}, y2{190}, y3{113}, y4{150}, y5{560}, y6{162}, y7{293}, y8{165},
        y9{192}, y10{154}, y11{228}, y12{342}, j{22};
        i < static_cast<int>(draw_type) / 8; ++i){

        // left side 2 buttons 5 column
        auto btn = drawTeamBtn(494, y, "W " + QString::number(i + 27));

        W_buttons[i + 26] = btn;

        // 3d place battle buttons
        auto btn_bronze_match = drawTeamBtn(494, y5, "L " + QString::number(i + 27), true);

        L27_L28_buttons.append(btn_bronze_match);

        // right side 2 buttons (4th column)
        auto btn_right = drawTeamBtn(796, y2, "W " + QString::number(i + 23));

        W_buttons[i + 22] = btn_right;

        // right side 2 buttons (4th column from winners)
        auto btn_right_2 = drawTeamBtn(796, y3, "L " + QString::number(j), true);

        L22_L21_buttons.append(btn_right_2);

        --j;

        // two loosers team for semifinal
        auto btn_right_3 = drawTeamBtn(675, y4, "W " + QString::number(i + 25));

        W_buttons[i + 24] = btn_right_3;

        // 2 win buttons(semifinal)
        auto win_btn = drawTeamBtn(373, y6, "W " + QString::number(i + 21));

        W_buttons[i + 20] = win_btn;

        //2 loosers buttons(semifinal)
        auto looser_btn = drawTeamBtn(373, y7, "W " + QString::number(i + 25), true);

        W25_W26_looser_buttons.append(looser_btn);

        // drawing 2 game result buttons (3rd column left side)
        game_result_buttons[i + 20] = drawGameResultBtn(285, y8);

        // drawing 2 game result buttons (3rd column right side)
        game_result_buttons[i + 22] = drawGameResultBtn(960, y9);

        // drawing 2 game result buttons (4th column right side)
        game_result_buttons[i + 24] = drawGameResultBtn(830, y10);

        // drawing 2 game result buttons (4th column left side)
        game_result_buttons[i + 26] = drawGameResultBtn(412, y11);

        // drawing 2 game result buttons (final and bronze match)
        game_result_buttons[i + 28] = drawGameResultBtn(527, y12);

        y += static_cast<int>(Geometry::Step) * 5;
        y2 += 355;
        y3 += 355;
        y4 += 360;
        y5 += static_cast<int>(Geometry::Step) * 2;
        y6 += static_cast<int>(Geometry::Step) * 8;
        y7 += 95;
        y8 += 353;
        y9 += 356;
        y10 += 356;
        y11 += 225;
        y12 += 277;
    }

    // 3 team buttons (1st, 2nd, 3rd places)
    for(int i{0}, y{285}; i < 3; ++i){

        QPushButton *btn{nullptr};

        if(i == 0){
            btn = drawTeamBtn(615, y, "1st place");
            final_silver_bronze_buttons.append(btn);
        }
        else if(i == 1){
            btn = drawTeamBtn(615, y, "2nd place", true);
            final_silver_bronze_buttons.append(btn);
        }
        else{
            btn = drawTeamBtn(615, y, "3rd place");
            final_silver_bronze_buttons.append(btn);
        }

        y += 50;
    }

    // Operation buttons
    // button for move forward to the next tour and skip "empty" games
    // left side button

    // left side button
    auto forward_left = makeOpsBtn(QRect(540, 15, 40, 20), ">>", ops_btn_style);

    //right side button 2nd column
    auto forward_right_1 = makeOpsBtn(QRect(590, 15, 40, 20), "<<", ops_btn_style);

    //right side button 3rd column
    auto forward_right_2 = makeOpsBtn(QRect(640, 15, 40, 20), "<<<", ops_btn_style);

    // create animations for butoons W1 - W8
    createAnimation(std::move(std::make_pair(0, 8)), W_buttons, animations, 1200);

    // create animations for buttons W13, W14, W15, W16
    createAnimation(std::move(std::make_pair(12, 16)), W_buttons, animations_right_1, 1200);

    // create animations for buttons W17, W18, W19, W20
    createAnimation(std::move(std::make_pair(16, 20)), W_buttons, animations_right_2, 1200);


    // Signals & slots

    //W1
    connect(W_buttons[0], &QPushButton::clicked, this, [this](){
        click_game(first_round_team_btns[0], first_round_team_btns[1],
                W_buttons[0], L1_L12_buttons[0], game_result_buttons[0],
                animations[0]);
    });

    //W2
    connect(W_buttons[1], &QPushButton::clicked, this, [this](){
        click_game(first_round_team_btns[2], first_round_team_btns[3],
                W_buttons[1], L1_L12_buttons[1], game_result_buttons[1],
                animations[1]);
    });

    //W3
    connect(W_buttons[2], &QPushButton::clicked, this, [this](){
        click_game(first_round_team_btns[4], first_round_team_btns[5],
                W_buttons[2], L1_L12_buttons[2], game_result_buttons[2], animations[2]);
    });

    //W4
    connect(W_buttons[3], &QPushButton::clicked, this, [this](){
        click_game(first_round_team_btns[6], first_round_team_btns[7],
                W_buttons[3], L1_L12_buttons[3], game_result_buttons[3], animations[3]);
    });

    //W5
    connect(W_buttons[4], &QPushButton::clicked, this, [this](){
        click_game(first_round_team_btns[8], first_round_team_btns[9],
                W_buttons[4],L1_L12_buttons[4], game_result_buttons[4], animations[4]);
    });

    //W6
    connect(W_buttons[5], &QPushButton::clicked, this, [this](){
        click_game(first_round_team_btns[10], first_round_team_btns[11],
                W_buttons[5], L1_L12_buttons[5], game_result_buttons[5], animations[5]);
    });

    //W7
    connect(W_buttons[6], &QPushButton::clicked, this, [this](){
        click_game(first_round_team_btns[12], first_round_team_btns[13],
                W_buttons[6], L1_L12_buttons[6], game_result_buttons[6], animations[6]);
    });

    //W8
    connect(W_buttons[7], &QPushButton::clicked, this, [this](){
        click_game(first_round_team_btns[14], first_round_team_btns[15],
                W_buttons[7], L1_L12_buttons[7], game_result_buttons[7], animations[7]);
    });

    //W9
    connect(W_buttons[8], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[0], W_buttons[1],
                W_buttons[8], L1_L12_buttons[9], game_result_buttons[8]);
    });

    //W10
    connect(W_buttons[9], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[2], W_buttons[3],
                W_buttons[9], L1_L12_buttons[8], game_result_buttons[9]);
    });

    //W11
    connect(W_buttons[10], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[4], W_buttons[5],
                W_buttons[10], L1_L12_buttons[11], game_result_buttons[10]);
    });

    //W12
    connect(W_buttons[11], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[6], W_buttons[7],
                W_buttons[11], L1_L12_buttons[10], game_result_buttons[11]);
    });

    //W13
    connect(W_buttons[12], &QPushButton::clicked, this, [this](){
        click_game(L1_L12_buttons[7], L1_L12_buttons[6],
                W_buttons[12], loser_btn, game_result_buttons[12], animations_right_1[0]);
    });

    //W14
    connect(W_buttons[13], &QPushButton::clicked, this, [this](){
        click_game(L1_L12_buttons[5], L1_L12_buttons[4],
                W_buttons[13], loser_btn, game_result_buttons[13], animations_right_1[1]);
    });

    //W15
    connect(W_buttons[14], &QPushButton::clicked, this, [this](){
        click_game(L1_L12_buttons[3], L1_L12_buttons[2],
                W_buttons[14], loser_btn, game_result_buttons[14], animations_right_1[2]);
    });

    //W16
    connect(W_buttons[15], &QPushButton::clicked, this, [this](){
        click_game(L1_L12_buttons[1], L1_L12_buttons[0],
                W_buttons[15], loser_btn, game_result_buttons[15], animations_right_1[3]);
    });

    //W17
    connect(W_buttons[16], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[12], L1_L12_buttons[8],
                W_buttons[16], loser_btn, game_result_buttons[16], animations_right_2[0]);
    });

    //W18
    connect(W_buttons[17], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[13], L1_L12_buttons[9],
                W_buttons[17], loser_btn, game_result_buttons[17], animations_right_2[1]);
    });

    //W19
    connect(W_buttons[18], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[14], L1_L12_buttons[10],
                W_buttons[18], loser_btn, game_result_buttons[18], animations_right_2[2]);
    });

    //W20
    connect(W_buttons[19], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[15], L1_L12_buttons[11],
                W_buttons[19], loser_btn, game_result_buttons[19], animations_right_2[3]);
    });

    //W21
    connect(W_buttons[20], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[8], W_buttons[9],
                W_buttons[20], L22_L21_buttons[1], game_result_buttons[20]);
    });

    //W22
    connect(W_buttons[21], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[10], W_buttons[11],
                W_buttons[21], L22_L21_buttons[0], game_result_buttons[21]);
    });

    //W23
    connect(W_buttons[22], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[16], W_buttons[17],
                W_buttons[22], loser_btn, game_result_buttons[22]);
    });

    //W24
    connect(W_buttons[23], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[18], W_buttons[19],
                W_buttons[23], loser_btn, game_result_buttons[23]);
    });

    //W25
    connect(W_buttons[24], &QPushButton::clicked, this, [this](){
        click_game(L22_L21_buttons[0], W_buttons[22],
                W25_W26_looser_buttons[0], loser_btn, game_result_buttons[24]);
    });

    //W26
    connect(W_buttons[25], &QPushButton::clicked, this, [this](){
        click_game(L22_L21_buttons[1], W_buttons[23],
                W25_W26_looser_buttons[1], loser_btn, game_result_buttons[25]);
    });

    //W27
    connect(W_buttons[26], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[20], W25_W26_looser_buttons[0],
                W_buttons[26], L27_L28_buttons[0], game_result_buttons[26]);
    });

    //W28
    connect(W_buttons[27], &QPushButton::clicked, this, [this](){
        click_game(W25_W26_looser_buttons[1],W_buttons[21],
                W_buttons[27], L27_L28_buttons[1], game_result_buttons[27]);
    });

    // bronze and golden medals matches
    // 3rd place
    connect(final_silver_bronze_buttons[2], &QPushButton::clicked, this, [this](){
        click_game(L27_L28_buttons[0], L27_L28_buttons[1],
                final_silver_bronze_buttons[2], loser_btn, game_result_buttons[29]);
    });

    // final
    connect(final_silver_bronze_buttons[0], &QPushButton::clicked, this, [this](){
        click_game(W_buttons[26], W_buttons[27],
                final_silver_bronze_buttons[0], final_silver_bronze_buttons[1],
                game_result_buttons[28]);
    });

    // left forward button
    connect(forward_left, &QPushButton::clicked, this, [this](){
        //make a list of W1 - W8 buttons
        QList<QPushButton *> W1_W8_btns = W_buttons.mid(0, 8);

        // make a list of L1 - L7 buttons
        QList<QPushButton *> L1_L8_btns = L1_L12_buttons.mid(0, 8);
        moveTeams(first_round_team_btns, W1_W8_btns, L1_L8_btns, animations);
    });

    //right 1st column forward button
    connect(forward_right_1, &QPushButton::clicked, this, [this](){

        // make a list of W13,W14,W15,W16
        QList<QPushButton *> W13_W16_btns;
        for(int i{0}; i < 4; ++i){
            W13_W16_btns.push_front(W_buttons[i + 12]);
        }

        // make a list of L1 - L7 buttons
        QList<QPushButton *> L1_L8_btns = L1_L12_buttons.mid(0, 8);

        moveTeams(L1_L8_btns, W13_W16_btns, loosers, animations_right_1);
    });

    connect(forward_right_2, &QPushButton::clicked, this, [this](){

        // make a list of W17, W18, W19, W20
        QList<QPushButton *> W17_W20_btns = W_buttons.mid(16, 4);  // bug is here

        // make a list of W13, L10, W14, L9, W15, L12, W16, L11
        QList<QPushButton *> WL_btns;

        for(int i{12}, j{8}; i < 16; ++i, ++j){
            WL_btns.push_back(W_buttons[i]);
            WL_btns.push_back(L1_L12_buttons[j]);
        }

        moveTeams(WL_btns, W17_W20_btns, loosers, animations_right_2);

    });
}



TourDraw::~TourDraw()
{
    // Deleting animation
    for (auto animation : animations) {
        animation->deleteLater();
    }

    delete ui;
}


/*
    Function creates a custom operation button with a specific style
*/
QPushButton* TourDraw::makeOpsBtn(const QRect &geom,
                                  const QString &text,
                                  const QString &style){
    auto btn = new QPushButton(this);
    btn->setGeometry(geom);
    btn->setText(text);
    btn->setStyleSheet(style);

    return btn;
}



/*
    Function creates animation for buttons
*/
void TourDraw::createAnimation(std::pair<int,int> &&range,
                               const QList<QPushButton *> &buttons,
                               QList<QPropertyAnimation *> &animations,
                               int duration){

    for(int i{range.first}; i < range.second; ++i){
        auto animation = new QPropertyAnimation(buttons[i], "geometry", buttons[i]);
        animation->setDuration(duration);
        QPoint btn_pos = buttons[i]->pos();
        animation->setStartValue(QRect(btn_pos.x(), btn_pos.y(),
                                 static_cast<int>(Geometry::BtnWidth),
                                 static_cast<int>(Geometry::BtnHeight)));
        animation->setEndValue(QRect(btn_pos.x(), btn_pos.y(),
                               static_cast<int>(Geometry::BtnWidth) + 10,
                               static_cast<int>(Geometry::BtnHeight) + 10));

        animations.push_back(animation);
    }
}


/*
    Function moves teams forward through the tournament
    if it neccessary (when team doesn't occur a real competitor
    or both teams slots are 'blank')
*/
void TourDraw::moveForward(QPushButton *teamA, QPushButton *teamB,
                           QPushButton *win_btn, QPushButton *los_btn,
                           QPropertyAnimation *animation){

    if(teamA && teamB && win_btn && los_btn && animation){
        const QString no_team = "None, None, None";

        if(teamA->text() == no_team && teamB->text() != no_team){
            win_btn->setText(teamB->text());
            win_btn->setToolTip(teamB->text());
            los_btn->setText(teamA->text());
            los_btn->setToolTip(teamA->text());
        }
        else if(teamB->text() == no_team && teamA->text() != no_team){
            win_btn->setText(teamA->text());
            win_btn->setToolTip(teamA->text());
            los_btn->setText(teamB->text());
            los_btn->setToolTip(teamB->text());
        }
        else if(teamA->text() == no_team && teamB->text() == no_team){
            // no matter
            win_btn->setText(teamA->text());
            win_btn->setToolTip(teamA->text());
            los_btn->setText(teamB->text());
            los_btn->setToolTip(teamB->text());
        }
        else{

            // make the button pulsing
            win_btn->setStyleSheet(team_btn_pulsing_style);
            animation->setEasingCurve(QEasingCurve::OutBounce);
            animation->setLoopCount(-1);  // Loop indefinitely
            animation->start();
        }

        if(los_btn->text() == no_team){
            los_btn->setStyleSheet(team_btn_style_for_none);
        }

        if(win_btn->text() == no_team){
            win_btn->setStyleSheet(team_btn_style_for_none);
        }

        fontAdapter(win_btn);
        fontAdapter(los_btn);
    }
}



void TourDraw::moveTeams(const QList<QPushButton *> &teams,
                         QList<QPushButton *> &win_btns,
                         QList<QPushButton *> &loosers_btns,
                         QList<QPropertyAnimation *> &animations){

    // use moveTeam()
    for(int i{0}, j{0}; i < teams.size(); i += 2, ++j){
        moveForward(teams[i], teams[i + 1], win_btns[j], loosers_btns[j], animations[j]);
    }
}


/*
    Function paints lines
*/
void TourDraw::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // drawing a tournament title
    painter.setFont(QFont("Times", 15));

    // make text and lines black color
    // color is #2e3440
    painter.setPen(QColor(46, 52, 64));

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


/*
    Function triggers when user clicks on game button
*/
void TourDraw::click_game(QPushButton *team_1, QPushButton *team_2,
                          QPushButton *winner_basket, QPushButton *loser_basket,
                          QPushButton *game_result_btn, QPropertyAnimation *animation){

    QStringList *empty_list{nullptr};
    game_result = std::make_unique<GameResult>(team_1, team_2,
                                               winner_basket, loser_basket,
                                               empty_list, game_result_btn,
                                               this);

    // stop animation
    if(animation != nullptr && animation->state() == QAbstractAnimation::Running){
        animation->stop();
        winner_basket->setStyleSheet(team_btn_style);
        winner_basket->resize(static_cast<int>(Geometry::BtnWidth),
                              static_cast<int>(Geometry::BtnHeight));
    }

    game_result->show();
}


/*
    Function draws game result button with a specific stylesheet
*/
QPushButton* TourDraw::drawGameResultBtn(const int x, const int y){

    // drawing game result button
    QPushButton *game_result_btn = new QPushButton(this);
    game_result_btn->setGeometry(QRect(x, y,
                                   static_cast<int>(Geometry::BtnResWidth),
                                   static_cast<int>(Geometry::BtnResHeight)));
    game_result_btn->setText("0 : 0");
    game_result_btn->setDisabled(true);
    game_result_btn->setStyleSheet(game_result_style);

    return game_result_btn;
}


/*
    Function draws team button with a specific stylesheet
*/
QPushButton* TourDraw::drawTeamBtn(const int x, const int y,
                                   const QString &team_name, bool is_disabled,
                                   const QString &names){

    // drawing a team button
    QPushButton *team_btn = new QPushButton(this);

    team_btn->setGeometry(QRect(x, y,
                           static_cast<int>(Geometry::BtnWidth),
                           static_cast<int>(Geometry::BtnHeight)));

    team_btn->setText(team_name);

    team_btn->setDisabled(is_disabled);

    // change team name's color if it is None
    if(team_btn->text() == "None, None, None"){
        team_btn->setStyleSheet(team_btn_style_for_none);
    }
    else{
        team_btn->setStyleSheet(team_btn_style);
    }

    team_btn->setToolTip(names);

    // change button's font if necessary
    fontAdapter(team_btn);

    return team_btn;
}
