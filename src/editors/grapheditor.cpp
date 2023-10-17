#include "editors/grapheditor.h"

#include <QLabel>

#include <QCheckBox>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QColorDialog>
#include <QChartView>
#include <QDateTime>
#include <QVector>

#include <QDebug>

#include <QMainWindow>

GraphEditor::GraphEditor(DataFrame *p_dataFrame, QWidget *parent)
    : Editor{p_dataFrame, parent}
{
    // create QCustomPlot
    m_plot = new QCustomPlot;
    m_plot->addGraph();

    // set QMaps
    checkBoxes = new QMap<QString, QCheckBox*>;
    colorButtons = new QMap<QString, QPushButton*>;
    series = new QMap<QString, DataSeries*>;

    // create QLabel as viewport
    failLabel = new QLabel(QString("Load a log file first!"), parent);
    failLabel->setAlignment(Qt::AlignCenter);

    // check if log file is already loaded
    if (m_dataFrame->isAlreadySetup()){
        setupGraph();
        setupDrawer();

        // set properlySetup
        properlySetup = true;
    }
    else {

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

void GraphEditor::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        m_plot->axisRect()->setRangeZoom(Qt::Vertical);
        event->accept();
    }
    Editor::keyPressEvent(event);
}

void GraphEditor::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        m_plot->axisRect()->setRangeZoom(Qt::Horizontal);
        event->accept();
    }
    Editor::keyReleaseEvent(event);
}

