#include "editors/dashboardeditor.h"

#include <QCheckBox>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QGridLayout>
#include <QPushButton>
#include <QColorDialog>
#include <QGroupBox>
#include <QDateTime>
#include <QMainWindow>

#include "helpers/drawer.h"


Card::Card(DataFrame *p_dataFrame, QString strLabel, QString strValue, QWidget *parent)
    : QDockWidget(parent)
{
    // set dataFrame
    m_dataFrame = p_dataFrame;

    // set features of QDockWidget
    setFeatures(QDockWidget::NoDockWidgetFeatures);
    setFeatures(QDockWidget::DockWidgetMovable);

    // create new QLabels
    m_label = new QLabel(strLabel, this);
    m_value = new QLabel(strValue, this);

    // add properties to QLabels
    m_label->setProperty("cssClass", "dashlabel");
    m_value->setProperty("cssClass", "dashvalue");

    // set Alignment of QLabels
    m_label->setAlignment(Qt::AlignCenter);
    m_value->setAlignment(Qt::AlignCenter);

    // set color
    m_label->setStyleSheet("color: "+m_dataFrame->getColor(strLabel)->name(QColor::HexRgb));

    // connect Label feedback
    connect(m_dataFrame, &DataFrame::onTimeChanged, [this, strLabel](){
        if (strLabel == QString("time")) {
            m_value->setText(m_dataFrame->getDateTime()->toString(QString("hh:mm:ss.z")));
        } else {
            m_value->setText(QString::number(m_dataFrame->getValue(m_label->text())));
        }
    });

    // set titlebar
    setTitleBarWidget(m_label);

    // set widget
    setWidget(m_value);
}

QSize Card::sizeHint() const {
    return QSize(20, 20);
}

QSize Card::minimumSizeHint() const {
    return QSize(0, 0);
}

DashboardEditor::DashboardEditor(DataFrame *p_dataFrame, QWidget *parent)
    : Editor{p_dataFrame, parent}
{
    // set QMaps
    m_checkBoxes = new QMap<QString, QCheckBox*>;
    m_colorButtons = new QMap<QString, QPushButton*>;
    m_cards = new QMap<QString, Card*>;

    // connect setupViewport and setupDrawer methods
    connect(m_dataFrame, &DataFrame::onFileChange, this, &DashboardEditor::setupViewport);
    connect(m_dataFrame, &DataFrame::onFileChange, this, &DashboardEditor::setupDrawer);

    // check if the dataframe is already ready
    if (m_dataFrame->isAlreadySetup()) {
        // call setupViewport and setupDrawer
        setupViewport();
        setupDrawer();
    }
}

void DashboardEditor::setupViewport() {
    // call parent function to delete the old viewport
    Editor::setupViewport();

    // create QWidget as viewport
    m_viewport = new QWidget(this);
    m_layout->addWidget(m_viewport, 0, 1, 1, 1);

    // add QGridLayout to viewport
    m_vpLayout = new QGridLayout(m_viewport);
    m_viewport->setLayout(m_vpLayout);

    // get size of dataframe
    int n = m_dataFrame->getKeys()->size();

    // current row and column
    int c_row = 0;
    int c_col = 0;

    // number of items in row
    int items = fmin(n, 3);

    for (int i = 0; i < n; ++i) {
        // get label and value
        QString key = m_dataFrame->getKeys()->at(i);
        QString value = key == QString("time") ? m_dataFrame->getDateTime()->toString(QString("hh:mm:ss.z")) : QString::number(m_dataFrame->getValue(key));

        // create Card
        Card *card = new Card(m_dataFrame, key, value, m_viewport);

        // add to layout
        m_vpLayout->addWidget(card, c_row, 6/items*c_col, 1, 6/items);

        ++c_col;
        if (c_col > 2) {
            c_col = 0;
            items = fmin(n-i, 3);
            ++c_row;
        }
    }

    // add property to dashboard
    setProperty("cssClass", "dashboard");
}

