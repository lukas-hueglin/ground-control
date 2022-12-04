#ifndef MODULE_H
#define MODULE_H

#include <QMainWindow>
#include <QDockWidget>

#include "editor.h"

class Module : public QDockWidget
{
    Q_OBJECT
public:
    explicit Module(QWidget *parent = nullptr);

public slots:
    void changeEditor(const int index);
    void splitModuleHorizontally();
    void splitModuleVertically();
    void makeFloating();

private:
    QMainWindow *workspace;
    QMainWindow *mainWindow;
    Editor *editor;

};

#endif // MODULE_H
