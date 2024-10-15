#ifndef DRAW32_H
#define DRAW32_H

#include <QDialog>
#include "drawschema.h"

namespace Ui {
class Draw32;
}

class Draw32 : public DrawSchema
{
    Q_OBJECT

public:
    explicit Draw32(QSqlDatabase &db,
                    Tournament *tour, QWidget *parent = nullptr);
    ~Draw32();

private:
    Ui::Draw32 *ui;
};

#endif // DRAW32_H
