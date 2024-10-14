#include "draw24.h"
#include "ui_draw24.h"

Draw24::Draw24(QWidget *parent)
    : DrawSchema(parent)
    , ui(new Ui::Draw24)
{
    ui->setupUi(this);
}

Draw24::~Draw24()
{
    delete ui;
}
