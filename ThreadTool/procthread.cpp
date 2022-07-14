#include "procthread.h"
#include "threaddatalist.h"
#include <QDebug>

#include "global.h"

ProcThread::ProcThread(QObject *parent) : QThread(parent)
{
    m_bStop = false;
    m_bPause = true;
}

void ProcThread::Start()
{
    m_bStop = false;
}

void ProcThread::Stop()
{
    m_bStop = true;
}

void ProcThread::Pause()
{
    m_bPause = true;
}

void ProcThread::Resume()
{
    m_bPause = false;
}

void ProcThread::run()
{
    while (!m_bStop)
    {
        if (m_bPause)
        {
           usleep(100);
           continue;
        }

        Task t;
        if (DATALIST->Pop(t))
        {
            emit SignalMsg(QString("处理完成：%1 %2")
                           .arg(qPrintable(t.timeAdd.toString("hh:mm:ss:zzz")))
                           .arg(QTime::currentTime().toString("hh:mm:ss:zzz")));
        }
    }

    qDebug() << __FUNCTION__ << __FILE__ << QThread::currentThreadId() << "end";
}
