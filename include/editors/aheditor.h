#ifndef AHEditorEDITOR_H
#define AHEditorEDITOR_H

#include "editor.h"


class AHEditor : public Editor
{
    Q_OBJECT
public:
    AHEditor(DataFrame *p_dataFrame, QWidget *parent = nullptr);

protected slots:
    void setupViewport() override;

};

#endif // AHEditorEDITOR_H
