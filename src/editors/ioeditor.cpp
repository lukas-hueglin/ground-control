#include "editors/ioeditor.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QElapsedTimer>

#include "helpers/statusbar.h"

IOEditor::IOEditor(DataFrame *p_dataFrame, QWidget *parent)
    : Editor{p_dataFrame, parent}
{
    // call setupViewport
    setupViewport();
}

void IOEditor::browse() {
    // set statusbar to working
    m_statusbar->setStatus(Status::WORKING, QString("Choose log file"));

    QString filePath = QFileDialog::getOpenFileName(m_viewport, QString("Select Log File"), QString("*.csv"));

    // create timer
    QElapsedTimer timer;

    if (!filePath.isEmpty()) {
        // start timer and loading
        timer.start();
        m_dataFrame->setFile(new QFile(filePath));
        m_statusbar->setStatus(Status::SUCCESS, QString("Elapsed Time: "+QString::number(timer.elapsed()))+QString(" ms")+ QString(" | ") + QString::number(m_dataFrame->getSize())+" Datapoints");
    }
    else{
        m_statusbar->setStatus(Status::FAIL, QString("Couldn't load log file!"));
    }
}

void IOEditor::changeType(const int index) {
    bool grayout = ! index;

    // grayout not used labels
    m_telemetryLabel->setDisabled(grayout);
    m_telemetrySelection->setDisabled(grayout);
    m_videoLabel->setDisabled(grayout);
    m_videoSelection->setDisabled(grayout);

    // send update to dataframe
    m_dataFrame->setLiveStream(index);
}

void IOEditor::setupViewport() {
    // call parent function to delete the old viewport
    Editor::setupViewport();

    // create QWidget as viewport
    m_viewport = new QWidget(this);
    m_layout->addWidget(m_viewport, 0, 1, 1, 1);

    // add QGridLayout to viewport
    m_vpLayout = new QGridLayout(m_viewport);
    m_viewport->setLayout(m_vpLayout);

    // add QComboBox
    m_comboBox = new QComboBox(m_viewport);
    m_comboBox->addItem("Load Recording");
    m_comboBox->addItem("Live Stream");

    connect(m_comboBox, &QComboBox::currentIndexChanged, this, &IOEditor::changeType);
    m_vpLayout->addWidget(m_comboBox, 0, 0, 1, 2);

    // Log File
    m_logFileLabel = new QLabel(QString("Log File"), m_viewport);
    m_vpLayout->addWidget(m_logFileLabel, 1, 1, 1, 1);

    m_logFileSelection = new QLineEdit(QString(""), m_viewport);
    m_logFileSelection->setReadOnly(true);
    m_vpLayout->addWidget(m_logFileSelection, 1, 2, 1, 5);

    m_logFileBrowseButton = new QPushButton(QString("Browse"), m_viewport);
    m_vpLayout->addWidget(m_logFileBrowseButton, 1, 8, 1, 2);

    connect(m_logFileBrowseButton, &QPushButton::pressed, this, &IOEditor::browse);

    connect(m_dataFrame, &DataFrame::onFileChange, [this](QFile* p_file){
        QFileInfo info(*p_file);
        m_logFileSelection->setText(info.fileName());
    });

    // telemetry
    m_telemetryLabel = new QLabel(QString("Telemetry"), m_viewport);
    m_vpLayout->addWidget(m_telemetryLabel, 2, 1, 1, 1);

    m_telemetrySelection = new QLineEdit(QString(""), m_viewport);
    m_telemetrySelection->setReadOnly(true);
    m_vpLayout->addWidget(m_telemetrySelection, 2, 2, 1, 5);

    // video
    m_videoLabel = new QLabel(QString("Video"), m_viewport);
    m_vpLayout->addWidget(m_videoLabel, 3, 1, 1, 1);

    m_videoSelection = new QLineEdit(QString(""), m_viewport);
    m_videoSelection->setReadOnly(true);
    m_vpLayout->addWidget(m_videoSelection, 3, 2, 1, 5);

    // grayout
    changeType(0);
}
