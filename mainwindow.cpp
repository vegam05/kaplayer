#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mediaPlayer(new QMediaPlayer(this))
    , videoWidget(new QVideoWidget(this))
    , isFullScreen(false) {
    ui->setupUi(this);

    // Set up video output
    mediaPlayer->setVideoOutput(videoWidget);
    ui->videoLayout->addWidget(videoWidget);

    // Connect all signals and slots
    connect(ui->openButton, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(ui->playButton, &QPushButton::clicked, this, &MainWindow::play);
    connect(ui->pauseButton, &QPushButton::clicked, this, &MainWindow::pause);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stop);
    connect(ui->fullscreenButton, &QPushButton::clicked, this, &MainWindow::toggleFullScreen);
    
    // Volume connections
    connect(ui->volumeSlider, &QSlider::valueChanged, mediaPlayer, &QMediaPlayer::setVolume);
    connect(mediaPlayer, &QMediaPlayer::volumeChanged, ui->volumeSlider, &QSlider::setValue);

    // Media position connections
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &MainWindow::updatePosition);
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &MainWindow::updateDuration);
    connect(ui->positionSlider, &QSlider::sliderMoved, this, &MainWindow::setPosition);

    // Set video widget's size policy
    videoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void MainWindow::toggleFullScreen() {
    if (!isFullScreen) {
        // Save normal window geometry
        normalGeometry = this->geometry();
        
        // Hide all controls
        ui->controlLayout->parentWidget()->hide();
        ui->volumeLayout->parentWidget()->hide();
        ui->progressLayout->parentWidget()->hide();
        
        // Make window fullscreen
        showFullScreen();
        videoWidget->setFullScreen(true);
    } else {
        // Restore all controls
        ui->controlLayout->parentWidget()->show();
        ui->volumeLayout->parentWidget()->show();
        ui->progressLayout->parentWidget()->show();
        
        // Restore normal window
        showNormal();
        setGeometry(normalGeometry);
        videoWidget->setFullScreen(false);
    }
    isFullScreen = !isFullScreen;
}


void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape && isFullScreen) {
        toggleFullScreen();
    }
    QMainWindow::keyPressEvent(event);
}


MainWindow::~MainWindow() {
    delete ui;
}

QString MainWindow::formatTime(qint64 milliseconds) {
    qint64 seconds = milliseconds / 1000;
    qint64 minutes = seconds / 60;
    seconds %= 60;
    return QString("%1:%2")
        .arg(minutes)
        .arg(seconds, 2, 10, QChar('0'));
}

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Media File");
    if (!fileName.isEmpty()) {
        mediaPlayer->setMedia(QUrl::fromLocalFile(fileName));
        ui->positionSlider->setValue(0);
    }
}

void MainWindow::play() {
    mediaPlayer->play();
}

void MainWindow::pause() {
    mediaPlayer->pause();
}

void MainWindow::stop() {
    mediaPlayer->stop();
    mediaPlayer->setPosition(0);
    ui->positionSlider->setValue(0);
}

void MainWindow::updateDuration(qint64 duration) {
    ui->positionSlider->setRange(0, duration);
    ui->totalTimeLabel->setText(formatTime(duration));
}

void MainWindow::updatePosition(qint64 position) {
    if (!ui->positionSlider->isSliderDown())
        ui->positionSlider->setValue(position);
    ui->currentTimeLabel->setText(formatTime(position));
}

void MainWindow::setPosition(int position) {
    mediaPlayer->setPosition(position);
}

