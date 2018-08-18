#ifndef UTIL_H
#define UTIL_H

#include <QJsonArray>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QRegularExpression>
#include <QTextStream>
#include "def.h"

class TasksRepository
{
public:
    static TasksRepository &instance(){
        static TasksRepository repo;
        return repo;
    }

    QJsonArray getTasks() const;
    void add(QJsonObject obj);
    bool isContains(QString taskName);
    QJsonObject getTaskByName(QString taskName);

private:
    QJsonArray tasks;
    TasksRepository();
    ~TasksRepository(){}

    TasksRepository(TasksRepository const&) = delete;
    TasksRepository& operator= (TasksRepository const&) = delete;

    QJsonArray parseTasksFromFile();
    void writeTasksToFile();
    QJsonArray jsonArrayFromData(QStringList data);
};


#endif // UTIL_H
