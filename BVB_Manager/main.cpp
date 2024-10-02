#include "bvb_manager.h"

#include <QApplication>
#include <QRect>
#include "utils.h"
#include <QToolTip>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    BVB_Manager manager;
    manager.setWindowTitle("BVB Manager");
    manager.setStyleSheet("background-color: #2c3e50; color : white");
    manager.setAutoFillBackground( true );
    manager.setGeometry(QRect(0, 0,
                              static_cast<int>(Sizes::MainWindowWidth),
                              static_cast<int>(Sizes::MainWindowHeight)));
    manager.show();
    return app.exec();
}
