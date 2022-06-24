#include "incofontpane.h"
#include <QVBoxLayout>
#include "iconhelper.h"
#include <QFile>
#include <QDebug>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>
#include <QApplication>
#include <QClipboard>


CIconFontPane::CIconFontPane(QWidget *parent) : QWidget(parent)
{
   m_pScrollArea = NULL;
   m_widgetContent = NULL;

   IconHelper::Load();

   CreateAllChildWnd();
   InitCtrl();
   InitSolts();
   Relayout();

   ParseJsonFile();
}

void CIconFontPane::OnBtnClicked()
{
}

void CIconFontPane::CreateAllChildWnd()
{
#define NEW_OBJECT(pObj, TYPE) \
    if (NULL == pObj) { pObj = new TYPE(this); }

    NEW_OBJECT(m_pScrollArea, QScrollArea);
    NEW_OBJECT(m_widgetContent, QWidget);
}

void CIconFontPane::InitCtrl()
{
    m_pScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏横向滚动条
    m_pScrollArea->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    m_pScrollArea->setWidgetResizable(true);
    m_pScrollArea->setWidget(m_widgetContent);
    m_pScrollArea->setStyleSheet(".QScrollArea{border-style:none;}");

    m_widgetContent->setAttribute(Qt::WA_StyledBackground);
    m_widgetContent->setProperty("form", "iconfontpane");
}

void CIconFontPane::InitSolts()
{

}

void CIconFontPane::Relayout()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_pScrollArea);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
}

void CIconFontPane::ParseJsonFile()
{
    int nRow = 0;
    int nCol = 0;
    QGridLayout *layoutMain = new QGridLayout(m_widgetContent);

    QFile file(":/iconfont.json");
    if (file.open(QFile::ReadOnly))
    {
        QByteArray json = file.readAll();
        file.close();

        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(json, &jsonError);
        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
        {
            if (doucment.isObject())
            {
                QJsonObject object = doucment.object();
                if(object.contains("glyphs"))
                {
                    QJsonValue glyphs = object.value("glyphs");
                    if (glyphs.isArray())
                    {
                        QJsonArray arrIcon = glyphs.toArray();
                        for (int i = 0; i < arrIcon.size(); i++)
                        {
                            QJsonValue jsonIcon = arrIcon.at(i);

                            if (jsonIcon.isObject())
                            {
                                QJsonObject objectIcon = jsonIcon.toObject();

                                QChar cUnicode = objectIcon.take("unicode_decimal").toInt();
                                QString strUnicodeHex = objectIcon.take("unicode").toString();
                                QString strName = objectIcon.take("name").toString();

                                QPushButton *btn = new QPushButton(m_widgetContent);
                                IconHelper::SetIcon(btn, cUnicode, 30);
                                btn->setProperty("IconfontBtn", "true");   // iconfont 单独为一类别
                                btn->setToolTip(QString("%1(0x%2)").arg(strName).arg(strUnicodeHex));
                                layoutMain->addWidget(btn, nRow, nCol);

                                nCol++;

                                if (nCol > 4)
                                {
                                    nRow++;
                                    nCol = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
