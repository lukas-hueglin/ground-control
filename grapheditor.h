#ifndef GRAPHEDITOR_H
#define GRAPHEDITOR_H

#include "editor.h"
#include "mainwindow.h"

class GraphEditor : public Editor
{
    Q_OBJECT
public:
    explicit GraphEditor(Module *parent = nullptr, MainWindow *mainWindow = nullptr);

signals:

};

#endif // GRAPHEDITOR_H