/*
void DashboardEditor::setupViewport() {
    // call parent function to delete the old viewport
    Editor::setupViewport();

    // create QMainWindow as viewport
    QMainWindow *mainWindow = new QMainWindow(this);

    // set viewport to mainWindow
    m_viewport = mainWindow;
    //m_layout->addWidget(m_viewport, 0, 1, 1, 1);
    m_viewport->setVisible(false);

    // get count of checkboxes and calculate rows
    int n = m_dataFrame->getKeys()->size();
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

        // create Card
        Card *card = new Card(m_dataFrame, key, value, mainWindow);

        // declare last key
        QString c_key;

        // if first column
        if (c_col == 0) {
            // if first row
            if (c_row == 0) {
                mainWindow->addDockWidget(Qt::RightDockWidgetArea, card);
            } else {
                // get last Card
                c_key = m_dataFrame->getKeys()->at(index-3);
                Card *l_card = m_cards->find(c_key).value();

                // split and resize the Cards
                mainWindow->splitDockWidget(l_card, card, Qt::Vertical);

                // resize Cards if necessairy
                if (c_row + 1 == rows) {
                    QList<QDockWidget*> rowCards;
                    QList<int> sizes;

                    // get all lastest Cards
                    for (int i = 0; i < rows-1; ++i) {
                        rowCards.append(m_cards->find(m_dataFrame->getKeys()->at(3*i)).value());
                        sizes.append(100);
                    }

                    // append current Card
                    rowCards.append(card);
                    sizes.append(100);

                    // resize Cards
                    mainWindow->resizeDocks(rowCards, sizes, Qt::Vertical);
                }
            }
        } else {
            // get last Card
            c_key = m_dataFrame->getKeys()->at(index-1);
            Card *l_card = m_cards->find(c_key).value();

            // split and resize the Cards
            mainWindow->splitDockWidget(l_card, card, Qt::Horizontal);

            // resize Cards if necessairy
            if(c_col == 2) {
                // get pre last Card
                QString lc_key = m_dataFrame->getKeys()->at(index-2);
                Card *ll_card = m_cards->find(lc_key).value();

                // resize Cards
                mainWindow->resizeDocks({ll_card, l_card, card}, {100, 100, 100}, Qt::Horizontal);
            } else if (c_col+1 == n) {
                // resize Cards
                mainWindow->resizeDocks({l_card, card}, {100, 100}, Qt::Horizontal);
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

        // add Card to labels
        m_cards->insert(key, card);
    }

    // add property to dashboard
    setProperty("cssClass", "dashboard");

    // set m_viewport visible
    m_viewport->setVisible(true);
}
*/

void DashboardEditor::setupDrawer() {
    // call parent function
    Editor::setupDrawer();

    // get parent
    QGroupBox* drawer = m_drawer->getSettings();

    // create QVBoxLayout
    QVBoxLayout *drawerLayout = new QVBoxLayout(drawer);

    for(QString key : *m_dataFrame->getKeys()){
        if (key != QString("time")) {
            QCheckBox *checkBox = new QCheckBox(key, drawer);
            checkBox->setCheckState(Qt::Checked);

            QPushButton *colorButton = new QPushButton(drawer);
            colorButton->setFixedSize(15, 15);
            colorButton->setStyleSheet("background-color: "+m_dataFrame->getColor(key)->name(QColor::HexRgb));

            connect(checkBox, &QCheckBox::clicked, [this, key](bool checked){toggleCard(key, checked);});

            connect(colorButton, &QPushButton::pressed, [this, key](){
                QColorDialog *colorDialog = new QColorDialog(this);

                connect(colorDialog, &QColorDialog::colorSelected, [this, key](QColor color){
                    m_cards->find(key).value()->m_label->setStyleSheet("color: " + color.name() + ";");
                });
                connect(colorDialog, &QColorDialog::colorSelected, [this, key](QColor color){
                    m_colorButtons->find(key).value()->setStyleSheet("background-color: " + color.name() + ";");
                });

                colorDialog->show();
            });

            // create container and add to layout
            QWidget *container = new QWidget();
            container->setProperty("cssClass", "drawerItem");

            QHBoxLayout *l = new QHBoxLayout;
            container->setLayout(l);
            l->addWidget(checkBox);
            l->addWidget(colorButton);

            drawerLayout->addWidget(container);

            m_checkBoxes->insert(key, checkBox);
            m_colorButtons->insert(key, colorButton);
        }
    }

    // add strech item last
    drawerLayout->addStretch(1);

    // add drawerLayout to drawer
    m_drawer->setupContent(drawerLayout);
}

void DashboardEditor::toggleCard(QString key, bool checked){
    if (checked){
        Card *c = new Card(m_dataFrame, key, QString::number(m_dataFrame->getValue(key)), m_viewport);
        qobject_cast<QMainWindow*>(m_viewport)->addDockWidget(Qt::BottomDockWidgetArea, c);

        m_cards->insert(key, c);
    }
    else {
        QDockWidget *l = m_cards->find(key).value();

        qobject_cast<QMainWindow*>(m_viewport)->removeDockWidget(l);
        delete l;
    }
}
