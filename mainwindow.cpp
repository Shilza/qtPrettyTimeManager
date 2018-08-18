#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusBar->setVisible(false);
    ui->mainToolBar->setVisible(false);

    this->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    this->setStyleSheet("background: white;");
    this->setWindowTitle("Time manager");
    this->setWindowFlags(Qt::WindowStaysOnTopHint);

    mainWidget = new QWidget(this);
    mainLayout = new QGridLayout(mainWidget);
    mainLayout->setContentsMargins(10, 10, 10, 15);

    this->setCentralWidget(mainWidget);
    mainWidget->setLayout(mainLayout);

    buttonLoop = new QPushButton(mainWidget);
    buttonLoop->setStyleSheet("background: white; border: 0;");
    buttonLoop->setFixedSize(AUXILIARY_BUTTON_SIZE, AUXILIARY_BUTTON_SIZE);
    buttonLoop->setIcon(QIcon(":/icons/icons/loop.svg"));
    buttonLoop->setIconSize(buttonLoop->size());
    buttonLoop->setToolTip("Loop");
    buttonLoop->setCursor(Qt::PointingHandCursor);

    labelDisplayTime = new QPushButton(mainWidget);

    QFont font = labelDisplayTime->font();
    font.setPointSize(20);

    labelDisplayTime->setStyleSheet("background: white; color: #535050; border: 0");
    labelDisplayTime->setText("0");
    labelDisplayTime->setCursor(Qt::PointingHandCursor);
    labelDisplayTime->setToolTip("Reset");
    labelDisplayTime->setFont(font);

    buttonStart = new QPushButton(this);
    buttonStart->setStyleSheet("background: white;"
                               "border: 1px solid #A1A1A1;"
                               "border-radius: 5%;"
                               "padding: 3px 12px 3px 12px;");
    buttonStart->setText("Start");
    buttonStart->setCursor(Qt::PointingHandCursor);


    buttonCreateNewTask = new QPushButton(mainWidget);
    buttonCreateNewTask->setStyleSheet("background: transparent;");
    buttonCreateNewTask->setToolTip("Create new task");
    buttonCreateNewTask->setFixedSize(AUXILIARY_BUTTON_SIZE, AUXILIARY_BUTTON_SIZE);
    buttonCreateNewTask->setIcon(QIcon(":/icons/icons/add.svg"));
    buttonCreateNewTask->setIconSize(buttonCreateNewTask->size());
    buttonCreateNewTask->setCursor(Qt::PointingHandCursor);

    buttonSound = new QPushButton(mainWidget);
    buttonSound->setStyleSheet("background: transparent");
    buttonSound->setFixedSize(AUXILIARY_BUTTON_SIZE, AUXILIARY_BUTTON_SIZE);
    buttonSound->setIcon(QIcon(":icons/icons/volume.svg"));
    buttonSound->setIconSize(buttonSound->size());
    buttonSound->setToolTip("Sound");
    buttonSound->setCursor(Qt::PointingHandCursor);

    spinsContainer = new SpinsContainer(mainWidget);

    tasksBox = new QComboBox(this);
    tasksBox->setMaximumWidth(60);
    tasksBox->setStyleSheet("QComboBox {"
                            " border: 0;"
                            "border-bottom: 1px solid #E8E8E8;"
                            "background: white;"
                            "color: #333;"
                            "padding-bottom: 3px;"
                            "}"

                            "QComboBox::drop-down {"
                            "border: 0;"
                            "margin-left: 2px;"
                            "padding-bottom: 3px;"
                            "}"

                            "QComboBox::down-arrow {"
                            "border: 1px solid #D3D3D3;"
                            "border-radius: 6px;"
                            " }");

    mainLayout->addWidget(tasksBox, 0, 0, 1, 4, Qt::AlignLeft);
    mainLayout->addWidget(buttonSound, 0, 7, 1, 1, Qt::AlignRight);
    mainLayout->addWidget(labelDisplayTime, 1, 0, 3, 8, Qt::AlignCenter);
    mainLayout->addWidget(buttonLoop, 2, 7, 1, 1, Qt::AlignRight);
    mainLayout->addWidget(buttonStart, 5, 3, 1, 2, Qt::AlignRight);
    mainLayout->addWidget(buttonCreateNewTask, 5, 7, 1, 1, Qt::AlignRight);
    mainLayout->addItem(spinsContainer->getSpinsLayout(), 6, 1, 1, 6);

    timer = new Timer;

    tasksWindow = new TasksWindow;

    soundNotification = new QMediaPlayer;
    soundNotification->setVolume(10);

    setupTasksList();

    connect(buttonStart, SIGNAL(released()), SLOT(startProcess()));
    connect(timer, SIGNAL(timeUpdated(quint32)), SLOT(updateState(quint32)));
    connect(buttonCreateNewTask, SIGNAL(released()), tasksWindow, SLOT(show()));
    connect(buttonSound, SIGNAL(released()), SLOT(changeSoundIcon()));
    connect(buttonLoop, SIGNAL(released()), SLOT(changeIsLooped()));
    connect(labelDisplayTime, SIGNAL(released()), SLOT(reset()));
    connect(spinsContainer, SIGNAL(timeUpdated(QString)), SLOT(changeDisplayTime(QString)));
    connect(tasksBox, SIGNAL(activated(QString)), SLOT(setCurrentTask(QString)));
    connect(tasksWindow, SIGNAL(tasksUpdated()), SLOT(setupTasksList()));
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    click_x_coord = event->x();
    click_y_coord = event->y();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalX() - click_x_coord, event->globalY() - click_y_coord);
}

