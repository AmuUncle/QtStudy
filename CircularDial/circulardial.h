#ifndef CIRCULARDIAL_H
#define CIRCULARDIAL_H

#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>

class CircularDial : public QWidget
{
    Q_OBJECT
public:
    explicit CircularDial(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    int m_nLow;
    int m_nHigh;
    int m_nMax;

    QFont m_iconFont;
    QPropertyAnimation *m_pAnimationOpacity;
};

#endif // CIRCULARDIAL_H
