#include "doubleeliminationtournament.h"
#include "ui_doubleeliminationtournament.h"
#include <QPainter>

DoubleEliminationTournament::DoubleEliminationTournament(QSqlDatabase &database,
                                                         TournamentMode mode,
                                                         QWidget *parent)
    : QDialog(parent)
    , tournament_mode{mode}
    , ui(new Ui::DoubleEliminationTournament)
{
    ui->setupUi(this);

    // pointer on database
    db = &database;

    //tournament_mode = mode;
}

DoubleEliminationTournament::~DoubleEliminationTournament()
{
    delete ui;
}

void DoubleEliminationTournament::paintEvent(QPaintEvent *event)
{
    // Drawing a text
    QPainter my_painter(this);

    my_painter.setFont(QFont("Times", 18));

    QString teams_amount;

    switch (tournament_mode) {
    case TournamentMode::ofSixteen:

        teams_amount = QString::number(static_cast<int>(TournamentMode::ofSixteen));

        // drawing a rects with numbers

        for(int i{0}, left_x{20}, left_y{30}, right_x{30}, right_y{30};
            i < static_cast<int>(TournamentMode::ofSixteen); ++i){

                my_painter.drawRect(QRect(left_x, left_y, right_x, right_y));

                // draw numbers in rects
                my_painter.drawText(QPoint(left_x + 3, left_y + 23),
                                    QString::number(i + 1));

                // drawing lines
                //my_painter.drawLine(QPoint(50, 45), QPoint(200, 45));
                my_painter.drawLine(QPoint(50, left_y + 15), QPoint(200, left_y + 15));

                left_y += 40;
        }

        //my_painter.drawRect(QRect(20, 20, 30, 30));
        //my_painter.drawRect(QRect(20, 60, 30, 30));

        break;
    case TournamentMode::ofThirtyTwo:
        teams_amount = QString::number(static_cast<int>(TournamentMode::ofThirtyTwo));
        break;
    case TournamentMode::ofSixtyFour:
        teams_amount = QString::number(static_cast<int>(TournamentMode::ofSixtyFour));
        break;
    }

    // Drawing a text
    my_painter.drawText(QPoint(575, 30), "Main draw ( " + teams_amount + " teams )");

    // Drawing a line
    //my_painter.drawLine(QPoint(50, 50), QPoint(200, 200));

    // Drawing a rect
    //my_painter.drawRect(QRect(20, 20, 30, 30));
}
