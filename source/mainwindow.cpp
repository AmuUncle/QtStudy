#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QToolButton>
#include <QDebug>

#include "iconhelper.h"
#include "navbutton.h"

#include "beautifultable.h"
#include "gradient.h"
#include "imageconvert.h"
#include "mainwnd.h"
#include "perfectclock.h"
#include "animation.h"
#include "layout.h"
#include "radar.h"
#include "incofontpane.h"
#include "carouselpane.h"
#include "threadtool.h"
#include "circulardial.h"
#include "filetransfer.h"
#include "captchatest.h"
#include "luckyturntable.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    IconHelper::Load();

    QWidget *pNavMain = new QWidget(this);
    m_mainVLayout = new QGridLayout();
    QVBoxLayout *mainVLayout = new QVBoxLayout(pNavMain);
    mainVLayout->addLayout(m_mainVLayout);
    mainVLayout->addStretch();

    ui->scrollArea->setWidgetResizable(true);//!!!注意  不加这个widget不会被ScroolArea拉伸 而是原有大小
    ui->scrollArea->setWidget(pNavMain);

    QTabWidget *pLayoutTabWidget = new QTabWidget();
    for (int i = 0; i <= 6; i++)
        pLayoutTabWidget->addTab(new Layout(i), QString("布局%1").arg(i + 1));

    AddNavBar("常用布局", QChar(0xe60b), pLayoutTabWidget);
    AddNavBar("动画演示", QChar(0xe671), new Animation);
    AddNavBar("自定义委托表格", QChar(0xe65b), new BeautifulTable);
    AddNavBar("渐变色", QChar(0xe636), new Gradient);
    AddNavBar("图片转换", QChar(0xe611), new ImageConvert);
    AddNavBar("水波进度条", QChar(0xe618), new MainWnd);
    AddNavBar("时钟绘制", QChar(0xe62d), new PerfectClock);
    AddNavBar("雷达界面", QChar(0xe62b), new Radar);
    AddNavBar("字体图标", QChar(0xe626), new CIconFontPane);
    AddNavBar("轮播图", QChar(0xe622), new CarouselPane);
    AddNavBar("多线程队列", QChar(0xe61d), new ThreadTool);
    AddNavBar("汽车仪表盘", QChar(0xe64c), new CircularDial);
    AddNavBar("文件传输", QChar(0xe641), new FileTransfer);
    AddNavBar("验证码", QChar(0xe64a), new CaptchaTest);
    AddNavBar("幸运大转盘", QChar(0xe630), new LuckyTurntable);

    connect(ui->btnBack, &QPushButton::clicked, [=]()
    {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(ui->stackedWidget, &QStackedWidget::currentChanged, [=](int nIndex)
    {
        ui->btnBack->setVisible(nIndex > 0);
    });

    ui->btnBack->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddNavBar(QString strTitle, QChar icon, QWidget *w)
{
    int nIndex = ui->stackedWidget->addWidget(w);

    NavButton *btn = new NavButton(ui->scrollArea);
    btn->SetData(strTitle, icon, nIndex);
    connect(btn, &NavButton::clicked, [=](QVariant data)
    {
        ui->stackedWidget->setCurrentIndex(data.toInt());
    });

    m_mainVLayout->addWidget(btn, (nIndex - 1) / 4, (nIndex - 1) % 4);
}
