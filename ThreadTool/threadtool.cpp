#include "threadtool.h"
#include "threaddatalist.h"
#include "procthread.h"
#include "workthread.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

ThreadTool::ThreadTool(QWidget *parent) : QWidget(parent)
{
    m_bStartProc = false;
    m_bStartWork = false;
    m_bNeedUpdate = false;

    DATALIST->Init();

    m_txtOut = new QTextEdit(this);
    m_btnStartProc = new QPushButton(this);
    m_btnStartWork = new QPushButton(this);
    m_btnStartWork->setText("开启工作线程");
    m_btnStartProc->setText("开启处理线程");

    QHBoxLayout *toolLayout = new QHBoxLayout();
    toolLayout->addWidget(m_btnStartWork);
    toolLayout->addWidget(m_btnStartProc);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(toolLayout);
    mainLayout->addWidget(m_txtOut);

    for (int i = 0; i < PROC_NUM; i++)
    {
        m_atProc[i] = new ProcThread();
        connect(m_atProc[i],SIGNAL(SignalMsg(QString)),this,SLOT(OnRecvMsg(QString)));
        m_atProc[i]->start();
    }

    for (int i = 0; i < WORK_NUM; i++)
    {
        m_atWork[i] = new WorkThread();
        connect(m_atWork[i],SIGNAL(SignalMsg(QString)),this,SLOT(OnRecvMsg(QString)));
        m_atWork[i]->start();
    }

    connect(m_btnStartWork, &QPushButton::clicked, [=]()
    {
        m_bStartWork = !m_bStartWork;

        if (m_bStartWork)
        {
            for (int i = 0; i < WORK_NUM; i++)
            {
                m_atWork[i]->Resume();
            }

            m_btnStartWork->setText("停止工作线程");
        }
        else
        {
            for (int i = 0; i < WORK_NUM; i++)
            {
                m_atWork[i]->Pause();
            }

            m_btnStartWork->setText("开启工作线程");
        }
    });

    connect(m_btnStartProc, &QPushButton::clicked, [=]()
    {
        m_bStartProc = !m_bStartProc;

        if (m_bStartProc)
        {
            for (int i = 0; i < PROC_NUM; i++)
            {
                m_atProc[i]->Resume();
            }

            m_btnStartProc->setText("停止处理线程");
        }
        else
        {
            for (int i = 0; i < PROC_NUM; i++)
            {
                m_atProc[i]->Pause();
            }

            m_btnStartProc->setText("开启处理线程");
        }
    });

    startTimer(100);

     m_txtOut->append("环形任务队列");
     m_txtOut->append("最大任务数80，达到最大任务数，工作线程进入等待，无法新增任务");
     m_txtOut->append("无任务时，处理线程进入等待，不空跑消耗cpu");
}

ThreadTool::~ThreadTool()
{
    // 优雅的退出环形数据链路，主动释放信号量
    DATALIST->Clear();

    for (int i = 0; i < PROC_NUM; i++)
    {
        m_atProc[i]->Stop();
        m_atProc[i]->quit();
        m_atProc[i]->wait();
    }

    for (int i = 0; i < WORK_NUM; i++)
    {
        m_atWork[i]->Stop();
        m_atWork[i]->quit();
        m_atWork[i]->wait();
    }
}

void ThreadTool::timerEvent(QTimerEvent *event)
{
    if (!m_bNeedUpdate)
        return;

    m_txtOut->clear();
    m_txtOut->append(m_strTxt);
    m_bNeedUpdate = false;
}

void ThreadTool::OnRecvMsg(QString msg)
{
    static int nCount = 0;

    if (++nCount > 1000)
    {
        nCount = 0;
        m_strTxt.clear();
    }
    m_strTxt.append(msg + "\r\n");
    m_bNeedUpdate = true;
}
