﻿#include "radar.h"
#include <QPainter>
#include <QTimerEvent>
#include <QDebug>


Radar::Radar(QWidget *parent) : QWidget(parent)
{
    m_pieRotate = 0;

    m_timerId = startTimer(50);
    m_pointTimerId = startTimer(1000);
}

void Radar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QRect rcClient = rect();
    painter.fillRect(rcClient, QColor("#111111"));

    int margin = 10;

    QRect rcArea = rcClient.marginsRemoved(QMargins(margin, margin, margin, margin));
    int len = qMin(rcArea.width(), rcArea.height());

    QPoint ptCenter = rcArea.center();
    rcArea = QRect(ptCenter.x() - len / 2, ptCenter.y() - len / 2, len, len);

    painter.setPen(QPen("#4fe665"));
    painter.drawLine(rcArea.center() - QPoint(0,len/2), rcArea.center() + QPoint(0,len/2));
    painter.drawLine(rcArea.center() - QPoint(len/2,0), rcArea.center() + QPoint(len/2,0));
    painter.drawEllipse(rcArea.center(),len/2,len/2);
    painter.drawEllipse(rcArea.center(),len/3,len/3);
    painter.drawEllipse(rcArea.center(),len/6,len/6);

    qreal x = rcArea.center().x()+(qreal)len/2 * cos(-m_pieRotate*3.14159/180);
    qreal y = rcArea.center().y()+(qreal)len/2 * sin(-m_pieRotate*3.14159/180);
    painter.drawLine(rcArea.center(),QPointF(x,y));

    QConicalGradient gradient;
    gradient.setCenter(rcArea.center());
    gradient.setAngle(m_pieRotate+180);
    gradient.setColorAt(0.4,QColor(0x4f,0xe6,0x65,100));
    gradient.setColorAt(0.8,QColor(0x4f,0xe6,0x65,0));
    painter.setBrush(QBrush(gradient));
    painter.setPen(Qt::NoPen);
    painter.drawPie(rcArea,m_pieRotate*16,60*16);

    foreach (TDstPoint tDstPoint, m_points)
    {
        painter.setBrush(tDstPoint.color);
        painter.drawEllipse(tDstPoint.pt, tDstPoint.size, tDstPoint.size);
    }
}

void Radar::timerEvent(QTimerEvent *event)
{
    if(m_timerId == event->timerId())
    {
        m_pieRotate -= 10;
        update();
    }
    else if(m_pointTimerId == event->timerId())
    {
        CreatePoints();
    }
}

void Radar::resizeEvent(QResizeEvent *event)
{
    CreatePoints();
}

void Radar::CreatePoints()
{
    //随机更换装饰的点
    m_points.clear();
    QRect rcClient = rect();
    if (rcClient.width() <= 0)
    {
        return;
    }

    TDstPoint tDstPoint;
    for(int i = 0 ; i < 20; ++i)
    {
        int offsetx = rand() % rcClient.width();
        int offsetY = rand() % rcClient.height();
        tDstPoint.pt = QPoint(offsetx, offsetY);
        tDstPoint.color = QColor(255, 255, 255, rand() % 255);
        tDstPoint.size = (rand() % 3) + 1;
        m_points.append(tDstPoint);
    }

    update();
}
