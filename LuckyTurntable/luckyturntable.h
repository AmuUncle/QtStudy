#ifndef LUCKYTURNTABLE_H
#define LUCKYTURNTABLE_H

#include <QWidget>
#include <QPropertyAnimation>

class LuckyTurntable : public QWidget
{
    Q_OBJECT

public:
    LuckyTurntable(QWidget *parent = nullptr);
    ~LuckyTurntable();

public:
    void Start();

private:
    void paintEvent(QPaintEvent *);
    void paintZhuanPan(QPainter &painter, QRect rcClient);
    virtual void timerEvent(QTimerEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    QString GetPrize();

private slots:
    void OnAnimationValueChanged(const QVariant&);
    void OnAnimationFinished();

private:
    QRect m_rcZhuanPan;
    int m_nTimerId;
    double m_nRotate;
    int m_nCount;
    bool m_bRuning;
    QPropertyAnimation* m_pAnimationOpacity;
    QPixmap m_pixImageBase;
    QPixmap m_pixImageZhuan;
};

#endif // LUCKYTURNTABLE_H
