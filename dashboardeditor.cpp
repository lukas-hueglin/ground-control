#include "dashboardeditor.h"
#include "qdatetime.h"

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
    label = new QLabel(strLabel, this);
    value = new QLabel(strValue, this);

    // add properties to QLabels
    label->setProperty("cssClass", "dashlabel");
    value->setProperty("cssClass", "dashvalue");

    // set Alignment of QLabels
    label->setAlignment(Qt::AlignCenter);
    value->setAlignment(Qt::AlignCenter);

    // set color
    label->setStyleSheet("color: "+m_dataFrame->getColor(strLabel)->name(QColor::HexRgb));

    // connect Label feedback
    connect(m_dataFrame, &DataFrame::onTimeChanged, [this, strLabel](){
        if (strLabel == QString("time")) {
            value->setText(m_dataFrame->getDateTime()->toString(QString("hh:mm:ss.z")));
        } else {
            value->setText(QString::number(m_dataFrame->getValue(label->text())));
        }
    });

    // set titlebar
    setTitleBarWidget(label);

    // set widget
    setWidget(value);
}

QSize DashboardLabel::sizeHint() const {
    return QSize(20, 20);
}

QSize DashboardLabel::minimumSizeHint() const {
    return QSize(0, 0);
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
                    //labels->find(key).value()->value->setStyleSheet("color: " + color.name() + ";");
                });
                connect(colorDialog, &QColorDialog::colorSelected, [this, key](QColor color){
                    //colorButtons->find(key).value()->setStyleSheet("background-color: " + color.name() + ";");
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
    // hide m_viewport
    m_viewport->setVisible(false);

    // cast m_viewport to QMainWindow
    QMainWindow *mainWindow =  qobject_cast<QMainWindow*>(m_viewport);

    // get count of checkboxes and calculate rows
    int n = checkBoxes->size();
    int rows = ceil(n/3)+1;

    // current row and column
    int c_row = 0;
    int c_col = 0;

    // current index
    int index = 0;

    while (index < n) {
        // get label and value
        QString key = m_dataFrame->getKeys()->at(index);
        QString value = key == QString("time") ? m_dataFrame->getDateTime()->toString(QString("hh:mm:ss.z")) : QString::number(m_dataFrame->getValue(key));

        // create DashboardLabel
        DashboardLabel *label = new DashboardLabel(m_dataFrame, key, value, m_viewport);

        // declare last key
        QString l_key;

        // if first column
        if (c_col == 0) {
            // if first row
            if (c_row == 0) {
                mainWindow->addDockWidget(Qt::RightDockWidgetArea, label);
            } else {
                // get last DashboardLabel
                l_key = m_dataFrame->getKeys()->at(index-3);
                DashboardLabel *l_label = labels->find(l_key).value();

                // split and resize the DashboardLabels
                mainWindow->splitDockWidget(l_label, label, Qt::Vertical);

                // resize DashboardLabels if necessairy
                if (c_row + 1 == rows) {
                    QList<QDockWidget*> rowLabels;
                    QList<int> sizes;

                    // get all lastest DashboardLabels
                    for (int i = 0; i < rows-1; ++i) {
                        rowLabels.append(labels->find(m_dataFrame->getKeys()->at(3*i)).value());
                        sizes.append(100);
                    }

                    // append current DashboardLabel
                    rowLabels.append(label);
                    sizes.append(100);

                    // resize DashboardLabels
                    mainWindow->resizeDocks(rowLabels, sizes, Qt::Vertical);
                }
            }
        } else {
            // get last DashboardLabel
            l_key = m_dataFrame->getKeys()->at(index-1);
            DashboardLabel *l_label = labels->find(l_key).value();

            // split and resize the DashboardLabels
            mainWindow->splitDockWidget(l_label, label, Qt::Horizontal);

            // resize DashboardLabels if necessairy
            if(c_col == 2) {
                // get pre last DashboardLabel
                QString ll_key = m_dataFrame->getKeys()->at(index-2);
                DashboardLabel *ll_label = labels->find(ll_key).value();

                // resize DashboardLabels
                mainWindow->resizeDocks({ll_label, l_label, label}, {100, 100, 100}, Qt::Horizontal);
            } else if (c_col+1 == n) {
                // resize DashboardLabels
                mainWindow->resizeDocks({l_label, label}, {100, 100}, Qt::Horizontal);
            }

        }

        // increase c_rw and c_col
        if (c_row + 1 < rows) {
            ++c_row;
        } else {
            c_row = 0;
            ++c_col;
        }

        // recalculate index
        index = 3*c_row + c_col;

        // add DashboardLabel to labels
        labels->insert(key, label);
    }

    // show m_viewport
    m_viewport->setVisible(true);
}

void DashboardEditor::toggleLabel(QString key, bool checked){
    if (checked){
        DashboardLabel *l = new DashboardLabel(m_dataFrame, key, QString::number(m_dataFrame->getValue(key)), m_viewport);
        qobject_cast<QMainWindow*>(m_viewport)->addDockWidget(Qt::BottomDockWidgetArea, l);

        labels->insert(key, l);
    }
    else {
        QDockWidget *l = labels->find(key).value();

        qobject_cast<QMainWindow*>(m_viewport)->removeDockWidget(l);
        delete l;
    }
}
