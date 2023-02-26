#include "dashboardeditor.h"

#include <QCheckBox>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QGridLayout>
#include <QPushButton>
#include <QColorDialog>
#include <QDebug>

#include <QMainWindow>

DashboardLabel::DashboardLabel(DataFrame *p_dataFrame, QString strLabel, QString strValue, QWidget *parent)
    : QDockWidget(parent)
{
    // set dataFrame
    m_dataFrame = p_dataFrame;

    // set features of QDockWidget
    setFeatures(QDockWidget::NoDockWidgetFeatures);
    setFeatures(QDockWidget::DockWidgetMovable);

    // create new QLabels
    label = new QLabel(strLabel);
    value = new QLabel(strValue);

    // add properties to QLabels
    label->setProperty("cssClass", "dashlabel");
    value->setProperty("cssClass", "dashvalue");

    // set Alignment of QLabels
    label->setAlignment(Qt::AlignCenter);
    value->setAlignment(Qt::AlignCenter);

    // connect Label feedback
    connect(m_dataFrame, &DataFrame::onTimeChanged, [this](){
        value->setText(QString::number(m_dataFrame->getValue(label->text())));
    });

    // set titlebar
    setTitleBarWidget(label);

    // set widget
    setWidget(value);
}


DashboardEditor::DashboardEditor(DataFrame *p_dataFrame, QWidget *parent)
    : Editor{p_dataFrame, parent}
{
    // set QMaps
    checkBoxes = new QMap<QString, QCheckBox*>;
    colorButtons = new QMap<QString, QPushButton*>;
    labels = new QMap<QString, DashboardLabel*>;

    // create QMainWindow as viewport
    m_viewport = new QMainWindow(parent);

    // add viewport to container
    m_container->addWidget(m_viewport);

    // check if log file is already loaded
    if (m_dataFrame->isAlreadySetup()){
        setupDrawer();
        setupDashboard();

        // set properlySetup
        properlySetup = true;
    }
    else {
        // create QLabel as viewport
        failLabel = new QLabel(QString("Load a log file first!"), m_viewport);
        failLabel->setAlignment(Qt::AlignCenter);

        // set central widget
        qobject_cast<QMainWindow*>(m_viewport)->setCentralWidget(failLabel);

        // set properlySetup
        properlySetup = false;
    }

    // connect reload of log file
    connect(m_dataFrame, &DataFrame::onFileChanged, [this](){
        delete failLabel;
        setupDrawer();
        setupDashboard();
        properlySetup = true;
    });

    // add property to dashboard
    setProperty("cssClass", "dashboard");
}

void DashboardEditor::setupDrawer() {
    // call parent function
    Editor::setupDrawer();

    // create QVBoxLayout
    QGridLayout *drawerLayout = new QGridLayout(m_drawer);
    m_drawer->setLayout(drawerLayout);

    // counter variable for row
    int row = 0;

    for(QString key : *m_dataFrame->getKeys()){
        if (key != QString("time")) {
            QCheckBox *checkBox = new QCheckBox(key, m_drawer);
            checkBox->setCheckState(Qt::Checked);

            QPushButton *colorButton = new QPushButton;
            colorButton->setFixedSize(15, 15);
            colorButton->setStyleSheet("background-color: black;");

            connect(checkBox, &QCheckBox::clicked, [this, key](bool checked){toggleLabel(key, checked);});
            drawerLayout->addWidget(checkBox, row, 0);

            connect(colorButton, &QPushButton::pressed, [this, key](){
                QColorDialog *colorDialog = new QColorDialog(this);

                connect(colorDialog, &QColorDialog::colorSelected, [this, key](QColor color){
                    labels->find(key).value()->value->setStyleSheet("color: " + color.name() + ";");
                });
                connect(colorDialog, &QColorDialog::colorSelected, [this, key](QColor color){
                    colorButtons->find(key).value()->setStyleSheet("background-color: " + color.name() + ";");
                });

                colorDialog->show();
            });

            drawerLayout->addWidget(colorButton, row, 1);

            checkBoxes->insert(key, checkBox);
            colorButtons->insert(key, colorButton);
            ++row;
        }
    }
}

void DashboardEditor::setupDashboard() {
    // get count of checkboxes
    int n = checkBoxes->size();
    int rows = ceil(n/3)+1;

    // counting variable
    int i = 0;

    // create new DashboardLabels for each checked QCheckbox
    for (QCheckBox *checkBox : *checkBoxes) {
        if (checkBox->isChecked()) {
            DashboardLabel *l = new DashboardLabel(m_dataFrame, checkBox->text(), QString::number(m_dataFrame->getValue(checkBox->text())), m_viewport);

            if (i < rows) {
                qobject_cast<QMainWindow*>(m_viewport)->addDockWidget(Qt::RightDockWidgetArea, l);
            }
            else {
                qobject_cast<QMainWindow*>(m_viewport)->splitDockWidget(labels->find(labels->keys().at(i%rows)).value(), l, Qt::Horizontal);
            }

            labels->insert(checkBox->text(), l);
            ++i;
        }
    }

    // raise drawer to top
    //drawer->raise();
}

void DashboardEditor::toggleLabel(QString key, bool checked){
    if (checked){
        DashboardLabel *l = new DashboardLabel(m_dataFrame, key, QString::number(m_dataFrame->getValue(key)), m_viewport);
        qobject_cast<QMainWindow*>(m_viewport)->addDockWidget(Qt::BottomDockWidgetArea, l);

        labels->insert(key, l);
    }
    else {
        DashboardLabel *l = labels->find(key).value();

        qobject_cast<QMainWindow*>(m_viewport)->removeDockWidget(l);
        delete l;
    }
}
