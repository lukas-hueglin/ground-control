#ifndef IOEditor_H
#define IOEditor_H

#include "editor.h"

#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>


class IOEditor : public Editor
{
    Q_OBJECT
public:
    IOEditor(DataFrame *p_dataFrame, QWidget *parent = nullptr);

private:
    QComboBox *m_comboBox;
    QGroupBox *m_groupBox;

    QLineEdit *m_logFileSelection;
    QLineEdit *m_telemetrySelection;
    QLineEdit *m_videoSelection;

    QPushButton *m_logFileBrowseButton;

    QLabel *m_logFileLabel;
    QLabel *m_telemetryLabel;
    QLabel *m_videoLabel;

private:
    void browse(QLineEdit *p_lineEdit);
    void changeType(const int index);

};

#endif // IOEditor_H
