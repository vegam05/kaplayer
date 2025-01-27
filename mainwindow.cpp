#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QKeyEvent>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QTimer> // Include QTimer

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mediaPlayer(new QMediaPlayer(this))
    , videoWidget(new QVideoWidget(this))
    , controlHideTimer(new QTimer(this)) // Initialize controlHideTimer
    , isPlaying(false)
    , isFullScreen(false)
{
    ui->setupUi(this);

    // Set up video widget
    mediaPlayer->setVideoOutput(videoWidget);
    ui->videoLayout->addWidget(videoWidget);

    // Initialize the control hide timer
    controlHideTimer->setInterval(3000); // Hide controls after 3 seconds of inactivity
    connect(controlHideTimer, &QTimer::timeout, this, &MainWindow::hideControls);

    // Show controls when the mouse moves
    videoWidget->setMouseTracking(true);
    videoWidget->installEventFilter(this);

    // Connect UI elements to functionalities
    connect(ui->openButton, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(ui->playButton, &QPushButton::clicked, this, &MainWindow::play);
    connect(ui->pauseButton, &QPushButton::clicked, this, &MainWindow::pause);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stop);
    connect(ui->fullscreenButton, &QPushButton::clicked, this, &MainWindow::toggleFullScreen);
    connect(ui->positionSlider, &QSlider::sliderMoved, this, &MainWindow::setPosition);
    connect(ui->volumeSlider, &QSlider::valueChanged, mediaPlayer, &QMediaPlayer::setVolume);

    // Initialize volume
    ui->volumeSlider->setValue(50);
    mediaPlayer->setVolume(50);

    // Update position and duration
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &MainWindow::updatePosition);
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &MainWindow::updateDuration);

    // Initialize button states
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
        toggleFullScreen(); // Exit full-screen mode when Esc is pressed
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
        controlHideTimer->start(); // Restart the timer
    }
}

void MainWindow::toggleFullScreen()
{
    if (isFullScreen) {
        showNormal(); // Exit full-screen mode for the main window
        videoWidget->setFullScreen(false); // Exit full-screen mode for the video widget
        showControls(); // Show controls when exiting full-screen
        controlHideTimer->stop(); // Stop the timer
    } else {
        showFullScreen(); // Enter full-screen mode for the main window
        videoWidget->setFullScreen(true); // Enter full-screen mode for the video widget
        showControls(); // Show controls initially
        controlHideTimer->start(); // Start the timer
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
        showControls(); // Show controls when the mouse moves
    }
    return QMainWindow::eventFilter(obj, event);
}