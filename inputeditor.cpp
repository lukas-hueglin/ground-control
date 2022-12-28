#include "inputeditor.h"

#include <QGridLayout>
#include <QFileDialog>
#include <QElapsedTimer>

#include <QDebug>

#include "module.h"

InputEditor::InputEditor(Module *parent, MainWindow *mainWindow)
    : Editor{parent}
{
    // set parent
    module = parent;

    // set mainWindow
    this->mainWindow = mainWindow;

    // create QWidget as viewport
    viewport = new QWidget(module);

    // add viewport to container
    container->addWidget(viewport);

    // add QGridLayout to viewport
    QGridLayout *layout = new QGridLayout(viewport);
    viewport->setLayout(layout);


    fileSelection = new QLineEdit(QString(""), viewport);
    fileSelection->setReadOnly(true);
    layout->addWidget(fileSelection, 0, 0, 1, 1);

    browseButton = new QPushButton(QString("Browse"), viewport);
    layout->addWidget(browseButton, 0, 1, 1, 2);

    connect(browseButton, &QPushButton::pressed, this, &InputEditor::browse);

}

void InputEditor::browse() {
    QString filePath = QFileDialog::getOpenFileName(viewport, QString("Select Log File"), QString("*.csv"));

    module->setStatusWorking(QString("Loading: "+filePath));

    // create timer
    QElapsedTimer timer;

    if (!filePath.isEmpty()) {
        mainWindow->setFile(new QFile(filePath));

        // start timer and loading
        timer.start();
        mainWindow->loadDataFromFile();
    }

    // Force time update to fully initialize timelines etc.
    mainWindow->updateTime(0);

    module->setStatusSuccess(QString("Elapsed Time: "+QString::number(timer.elapsed()))+QString(" ms")+ QString(" | ") + QString::number(mainWindow->getDataLen())+" Datapoints");
    fileSelection->setText(filePath.split("/").last());
}
