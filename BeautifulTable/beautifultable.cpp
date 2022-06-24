#include "beautifultable.h"
#include "qstyleditemdelegateex.h"

#include <QTime>
#include <QApplication>
#include <QFile>

BeautifulTable::BeautifulTable(QWidget *parent)
    : QDialog(parent)
{
    m_pTableView = new QTableView(this);
    m_pModel = new QStandardItemModel(this);

    //加载样式表
    QFile file(":/style.css");
    if (file.open(QFile::ReadOnly))
    {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }

    AddData();
    InitCtrl();

    SetStyle();
}

BeautifulTable::~BeautifulTable()
{

}

void BeautifulTable::InitCtrl()
{
    // 设置单行选中、最后一列拉伸、表头不高亮、无边框等
    m_pTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pTableView->horizontalHeader()->setStretchLastSection(true);
    m_pTableView->horizontalHeader()->setHighlightSections(false);
    m_pTableView->verticalHeader()->setVisible(false);
    m_pTableView->setShowGrid(false);
    m_pTableView->setFrameShape(QFrame::NoFrame);
    m_pTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTableView->setModel(m_pModel);

    m_pTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);    // 不可编辑

    m_pTableView->horizontalHeader()->setStretchLastSection(true);
    m_pTableView->setColumnWidth(0, 50);
    m_pTableView->setColumnWidth(1, 100);
    m_pTableView->setColumnWidth(2, 50);
    m_pTableView->setColumnWidth(3, 200);
    m_pTableView->setColumnWidth(4, 50);
    m_pTableView->setColumnWidth(5, 100);
    m_pTableView->setColumnWidth(6, 150);

    QStyledItemDelegateEx *pStyledItemDelegateEx = new QStyledItemDelegateEx();
    m_pTableView->setItemDelegate(pStyledItemDelegateEx);

    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->addWidget(m_pTableView);
    pLayout->setMargin(0);

    resize(900, 400);
}

void BeautifulTable::AddData()
{
    QStringList table_h_headers;
    table_h_headers << " " << "姓名" << "性别" << "邮箱"<< "评分"<< "进度"<< "注册时间"<< "操作";
    m_pModel->setHorizontalHeaderLabels(table_h_headers);


    QTime randtime;
    randtime = QTime::currentTime();
    qsrand(randtime.msec()+randtime.second()*1000); //以当前时间ms为随机种子

    for (int i = 0; i < 2000; i++)
    {
        m_pModel->setItem(i, 0, new QStandardItem(""));

        QStandardItem *item = new QStandardItem("xxx");
        item->setTextAlignment(Qt::AlignCenter);
        m_pModel->setItem(i, 1, item);

        item = new QStandardItem("");
        item->setTextAlignment(Qt::AlignCenter);
        m_pModel->setItem(i, 2, item);

        item = new QStandardItem("12152525@qq.com");
        item->setTextAlignment(Qt::AlignCenter);
        m_pModel->setItem(i, 3, item);

        item = new QStandardItem("4.5");
        item->setTextAlignment(Qt::AlignCenter);
        m_pModel->setItem(i, 4, item);

        item = new QStandardItem("");
        item->setData(qrand() % 100);
        item->setTextAlignment(Qt::AlignCenter);
        m_pModel->setItem(i, 5, item);

        item = new QStandardItem("2022年6月22日15:32:13");
        item->setTextAlignment(Qt::AlignCenter);
        m_pModel->setItem(i, 6, item);

        item = new QStandardItem();
        item->setData("编辑");
        item->setTextAlignment(Qt::AlignCenter);
        m_pModel->setItem(i, 7, item);
    }

    //startTimer(100);
}

void BeautifulTable::SetStyle()
{
    m_pTableView->setAlternatingRowColors(true);  //开启间隔行颜色
}

void BeautifulTable::timerEvent(QTimerEvent *event)
{
    QTime randtime;
    randtime = QTime::currentTime();
    qsrand(randtime.msec()+randtime.second()*1000); //以当前时间ms为随机种子

    for (int i = 0; i < 2000; i++)
    {
        m_pModel->setItem(i, 0, new QStandardItem(""));

        QStandardItem *item = new QStandardItem("xxx");
        item->setTextAlignment(Qt::AlignCenter);
        m_pModel->setItem(i, 1, item);

        item = new QStandardItem("");
        item->setTextAlignment(Qt::AlignCenter);
        m_pModel->setItem(i, 2, item);

        item = new QStandardItem("12152525@qq.com");
        item->setTextAlignment(Qt::AlignCenter);
        m_pModel->setItem(i, 3, item);

        item = new QStandardItem("4.5");
        item->setTextAlignment(Qt::AlignCenter);
        m_pModel->setItem(i, 4, item);

        item = new QStandardItem("");
        item->setData(qrand() % 100);
        item->setTextAlignment(Qt::AlignCenter);
        m_pModel->setItem(i, 5, item);

        item = new QStandardItem("2022年6月22日15:32:13");
        item->setTextAlignment(Qt::AlignCenter);
        m_pModel->setItem(i, 6, item);

        item = new QStandardItem();
        item->setData("编辑");
        item->setTextAlignment(Qt::AlignCenter);
        m_pModel->setItem(i, 7, item);
    }
}
