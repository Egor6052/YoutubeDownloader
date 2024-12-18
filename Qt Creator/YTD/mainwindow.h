// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addButton_clicked();   // Слот для кнопки Add Video
    void on_downloadButton_clicked();  // Слот для кнопки Download All Videos

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
