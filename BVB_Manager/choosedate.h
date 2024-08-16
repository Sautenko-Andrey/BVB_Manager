#ifndef CHOOSEDATE_H
#define CHOOSEDATE_H

#include <QDialog>

namespace Ui {
class ChooseDate;
}

class ChooseDate : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseDate(QWidget *parent = nullptr);
    ~ChooseDate();

private:
    Ui::ChooseDate *ui;
};

#endif // CHOOSEDATE_H
