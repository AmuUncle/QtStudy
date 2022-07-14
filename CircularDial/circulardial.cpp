#include "circulardial.h"

#include <QPainter>
#include <QDebug>
#include <QtMath>
#include <QFontDatabase>
#include <QPropertyAnimation>

#include "global.h"

static QRectF textRectF(double radius, int pointSize, double angle)
{
    QRectF rectF;
    rectF.setX(radius*qCos(angle*M_PI/180.0) - pointSize*2);
    rectF.setY(radius*qSin(angle*M_PI/180.0) - pointSize/2.0);
    rectF.setWidth(pointSize*4);
    rectF.setHeight(pointSize);
    return rectF;
}

CircularDial::CircularDial(QWidget *parent) : QWidget(parent)
{
    m_nLow = 30;
    m_nHigh = 120;
    m_nMax = 220;

    setProperty("DialValue", 0.0);
    m_pAnimationOpacity = new QPropertyAnimation(this, "DialValue");
    connect(m_pAnimationOpacity, SIGNAL(valueChanged(const QVariant&)), this, SLOT(update()));

    int nFontId = QFontDatabase::addApplicationFont(":/circulardial/iconfont.ttf");
    QStringList strlistFontName = QFontDatabase::applicationFontFamilies(nFontId);

    if (strlistFontName.count() > 0)
    {
        m_iconFont = QFont(strlistFontName.at(0));
    }
}

