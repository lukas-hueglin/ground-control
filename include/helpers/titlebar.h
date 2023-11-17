#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QComboBox>

class Module;

/**
 * @brief The Titlebar class
 * Class representing the titlebar widget at the top of the module
 */
class Titlebar : public QWidget
{
    Q_OBJECT
public:
    explicit Titlebar(char editorIndex = 0, Module *parent = nullptr);

public:
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    QComboBox* getComboBox();

private:
    QComboBox *m_comboBox;
};

#endif // TITLEBAR_H
