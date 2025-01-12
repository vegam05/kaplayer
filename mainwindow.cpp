#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mediaPlayer(new QMediaPlayer(this))
    , videoWidget(new QVideoWidget(this)) {
    ui->setupUi(this);

    // Set up video output
    mediaPlayer->setVideoOutput(videoWidget);
    ui->videoLayout->addWidget(videoWidget);

    // Connect signals and slots
    connect(ui->openButton, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(ui->playButton, &QPushButton::clicked, this, &MainWindow::play);
    connect(ui->pauseButton, &QPushButton::clicked, this, &MainWindow::pause);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stop);
    
    // Use the volume slider from the UI
    connect(ui->volumeSlider, &QSlider::valueChanged, mediaPlayer, &QMediaPlayer::setVolume);
    connect(mediaPlayer, &QMediaPlayer::volumeChanged, ui->volumeSlider, &QSlider::setValue);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Media File");
    if (!fileName.isEmpty()) {
        mediaPlayer->setMedia(QUrl::fromLocalFile(fileName));
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
    // Reset the media position to the beginning
    mediaPlayer->setPosition(0);
}