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
        // auto comma_index = tournament.selected_teams[i]->text().indexOf("(");
        auto comma_index = tournament.selected_teams[rank_baskets[i]]->text().indexOf("(");

        // QString team_name = tournament.selected_teams[i]->text().left(comma_index - 1);
        QString team_name = tournament.selected_teams[rank_baskets[i]]->text().left(comma_index - 1);

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

        y += static_cast<int>(Geometry::Step);
    }

    // 8 teams buttons (2nd column) for the left side and the same for the right
    for(int i{0}, j{8}, y{32};
        i < static_cast<int>(draw_type) / 2; ++i){

        // left side(winners)--------------------------------------------------------------
        QPushButton *btn_left = new QPushButton(this);
        btn_left->setGeometry(QRect(131, y,
                               static_cast<int>(Geometry::BtnWidth),
                               static_cast<int>(Geometry::BtnHeight)));

        btn_left->setText("W " + QString::number(i + 1));

        // save pointer on the button
        W1_W8_buttons.push_back(btn_left);
        //------------------------------------------------------------------------

        // right side(loosers)-------------------------------------------------------------
        QPushButton *btn_right = new QPushButton(this);
        btn_right->setGeometry(QRect(1164, y,
                                    static_cast<int>(Geometry::BtnWidth),
                                    static_cast<int>(Geometry::BtnHeight)));

        btn_right->setDisabled(true);
        btn_right->setText("L " + QString::number(j));
        --j;
        //------------------------------------------------------------------------

        y += static_cast<int>(Geometry::Step) * 2;

        // container with pointers on these buttons????
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
        y += static_cast<int>(Geometry::Step) * 4;
        //----------------------------------------------------------------------

        // right side (loosers 2nd column winners)----------------------------------------
        QPushButton *btn_right_1 = new QPushButton(this);
        btn_right_1->setGeometry(QRect(1042, y2,
                                 static_cast<int>(Geometry::BtnWidth),
                                 static_cast<int>(Geometry::BtnHeight)));

        btn_right_1->setText("W " + QString::number(i + 13));
        y2 += static_cast<int>(Geometry::Step) * 4;
        //------------------------------------------------------------------------

        // right side (loosers 2nd column loosers)------------------------------------
        QPushButton *btn_right_2 = new QPushButton(this);
        btn_right_2->setGeometry(QRect(1042, y3,
                                       static_cast<int>(Geometry::BtnWidth),
                                       static_cast<int>(Geometry::BtnHeight)));

        btn_right_2->setText(loosers_labels[i]);
        y3 += 177;
        //----------------------------------------------------------------------------

        // loosers buttons right side (3 column)-------------------------------------
        QPushButton *looser_btn_3c = new QPushButton(this);
        looser_btn_3c->setGeometry(QRect(919, y4,
                                         static_cast<int>(Geometry::BtnWidth),
                                         static_cast<int>(Geometry::BtnHeight)));

        looser_btn_3c->setText("W " + QString::number(i + 17));
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
        y += static_cast<int>(Geometry::Step) * 5;
        //-----------------------------------------------------------------------

        // 3d place battle buttons-----------------------------------------------
        QPushButton *btn_bronze_match = new QPushButton(this);
        btn_bronze_match->setGeometry(QRect(494, y5,
                               static_cast<int>(Geometry::BtnWidth),
                               static_cast<int>(Geometry::BtnHeight)));

        btn_bronze_match->setText("L " + QString::number(i + 27));
        btn_bronze_match->setDisabled(true);
        y5 += static_cast<int>(Geometry::Step) * 2;
        //-----------------------------------------------------------------------

        // right side 2 buttons (4th column)--------------------------------------
        QPushButton *btn_right = new QPushButton(this);
        btn_right->setGeometry(QRect(796, y2,
                                        static_cast<int>(Geometry::BtnWidth),
                                        static_cast<int>(Geometry::BtnHeight)));

        btn_right->setText("W " + QString::number(i + 23));
        y2 += 355;
        //------------------------------------------------------------------------

        // right side 2 buttons (4th column from winners)-------------------------
        QPushButton *btn_right_2 = new QPushButton(this);
        btn_right_2->setGeometry(QRect(796, y3,
                                        static_cast<int>(Geometry::BtnWidth),
                                        static_cast<int>(Geometry::BtnHeight)));

        btn_right_2->setText("L " + QString::number(j));
        y3 += 355;
        --j;
        //------------------------------------------------------------------------

        // two loosers team for semifinal--------------------------------------------
        QPushButton *btn_right_3 = new QPushButton(this);
        btn_right_3->setGeometry(QRect(675, y4,
                                       static_cast<int>(Geometry::BtnWidth),
                                       static_cast<int>(Geometry::BtnHeight)));

        btn_right_3->setText("W " + QString::number(i + 25));
        y4 += 360;
        //--------------------------------------------------------------------------

        // 2 win buttons(semifinal)-------------------------------------------------
        QPushButton *win_btn = new QPushButton(this);
        win_btn->setGeometry(QRect(373, y6,
                                   static_cast<int>(Geometry::BtnWidth),
                                   static_cast<int>(Geometry::BtnHeight)));

        win_btn->setText("W " + QString::number(i + 21));
        y6 += static_cast<int>(Geometry::Step) * 8;
        //--------------------------------------------------------------------------

        //2 loosers buttons(semifinal)-----------------------------------------------
        QPushButton *looser_btn = new QPushButton(this);
        looser_btn->setGeometry(QRect(373, y7,
                                      static_cast<int>(Geometry::BtnWidth),
                                      static_cast<int>(Geometry::BtnHeight)));

        looser_btn->setText("W " + QString::number(i + 25));
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

    // W1 buttons has been clicked
    //connect(W1_W8_buttons[0], SIGNAL(clicked()), this, SLOT(W1_clicked()));
    // pointers on slots
    // QVector<void (*)()> vector_of_slots_w1_w8 = {
    //     W1_clicked, W2_clicked, W3_clicked, W4_clicked,
    //     W5_clicked, W6_clicked, W7_clicked, W8_clicked
    // };

    QVector<void (*)()> vector_of_slots_w1_w8;
    void (*w1_ptr)();
    w1_ptr = W1_clicked;
    addFunction(vector_of_slots_w1_w8, w1_ptr);
    // addFunction(vector_of_slots_w1_w8, W2_clicked);
    // addFunction(vector_of_slots_w1_w8, W3_clicked);
    // addFunction(vector_of_slots_w1_w8, W4_clicked);
    // addFunction(vector_of_slots_w1_w8, W5_clicked);
    // addFunction(vector_of_slots_w1_w8, W6_clicked);
    // addFunction(vector_of_slots_w1_w8, W7_clicked);
    // addFunction(vector_of_slots_w1_w8, W8_clicked);

    // for(int i{0}; i < W1_W8_buttons.size(); ++i){
    //     connect(W1_W8_buttons[i], SIGNAL(clicked()), this, SLOT(vector_of_slots_w1_w8[i]));
    // }
}

TourDraw::~TourDraw()
{
    delete ui;
}

// adding function's pointer to the vector
void TourDraw::addFunction(QVector<void (*)()> &func_vector, void (*func)()){
    func_vector.push_back(func);
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

void TourDraw::W1_clicked()
{
    QMessageBox::information(this, "W1", "W1 clicked!");
}

void TourDraw::W2_clicked()
{
    QMessageBox::information(this, "W2", "W2 clicked!");
}

void TourDraw::W3_clicked()
{
    QMessageBox::information(this, "W3", "W3 clicked!");
}

void TourDraw::W4_clicked()
{
    QMessageBox::information(this, "W4", "W4 clicked!");
}

void TourDraw::W5_clicked()
{
    QMessageBox::information(this, "W5", "W5 clicked!");
}

void TourDraw::W6_clicked()
{
    QMessageBox::information(this, "W6", "W6 clicked!");
}

void TourDraw::W7_clicked()
{
    QMessageBox::information(this, "W7", "W7 clicked!");
}

void TourDraw::W8_clicked()
{
    QMessageBox::information(this, "W8", "W8 clicked!");
}

// void TourDraw::showTeamInfo()
// {
//     QMessageBox::information(this, "Team", "Team info");
// }
