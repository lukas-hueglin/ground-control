#include "aheditor.h"
#include "artificialhorizon.h"


AHEditor::AHEditor(DataFrame *p_dataFrame, QWidget *parent)
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

    // create QLabel as failLabel
    failLabel = new QLabel(QString("Load a log file first!"), m_viewport);
    failLabel->setAlignment(Qt::AlignCenter);


    // check if log file is already loaded
    if (m_dataFrame->isAlreadySetup()){
        setupHorizon();

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
        setupHorizon();
        properlySetup = true;
    });
}

void AHEditor::setupHorizon(){
    // delete old QGridlayout and QLabel
    delete m_viewport->layout();
    delete failLabel;

    // create QGridLayout
    QGridLayout *layout = new QGridLayout(m_viewport);
    m_viewport->setLayout(layout);

    layout->setContentsMargins(0, 0, 0, 0);

    ArtificialHorizon *ah = new ArtificialHorizon(m_dataFrame, m_viewport);
    layout->addWidget(ah);

    connect(m_dataFrame, &DataFrame::onTimeChanged, ah, &ArtificialHorizon::update);
}
