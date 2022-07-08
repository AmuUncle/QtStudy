#include "loadingdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>


CLoadingDialog *CLoadingDialog::m_pSelf = NULL;
CLoadingDialog::CLoadingDialog(QWidget *parent) : QDialog(parent)
{
    m_pMovie = NULL;
    m_labelWait = NULL;
    m_labelTip = NULL;
    m_pPane = NULL;

    if (NULL == m_pPane)
        m_pPane = new QWidget(this);
    if (NULL == m_pMovie)
        m_pMovie = new QMovie(":/loadingdialog/loading.gif");
    if (NULL == m_labelWait)
        m_labelWait = new QLabel(this);
    if (NULL == m_labelTip)
        m_labelTip = new QLabel(tr("请稍后..."), this);

    setWindowFlags(windowFlags() | (Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint));

    m_labelWait->setScaledContents(true);
    m_labelWait->setMovie(m_pMovie);

    m_labelTip->setAlignment(Qt::AlignCenter);

    QHBoxLayout *hLayoutMovie = new QHBoxLayout();
    hLayoutMovie->addStretch();
    hLayoutMovie->addWidget(m_labelWait);
    hLayoutMovie->addStretch();

    QVBoxLayout *layoutPane = new QVBoxLayout(m_pPane);
    layoutPane->addStretch();
    layoutPane->addLayout(hLayoutMovie);
    layoutPane->addWidget(m_labelTip);
    layoutPane->addStretch();
    layoutPane->setMargin(0);
    layoutPane->setSpacing(0);

    QVBoxLayout *layoutMain = new QVBoxLayout();
    layoutMain->addWidget(m_pPane);
    layoutMain->setMargin(10);
    layoutMain->setSpacing(0);
    setLayout(layoutMain);

    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet(".QWidget {background: #66ceff; border-radius:5px; }");
    setFixedSize(250, 200);

    Stop();
}

CLoadingDialog *CLoadingDialog::GetInstance()
{
    return m_pSelf;
}

void CLoadingDialog::ExitInstance()
{
    if (m_pSelf)
    {
        m_pSelf->deleteLater();
        m_pSelf = NULL;
    }
}

void CLoadingDialog::Init( QWidget *parent )
{
    if (NULL == m_pSelf)
    {
        m_pSelf = new CLoadingDialog(parent);
        m_pSelf->setModal(true);
    }
}

void CLoadingDialog::Start(uint dwMs/* = 0*/)
{
    m_pMovie->start();
    show();

    QWidget *pParent = (QWidget *)parent();
    if (pParent)
    {
        QPoint pt = pParent->mapToGlobal(QPoint(0, 0));
        QRect deskRect = pParent->geometry();
        move(pt.x() + (deskRect.width() - width()) / 2,\
             pt.y() + (deskRect.height() - height()) / 2);
    }

    if (dwMs > 0)
    {
        QTimer::singleShot(dwMs, this, &CLoadingDialog::Stop);
    }
}

void CLoadingDialog::Stop()
{
    m_pMovie->stop();
    hide();
}

void CLoadingDialog::SetTip( QString strTip )
{
    m_labelTip->setText(strTip);
}
