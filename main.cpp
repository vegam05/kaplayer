#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    // Set OpenGL configuration before creating QApplication
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    // Only if above fails, try software rendering:
    // QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}