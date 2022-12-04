#ifndef EDITOR_H
#define EDITOR_H

#include <QDockWidget>
#include <QMainWindow>

#include <QGroupBox>
#include <QToolButton>

#include <QParallelAnimationGroup>


class Editor : public QDockWidget
{
    Q_OBJECT
public:
    explicit Editor(QWidget *parent = nullptr);

public slots:
    void splitEditorHorizontally();
    void splitEditorVertically();

private:
    QMainWindow *workspace;
    QMainWindow *mainWindow;
    QWidget* viewport;

    QGroupBox *drawer;
    QToolButton *drawerButton;
    QParallelAnimationGroup *drawerAnim;

};

#endif // EDITOR_H
