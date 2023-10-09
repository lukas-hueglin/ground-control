#ifndef EDITOR_H
#define EDITOR_H

#include <QHBoxLayout>
#include <QGroupBox>
#include <QToolButton>
#include <QParallelAnimationGroup>

#include "dataframe.h"

class Module;


class Editor : public QWidget
{
    Q_OBJECT
public:
    Editor(DataFrame *p_dataFrame, QWidget *parent = nullptr);

public:
    QWidget* getViewport();

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void setupDrawer();

signals:
    void onStatusChangeSuccess(QString message = QString());
    void onStatusChangeWorking(QString message = QString());
    void onStatusChangeOK(QString message = QString());
    void onStatusChangeFail(QString message = QString());

protected:
    DataFrame *m_dataFrame;

    QHBoxLayout *m_container;
    QWidget *m_viewport;

    QGroupBox *m_drawer;
    QToolButton *m_drawerButton;
    QParallelAnimationGroup *m_drawerAnim;
};

#endif // EDITOR_H
