#include "qcustomplottest.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include "qcustomplot.h"

QCustomPlotTest::QCustomPlotTest(QWidget *parent) : QWidget(parent)
{
    /// 1
    QCustomPlot *pCustomPlot = new QCustomPlot(this);

    {
        // generate some data:
        QVector<double> x(101), y(101); // initialize with entries 0..100
        for (int i = 0; i < 101; ++i) {
            x[i] = i / 50.0 - 1; // x goes from -1 to 1
            y[i] = x[i] * x[i]; // let's plot a quadratic function
        }

        // create graph and assign data to it:
        pCustomPlot->addGraph();
        pCustomPlot->graph(0)->setData(x, y);
        // give the axes some labels:
        pCustomPlot->xAxis->setLabel("x");
        pCustomPlot->yAxis->setLabel("y");
        // set axes ranges, so we see all data:
        pCustomPlot->xAxis->setRange(-1, 1);
        pCustomPlot->yAxis->setRange(0, 1);
        pCustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    }


    // 2
    QCustomPlot *pCustomPlot2 = new QCustomPlot(this);

    {
        // add two new graphs and set their look:
        pCustomPlot2->addGraph();
        pCustomPlot2->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
        pCustomPlot2->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
        pCustomPlot2->addGraph();
        pCustomPlot2->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph

        // generate some points of data (y0 for first, y1 for second graph):
        QVector<double> x(251), y0(251), y1(251);
        for (int i = 0; i < 251; ++i) {
            x[i] = i;
            y0[i] = qExp(-i / 150.0) * qCos(i / 10.0); // exponentially decaying cosine
            y1[i] = qExp(-i / 150.0);            // exponential envelope
        }

        // configure right and top axis to show ticks but no labels:
        // (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
        pCustomPlot2->xAxis2->setVisible(true);
        pCustomPlot2->xAxis2->setTickLabels(false);
        pCustomPlot2->yAxis2->setVisible(true);
        pCustomPlot2->yAxis2->setTickLabels(false);

        // make left and bottom axes always transfer their ranges to right and top axes:
        connect(pCustomPlot2->xAxis, SIGNAL(rangeChanged(QCPRange)), pCustomPlot2->xAxis2, SLOT(setRange(QCPRange)));
        connect(pCustomPlot2->yAxis, SIGNAL(rangeChanged(QCPRange)), pCustomPlot2->yAxis2, SLOT(setRange(QCPRange)));

        // pass data points to graphs:
        pCustomPlot2->graph(0)->setData(x, y0);
        pCustomPlot2->graph(1)->setData(x, y1);
        // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
        pCustomPlot2->graph(0)->rescaleAxes();
        // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
        //pCustomPlot2->graph(1)->rescaleAxes(true);
        // Note: we could have also just called customPlot->rescaleAxes(); instead
        // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
        pCustomPlot2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    }

    // 3
    QCustomPlot *pCustomPlot3 = new QCustomPlot(this);

    {
        pCustomPlot3->legend->setVisible(true);

        // generate two sets of random walk data (one for candlestick and one for ohlc chart):
        int n = 500;
        QVector<double> time(n), value1(n), value2(n);
        QDateTime start(QDate(2022, 1, 1), QTime(0, 0));
        start.setTimeSpec(Qt::UTC);
        double startTime = start.toMSecsSinceEpoch() / 1000.0;
        double binSize = 3600 * 24; // bin data in 1 day intervals
        time[0] = startTime;
        value1[0] = 60;
        value2[0] = 20;
        std::srand(9);
        for (int i = 1; i < n; ++i) {
            time[i] = startTime + 3600 * i;
            value1[i] = value1[i - 1] + (std::rand() / (double)RAND_MAX - 0.5) * 10;
            value2[i] = value2[i - 1] + (std::rand() / (double)RAND_MAX - 0.5) * 3;
        }

        // create candlestick chart:
        QCPFinancial *candlesticks = new QCPFinancial(pCustomPlot3->xAxis, pCustomPlot3->yAxis);
        candlesticks->setName("Candlestick");
        candlesticks->setChartStyle(QCPFinancial::csCandlestick);
        candlesticks->data()->set(QCPFinancial::timeSeriesToOhlc(time, value1, binSize, startTime));
        candlesticks->setWidth(binSize * 0.9);
        candlesticks->setTwoColored(true);
        candlesticks->setBrushPositive(QColor(245, 245, 245));
        candlesticks->setBrushNegative(QColor(40, 40, 40));
        candlesticks->setPenPositive(QPen(QColor(0, 0, 0)));
        candlesticks->setPenNegative(QPen(QColor(0, 0, 0)));

        // create ohlc chart:
        QCPFinancial *ohlc = new QCPFinancial(pCustomPlot3->xAxis, pCustomPlot3->yAxis);
        ohlc->setName("OHLC");
        ohlc->setChartStyle(QCPFinancial::csOhlc);
        ohlc->data()->set(QCPFinancial::timeSeriesToOhlc(time, value2, binSize / 3.0, startTime)); // divide binSize by 3 just to make the ohlc bars a bit denser
        ohlc->setWidth(binSize * 0.2);
        ohlc->setTwoColored(true);

        // create bottom axis rect for volume bar chart:
        QCPAxisRect *volumeAxisRect = new QCPAxisRect(pCustomPlot3);
        pCustomPlot3->plotLayout()->addElement(1, 0, volumeAxisRect);
        volumeAxisRect->setMaximumSize(QSize(QWIDGETSIZE_MAX, 100));
        volumeAxisRect->axis(QCPAxis::atBottom)->setLayer("axes");
        volumeAxisRect->axis(QCPAxis::atBottom)->grid()->setLayer("grid");

        // bring bottom and main axis rect closer together:
        pCustomPlot3->plotLayout()->setRowSpacing(0);
        volumeAxisRect->setAutoMargins(QCP::msLeft | QCP::msRight | QCP::msBottom);
        volumeAxisRect->setMargins(QMargins(0, 0, 0, 0));

        // create two bar plottables, for positive (green) and negative (red) volume bars:
        pCustomPlot3->setAutoAddPlottableToLegend(false);
        QCPBars *volumePos = new QCPBars(volumeAxisRect->axis(QCPAxis::atBottom), volumeAxisRect->axis(QCPAxis::atLeft));
        QCPBars *volumeNeg = new QCPBars(volumeAxisRect->axis(QCPAxis::atBottom), volumeAxisRect->axis(QCPAxis::atLeft));
        for (int i = 0; i < n / 5; ++i) {
            int v = std::rand() % 20000 + std::rand() % 20000 + std::rand() % 20000 - 10000 * 3;
            (v < 0 ? volumeNeg : volumePos)->addData(startTime + 3600 * 5.0 * i, qAbs(v)); // add data to either volumeNeg or volumePos, depending on sign of v
        }

        volumePos->setWidth(3600 * 4);
        volumePos->setPen(Qt::NoPen);
        volumePos->setBrush(QColor(100, 180, 110));
        volumeNeg->setWidth(3600 * 4);
        volumeNeg->setPen(Qt::NoPen);
        volumeNeg->setBrush(QColor(180, 90, 90));

        // interconnect x axis ranges of main and bottom axis rects:
        connect(pCustomPlot3->xAxis, SIGNAL(rangeChanged(QCPRange)), volumeAxisRect->axis(QCPAxis::atBottom), SLOT(setRange(QCPRange)));
        connect(volumeAxisRect->axis(QCPAxis::atBottom), SIGNAL(rangeChanged(QCPRange)), pCustomPlot3->xAxis, SLOT(setRange(QCPRange)));

        // configure axes of both main and bottom axis rect:
        QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
        dateTimeTicker->setDateTimeSpec(Qt::UTC);
        dateTimeTicker->setDateTimeFormat("dd. MMMM");
        volumeAxisRect->axis(QCPAxis::atBottom)->setTicker(dateTimeTicker);
        volumeAxisRect->axis(QCPAxis::atBottom)->setTickLabelRotation(15);
        pCustomPlot3->xAxis->setBasePen(Qt::NoPen);
        pCustomPlot3->xAxis->setTickLabels(false);
        pCustomPlot3->xAxis->setTicks(false); // only want vertical grid in main axis rect, so hide xAxis backbone, ticks, and labels
        pCustomPlot3->xAxis->setTicker(dateTimeTicker);
        pCustomPlot3->rescaleAxes();
        pCustomPlot3->xAxis->scaleRange(1.025, pCustomPlot3->xAxis->range().center());
        pCustomPlot3->yAxis->scaleRange(1.1, pCustomPlot3->yAxis->range().center());

        // make axis rects' left side line up:
        QCPMarginGroup *group = new QCPMarginGroup(pCustomPlot3);
        pCustomPlot3->axisRect()->setMarginGroup(QCP::msLeft | QCP::msRight, group);
        volumeAxisRect->setMarginGroup(QCP::msLeft | QCP::msRight, group);
        pCustomPlot3->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    }

    //
    QCustomPlot *pCustomPlot4 = new QCustomPlot(this);

    {
        // set dark background gradient:
        QLinearGradient gradient(0, 0, 0, 400);
        gradient.setColorAt(0, QColor(90, 90, 90));
        gradient.setColorAt(0.38, QColor(105, 105, 105));
        gradient.setColorAt(1, QColor(70, 70, 70));
        pCustomPlot4->setBackground(QBrush(gradient));

        // create empty bar chart objects:
        QCPBars *regen = new QCPBars(pCustomPlot4->xAxis, pCustomPlot4->yAxis);
        QCPBars *nuclear = new QCPBars(pCustomPlot4->xAxis, pCustomPlot4->yAxis);
        QCPBars *fossil = new QCPBars(pCustomPlot4->xAxis, pCustomPlot4->yAxis);

        regen->setAntialiased(false); // gives more crisp, pixel aligned bar borders
        nuclear->setAntialiased(false);
        fossil->setAntialiased(false);
        regen->setStackingGap(1);
        nuclear->setStackingGap(1);
        fossil->setStackingGap(1);

        // set names and colors:
        fossil->setName("Fossil fuels");
        fossil->setPen(QPen(QColor(111, 9, 176).lighter(170)));
        fossil->setBrush(QColor(111, 9, 176));
        nuclear->setName("Nuclear");
        nuclear->setPen(QPen(QColor(250, 170, 20).lighter(150)));
        nuclear->setBrush(QColor(250, 170, 20));
        regen->setName("Regenerative");
        regen->setPen(QPen(QColor(0, 168, 140).lighter(130)));
        regen->setBrush(QColor(0, 168, 140));

        // stack bars on top of each other:
        nuclear->moveAbove(fossil);
        regen->moveAbove(nuclear);

        // prepare x axis with country labels:
        QVector<double> ticks;
        QVector<QString> labels;
        ticks << 1 << 2 << 3 << 4 << 5 << 6 << 7;
        labels << "USA" << "Japan" << "Germany" << "France" << "UK" << "Italy" << "Canada";

        QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
        textTicker->addTicks(ticks, labels);
        pCustomPlot4->xAxis->setTicker(textTicker);
        pCustomPlot4->xAxis->setTickLabelRotation(60);
        pCustomPlot4->xAxis->setSubTicks(false);
        pCustomPlot4->xAxis->setTickLength(0, 4);
        pCustomPlot4->xAxis->setRange(0, 8);
        pCustomPlot4->xAxis->setBasePen(QPen(Qt::white));
        pCustomPlot4->xAxis->setTickPen(QPen(Qt::white));
        pCustomPlot4->xAxis->grid()->setVisible(true);
        pCustomPlot4->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
        pCustomPlot4->xAxis->setTickLabelColor(Qt::white);
        pCustomPlot4->xAxis->setLabelColor(Qt::white);

        // prepare y axis:
        pCustomPlot4->yAxis->setRange(0, 12.1);
        pCustomPlot4->yAxis->setPadding(5); // a bit more space to the left border
        pCustomPlot4->yAxis->setLabel("Power Consumption in\nKilowatts per Capita (2007)");
        pCustomPlot4->yAxis->setBasePen(QPen(Qt::white));
        pCustomPlot4->yAxis->setTickPen(QPen(Qt::white));
        pCustomPlot4->yAxis->setSubTickPen(QPen(Qt::white));
        pCustomPlot4->yAxis->grid()->setSubGridVisible(true);
        pCustomPlot4->yAxis->setTickLabelColor(Qt::white);
        pCustomPlot4->yAxis->setLabelColor(Qt::white);
        pCustomPlot4->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
        pCustomPlot4->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

        // Add data:
        QVector<double> fossilData, nuclearData, regenData;
        fossilData  << 0.86 * 10.5 << 0.83 * 5.5 << 0.84 * 5.5 << 0.52 * 5.8 << 0.89 * 5.2 << 0.90 * 4.2 << 0.67 * 11.2;
        nuclearData << 0.08 * 10.5 << 0.12 * 5.5 << 0.12 * 5.5 << 0.40 * 5.8 << 0.09 * 5.2 << 0.00 * 4.2 << 0.07 * 11.2;
        regenData   << 0.06 * 10.5 << 0.05 * 5.5 << 0.04 * 5.5 << 0.06 * 5.8 << 0.02 * 5.2 << 0.07 * 4.2 << 0.25 * 11.2;
        fossil->setData(ticks, fossilData);
        nuclear->setData(ticks, nuclearData);
        regen->setData(ticks, regenData);

        // setup legend:
        pCustomPlot4->legend->setVisible(true);
        pCustomPlot4->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignHCenter);
        pCustomPlot4->legend->setBrush(QColor(255, 255, 255, 100));
        pCustomPlot4->legend->setBorderPen(Qt::NoPen);
        QFont legendFont = font();
        legendFont.setPointSize(10);
        pCustomPlot4->legend->setFont(legendFont);
        pCustomPlot4->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    }


    pCustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->addWidget(pCustomPlot, 0, 0, 1, 1);
    mainLayout->addWidget(pCustomPlot2, 0, 1, 1, 1);

    mainLayout->addWidget(pCustomPlot3, 1, 0, 1, 1);
    mainLayout->addWidget(pCustomPlot4, 1, 1, 1, 1);

    setLayout(mainLayout);
}
