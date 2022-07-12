#include "dragverify.h"

const int SLIDER_WIDTH = 60;
const int SLIDER_HEIGHT = 40;

DragVerify::DragVerify(QWidget *parent, EType eType/* = TYPE_ROTATE*/) : QWidget(parent)
{
    m_eType = eType;

    setAttribute(Qt::WA_TranslucentBackground, true);

    m_pixBase.load(":/captcha/test.jpeg");

    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    m_nRandRote = qrand() % 360;

    m_pSlider = new QSlider(this);
    m_pSlider->setRange(0, 360);
    m_pSlider->setOrientation(Qt::Horizontal);  // 水平方向

    QVBoxLayout *layoutMain = new QVBoxLayout();
    layoutMain->addStretch();
    layoutMain->addWidget(m_pSlider);
    layoutMain->setMargin(8);
    setLayout(layoutMain);


    connect(m_pSlider, &QSlider::valueChanged, [=](int value) {
        update();
    });

    connect(m_pSlider, &QSlider::sliderReleased, [=]() {

        switch (m_eType)
        {
        case TYPE_ROTATE:
            {
                bool bSuc = ((m_pSlider->value() + m_nRandRote) % 360 > 350 || (m_pSlider->value() + m_nRandRote) % 360 < 10);
                if (bSuc)
                {

                }
                else
                {
                   m_pSlider->setValue(0);
                }

                emit bSuc ? SignalVerifySuc() : SignalVerifyFail();
            }
            break;

        case TYPE_JIGSAW:
            {
                bool bSuc = qAbs(m_nRandOffset - ((double)m_pSlider->value() / 360.0 * (m_rcImg.width() - SLIDER_WIDTH))) < 2;
                if (bSuc)
                {

                }
                else
                {
                   m_pSlider->setValue(0);
                }

                emit bSuc ? SignalVerifySuc() : SignalVerifyFail();
            }
            break;

        default:
            break;
        }

        update();
    });
}

void DragVerify::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);         // 创建画家对象
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing, true); // 抗锯齿和使用平滑转换算法

    switch (m_eType)
    {
    case TYPE_ROTATE:
        {
            QPainterPath path;
            path.addEllipse(m_rcImg.left(), m_rcImg.top(), m_rcImg.width(), m_rcImg.height());//添加一个圆
            painter.setClipPath(path);

            painter.save();
            painter.translate(m_rcImg.center());

            QRect rcAvatarBg(0 - m_rcImg.width() / 2, 0 - m_rcImg.height() / 2, m_rcImg.width(), m_rcImg.height());
            painter.rotate(m_nRandRote + m_pSlider->value());
            QPixmap pImgEyeWriteRight = m_pixBase.scaled(rcAvatarBg.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            painter.drawPixmap(0 - m_rcImg.width() / 2, 0 - m_rcImg.height() / 2, pImgEyeWriteRight);
            painter.restore();
        }
        break;

    case TYPE_JIGSAW:
        {
            QRect rcAvatarBg(m_rcImg);
            QPixmap pImgEyeWriteRight = m_pixBase.scaled(rcAvatarBg.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            painter.drawPixmap(m_rcImg.left(), m_rcImg.top(), pImgEyeWriteRight);

            QRect rcWirteBg(m_rcImg.left() + m_nRandOffset, m_rcImg.top() + 50, SLIDER_WIDTH, SLIDER_HEIGHT);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(255, 255, 255, 200));
            painter.drawRoundedRect(rcWirteBg, 1, 1);

            QPixmap pImg = pImgEyeWriteRight.copy(m_nRandOffset, 50, SLIDER_WIDTH, SLIDER_HEIGHT);

            painter.drawPixmap(m_rcImg.left() + ((double)m_pSlider->value() / 360.0 * (m_rcImg.width() - SLIDER_WIDTH)), m_rcImg.top() + 50, pImg);
        }
        break;

    default:
        break;
    }
}

void DragVerify::resizeEvent(QResizeEvent *event)
{
    QRect rcClient = rect();
    QRect rcImg = rcClient.marginsRemoved(QMargins(40, 40, 40, 40));
    QPoint ptCenter = rcClient.center();
    int len = qMin(rcImg.width(), rcImg.height());
    len /= 2;
    m_rcImg = QRect(ptCenter.x() - len, ptCenter.y() - len, len * 2, len * 2);


    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    m_nRandOffset = qrand() % (m_rcImg.width() - SLIDER_WIDTH * 2 - 10) + SLIDER_WIDTH;
    m_nRandRote = qrand() % 360;
    m_pSlider->setValue(0);
    update();
}
