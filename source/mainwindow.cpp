#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "beautifultable.h"
#include "gradient.h"
#include "imageconvert.h"
#include "mainwnd.h"
#include "perfectclock.h"
#include "animation.h"
#include "layout.h"
#include "radar.h"
#include "incofontpane.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTabWidget *pLayouTabWidget = new QTabWidget();
    for (int i = 0; i <= 6; i++)
        pLayouTabWidget->addTab(new Layout(i), QString("布局%1").arg(i + 1));
    ui->tabWidget->addTab(pLayouTabWidget, "常用布局");
    ui->tabWidget->addTab(new Animation, "动画演示");
    ui->tabWidget->addTab(new BeautifulTable, "自定义委托表格");
    ui->tabWidget->addTab(new Gradient, "渐变色");
    ui->tabWidget->addTab(new ImageConvert, "图片转换");
    ui->tabWidget->addTab(new MainWnd, "水波进度条");
    ui->tabWidget->addTab(new PerfectClock, "时钟绘制");
    ui->tabWidget->addTab(new Radar, "雷达界面");
    ui->tabWidget->addTab(new CIconFontPane, "字体图标");
}

MainWindow::~MainWindow()
{
    delete ui;
}
