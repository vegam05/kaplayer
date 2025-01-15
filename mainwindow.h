#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QGraphicsVideoItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEffect>
#include <QGraphicsColorizeEffect> 

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
    QGraphicsVideoItem *videoItem;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsColorizeEffect *colorEffect;

private slots:
    void openFile();
    void play();
    void pause();
    void stop();
    void updateDuration(qint64 duration);
    void updatePosition(qint64 position);
    void setPosition(int position);
    void setBrightness(int value); // New slot for brightness
};

#endif // MAINWINDOW_H