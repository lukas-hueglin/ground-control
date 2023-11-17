#ifndef MODULE_H
#define MODULE_H

#include <QMainWindow>
#include <QDockWidget>

#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>

class DataFrame;
class Workspace;
class Editor;
class Titlebar;


class Module : public QDockWidget
{
    Q_OBJECT
public:
    explicit Module(DataFrame *p_dataFrame, Workspace *parent = nullptr, char editorIndex=0);

public:
    Editor* getEditor();

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    void changeComboBox(int index);

public slots:
    void makeFloating();

    Module* splitModuleHorizontally();
    Module* splitModuleVertically();

    void changeEditor(const int index);

private:
    DataFrame *m_dataFrame;
    Workspace *m_workspace; // is it really necessary

    Editor* m_editor;
    Titlebar* m_titlebar;
};

#endif // MODULE_H
