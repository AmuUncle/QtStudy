#ifndef GRADIENT_H
#define GRADIENT_H

#include <QWidget>

class Gradient : public QWidget
{
    Q_OBJECT
public:
    explicit Gradient(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent *event);
};

#endif // GRADIENT_H
