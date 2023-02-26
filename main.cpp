#include "mainwindow.h"

#include "ArcGISRuntimeEnvironment.h"

#include <QApplication>
#include <QDir>
#include <QFontDatabase>
#include <QStyleFactory>

using namespace Esri::ArcGISRuntime;


int main(int argc, char *argv[])
{
    // activate ArcGis
    QFile keyFile(":/apiKey/key.txt");

    if (keyFile.open(QFile::ReadOnly | QIODevice::Text)) {
        const QString apiKey = keyFile.readLine().chopped(1);
        ArcGISRuntimeEnvironment::setApiKey(apiKey);
    }

    QApplication a(argc, argv);
    MainWindow w;

    // create dark palette for dark title bar
    a.setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, Qt::black);
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    a.setPalette(darkPalette);

    // create window
    w.setWindowState(Qt::WindowMaximized);
    w.show();

    return a.exec();
}
