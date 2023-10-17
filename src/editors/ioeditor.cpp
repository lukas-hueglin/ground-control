#include "editors/ioeditor.h"

#include <QGridLayout>
#include <QFileDialog>
#include <QElapsedTimer>

#include <QDebug>

IOEditor::IOEditor(DataFrame *p_dataFrame, QWidget *parent)
    : Editor{p_dataFrame, parent}
{
    // create QWidget as viewport
    m_viewport = new QWidget(parent);

    // add viewport to container
    m_container->addWidget(m_viewport);

    // add QGridLayout to viewport
    QGridLayout *layout = new QGridLayout(m_viewport);
    m_viewport->setLayout(layout);

    // add QComboBox
    m_comboBox = new QComboBox(m_viewport);
    m_comboBox->addItem("Load Recording");
    m_comboBox->addItem("Live Stream");

    connect(m_comboBox, &QComboBox::currentIndexChanged, this, &IOEditor::changeType);
    layout->addWidget(m_comboBox, 0, 0, 1, 2);

    // Log File
    m_logFileLabel = new QLabel(QString("Log File"), m_viewport);
    layout->addWidget(m_logFileLabel, 1, 1, 1, 1);

    m_logFileSelection = new QLineEdit(QString(""), m_viewport);
    m_logFileSelection->setReadOnly(true);
    layout->addWidget(m_logFileSelection, 1, 2, 1, 5);

    m_logFileBrowseButton = new QPushButton(QString("Browse"), m_viewport);
    layout->addWidget(m_logFileBrowseButton, 1, 8, 1, 2);

    connect(m_logFileBrowseButton, &QPushButton::pressed, [this](){
        this->browse(this->m_logFileSelection);
    });

    // telemetry
    m_telemetryLabel = new QLabel(QString("Telemetry"), m_viewport);
    layout->addWidget(m_telemetryLabel, 2, 1, 1, 1);

    m_telemetrySelection = new QLineEdit(QString(""), m_viewport);
    m_telemetrySelection->setReadOnly(true);
    layout->addWidget(m_telemetrySelection, 2, 2, 1, 5);

    // video
    m_videoLabel = new QLabel(QString("Video"), m_viewport);
    layout->addWidget(m_videoLabel, 3, 1, 1, 1);

    m_videoSelection = new QLineEdit(QString(""), m_viewport);
    m_videoSelection->setReadOnly(true);
    layout->addWidget(m_videoSelection, 3, 2, 1, 5);

    // grayout
    changeType(0);
}

void IOEditor::browse(QLineEdit *p_lineEdit) {
    // set statusbar to working
    onStatusChangeWorking(QString("Choose log file"));

    QString filePath = QFileDialog::getOpenFileName(m_viewport, QString("Select Log File"), QString("*.csv"));

    // create timer
    QElapsedTimer timer;

    if (!filePath.isEmpty()) {
        // start timer and loading
        timer.start();

        m_dataFrame->setFile(new QFile(filePath));

        onStatusChangeSuccess(QString("Elapsed Time: "+QString::number(timer.elapsed()))+QString(" ms")+ QString(" | ") + QString::number(m_dataFrame->getSize())+" Datapoints");
        p_lineEdit->setText(filePath.split("/").last());
    }
    else{
        onStatusChangeFail(QString("Couldn't load log file!"));
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
