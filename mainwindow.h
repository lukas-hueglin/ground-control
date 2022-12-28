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
    void loadDataFromFile();
    int getDataLen();

    QDateTime* getDateTime(int i);
    float getValue(int i, QString key);
    QList<QString>* getKeys();

public slots:
    void updateTime(int t);
    void playTime(bool start);
    void pauseTime();

private slots:
    void on_actionNew_Editor_triggered();

private:
    Ui::MainWindow *ui;
    QTabWidget *tabWidget;

    QFile *file;

    QList<QDateTime*> *times;
    QList<QMap<QString, float>*> *values;
    QList<QString> *keys;

    int dataLen;
    int time;

    bool play;
};
#endif // MAINWINDOW_H
