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

protected slots:
    void setupViewport() override;

private:
    QPushButton *m_toBeginButton;
    QPushButton *m_backwardButton;
    QPushButton *m_playButton;
    QPushButton *m_toEndButton;
    QPushButton *m_forwardButton;
    QPushButton *m_speedButton;

    QSlider *m_timeline;
    QLabel *m_timeIndicator;

};

#endif // TIMELINEEDITOR_H
