#ifndef THREADDATALIST_H
#define THREADDATALIST_H

#include <QTime>
#include <QObject>
#include <QMutex>

struct Task
{
    uint nId;
    QTime timeAdd;
    QTime timeProc;
    QString strContent;
};

typedef QList<Task> TaskList;

class ThreadDataList : public QObject
{
    Q_OBJECT
public:
    static void Init();
    static ThreadDataList *Interface();

    bool Push(Task t);
    bool Pop(Task &task);

    void Clear();

private:
    explicit ThreadDataList(QObject *parent = nullptr);

private:

    static ThreadDataList *m_self;

    bool m_bClear;

    QMutex mutex;
    TaskList m_taskList;
};
#define DATALIST ThreadDataList::Interface()
#endif // THREADDATALIST_H
