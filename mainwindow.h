#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QSlider>
#include <QTimer>  // Add this include

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
    bool eventFilter(QObject *obj, QEvent *event) override;  // Add this

private slots:
    void openFile();
    void play();
    void pause();
    void stop();
    void setPosition(int position);
    void updatePosition(qint64 position);
    void updateDuration(qint64 duration);
    void toggleFullScreen();
    void hideControls();   // Add this
    void showControls();   // Add this

private:
    Ui::MainWindow *ui;
    QMediaPlayer *mediaPlayer;
    QVideoWidget *videoWidget;
    QTimer *controlHideTimer;  // Add this
    bool isPlaying = false;
    bool isFullScreen = false;
};

#endif // MAINWINDOW_H