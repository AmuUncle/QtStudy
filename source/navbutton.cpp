#include "navbutton.h"
#include <QPainter>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QDebug>

#include "iconhelper.h"

int g_index = 0;

NavButton::NavButton(QWidget *parent) : QPushButton(parent)
{
    m_bEnter = false;
    m_colorBg = QColor("#1570a5");
    setFixedHeight(50);
    setCursor(Qt::PointingHandCursor);

    m_listColor << TGradientCfg(QColor("#5dbdd7"), QColor("#4097b8"))
                   << TGradientCfg(QColor("#ec9173"), QColor("#d17c71"))
                   << TGradientCfg(QColor("#d5a664"), QColor("#ba864d"))
                   << TGradientCfg(QColor("#1a80b3"), QColor("#0f6fb0"))
                   << TGradientCfg(QColor("#4750b4"), QColor("#433d9b"))
                   << TGradientCfg(QColor("#32c47f"), QColor("#35bd72"))
                   << TGradientCfg(QColor("#31c59a"), QColor("#3ac89a"));

    m_nColorIndex = (g_index++) % m_listColor.length();
}

void NavButton::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    QRect rcClient = rect();

    QLinearGradient linearGradient(0, rcClient.center().y(), rcClient.width(), rcClient.center().y());
    linearGradient.setColorAt(0, m_listColor.at(m_nColorIndex).start);
    linearGradient.setColorAt(1, m_listColor.at(m_nColorIndex).end);

    //画刷设置渐变
    QBrush brush(linearGradient);
    painter.setBrush(brush);
    painter.drawRect(rcClient);

    painter.scale(rcClient.width() / 160.0, rcClient.height() / 90.0);

    QRect rcIcon(0, 0, 160, 70);

    painter.save();
    QFont ftTmp = IconHelper::GetFont();
    ftTmp.setPixelSize(m_bEnter ? 60 : 50);
    painter.setFont(ftTmp);
    painter.setPen(QColor("#f1f1f1"));
    painter.drawText(rcIcon, Qt::AlignCenter, m_icon);
    painter.restore();

    painter.save();
    QRect rcText(0, 0, 160, 80);
    painter.setPen(QColor("#f1f1f1"));
    painter.drawText(rcText, Qt::AlignHCenter | Qt::AlignBottom, m_strTitle);
    painter.restore();
}

void NavButton::resizeEvent(QResizeEvent *event)
{
    //当前宽高
    int nWidth = event->size().width();

    setFixedHeight(nWidth * 9.0 / 16.0);
}

void NavButton::enterEvent(QEvent *event)
{
    m_bEnter = true;
    m_colorBg = QColor("#157000");
    update();
}

void NavButton::leaveEvent(QEvent *event)
{
    m_bEnter = false;
    m_colorBg = QColor("#1570a5");
    update();
}

void NavButton::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
       emit clicked(m_data);
    }
}
