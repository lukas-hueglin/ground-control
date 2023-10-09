#ifndef AHEditorEDITOR_H
#define AHEditorEDITOR_H


#include "editor.h"
#include "qlabel.h"


class AHEditor : public Editor
{
    Q_OBJECT
public:
    AHEditor(DataFrame *p_dataFrame, QWidget *parent = nullptr);

private:
    void setupHorizon();

private:
    QLabel *failLabel;

    bool properlySetup;

};

#endif // AHEditorEDITOR_H
