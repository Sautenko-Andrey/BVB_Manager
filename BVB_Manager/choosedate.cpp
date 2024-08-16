#include "choosedate.h"
#include "ui_choosedate.h"

ChooseDate::ChooseDate(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChooseDate)
{
    ui->setupUi(this);
}

ChooseDate::~ChooseDate()
{
    delete ui;
}
