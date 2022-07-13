#ifndef NAVBUTTON_H
#define NAVBUTTON_H

#include <QPushButton>
#include <QVariant>

class NavButton : public QPushButton
{
    Q_OBJECT
public:
    explicit NavButton(QWidget *parent = nullptr);
    void SetData(QString strTitle, QChar icon, QVariant data) { m_data = data; m_strTitle = strTitle; m_icon = icon; update(); }

private:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mouseReleaseEvent(QMouseEvent *e);

signals:
    void clicked(QVariant data);

public slots:

private:
    struct TGradientCfg
    {
       QColor start;
       QColor end;

       TGradientCfg(QColor _start, QColor _end)
       {
           start = _start;
           end = _end;
       }
    };

    QVariant m_data;
    QString m_strTitle;
    QChar m_icon;

    bool m_bEnter;
    QColor m_colorBg;

    QList<TGradientCfg> m_listColor;
    int m_nColorIndex;
};

#endif // NAVBUTTON_H
