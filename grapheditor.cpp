#include "grapheditor.h"

#include <QLabel>

#include <QCheckBox>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QColorDialog>
#include <QChartView>
#include <QDateTime>

#include <QDebug>

#include <QMainWindow>

#include "chartview.h"


GraphEditor::GraphEditor(DataFrame *p_dataFrame, QWidget *parent)
    : Editor{p_dataFrame, parent}
{
    // create new QChart
    m_chart = new QChart;

    // set QMaps
    checkBoxes = new QMap<QString, QCheckBox*>;
    colorButtons = new QMap<QString, QPushButton*>;
    series = new QMap<QString, DataSeries*>;

    // check if log file is already loaded
    if (m_dataFrame->isAlreadySetup()){
        setupGraph();
        setupDrawer();

        // set properlySetup
        properlySetup = true;
    }
    else {
        // create QLabel as viewport
        failLabel = new QLabel(QString("Load a log file first!"), parent);
        failLabel->setAlignment(Qt::AlignCenter);

        m_viewport = failLabel;

        // add viewport to container
        m_container->addWidget(m_viewport);

        // set properlySetup
        properlySetup = false;
    }

    // connect reload of log file
    connect(m_dataFrame, &DataFrame::onFileChanged, [this](){
        delete failLabel;
        setupGraph();
        setupDrawer();
        properlySetup = true;
    });

}

void GraphEditor::setupDrawer() {
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
            colorButton->setStyleSheet("background-color: blue;");

            connect(checkBox, &QCheckBox::clicked, series->find(key).value(), &DataSeries::setVisibility);
            drawerLayout->addWidget(checkBox, row, 0);

            connect(colorButton, &QPushButton::pressed, [this, key](){
                QColorDialog *colorDialog = new QColorDialog;

                connect(colorDialog, &QColorDialog::colorSelected, series->find(key).value(), &DataSeries::setColor);
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

void GraphEditor::setupGraph() {
    // create QMainWindow as viewport
    ChartView *chartView = new ChartView(m_chart);
    m_viewport = chartView;

    // add viewport to container
    m_container->addWidget(m_viewport);

    // create Axis
    m_axisT = new QDateTimeAxis();
    m_axisY = new QValueAxis();

    m_chart->addAxis(m_axisT, Qt::AlignBottom);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);

    m_axisT->setRange(*m_dataFrame->getDateTime(0), *m_dataFrame->getDateTime(m_dataFrame->getSize()-1));
    m_axisY->setRange(0, 10);

    m_axisY->setTickAnchor(0);
    m_axisY->setTickInterval(5);

    m_axisT->setFormat(QString("hh:mm:ss.zzz"));

    // counting variable
    int i = 0;

    // create new DashboardLabels for each checked QCheckbox
    for (QString key : *m_dataFrame->getKeys()) {
        if (key != QString("time")) {
            DataSeries *ds = new DataSeries(m_dataFrame, key, m_axisT, m_axisY, m_chart);

            connect(ds->m_series, &QLineSeries::clicked, [chartView, ds](QPointF point){
                chartView->requestCallout(point, ds->m_key, true);
            });

            series->insert(key, ds);
            ++i;
        }
    }
}
