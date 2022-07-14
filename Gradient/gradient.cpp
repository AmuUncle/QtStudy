#include "gradient.h"
#include <QPainter>
#include <QLinearGradient>

#include "global.h"

Gradient::Gradient(QWidget *parent) : QWidget(parent)
{

}

void Gradient::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    QRect rcClient = rect();

    ///////////////////////线性渐变///////////////////////////////////////////
    QRect rcLinear = rcClient;
    rcLinear = rcLinear.marginsRemoved(QMargins(10, 10, 10, 10));
    rcLinear.setBottom(rcLinear.top() + rcClient.height() / 3);
    // # PadSpread(默认值): 填充和结束值一样的颜色。
    QRect rcLinear1 = rcLinear;
    rcLinear1.setWidth(rcLinear.width() / 3);
    rcLinear1 = rcLinear1.marginsRemoved(QMargins(10, 10, 10, 10));

    {
        //构建线性渐变对象
        QPoint ptCenter = rcLinear1.center();
        QLinearGradient linearGradient(QPointF(ptCenter.x(), rcLinear1.top()),QPointF(ptCenter.x(), ptCenter.y()));
        linearGradient.setSpread(QGradient::PadSpread);

        linearGradient.setColorAt(0,QColor (Qt::green));
        linearGradient.setColorAt(0.6,QColor(Qt::blue));
        linearGradient.setColorAt(1,QColor(Qt::red));
        //画刷设置渐变
        QBrush brush(linearGradient);
        painter.setBrush(brush);
        painter.drawRect(rcLinear1);
    }

    // # RepeatSpread:重复渐变。
    QRect rcLinear2 = rcLinear;
    rcLinear2.setLeft(rcLinear1.right());
    rcLinear2.setWidth(rcLinear.width() / 3);
    rcLinear2 = rcLinear2.marginsRemoved(QMargins(10, 10, 10, 10));
    {
        //构建线性渐变对象
        QPoint ptCenter = rcLinear2.center();
        QLinearGradient linearGradient(QPointF(ptCenter.x(), rcLinear2.top()),QPointF(ptCenter.x(), ptCenter.y()));
        linearGradient.setSpread(QGradient::RepeatSpread);

        linearGradient.setColorAt(0,QColor (Qt::green));
        linearGradient.setColorAt(0.6,QColor(Qt::blue));
        linearGradient.setColorAt(1,QColor(Qt::red));
        //画刷设置渐变
        QBrush brush(linearGradient);
        painter.setBrush(brush);
        painter.drawRect(rcLinear2);
    }

    // # ReflectSpread:反射渐变。比如渐变颜色为绿-红-蓝，整个填充将会是绿-红-蓝-蓝-红-绿-绿-红-蓝.....
    QRect rcLinear3 = rcLinear;
    rcLinear3.setLeft(rcLinear2.right());
    rcLinear3.setWidth(rcLinear.width() / 3);
    rcLinear3 = rcLinear3.marginsRemoved(QMargins(10, 10, 10, 10));
    {
        //构建线性渐变对象
        QPoint ptCenter = rcLinear3.center();
        QLinearGradient linearGradient(QPointF(ptCenter.x(), rcLinear3.top()),QPointF(ptCenter.x(), ptCenter.y()));
        linearGradient.setSpread(QGradient::ReflectSpread);

        linearGradient.setColorAt(0,QColor (Qt::green));
        linearGradient.setColorAt(0.6,QColor(Qt::blue));
        linearGradient.setColorAt(1,QColor(Qt::red));
        //画刷设置渐变
        QBrush brush(linearGradient);
        painter.setBrush(brush);
        painter.drawRect(rcLinear3);
    }


    ///////////////////////径向渐变///////////////////////////////////////////
    QRect rcRadial = rcClient;
    rcRadial = rcRadial.marginsRemoved(QMargins(10, 10, 10, 10));
    rcRadial.setTop(rcLinear.bottom());
    rcRadial.setHeight(rcClient.height() / 3);

    // 默认
    QRect rcRadial1 = rcRadial;
    rcRadial1.setWidth(rcRadial.width() / 2);
    rcRadial1 = rcRadial1.marginsRemoved(QMargins(10, 10, 10, 10));

    {
        QPoint ptCenter = rcRadial1.center();
        QRadialGradient radialGradient(ptCenter, rcRadial1.height() - 50);
        radialGradient.setColorAt (0,QColor (Qt::green));
        radialGradient.setColorAt (1,QColor (Qt::red));

        QBrush brush(radialGradient);
        painter.setBrush (brush);
        painter.drawRect (rcRadial1);
    }

    // # 设置焦点
    QRect rcRadial2 = rcRadial;
    rcRadial2.setLeft(rcRadial1.right());
    rcRadial2.setWidth(rcRadial.width() / 2);
    rcRadial2 = rcRadial2.marginsRemoved(QMargins(10, 10, 10, 10));

    {
        QPoint ptCenter = rcRadial2.center();
        QRadialGradient radialGradient(ptCenter, rcRadial2.height() - 50);
        radialGradient.setFocalPoint(ptCenter += QPoint(50, 50));
        radialGradient.setColorAt (0,QColor (Qt::green));
        radialGradient.setColorAt (1,QColor (Qt::red));

        QBrush brush(radialGradient);
        painter.setBrush (brush);
        painter.drawRect (rcRadial2);
    }


    ///////////////////////锥形渐变///////////////////////////////////////////
    QRect rcConical = rcClient;
    rcConical = rcConical.marginsRemoved(QMargins(10, 10, 10, 10));
    rcConical.setTop(rcRadial.bottom());
    rcConical.setHeight(rcClient.height() / 3);

    // 默认
    QRect rcConical1 = rcConical;
    rcConical1.setWidth(rcConical.width() / 2);
    rcConical1 = rcConical1.marginsRemoved(QMargins(10, 10, 10, 10));

    {
        QPoint ptCenter = rcConical1.center();
        QConicalGradient conicalGradient;
        conicalGradient.setCenter(ptCenter);
        conicalGradient.setAngle(45);
        conicalGradient.setColorAt (0,QColor (Qt::green));
        conicalGradient.setColorAt (1,QColor (Qt::red));

        QBrush brush(conicalGradient);
        painter.setBrush (brush);
        int r = qMin(rcConical1.width(), rcConical1.height());
        painter.drawEllipse(ptCenter, r / 2, r / 2);
    }

    // 默认
    QRect rcConical2 = rcConical;
    rcConical2.setLeft(rcConical1.right());
    rcConical2.setWidth(rcConical.width() / 2);
    rcConical2 = rcConical2.marginsRemoved(QMargins(10, 10, 10, 10));

    {
        QPoint ptCenter = rcConical2.center();
        QConicalGradient conicalGradient;
        conicalGradient.setCenter(ptCenter);
        conicalGradient.setAngle(45);
        conicalGradient.setColorAt (0,QColor (Qt::green));
        conicalGradient.setColorAt (0.2,QColor (Qt::red));
        conicalGradient.setColorAt (0.4,QColor (Qt::blue));
        conicalGradient.setColorAt (0.6,QColor (Qt::cyan));
        conicalGradient.setColorAt (0.8,QColor (Qt::magenta));
        conicalGradient.setColorAt (1,QColor (Qt::green));

        QBrush brush(conicalGradient);
        painter.setBrush (brush);
        int r = qMin(rcConical2.width(), rcConical2.height());
        painter.drawEllipse(ptCenter, r / 2, r / 2);
    }
}
