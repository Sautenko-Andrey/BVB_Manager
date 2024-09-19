#include "gameresult.h"
#include "ui_gameresult.h"
#include <QDebug>
#include <QMessageBox>
#include "utils.h"

GameResult::GameResult(QPushButton *team_1,
                       QPushButton *team_2,
                       QPushButton *winer_basket,
                       QPushButton *loser_basket,
                       QPushButton *game_res_btn,
                       QWidget *parent)
    : QDialog(parent)
    , winner{winer_basket}
    , loser{loser_basket}
    , first_team{team_1}
    , second_team{team_2}
    , game_result{game_res_btn}
    , ui(new Ui::GameResult)
{
    ui->setupUi(this);

    ui->okButton->setStyleSheet("QPushButton {"
            "    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
            "    stop:0 rgba(50, 50, 50, 255), stop:1 rgba(10, 10, 10, 255));"
            "    border: 2px solid #555;"
            "    border-radius: 10px;"
            "    color: Aqua;"
            "    padding: 5px 10px;"
            "    font-size: 16px;"
            "}"
            "QPushButton:hover {"
            "    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
            "    stop:0 rgba(70, 70, 70, 255), stop:1 rgba(20, 20, 20, 255));"
            "}"
            "QPushButton:pressed {"
            "    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
            "    stop:0 rgba(30, 30, 30, 255), stop:1 rgba(5, 5, 5, 255));"
            "}");

    this->setStyleSheet("background-color: DarkSlateGray; color : Aqua");

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

    // change font if necessary
    fontAdapter(winner);
    fontAdapter(loser);

    game_result->setText(QString::number(first_team_score) +
                         " : " +
                         QString::number(second_team_score));
    game_result->setStyleSheet("QPushButton {"
                               "    background-color: #2e2e2e;"   // Dark grey color
                               "    color: DeepPink;"                // text for contrast
                               "    padding: 15px 5px;"          // Add padding to the button
                               "    border: 2px solid #1a1a1a;"   // Slightly darker grey border
                               "    border-radius: 8px;"          // Rounded corners
                               "    font-size: 16px;"             // Font size
                               "    font-weight: bold;"           // Bold text
                               "    text-align: center;"          // Center the text
                               "}");

    QDialog::accept();
}
