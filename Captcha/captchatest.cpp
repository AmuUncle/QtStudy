#include "captchatest.h"
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

#include "captcha.h"
#include "dragverify.h"

#include "global.h"

CaptchaTest::CaptchaTest(QWidget *parent) : QWidget(parent)
{
    m_lineEdit = new QLineEdit(this);
    m_pCaptcha = new Captcha(this);
    m_btnOk = new QPushButton("忽略大小写", this);
    m_btnOk1 = new QPushButton("匹配大小写", this);
    m_pDragVerify1 = new DragVerify(this);
    m_pDragVerify2 = new DragVerify(this, DragVerify::TYPE_JIGSAW);

    QHBoxLayout *hDragVerifyLayout = new QHBoxLayout();
    hDragVerifyLayout->addWidget(m_pDragVerify1);
    hDragVerifyLayout->addWidget(m_pDragVerify2);

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(m_lineEdit);
    hLayout->addWidget(m_pCaptcha);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(new QLabel("图片验证码"));
    mainLayout->addLayout(hDragVerifyLayout);
    mainLayout->addWidget(new QLabel("字符验证码"));
    mainLayout->addLayout(hLayout);
    mainLayout->addWidget(m_btnOk);
    mainLayout->addWidget(m_btnOk1);

    setLayout(mainLayout);

    m_pCaptcha->setFixedSize(200, 60);

    connect(m_btnOk, &QPushButton::clicked, this, &CaptchaTest::OnOk);
    connect(m_btnOk1, &QPushButton::clicked, this, &CaptchaTest::OnOk1);


    connect(m_pDragVerify1, &DragVerify::SignalVerifySuc, [=]() {
        QMessageBox::information(NULL, "提示", "验证成功");
    });

    connect(m_pDragVerify2, &DragVerify::SignalVerifySuc, [=]() {
        QMessageBox::information(NULL, "提示", "验证成功");
    });

    connect(m_pDragVerify1, &DragVerify::SignalVerifyFail, [=]() {
        QMessageBox::information(NULL, "提示", "验证失败");
    });

    connect(m_pDragVerify2, &DragVerify::SignalVerifyFail, [=]() {
        QMessageBox::information(NULL, "提示", "验证失败");
    });
}

void CaptchaTest::OnOk()
{
    bool bRet = m_pCaptcha->Check(m_lineEdit->text());
    if (bRet)
    {
       QMessageBox::information(this, "提示", "验证成功");
    }
    else
    {
        QMessageBox::warning(this, "提示", "验证失败");
    }
}

void CaptchaTest::OnOk1()
{
    bool bRet = m_pCaptcha->Check(m_lineEdit->text(), false);
    if (bRet)
    {
       QMessageBox::information(this, "提示", "验证成功");
    }
    else
    {
        QMessageBox::warning(this, "提示", "验证失败");
    }
}
