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
    ui->controlsOverlay->setAttribute(Qt::WA_TranslucentBackground); // Optional: Make controls semi-transparent
    ui->controlsOverlay->raise(); // Bring controls to the front
}

MainWindow::~MainWindow()
{
    delete mediaPlayer;
    delete videoWidget;
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F11 || event->key() == Qt::Key_F) {
        toggleFullScreen(); // Toggle full-screen on F11 or F key
    } else if (event->key() == Qt::Key_Escape && isFullScreen) {
        toggleFullScreen(); // Exit full-screen on Escape key
    } else {
        QMainWindow::keyPressEvent(event); // Pass the event to the base class
    }
}

void MainWindow::hideControls()
{
    if (isFullScreen) {
        ui->controlsOverlay->setVisible(false); // Hide entire overlay
    }
}

void MainWindow::showControls()
{
    if (isFullScreen) {
        ui->controlsOverlay->setVisible(true); // Show entire overlay
        controlHideTimer->start(); 
    }
}

void MainWindow::toggleFullScreen()
{
    if (isFullScreen) {
        showNormal(); 
        videoWidget->setGeometry(0, 0, width(), height()); 
        showControls(); 
        controlHideTimer->stop();
    } else {
        showFullScreen(); 
        videoWidget->setGeometry(0, 0, width(), height());
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
    if (obj == videoWidget) {
        if (event->type() == QEvent::MouseMove) {
            showControls(); // Show controls on mouse move
        }
        // Forward key events to MainWindow
        if (event->type() == QEvent::KeyPress) {
            keyPressEvent(static_cast<QKeyEvent*>(event));
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}