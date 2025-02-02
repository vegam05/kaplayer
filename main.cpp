#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    qputenv("QT_QPA_PLATFORM", "xcb"); // Force X11 backend
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}