void MainWindow::updateState(quint32 time){
    if(time > TIMER_INTERVAL)
        displayTime(time);
    else{
        timer->setTimerMillsCounter(0);

        if(currentTask != QJsonObject() && currentTask["break_time"].toBool() && !breakIsStarted){
            breakIsStarted = true;
            timer->setTimerMilliseconds(currentTaskBreakTime());
            soundNotification->setMedia(QUrl("qrc:/sounds/sounds/notification.mp3"));
            labelDisplayTime->setStyleSheet("background: white; color: green; border: 0");
        }
        else if(isLooped){
            breakIsStarted = false;
            labelDisplayTime->setStyleSheet("background: white; color: #535050; border: 0");
            setRunningTime();
            timer->start();
        }
        else{
            stopTimer();
            isStarted = false;
            breakIsStarted = false;
            labelDisplayTime->setText("0");
            soundNotification->setMedia(QUrl("qrc:/sounds/sounds/notification3.mp3"));
            addSpinsLayout();
        }
        if(isSoundOn)
            soundNotification->play();
    }
}

void MainWindow::displayTime(quint32 time){
    quint8 minutes = time / 60000;
    quint8 seconds = (time - minutes * 60000) / 1000;
    quint8 mills = (time - minutes * 60000 - seconds * 1000) / 100;

    labelDisplayTime->setText(
                QString::number(minutes) + ':' + QString::number(seconds) + ':' + QString::number(mills)
                );
}

void MainWindow::removeSpinsLayout()
{
    mainLayout->removeItem(spinsContainer->getSpinsLayout());
    this->setFixedSize(WINDOW_WIDTH, WINDOW_RESIZE_HEIGHT);
}

void MainWindow::addSpinsLayout()
{
    mainLayout->addItem(spinsContainer->getSpinsLayout(), 6, 0, 1, 8);
    this->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void MainWindow::setupTasksList()
{
    for(auto obj : TasksRepository::instance().getTasks())
        tasksBox->addItem(obj.toObject()["task_name"].toString());
    tasksBox->setVisible(tasksBox->count() == 0 ? false : true);
}

void MainWindow::changeIsLooped()
{
    isLooped = !isLooped;
    buttonLoop->setStyleSheet(isLooped ? "background: #E8E8E8" : "background: transparent");
}

void MainWindow::changeSoundIcon()
{
    isSoundOn = !isSoundOn;
    buttonSound->setIcon(
                QIcon(isSoundOn ? ":icons/icons/volume.svg" : ":icons/icons/volume_off.svg")
                );
}

void MainWindow::startProcess()
{
    isStarted = !isStarted;

    if(isStarted && (spinsContainer->isSpinsValid() || currentTask != QJsonObject())){
        setRunningTime();

        buttonStart->setText("Stop");
        timer->start();

        if(isSoundOn){
            soundNotification->setMedia(QUrl("qrc:/sounds/sounds/notification.mp3"));
            soundNotification->play();
        }

        removeSpinsLayout();
    } else{
        stopTimer();
    }
}

void MainWindow::stopTimer(){
    timer->stop();
    buttonStart->setText("Start");
    labelDisplayTime->setStyleSheet("background: white; color: #535050; border: 0");
}

void MainWindow::reset()
{
    isStarted = false;

    breakIsStarted = false;
    labelDisplayTime->setText("0");
    timer->setTimerMillsCounter(0);
    stopTimer();
    addSpinsLayout();
}

void MainWindow::setCurrentTask(QString taskName)
{
    currentTask = TasksRepository::instance().getTaskByName(taskName);
    timer->setTimerMillsCounter(0);
    displayTime(currentTaskRunnngTime());
}

quint32 MainWindow::currentTaskRunnngTime(){
    return (currentTask["rt_min"].toInt()*60000 +
            currentTask["rt_sec"].toInt()*1000 +
            currentTask["rt_millsec"].toInt());
}

quint32 MainWindow::currentTaskBreakTime(){
    return (currentTask["bt_min"].toInt()*60000 +
            currentTask["bt_sec"].toInt()*1000 +
            currentTask["bt_millsec"].toInt());
}

void MainWindow::setRunningTime()
{
    if(currentTask != QJsonObject())
        timer->setTimerMilliseconds(currentTaskRunnngTime());
    else
        timer->setTimerMilliseconds(spinsContainer->getMinutes() +
                                    spinsContainer->getSeconds() +
                                    spinsContainer->getMillSec());
}

void MainWindow::changeDisplayTime(QString timeString)
{
    labelDisplayTime->setText(timeString);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete buttonStart;
    delete mainLayout;
    delete labelDisplayTime;
    delete mainWidget;

    delete buttonCreateNewTask;
    delete buttonSound;
    delete tasksWindow;
    delete buttonLoop;

    delete spinsContainer;
    delete timer;

    delete tasksBox;

    delete soundNotification;
}
