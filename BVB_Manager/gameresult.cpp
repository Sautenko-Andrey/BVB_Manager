#include "gameresult.h"
#include "ui_gameresult.h"
#include <QMessageBox>
#include "utils.h"

GameResult::GameResult(QPushButton *team_1,
                       QPushButton *team_2,
                       QPushButton *winer_basket,
                       QPushButton *loser_basket,
                       QStringList *results_list,
                       QPushButton *game_res_btn,
                       QWidget *parent)
    : QDialog(parent)
    , winner{winer_basket}
    , loser{loser_basket}
    , first_team{team_1}
    , second_team{team_2}
    , games_results_list{results_list}
    , game_result{game_res_btn}
    , ui(new Ui::GameResult)
{
    ui->setupUi(this);

    ui->okButton->setStyleSheet(ok_button_style);

    ui->firstSpinBox->setStyleSheet(game_result_spinbox_style);

    ui->secSpinBox->setStyleSheet(game_result_spinbox_style);

    this->setStyleSheet("background-color: DarkSlateGray; color : Aqua");

    setWindowTitle(team_1->text() + " vs " + team_2->text());

    ui->teamLabel_1->setText(team_1->text());
    ui->teamLabel_2->setText(team_2->text());

    ui->teamLabel_1->setStyleSheet(team_label_style);
    ui->teamLabel_2->setStyleSheet(team_label_style);

    // constraints
    ui->firstSpinBox->setMinimum(0);
    ui->firstSpinBox->setMaximum(2);
    ui->secSpinBox->setMinimum(0);
    ui->secSpinBox->setMaximum(2);

    // signals & slots
    connect(ui->firstSpinBox, SIGNAL(valueChanged(int)), this, SLOT(resultTeam1Changed()));
    connect(ui->secSpinBox, SIGNAL(valueChanged(int)), this, SLOT(resultTeam2Changed()));
}

GameResult::~GameResult()
{
    delete ui;
}


/*
    Function triggers when user changes result in a match
    for the team #1
*/
void GameResult::resultTeam1Changed(){

    if(ui->firstSpinBox->value() == won_two_sets){
        ui->secSpinBox->setValue(won_one_set);
    }
    else if(ui->firstSpinBox->value() == won_one_set){
        ui->secSpinBox->setValue(won_two_sets);
    }
}


/*
    Function triggers when user changes result in a match
    for the team #2
*/
void GameResult::resultTeam2Changed(){
    if(ui->secSpinBox->value() == won_two_sets){
        ui->firstSpinBox->setValue(won_one_set);
    }
    else if(ui->secSpinBox->value() == won_one_set){
        ui->firstSpinBox->setValue(won_two_sets);
    }
}


/*
    Function triggers when user push "Ok" button
    and it write general result in the game result button
*/
void GameResult::on_okButton_clicked()
{
    int first_team_score = ui->firstSpinBox->value();
    int second_team_score = ui->secSpinBox->value();

    // match result can't be 0 : 0
    if(constexpr int zero_scored{0};
        first_team_score == zero_scored &&
        second_team_score == zero_scored)
    {
        QMessageBox::warning(this, "Match result error", "Match result can't be 0:0");
        return;
    }

    if(constexpr int two_scored{2}; first_team_score == two_scored)
    {
        winner->setText(first_team->text());
        winner->setToolTip(first_team->text());
        loser->setText(second_team->text());
        loser->setToolTip(second_team->text());
    }
    else{
        winner->setText(second_team->text());
        winner->setToolTip(second_team->text());
        loser->setText(first_team->text());
        loser->setToolTip(first_team->text());
    }

    // change font if necessary
    fontAdapter(winner);
    fontAdapter(loser);

    // if user use push button for game result
    if(game_result != nullptr){
        game_result->setText(QString::number(first_team_score) +
                             " : " +
                             QString::number(second_team_score));
        game_result->setStyleSheet(game_result_style_2);
    }
    else{
        // if we just save result in a container as a string
        games_results_list->emplace_back(first_team->text() + " vs " +
                                         second_team->text() + " ( " +
                                         QString::number(first_team_score) +
                                         " : " +
                                         QString::number(second_team_score) + " )");
    }

    QDialog::accept();
}
