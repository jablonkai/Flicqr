#include <QtCore/QTranslator>
#include <QtWidgets/QApplication>

#include "mainwindow.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setOrganizationName("Jablonkai");
    app.setApplicationName("Flicqr");

    QTranslator translator;
    translator.load("qflickr_" + QLocale::system().name(), app.applicationDirPath() + "/i18n");
    app.installTranslator(&translator);

    MainWindow mainWindow;

    return app.exec();
}
