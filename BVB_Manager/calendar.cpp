#include "calendar.h"
#include "ui_calendar.h"

Calendar::Calendar(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Calendar)
{
    ui->setupUi(this);

    // view settings
    ui->calendarWidget->setLocale(QLocale::English);
    ui->calendarWidget->setGridVisible(true);
    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    ui->calendarWidget->setFirstDayOfWeek(Qt::Monday);

    // signals & slots
    connect(ui->calendarWidget, SIGNAL(clicked(QDate)), this, SLOT(dateChanged()));
}

Calendar::~Calendar()
{
    delete ui;
}

void Calendar::on_pushButton_clicked()
{
    // save selected date
    selected_date = ui->calendarWidget->selectedDate();

    QDialog::accept();
}

void Calendar::dateChanged()
{
    selected_date = ui->calendarWidget->selectedDate();
}

