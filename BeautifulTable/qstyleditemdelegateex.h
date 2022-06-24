#ifndef QSTYLEDITEMDELEGATEEX_H
#define QSTYLEDITEMDELEGATEEX_H

#include <QStyledItemDelegate>

class QStyledItemDelegateEx : public QStyledItemDelegate
{
    Q_OBJECT
public:
    QStyledItemDelegateEx(QWidget *parent = 0);
    ~QStyledItemDelegateEx();

private:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
};

#endif // QSTYLEDITEMDELEGATEEX_H
