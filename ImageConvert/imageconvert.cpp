#include "imageconvert.h"
#include <QPainter>
#include <QDebug>


ImageConvert::ImageConvert(QWidget *parent) : QWidget(parent)
{

}

static char toChar(int g)
{
    if (g <= 30) {
        return 'A';
    }
    else if (g > 30 && g <= 60) {
        return 'B';
    }
    else if (g > 60 && g <= 120) {
        return 'C';
    }
    else if (g > 120 && g <= 150) {
        return 'D';
    }
    else if (g > 150 && g <= 180) {
        return 'E';
    }
    else if (g > 180 && g <= 210) {
        return 'F';
    }
    else {
        return ' ';
    }
}

static int rgbtoGray(int r, int g, int b)
{
    int a = (int)((((quint32)((r << 5) + (r << 2) + (r << 1))) + (quint32)((g << 6) + (g << 3) + (g << 1) + g)
        + (quint32)((b << 4) - b)) >> 7);

    return a;
}

static QImage setImage(const QImage &image)
{
    const int ih = image.height();
    const int iw = image.width();

    QPixmap txtImage(iw, ih);
    txtImage.fill(Qt::transparent);
    QPainter painter(&txtImage);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QColor::fromHsl(rand() % 360, rand() % 256, rand() % 200));
    QFont font = painter.font();
    font.setPixelSize(7);
    font.setFamily("Microsoft YaHei");
    painter.setFont(font);

    for (int i = 0; i < iw; i+= 7)
    {
        for (int j = 0; j < ih; j+= 7)
        {
            const QRgb&& pixel = image.pixel(i, j);
            int r = qRed(pixel);
            int g = qGreen(pixel);
            int b = qBlue(pixel);
            int gray = rgbtoGray(r, g, b);
            char c = toChar((int)gray);
            painter.drawText(i, j, QChar(c));
        }
    }
    return txtImage.toImage();
}

void ImageConvert::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    QRect rcClient = rect();

    QImage img(":/app_logo.png");

    ///////////////////////负片效果///////////////////////////////////////////
    QRect rcImg1 = rcClient;
    rcImg1 = rcImg1.marginsRemoved(QMargins(10, 10, 10, 10));
    rcImg1.setBottom(rcImg1.top() + rcImg1.height() / 2);
    rcImg1.setRight(rcImg1.left() + rcImg1.width() / 2);

    {
        QImage newImg(img.width(),img.height(),img.format());
        for(int i=0;i<img.width();++i)
        {
            for(int j=0;j<img.height();++j)
            {
                QRgb rgb = img.pixel(i,j);
                QRgb newRgb = qRgb(255 - qRed(rgb),255 -qGreen(rgb),255 -qBlue(rgb));
                newImg.setPixel(i,j,newRgb);
            }
        }
        QPoint ptCenter = rcImg1.center();
        int r = qMin(rcImg1.width(), rcImg1.height());

        //QImage result = newImg.scaled(r, r, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        painter.drawImage(QRect(ptCenter.x() - r / 2, ptCenter.y() - r / 2, r, r), newImg);
    }

    {
        ///////////////////////灰度图///////////////////////////////////////////
        QRect rcImg2 = rcClient;
        rcImg2 = rcImg2.marginsRemoved(QMargins(10, 10, 10, 10));
        rcImg2.setBottom(rcImg1.top() + rcImg2.height() / 2);
        rcImg2.setLeft(rcImg1.right());

        QImage grayImg(img.width(),img.height(),img.format());
        for(int i=0;i<img.width();++i)
        {
            for(int j=0;j<img.height();++j)
            {
                QColor clr = img.pixelColor(i,j);
                int gray = 0.3*clr.red() + 0.59*clr.green() + 0.11*clr.blue();
                grayImg.setPixelColor(i,j,QColor(gray, gray, gray, clr.alpha()));
            }
        }

        QPoint ptCenter = rcImg2.center();
        int r = qMin(rcImg2.width(), rcImg2.height());
        painter.drawImage(QRect(ptCenter.x() - r / 2, ptCenter.y() - r / 2, r, r), grayImg);
    }

    {
        ///////////////////////倒影///////////////////////////////////////////
        QRect rcImg3 = rcClient;
        rcImg3 = rcImg3.marginsRemoved(QMargins(10, 10, 10, 10));
        rcImg3.setTop(rcImg1.bottom() + 20);
        rcImg3.setRight(rcImg3.left() + rcImg3.width() / 2);

        QImage mirrImg = img.mirrored();

        QPoint ptCenter = rcImg3.center();
        int r = qMin(rcImg3.width(), rcImg3.height());
        r = r / 2;

        painter.save();
        painter.drawImage(QRect(ptCenter.x() - r / 2, ptCenter.y() - r, r, r), img);
        painter.setOpacity(0.3);
        painter.drawImage(QRect(ptCenter.x() - r / 2, ptCenter.y() + 2, r, r), mirrImg);
        painter.restore();
    }

    {
        ///////////////////////像素风///////////////////////////////////////////
        QRect rcImg4 = rcClient;
        rcImg4 = rcImg4.marginsRemoved(QMargins(10, 10, 10, 10));
        rcImg4.setTop(rcImg1.bottom() + 20);
        rcImg4.setLeft(rcImg1.right());

        QImage mirrImg = setImage(img);
        QPoint ptCenter = rcImg4.center();
        int r = qMin(rcImg4.width(), rcImg4.height());
        painter.drawImage(QRect(ptCenter.x() - r / 2, ptCenter.y() - r / 2, r, r), mirrImg);
    }
}
