#include "dataframe.h"

#include <QMap>
#include <QDateTime>
#include <QTimer>


DataFrame::DataFrame(QFile *p_file, QObject *parent)
    : QObject{parent}
{
    // set file
    m_file = p_file;

    // create QLists
    m_times = new QList<QDateTime*>;
    m_values = new QList<QMap<QString, float>*>;
    m_keys = new QList<QString>;

    // set size
    m_size = 0;

    // set time;
    m_time = 0;

    // set play
    m_play = false;

    // load file
    if (p_file != nullptr) {
        if (p_file->open(QIODevice::ReadOnly)) {
            setFile(p_file);
        }
        else {
            m_alreadySetup = false;
        }
    }
    else {
        m_alreadySetup = false;
    }
}

float DataFrame::getValue(QString p_key) {
    return m_values->at(m_time)->find(p_key).value();
}

QList<QString>* DataFrame::getKeys() {
    return m_keys;
}

QDateTime* DataFrame::getDateTime() {
    return m_times->at(m_time);
}

unsigned int DataFrame::getTime() {
    return m_time;
}

unsigned int DataFrame::getSize() {
    return m_size;
}

bool DataFrame::isAlreadySetup() {
    return m_alreadySetup;
}

void DataFrame::setTime(unsigned int p_time) {
    m_time = p_time;
    emit onTimeChanged(m_time);
}

void DataFrame::setPlayState(bool p_play) {
    if (!m_play && p_play) {
        m_play = p_play;
        playLoop();
    }

    m_play = p_play;
    emit onPlayStateChanged(m_play);
}

void DataFrame::setFile(QFile *p_file){
    // set file
    m_file = p_file;

    // open file
    m_file->open(QIODevice::ReadOnly);

    // read first line and get headers
    QString firstLine = m_file->readLine();
    firstLine = firstLine.left(firstLine.size()-2);

    for (QString str : firstLine.split(",")){
        m_keys->append(str);
    }

    // load all lines
    while (!m_file->atEnd()) {
        // read line
        QString line = m_file->readLine();
        line = line.left(line.size()-2); // remove \r\n

        // store single strings in QList
        QList<QString> elements = line.split(",").toList();

        // create QMap
        QMap<QString, float> *map = new QMap<QString, float>;

        // for every col
        for (int i = 0; i < m_keys->size(); ++i){
            // if col is time
            if (m_keys->at(i) == QString("time")) {
                QString date = elements.at(i).split(" ").at(0);
                QString time = elements.at(i).split(" ").at(1);

                int y = date.split("-").at(0).toInt();
                int mo = date.split("-").at(1).toInt();
                int d = date.split("-").at(2).toInt();

                int h = time.split(":").at(0).toInt();
                int mi = time.split(":").at(1).toInt();
                int s = time.split(":").at(2).toInt();

                QDateTime *datetime = new QDateTime(QDate(y, mo, d), QTime(h, mi, s));
                m_times->append(datetime);
            }
            // else if it is not time
            else {
                map->insert(m_keys->at(i), elements.at(i).toFloat());
            }
        }

        m_values->append(map);
    }

    m_size = m_times->size();
    m_alreadySetup = true;

    emit onFileChanged(m_file);
}

void DataFrame::playLoop() {
    if (m_play) {
        QDateTime *t1 = m_times->at(m_time);
        QDateTime *t2 = m_times->at(m_time+1);

        int ms = t1->daysTo(*t2)*1000*60*60*24 + t1->time().msecsTo(t2->time());

        emit onTimeChanged(++m_time);
        QTimer::singleShot(ms, [this](){playLoop();});
    }
}

