#include "ctimeline.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QFontMetrics>


#define QLOGD qDebug() << QString("[%1:%2]").arg(__FUNCTION__).arg(__LINE__)

const uint HOUR_TO_SECOND = 60 * 60;

const uint ZOOM_RANGE_LIST[] = {1, 2, 5, 10, 15, 20, 30, 45,
                                60, 2 * 60, 3 * 60, 4 * 60, 5 * 60,
                                6 * 60, 7 * 60, 8 * 60, 9 * 60, 10 * 60,
                                11 * 60, 12 * 60, 13 * 60, 14 * 60, 15 * 60,
                                16 * 60, 17 * 60, 18 * 60, 19 * 60, 20 * 60,
                                21 * 60, 22 * 60, 23 * 60, 24 * 60};

const int BOMMOE = 20;

CTimeLine::CTimeLine(QWidget *parent, ETimeLineType eTimeLineType/* = TIMELINETYPE_NORMAL*/) : QWidget(parent)
{
    m_eTimeLineType = eTimeLineType;

    m_TotalMinutes = 24 * 60;
    m_dateTimeMid = QDateTime::currentDateTime();
    m_bMouseHover = false;
    m_dwHoverPos = 0;
    m_bMouseDown = false;
    m_dwDownPos = 0;
    m_bMouseMoved = false;

    qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());
    uint dwTimeStart = GetMidTimestamp() - 12 * 60 * 60;
    uint dwTimeEndTime = dwTimeStart + 1 * 60 * 60;
    for (int i = 0; i < 100; i++)
    {
        m_listRecord << TRecord(dwTimeStart, dwTimeEndTime, (ERecordType)(i % TYPE_COUNT));
        dwTimeStart = dwTimeEndTime;
        dwTimeEndTime = dwTimeStart + (qrand() % 10) * 60 * 60;
    }

    setFixedHeight(50);
    setMouseTracking(true);
    SetType(m_eTimeLineType);
}

void CTimeLine::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::TextAntialiasing);

    QRect rcClient = rect();

    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_colorBg);
    painter.drawRect(rcClient);
    painter.restore();

    DrawGraduations(painter);
    DrawRecord(painter);
    DrawMidTime(painter);
    DrawHover(painter);

    painter.save();
    painter.setBrush(Qt::NoBrush);
    painter.setPen(m_colorBorder);
    painter.drawRect(rcClient.adjusted(0, 0, -1, -1));
    painter.restore();
}

void CTimeLine::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    m_bMouseMoved = false;
    m_bMouseDown = true;
    m_dwDownPos = event->pos().x();
    update();
}

void CTimeLine::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    if (!m_bMouseMoved)
    {
        double pxPerMs = GetPixelSeconds();
        uint posX = event->pos().x();
        SetMidTimestamp(GetStartTimestamp() + (posX / pxPerMs));

        emit seeked(GetMidTimestamp());
    }

    m_bMouseMoved = false;
    m_bMouseDown = false;
    setCursor(Qt::ArrowCursor);
    update();
}

void CTimeLine::mouseMoveEvent(QMouseEvent *event)
{
    m_bMouseMoved = true;

    if (m_bMouseDown)
    {
        setCursor(Qt::ClosedHandCursor);

        double pxPerMs = GetPixelSeconds();

        uint posX = event->pos().x();
        int diffX = posX - m_dwDownPos;
        SetMidTimestamp(GetMidTimestamp() - (diffX / pxPerMs));

        m_dwDownPos = posX;
        m_bMouseHover = false;
        emit seeked(GetMidTimestamp());
    }
    else
    {
        m_bMouseHover = true;
        m_dwHoverPos = event->pos().x();
    }

    update();
}

void CTimeLine::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0)
    {
        ZoomIn();
    }
    else
    {
        ZoomOut();
    }

    update();
}

void CTimeLine::leaveEvent(QEvent *event)
{
    m_bMouseHover = false;
    setCursor(Qt::ArrowCursor);
    update();
}

void CTimeLine::resizeEvent(QResizeEvent *event)
{
    if (m_TotalMinutes * 60 < width())
    {
        for (int i = 0; i < sizeof(ZOOM_RANGE_LIST)/sizeof(uint); i++)
        {
            if (ZOOM_RANGE_LIST[i] * 60 >= width())
            {
                m_TotalMinutes = ZOOM_RANGE_LIST[i];
                return;
            }
        }
    }
}

