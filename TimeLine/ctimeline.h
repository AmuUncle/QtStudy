#ifndef CTIMELINE_H
#define CTIMELINE_H

#include <QWidget>
#include <QDateTime>

enum ETimeLineType
{
    TIMELINETYPE_NORMAL,
    TIMELINETYPE_RICEWHITE,
    TIMELINETYPE_COUNT
};

class CTimeLine : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(ETimeLineType m_eTimeLineType READ GetType WRITE SetType)

public:
    explicit CTimeLine(QWidget *parent = 0, ETimeLineType eTimeLineType = TIMELINETYPE_NORMAL);

    enum ERecordType
    {
        TYPE_NORMAL,
        TYPE_ALARM,
        TYPE_TIME,
        TYPE_COUNT
    };

    struct TRecord
    {
        uint dwSrartTime;
        uint dwEndTime;
        ERecordType eType;

        TRecord()
        {
            dwSrartTime = 0;
            dwEndTime = 0;
            eType = TYPE_NORMAL;
        }

        TRecord(uint _dwSrartTime, uint _dwEndTime, ERecordType _eType)
        {
            dwSrartTime = _dwSrartTime;
            dwEndTime = _dwEndTime;
            eType = _eType;
        }

        uint Duration()
        {
            return dwEndTime - dwSrartTime;
        }
    };

public:
    uint GetMidTimestamp();
    void SetMidTimestamp(uint dwTimestamp);
    void SetType(ETimeLineType eTimeLineType);
    ETimeLineType GetType() { return m_eTimeLineType; }

private:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void leaveEvent(QEvent *event);
    void resizeEvent(QResizeEvent *event);

    void DrawGraduations(QPainter &painter);
    void DrawMidTime(QPainter &painter);
    void DrawHover(QPainter &painter);
    void DrawRecord(QPainter &painter);

    uint GetStartTimestamp();
    uint GetEndTimestamp();

    double GetPixelSeconds();
    uint GetTotalSecond();

    void ZoomIn();
    void ZoomOut();

signals:
    void seeked(uint dwSeekTime);

private:
    ETimeLineType m_eTimeLineType;

    QDateTime m_dateTimeMid;
    uint m_TotalMinutes;

    bool m_bMouseHover;
    uint m_dwHoverPos;

    bool m_bMouseDown;
    uint m_dwDownPos;

    bool m_bMouseMoved;

    QList<TRecord> m_listRecord;

    QColor m_colorBg;
    QColor m_colorBorder;
    QColor m_colorGraduation;
    QColor m_colorMidTime;
    QColor m_colorHoverTime;

    QColor m_colorRecNormal;
    QColor m_colorRecTime;
    QColor m_colorRecAlarm;
};

#endif // CTIMELINE_H
