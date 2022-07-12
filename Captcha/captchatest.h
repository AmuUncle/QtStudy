#ifndef CAPTCHATEST_H
#define CAPTCHATEST_H

#include <QWidget>

class QLineEdit;
class QPushButton;
class Captcha;
class DragVerify;
class CaptchaTest : public QWidget
{
    Q_OBJECT
public:
    explicit CaptchaTest(QWidget *parent = nullptr);

private slots:
    void OnOk();
    void OnOk1();

private:
    QLineEdit *m_lineEdit;
    QPushButton *m_btnOk;
    QPushButton *m_btnOk1;
    Captcha *m_pCaptcha;

    DragVerify *m_pDragVerify1;
    DragVerify *m_pDragVerify2;
};

#endif // CAPTCHATEST_H
