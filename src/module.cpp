#include "module.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QStatusBar>
#include <QTimer>

#include "workspace.h"
#include "editor.h"

#include "editors/grapheditor.h"
#include "editors/ioeditor.h"
#include "editors/timelineeditor.h"
#include "editors/dashboardeditor.h"
#include "editors/globeeditor.h"
#include "editors/mapeditor.h"
#include "editors/aheditor.h"

#include "helpers/titlebar.h"

Module::Module(DataFrame *p_dataFrame, Workspace *parent, char editorIndex)
    : QDockWidget{parent}
{
    // set dataFrame
    m_dataFrame = p_dataFrame;

    // set parent
    m_workspace = parent;

    // create new Editor
    m_editor = nullptr;
    changeEditor(editorIndex);

    // create new Titlebar
    m_titlebar = new Titlebar(editorIndex, this);
    setTitleBarWidget(m_titlebar);
}

QSize Module::sizeHint() const {
    return QSize(300, 200);
}

QSize Module::minimumSizeHint() const {
    return QSize(0, 0);
}

struct BaseEditorFactory {
    virtual Editor* create(DataFrame* df, QWidget* p){
        return new Editor(df, p);
    }
};

template <typename T> struct EditorFactory : public BaseEditorFactory {
    virtual T* create(DataFrame* df, QWidget* p) {
        return new T(df, p);
    }
};

// change editor accordingly to QComboBox
void Module::changeEditor(const int index) {
    QList<BaseEditorFactory*> factories = {
        new EditorFactory<Editor>,
        new EditorFactory<GraphEditor>,
        new EditorFactory<IOEditor>,
        new EditorFactory<TimelineEditor>,
        new EditorFactory<DashboardEditor>,
        new EditorFactory<GlobeEditor>,
        new EditorFactory<MapEditor>,
        new EditorFactory<AHEditor>,
    };

    // remove old editor
    delete m_editor;

    // create new editor and set as widget
    m_editor = factories.at(index)->create(m_dataFrame, this);
    setWidget(m_editor);
}

// makes the module float
void Module::makeFloating() {
    setFloating(true);
}

// splits the module into two modules, these are horizontally arranged
Module* Module::splitModuleHorizontally() {
    // let the parent workspace split the Modules
    if (m_workspace != nullptr){
        Module* newModule = new Module(m_dataFrame, m_workspace);
        m_workspace->addModule(newModule);
        m_workspace->splitDockWidget(this, newModule, Qt::Horizontal);

        return newModule;
    }
    return nullptr;
}

// splits the module into two modules, these are vertically arranged
Module* Module::splitModuleVertically() {
    // let the parent workspace split the Modules
    if (m_workspace != nullptr){
        Module* newModule = new Module(m_dataFrame, m_workspace);
        m_workspace->addModule(newModule);
        m_workspace->splitDockWidget(this, newModule, Qt::Vertical);

        return newModule;
    }
    return nullptr;
}

Editor* Module::getEditor() {
    return m_editor;
}

void Module::changeComboBox(int index) {
    m_titlebar->getComboBox()->setCurrentIndex(index);
}