void CTimeLine::DrawGraduations(QPainter &painter)
{
    painter.save();

    uint startTimestamp = GetStartTimestamp();

    // px/min 一分钟占多少像素
    double pxPerMin = (double)width() / double(m_TotalMinutes);

    // px/s  一毫秒占多少像素
    double pxPerMs = GetPixelSeconds();

    // px/格 刻度间隔像素
    double pxPerStep = 20;

    // min/格   一格多少分钟
    int minPerStep = pxPerStep / pxPerMin;

    // min/格  每格多少分钟
    int minutesPerStep[] = {1, 2, 5, 10, 15, 20, 30, 60, 120, 180, 240, 360, 720, 1440};
    for (int i = 0; i < sizeof(minutesPerStep)/sizeof(int); i++)
    {
        if (minPerStep <= minutesPerStep[i])
        {
            // 让每格时间在minutes_per_step规定的范围内
            minPerStep = minutesPerStep[i];
            pxPerStep = pxPerMin * minPerStep;
            break;
        }
    }

    int mediumStep = 0;     // 两个标题间隔时间
    switch (minPerStep)
    {
        case 1:
            {
                mediumStep = 5;
            }
            break;

        case 5:
            {
                mediumStep = minPerStep * 3;
            }
            break;

        case 10:
            {
                mediumStep = minPerStep * 3;
            }
            break;

        case 15:
            {
                mediumStep = minPerStep * 4;
            }
            break;

        case 20:
            {
                mediumStep = minPerStep * 3;
            }
            break;

        case 30:
            {
                mediumStep = minPerStep * 2;
            }
            break;

        case 60:
            {
                mediumStep = minPerStep;
            }
            break;

        default:
            {
                mediumStep = minPerStep * 5;
            }
            break;
    }

    // 总格数
    int numSteps = width() / (int)pxPerStep;
    int graduationTime;
    int lineHeight = 0;

    // 开始第一个格子的偏移时间
    uint remainder = startTimestamp % (minPerStep * 60);
    int msOffset = remainder ? (minPerStep * 60) - remainder : 0;

    // 第一个格子的偏移像素
    int pxOffset = msOffset * pxPerMs;                // 开始的偏移距离 px
    // 一刻度占用多少毫秒
    double msPerStep = (double)pxPerStep / pxPerMs;              // ms/step

    if (TIMELINETYPE_RICEWHITE == m_eTimeLineType)
    {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(m_colorGraduation);
        painter.drawLine(0, height() - BOMMOE, width(), height() - BOMMOE);
    }

    for(int i = -1; i < numSteps; i++)
    {
        // 距离=开始的偏移距离+格数*px/格           格子的偏移位置像素
        int graduationLeft = pxOffset + i * pxPerStep;

        // 时间=左侧开始时间+偏移时间+格数*ms/格     格子的当前时间
        graduationTime = startTimestamp + msOffset + i * msPerStep;

        if (0 == (graduationTime / (60 )) % mediumStep)
        {
            QString strText = QDateTime::fromTime_t(graduationTime).toString("hh:mm");
            painter.setPen(m_colorGraduation);

            if (TIMELINETYPE_NORMAL == m_eTimeLineType)
            {
                lineHeight = 12;

                QFontMetrics elideFont(painter.font());
                int nTextWidth = elideFont.width(strText);

                QPoint ptCenter(graduationLeft, height() - 12);
                QRect rcText(ptCenter.x() - (nTextWidth / 2), ptCenter.y() - 30, nTextWidth, 30);

                painter.drawText(rcText, Qt::AlignBottom | Qt::AlignHCenter, QDateTime::fromTime_t(graduationTime).toString("hh:mm"));
            }
            else if (TIMELINETYPE_RICEWHITE == m_eTimeLineType)
            {
                lineHeight = 6;

                QFontMetrics elideFont(painter.font());
                int nTextWidth = elideFont.width(strText);

                QPoint ptCenter(graduationLeft, height());
                QRect rcText(ptCenter.x() - (nTextWidth / 2), ptCenter.y() - 30, nTextWidth, 30);

                painter.drawText(rcText, Qt::AlignBottom | Qt::AlignHCenter, strText);
            }
        }
        else
        {
            if (TIMELINETYPE_NORMAL == m_eTimeLineType)
            {
                lineHeight = 6;
            }
            else if (TIMELINETYPE_RICEWHITE == m_eTimeLineType)
            {
                lineHeight = 3;
            }
        }

        if (TIMELINETYPE_NORMAL == m_eTimeLineType)
        {
            painter.setBrush(Qt::NoBrush);
            painter.setPen(m_colorGraduation);
            painter.drawLine(graduationLeft, height(), graduationLeft, height() - lineHeight);
        }
        else if (TIMELINETYPE_RICEWHITE == m_eTimeLineType)
        {
            painter.setBrush(Qt::NoBrush);
            painter.setPen(m_colorGraduation);
            painter.drawLine(graduationLeft, height() - BOMMOE, graduationLeft, height() - BOMMOE + lineHeight);
        }
    }

    painter.restore();
}

