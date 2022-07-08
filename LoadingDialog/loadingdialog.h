#ifndef LOADINGDIALOG_H
#define LOADINGDIALOG_H

#include <QDialog>
#include <QMovie>
#include <QLabel>

class CLoadingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CLoadingDialog(QWidget *parent = nullptr);

public:
    static CLoadingDialog* GetInstance();
    static void ExitInstance();
    static void Init(QWidget *parent);

public slots:
    void Start(uint dwMs = 0);
    void Stop();
    void SetTip(QString strTip);

private:
    static CLoadingDialog *m_pSelf;

    QWidget *m_pPane;
    QMovie *m_pMovie;
    QLabel *m_labelWait;
    QLabel *m_labelTip;
};

#define LOADINGDLG CLoadingDialog::GetInstance()

#endif // LOADINGDIALOG_H
