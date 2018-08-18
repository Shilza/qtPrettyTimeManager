#ifndef CREATETASKDIALOG_H
#define CREATETASKDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QWindow>
#include <QGridLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QTextStream>
#include <QDebug>
#include <QCheckBox>
#include "def.h"
#include "tasksrepository.h"

class TasksWindow : public QWidget
{
    Q_OBJECT
public:
    TasksWindow();
private:
    QVBoxLayout *mainLayout;

    QLineEdit *lineTaskName;

    QSpinBox *spinRunningMinutes;
    QSpinBox *spinRunningSeconds;
    QSpinBox *spinRunningMillSec;

    QSpinBox *spinBreakMinutes;
    QSpinBox *spinBreakSeconds;
    QSpinBox *spinBreakMillSec;

    QLabel *labelTaskName;
    QLabel *labelStart;

    QPushButton *buttonSubmit;
    QPushButton *buttonCancel;

    QCheckBox *checkEndSpin;
    QLabel *labelError;

    bool isValidTaskData();
    bool isTaskDataUnique();

    void setErrorMessage(QString message);
    void clearAllInputs();
    QJsonObject createNewTask();

signals:
    void tasksUpdated();

private slots:
    void submit();
    void enableEndSpin();
    void setLabelErrorInvisible();
};

#endif // CREATETASKDIALOG_H
