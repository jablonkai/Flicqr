#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

#define FLICQR_VERSION "0.1"

class Photoset;

namespace Ui {
class MainWindow;
}

class FlickrAPI;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_sendPushButton_clicked();
    void on_action_Authenticate_triggered();
    void on_action_About_triggered();
    void on_actionPhotosets_triggered();
    void on_actionDownload_photoset_triggered();

    void photoSetActivated(Photoset *photoset);

private:
    Ui::MainWindow *ui;
    FlickrAPI *flickrAPI;

    void readSettings();
    void writeSettings();
};

#endif // MAINWINDOW_H
