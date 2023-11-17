#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QGridLayout>

#include "dataframe.h"

class Module;
class Statusbar;
class Drawer;

class Editor : public QWidget
{
    Q_OBJECT
public:
    Editor(DataFrame *p_dataFrame, QWidget *parent = nullptr);

public:
    QWidget* getViewport();

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected slots:
    virtual void setupDrawer();
    virtual void setupViewport();

protected:
    DataFrame *m_dataFrame;

    QGridLayout* m_layout;

    QWidget *m_viewport;
    Statusbar* m_statusbar;
    Drawer* m_drawer;
};

#endif // EDITOR_H
