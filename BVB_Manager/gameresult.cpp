#include "gameresult.h"
#include "ui_gameresult.h"
#include <QDebug>
#include <QMessageBox>

GameResult::GameResult(const QString team_1,
                       const QString team_2,
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

    setWindowTitle(team_1 + " vs " + team_2);

    ui->teamLabel_1->setText(team_1);
    ui->teamLabel_2->setText(team_2);

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
    // match result can't be 0 : 0
    if(ui->firstSpinBox->value() == 0 && ui->secSpinBox->value() == 0){
        QMessageBox::warning(this, "Match result error", "Match result can't be 0:0");
        return;
    }

    if(ui->firstSpinBox->value() == 2){
        winner->setText(first_team);
        loser->setText(second_team);
    }
    else{
        winner->setText(second_team);
        loser->setText(first_team);
    }

    QDialog::accept();
}
