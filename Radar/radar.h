#ifndef RADAR_H
#define RADAR_H

#include <QWidget>
#include <QList>

class Radar : public QWidget
{
    Q_OBJECT
public:
    explicit Radar(QWidget *parent = nullptr);

    struct TDstPoint
    {
        QPoint pt;
        QColor color;
        int size;
    };

private:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    void resizeEvent(QResizeEvent *event);

    void CreatePoints();

private:
    int m_pieRotate;
    int m_timerId;
    int m_pointTimerId;
    QList<TDstPoint> m_points;
};

#endif // RADAR_H
