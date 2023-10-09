#ifndef INPUTEDITOR_H
#define INPUTEDITOR_H

#include "editor.h"

#include <QLineEdit>
#include <QPushButton>


class InputEditor : public Editor
{
    Q_OBJECT
public:
    InputEditor(DataFrame *p_dataFrame, QWidget *parent = nullptr);

private:
    QLineEdit *fileSelection;
    QPushButton *browseButton;

private slots:
    void browse();

};

#endif // INPUTEDITOR_H
