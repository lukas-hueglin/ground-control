#include "dataframe.h"

#include <QMap>
#include <QDateTime>
#include <QTimer>
#include <QtXml>
#include <QColor>


DataFrame::DataFrame(QFile *p_file, QObject *parent)
    : QObject{parent}
{
    // set file
    m_file = p_file;

    // create QLists
    m_times = new QVector<double>;
    m_dateTimes = new QVector<QDateTime*>;
    m_values = new QMap<QString, QVector<double>*>;
    m_colors = new QMap<QString, QColor*>;
    m_keys = new QList<QString>;

    // set size
    m_size = 0;

    // set time;
    m_time = 0;

    // set speed
    m_speed = 1;

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

// has to be improved, not nice
float DataFrame::getValue(QString p_key) {
    return m_values->find(p_key).value()->at(m_time);
}

// has to be improved, not nice
float DataFrame::getValue(QString p_key, unsigned int p_time) {
    return m_values->find(p_key).value()->at(p_time);
}

QColor* DataFrame::getColor(QString p_key){
    return m_colors->find(p_key).value();
}

QVector<double>* DataFrame::getValues(QString p_key) {
    return m_values->find(p_key).value();
}

QVector<double>* DataFrame::getTimes() {
    return m_times;
}

QDateTime* DataFrame::getDateTime() {
    return m_dateTimes->at(m_time);
}

QDateTime* DataFrame::getDateTime(unsigned int p_time){
    return m_dateTimes->at(p_time);
}

QList<QString>* DataFrame::getKeys() {
    return m_keys;
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

        QVector<double>* vec = new QVector<double>;
        m_values->insert(str, vec);
    }

    // load all lines
    while (!m_file->atEnd()) {
        // read line
        QString line = m_file->readLine();
        line = line.left(line.size()-2); // remove \r\n

        // store single strings in QList
        QList<QString> elements = line.split(",").toList();

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
                m_dateTimes->append(datetime);
                m_times->append(datetime->toMSecsSinceEpoch());
            }
            // else if it is not time
            else {
                m_values->find(m_keys->at(i)).value()->append(elements.at(i).toFloat());
            }
        }
    }

    m_size = m_dateTimes->size();

    // set colors
    QDomDocument *xml = new QDomDocument;
    QFile colorFile(":/themes/colorpresets.xml");

    // check if file can be opened
    if (colorFile.open(QFile::ReadOnly | QIODevice::Text)) {
        xml->setContent(&colorFile);

        // get palette
        QDomElement palette = xml->documentElement();

        // go through all elements
        QDomElement element = palette.firstChildElement();

        while(!element.isNull()) {
            QString key = element.attribute("NAME", "noName");
            QColor *color = new QColor(element.text());

            if (key != QString("noName")) {
                m_colors->insert(key, color);
            }

            element = element.nextSiblingElement();
        }
    }

    m_alreadySetup = true;

    emit onFileChanged(m_file);
}

void DataFrame::increaseSpeed() {
    m_speed *= 2;

    if (m_speed > 32) {
        m_speed = 1;
    }
    emit onSpeedIncreased(m_speed);
}

void DataFrame::playLoop() {
    if (m_play) {
        QDateTime *t1 = m_dateTimes->at(m_time);
        QDateTime *t2 = m_dateTimes->at(m_time+1);

        int ms = t1->daysTo(*t2)*1000*60*60*24 + t1->time().msecsTo(t2->time());

        emit onTimeChanged(++m_time);
        QTimer::singleShot(ms/m_speed, [this](){playLoop();});
    }
}

