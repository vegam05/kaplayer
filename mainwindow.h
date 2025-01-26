#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QSlider>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void openFile();
    void play();
    void pause();
    void stop();
    void setPosition(int position);
    void updatePosition(qint64 position);
    void updateDuration(qint64 duration);
    void toggleFullScreen();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *mediaPlayer;
    QVideoWidget *videoWidget;
    bool isPlaying = false;
    bool isFullScreen = false;
};

#endif // MAINWINDOW_H
