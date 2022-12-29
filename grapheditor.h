#ifndef GRAPHEDITOR_H
#define GRAPHEDITOR_H

#include "editor.h"

class GraphEditor : public Editor
{
    Q_OBJECT
public:
    GraphEditor(DataFrame *p_dataFrame, QWidget *parent = nullptr);

signals:

};

#endif // GRAPHEDITOR_H
