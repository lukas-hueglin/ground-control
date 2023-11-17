#include "helpers/statusbar.h"

#include <QTimer>

#include <module.h>

Statusbar::Statusbar(QWidget *parent)
    : QWidget{parent}
{
    // create new QWidget to fill the background
    //new QWidget(this);

    // create a QHBoxLayout
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    setLayout(m_layout);

    // create icon
    m_icon = new QLabel("");
    m_icon->setPixmap(QIcon(":/icons/dark_OK.svg").pixmap(QSize(20, 20)));
    m_icon->setAlignment(Qt::AlignCenter);
    m_icon->setFixedSize(QSize(50, 25));
    m_layout->addWidget(m_icon);


    // create message
    m_message = new QLabel("");
    m_message->setAlignment(Qt::AlignVCenter);
    m_message->setFixedHeight(25);
    m_layout->addWidget(m_message, 1);

    // set status
    setStatus(Status::OK, QString(""));

}

QSize Statusbar::sizeHint() const {
    return QSize(300, 25);
}

QSize Statusbar::minimumSizeHint() const {
    return QSize(0, 0);
}

void Statusbar::setStatus(Status p_status, QString p_message) {
    // change icon and
    switch(p_status) {
        case(Status::OK):
            m_message->setStyleSheet(QString("background-color: hsv(279, 94%, 89%);"));
            m_icon->setPixmap(QIcon(":/icons/dark_OK.svg").pixmap(QSize(20, 20)));
            break;
        case(Status::SUCCESS):
            m_message->setStyleSheet(QString("background-color: hsv(117, 76%, 73%)"));
            m_icon->setPixmap(QIcon(":/icons/dark_success.svg").pixmap(QSize(20, 20)));

            // the success status will only be shown shortly
            QTimer::singleShot(7000, [this](){setStatus(m_status, m_message->text());});
            break;
        case(Status::WORKING):
            m_message->setStyleSheet(QString("background-color: hsv(209, 84%, 97%);"));
            m_icon->setPixmap(QIcon(":/icons/dark_working.svg").pixmap(QSize(20, 20)));
            break;
        case(Status::FAIL):
            m_message->setStyleSheet(QString("background-color: hsv(0, 87%, 79%);"));
            m_icon->setPixmap(QIcon(":/icons/dark_error.svg").pixmap(QSize(20, 20)));
            break;
    }

    // set new status
    m_status = p_status;

    // set message
    m_message->setText(p_message);
}
