#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <QObject>
#include <QFile>


class DataFrame : public QObject
{
    Q_OBJECT

public:
    DataFrame(QFile *p_file=nullptr, QObject *parent = nullptr);

public:
    float getValue(QString p_key);
    float getValue(QString p_key, unsigned int p_time);

    QColor* getColor(QString p_key);

    QVector<double>* getValues(QString p_key);
    QVector<double>* getTimes();

    QDateTime* getDateTime();
    QDateTime* getDateTime(unsigned int p_time);

    QList<QString>* getKeys();
    unsigned int getTime();
    unsigned int getSize();

    bool isAlreadySetup();

public slots:
    void setTime(unsigned int p_time);
    void setPlayState(bool p_play);
    void setFile(QFile *p_file);

    void increaseSpeed();

private slots:
    void playLoop();

signals:
    void onTimeChanged(unsigned int p_time);
    void onPlayStateChanged(bool p_play);
    void onFileChanged(QFile *p_file);
    void onSpeedIncreased(int p_speed);

private:
    QFile *m_file;

    QVector<double> *m_times;
    QVector<QDateTime*> *m_dateTimes;

    QMap<QString, QVector<double>*> *m_values;
    QMap<QString, QColor*> *m_colors;
    QList<QString> *m_keys;

    unsigned int m_size;
    unsigned int m_time;

    int m_speed;

    bool m_play;
    bool m_alreadySetup;
};

#endif // DATAFRAME_H
