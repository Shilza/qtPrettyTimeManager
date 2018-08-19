#ifndef SPINSCONTAINER_H
#define SPINSCONTAINER_H

#include <QSpinBox>
#include <QLabel>
#include <QHBoxLayout>
#include "def.h"

class SpinsContainer : public QObject
{
    Q_OBJECT
public:
    SpinsContainer(QWidget *parent);
    ~SpinsContainer();

    QHBoxLayout *getSpinsLayout() const;
    bool isSpinsValid();

    quint32 getMinutes();
    quint32 getSeconds();
    quint32 getMillSec();

signals:
    void timeUpdated(QString time);

private:
    QHBoxLayout *spinsLayout;

    QSpinBox *spinMinutes;
    QSpinBox *spinSeconds;
    QSpinBox *spinMillSec;

    QLabel *labelMinutes;
    QLabel *labelSeconds;
    QLabel *labelMillSec;

private slots:
    void compileDisplayTimeString();

};

#endif // SPINSCONTAINER_H
