#ifndef IMAGECONVERT_H
#define IMAGECONVERT_H

#include <QWidget>

class ImageConvert : public QWidget
{
    Q_OBJECT
public:
    explicit ImageConvert(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent *event);
};

#endif // IMAGECONVERT_H
