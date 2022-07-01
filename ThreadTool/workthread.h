#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QThread>
#include <QMutex>

class WorkThread : public QThread
{
    Q_OBJECT
public:
    explicit WorkThread(QObject *parent = nullptr);

    void Start();
    void Stop();
    void Pause();
    void Resume();

protected:
    void run();

signals:
    void SignalMsg(QString txt);

private:
    bool m_bPause;
    bool m_bStop;
};

#endif // WORKTHREAD_H
