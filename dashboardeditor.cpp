#include "dashboardeditor.h"

#include <QCheckBox>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QGridLayout>
#include <QPushButton>
#include <QColorDialog>
#include <QDebug>

#include "module.h"

DashboardLabel::DashboardLabel(QString strLabel, QString strValue, QWidget *parent)
    : QDockWidget(parent)
{
    // set features of QDockWidget
    setFeatures(QDockWidget::NoDockWidgetFeatures);
    setFeatures(QDockWidget::DockWidgetMovable);

    // create new QLabels
    label = new QLabel(strLabel);
    value = new QLabel(strValue);

    // set Alignment of QLabels
    label->setAlignment(Qt::AlignCenter);
    value->setAlignment(Qt::AlignCenter);

    // set titlebar
    setTitleBarWidget(label);

    // set widget
    setWidget(value);
}


DashboardEditor::DashboardEditor(Module *parent, MainWindow *mainWindow)
    : Editor{parent}
{
    // set parent
    module = parent;

    // set mainWindow
    this->mainWindow = mainWindow;

    // set QMaps
    checkBoxes = new QMap<QString, QCheckBox*>;
    colorButtons = new QMap<QString, QPushButton*>;
    labels = new QMap<QString, DashboardLabel*>;

    // create QMainWindow as viewport
    viewport = new QMainWindow(module);

    // add viewport to container
    container->addWidget(viewport);

    // check if log file is already loaded
    if (mainWindow->getDateTime(0) != nullptr){
        setupDrawer();
        setupDashboard();

        // set properlySetup
        properlySetup = true;
    }
    else {
        // create QLabel as viewport
        failLabel = new QLabel(QString("Load a log file first!"), viewport);
        failLabel->setAlignment(Qt::AlignCenter);

        // set central widget
        qobject_cast<QMainWindow*>(viewport)->setCentralWidget(failLabel);

        // set properlySetup
        properlySetup = false;
    }
}

void DashboardEditor::setupDrawer() {
    // call parent function
    Editor::setupDrawer();

    // create QVBoxLayout
    QGridLayout *drawerLayout = new QGridLayout(drawer);
    drawer->setLayout(drawerLayout);

    // counter variable for row
    int row = 0;

    for(QString key : *mainWindow->getKeys()){
        if (key != QString("time")) {
            QCheckBox *checkBox = new QCheckBox(key, drawer);
            checkBox->setCheckState(Qt::Checked);

            QPushButton *colorButton = new QPushButton;
            colorButton->setFixedSize(15, 15);
            colorButton->setStyleSheet("background-color: black;");

            connect(checkBox, &QCheckBox::clicked, [this, key](bool checked){toggleLabel(key, checked);});
            drawerLayout->addWidget(checkBox, row, 0);

            connect(colorButton, &QPushButton::pressed, [this, key](){
                QColorDialog *colorDialog = new QColorDialog;

                connect(colorDialog, &QColorDialog::colorSelected, [this, key](QColor color){
                    labels->find(key).value()->label->setStyleSheet("color: " + color.name() + ";");
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
    // counting variable
    int i = 0;

    // create new DashboardLabels for each checked QCheckbox
    for (QCheckBox *checkBox : *checkBoxes) {
        if (checkBox->isChecked()) {
            DashboardLabel *l = new DashboardLabel(checkBox->text(), QString::number(mainWindow->getValue(time, checkBox->text())), viewport);

            // set dock widget area
            Qt::DockWidgetArea area;
            if (i % 2 == 0) {
                area = Qt::LeftDockWidgetArea;
            }
            else {
                area = Qt::RightDockWidgetArea;
            }

            qobject_cast<QMainWindow*>(viewport)->addDockWidget(area, l);
            labels->insert(checkBox->text(), l);
            ++i;
        }
    }

    // raise drawer to top
    //drawer->raise();
}

void DashboardEditor::toggleLabel(QString key, bool checked){
    if (checked){
        DashboardLabel *l = new DashboardLabel(key, QString::number(mainWindow->getValue(time, key)), viewport);
        qobject_cast<QMainWindow*>(viewport)->addDockWidget(Qt::BottomDockWidgetArea, l);

        labels->insert(key, l);
    }
    else {
        DashboardLabel *l = labels->find(key).value();

        qobject_cast<QMainWindow*>(viewport)->removeDockWidget(l);
        delete l;
    }
}

void DashboardEditor::updateTime(int t) {
    if (!properlySetup){
        // delete QLayout and failLabel
        delete drawer->layout();
        delete failLabel;

        setupDrawer();
        setupDashboard();

        // set properlySetup
        properlySetup = true;
    }

    time = t;

    // set all labels of QCheckBoxes
    for (DashboardLabel *l : *labels) {
        l->value->setText(QString::number(mainWindow->getValue(time, l->label->text())));
    }
}
