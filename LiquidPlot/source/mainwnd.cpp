#include "mainwnd.h"
#include <QGridLayout>
#include "liquidplot.h"
#include <QVariant>
#include <QSlider>
#include <QMouseEvent>
#include <QColorDialog>
#include <QDebug>
#include <QPropertyAnimation>


MainWnd::MainWnd(QWidget *parent) : QWidget(parent)
{
    setProperty("canMove", "true");
    setMinimumSize(400, 400);
    CreateAllChildWnd();
    InitCtrl();
    InitSolts();
    Relayout();
}

void MainWnd::CreateAllChildWnd()
{

}

void MainWnd::InitCtrl()
{

}

void MainWnd::InitSolts()
{

}

void MainWnd::Relayout()
{
    LiquidPlot *pLiquidPlot1 = new LiquidPlot(this);
    LiquidPlot *pLiquidPlot2 = new LiquidPlot(this);
    LiquidPlot *pLiquidPlot3 = new LiquidPlot(this);
    LiquidPlot *pLiquidPlot4 = new LiquidPlot(this);

    pLiquidPlot2->SetWaterColor(QColor("#DEAF39"));
    pLiquidPlot3->SetWaterColor(QColor("#FF6B6B"));
    pLiquidPlot4->SetWaterColor(QColor("#18BD9B"));

    pLiquidPlot2->SetPlotType(LiquidPlot::PLOTTYPE_RECTANGLE);
    pLiquidPlot3->SetPlotType(LiquidPlot::PLOTTYPE_DIAMOND);
    pLiquidPlot4->SetPlotType(LiquidPlot::PLOTTYPE_RECTANGLE);

    pLiquidPlot1->EnableSpace(false);
    pLiquidPlot4->EnableSpace(false);

    pLiquidPlot1->installEventFilter(this);
    pLiquidPlot2->installEventFilter(this);
    pLiquidPlot3->installEventFilter(this);
    pLiquidPlot4->installEventFilter(this);

    // 滑动条
    QSlider *pSlider = new QSlider(this);
    connect(pSlider, &QSlider::valueChanged, pLiquidPlot1, &LiquidPlot::SetPercent);
    connect(pSlider, &QSlider::valueChanged, pLiquidPlot2, &LiquidPlot::SetPercent);
    connect(pSlider, &QSlider::valueChanged, pLiquidPlot3, &LiquidPlot::SetPercent);
    connect(pSlider, &QSlider::valueChanged, pLiquidPlot4, &LiquidPlot::SetPercent);

    pSlider->setOrientation(Qt::Horizontal);
    pSlider->setMinimum(0);
    pSlider->setMaximum(100);
    pSlider->setSingleStep(1);
    pSlider->setValue(50);

    QSlider *pSliderSpeed = new QSlider(this);
    connect(pSliderSpeed, &QSlider::valueChanged, pLiquidPlot1, &LiquidPlot::SetSpeed);
    connect(pSliderSpeed, &QSlider::valueChanged, pLiquidPlot2, &LiquidPlot::SetSpeed);
    connect(pSliderSpeed, &QSlider::valueChanged, pLiquidPlot3, &LiquidPlot::SetSpeed);
    connect(pSliderSpeed, &QSlider::valueChanged, pLiquidPlot4, &LiquidPlot::SetSpeed);

    pSliderSpeed->setOrientation(Qt::Horizontal);
    pSliderSpeed->setMinimum(1);
    pSliderSpeed->setMaximum(100);
    pSliderSpeed->setSingleStep(1);
    pSliderSpeed->setValue(10);

    setProperty("RotationAngle", 1.0);
    QPropertyAnimation *m_pAnimationOpacity = new QPropertyAnimation(this, "RotationAngle");
    m_pAnimationOpacity->setDuration(1000);
    m_pAnimationOpacity->setEasingCurve(QEasingCurve::OutCubic);  // 缓和曲线风格
    m_pAnimationOpacity->setStartValue(0.0);
    m_pAnimationOpacity->setEndValue(50.0);

    connect(m_pAnimationOpacity, &QPropertyAnimation::valueChanged, [=](const QVariant &value)
    {
        pLiquidPlot1->SetPercent(value.toInt());
        pLiquidPlot2->SetPercent(value.toInt());
        pLiquidPlot3->SetPercent(value.toInt());
        pLiquidPlot4->SetPercent(value.toInt());
    });

    m_pAnimationOpacity->start();

    QGridLayout *layoutMain = new QGridLayout(this);
    layoutMain->addWidget(pLiquidPlot1, 0, 0);
    layoutMain->addWidget(pLiquidPlot2, 0, 1);
    layoutMain->addWidget(pLiquidPlot3, 1, 0);
    layoutMain->addWidget(pLiquidPlot4, 1, 1);

    layoutMain->addWidget(pSlider, 2, 0, 1, 2);
    layoutMain->addWidget(pSliderSpeed, 3, 0, 1, 2);

    layoutMain->setMargin(8);
}

bool MainWnd::eventFilter(QObject *watched, QEvent *evt)
{
    QWidget *w = (QWidget *)watched;
    if (!w->property("LiquidPlot").toBool()) {
        return QObject::eventFilter(watched, evt);
    }

    QMouseEvent *event = static_cast<QMouseEvent *>(evt);
    if (event->type() == QEvent::MouseButtonDblClick)
    {
        if (event->button() == Qt::LeftButton)
        {
            LiquidPlot *pLiquidPlot = (LiquidPlot *)w;

            QColorDialog color;//调出颜色选择器对话框
            bool ok = false;
            QColor c = color.getRgba(pLiquidPlot->GetWaterColor().rgba(), &ok);
            if (ok)
            {
                pLiquidPlot->SetWaterColor(c);
            }

            return true;
        }
    }

    return QObject::eventFilter(watched, evt);
}
