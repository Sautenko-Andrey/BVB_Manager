#include "drawschema.h"
#include <QGraphicsOpacityEffect>
#include <QMessageBox>

DrawSchema::DrawSchema(QWidget* parent)
    : QDialog(parent),
      parent_dialog{parent}
{
    // change dialog's background color
    this->setStyleSheet("background-color: azure; color : black");

    // pointer on nowhere for out team
    loser_btn = new QPushButton(this);
    loser_btn->hide();

    // 8 loser buttons
    for(int i{0}; i < 8; ++i){
        QPushButton *loser_btn = new QPushButton(this);
        loser_btn->setGeometry(5000, 5000, 1, 1);
        loser_btn->hide();
        loosers_btns.push_back(loser_btn);
    }
}


/*
    Function allows users to automatically 'play' matches
    where one of the teams or both are abscent.
    It occurs when user add to tournament less teams
    than it requires.
    For example 16 teams tournament has been choosen,
    but 12 teams added only.
*/
void DrawSchema::moveForward(QPushButton *teamA, QPushButton *teamB,
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



void DrawSchema::setDB(QSqlDatabase *db)
{
    if(db != nullptr){
        this->db = db;
    }
}


/*
    Function simply uses moveForward() function for the same purpose
*/
void DrawSchema::moveTeams(const QList<QPushButton *> &teams,
                         QList<QPushButton *> &win_btns,
                         QList<QPushButton *> &loosers_btns,
                         QList<QPropertyAnimation *> &animations){

    // use moveTeam()
    for(int i{0}, j{0}; i < teams.size(); i += 2, ++j){
        moveForward(teams[i], teams[i + 1], win_btns[j], loosers_btns[j], animations[j]);
    }
}


/*
    Function triggers when user wants to add result for the particular game
*/
void DrawSchema::click_game(QPushButton *team_1, QPushButton *team_2,
                            QPushButton *winner_basket, QPushButton *loser_basket,
                            QPushButton *game_result_btn,
                            QPropertyAnimation *animation){

    if(team_1 && team_2 && winner_basket &&
       loser_basket && game_result_btn && animation){

        game_result = std::make_unique<GameResult>(team_1, team_2,
                                                   winner_basket, loser_basket,
                                                   getAllGamesResults(),
                                                   game_result_btn, this);

        // stop animation
        if(animation != nullptr && animation->state() == QAbstractAnimation::Running){

            animation->stop();

            // Remove the opacity effect (return button to its original state)
            winner_basket->setGraphicsEffect(nullptr);
            // return previous style
            winner_basket->setStyleSheet(team_btn_style);
        }

        game_result->show();
    }
}


/*
    Function fills all buttons with competitors names
*/
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


/*
    Function changes QPushButton's style
*/
void DrawSchema::changeBtnStyle(QPushButton *btn, const QSize &size,
                            const QString &css_style)
{
    if(btn){
        btn->setFixedSize(size);
        btn->setStyleSheet(css_style);
        btn->setToolTip(btn->text());
    }
}


/*
    Function creates animations for buttons
*/
void DrawSchema::createAnimation(std::pair<int,int> &&range,
                               const QList<QPushButton *> &buttons,
                               QList<QPropertyAnimation *> &animations,
                               int duration){

    if(range.first < buttons.size() && range.second < buttons.size() &&
       range.second > range.first){
        for(int i{range.first}; i < range.second; ++i){

            //Create an opacity effect and apply it to the button
            QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect;
            buttons[i]->setGraphicsEffect(opacityEffect);

            // Ensure the button starts with full opacity
            opacityEffect->setOpacity(1.0);  // Full opacity, button is fully visible at start

            // Create a QPropertyAnimation for opacity
            QPropertyAnimation *animation = new QPropertyAnimation(opacityEffect, "opacity");
            animation->setDuration(duration);  // 2 seconds
            animation->setStartValue(1.0);  // Full opacity
            animation->setEndValue(0.1);    // Fully transparent (invisible)
            animation->setLoopCount(-1);    // Infinite loop
            animation->setEasingCurve(QEasingCurve::InOutQuad);  // Smooth easing
            animations.push_back(animation);
        }
    }
}


/*
    Function shows list of all games results
*/
void DrawSchema::showAllGamesResults()
{
    QString result;
    for(auto it{getAllGamesResults()->begin()}; it != getAllGamesResults()->end();++it){
        result += *it + "\n";
    }

    QMessageBox::information(this, "Games results", std::move(result));
}
