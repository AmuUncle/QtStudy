#include "epushbutton.h"

#include <QPainter>
#include <QTimerEvent>
#include <QDebug>

EPushButton::EPushButton(QWidget *parent) : QWidget(parent)
{
    m_nRotate = 0;

    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);

    m_nTimeId = startTimer(5);
}

QSize EPushButton::sizeHint() const
{
    return QSize(200, 100);
}

void EPushButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect rcClient = rect();

    QPainterPath path;
    path.addRoundedRect(rcClient, 7, 7);
    painter.setClipPath(path);

//    painter.save();
//    painter.setPen(Qt::NoPen);
//    painter.setBrush(QColor("#0000"));
//    painter.drawRoundedRect(rcClient, 7, 7);
//    painter.restore();


    painter.save();
    painter.translate(rcClient.center());
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#f00"));

    painter.rotate(m_nRotate);
    QRect rcClient1 = rcClient;
    rcClient1.setWidth(rcClient.width());
    rcClient1.setHeight(rcClient.width());
    painter.drawRect(rcClient1);
    painter.restore();


    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#000"));
    painter.drawRoundedRect(rcClient.marginsRemoved(QMargins(2, 2, 2, 2)), 7, 7);
    painter.restore();
}

void EPushButton::timerEvent(QTimerEvent *event)
{
    if (m_nTimeId == event->timerId())
    {
        m_nRotate++;
        if (m_nRotate > 360)
            m_nRotate = 0;

        update();
    }
}
