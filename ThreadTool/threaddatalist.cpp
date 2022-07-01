#include "threaddatalist.h"
#include <QSemaphore>
#include <QDebug>


QSemaphore freeBytes(80);   // 80是这个list的缓冲区大小，当然可以随意设定；80的意思是初始空闲的缓冲区大小是80
QSemaphore usedBytes(0);    // 初始被使用的缓冲区大小是0

ThreadDataList *ThreadDataList::m_self = nullptr;
void ThreadDataList::Init()
{
    m_self = new ThreadDataList();
}

ThreadDataList *ThreadDataList::Interface()
{
    return m_self;
}

bool ThreadDataList::Push(Task t)
{
    freeBytes.acquire();        // 先申请一个位置，相当于空闲的缓冲区变为79
    if (m_bClear)
    {
        freeBytes.release();
        return false;
    }

    QMutexLocker locker(&mutex);
    t.timeAdd = QTime::currentTime();
    m_taskList.append(t);
    usedBytes.release();        // 给使用的大小+1（因为这个是后加的，所以在提取的时候总是在它加完之后）

    return true;
}

bool ThreadDataList::Pop(Task &task)
{
    usedBytes.acquire();
    if (m_bClear)
    {
        usedBytes.release();
        return false;
    }

    QMutexLocker locker(&mutex);
    task = m_taskList.takeFirst();
    freeBytes.release();
    return true;
}

void ThreadDataList::Clear()
{
    m_bClear = true;
    usedBytes.release();
    freeBytes.release();
}

ThreadDataList::ThreadDataList(QObject *parent) : QObject(parent)
{
    m_bClear = false;
}
