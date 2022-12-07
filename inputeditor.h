#ifndef INPUTEDITOR_H
#define INPUTEDITOR_H

#include "editor.h"

#include <QLineEdit>
#include <QPushButton>

#include "mainwindow.h"


class InputEditor : public Editor
{
    Q_OBJECT
public:
    explicit InputEditor(Module *parent = nullptr, MainWindow *mainWindow = nullptr);

private:
    QLineEdit *fileSelection;
    QPushButton *browseButton;

private slots:
    void browse();

};

#endif // INPUTEDITOR_H
