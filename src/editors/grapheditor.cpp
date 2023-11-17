#include "editors/grapheditor.h"

#include <QLabel>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QColorDialog>
#include <QChartView>
#include <QDateTime>
#include <QVector>

#include <QMainWindow>

#include "helpers/drawer.h"

GraphEditor::GraphEditor(DataFrame *p_dataFrame, QWidget *parent)
    : Editor{p_dataFrame, parent}
{
    // connect setupViewport and setupDrawer methods
    connect(m_dataFrame, &DataFrame::onFileChange, this, &GraphEditor::setupViewport);
    connect(m_dataFrame, &DataFrame::onFileChange, this, &GraphEditor::setupDrawer);

    // check if the dataframe is already ready
    if (m_dataFrame->isAlreadySetup()) {
        // call setupViewport and setupDrawer
        setupViewport();
        setupDrawer();
    }
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

void GraphEditor::setupViewport() {
    // call parent function to delete the old viewport
    Editor::setupViewport();

    // create QCustomPlot
    m_plot = new QCustomPlot(this);
    m_plot->addGraph();

    // set QMaps
    checkBoxes = new QMap<QString, QCheckBox*>;
    colorButtons = new QMap<QString, QPushButton*>;
    series = new QMap<QString, DataSeries*>;

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

    // set viewport to cutromPlot
    m_viewport = m_plot;
    m_layout->addWidget(m_viewport, 0, 1, 1, 1);
}

void GraphEditor::setupDrawer() {
    // call parent function
    Editor::setupDrawer();

    // get parent
    QGroupBox* drawer = m_drawer->getSettings();

    // create QVBoxLayout
    QVBoxLayout *drawerLayout = new QVBoxLayout(drawer);

    // create QCheckbox for aligning the plot on the x-axis
    QCheckBox *centerBox = new QCheckBox(QString("Align x-Axis"), drawer);
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
            QCheckBox *checkBox = new QCheckBox(key, drawer);
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

            QHBoxLayout *l = new QHBoxLayout(container);
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

    // add drawerLayout to drawer
    m_drawer->setupContent(drawerLayout);
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
