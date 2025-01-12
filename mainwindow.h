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
    QString formatTime(qint64 milliseconds);  // Add this helper function

private slots:
    void openFile();
    void play();
    void pause();
    void stop();
    void updateDuration(qint64 duration);     // Add this
    void updatePosition(qint64 position);     // Add this
    void setPosition(int position);           // Add this
};

#endif // MAINWINDOW_H