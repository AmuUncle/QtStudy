#include "luckyturntable.h"

#include <QPainter>
#include <QTimerEvent>
#include <QTime>
#include <QDebug>
#include <QMouseEvent>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

LuckyTurntable::LuckyTurntable(QWidget *parent)
    : QWidget(parent)
{
    m_nRotate = 0;
    m_nCount = 0;
    m_bRuning = false;

    setWindowTitle(tr("抽奖大转盘"));
    m_pixImageBase.load(":/LuckyTurntable/base.png");
    m_pixImageZhuan.load(":/LuckyTurntable/zhen.png");

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    qreal fEnd = 360.0 + (qrand() % 360);
    setProperty("RotationAngle", 1.0);
    m_pAnimationOpacity = new QPropertyAnimation(this, "RotationAngle");
    m_pAnimationOpacity->setDuration(3000);
    m_pAnimationOpacity->setEasingCurve(QEasingCurve::OutCubic);  // 缓和曲线风格
    m_pAnimationOpacity->setStartValue(0.0);
    m_pAnimationOpacity->setEndValue(fEnd);
    connect(m_pAnimationOpacity, SIGNAL(finished()), this, SLOT(OnAnimationFinished()));
    connect(m_pAnimationOpacity, SIGNAL(valueChanged(const QVariant&)), this, SLOT(OnAnimationValueChanged(const QVariant&)));
}

LuckyTurntable::~LuckyTurntable()
{
}

void LuckyTurntable::Start()
{
    qsrand(QTime::currentTime().msec());
    qreal fEnd = 360.0 + (qrand() % 360);
    m_pAnimationOpacity->setEndValue(fEnd);
    qDebug() << fEnd;

    m_nCount = 0;
    m_nTimerId = startTimer(1);
    m_bRuning = true;
}

void LuckyTurntable::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing, true); // 抗锯齿和使用平滑转换算法

    int nTextHeight = 60;
    QRect rcClient = rect();
    int nWidth = qMin(rcClient.width(), rcClient.height() - nTextHeight);

    m_rcZhuanPan.setLeft(rcClient.left() + (rcClient.width() - nWidth) / 2);
    m_rcZhuanPan.setRight(m_rcZhuanPan.left() + nWidth);
    m_rcZhuanPan.setTop(rcClient.top() + (rcClient.height() - nTextHeight - nWidth) / 2);
    m_rcZhuanPan.setBottom(m_rcZhuanPan.top() + nWidth);
    paintZhuanPan(painter, m_rcZhuanPan);

    QRect rcText(m_rcZhuanPan);
    rcText.setTop(rcText.bottom() + 10);
    rcText.setBottom(rcClient.bottom());

    painter.setPen(QColor("#333"));
    QFont ftTemp = painter.font();
    ftTemp.setPointSize(15);
    painter.setFont(ftTemp);
    painter.drawText(rcText, Qt::AlignTop | Qt::AlignHCenter, GetPrize());
}

void LuckyTurntable::paintZhuanPan(QPainter &painter, QRect rcClient)
{
    painter.save();

    painter.translate(rcClient.left(), rcClient.top());  // 移动绘制原点
    painter.scale(rcClient.width() / 1302.0, rcClient.height() / 1302.0);

    painter.save();
    QRect rectBase(0, 0, 1302.0, 1302.0);
    painter.translate(rectBase.center());

    painter.rotate(m_nRotate);

    QPixmap p = m_pixImageBase.scaled(rectBase.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QRect _rectBase(-1302.0 / 2, -1302.0 / 2, 1302.0, 1302.0);
    painter.drawPixmap(_rectBase, p);

    painter.restore();

    QRect rectzhuan((1302.0 - 276.0) / 2, (1302.0 - 363.0) / 2 - 40, 276.0, 363.0);
    QPixmap pZhuan = m_pixImageZhuan.scaled(rectzhuan.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    painter.drawPixmap(rectzhuan, pZhuan);

    painter.restore();
}

void LuckyTurntable::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    if (event->timerId() == m_nTimerId)
    {
        m_nRotate+= 10;

        if (m_nRotate >= 360.0)
        {
            m_nCount++;
            m_nRotate = 0.0;
        }

        update();

        if (m_nCount >= 5)
        {
            killTimer(m_nTimerId);

            m_pAnimationOpacity->stop();
            m_pAnimationOpacity->start();
        }
    }
}

void LuckyTurntable::mousePressEvent(QMouseEvent *event)
{


}

void LuckyTurntable::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_bRuning)
        return;

    double fRadio = 170.0 / 420;
    QRect rcStart;
    rcStart.setLeft(m_rcZhuanPan.left() + m_rcZhuanPan.width() * fRadio);
    rcStart.setRight(m_rcZhuanPan.right() - m_rcZhuanPan.width() * fRadio);
    rcStart.setTop(m_rcZhuanPan.top() + m_rcZhuanPan.height() * fRadio);
    rcStart.setBottom(m_rcZhuanPan.bottom() + m_rcZhuanPan.height() * fRadio);

    //qDebug() << event->pos() << m_rcZhuanPan << rcStart << rcStart.contains(event->pos());

    if (rcStart.contains(event->pos()))
        Start();
}

QString LuckyTurntable::GetPrize()
{
    QString strPrize = "";
    double fUnitRotate = 360.0 / 7;
    int nItem = m_nRotate / fUnitRotate;
    nItem++;
    //qDebug() << m_nRotate << fUnitRotate << nItem;

    switch (nItem)
    {
    case 1:
        {
            strPrize = tr("壁挂炉");
        }
        break;

    case 2:
        {
            strPrize = tr("500热力值");
        }
        break;

    case 3:
        {
            strPrize = tr("联名运动包");
        }
        break;

    case 4:
        {
            strPrize = tr("阿里斯顿毛巾");
        }
        break;

    case 5:
        {
            strPrize = tr("热水管家");
        }
        break;

    case 6:
        {
            strPrize = tr("100热力值");
        }
        break;

    case 7:
        {
            strPrize = tr("联名保温杯");
        }
        break;
    }

    return strPrize;
}

void LuckyTurntable::OnAnimationValueChanged(const QVariant &)
{
    float fRotationAngle = property("RotationAngle").toFloat();
    fRotationAngle = qMax(fRotationAngle, (float)0.0);

    m_nRotate = fRotationAngle;

    if (m_nRotate >= 360.0)
    {
        m_nRotate = m_nRotate - 360.0;
    }

    //qDebug() << m_nRotate;

    update();
}

void LuckyTurntable::OnAnimationFinished()
{
    qDebug() << GetPrize();
    m_bRuning = false;
}
