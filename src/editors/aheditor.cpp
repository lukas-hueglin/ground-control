#include "editors/aheditor.h"
#include "helpers/artificialhorizon.h"


AHEditor::AHEditor(DataFrame *p_dataFrame, QWidget *parent)
    : Editor{p_dataFrame, parent}
{
    // connect setupViewport method
    connect(m_dataFrame, &DataFrame::onFileChange, this, &AHEditor::setupViewport);

    // check if the dataframe is already ready
    if (m_dataFrame->isAlreadySetup()) {
        // call setupViewport
        setupViewport();
    }
}

void AHEditor::setupViewport(){
    // call parent function to delete the old viewport
    Editor::setupViewport();

    // create artificial horizon
    ArtificialHorizon *ah = new ArtificialHorizon(m_dataFrame, this);

    // set viewport to artificial horizon
    m_viewport = ah;
    m_layout->addWidget(m_viewport, 0, 1, 1, 1);

    // connect update
    connect(m_dataFrame, &DataFrame::onTimeChanged, ah, &ArtificialHorizon::update);
}
