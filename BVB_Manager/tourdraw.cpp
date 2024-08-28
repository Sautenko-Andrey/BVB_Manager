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
    coordXStartBtn = 10,
    coordYStartBtn = 15,
    BtnWidth = 115,
    BtnHeight = 23,
    Step = 44,
    StepX = 121,
    XVerticalLine = 130
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


    // 16 teams buttons (1st column)
    for(int i{0},
         y = static_cast<int>(DrawGeometry::coordYStartBtn);
         i < static_cast<int>(net_type); ++i){

        // drawing a teams buttons
        QPushButton *btn = new QPushButton(this);

        first_round_team_btns.push_back(btn);

        btn->setGeometry(QRect(static_cast<int>(DrawGeometry::coordXStartBtn), y,
                               static_cast<int>(DrawGeometry::BtnWidth),
                               static_cast<int>(DrawGeometry::BtnHeight)));

        // getting team's name
        auto comma_index = tournament.selected_teams[i]->text().indexOf("(");

        QString team_name = tournament.selected_teams[i]->text().left(comma_index - 1);

        btn->setText(team_name);

        btn->setDisabled(true);

        constexpr int max_symbols_amount = 24;

        if(team_name.size() >= max_symbols_amount){
            btn->setFont(QFont("Ubuntu", 7));
        }
        else if(team_name.size() > max_symbols_amount / 2 + 1 &&
                team_name.size() < max_symbols_amount){
            btn->setFont(QFont("Ubuntu", 10));
        }

        y += static_cast<int>(DrawGeometry::Step);
    }


    // make draw for the first round buttons via first_round_team_btns vector
    //...code
    // ... create two different vectors with high rank and low rank ???here???
    // our sort teams by rank in previous widget?
    // 8 teams buttons (2nd column) for the left side and the same for the right
    for(int i{0}, j{8}, y{static_cast<int>(DrawGeometry::coordYStartBtn) + 22};
        i < static_cast<int>(draw_type) / 2; ++i){

        // left side(winners)--------------------------------------------------------------
        QPushButton *btn_left = new QPushButton(this);
        btn_left->setGeometry(QRect(static_cast<int>(DrawGeometry::coordXStartBtn) +
                               static_cast<int>(DrawGeometry::StepX),
                               y,
                               static_cast<int>(DrawGeometry::BtnWidth),
                               static_cast<int>(DrawGeometry::BtnHeight)));

        btn_left->setText("W " + QString::number(i + 1));
        // y += static_cast<int>(DrawGeometry::Step) * 2;
        //------------------------------------------------------------------------

        // right side(loosers)-------------------------------------------------------------
        QPushButton *btn_right = new QPushButton(this);
        btn_right->setGeometry(QRect(static_cast<int>(DrawGeometry::coordXStartBtn) +
                                        static_cast<int>(DrawGeometry::StepX) * 9 + 65,
                                    y,
                                    static_cast<int>(DrawGeometry::BtnWidth),
                                    static_cast<int>(DrawGeometry::BtnHeight)));
        btn_right->setText("L " + QString::number(j));
        --j;
        //------------------------------------------------------------------------

        y += static_cast<int>(DrawGeometry::Step) * 2;

        // container with pointers on these buttons????
    }


    // 4 teams buttons (3rd column) for both sides
    const QStringList loosers_labels{"L10", "L9", "L12", "L11"};
    for(int i{0}, y{static_cast<int>(DrawGeometry::coordYStartBtn) + 64},
        y2{static_cast<int>(DrawGeometry::coordYStartBtn) + 65},
        y3{static_cast<int>(DrawGeometry::coordYStartBtn) + 130},
        y4{113};
        i < static_cast<int>(draw_type) / 4; ++i){

        // left side (winners 3rd column)------------------------------------------------
        QPushButton *btn_left = new QPushButton(this);
        btn_left->setGeometry(QRect(static_cast<int>(DrawGeometry::coordXStartBtn) +
                               static_cast<int>(DrawGeometry::StepX) * 2,
                               y,
                               static_cast<int>(DrawGeometry::BtnWidth),
                               static_cast<int>(DrawGeometry::BtnHeight)));

        btn_left->setText("W " + QString::number(i + 9));
        y += static_cast<int>(DrawGeometry::Step) * 4;
        //----------------------------------------------------------------------

        // right side (loosers 2nd column winners)----------------------------------------
        QPushButton *btn_right_1 = new QPushButton(this);
        btn_right_1->setGeometry(QRect(static_cast<int>(DrawGeometry::coordXStartBtn) +
                                 static_cast<int>(DrawGeometry::StepX) * 8 + 64,
                                 y2,
                                 static_cast<int>(DrawGeometry::BtnWidth),
                                 static_cast<int>(DrawGeometry::BtnHeight)));
        btn_right_1->setText("W " + QString::number(i + 13));
        y2 += static_cast<int>(DrawGeometry::Step) * 4;
        //------------------------------------------------------------------------

        // right side (loosers 2nd column loosers)------------------------------------
        QPushButton *btn_right_2 = new QPushButton(this);
        btn_right_2->setGeometry(QRect(static_cast<int>(DrawGeometry::coordXStartBtn) +
                                           static_cast<int>(DrawGeometry::StepX) * 8 + 64,
                                       y3,
                                       static_cast<int>(DrawGeometry::BtnWidth),
                                       static_cast<int>(DrawGeometry::BtnHeight)));
        btn_right_2->setText(loosers_labels[i]);
        // y3 += static_cast<int>(DrawGeometry::Step) * 3 + 45;
        y3 += 177;
        //----------------------------------------------------------------------------

        // loosers buttons right side (3 column)-------------------------------------
        QPushButton *looser_btn_3c = new QPushButton(this);
        looser_btn_3c->setGeometry(QRect(919, y4,
                                         static_cast<int>(DrawGeometry::BtnWidth),
                                         static_cast<int>(DrawGeometry::BtnHeight)));
        looser_btn_3c->setText("W " + QString::number(i + 17));
        y4 += 177;
        //---------------------------------------------------------------------------
    }

    // 4 teams buttons (semifinal column)
    for(int i{0}, y1{static_cast<int>(DrawGeometry::coordYStartBtn) + 147},
        y2{static_cast<int>(DrawGeometry::coordYStartBtn) + 278};
        i < static_cast<int>(draw_type) / 8; ++i){

        // 2 win buttons
        QPushButton *win_btn = new QPushButton(this);
        win_btn->setGeometry(QRect(static_cast<int>(DrawGeometry::coordXStartBtn) +
                                   static_cast<int>(DrawGeometry::StepX) * 3,
                                   y1,
                                   static_cast<int>(DrawGeometry::BtnWidth),
                                   static_cast<int>(DrawGeometry::BtnHeight)));

        win_btn->setText("W " + QString::number(i + 21));
        y1 += static_cast<int>(DrawGeometry::Step) * 8;

        //2 loosers buttons---------------------------------------------------------
        QPushButton *looser_btn = new QPushButton(this);
        looser_btn->setGeometry(QRect(static_cast<int>(DrawGeometry::coordXStartBtn) +
                                      static_cast<int>(DrawGeometry::StepX) * 3,
                                      y2,
                                      static_cast<int>(DrawGeometry::BtnWidth),
                                      static_cast<int>(DrawGeometry::BtnHeight)));
        looser_btn->setText("W " + QString::number(i + 25));
        y2 += static_cast<int>(DrawGeometry::Step) * 2 + 7;
        //-----------------------------------------------------------------------------
    }

    // 2  team buttons (final column)
    for(int i{0}, y{static_cast<int>(DrawGeometry::coordYStartBtn) + 210},
        y2{190}, y3{113}, y4{150}, y5{560}, j{22};
        i < static_cast<int>(draw_type) / 8; ++i){

        // left side 2 buttons 5 column-------------------------------------------
        QPushButton *btn = new QPushButton(this);
        btn->setGeometry(QRect(static_cast<int>(DrawGeometry::coordXStartBtn) +
                               static_cast<int>(DrawGeometry::StepX) * 4,
                               y,
                               static_cast<int>(DrawGeometry::BtnWidth),
                               static_cast<int>(DrawGeometry::BtnHeight)));

        btn->setText("W " + QString::number(i + 27));
        y += static_cast<int>(DrawGeometry::Step) * 5;
        //-----------------------------------------------------------------------

        // 3d place battle buttons-----------------------------------------------
        QPushButton *btn_bronze_match = new QPushButton(this);
        btn_bronze_match->setGeometry(QRect(static_cast<int>(DrawGeometry::coordXStartBtn) +
                                   static_cast<int>(DrawGeometry::StepX) * 4,
                               y5,
                               static_cast<int>(DrawGeometry::BtnWidth),
                               static_cast<int>(DrawGeometry::BtnHeight)));

        btn_bronze_match->setText("L " + QString::number(i + 27));
        y5 += static_cast<int>(DrawGeometry::Step) * 2;
        //-----------------------------------------------------------------------


        // right side 2 buttons (4th column)--------------------------------------
        QPushButton *btn_right = new QPushButton(this);
        btn_right->setGeometry(QRect(796, y2,
                                        static_cast<int>(DrawGeometry::BtnWidth),
                                        static_cast<int>(DrawGeometry::BtnHeight)));
        btn_right->setText("W " + QString::number(i + 23));
        y2 += 355;
        //------------------------------------------------------------------------
        // right side 2 buttons (4th column from winners)--------------------------------------
        QPushButton *btn_right_2 = new QPushButton(this);
        btn_right_2->setGeometry(QRect(796, y3,
                                        static_cast<int>(DrawGeometry::BtnWidth),
                                        static_cast<int>(DrawGeometry::BtnHeight)));
        btn_right_2->setText("L " + QString::number(j));
        y3 += 355;
        --j;
        //------------------------------------------------------------------------

        // two loosers team for semifinal
        QPushButton *btn_right_3 = new QPushButton(this);
        btn_right_3->setGeometry(QRect(675, y4,
                                       static_cast<int>(DrawGeometry::BtnWidth),
                                       static_cast<int>(DrawGeometry::BtnHeight)));
        btn_right_3->setText("W " + QString::number(i + 25));
        y4 += 360;

    }

    // 3 team buttons (1st, 2nd, 3rd)
    for(int i{0}, y{static_cast<int>(DrawGeometry::coordYStartBtn) + 270}; i < 3; ++i){
        QPushButton *btn = new QPushButton(this);

        btn->setGeometry(QRect(static_cast<int>(DrawGeometry::coordXStartBtn) +
                               static_cast<int>(DrawGeometry::StepX) * 5,
                               y,
                               static_cast<int>(DrawGeometry::BtnWidth),
                               static_cast<int>(DrawGeometry::BtnHeight)));

        if(i == 0){
            btn->setText("1st place");
        }
        else if(i == 1){
            btn->setText("2nd place");
            btn->setDisabled(true);
        }
        else{
            btn->setText("3rd place");
        }

        y += 50;
    }


    // signals & slots
    // connect(first_round_team_btns[0], SIGNAL(clicked()),
    //         this, SLOT(showTeamInfo()));

    // for(auto btn : first_round_team_btns){
    //     connect( btn, SIGNAL(clicked()), this, SLOT(showTeamInfo()));
    // }
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

    // final text
    painter.setFont(QFont("Times", 12));
    painter.drawText(QPoint(505, 340), "Final match");
    // bronze medal text
    painter.drawText(QPoint(505, 615), "3rd place match");

    // drawing 16 horizonthal lines ( 1st column )
    for(int i{0}, y{26}; i < static_cast<int>(draw_type); ++i){
        painter.drawLine(QPoint(125, y), QPoint(130, y));
        y += static_cast<int>(DrawGeometry::Step);
    }

    // drawing 8 horizontal lines ( 2nd column left side ) and same for the right
    for(int i{0}, y{47}; i < static_cast<int>(draw_type) / 2; ++i){
        // left side (2nd columnn)--------------------------
        painter.drawLine(QPoint(245, y), QPoint(250, y));
        // y += static_cast<int>(DrawGeometry::Step) * 2;
        //-------------------------------------------------

        // right side (2nd column)-----------------------------------------
        painter.drawLine(QPoint(1158, y), QPoint(1163, y));
        //-----------------------------------------------------------------

        y += static_cast<int>(DrawGeometry::Step) * 2;
    }

    // drawing 4 horizontal lines (winners 3rd column and loosers 2nd column)
    for(int i{0}, y{90}, y2{92}, y3{157}, y4{126}; i < static_cast<int>(draw_type) / 4; ++i){

        // left side----------------------------------------
        painter.drawLine(QPoint(365, y), QPoint(370, y));
        //----------------------------------------------------

        // right side----------------------------------------
        painter.drawLine(QPoint(1036, y2), QPoint(1041, y2));
        painter.drawLine(QPoint(1036, y3), QPoint(1041, y3));
        painter.drawLine(QPoint(913, y4), QPoint(918, y4));
        //----------------------------------------------------

        y += static_cast<int>(DrawGeometry::Step) * 4;
        y2 += static_cast<int>(DrawGeometry::Step) * 4;
        y3 += static_cast<int>(DrawGeometry::Step) * 4;
        y4 += static_cast<int>(DrawGeometry::Step) * 4;
    }

    // drawing 4 horizontal lines ( 4 column )
    for(int i{0}, y1{173}, y2{300}, y3{570}; i < static_cast<int>(draw_type) / 8; ++i){

        painter.drawLine(QPoint(487, y1), QPoint(492, y1));
        y1 += static_cast<int>(DrawGeometry::Step) * 8;

        painter.drawLine(QPoint(487, y2), QPoint(492, y2));
        y2 += static_cast<int>(DrawGeometry::Step) * 2 + 10;

        // 2 horizontal 3rd place lines
        painter.drawLine(QPoint(608, y3), QPoint(613, y3));
        y3 += 89;
    }

    // drawing 2 horizontal lines ( semifinals column )
    for(int i{0}, y{235}, y2{125}, y3{201}; i < static_cast<int>(draw_type) / 8; ++i){
        //left side----------------------------------------
        painter.drawLine(QPoint(607, y), QPoint(612, y));
        y += static_cast<int>(DrawGeometry::Step) * 5;
        //-------------------------------------------------

        // right side quarter finals 4 lines
        painter.drawLine(QPoint(790, y2), QPoint(795, y2));
        y2 += 355;
        painter.drawLine(QPoint(790, y3), QPoint(795, y3));
        y3 += 355;
        //-------------------------------------------------
    }

    // drawing 8 vertical lines ( 1st column left side)
    for(int i{0}, y1{26}, y2{70}; i < static_cast<int>(draw_type) / 2; ++i){
        painter.drawLine(QPoint(static_cast<int>(DrawGeometry::XVerticalLine),
                         y1),
                         QPoint(static_cast<int>(DrawGeometry::XVerticalLine),
                         y2));
        y1 += static_cast<int>(DrawGeometry::Step) * 2;
        y2 += static_cast<int>(DrawGeometry::Step) * 2;
    }


    // drawing 4 vertical lines ( 2nd column ) for both sides
    for(int i{0}, y1{47}, y2{135}, y3{92}, y4{157}; i < static_cast<int>(draw_type) / 4; ++i){

        // left side(winners)------------------------------------------------------
        constexpr int add_left = 120;
        painter.drawLine(QPoint(static_cast<int>(DrawGeometry::XVerticalLine) + add_left,
                                y1),
                         QPoint(static_cast<int>(DrawGeometry::XVerticalLine) + add_left,
                                y2));
        constexpr int mul = 4;
        // y1 += static_cast<int>(DrawGeometry::Step) * mul;
        // y2 += static_cast<int>(DrawGeometry::Step) * mul;
        //-------------------------------------------------------------------------
        constexpr int add_right = 1027;
        // right side (loosers)-----------------------------------------------------
        painter.drawLine(QPoint(static_cast<int>(DrawGeometry::XVerticalLine) + add_right,
                                y1),
                         QPoint(static_cast<int>(DrawGeometry::XVerticalLine) + add_right,
                                y2));
        //--------------------------------------------------------------------------

        //right side (winners 2 column)---------------------------------------------
        painter.drawLine(QPoint(1035, y3), QPoint(1035, y4));
        //--------------------------------------------------------------------------
        y1 += static_cast<int>(DrawGeometry::Step) * mul;
        y2 += static_cast<int>(DrawGeometry::Step) * mul;
        y3 += 176;
        y4 += 176;
    }

    // drawing 4 vertical lines ( 3rd column )
    for(int i{0}, y1{90}, y2{266}, y3{173}, y4{299}, y5{126}, y6{302}, y7{126}, y8{201};
        i < static_cast<int>(draw_type) / 8; ++i){
            constexpr int add = 241;
            painter.drawLine(QPoint(static_cast<int>(DrawGeometry::XVerticalLine) + add,
                                y1),
                             QPoint(static_cast<int>(DrawGeometry::XVerticalLine) + add,
                                y2));

            constexpr int mul = 8;
            y1 += static_cast<int>(DrawGeometry::Step) * mul;
            y2 += static_cast<int>(DrawGeometry::Step) * mul;

            constexpr int add_2 = 362;
            painter.drawLine(QPoint(static_cast<int>(DrawGeometry::XVerticalLine) + add_2,
                                    y3),
                             QPoint(static_cast<int>(DrawGeometry::XVerticalLine) + add_2,
                                    y4));
            constexpr int mul_2 = 5;
            constexpr int add_3 = 5;
            y3 += static_cast<int>(DrawGeometry::Step) * mul_2 + add_3;
            y4 += static_cast<int>(DrawGeometry::Step) * mul_2 + add_3;

            // 2 vertical lines(loosers 3rd columns)
            painter.drawLine(QPoint(912, y5), QPoint(912, y6));
            y5 += 352;
            y6 += 352;
            //-------------------------------------------------

            // two loosers semifinal lines
            painter.drawLine(QPoint(790, y7), QPoint(790, y8));
            y7 += 354;
            y8 += 354;

        }

    // drawing 1 vertical line ( semifinal column )
    constexpr int add = 483;
    painter.drawLine(QPoint(static_cast<int>(DrawGeometry::XVerticalLine) + add,
                                235),
                     QPoint(static_cast<int>(DrawGeometry::XVerticalLine) + add,
                                455));

    // 1 vertical line for bronze medal match
    painter.drawLine(QPoint(614, 570), QPoint(614, 659));

}

// void TourDraw::showTeamInfo()
// {
//     QMessageBox::information(this, "Team", "Team info");
// }
