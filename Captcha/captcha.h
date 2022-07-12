#ifndef CAPTCHA_H
#define CAPTCHA_H

#include <QWidget>
#include <QList>

class Captcha : public QWidget
{
    Q_OBJECT
public:
    explicit Captcha(QWidget *parent = nullptr, bool bChinese = false);
    void Generate(uint dwLen = 4);
    bool Check(QString strCode, bool ignoreCase = true);

private:
    struct TCode
    {
        QChar chCode;
        QColor color;
        bool bBold;
        int size;
        int rotate;
    };

    struct TLine
    {
        QLine line;
        QColor color;
    };

    struct TPoint
    {
        QPoint point;
        QColor color;
    };

    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

    QString MakeRandomString(int length);
    QString MakeRandChinese(int length);

private:
    QList<TCode> m_listCode;
    QList<TLine> m_listLine;
    QList<TPoint> m_listPoint;

    uint m_dwLen;
    QString m_strCode;

    bool m_bInit;
    bool m_bChinese;
};

#endif // CAPTCHA_H
