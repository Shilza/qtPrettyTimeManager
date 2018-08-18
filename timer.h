#ifndef TIMER_H
#define TIMER_H
#include <QTimer>
#include "def.h"
#include <QObject>

class Timer : public QObject
{
    Q_OBJECT

public:
    Timer();

    void stop();
    void start();
    void setTimerMillsCounter(quint32 value);
    void setTimerMilliseconds(quint32 value);

private:
    QTimer *timer;
    quint32 timerMilliseconds = 0;
    quint32 timerMillisecondsCounter = 0;

signals:
    void timeUpdated(quint32 time);

private slots:
    void counting();
};

#endif // TIMER_H
