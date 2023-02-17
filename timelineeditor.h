#ifndef TIMELINEEDITOR_H
#define TIMELINEEDITOR_H

#include "editor.h"

#include <QPushButton>
#include <QSlider>
#include <QLabel>


class TimelineEditor : public Editor
{
    Q_OBJECT
public:
    TimelineEditor(DataFrame *p_dataFrame, QWidget *parent = nullptr);

private:
    void setupTimeline();

private:
    QPushButton *toBeginButton;
    QPushButton *backwardButton;
    QPushButton *playButton;
    QPushButton *toEndButton;
    QPushButton *forwardButton;
    QPushButton *speedButton;

    QSlider *timeline;
    QLabel *timeIndicator;

    QLabel *failLabel;

    bool properlySetup;

};

#endif // TIMELINEEDITOR_H
