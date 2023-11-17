#include "editors/timelineeditor.h"

#include <QGridLayout>
#include <QDateTime>

#include "helpers/statusbar.h"

TimelineEditor::TimelineEditor(DataFrame *p_dataFrame, QWidget *parent)
    : Editor{p_dataFrame, parent}
{
    // connect setupViewport method
    connect(m_dataFrame, &DataFrame::onFileChange, this, &TimelineEditor::setupViewport);

    // check if the dataframe is already ready
    if (m_dataFrame->isAlreadySetup()) {
        // call setupViewport
        setupViewport();
    }
}

void TimelineEditor::setupViewport(){
    // call parent function to delete the old viewport
    Editor::setupViewport();

    // create QWidget as viewport
    m_viewport = new QWidget(this);
    m_layout->addWidget(m_viewport, 0, 1, 1, 1);

    // create QGridLayout
    QGridLayout *layout = new QGridLayout(m_viewport);
    layout->setContentsMargins(0, 0, 0, 0);
    m_viewport->setLayout(layout);

    // create QPushButtons
    m_toBeginButton = new QPushButton(QString(QChar(0x2B25))+QString(QChar(0x276E)), m_viewport);
    layout->addWidget(m_toBeginButton, 0, 0, 1, 1);

    m_backwardButton = new QPushButton(QString(QChar(0x276E))+QString(QChar(0x276E)), m_viewport);
    layout->addWidget(m_backwardButton, 0, 1, 1, 1);

    m_playButton = new QPushButton(QString(QChar(0x2BC8)), m_viewport);
    m_playButton->setCheckable(true);
    m_playButton->setChecked(false);
    layout->addWidget(m_playButton, 0, 2, 1, 2);

    m_forwardButton = new QPushButton(QString(QChar(0x276F))+QString(QChar(0x276F)), m_viewport);
    layout->addWidget(m_forwardButton, 0, 4, 1, 1);

    m_toEndButton = new QPushButton(QString(QChar(0x276F))+QString(QChar(0x2B25)), m_viewport);
    layout->addWidget(m_toEndButton, 0, 5, 1, 1);

    m_speedButton = new QPushButton(QString("1x"), m_viewport);
    layout->addWidget(m_speedButton, 0, 14, 1, 1);

    // connect QPushButtons
    connect(m_playButton, &QPushButton::clicked, m_dataFrame, &DataFrame::setPlayState);

    connect(m_toBeginButton, &QPushButton::pressed, [this](){m_dataFrame->setTime(0);});
    connect(m_backwardButton, &QPushButton::pressed, [this](){m_dataFrame->setTime(fmax(m_dataFrame->getTime()-1, 0));});
    connect(m_forwardButton, &QPushButton::pressed, [this](){m_dataFrame->setTime(fmin(m_dataFrame->getTime()+1, m_dataFrame->getSize()-1));});
    connect(m_toEndButton, &QPushButton::pressed, [this](){m_dataFrame->setTime(m_dataFrame->getSize()-1);});

    connect(m_speedButton, &QPushButton::pressed, m_dataFrame, &DataFrame::increaseSpeed);

    // connect QPushButton Feedback
    connect(m_dataFrame, &DataFrame::onPlayStateChanged, [this](bool p_play){
        if (p_play) {
            m_playButton->setText(QString(QChar(0x2016)));
            m_statusbar->setStatus(Status::WORKING, QString("Playing"));
        } else {
            m_playButton->setText(QString(QChar(0x2BC8)));
            emit m_statusbar->setStatus(Status::OK, QString(""));
        }

    });

    connect(m_dataFrame, &DataFrame::onSpeedIncreased, [this](int p_speed){
        m_speedButton->setText(QString::number(p_speed)+"x");
    });

    // create QSlider
    m_timeline = new QSlider(Qt::Orientation::Horizontal, m_viewport);
    m_timeline->setTickPosition(QSlider::NoTicks);
    m_timeline->setMaximum(m_dataFrame->getSize()-1);
    m_timeline->setSliderPosition(m_dataFrame->getTime());
    layout->addWidget(m_timeline, 1, 0, 1, 20);

    // connect QSlider
    connect(m_timeline, &QSlider::valueChanged, m_dataFrame, &DataFrame::setTime);

    // connect QSlider feedback
    connect(m_dataFrame, &DataFrame::onTimeChanged, m_timeline, &QSlider::setValue);

    // create QLabel
    m_timeIndicator = new QLabel(m_dataFrame->getDateTime()->toString(), m_viewport);
    layout->addWidget(m_timeIndicator, 0, 7, 1, 5);

    // connect QLabel Feedback
    connect(m_dataFrame, &DataFrame::onTimeChanged, [this](){
        m_timeIndicator->setText(m_dataFrame->getDateTime()->toString());
    });

}
