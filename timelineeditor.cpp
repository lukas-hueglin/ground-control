#include "timelineeditor.h"

#include <QGridLayout>
#include <QDateTime>

TimelineEditor::TimelineEditor(DataFrame *p_dataFrame, QWidget *parent)
    : Editor{p_dataFrame, parent}
{
    // create QWidget as viewport
    m_viewport = new QWidget(parent);

    // add viewport to container
    m_container->addWidget(m_viewport);

    // create QGridLayout
    QGridLayout *layout = new QGridLayout(m_viewport);
    layout->setContentsMargins(0, 0, 0, 0);
    m_viewport->setLayout(layout);

    // create QLabel as viewport
    failLabel = new QLabel(QString("Load a log file first!"), m_viewport);
    failLabel->setAlignment(Qt::AlignCenter);

    // check if log file is already loaded
    if (m_dataFrame->isAlreadySetup()){
        setupTimeline();

        // set properlySetup
        properlySetup = true;
    }
    else {
        // set viewport
        layout->addWidget(failLabel, 0, 0, 1, 1);

        // set properlySetup
        properlySetup = false;
    }

    // connect reload of log file
    connect(m_dataFrame, &DataFrame::onFileChanged, [this](){
        setupTimeline();
        properlySetup = true;
    });

    //setupDrawer(); // Disable Drawer
}

void TimelineEditor::setupTimeline(){
    // delete old QGridlayout and QLabel
    delete m_viewport->layout();
    delete failLabel;

    // create QGridLayout
    QGridLayout *layout = new QGridLayout(m_viewport);
    m_viewport->setLayout(layout);

    // create QPushButtons
    toBeginButton = new QPushButton(QString(QChar(0x2B25))+QString(QChar(0x276E)), m_viewport);
    layout->addWidget(toBeginButton, 0, 0, 1, 1);

    backwardButton = new QPushButton(QString(QChar(0x276E))+QString(QChar(0x276E)), m_viewport);
    layout->addWidget(backwardButton, 0, 1, 1, 1);

    playButton = new QPushButton(QString(QChar(0x2BC8)), m_viewport);
    playButton->setCheckable(true);
    playButton->setChecked(false);
    layout->addWidget(playButton, 0, 2, 1, 2);

    forwardButton = new QPushButton(QString(QChar(0x276F))+QString(QChar(0x276F)), m_viewport);
    layout->addWidget(forwardButton, 0, 4, 1, 1);

    toEndButton = new QPushButton(QString(QChar(0x276F))+QString(QChar(0x2B25)), m_viewport);
    layout->addWidget(toEndButton, 0, 5, 1, 1);

    speedButton = new QPushButton(QString("1x"), m_viewport);
    layout->addWidget(speedButton, 0, 14, 1, 1);

    // connect QPushButtons
    connect(playButton, &QPushButton::clicked, m_dataFrame, &DataFrame::setPlayState);

    connect(toBeginButton, &QPushButton::pressed, [this](){m_dataFrame->setTime(0);});
    connect(backwardButton, &QPushButton::pressed, [this](){m_dataFrame->setTime(fmax(m_dataFrame->getTime()-1, 0));});
    connect(forwardButton, &QPushButton::pressed, [this](){m_dataFrame->setTime(fmin(m_dataFrame->getTime()+1, m_dataFrame->getSize()-1));});
    connect(toEndButton, &QPushButton::pressed, [this](){m_dataFrame->setTime(m_dataFrame->getSize()-1);});

    connect(speedButton, &QPushButton::pressed, m_dataFrame, &DataFrame::increaseSpeed);

    // connect QPushButton Feedback
    connect(m_dataFrame, &DataFrame::onPlayStateChanged, [this](bool p_play){
        if (p_play) {
            playButton->setText(QString(QChar(0x2016)));
            emit onStatusChangeWorking(QString("Playing"));
        } else {
            playButton->setText(QString(QChar(0x2BC8)));
            emit onStatusChangeOK(QString(""));
        }

    });

    connect(m_dataFrame, &DataFrame::onSpeedIncreased, [this](int p_speed){
        speedButton->setText(QString::number(p_speed)+"x");
    });

    // create QSlider
    timeline = new QSlider(Qt::Orientation::Horizontal, m_viewport);
    timeline->setTickPosition(QSlider::NoTicks);
    timeline->setMaximum(m_dataFrame->getSize()-1);
    timeline->setSliderPosition(m_dataFrame->getTime());
    layout->addWidget(timeline, 1, 0, 1, 20);

    // connect QSlider
    connect(timeline, &QSlider::valueChanged, m_dataFrame, &DataFrame::setTime);

    // connect QSlider feedback
    connect(m_dataFrame, &DataFrame::onTimeChanged, timeline, &QSlider::setValue);

    // create QLabel
    timeIndicator = new QLabel(m_dataFrame->getDateTime()->toString(), m_viewport);
    layout->addWidget(timeIndicator, 0, 7, 1, 5);

    // connect QLabel Feedback
    connect(m_dataFrame, &DataFrame::onTimeChanged, [this](){
        timeIndicator->setText(m_dataFrame->getDateTime()->toString());
    });

}