void GraphEditor::setupDrawer() {
    // call parent function
    Editor::setupDrawer();

    // create QVBoxLayout
    QVBoxLayout *drawerLayout = new QVBoxLayout(m_drawer);
    m_drawer->setLayout(drawerLayout);

    // create QCheckbox for aligning the plot on the x-axis
    QCheckBox *centerBox = new QCheckBox(QString("Align x-Axis"), m_drawer);
    centerBox->setProperty("cssClass", "drawerItem");
    centerBox->setChecked(false);
    drawerLayout->addWidget(centerBox);

    connect(centerBox, &QCheckBox::clicked, [this](bool state){
        if(state){
            m_plot->yAxis->setAxisFreedom(QCPAxis::centered);
        } else {
            m_plot->yAxis->setAxisFreedom(QCPAxis::aligned);
        }
    });

    // counter variable for row
    int row = 1;

    for(QString key : *m_dataFrame->getKeys()){
        if (key != QString("time")) {
            QCheckBox *checkBox = new QCheckBox(key, m_drawer);
            checkBox->setCheckState(Qt::Checked);

            QPushButton *colorButton = new QPushButton;
            colorButton->setFixedSize(15, 15);
            colorButton->setStyleSheet("background-color: "+m_dataFrame->getColor(key)->name()+";");

            connect(checkBox, &QCheckBox::clicked, [this, row](bool checked){
                m_plot->graph(row)->setVisible(checked);
            });

            connect(colorButton, &QPushButton::pressed, [this, colorButton, row](){
                QColorDialog *colorDialog = new QColorDialog(this);

                connect(colorDialog, &QColorDialog::colorSelected, [this, row](QColor color){
                    setGraphColor(row, color);
                });
                connect(colorDialog, &QColorDialog::colorSelected, [colorButton](QColor color){
                    colorButton->setStyleSheet("background-color: " + color.name() + ";");
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

            checkBoxes->insert(key, checkBox);
            colorButtons->insert(key, colorButton);
            ++row;
        }
    }

    // add strech item last
    drawerLayout->addStretch(1);
}

void GraphEditor::setupGraph() {
    // add viewport to container
    m_viewport = m_plot;
    m_container->addWidget(m_viewport, 1);

    m_plot->setInteraction(QCP::iRangeDrag, true);
    m_plot->yAxis->setAxisFreedom(QCPAxis::aligned);
    m_plot->xAxis->setAxisFreedom(QCPAxis::aligned);

    m_plot->setInteraction(QCP::iRangeZoom, true);
    m_plot->axisRect()->setRangeZoom(Qt::Horizontal);

    // remove legend
    QVector<double> *times = m_dataFrame->getTimes();

    for (QString key : *m_dataFrame->getKeys()){
        if (key != QString("time")) {
            QVector<double> *values = m_dataFrame->getValues(key);
            QVector<double> *v = new QVector<double>;
            QVector<double> *t = new QVector<double>;

            for (int i = 0; i<values->size(); i+=50) {
                v->append(values->at(i));
                t->append(times->at(i));
            }

            m_plot->addGraph();
            m_plot->graph()->setName(key);

            m_plot->graph()->setLineStyle(QCPGraph::lsLine);
            setGraphColor(*m_dataFrame->getColor(key));

            m_plot->graph()->setData(*t, *v);
        }
    }

    // configure bottom axis to show date instead of number:
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("hh:mm::ss \n (d. MMMM)");
    m_plot->xAxis->setTicker(dateTicker);
    // configure left axis text labels:
    QSharedPointer<QCPAxisTicker> Ticker(new QCPAxisTicker);
    Ticker->setTickCount(5);
    m_plot->yAxis->setTicker(Ticker);
    // set a more compact font size for bottom and left axis tick labels:
    m_plot->xAxis->setTickLabelFont(QFont(QFont().family(), 8, 8));
    m_plot->yAxis->setTickLabelFont(QFont(QFont().family(), 8, 8));
    m_plot->xAxis->setTickLabelColor(QColor(227, 227, 230));
    m_plot->yAxis->setTickLabelColor(QColor(227, 227, 230));
    m_plot->xAxis->setLabelColor(QColor(227, 227, 230));
    m_plot->yAxis->setLabelColor(QColor(227, 227, 230));
    // set axis labels:
    m_plot->xAxis->setLabel("Date Time");
    m_plot->yAxis->setLabel("Value");
    // make top and right axes visible but without ticks and labels:
    m_plot->xAxis2->setVisible(false);
    m_plot->yAxis2->setVisible(false);
    // set axis ranges to show all data:
    m_plot->xAxis->setRange(m_dataFrame->getDateTime(0)->toMSecsSinceEpoch(), m_dataFrame->getDateTime(m_dataFrame->getSize()-1)->toMSecsSinceEpoch());
    m_plot->yAxis->setRange(0, 60);
    m_plot->xAxis->setOrigin(m_dataFrame->getDateTime(0)->toMSecsSinceEpoch());
    // show legend with slightly transparent background brush:
    m_plot->legend->setVisible(false); // legend hidden
    m_plot->legend->setBrush(QColor(255, 255, 255, 20));
    m_plot->legend->setTextColor(QColor(227, 227, 230));

    m_plot->setBackground(QBrush(QColor(37, 37, 38)));
    m_plot->xAxis->setBasePen(QPen(QColor(0, 0, 0, 0)));
    m_plot->yAxis->setBasePen(QPen(QColor(0, 0, 0, 0)));
    m_plot->xAxis->setTickPen(QPen(QColor(0, 0, 0, 0)));
    m_plot->yAxis->setTickPen(QPen(QColor(0, 0, 0, 0)));
    m_plot->xAxis->setSubTickPen(QPen(QColor(0, 0, 0, 0)));
    m_plot->yAxis->setSubTickPen(QPen(QColor(0, 0, 0, 0)));

    // change grid color to @Secondary02
    m_plot->xAxis->grid()->setPen(QPen(QColor(59, 59, 64), 1, Qt::SolidLine));
    m_plot->yAxis->grid()->setPen(QPen(QColor(59, 59, 64), 1, Qt::SolidLine));
}


void GraphEditor::setGraphColor(int index, QColor color) {
    // Pen
    m_plot->graph(index)->setPen(QPen(color.lighter(100), 1.5));

    // Brush
    color.setAlphaF(0.35);
    m_plot->graph(index)->setBrush(QBrush(color));
}

void GraphEditor::setGraphColor(QColor color) {
    // Pen
    m_plot->graph()->setPen(QPen(color.lighter(100), 1.5));

    // Brush
    color.setAlphaF(0.35);
    m_plot->graph()->setBrush(QBrush(color));
}
