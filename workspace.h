#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QMainWindow>

#include "editor.h"

class Workspace : public QMainWindow
{
    Q_OBJECT
public:
    explicit Workspace(QWidget *parent = nullptr);

signals:

};

#endif // WORKSPACE_H
