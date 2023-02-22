#include "mainwindow.h"

#include "ArcGISRuntimeEnvironment.h"

#include <QApplication>
#include <QDir>

using namespace Esri::ArcGISRuntime;



int main(int argc, char *argv[])
{
    QFile keyFile(":/apiKey/key.txt");

    if (keyFile.open(QFile::ReadOnly | QIODevice::Text)) {
        const QString apiKey = keyFile.readLine().chopped(1);
        ArcGISRuntimeEnvironment::setApiKey(apiKey);
    }

    QApplication a(argc, argv);
    MainWindow w;

    w.setWindowState(Qt::WindowMaximized);
    w.show();

    return a.exec();
}
