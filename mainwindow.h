#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *mediaPlayer;
    QVideoWidget *videoWidget;
    QString formatTime(qint64 milliseconds);
    bool isFullScreen;  // Add this to track fullscreen state
    QRect normalGeometry;

private slots:
    void openFile();
    void play();
    void pause();
    void stop();
    void updateDuration(qint64 duration);
    void updatePosition(qint64 position);
    void setPosition(int position);
    void toggleFullScreen();  // Add this for fullscreen functionality
    void keyPressEvent(QKeyEvent *event) override;  // Add this to handle ESC key
};

#endif // MAINWINDOW_H