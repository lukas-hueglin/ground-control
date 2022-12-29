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
    QList<QString>* getKeys();
    QDateTime* getDateTime();
    unsigned int getTime();
    unsigned int getSize();
    bool isAlreadySetup();

public slots:
    void setTime(unsigned int p_time);
    void setPlayState(bool p_play);
    void setFile(QFile *p_file);

private slots:
    void playLoop();

signals:
    void onTimeChanged(unsigned int p_time);
    void onPlayStateChanged(bool p_play);
    void onFileChanged(QFile *p_file);

private:
    QFile *m_file;

    QList<QDateTime*> *m_times;
    QList<QMap<QString, float>*> *m_values;
    QList<QString> *m_keys;

    unsigned int m_size;
    unsigned int m_time;

    bool m_play;
    bool m_alreadySetup;
};

#endif // DATAFRAME_H
