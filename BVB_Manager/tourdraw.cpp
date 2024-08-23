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

enum class DrawGeometry{
    coordXStartButton = 10,
    coordYStartButton = 15,
    // teamButtonWidth = 150,
    teamButtonWidth = 115,
    teamButtonHeight = 23,
    teamButtonStep = 44
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

    // drawing a tournament schema

    // teams data
    // test
    qDebug() << "Teams: ";
    for(auto team : tournament.selected_teams){
        qDebug() << team->text();
        QStringList team_data = team->text().split(",");
        for(const auto &data : team_data){
            qDebug() << data.trimmed();
        }
        qDebug() << team_data[2].last(3).toInt();

        qDebug() << "--------------------------------------";
    }

    qDebug() << tournament.selected_teams[0]->text();


    // first round 16 teams buttons
    for(int i{0}, x = static_cast<int>(DrawGeometry::coordXStartButton),
         y = static_cast<int>(DrawGeometry::coordYStartButton);
         i < static_cast<int>(net_type); ++i){

        // drawing a teams buttons
        QPushButton *team_first_round_btn = new QPushButton(this);

        first_round_team_btns.push_back(team_first_round_btn);

        team_first_round_btn->setGeometry(QRect(x, y,
                                static_cast<int>(DrawGeometry::teamButtonWidth),
                                static_cast<int>(DrawGeometry::teamButtonHeight)));

        // getting team's name
        auto comma_index = tournament.selected_teams[i]->text().indexOf("(");

        QString team_name = tournament.selected_teams[i]->text().left(comma_index - 1);

        team_first_round_btn->setText(team_name);

        constexpr int max_symbols_amount = 24;

        if(team_name.size() >= max_symbols_amount){
            team_first_round_btn->setFont(QFont("Ubuntu", 7));
        }
        else if(team_name.size() > max_symbols_amount / 2 + 1 &&
                team_name.size() < max_symbols_amount){
            team_first_round_btn->setFont(QFont("Ubuntu", 10));
        }

        // make button disabled
        team_first_round_btn->setDisabled(true);

        y += static_cast<int>(DrawGeometry::teamButtonStep);
    }


    // make draw for the first round buttons via first_round_team_btns vector
    //...code
    // ... create two different vectors with high rank and low rank ???here???
    // our sort teams by rank in previous widget?

    // second round 8 teams buttons 8 game result buttons
    for(int i{0}, y{37}; i < static_cast<int>(draw_type) / 2; ++i){
        QPushButton *team_sec_round_btn = new QPushButton(this);
        team_sec_round_btn->setGeometry(QRect(176, y,   //216
                                    static_cast<int>(DrawGeometry::teamButtonWidth),
                                    static_cast<int>(DrawGeometry::teamButtonHeight)));

        team_sec_round_btn->setText("Unknown");
        // make button disabled
        team_sec_round_btn->setDisabled(true);

        y += static_cast<int>(DrawGeometry::teamButtonStep) * 2;

        // container with pointers on this buttons????
    }

    // 8 game result buttons (1 round)
    for(int i{0}, y = 34; i < static_cast<int>(draw_type) / 2; ++i){
        QPushButton *game_res_btn_1 = new QPushButton(this);
        game_res_btn_1->setGeometry(QRect(136, y, 30, 30));
        game_res_btn_1->setText("res");

        y += static_cast<int>(DrawGeometry::teamButtonStep) * 2;

        // append to list of game res buttons
        game_res_1r_btns.push_back(game_res_btn_1);
    }

    // signals & slots
    // connect(first_round_team_btns[0], SIGNAL(clicked()),
    //         this, SLOT(showTeamInfo()));

    for(auto btn : first_round_team_btns){
        connect( btn, SIGNAL(clicked()), this, SLOT(showTeamInfo()));
    }

}

TourDraw::~TourDraw()
{
    delete ui;
}

void TourDraw::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // drawing a tournament title
    painter.setFont(QFont("Times", 15));
    painter.drawText(QPoint(450, 20), tournament.tour_title +
                                      " ( " +
                                      tournament.date_begin +
                                      " - " +
                                      tournament.date_end +
                                      ") MAIN DRAW ( " +
                                      tournament.tour_gender_type +
                                      " )");



    // drawing horizonthal lines near the first round buttons
    for(int i{0}, y{26}; i < static_cast<int>(draw_type); ++i){
        // painter.drawLine(QPoint(125, y), QPoint(215, y));
        painter.drawLine(QPoint(125, y), QPoint(175, y));
        y += static_cast<int>(DrawGeometry::teamButtonStep);
    }

    // drawing vertical lines near the first round buttons
    for(int i{0}, y1{26}, y2{70}; i < static_cast<int>(draw_type) / 2; ++i){
        painter.drawLine(QPoint(175, y1), QPoint(175, y2));
        y1 += static_cast<int>(DrawGeometry::teamButtonStep) * 2;
        y2 += static_cast<int>(DrawGeometry::teamButtonStep) * 2;
    }

    // drawing horizontal lines near the second round buttons
    for(int i{0}, y{47}; i < static_cast<int>(draw_type) / 2; ++i){
        painter.drawLine(QPoint(290, y), QPoint(340, y));
        y += static_cast<int>(DrawGeometry::teamButtonStep) * 2;
    }

}

void TourDraw::showTeamInfo()
{
    QMessageBox::information(this, "Team", "Team info");
}
