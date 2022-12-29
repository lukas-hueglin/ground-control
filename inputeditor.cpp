#include "inputeditor.h"

#include <QGridLayout>
#include <QFileDialog>
#include <QElapsedTimer>

#include <QDebug>

InputEditor::InputEditor(DataFrame *p_dataFrame, QWidget *parent)
    : Editor{p_dataFrame, parent}
{
    // create QWidget as viewport
    m_viewport = new QWidget(parent);

    // add viewport to container
    m_container->addWidget(m_viewport);

    // add QGridLayout to viewport
    QGridLayout *layout = new QGridLayout(m_viewport);
    m_viewport->setLayout(layout);


    fileSelection = new QLineEdit(QString(""), m_viewport);
    fileSelection->setReadOnly(true);
    layout->addWidget(fileSelection, 0, 0, 1, 1);

    browseButton = new QPushButton(QString("Browse"), m_viewport);
    layout->addWidget(browseButton, 0, 1, 1, 2);

    connect(browseButton, &QPushButton::pressed, this, &InputEditor::browse);

}

void InputEditor::browse() {
    QString filePath = QFileDialog::getOpenFileName(m_viewport, QString("Select Log File"), QString("*.csv"));

    //module->setStatusWorking(QString("Loading: "+filePath));

    // create timer
    QElapsedTimer timer;

    if (!filePath.isEmpty()) {
        m_dataFrame->setFile(new QFile(filePath));

        // start timer and loading
        timer.start();
    }
    else{

    }

    //module->setStatusSuccess(QString("Elapsed Time: "+QString::number(timer.elapsed()))+QString(" ms")+ QString(" | ") + QString::number(mainWindow->getDataLen())+" Datapoints");
    fileSelection->setText(filePath.split("/").last());
}
