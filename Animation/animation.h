#ifndef ANIMATION_H
#define ANIMATION_H

#include <QWidget>

class Animation : public QWidget
{
    Q_OBJECT
public:
    explicit Animation(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent *event);

private slots:
    void OnSignalPushedMapped(int nCmd);
};

#endif // ANIMATION_H
