#include "drawschema.h"


DrawSchema::DrawSchema(QWidget* parent): QDialog(parent), parent_dialog{parent}
{
    // change dialog's background color
    this->setStyleSheet("background-color: azure; color : black");

    // pointer on nowhere for out team
    loser_btn = new QPushButton(this);
    loser_btn->hide();
}

void DrawSchema::moveForward(QPushButton *teamA, QPushButton *teamB,
                           QPushButton *win_btn, QPushButton *los_btn,
                           QPropertyAnimation *animation){

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

void DrawSchema::setDB(QSqlDatabase *db)
{
    if(db != nullptr){
        this->db = db;
    }
}


void DrawSchema::moveTeams(const QList<QPushButton *> &teams,
                         QList<QPushButton *> &win_btns,
                         QList<QPushButton *> &loosers_btns,
                         QList<QPropertyAnimation *> &animations){

    // use moveTeam()
    for(int i{0}, j{0}; i < teams.size(); i += 2, ++j){
        moveForward(teams[i], teams[i + 1], win_btns[j], loosers_btns[j], animations[j]);
    }
}


void DrawSchema::click_game(QPushButton *team_1, QPushButton *team_2,
                          QPushButton *winner_basket, QPushButton *loser_basket,
                          QPushButton *game_result_btn, QPropertyAnimation *animation){

    game_result = std::make_unique<GameResult>(team_1, team_2,
                                               winner_basket, loser_basket,
                                               game_result_btn,
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

void DrawSchema::fillTeamBtns(Net net, const QList<QPushButton *> &all_teams,
                          const QSize &btn_size)
{
    if(all_teams.empty()){
        throw std::invalid_argument{"Teams list is empty."};
    }

    for(int i{0}; i < static_cast<int>(net); ++i){
        // get players names

        auto comma_index =
            getTournament()->selected_teams[i]->text().indexOf("(");

        QString team_name =
            getTournament()->selected_teams[i]->text().left(comma_index - 1);

        all_teams[i]->setFixedSize(btn_size);

        all_teams[i]->setDisabled(true);

        all_teams[i]->setText(team_name);

        fontAdapter(all_teams[i]);

        if(all_teams[i]->text() == "None, None, None"){
            all_teams[i]->setStyleSheet(team_btn_style_for_none);
        }
        else{
            all_teams[i]->setStyleSheet(team_btn_style);
        }
    }
}

void DrawSchema::changeBtnStyle(QPushButton *btn, const QSize &size,
                            const QString &css_style)
{
    if(btn != nullptr){
        btn->setFixedSize(size);
        btn->setStyleSheet(css_style);
    }
}

// create animations for butoons
void DrawSchema::createAnimation(std::pair<int,int> &&range,
                               const QList<QPushButton *> &buttons,
                               QList<QPropertyAnimation *> &animations,
                               int duration){

    for(int i{range.first}; i < range.second; ++i){
        auto animation = new QPropertyAnimation(buttons[i], "geometry");
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
