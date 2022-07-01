#ifndef THREADTOOL_H
#define THREADTOOL_H

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>

const int PROC_NUM = 100;
const int WORK_NUM = 100;

class ProcThread;
class WorkThread;
class ThreadTool : public QWidget
{
    Q_OBJECT
public:
    explicit ThreadTool(QWidget *parent = nullptr);
    ~ThreadTool();

private:
    void timerEvent(QTimerEvent *event);

private slots:
    void OnRecvMsg(QString msg);

private:
    ProcThread *m_atProc[PROC_NUM];
    WorkThread *m_atWork[WORK_NUM];

    bool m_bStartProc;
    bool m_bStartWork;

    QTextEdit *m_txtOut;
    QPushButton *m_btnStartProc;
    QPushButton *m_btnStartWork;

    QString m_strTxt;
    bool m_bNeedUpdate;
};

#endif // THREADTOOL_H
