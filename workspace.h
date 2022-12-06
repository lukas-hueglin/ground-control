#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QMainWindow>


class Workspace : public QMainWindow
{
    Q_OBJECT
public:
    explicit Workspace(QWidget *parent = nullptr);

private:
    QMainWindow *parent;

};

#endif // WORKSPACE_H
