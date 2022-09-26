#ifndef EPUSHBUTTON_H
#define EPUSHBUTTON_H

#include <QWidget>

class EPushButton : public QWidget
{
    Q_OBJECT
public:
    explicit EPushButton(QWidget *parent = 0);


private:
    QSize sizeHint() const;

    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);


signals:

public slots:

private:
    int m_nTimeId;
    int m_nRotate;
};

#endif // EPUSHBUTTON_H
