#include "timelinetest.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "ctimeline.h"

TimeLineTest::TimeLineTest(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout1 = new QVBoxLayout();
    layout1->addWidget(new CTimeLine(this));
    layout1->addWidget(new CTimeLine(this, TIMELINETYPE_RICEWHITE));
    layout1->setMargin(0);
    layout1->setSpacing(0);
    setLayout(layout1);
}
