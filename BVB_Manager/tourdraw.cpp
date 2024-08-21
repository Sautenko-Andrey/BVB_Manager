#include "tourdraw.h"
#include "ui_tourdraw.h"

TourDraw::TourDraw(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TourDraw)
{
    ui->setupUi(this);
}

TourDraw::~TourDraw()
{
    delete ui;
}
