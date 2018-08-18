#include "tasksrepository.h"

TasksRepository::TasksRepository()
{
    tasks = parseTasksFromFile();
}

QJsonArray TasksRepository::getTasks() const
{
    return tasks;
}

void TasksRepository::add(QJsonObject obj)
{
    tasks.push_back(obj);
    writeTasksToFile();
}

bool TasksRepository::isContains(QString taskName)
{
    for(auto obj : tasks)
        if(obj.toObject()["task_name"] == taskName)
            return false;
    return true;
}

QJsonObject TasksRepository::getTaskByName(QString taskName)
{
    for(auto obj : tasks)
        if(obj.toObject()["task_name"] == taskName)
            return obj.toObject();
}

QJsonArray TasksRepository::parseTasksFromFile()
{
    QFile file(TASKS_FILENAME);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QJsonArray();

    const auto& data = QString(file.readAll());
    file.close();

    QRegularExpression regex("\\[|\\]");
    const auto& jsons = data.split(regex);


    return jsonArrayFromData(jsons);
}

void TasksRepository::writeTasksToFile()
{
    QJsonDocument doc;
    doc.setArray(tasks);
    QFile file(TASKS_FILENAME);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << doc.toJson();

    file.close();
}

QJsonArray TasksRepository::jsonArrayFromData(QStringList data)
{
    QJsonArray arr;

    for(const QString json : data)
        if(!json.trimmed().isEmpty()){
            const auto& formattedJson = QString("[%1]").arg(json);
            const auto& doc = QJsonDocument::fromJson(formattedJson.toUtf8());


            if(doc.isArray())
                for(const auto& item : doc.array()){
                    const auto& obj = item.toObject();
                    arr.push_back(obj);

                }
        }
    return arr;
}
