#ifndef CALENDAR_H
#define CALENDAR_H

#include <QDialog>
#include <QDate>

namespace Ui {
class Calendar;
}

class Calendar : public QDialog
{
    Q_OBJECT

public:
    explicit Calendar(QWidget *parent = nullptr);
    ~Calendar();

    const QDate& getDate() const { return selected_date; }

private slots:
    void on_pushButton_clicked();

    void dateChanged();

private:
    Ui::Calendar *ui;

    QDate selected_date;
};

#endif // CALENDAR_H
