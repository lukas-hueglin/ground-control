#ifndef IOEditor_H
#define IOEditor_H

#include "editor.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>

/**
 * @brief The IOEditor class
 * Handels the input and output of data. The user can choose between loading
 * or streaming the data. Input-/output files resp. input channels can be selected.
 */
class IOEditor : public Editor
{
    Q_OBJECT
public:
    IOEditor(DataFrame *p_dataFrame, QWidget *parent = nullptr);

protected slots:
    void setupViewport() override;

private:
    void browse();
    void changeType(const int index);

private:
    QGridLayout *m_vpLayout;

    QComboBox *m_comboBox;
    QGroupBox *m_groupBox;

    QLineEdit *m_logFileSelection;
    QLineEdit *m_telemetrySelection;
    QLineEdit *m_videoSelection;

    QPushButton *m_logFileBrowseButton;

    QLabel *m_logFileLabel;
    QLabel *m_telemetryLabel;
    QLabel *m_videoLabel;

};

#endif // IOEditor_H
