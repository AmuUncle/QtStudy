#include "animation.h"
#include <QPainter>
#include <QPushButton>
#include <QtCore/QMetaEnum>
#include <QPropertyAnimation>
#include <QGridLayout>
#include <QSignalMapper>

#include "global.h"

const int PAINT_WIDTH = 200;

Animation::Animation(QWidget *parent) : QWidget(parent)
{
    setProperty("BallHeight", 1.0);

    QGridLayout *layoutMain = new QGridLayout(this);
    layoutMain->setContentsMargins(220, 10, 10, 10);

    QSignalMapper *pSignalMapperPushed = new QSignalMapper(this);

    QMetaEnum metaEnum = QMetaEnum::fromType<QEasingCurve::Type>();
    for (int i = 0; i <= 40; i++)
    {
        QEasingCurve::Type eType = (QEasingCurve::Type)i;
        const char* s = metaEnum.valueToKey(eType);
        QPushButton *btn = new QPushButton(this);
        btn->setText(s);
        layoutMain->addWidget(btn, i / 5, i % 5);

        pSignalMapperPushed->setMapping(btn, eType);
        connect(btn, SIGNAL(clicked()), pSignalMapperPushed, SLOT(map()));
    }

    connect(pSignalMapperPushed, SIGNAL(mapped(int)), this, SLOT(OnSignalPushedMapped(int)));
}

void Animation::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    QRect rcClient = rect();

    QRect rcDraw = rcClient;
    rcDraw = rcClient.marginsRemoved(QMargins(10, 10, 10, 10));
    rcDraw.setWidth(PAINT_WIDTH);

    QPoint ptCenter = rcDraw.center();

    int r = 50;
    float fBallHeight = property("BallHeight").toFloat();
    fBallHeight = qMax(fBallHeight, (float)0.0);

    QRadialGradient radialGradient(QPoint(ptCenter.x(), rcDraw.top() + (rcDraw.height() - 2 * r) * fBallHeight + r), r);
    radialGradient.setColorAt (0,QColor (Qt::lightGray));
    radialGradient.setColorAt (0.6,QColor (Qt::gray));
    radialGradient.setColorAt (1,QColor (Qt::darkGray));
    radialGradient.setFocalPoint(QPoint(ptCenter.x() - 10, rcDraw.top() + (rcDraw.height() - 2 * r) * fBallHeight + r - 10));

    QBrush brush(radialGradient);
    painter.setBrush (brush);
    painter.setOpacity(fBallHeight);
    painter.drawEllipse(QPoint(ptCenter.x(), rcDraw.top() + (rcDraw.height() - 2 * r) * fBallHeight + r), r, r);
}

void Animation::OnSignalPushedMapped(int nCmd)
{
    QPropertyAnimation *pAnimationOpacity = new QPropertyAnimation(this, "BallHeight");
    pAnimationOpacity->setDuration(800);
    pAnimationOpacity->setEasingCurve((QEasingCurve::Type)nCmd);
    pAnimationOpacity->setStartValue(0.0);
    pAnimationOpacity->setEndValue(1.0);
    connect(pAnimationOpacity, SIGNAL(valueChanged(const QVariant&)), this, SLOT(update()));
    pAnimationOpacity->start(QPropertyAnimation::DeleteWhenStopped);
}