void CircularDial::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    QRect rcClient = rect();
    int nLen = qMin(rcClient.width(), rcClient.height()) - 50;
    QRect rcBase(rcClient.center().x() - nLen / 2, rcClient.center().y() - nLen / 2, nLen, nLen);

    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);

    p.translate(rcBase.center());

    int side = qMin(rcBase.width(), rcBase.height());
    p.scale(side / 300.0, side / 300.0);
    QRect rcCircularDial(-150, -150, 300, 300);

    const double START_ANGLE = -60;
    const double ALL_ANGLE = 360 - 2 * (90 + START_ANGLE);
    const double BIG_SINGLE_ANGLE = ALL_ANGLE / (m_nMax / 10.0);
    const double SMALL_SINGLE_ANGLE = BIG_SINGLE_ANGLE / 10.0;

    p.save();
    // 绘制[100,80)
    int startAngle = START_ANGLE * 16;    //起始角，以3点方向为0°，逆时针为整。
    int spanAngle = SMALL_SINGLE_ANGLE * (m_nMax - m_nHigh) * 16;    //偏移角，就是起始角和终止角的差值。绘制整个圆就输入360 * 16
    p.setPen(QPen(QColor("#ff4400"), 10, Qt::SolidLine, Qt::FlatCap));
    p.drawArc(rcCircularDial, startAngle, spanAngle);    //调用绘图命令

    // 绘制[80,20]
    startAngle = (START_ANGLE + SMALL_SINGLE_ANGLE * (m_nMax - m_nHigh)) * 16;    //起始角，以3点方向为0°，逆时针为整。
    spanAngle = SMALL_SINGLE_ANGLE * (m_nHigh - m_nLow) * 16;    //偏移角，就是起始角和终止角的差值。绘制整个圆就输入360 * 16
    p.setPen(QPen(QColor("#4687c1"), 10, Qt::SolidLine, Qt::FlatCap));
    p.drawArc(rcCircularDial, startAngle, spanAngle);    //调用绘图命令

    // 绘制(20,0]
    startAngle = (START_ANGLE + SMALL_SINGLE_ANGLE * (m_nMax - m_nLow)) * 16;    //起始角，以3点方向为0°，逆时针为整。
    spanAngle = SMALL_SINGLE_ANGLE * m_nLow * 16;    //偏移角，就是起始角和终止角的差值。绘制整个圆就输入360 * 16
    p.setPen(QPen(QColor("#238a23"), 10, Qt::SolidLine, Qt::FlatCap));
    p.drawArc(rcCircularDial, startAngle, spanAngle);    //调用绘图命令
    p.restore();

    p.save();
    p.rotate(90  + START_ANGLE);
    // 绘制[0,20]
    int i = 0;
    for(i = 0; i < m_nLow / 10; i++)
    {
        p.save();
        for (int j = 0; j < 10; j ++)
        {
            p.setPen(QPen(QColor("#238a23"), 1, Qt::SolidLine, Qt::FlatCap));
            p.drawLine(0, rcCircularDial.width() / 2 - 10, 0, rcCircularDial.width() / 2);
            p.rotate(SMALL_SINGLE_ANGLE);
        }
        p.restore();

        p.setPen(QPen(QColor("#238a23"), 2, Qt::SolidLine, Qt::FlatCap));
        p.drawLine(0, rcCircularDial.width() / 2 - 20, 0, rcCircularDial.width() / 2 + 5);
        p.rotate(BIG_SINGLE_ANGLE);
    }

    // 绘制[80,20]
    for(; i < m_nHigh / 10; i++)
    {
        p.save();
        for (int j = 0; j < 10; j ++)
        {
            p.setPen(QPen(QColor("#4687c1"), 1, Qt::SolidLine, Qt::FlatCap));
            p.drawLine(0, rcCircularDial.width() / 2 - 10, 0, rcCircularDial.width() / 2);
            p.rotate(SMALL_SINGLE_ANGLE);
        }
        p.restore();

        p.setPen(QPen(QColor("#4687c1"), 2, Qt::SolidLine, Qt::FlatCap));
        p.drawLine(0, rcCircularDial.width() / 2 - 20, 0, rcCircularDial.width() / 2 + 5);
        p.rotate(BIG_SINGLE_ANGLE);
    }

    // 绘制[80,20]
    for(; i <= m_nMax / 10; i++)
    {
        p.save();
        for (int j = 0; j < 10 && i < m_nMax / 10; j ++)
        {
            p.setPen(QPen(QColor("#ff4400"), 1, Qt::SolidLine, Qt::FlatCap));
            p.drawLine(0, rcCircularDial.width() / 2 - 10, 0, rcCircularDial.width() / 2);
            p.rotate(SMALL_SINGLE_ANGLE);
        }
        p.restore();

        p.setPen(QPen(QColor("#ff4400"), 2, Qt::SolidLine, Qt::FlatCap));
        p.drawLine(0, rcCircularDial.width() / 2 - 20, 0, rcCircularDial.width() / 2 + 5);
        p.rotate(BIG_SINGLE_ANGLE);
    }
    p.restore();


    p.save();
    QFont ftTmp1 = p.font();
    ftTmp1.setBold(true);
    ftTmp1.setPixelSize(10);
    p.setFont(ftTmp1);
    for(int i = 0; i <= m_nMax / 10; i++)
    {
        if (i < m_nLow / 10)
            p.setPen(QColor("#238a23"));
        else if (i < m_nHigh / 10)
            p.setPen(QColor("#4687c1"));
        else
            p.setPen(QColor("#ff4400"));

        p.drawText(textRectF(150 * 0.8, ftTmp1.pixelSize() + 5, 180 + START_ANGLE + i * BIG_SINGLE_ANGLE),
                   Qt::AlignCenter,
                   QString::number(i * 10));
    }
    p.restore();


    // 绘制 指示灯
    p.save();

    QFont ftTmp = m_iconFont;
    ftTmp.setBold(true);
    ftTmp.setPixelSize(30);
    p.setFont(ftTmp);
    p.setPen(QColor("#f9e565"));
    p.drawText(-60, -40, QChar(0xe603));
    p.drawText(30, -40, QChar(0xe605));

    p.setPen(QColor("#eaa09b"));
    p.drawText(-60, 40, QChar(0xe604));
    p.drawText(30, 40, QChar(0xe606));
    p.restore();


    float DialValue = property("DialValue").toFloat();

    p.save();
    QFont font1 = font();
    font1.setBold(true);
    font1.setPixelSize(26);
    p.setFont(font1);
    if (DialValue < m_nLow)
        p.setPen(QColor("#238a23"));
    else if (DialValue < m_nHigh)
        p.setPen(QColor("#4687c1"));
    else
        p.setPen(QColor("#ff4400"));

    QRect rcValue(-60, 60, 120, 50);
    p.drawText(rcValue, Qt::AlignCenter, QString("%1Km/h").arg(QString::number(DialValue, 'f', 0)));
    p.restore();

    p.save();
    p.rotate(180 + START_ANGLE);  // 先到0点
    p.rotate((180 - 2 * (START_ANGLE)) / m_nMax * DialValue);

    QPainterPath path;
    QPolygon triangle;
    triangle.setPoints(4,    110, 0,    0, -5,    -10, 0,    0, 5);
    path.addPolygon(triangle);
    p.setPen(Qt::NoPen);
    p.setBrush(QColor("#1f901a"));      // 画刷
    p.drawPath(path);
    p.restore();

    p.save();
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::white);              // 画刷
    p.drawEllipse(rcCircularDial.center(), 2, 2);
    p.restore();
}

void CircularDial::mousePressEvent(QMouseEvent *event)
{
    m_pAnimationOpacity->stop();

    float DialValue = property("DialValue").toFloat();
    m_pAnimationOpacity->setDuration(2000 * (m_nMax - DialValue) / m_nMax);
    m_pAnimationOpacity->setEasingCurve(QEasingCurve::InOutQuad);
    m_pAnimationOpacity->setStartValue(DialValue);
    m_pAnimationOpacity->setEndValue(m_nMax);
    m_pAnimationOpacity->start();
}

void CircularDial::mouseReleaseEvent(QMouseEvent *event)
{
    m_pAnimationOpacity->stop();

    float DialValue = property("DialValue").toFloat();
    m_pAnimationOpacity->setDuration(2000 * DialValue / m_nMax);
    m_pAnimationOpacity->setEasingCurve(QEasingCurve::Linear);
    m_pAnimationOpacity->setStartValue(DialValue);
    m_pAnimationOpacity->setEndValue(0);
    m_pAnimationOpacity->start();
}
