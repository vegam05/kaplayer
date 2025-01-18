#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QGraphicsScene>
#include <QGraphicsView>

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

private slots:
    void openFile();
    void play();
    void pause();
    void stop();
    void updateDuration(qint64 duration);
    void updatePosition(qint64 position);
    void setPosition(int position);
};

#endif // MAINWINDOW_H