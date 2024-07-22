#include "bvb_manager.h"

#include <QApplication>
#include <QRect>
#include "utils.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BVB_Manager manager;
    manager.setWindowTitle("BVB Manager");
    manager.setStyleSheet(BACKGROUND_COLOR);
    manager.setAutoFillBackground( true );
    manager.setGeometry(QRect(0, 0,
                              static_cast<int>(Sizes::MainWindowWidth),
                              static_cast<int>(Sizes::MainWindowHeight)));
    manager.show();
    return a.exec();
}
