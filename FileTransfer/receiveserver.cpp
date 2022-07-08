#include "receiveserver.h"
#include "ui_receiveserver.h"

#include <QDebug>
#include <QTcpSocket>
#include <QDataStream>
#include <QPointer>
#include <QFileInfo>
#include <QStyleFactory>
#include <QMessageBox>

ReceiveServer::ReceiveServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReceiveServer),
    m_pSocket(nullptr),
    m_fileSize(0),
    m_fileBytesReceived(0)
{
    ui->setupUi(this);

    if (m_server.listen(QHostAddress::Any, 55455)) {
        ui->plainTextEditLog->appendPlainText(QStringLiteral("状态：正在监听！"));
    } else {
        ui->plainTextEditLog->appendPlainText(QStringLiteral("状态：监听失败！"));
    }
    ui->labelListenPort->setText(QString::number(m_server.serverPort()));

    connect(&m_server, &QTcpServer::newConnection, this, &ReceiveServer::onNewConnection);
}

ReceiveServer::~ReceiveServer()
{
    delete ui;
}

void ReceiveServer::onNewConnection()
{
    m_pSocket = m_server.nextPendingConnection();

    connect(m_pSocket, &QTcpSocket::disconnected, m_pSocket, &QTcpSocket::deleteLater);
    connect(m_pSocket, &QIODevice::readyRead, this, &ReceiveServer::onReadyRead);
    connect(m_pSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));

    m_inStream.setDevice(m_pSocket);
    m_inStream.setVersion(QDataStream::Qt_5_0);
}

void ReceiveServer::onReadyRead()
{
    while (m_pSocket->bytesAvailable()) {
        if (0 == m_fileSize && m_pSocket->bytesAvailable() > sizeof(qint64)) {
            m_inStream >> m_fileSize >> m_fileName;
            m_file.close();
            m_file.setFileName(m_fileName);
            if (!m_file.open(QIODevice::WriteOnly)) {
                qCritical() << m_file.errorString();
                return;
            }
            ui->plainTextEditLog->appendPlainText(QStringLiteral("正在接收【%1】 ...").arg(m_fileName));
        } else {
            qint64 size = qMin(m_pSocket->bytesAvailable(), m_fileSize - m_fileBytesReceived);
            if (size == 0) {
                reset();
                continue;
            }

            QByteArray arry(size, 0);
            m_inStream.readRawData(arry.data(), size);
            m_file.write(arry);

            m_fileBytesReceived += size;
            ui->plainTextEditLog->appendPlainText(QStringLiteral("正在接收【%1】【%2/%3】 ...")
                                                  .arg(m_fileName)
                                                  .arg(m_fileBytesReceived)
                                                  .arg(m_fileSize));
            if (m_fileBytesReceived == m_fileSize) {
                QFileInfo info(m_fileName);
                ui->plainTextEditLog->appendPlainText(QStringLiteral("成功接收【%1】 -> %2").arg(m_fileName).arg(info.absoluteFilePath()));
                reset();
            }
        }
    }
}

void ReceiveServer::reset()
{
    m_file.close();
    m_fileName.clear();
    m_fileSize = 0;
    m_fileBytesReceived = 0;
}

void ReceiveServer::onSocketError(QAbstractSocket::SocketError error)
{
    switch (error) {
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("%1").arg(m_pSocket->errorString()));
        qDebug() << __FUNCTION__ << "QAbstractSocket::ConnectionRefusedError";
        break;
    case QAbstractSocket::RemoteHostClosedError:
        qDebug() << __FUNCTION__ << "QAbstractSocket::RemoteHostClosedError";
        ui->plainTextEditLog->appendPlainText(QStringLiteral("文件传输终止！"));
        reset();
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("%1").arg(m_pSocket->errorString()));
        qDebug() << __FUNCTION__ << "QAbstractSocket::HostNotFoundError";
        break;
    case QAbstractSocket::SocketTimeoutError:
        QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("%1").arg(m_pSocket->errorString()));
        qDebug() << __FUNCTION__ << "QAbstractSocket::SocketTimeoutError";
        break;
    case QAbstractSocket::AddressInUseError:
        qDebug() << __FUNCTION__ << "QAbstractSocket::AddressInUseError";
        break;
    default:
        break;
    }
}
