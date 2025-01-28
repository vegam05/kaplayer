#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QKeyEvent>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QTimer> 

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mediaPlayer(new QMediaPlayer(this))
    , videoWidget(new QVideoWidget(this))
    , controlHideTimer(new QTimer(this)) 
    , isPlaying(false)
    , isFullScreen(false)
{
    ui->setupUi(this);

    
    mediaPlayer->setVideoOutput(videoWidget);
    ui->videoLayout->addWidget(videoWidget);

    
    controlHideTimer->setInterval(3000); 
    connect(controlHideTimer, &QTimer::timeout, this, &MainWindow::hideControls);


    videoWidget->setMouseTracking(true);
    videoWidget->installEventFilter(this);

    
    connect(ui->openButton, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(ui->playButton, &QPushButton::clicked, this, &MainWindow::play);
    connect(ui->pauseButton, &QPushButton::clicked, this, &MainWindow::pause);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stop);
    connect(ui->fullscreenButton, &QPushButton::clicked, this, &MainWindow::toggleFullScreen);
    connect(ui->positionSlider, &QSlider::sliderMoved, this, &MainWindow::setPosition);
    connect(ui->volumeSlider, &QSlider::valueChanged, mediaPlayer, &QMediaPlayer::setVolume);

    
    ui->volumeSlider->setValue(50);
    mediaPlayer->setVolume(50);

    
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &MainWindow::updatePosition);
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &MainWindow::updateDuration);

    
    ui->pauseButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete mediaPlayer;
    delete videoWidget;
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F11) {
        toggleFullScreen();
    } else if (event->key() == Qt::Key_Escape && isFullScreen) {
        toggleFullScreen(); 
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::hideControls()
{
    if (isFullScreen) {
        ui->openButton->setVisible(false);
        ui->playButton->setVisible(false);
        ui->pauseButton->setVisible(false);
        ui->stopButton->setVisible(false);
        ui->positionSlider->setVisible(false);
        ui->currentTimeLabel->setVisible(false);
        ui->totalTimeLabel->setVisible(false);
        ui->volumeSlider->setVisible(false);
        ui->volumeLabel->setVisible(false);
        ui->fullscreenButton->setVisible(false);
    }
}

void MainWindow::showControls()
{
    if (isFullScreen) {
        ui->openButton->setVisible(true);
        ui->playButton->setVisible(true);
        ui->pauseButton->setVisible(true);
        ui->stopButton->setVisible(true);
        ui->positionSlider->setVisible(true);
        ui->currentTimeLabel->setVisible(true);
        ui->totalTimeLabel->setVisible(true);
        ui->volumeSlider->setVisible(true);
        ui->volumeLabel->setVisible(true);
        ui->fullscreenButton->setVisible(true);
        controlHideTimer->start(); 
    }
}

void MainWindow::toggleFullScreen()
{
    if (isFullScreen) {
        showNormal(); 
        videoWidget->setFullScreen(false); 
        showControls(); 
        controlHideTimer->stop(); 
    } else {
        showFullScreen(); 
        videoWidget->setFullScreen(true); 
        showControls(); 
        controlHideTimer->start(); 
    }
    isFullScreen = !isFullScreen;
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Video File", "", "Videos (*.mp4 *.avi *.mkv)");
    if (!fileName.isEmpty()) {
        mediaPlayer->setMedia(QUrl::fromLocalFile(fileName));
        play();
    }
}

void MainWindow::play()
{
    mediaPlayer->play();
    isPlaying = true;
    ui->playButton->setEnabled(false);
    ui->pauseButton->setEnabled(true);
}

void MainWindow::pause()
{
    mediaPlayer->pause();
    isPlaying = false;
    ui->playButton->setEnabled(true);
    ui->pauseButton->setEnabled(false);
}

void MainWindow::stop() {
    mediaPlayer->stop();
    isPlaying = false;
    ui->playButton->setEnabled(true);
    ui->pauseButton->setEnabled(false);
}

void MainWindow::setPosition(int position)
{
    mediaPlayer->setPosition(position);
}

void MainWindow::updatePosition(qint64 position)
{
    ui->positionSlider->setValue(position);
    qint64 seconds = position / 1000;
    QString time = QString("%1:%2").arg(seconds / 60, 2, 10, QChar('0')).arg(seconds % 60, 2, 10, QChar('0'));
    ui->currentTimeLabel->setText(time);
}

void MainWindow::updateDuration(qint64 duration)
{
    ui->positionSlider->setRange(0, duration);
    qint64 seconds = duration / 1000;
    QString time = QString("%1:%2").arg(seconds / 60, 2, 10, QChar('0')).arg(seconds % 60, 2, 10, QChar('0'));
    ui->totalTimeLabel->setText(time);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == videoWidget && event->type() == QEvent::MouseMove) {
        showControls(); 
    return QMainWindow::eventFilter(obj, event);
}