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
    explicit Draw24(QWidget *parent = nullptr);
    ~Draw24();

private:
    Ui::Draw24 *ui;
};

#endif // DRAW24_H
