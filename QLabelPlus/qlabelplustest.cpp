#include "qlabelplustest.h"
#include "rolltitles.h"
#include <QGridLayout>


QLabelPlusTest::QLabelPlusTest(QWidget *parent) : QWidget(parent)
{
    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->addWidget(new RollTitles(),0, 0);
    setLayout(mainLayout);
}
