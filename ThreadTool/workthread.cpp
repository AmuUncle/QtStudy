#include "workthread.h"
#include "threaddatalist.h"
#include <QDebug>

WorkThread::WorkThread(QObject *parent) : QThread(parent)
{
   m_bStop = false;
   m_bPause = true;
}

void WorkThread::Start()
{
    m_bStop = false;
}

void WorkThread::Stop()
{
    m_bStop = true;
}

void WorkThread::Pause()
{
    m_bPause = true;
}

void WorkThread::Resume()
{
    m_bPause = false;
}

void WorkThread::run()
{
    while (!m_bStop)
    {
        if (m_bPause)
        {
           usleep(100);
           continue;
        }

        Task t;
        if (DATALIST->Push(t))
        {
            emit SignalMsg(QString("开始处理：%1")
                           .arg(QTime::currentTime().toString("hh:mm:ss:zzz")));
            usleep(10);
        }
    }

    qDebug() << __FUNCTION__ << __FILE__ << QThread::currentThreadId() << "end";
}
