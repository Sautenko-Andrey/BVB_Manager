#include "tourdraw.h"
#include "ui_tourdraw.h"
#include <QPushButton>
#include <QPainter>
#include <QTextDocument>
#include <QLine>
//#include <QList>
#include <QVector>
#include <QMessageBox>

enum class DrawGeometry{
    coordXStartButton = 10,
    coordYStartButton = 15,
    // teamButtonWidth = 150,
    teamButtonWidth = 115,
    teamButtonHeight = 23,
    teamButtonStep = 44
};

TourDraw::TourDraw(Net net_type, QWidget *parent)
    : QDialog(parent)
    , draw_type{net_type}
    , ui(new Ui::TourDraw)
{
    ui->setupUi(this);

    // drawing a tournament schema

    // first round 16 teams buttons
    for(int i{0}, x = static_cast<int>(DrawGeometry::coordXStartButton),
         y = static_cast<int>(DrawGeometry::coordYStartButton);
         i < static_cast<int>(net_type); ++i){

        // drawing a teams buttons
        QPushButton *team_first_round = new QPushButton(this);

        first_round_team_btns.push_back(team_first_round);

        team_first_round->setGeometry(QRect(x, y,
                                static_cast<int>(DrawGeometry::teamButtonWidth),
                                static_cast<int>(DrawGeometry::teamButtonHeight)));

        team_first_round->setText("Burevestnik 2");

        y += static_cast<int>(DrawGeometry::teamButtonStep);
    }

    // second round 8 teams buttons

    for(int i{0}, y{37}; i < static_cast<int>(draw_type) / 2; ++i){
        QPushButton *team_sec_round = new QPushButton(this);
        team_sec_round->setGeometry(QRect(216, y,   //251
                                    static_cast<int>(DrawGeometry::teamButtonWidth),
                                    static_cast<int>(DrawGeometry::teamButtonHeight)));

        team_sec_round->setText("Kramtehcenter");

        y += static_cast<int>(DrawGeometry::teamButtonStep) * 2;
    }

    // signals & slots
    // connect(first_round_team_btns[0], SIGNAL(clicked()),
    //         this, SLOT(showTeamInfo()));

    for(auto btn : first_round_team_btns){
        connect(btn, SIGNAL(clicked()), this, SLOT(showTeamInfo()));
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
    painter.drawText(QPoint(550, 20), "Tournament 22.08 - 25.08.2024");

    // QTextDocument tournament_title;
    // tournament_title.setHtml("<b><font size='12'>Tournament 22.08 - 25.08.2024</font></b>");
    // tournament_title.drawContents(&painter);

    // drawing horizonthal lines near the first round buttons
    for(int i{0}, y = 26; i < static_cast<int>(draw_type); ++i){
        // painter.drawLine(QPoint(160, y), QPoint(250, y));
        painter.drawLine(QPoint(125, y), QPoint(215, y));

        y += static_cast<int>(DrawGeometry::teamButtonStep);
    }

    // drawing vertical lines near the first round buttons
    for(int i{0}, y1{26}, y2{70}; i < static_cast<int>(draw_type) / 2; ++i){
        // painter.drawLine(QPoint(250, y1), QPoint(250, y2));
        painter.drawLine(QPoint(215, y1), QPoint(215, y2));
        y1 += static_cast<int>(DrawGeometry::teamButtonStep) * 2;
        y2 += static_cast<int>(DrawGeometry::teamButtonStep) * 2;
    }

}

void TourDraw::showTeamInfo()
{
    QMessageBox::information(this, "Team", "Team info");
}
