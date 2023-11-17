#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class Editor;

enum Status {
    OK = 0,
    SUCCESS = 1,
    WORKING = 2,
    FAIL = 3
};

/**
 * @brief The Statusbar class
 * Class representing the statusbar widget at the bottom of the module.
 * It can change color, indicating the current status and display information.
 */
class Statusbar : public QWidget
{
    Q_OBJECT

public:
    explicit Statusbar(QWidget *parent = nullptr);

public:
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

public slots:
    void setStatus(Status p_status, QString p_message);

private:
    QHBoxLayout *m_layout;

    Status m_status;
    QLabel *m_icon;
    QLabel *m_message;
};

#endif // STATUSBAR_H
