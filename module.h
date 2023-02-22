#ifndef MODULE_H
#define MODULE_H

#include <QMainWindow>
#include <QDockWidget>

#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>

#include "editor.h"
#include "workspace.h"
#include "dataframe.h"


class Titlebar : public QWidget
{
    Q_OBJECT
public:
    explicit Titlebar(Module *parent = nullptr);

public:
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    QComboBox* getComboBox();

private:
    QComboBox *m_comboBox;
};

class Statusbar : public QWidget
{
    Q_OBJECT
public:
    explicit Statusbar(Module *parent = nullptr);

public:
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    void setIcon(QString str);
    void setMessage(QString str);

private:
    QLabel *m_icon;
    QLabel *m_message;
};

class ContainerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ContainerWidget(Module *parent = nullptr);

public:
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
};


class Module : public QDockWidget
{
    Q_OBJECT
public:
    explicit Module(DataFrame *p_dataFrame, Workspace *parent = nullptr);


public:
    Editor* getEditor();
    void changeComboBox(int index); // not really pretty

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

public slots:
    void setStatusSuccess(QString message = QString());
    void setStatusWorking(QString message = QString());
    void setStatusOK(QString message = QString());
    void setStatusFail(QString message = QString());

    void makeFloating();

    Module* splitModuleHorizontally();
    Module* splitModuleVertically();

    void changeEditor(const int index);

private:
    DataFrame *m_dataFrame;
    Workspace *m_workspace; // is it really necessary

    Titlebar *m_titlebar;
    Statusbar *m_statusbar;

    QVBoxLayout *m_layout;

    Editor *m_editor;
};

#endif // MODULE_H
