#include "taskswindow.h"

TasksWindow::TasksWindow()
{
    this->setFixedSize(QSize(250, 200));
    this->setStyleSheet("background: white");
    this->setWindowFlags(Qt::WindowStaysOnTopHint);

    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->setContentsMargins(10, 20, 10, 10);
    this->setLayout(mainLayout);

    lineTaskName = new QLineEdit(this);
    spinRunningMinutes = new QSpinBox(this);
    spinRunningSeconds = new QSpinBox(this);
    spinRunningMillSec = new QSpinBox(this);

    spinBreakMinutes = new QSpinBox(this);
    spinBreakSeconds = new QSpinBox(this);
    spinBreakMillSec = new QSpinBox(this);

    labelTaskName = new QLabel(this);
    labelStart = new QLabel(this);

    buttonSubmit = new QPushButton(this);
    buttonCancel = new QPushButton(this);

    checkEndSpin = new QCheckBox("Break time", this);

    labelError = new QLabel(this);
    labelError->setStyleSheet("background: white; color: red;");
    labelError->setAlignment(Qt::AlignCenter);
    labelError->setVisible(false);

    QFont font = labelError->font();
    font.setPointSize(10);

    labelError->setFont(font);

    buttonSubmit->setText("Ok");
    buttonCancel->setText("Cancel");

    buttonSubmit->setStyleSheet("background: white; border: 0;");
    buttonSubmit->setCursor(Qt::PointingHandCursor);

    buttonCancel->setStyleSheet("QPushButton{"
                                "background: transparent;"
                                "color: blue;"
                                "}"
                                "QPushButton:hover{"
                                "text-decoration: underline;"
                                "}");
    buttonCancel->setCursor(Qt::PointingHandCursor);

    labelTaskName->setText("Task name");
    labelStart->setText("Running time");

    spinRunningMinutes->setMaximum(MAX_COUNT_MINUTES_TIME);
    spinRunningSeconds->setMaximum(MAX_COUNT_SECONDS_TIME);
    spinRunningMillSec->setMaximum(MAX_COUNT_MILLSEC_TIME);

    spinBreakMinutes->setMaximum(MAX_COUNT_MINUTES_TIME);
    spinBreakSeconds->setMaximum(MAX_COUNT_SECONDS_TIME);
    spinBreakMillSec->setMaximum(MAX_COUNT_MILLSEC_TIME);

    spinBreakMinutes->setEnabled(false);
    spinBreakSeconds->setEnabled(false);
    spinBreakMillSec->setEnabled(false);

    spinRunningMinutes->setMaximumWidth(45);
    spinRunningSeconds->setMaximumWidth(45);
    spinRunningMillSec->setMaximumWidth(45);

    spinBreakMinutes->setMaximumWidth(45);
    spinBreakSeconds->setMaximumWidth(45);
    spinBreakMillSec->setMaximumWidth(45);

    spinRunningMinutes->setToolTip("Minutes");
    spinRunningSeconds->setToolTip("Seconds");
    spinRunningMillSec->setToolTip("Milliseconds");

    spinBreakMinutes->setToolTip("Minutes");
    spinBreakSeconds->setToolTip("Seconds");
    spinBreakMillSec->setToolTip("Milliseconds");

    QVBoxLayout *first = new QVBoxLayout(this);
    first->setSpacing(4);
    first->addWidget(labelTaskName);
    first->addWidget(lineTaskName);

    QHBoxLayout *second = new QHBoxLayout(this);
    second->addWidget(labelStart);
    second->setSpacing(6);
    second->addWidget(spinRunningMinutes);
    second->addWidget(spinRunningSeconds);
    second->addWidget(spinRunningMillSec);

    QHBoxLayout *third = new QHBoxLayout(this);
    third->addWidget(checkEndSpin);
    third->setSpacing(6);
    third->addWidget(spinBreakMinutes);
    third->addWidget(spinBreakSeconds);
    third->addWidget(spinBreakMillSec);

    QHBoxLayout *fourth = new QHBoxLayout(this);
    fourth->addWidget(buttonSubmit);
    fourth->addWidget(buttonCancel);
    fourth->setContentsMargins(0, 30, 0, 0);

    mainLayout->addItem(first);
    mainLayout->addItem(second);
    mainLayout->addItem(third);
    mainLayout->addItem(fourth);
    mainLayout->addWidget(labelError);

    connect(buttonSubmit, SIGNAL(released()), SLOT(submit()));
    connect(buttonCancel, SIGNAL(released()), SLOT(close()));
    connect(checkEndSpin, SIGNAL(released()), SLOT(enableEndSpin()));
    connect(lineTaskName, SIGNAL(textChanged(QString)), SLOT(setLabelErrorInvisible()));
    connect(spinRunningMillSec, SIGNAL(valueChanged(int)), SLOT(setLabelErrorInvisible()));
    connect(spinBreakMinutes, SIGNAL(valueChanged(int)), SLOT(setLabelErrorInvisible()));
}