void CTimeLine::DrawMidTime(QPainter &painter)
{
    painter.save();
    QPen pen(m_colorMidTime);
    painter.setPen(pen);

    QRect rcText = rect();

    QString strText = m_dateTimeMid.toString("yyyy-MM-dd");
    QFontMetrics elideFont(painter.font());
    int nTextWidth = elideFont.width(strText);
    int nSpaceWidth = elideFont.width(" ");

    uint dwMidPos = width() / 2;
    rcText.setLeft(dwMidPos - nTextWidth - (nSpaceWidth / 2));
    rcText.setWidth((nTextWidth + nSpaceWidth) * 2);

    rcText.setHeight(rcText.height() - 24);

    painter.drawText(rcText, Qt::AlignBottom | Qt::AlignLeft, m_dateTimeMid.toString("yyyy-MM-dd hh:mm:ss"));

    if (TIMELINETYPE_NORMAL == m_eTimeLineType)
    {
        painter.drawLine(width() / 2, 0, width() / 2, height());
    }
    else if (TIMELINETYPE_RICEWHITE == m_eTimeLineType)
    {
        pen.setWidth(2);
        painter.drawLine(width() / 2, 0, width() / 2, height() - BOMMOE);

        painter.save();
        painter.setBrush(m_colorMidTime);   // 画刷

        QPolygon triangle;
        triangle.setPoints(3, width() / 2, height() - BOMMOE - 4, width() / 2 - 4, height() - BOMMOE, width() / 2 + 4, height() - BOMMOE); // 三点坐标
        painter.drawPolygon(triangle);           // 画三角形
        painter.restore();
    }

    painter.restore();
}

void CTimeLine::DrawHover(QPainter &painter)
{
   if (!m_bMouseHover) return;

    painter.save();
    double pxPerMs = GetPixelSeconds();
    uint time = GetStartTimestamp() + (double)m_dwHoverPos / pxPerMs;

    painter.setPen(m_colorHoverTime);

    QString strText = QDateTime::fromTime_t(time).toString("yyyy-MM-dd");
    QFontMetrics elideFont(painter.font());
    int nTextWidth = elideFont.width(strText);
    int nSpaceWidth = elideFont.width(" ");

    QRect rcText = rect();
    rcText.setHeight(rcText.height() - 24);
    rcText.setLeft(m_dwHoverPos - nTextWidth - (nSpaceWidth / 2));
    rcText.setWidth((nTextWidth + nSpaceWidth) * 2);
    painter.drawText(rcText, Qt::AlignBottom | Qt::AlignLeft, QDateTime::fromTime_t(time).toString("yyyy-MM-dd hh:mm:ss"));

    if (TIMELINETYPE_NORMAL == m_eTimeLineType)
    {
        painter.drawLine(m_dwHoverPos, 0, m_dwHoverPos, height());
    }
    else if (TIMELINETYPE_RICEWHITE == m_eTimeLineType)
    {
        painter.drawLine(m_dwHoverPos, 0, m_dwHoverPos, height() - BOMMOE);
    }

    painter.restore();
}

