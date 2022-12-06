#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QDockWidget>

#include <QFile>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void setFile(QFile *f);

public slots:
    void loadDataFromFile();

private slots:
    void on_actionNew_Editor_triggered();

private:
    Ui::MainWindow *ui;
    QTabWidget *tabWidget;

    QFile *file;

    QList<QDateTime*> *times;

    QList<QMap<QString, float>*> *values;
};
#endif // MAINWINDOW_H
