#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QDockWidget>

#include <QFile>
#include <QDateTime>

#include "dataframe.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_Editor_triggered();

private:
    Ui::MainWindow *ui;
    QTabWidget *tabWidget;

    DataFrame *m_dataFrame;
};
#endif // MAINWINDOW_H