void CTimeLine::DrawRecord(QPainter &painter)
{
    if (m_listRecord.length() < 1) return;

    painter.save();

    uint dwStartTimestamp = GetStartTimestamp();
    uint dwEndTimestamp = GetEndTimestamp();
    double fPixelSeconds = GetPixelSeconds();

    QRect rcRec;
    if (TIMELINETYPE_NORMAL == m_eTimeLineType)
    {
        rcRec.setTop(5);
        rcRec.setHeight(8);
    }
    else if (TIMELINETYPE_RICEWHITE == m_eTimeLineType)
    {
        rcRec.setTop(0);
        rcRec.setHeight(height() - BOMMOE);
    }

    foreach (TRecord rec, m_listRecord)
    {
        if (rec.dwEndTime <= dwStartTimestamp || rec.dwSrartTime >= dwEndTimestamp)
        {
            continue;
        }

        if (rec.dwSrartTime <= dwStartTimestamp)
        {
            rcRec.setLeft(0);
        }
        else
        {
            rcRec.setLeft((rec.dwSrartTime - dwStartTimestamp) * fPixelSeconds);
        }

        if (rec.dwEndTime >= dwEndTimestamp)
        {
           rcRec.setRight(width());
        }
        else
        {
           rcRec.setRight((rec.dwEndTime - dwStartTimestamp) * fPixelSeconds);
        }

        painter.setPen(Qt::NoPen);
        switch (rec.eType)
        {
        case TYPE_ALARM:
            {
                painter.setBrush(m_colorRecNormal);
            }
            break;

        case TYPE_TIME:
            {
                painter.setBrush(m_colorRecTime);
            }
            break;

        default:
            {
                painter.setBrush(m_colorRecAlarm);
            }
            break;
        }



        painter.drawRect(rcRec);
    }

    painter.restore();
}

uint CTimeLine::GetStartTimestamp()
{
    return GetMidTimestamp() - GetTotalSecond() / 2;
}

uint CTimeLine::GetMidTimestamp()
{
    return m_dateTimeMid.toTime_t();
}

uint CTimeLine::GetEndTimestamp()
{
    return GetMidTimestamp() + GetTotalSecond() / 2;
}

void CTimeLine::SetMidTimestamp(uint dwTimestamp)
{
    m_dateTimeMid = QDateTime::fromTime_t(dwTimestamp);
    update();
}

void CTimeLine::SetType(ETimeLineType eTimeLineType)
{
    m_eTimeLineType = eTimeLineType;

    if (TIMELINETYPE_NORMAL == m_eTimeLineType)
    {
        m_colorBg = QColor("#11141b");
        m_colorBorder = QColor("#11141b");
        m_colorGraduation = QColor("#8f96b0");

        m_colorMidTime = QColor("#dd0000");
        m_colorHoverTime = QColor("#c2cad7");

        m_colorRecNormal = QColor("#ff0000");
        m_colorRecTime = QColor("#00ff00");
        m_colorRecAlarm = QColor("#1584d9");
    }
    else if (TIMELINETYPE_RICEWHITE == m_eTimeLineType)
    {
        m_colorBg = QColor("#f7f6f9");
        m_colorBorder = QColor("#11141b");
        m_colorGraduation = QColor("#aeb0b2");

        m_colorMidTime = QColor("#2470af");
        m_colorHoverTime = QColor("#62abf5");

        m_colorRecNormal = QColor("#d4d1d9");
        m_colorRecTime = QColor("#8dd1cf");
        m_colorRecAlarm = QColor("#f97549");
    }
}

double CTimeLine::GetPixelSeconds()
{
    return (double)width() / (double)(GetTotalSecond());
}

uint CTimeLine::GetTotalSecond()
{
    return m_TotalMinutes * 60;
}

void CTimeLine::ZoomIn()
{
    for (int i = 0; i < sizeof(ZOOM_RANGE_LIST)/sizeof(uint); i++)
    {
        if (m_TotalMinutes == ZOOM_RANGE_LIST[i])
        {
            if (i > 0)
            {
                m_TotalMinutes = ZOOM_RANGE_LIST[i - 1];

                if (m_TotalMinutes * 60 < width())
                {
                    m_TotalMinutes = ZOOM_RANGE_LIST[i];
                }

                return;
            }
        }
    }
}

void CTimeLine::ZoomOut()
{
    for (int i = 0; i < sizeof(ZOOM_RANGE_LIST)/sizeof(uint); i++)
    {
        if (m_TotalMinutes == ZOOM_RANGE_LIST[i])
        {
            if (i < (sizeof(ZOOM_RANGE_LIST)/sizeof(uint) - 1))
            {
                m_TotalMinutes = ZOOM_RANGE_LIST[i + 1];
                return;
            }
        }
    }
}
