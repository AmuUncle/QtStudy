#include "captcha.h"
#include <QPainter>
#include <QDebug>
#include <QTime>

Captcha::Captcha(QWidget *parent, bool bChinese/* = false*/) : QWidget(parent)
{
    m_dwLen = 4;
    m_bInit = false;
    m_bChinese = bChinese;
}

/** 随机生成汉字
* @brief getRandChinese
* @param count  生成个数
* @return
*/
QString Captcha::MakeRandChinese(int length)
{
  if (length<=0) return QString();

  int rand1 = 0xf7 - 0xb0;
  int rand2 = 0xfe - 0xa1;
  QString text;
  srand(time(NULL));
  for (int i = 0; i < length; ++i)
  {
      QByteArray byte1, byte2;
      QByteArray str;
      byte1.append(rand() % rand1 + 0xb0);
      byte2.append(rand() % rand2 + 0xa1);
      str = byte1;
      str += byte2;
      text += QString::fromLocal8Bit(str);
  }
  return text;
}

QString Captcha::MakeRandomString(int length)
{
    if (m_bChinese)
    {
        return MakeRandChinese(length);
    }

    QString result;

    qsrand(QDateTime::currentMSecsSinceEpoch());//随机数种子

    QString array_str = "abcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    int array_size = array_str.length();
    int idx = 0;
    for (int i = 0; i < length; ++i)
    {
        idx = qrand() % (array_size - 1);
        QChar ch = array_str.at(idx);
        result.append(ch);
    }

    return result;
}

void Captcha::Generate(uint dwLen)
{
    m_dwLen = dwLen;
    m_strCode = MakeRandomString(dwLen);
    int nCodeWidth = width() / m_dwLen;

    m_listCode.clear();
    foreach (QChar chCode, m_strCode)
    {
        TCode code;
        code.chCode = chCode;
        code.color = QColor(qrand() % 255, qrand() % 255, qrand() % 255);
        code.bBold = (qrand() % 10) > 5;
        code.size = 12 + (qrand() % 5);
        code.rotate = ((qrand() % 2) > 0) ? (qrand() % 80) : (qrand() % -80);

        m_listCode.append(code);
    }

    m_listLine.clear();
    for (int i = 0; i < m_dwLen; i++)
    {
        TLine line;
        line.color = QColor(qrand() % 255, qrand() % 255, qrand() % 255);
        line.line = QLine(i * nCodeWidth +  qrand() % nCodeWidth,
                           qrand() % height(),
                           (m_dwLen - 1) * nCodeWidth + qrand() % nCodeWidth,
                           qrand() % height());

        m_listLine.append(line);
    }

    m_listPoint.clear();
    for (int i = 0; i < width(); i+=2)
    {
        TPoint point;
        point.color = QColor(qrand() % 255, qrand() % 255, qrand() % 255);
        point.point = QPoint(i, qrand() % height());

        m_listPoint.append(point);
    }
}

bool Captcha::Check(QString strCode, bool ignoreCase/* = true*/)
{
    if (!ignoreCase)
        return strCode == m_strCode;

    return strCode.toLower() == m_strCode.toLower();
}

void Captcha::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    QRect rcClient = rect();

    painter.setBrush(QColor("#c6c9e4"));
    painter.drawRect(rcClient);

    QRect rcDraw = rcClient;
    rcDraw = rcClient.marginsRemoved(QMargins(5, 5, 5, 5));

    int nCodeWidth = rcClient.width() / m_dwLen;

    QRect rcCode = rcDraw;
    int i = 0;

    qsrand(QTime::currentTime().msec()); //以当前时间ms为随机种子

    foreach (TCode code, m_listCode)
    {
        rcCode.setLeft(rcDraw.left() + nCodeWidth * i);
        rcCode.setWidth(nCodeWidth);

        painter.save();
        painter.translate(rcCode.center());
        painter.rotate(code.rotate);

        QFont ftTmp1 = painter.font();
        ftTmp1.setBold(code.bBold);
        ftTmp1.setPointSize(code.size);
        painter.setFont(ftTmp1);

        painter.setPen(code.color);
        painter.drawText(-nCodeWidth / 2, -nCodeWidth / 2, nCodeWidth, nCodeWidth, Qt::AlignCenter, code.chCode);
        painter.restore();

        i++;
    }

    painter.save();
    foreach (TLine line, m_listLine)
    {
        painter.setPen(line.color);
        painter.drawLine(line.line);
    }
    painter.restore();

    painter.save();
    foreach (TPoint point, m_listPoint)
    {
        painter.setPen(point.color);
        painter.drawPoint(point.point);
    }
    painter.restore();
}

void Captcha::mouseReleaseEvent(QMouseEvent *event)
{
    Generate(m_dwLen);
    update();
}

void Captcha::resizeEvent(QResizeEvent *event)
{
    if (m_bInit)
        return;

    Generate(m_dwLen);
    update();
    m_bInit = true;
}
