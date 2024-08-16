#include "calendar.h"
#include "ui_calendar.h"

Calendar::Calendar(QDateEdit *start_date,
                   QDateEdit *end_date,
                   QSpinBox *duration,
                   QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Calendar)
{
    ui->setupUi(this);

    if(start_date != nullptr && end_date != nullptr && duration != nullptr){
        start_tour_date = start_date;
        end_tour_date = end_date;
        tour_duration = duration;
    }

    // view settings
    ui->calendarWidget->setLocale(QLocale::English);
    ui->calendarWidget->setGridVisible(true);
    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    ui->calendarWidget->setFirstDayOfWeek(Qt::Monday);

}

Calendar::~Calendar()
{
    delete ui;
}

void Calendar::on_pushButton_clicked()
{
    // get selected date and change desired date widgets
    auto date = ui->calendarWidget->selectedDate();

    start_tour_date->setDate(date);
    end_tour_date->setDate(date);

    // drop duration value
    tour_duration->setValue(0);;

    QDialog::accept();
}
