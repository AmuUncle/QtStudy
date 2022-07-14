#include "layout.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "global.h"

Layout::Layout(int nStyleType, QWidget *parent) : QWidget(parent)
{
    m_nStyleType = nStyleType;

    setObjectName("Layout");
    m_pHeader = new QLabel("Header");
    m_pHeader->setObjectName("Header");
    m_pHeader->setAlignment(Qt::AlignCenter);
    m_pAside = new QLabel("Aside");
    m_pAside->setObjectName("Aside");
    m_pAside->setAlignment(Qt::AlignCenter);
    m_pMain = new QLabel("Main");
    m_pMain->setObjectName("Main");
    m_pMain->setAlignment(Qt::AlignCenter);
    m_pFooter = new QLabel("Footer");
    m_pFooter->setObjectName("Footer");
    m_pFooter->setAlignment(Qt::AlignCenter);

    m_pHeader->setFixedHeight(60);
    m_pAside->setFixedWidth(250);
    m_pFooter->setFixedHeight(60);

    setStyleSheet("QWidget#Layout QLabel#Header { \
                      background:#c6e2ff; \
                  } \
                  QWidget#Layout QLabel#Aside { \
                      background:#d7e9fc; \
                  } \
                  QWidget#Layout QLabel#Main { \
                      background:#ecf5ff; \
                  } \
                  QWidget#Layout QLabel#Footer { \
                      background:#c6e2ff; \
                  }");

    Relayout();
}

void Layout::Relayout()
{
    if (0 == m_nStyleType)
    {
        m_pFooter->hide();
        m_pAside->hide();

        QVBoxLayout *layout1 = new QVBoxLayout();
        layout1->addWidget(m_pHeader);
        layout1->addWidget(m_pMain);
        layout1->setMargin(0);
        layout1->setSpacing(0);
        setLayout(layout1);
    }
    else if (1 == m_nStyleType)
    {
        m_pAside->hide();

        QVBoxLayout *layout1 = new QVBoxLayout();
        layout1->addWidget(m_pHeader);
        layout1->addWidget(m_pMain);
        layout1->addWidget(m_pFooter);
        layout1->setMargin(0);
        layout1->setSpacing(0);
        setLayout(layout1);
    }
    else if (2 == m_nStyleType)
    {
        m_pHeader->hide();
        m_pFooter->hide();

        QHBoxLayout *layout1 = new QHBoxLayout();
        layout1->addWidget(m_pAside);
        layout1->addWidget(m_pMain);
        layout1->setMargin(0);
        layout1->setSpacing(0);
        setLayout(layout1);
    }
    else if (3 == m_nStyleType)
    {
        m_pFooter->hide();

        QHBoxLayout *layout1 = new QHBoxLayout();
        layout1->addWidget(m_pAside);
        layout1->addWidget(m_pMain);
        layout1->setMargin(0);
        layout1->setSpacing(0);

        QVBoxLayout *layout2 = new QVBoxLayout();
        layout2->addWidget(m_pHeader);
        layout2->addLayout(layout1);
        layout2->setMargin(0);
        layout2->setSpacing(0);

        setLayout(layout2);
    }
    else if (4 == m_nStyleType)
    {
        QVBoxLayout *layout3 = new QVBoxLayout();
        layout3->addWidget(m_pMain);
        layout3->addWidget(m_pFooter);
        layout3->setMargin(0);
        layout3->setSpacing(0);

        QHBoxLayout *layout1 = new QHBoxLayout();
        layout1->addWidget(m_pAside);
        layout1->addLayout(layout3);
        layout1->setMargin(0);
        layout1->setSpacing(0);

        QVBoxLayout *layout2 = new QVBoxLayout();
        layout2->addWidget(m_pHeader);
        layout2->addLayout(layout1);
        layout2->setMargin(0);
        layout2->setSpacing(0);

        setLayout(layout2);
    }
    else if (5 == m_nStyleType)
    {
        m_pFooter->hide();

        QVBoxLayout *layout1 = new QVBoxLayout();
        layout1->addWidget(m_pHeader);
        layout1->addWidget(m_pMain);
        layout1->setMargin(0);
        layout1->setSpacing(0);

        QHBoxLayout *layout2 = new QHBoxLayout();
        layout2->addWidget(m_pAside);
        layout2->addLayout(layout1);
        layout2->setMargin(0);
        layout2->setSpacing(0);

        setLayout(layout2);
    }
    else if (6 == m_nStyleType)
    {
        QVBoxLayout *layout1 = new QVBoxLayout();
        layout1->addWidget(m_pHeader);
        layout1->addWidget(m_pMain);
        layout1->addWidget(m_pFooter);
        layout1->setMargin(0);
        layout1->setSpacing(0);

        QHBoxLayout *layout2 = new QHBoxLayout();
        layout2->addWidget(m_pAside);
        layout2->addLayout(layout1);
        layout2->setMargin(0);
        layout2->setSpacing(0);

        setLayout(layout2);
    }
}
