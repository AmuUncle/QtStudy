#include "rolltitles.h"
#include <QPainter>
#include <QTimer>
#include <QFontMetrics>
#include <QTimerEvent>

const QString strSpace("      ");

RollTitles::RollTitles(QWidget *parent, QString strTitle) : QWidget(parent)
{
    m_nLeftOffset = 0;
    m_nTimerId = -1;
    strTitle = strTitle;

    setMinimumHeight(24);
}

void RollTitles::SetTitle(QString strTitle)
{
    m_strTitle = strTitle;
    UpdateRollingState();
}

void RollTitles::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);         // 创建画家对象
    QRect rcClient = rect();

    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 0));
    painter.drawRect(rcClient);
    painter.restore();

    //  如果文字已经显示到末尾，则再添加一遍文字，做出循环滚动的效果
    QString strText = m_strTitle;
    if(m_nTimerId >= 0)
        strText += strSpace + m_strTitle;

    painter.save();
    rcClient.setLeft(rcClient.left() - m_nLeftOffset);
    painter.drawText(rcClient, Qt::AlignVCenter | Qt::AlignLeft, strText);
    painter.restore();
}

void RollTitles::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_nTimerId && isVisible())
    {
        //  每次左移1个像素
        m_nLeftOffset += 1;

        //  判断是否已经完成一遍循环，完成则恢复起始位置，重新开始循环
        QFont ft = font();
        QFontMetrics fm(ft);
        int txtWidth = fm.width(m_strTitle);
        int spaceWidth = fm.width(strSpace);

        if((txtWidth + spaceWidth) < m_nLeftOffset)
            m_nLeftOffset = 0;

        update();
    }

    QWidget::timerEvent(event);
}

void RollTitles::resizeEvent(QResizeEvent *event)
{
    UpdateRollingState();
    QWidget::resizeEvent(event);
}

void RollTitles::showEvent(QShowEvent *event)
{
    UpdateRollingState();
}

void RollTitles::hideEvent(QHideEvent *event)
{
    if(m_nTimerId >= 0)
    {
        killTimer(m_nTimerId);
        m_nTimerId = -1;
    }
}

void RollTitles::UpdateRollingState()
{
    //  获取文本大小，小于文本框长度，则无需滚动
    QFont ft = font();
    QFontMetrics fm(ft);
    int nW = fm.width(m_strTitle);

    m_nLeftOffset = 0;
    //  开启文本框滚动
    if(nW > width())
    {
        m_nTimerId = startTimer(100);
    }
    //  关闭文本框滚动
    else
    {
        if(m_nTimerId >= 0)
        {
            killTimer(m_nTimerId);
            m_nTimerId = -1;
        }
    }

    update();
}
