#ifndef CALENDAR_H
#define CALENDAR_H

#include <QDialog>
#include <QDateEdit>
#include <QSpinBox>

namespace Ui {
class Calendar;
}

class Calendar : public QDialog
{
    Q_OBJECT

public:
    Calendar(QDateEdit *start_date,
                      QDateEdit *end_date,
                      QSpinBox *duration,
                      QWidget *parent = nullptr);
    ~Calendar();

private slots:
    void on_pushButton_clicked();

    //void dateChanged();

private:
    Ui::Calendar *ui;

    QDateEdit *start_tour_date{nullptr};

    QDateEdit *end_tour_date{nullptr};

    QSpinBox *tour_duration{nullptr};

};

#endif // CALENDAR_H
