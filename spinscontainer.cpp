#include "spinscontainer.h"

SpinsContainer::SpinsContainer(QWidget *parent)
{
    spinMinutes = new QSpinBox(parent);
    spinMinutes->setStyleSheet("padding: 2px; border: 0; border-bottom: 1px solid black;");

    spinSeconds = new QSpinBox(parent);
    spinSeconds->setStyleSheet("padding: 2px; border: 0; border-bottom: 1px solid black;");

    spinMillSec = new QSpinBox(parent);
    spinMillSec->setStyleSheet("padding: 2px; border: 0; border-bottom: 1px solid black;");

    spinMillSec->setToolTip("Milliseconds");
    spinMinutes->setToolTip("Minutes");
    spinSeconds->setToolTip("Seconds");

    spinMinutes->setMaximum(MAX_COUNT_MINUTES_TIME);
    spinSeconds->setMaximum(MAX_COUNT_SECONDS_TIME);
    spinMillSec->setMaximum(MAX_COUNT_MILLSEC_TIME);

    QHBoxLayout *layoutMinutes = new QHBoxLayout(parent);
    layoutMinutes->addWidget(new QLabel("m", parent));
    layoutMinutes->addWidget(spinMinutes);
    layoutMinutes->setSpacing(4);

    QHBoxLayout *layoutSeconds = new QHBoxLayout(parent);
    layoutSeconds->addWidget(new QLabel("s", parent));
    layoutSeconds->addWidget(spinSeconds);
    layoutSeconds->setSpacing(4);

    QHBoxLayout *layoutMillSec = new QHBoxLayout(parent);
    layoutMillSec->addWidget(new QLabel("ms", parent));
    layoutMillSec->addWidget(spinMillSec);
    layoutMillSec->setSpacing(4);

    spinsLayout = new QHBoxLayout(parent);
    spinsLayout->setContentsMargins(4, 12, 4, 4);
    spinsLayout->addItem(layoutMinutes);
    spinsLayout->addItem(layoutSeconds);
    spinsLayout->addItem(layoutMillSec);
    spinsLayout->setSpacing(16);

    connect(spinMinutes, SIGNAL(valueChanged(int)), SLOT(compileDisplayTimeString()));
    connect(spinSeconds, SIGNAL(valueChanged(int)), SLOT(compileDisplayTimeString()));
    connect(spinMillSec, SIGNAL(valueChanged(int)), SLOT(compileDisplayTimeString()));
}

QHBoxLayout *SpinsContainer::getSpinsLayout() const
{
    return spinsLayout;
}

bool SpinsContainer::isSpinsValid()
{
    return (spinMinutes->value() > 0 ||
            spinSeconds->value() > 0 ||
            spinMillSec->value() > 0);
}

quint32 SpinsContainer::getMinutes()
{
    return spinMinutes->value() * 1000 * 60;
}

quint32 SpinsContainer::getSeconds()
{
    return spinSeconds->value() * 1000;
}

quint32 SpinsContainer::getMillSec()
{
    return spinMillSec->value();
}

void SpinsContainer::compileDisplayTimeString()
{
    emit timeUpdated(QString::number(spinMinutes->value()) + '.' +
                     QString::number(spinSeconds->value()) + '.' +
                     QString::number(spinMillSec->value()/10));
}

SpinsContainer::~SpinsContainer()
{
    delete spinsLayout;

    delete spinMinutes;
    delete spinSeconds;
    delete spinMillSec;

    delete labelMinutes;
    delete labelSeconds;
    delete labelMillSec;
}
