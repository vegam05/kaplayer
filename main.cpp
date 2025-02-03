#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
    // Force X11 backend
    qputenv("QT_QPA_PLATFORM", "xcb");
    
    // Force GStreamer backend
    qputenv("QT_MEDIA_BACKEND", "gstreamer");
    
    // Additional GStreamer debug
    qputenv("GST_DEBUG", "2");
    
    // Use desktop OpenGL with NVIDIA
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}