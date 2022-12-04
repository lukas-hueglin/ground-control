#ifndef EDITOR_H
#define EDITOR_H

#include <QDockWidget>
#include <QMainWindow>

#include <QGroupBox>
#include <QToolButton>

#include <QParallelAnimationGroup>


class Editor : public QWidget
{
    Q_OBJECT
public:
    explicit Editor(QWidget *parent = nullptr);

public:
    QWidget* getViewport();

protected:
    void setupDrawer();

protected:
    QWidget *viewport;

    QGroupBox *drawer;
    QToolButton *drawerButton;
    QParallelAnimationGroup *drawerAnim;

};

#endif // EDITOR_H
