#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QDockWidget>

#include <QFile>
#include <QDateTime>
#include <QtXml>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Workspace;
class Module;
class DataFrame;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    void parseLayout(QDomDocument *xml);
    void parseModule(Workspace *w, Module *fm, QDomElement e);

    void parseStylesheet(QDomDocument *xml, QString &str);

private slots:
    void on_actionNew_Editor_triggered();

private:
    Ui::MainWindow *ui;
    QTabWidget *m_tabWidget;

    DataFrame *m_dataFrame;
};
#endif // MAINWINDOW_H
