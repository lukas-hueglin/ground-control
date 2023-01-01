#ifndef MODULE_H
#define MODULE_H

#include <QMainWindow>
#include <QDockWidget>

#include "editor.h"
#include "workspace.h"
#include "dataframe.h"

class Module : public QDockWidget
{
    Q_OBJECT
public:
    explicit Module(DataFrame *p_dataFrame, Workspace *parent = nullptr);


public:
    Editor* getEditor();

public slots:
    void setStatusSuccess(QString message = QString());
    void setStatusWorking(QString message = QString());
    void setStatusOK(QString message = QString());
    void setStatusFail(QString message = QString());

    void changeEditor(const int index);
    void splitModuleHorizontally();
    void splitModuleVertically();
    void makeFloating();

private:
    DataFrame *m_dataFrame;
    Workspace *m_workspace; // is it really necessary

    QMainWindow *m_moduleWindow;

    Editor *m_editor;
};

#endif // MODULE_H
