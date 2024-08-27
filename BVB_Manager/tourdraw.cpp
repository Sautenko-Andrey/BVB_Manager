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

    // 8 teams buttons (2nd column)
    for(int i{0}, y{static_cast<int>(DrawGeometry::coordYStartBtn) + 22};
        i < static_cast<int>(draw_type) / 2; ++i){

        QPushButton *btn = new QPushButton(this);
        btn->setGeometry(QRect(static_cast<int>(DrawGeometry::coordXStartBtn) +
                               static_cast<int>(DrawGeometry::StepX),
                               y,
                               static_cast<int>(DrawGeometry::BtnWidth),
                               static_cast<int>(DrawGeometry::BtnHeight)));

        btn->setText("W " + QString::number(i + 1));
        y += static_cast<int>(DrawGeometry::Step) * 2;

        // container with pointers on these buttons????
    }


    // 4 teams buttons (3rd column)
    for(int i{0}, y{static_cast<int>(DrawGeometry::coordYStartBtn) + 64};
        i < static_cast<int>(draw_type) / 4; ++i){

        QPushButton *btn = new QPushButton(this);
        btn->setGeometry(QRect(static_cast<int>(DrawGeometry::coordXStartBtn) +
                               static_cast<int>(DrawGeometry::StepX) * 2,
                               y,
                               static_cast<int>(DrawGeometry::BtnWidth),
                               static_cast<int>(DrawGeometry::BtnHeight)));

        btn->setText("W " + QString::number(i + 9));
        y += static_cast<int>(DrawGeometry::Step) * 4;
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

        //2 loosers buttons
        QPushButton *looser_btn = new QPushButton(this);
        looser_btn->setGeometry(QRect(static_cast<int>(DrawGeometry::coordXStartBtn) +
                                      static_cast<int>(DrawGeometry::StepX) * 3,
                                      y2,
                                      static_cast<int>(DrawGeometry::BtnWidth),
                                      static_cast<int>(DrawGeometry::BtnHeight)));
        looser_btn->setText("W " + QString::number(i + 25));
        y2 += static_cast<int>(DrawGeometry::Step) * 2 + 7;
    }

    // 2  team buttons (final column)
    for(int i{0}, y{static_cast<int>(DrawGeometry::coordYStartBtn) + 210};
        i < static_cast<int>(draw_type) / 8; ++i){

        QPushButton *btn = new QPushButton(this);
        btn->setGeometry(QRect(static_cast<int>(DrawGeometry::coordXStartBtn) +
                               static_cast<int>(DrawGeometry::StepX) * 4,
                               y,
                               static_cast<int>(DrawGeometry::BtnWidth),
                               static_cast<int>(DrawGeometry::BtnHeight)));

        btn->setText("W " + QString::number(i + 27));
        y += static_cast<int>(DrawGeometry::Step) * 5;
    }

    // 1 team button (winner column)
    QPushButton *btn = new QPushButton(this);

    btn->setGeometry(QRect(static_cast<int>(DrawGeometry::coordXStartBtn) +
                           static_cast<int>(DrawGeometry::StepX) * 5,
                           static_cast<int>(DrawGeometry::coordYStartBtn) + 320,
                           static_cast<int>(DrawGeometry::BtnWidth),
                           static_cast<int>(DrawGeometry::BtnHeight)));
    btn->setText("1st place");

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

    // drawing 16 horizonthal lines ( 1st column )
    for(int i{0}, y{26}; i < static_cast<int>(draw_type); ++i){
        painter.drawLine(QPoint(125, y), QPoint(130, y));
        y += static_cast<int>(DrawGeometry::Step);
    }

    // drawing 8 horizontal lines ( 2nd column )
    for(int i{0}, y{47}; i < static_cast<int>(draw_type) / 2; ++i){
        painter.drawLine(QPoint(245, y), QPoint(250, y));
        y += static_cast<int>(DrawGeometry::Step) * 2;
    }

    // drawing 4 horizontal lines ( 3rd column )
    for(int i{0}, y{90}; i < static_cast<int>(draw_type) / 4; ++i){

        painter.drawLine(QPoint(365, y), QPoint(370, y));
        y += static_cast<int>(DrawGeometry::Step) * 4;
    }

    // drawing 4 horizontal lines ( 4 column )
    for(int i{0}, y1{173}, y2{300}; i < static_cast<int>(draw_type) / 8; ++i){

        painter.drawLine(QPoint(487, y1), QPoint(492, y1));
        y1 += static_cast<int>(DrawGeometry::Step) * 8;

        painter.drawLine(QPoint(487, y2), QPoint(492, y2));
        y2 += static_cast<int>(DrawGeometry::Step) * 2 + 10;
    }

    // drawing 2 horizontal lines ( semifinals column )
    for(int i{0}, y{235}; i < static_cast<int>(draw_type) / 8; ++i){
        painter.drawLine(QPoint(607, y), QPoint(612, y));
        y += static_cast<int>(DrawGeometry::Step) * 5;
    }

    // drawing 8 vertical lines ( 1st column )
    for(int i{0}, y1{26}, y2{70}; i < static_cast<int>(draw_type) / 2; ++i){
        painter.drawLine(QPoint(static_cast<int>(DrawGeometry::XVerticalLine),
                         y1),
                         QPoint(static_cast<int>(DrawGeometry::XVerticalLine),
                         y2));
        y1 += static_cast<int>(DrawGeometry::Step) * 2;
        y2 += static_cast<int>(DrawGeometry::Step) * 2;
    }


    // drawing 4 vertical lines ( 2nd column )
    for(int i{0}, y1{47}, y2{135}; i < static_cast<int>(draw_type) / 4; ++i){

        constexpr int add = 120;
        painter.drawLine(QPoint(static_cast<int>(DrawGeometry::XVerticalLine) + add,
                                y1),
                         QPoint(static_cast<int>(DrawGeometry::XVerticalLine) + add,
                                y2));
        y1 += static_cast<int>(DrawGeometry::Step) * 4;
        y2 += static_cast<int>(DrawGeometry::Step) * 4;
    }

    // drawing 4 vertical lines ( 3rd column )
    for(int i{0}, y1{90}, y2{266}, y3{173}, y4{299};
        i < static_cast<int>(draw_type) / 8; ++i){
            constexpr int add = 241;
            painter.drawLine(QPoint(static_cast<int>(DrawGeometry::XVerticalLine) + add,
                                y1),
                             QPoint(static_cast<int>(DrawGeometry::XVerticalLine) + add,
                                y2));

            y1 += static_cast<int>(DrawGeometry::Step) * 8;
            y2 += static_cast<int>(DrawGeometry::Step) * 8;

            constexpr int add_2 = 362;
            painter.drawLine(QPoint(static_cast<int>(DrawGeometry::XVerticalLine) + add_2,
                                    y3),
                             QPoint(static_cast<int>(DrawGeometry::XVerticalLine) + add_2,
                                    y4));
            y3 += static_cast<int>(DrawGeometry::Step) * 5 + 5;
            y4 += static_cast<int>(DrawGeometry::Step) * 5 + 5;
        }

    // drawing 1 vertical line ( semifinal column )
    constexpr int add = 483;
    painter.drawLine(QPoint(static_cast<int>(DrawGeometry::XVerticalLine) + add,
                                235),
                     QPoint(static_cast<int>(DrawGeometry::XVerticalLine) + add,
                                455));

}

// void TourDraw::showTeamInfo()
// {
//     QMessageBox::information(this, "Team", "Team info");
// }
