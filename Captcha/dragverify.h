#ifndef DRAGVERIFY_H
#define DRAGVERIFY_H

#include <QWidget>
#include <QtWidgets>

class DragVerify : public QWidget
{
    Q_OBJECT
public:
    enum EType
    {
        TYPE_ROTATE = 0,
        TYPE_JIGSAW = 1,
    };

    explicit DragVerify(QWidget *parent = 0, EType eType = TYPE_ROTATE);

public:
    void SetType(EType eType) { m_eType = eType; update(); }

private:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

signals:
    void SignalVerifySuc();
    void SignalVerifyFail();

private:
    EType m_eType;
    QPixmap m_pixBase;

    QSlider *m_pSlider;

    int m_nRandRote;
    int m_nRote;

    int m_nRandOffset;
    QRect m_rcImg;
};

#endif // DRAGVERIFY_H
