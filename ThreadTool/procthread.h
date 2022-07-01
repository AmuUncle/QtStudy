#ifndef PROCTHREAD_H
#define PROCTHREAD_H

#include <QThread>
#include <QMutex>

class ProcThread : public QThread
{
    Q_OBJECT
public:
    explicit ProcThread(QObject *parent = nullptr);

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

#endif // PROCTHREAD_H