bool TasksWindow::isValidTaskData()
{
    if(lineTaskName->text() == ""){
        setErrorMessage("Error: task must have name");
        return false;
    }

    if(!TasksRepository::instance().isContains(lineTaskName->text())){
        setErrorMessage("Error: task name already exists");
        return false;
    }

    if(
            (spinRunningMinutes->value() <= 0 &&
             spinRunningSeconds->value() <= 0 &&
             spinRunningMillSec->value() <= 0)
            ||
            (checkEndSpin->checkState() && (
                 spinBreakMinutes->value() <= 0 &&
                 spinBreakSeconds->value() <= 0 &&
                 spinBreakMillSec->value() <= 0)
             )
            ){

        setErrorMessage("Error: time must be greater then 0");
        return false;
    }

    return true;
}

void TasksWindow::setErrorMessage(QString message)
{
    labelError->setText(message);
    labelError->setVisible(true);
}

void TasksWindow::clearAllInputs()
{
    lineTaskName->setText("");
    spinRunningMinutes->setValue(0);
    spinRunningSeconds->setValue(0);
    spinRunningMillSec->setValue(0);
    spinBreakMinutes->setValue(0);
    spinBreakSeconds->setValue(0);
    spinBreakMillSec->setValue(0);
    checkEndSpin->setChecked(false);
}

QJsonObject TasksWindow::createNewTask()
{
    QJsonObject itemData;

    itemData.insert("task_name", QJsonValue(lineTaskName->text()));
    itemData.insert("rt_min", QJsonValue(spinRunningMinutes->value()));
    itemData.insert("rt_sec", QJsonValue(spinRunningSeconds->value()));
    itemData.insert("rt_millsec", QJsonValue(spinRunningMillSec->value()));
    itemData.insert("break_time", QJsonValue(checkEndSpin->isChecked()));
    itemData.insert("bt_min", QJsonValue(spinBreakMinutes->value()));
    itemData.insert("bt_sec", QJsonValue(spinBreakSeconds->value()));
    itemData.insert("bt_millsec", QJsonValue(spinBreakMillSec->value()));

    return itemData;
}

void TasksWindow::submit()
{
    if(isValidTaskData()){
        TasksRepository::instance().add(createNewTask());
        clearAllInputs();
        emit tasksUpdated();
        close();
    }
}

void TasksWindow::enableEndSpin()
{
    spinBreakMinutes->setEnabled(checkEndSpin->isChecked() ? true : false);
    spinBreakSeconds->setEnabled(checkEndSpin->isChecked() ? true : false);
    spinBreakMillSec->setEnabled(checkEndSpin->isChecked() ? true : false);
}

void TasksWindow::setLabelErrorInvisible()
{
    labelError->setVisible(false);
}

TasksWindow::~TasksWindow()
{
    delete mainLayout;

    delete lineTaskName;

    delete spinRunningMinutes;
    delete spinRunningSeconds;
    delete spinRunningMillSec;

    delete spinBreakMinutes;
    delete spinBreakSeconds;
    delete spinBreakMillSec;

    delete labelTaskName;
    delete labelStart;

    delete buttonSubmit;
    delete buttonCancel;

    delete checkEndSpin;
    delete labelError;
}
