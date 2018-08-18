#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include "QPushButton"
#include <QGridLayout>
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QSpinBox>
#include <QHBoxLayout>
#include <taskswindow.h>
#include <QMediaPlayer>
#include <QComboBox>
#include "spinscontainer.h"
#include "timer.h"
#include "tasksrepository.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QPushButton *buttonStart;
    QGridLayout *mainLayout;
    QPushButton *labelDisplayTime;
    QWidget *mainWidget;

    QPushButton *buttonCreateNewTask;
    QPushButton *buttonSound;
    TasksWindow *tasksWindow;
    QPushButton *buttonLoop;

    SpinsContainer *spinsContainer;
    Timer *timer;

    QComboBox *tasksBox;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void displayTime(quint32 time);
    void removeSpinsLayout();
    void addSpinsLayout();

    int click_x_coord;
    int click_y_coord;

    bool isSoundOn = true;
    bool isStarted = false;
    bool isLooped = false;

    QMediaPlayer *soundNotification;
    QJsonObject currentTask;

    void stopTimer();
    quint32 currentTaskRunnngTime();
    quint32 currentTaskBreakTime();
    bool breakIsStarted = false;
    void setRunningTime();

private slots:
    void updateState(quint32 time);
    void changeSoundIcon();
    void startProcess();
    void reset();
    void setCurrentTask(QString taskName);
    void setupTasksList();
    void changeIsLooped();

public slots:
    void changeDisplayTime(QString timeString);
};

#endif // MAINWINDOW_H
