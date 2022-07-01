#ifndef CIRCULARDIAL_H
#define CIRCULARDIAL_H

#include <QWidget>
#include <QLabel>

class CircularDial : public QWidget
{
    Q_OBJECT
public:
    explicit CircularDial(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    int m_nLow;
    int m_nHigh;
    int m_nMax;

    QFont m_iconFont;
};

#endif // CIRCULARDIAL_H
