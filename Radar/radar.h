#ifndef RADAR_H
#define RADAR_H

#include <QWidget>

class Radar : public QWidget
{
    Q_OBJECT
public:
    explicit Radar(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    void resizeEvent(QResizeEvent *event);

    void CreatePoints();

private:
    int m_pieRotate;
    int m_timerId;
    int m_pointTimerId;
    QPoint m_points[100];
};

#endif // RADAR_H
