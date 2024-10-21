#ifndef DRAW24_H
#define DRAW24_H

#include <QDialog>
#include "drawschema.h"

namespace Ui {
class Draw24;
}

class Draw24 : public DrawSchema
{
    Q_OBJECT

public:
    Draw24(QSqlDatabase &db,
           Tournament *tour, QWidget *parent = nullptr);
    ~Draw24();

private slots:
    void on_allResultsBtn_clicked();

private:
    Ui::Draw24 *ui;

    QList<QPushButton *> W24_W17_btns;
};

#endif // DRAW24_H
