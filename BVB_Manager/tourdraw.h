#ifndef TOURDRAW_H
#define TOURDRAW_H

#include <QDialog>

namespace Ui {
class TourDraw;
}

class TourDraw : public QDialog
{
    Q_OBJECT

public:
    explicit TourDraw(QWidget *parent = nullptr);
    ~TourDraw();

private:
    Ui::TourDraw *ui;
};

#endif // TOURDRAW_H
