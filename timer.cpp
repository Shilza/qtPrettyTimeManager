#include "timer.h"

Timer::Timer()
{
    timer = new QTimer;
    timer->setInterval(TIMER_INTERVAL);
    connect(timer, SIGNAL(timeout()), SLOT(counting()));
}

void Timer::stop()
{
    timer->stop();
}

void Timer::start()
{
    timer->start();
}

void Timer::setTimerMillsCounter(quint32 value)
{
    timerMillisecondsCounter = value;
}

void Timer::setTimerMilliseconds(quint32 value)
{
    timerMilliseconds = value;
}


void Timer::counting()
{
    timerMillisecondsCounter += TIMER_INTERVAL;

    quint32 time = timerMilliseconds - timerMillisecondsCounter;

    emit timeUpdated(time);
}
