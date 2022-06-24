#include "qstyleditemdelegateex.h"
#include <QCheckBox>
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

QStyledItemDelegateEx::QStyledItemDelegateEx(QWidget *parent)
    : QStyledItemDelegate(parent)
{

}

QStyledItemDelegateEx::~QStyledItemDelegateEx()
{

}

void QStyledItemDelegateEx::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

    QStyledItemDelegate::paint(painter, viewOption, index);

    if (index.column() == 0)
    {
        bool data = index.model()->data(index, Qt::UserRole).toBool();
        QStyleOptionButton checkBoxStyle;
        checkBoxStyle.state = data ? QStyle::State_On : QStyle::State_Off;
        checkBoxStyle.state |= QStyle::State_Enabled;
        checkBoxStyle.iconSize = QSize(20, 20);
        checkBoxStyle.rect = option.rect;
        QCheckBox checkBox;
        QApplication::style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &checkBoxStyle, painter, &checkBox);
    }
    else if (index.column() == 2)
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        if(index.row()%2){
            painter->setBrush(QColor("#f0f9eb"));
        }else{
            painter->setBrush(QColor("#ebf5ff"));
        }

        QPoint ct = option.rect.center();
        QRect rc = QRect(ct.x() - 14, ct.y() - 10, 28, 20);
        painter->drawRoundedRect(rc, 5, 5);

        if(index.row()%2){
            painter->setPen(QColor("#e1f3d8"));
        }else{
            painter->setPen(QColor("#d8ebff"));
        }
        painter->setBrush(Qt::NoBrush);
        painter->drawRoundedRect(rc, 5, 5);

        if(index.row()%2){
            painter->setPen(QColor("#67c23a"));
            painter->drawText(rc, Qt::AlignCenter, "女");
        }else{
            painter->setPen(QColor("#409dfe"));
            painter->drawText(rc, Qt::AlignCenter, "男");
        }
        painter->restore();
    }
    else if (index.column() == 5)
    {
        painter->save();
        painter->setPen(QColor("#409dfe"));

        int data = index.model()->data(index, Qt::UserRole + 1).toInt();

        QStyleOptionProgressBarV2* progressOpt = new QStyleOptionProgressBarV2();
        progressOpt->rect = option.rect.adjusted(5,5,-5,-5);
        progressOpt->minimum = 0;
        progressOpt->maximum = 100;
        progressOpt->progress = data;  //设置固定的值
        progressOpt->text = QString("%1%").arg(data);
        progressOpt->textVisible = true;
        progressOpt->textAlignment = Qt::AlignCenter;
        QApplication::style()->drawControl(QStyle::CE_ProgressBar,progressOpt,painter);
        painter->restore();
    }
    else if (index.column() == 7)
    {
        painter->save();

        QString data = index.model()->data(index, Qt::UserRole + 1).toString();
        painter->setPen(QColor("#409dfe"));
        QStyleOptionButton buttonOpt;
        buttonOpt.rect = option.rect.adjusted(5,5,-5,-5);
        buttonOpt.text = data;// 设置文本
        buttonOpt.state |= QStyle::State_Enabled;

        if(option.state & QStyle::State_MouseOver)
        {
            buttonOpt.state |= QStyle::State_MouseOver;
        }
        else
        {
            buttonOpt.state |= QStyle::State_Sunken;

        }
        // 绘制QStyle::CE_PushButtonLabel
        QApplication::style()->drawControl(QStyle::CE_PushButtonLabel, &buttonOpt, painter);

        painter->restore();
    }
}

// 响应鼠标事件，更新数据
bool QStyledItemDelegateEx::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    qDebug() << event->type();
    QRect decorationRect = option.rect;

    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    if (event->type() == QEvent::MouseButtonPress && decorationRect.contains(mouseEvent->pos()))
    {
        if (index.column() == 0)
        {
            bool data = model->data(index, Qt::UserRole).toBool();
            model->setData(index, !data, Qt::UserRole);
        }
    }

    if (event->type() == QEvent::HoverEnter)
    {
        if (index.column() == 7)
        {
            model->setData(index, "123", Qt::UserRole + 1);
        }
    }


    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
