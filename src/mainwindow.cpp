#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QSettings>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#include "flickrapi.h"
#include "photosetsmodel.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    readSettings();
    show();

    flickrAPI = new FlickrAPI(this, ui);
    ui->photosetsView->setModel(flickrAPI->photosetsModel());

    connect(ui->actionE_xit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(flickrAPI, SIGNAL(sendResponse(QString)), ui->responseTextEdit, SLOT(setText(QString)));
    connect(flickrAPI, SIGNAL(photoSetActivated(Photoset*)), this, SLOT(photoSetActivated(Photoset*)));
    connect(ui->photosetsView, SIGNAL(activated(QModelIndex)), flickrAPI, SLOT(activated(QModelIndex)));
    connect(ui->photosView, SIGNAL(activated(QModelIndex)), flickrAPI, SLOT(photoActivated(QModelIndex)));
}

MainWindow::~MainWindow()
{
    writeSettings();

    delete flickrAPI;
    delete ui;
}

void MainWindow::on_action_Authenticate_triggered()
{
    flickrAPI->authenticate();
}

void MainWindow::on_actionPhotosets_triggered()
{
    flickrAPI->getPhotosetsList();
}

void MainWindow::on_actionDownload_photoset_triggered()
{
    QString directory = QFileDialog::getExistingDirectory(this, "Directory to save the album");
//    QModelIndex currentIndex = ui->photosetsView->selectionModel()->currentIndex();
    if (!directory.isEmpty())
        flickrAPI->downloadPhotoset(directory);
}

void MainWindow::on_action_About_triggered()
{
    QMessageBox::about(this, tr("About Flicqr"), QString::fromUtf8("<b>Flicqr</b><br>Version " FLICQR_VERSION "<br>Copyright (C) 2015 by Tamás Jablonkai<br><a href=\"mailto:tamas.jablonkai@gmail.com\">tamas.jablonkai@gmail.com</a>"));
}

void MainWindow::photoSetActivated(Photoset *photoset)
{
    if (!photoset)
        return;
    ui->photosView->setModel(photoset);
}

void MainWindow::readSettings()
{
    QSettings conf;

    conf.beginGroup("MainWindow");
    restoreState(conf.value("state", saveState()).toByteArray());
    restoreGeometry(conf.value("geometry", saveGeometry()).toByteArray());
    ui->verticalSplitter->restoreState(conf.value("vSplitterState").toByteArray());
    ui->horizontalSplitter->restoreState(conf.value("hSplitterState").toByteArray());
    ui->tabWidget->setCurrentIndex(conf.value("activeTab").toInt());
    conf.endGroup();

    conf.beginGroup("Account");
    Settings::instance()->setUserName(conf.value("userName", Settings::instance()->userName()).toString());
    Settings::instance()->setUserID(conf.value("userID", Settings::instance()->userID()).toString());
    Settings::instance()->setOauthTokenKey(conf.value("oauthTokenKey", Settings::instance()->oauthTokenKey()).toString());
    Settings::instance()->setOauthTokenSecret(conf.value("oauthTokenSecret", Settings::instance()->oauthTokenSecret()).toString());
    conf.endGroup();
}

void MainWindow::writeSettings()
{
    QSettings conf;

    conf.beginGroup("MainWindow");
    conf.setValue("state", saveState());
    conf.setValue("geometry", saveGeometry());
    conf.setValue("vSplitterState", ui->verticalSplitter->saveState());
    conf.setValue("hSplitterState", ui->horizontalSplitter->saveState());
    conf.setValue("activeTab", ui->tabWidget->currentIndex());
    conf.endGroup();

    conf.beginGroup("Account");
    conf.setValue("userName", Settings::instance()->userName());
    conf.setValue("userID", Settings::instance()->userID());
    conf.setValue("oauthTokenKey", Settings::instance()->oauthTokenKey());
    conf.setValue("oauthTokenSecret", Settings::instance()->oauthTokenSecret());
    conf.endGroup();
}
