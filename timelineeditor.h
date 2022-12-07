#ifndef TIMELINEEDITOR_H
#define TIMELINEEDITOR_H

#include "editor.h"
#include "mainwindow.h"

#include <QPushButton>
#include <QSlider>
#include <QLabel>

class TimelineEditor : public Editor
{
    Q_OBJECT
public:
    explicit TimelineEditor(Module *parent = nullptr, MainWindow *mainWindow = nullptr);

public:
    void updateTime(int t) override;

private:
    void setupTimeline();

private slots:
    void playButtonPressed(bool toggled);

private:
    QPushButton *toBeginButton;
    QPushButton *backwardButton;
    QPushButton *playButton;
    QPushButton *toEndButton;
    QPushButton *forwardButton;

    QSlider *timeline;
    QLabel *timeIndicator;

    QLabel *failLabel;

    bool properlySetup;

};

#endif // TIMELINEEDITOR_H
