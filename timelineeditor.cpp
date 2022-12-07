#include "timelineeditor.h"

#include <QGridLayout>

#include "module.h"


TimelineEditor::TimelineEditor(Module *parent, MainWindow *mainWindow)
    : Editor{parent}
{
    // set parent
    module = parent;

    // set mainWindow
    this->mainWindow = mainWindow;

    // create QWidget as viewport
    viewport = new QWidget(module);

    // create QGridLayout
    QGridLayout *layout = new QGridLayout(viewport);
    viewport->setLayout(layout);


    // check if log file is already loaded
    if (mainWindow->getDateTime(0) != nullptr){
        setupTimeline();

        // set properlySetup
        properlySetup = true;
    }
    else {
        // create QLabel as viewport
        failLabel = new QLabel(QString("Load a log file first!"), viewport);
        failLabel->setAlignment(Qt::AlignCenter);

        // set viewport
        layout->addWidget(failLabel, 0, 0, 1, 1);

        // set properlySetup
        properlySetup = false;
    }

    //setupDrawer(); // Disable Drawer
}

void TimelineEditor::setupTimeline(){
    // delete old QGridlayout and QLabel
    delete viewport->layout();
    delete failLabel;

    // create QGridLayout
    QGridLayout *layout = new QGridLayout(viewport);
    viewport->setLayout(layout);

    // create QPushButtons
    toBeginButton = new QPushButton(QString(QChar(0x2B25))+QString(QChar(0x276E)), viewport);
    layout->addWidget(toBeginButton, 0, 0, 1, 1);

    backwardButton = new QPushButton(QString(QChar(0x276E))+QString(QChar(0x276E)), viewport);
    layout->addWidget(backwardButton, 0, 1, 1, 1);

    playButton = new QPushButton(QString(QChar(0x2BC8)), viewport);
    layout->addWidget(playButton, 0, 2, 1, 2);

    forwardButton = new QPushButton(QString(QChar(0x276F))+QString(QChar(0x276F)), viewport);
    layout->addWidget(forwardButton, 0, 4, 1, 1);

    toEndButton = new QPushButton(QString(QChar(0x276F))+QString(QChar(0x2B25)), viewport);
    layout->addWidget(toEndButton, 0, 5, 1, 1);

    // create QSlider
    timeline = new QSlider(Qt::Orientation::Horizontal, viewport);
    timeline->setTickPosition(QSlider::NoTicks);
    timeline->setMaximum(mainWindow->getDataLen()-1);
    timeline->setSliderPosition(0);
    layout->addWidget(timeline, 1, 0, 1, 20);

    // create QLabel
    timeIndicator = new QLabel(mainWindow->getDateTime(0)->toString(), viewport);
    layout->addWidget(timeIndicator, 0, 7, 1, 5);

    // connect QSlider
    connect(timeline, &QSlider::valueChanged, mainWindow, &MainWindow::updateTime);
}


void TimelineEditor::updateTime(int t){
    if (!properlySetup){
        setupTimeline();

        // set properlySetup
        properlySetup = true;
    }

    time = t;
    timeIndicator->setText(mainWindow->getDateTime(time)->toString());
}
