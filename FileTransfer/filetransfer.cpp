#include "filetransfer.h"
#include "sendclient.h"
#include "receiveserver.h"
#include <QHBoxLayout>

FileTransfer::FileTransfer(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout1 = new QHBoxLayout();
    layout1->addWidget(new SendClient);
    layout1->addWidget(new ReceiveServer);
    layout1->setMargin(0);
    layout1->setSpacing(0);
    setLayout(layout1);
}
