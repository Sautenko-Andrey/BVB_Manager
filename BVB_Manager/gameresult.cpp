#include "gameresult.h"
#include "ui_gameresult.h"
#include <QDebug>
#include <QMessageBox>

GameResult::GameResult(QPushButton *team_1,
                       QPushButton *team_2,
                       QPushButton *winer_basket,
                       QPushButton *loser_basket,
                       QWidget *parent)
    : QDialog(parent)
    , winner{winer_basket}
    , loser{loser_basket}
    , first_team{team_1}
    , second_team{team_2}
    , ui(new Ui::GameResult)
{
    ui->setupUi(this);

    setWindowTitle(team_1->text() + " vs " + team_2->text());

    ui->teamLabel_1->setText(team_1->text());
    ui->teamLabel_2->setText(team_2->text());

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

void GameResult::resultTeam1Changed(){
    if(ui->firstSpinBox->value() == 2){
        ui->secSpinBox->setValue(1);
    }
    else if(ui->firstSpinBox->value() == 1){
        ui->secSpinBox->setValue(2);
    }
}

void GameResult::resultTeam2Changed(){
    if(ui->secSpinBox->value() == 2){
        ui->firstSpinBox->setValue(1);
    }
    else if(ui->secSpinBox->value() == 1){
        ui->firstSpinBox->setValue(2);
    }
}

void GameResult::on_okButton_clicked()
{
    int first_team_score = ui->firstSpinBox->value();
    int second_team_score = ui->secSpinBox->value();

    QString first_team_name = first_team->text();
    QString second_team_name = second_team->text();

    // match result can't be 0 : 0
    if(first_team_score == 0 && second_team_score == 0){
        QMessageBox::warning(this, "Match result error", "Match result can't be 0:0");
        return;
    }

    if(first_team_score == 2){
        winner->setText(first_team->text());
        loser->setText(second_team->text());
    }
    else{
        winner->setText(second_team->text());
        loser->setText(first_team->text());
    }

    // show game result (sets)
    //first_team->setText(first_team_name + " <" + QString::number(first_team_score) + ">");
    //second_team->setText(second_team_name + " <" + QString::number(second_team_score) + ">");

    QDialog::accept();
}
