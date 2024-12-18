#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QListWidgetItem>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

class YoutubeDownloader {
public:
    int numberVideo;
    string link;
    string format;

    void downloadVideo() const {
        string cleanedLink = link.substr(0, link.find('?'));
        string command;
        if (format == ".mp3" || format == ".aac" || format == ".m4a") {
            command = "yt-dlp -x --audio-format " + format.substr(1) + " " + cleanedLink + " -o '%(title)s_" + to_string(numberVideo) + format + "'";
        } else {
            command = "yt-dlp " + cleanedLink + " -o '%(title)s_" + to_string(numberVideo) + format + "'";
        }

        cout << "Executing command: " << command << endl;
        system(command.c_str());
    }
};

vector<YoutubeDownloader> videos;
int numberOfObjectsVideo = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::on_addButton_clicked);
    connect(ui->downloadButton, &QPushButton::clicked, this, &MainWindow::on_downloadButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addButton_clicked()
{
    QString link = ui->linkInput->text();
    QString format = ui->formatComboBox->currentText();

    // Перевірка на порожнє посилання
    if (link.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please enter a video link.");
        return;
    }

    YoutubeDownloader video;
    video.numberVideo = ++numberOfObjectsVideo;
    video.link = link.toStdString();
    video.format = format.toStdString();

    videos.push_back(video);

    ui->videoList->addItem("Video " + QString::number(video.numberVideo) + ": " + link);
}

void MainWindow::on_downloadButton_clicked()
{
    for (const auto& video : videos) {
        video.downloadVideo();
    }

    QMessageBox::information(this, "Download Complete", "All videos have been downloaded.");
}
