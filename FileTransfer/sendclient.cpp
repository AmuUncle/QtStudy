#include "sendclient.h"
#include "ui_sendclient.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>
#include <QHostAddress>
#include <QMessageBox>
#include <QFileInfo>
#include <QStyleFactory>

SendClient::SendClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SendClient),
    m_blockSize(1024 * 1024 * 1),
    m_currentFileSize(0),
    m_totalFileBytesWritten(0),
    m_totalFileSize(0),
    m_currentFileBytesWritten(0)
{
    ui->setupUi(this);

    QRegExp rx("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
    ui->lineEditIpAddress->setValidator(new QRegExpValidator(rx, this));

    connect(ui->pushButtonAddFile, &QPushButton::clicked, this, &SendClient::addFile);
    connect(ui->pushButtonSend, &QPushButton::clicked, this, &SendClient::sendFile);
    connect(ui->pushButtonDelFile, &QPushButton::clicked, this, &SendClient::delFile);
    connect(ui->pushButtonEmptyFile, &QPushButton::clicked, this, &SendClient::clearFile);

    connect(&m_socket, &QIODevice::bytesWritten, this, &SendClient::onBytesWritten);
    connect(&m_socket, &QAbstractSocket::stateChanged, this, &SendClient::onSocketStateChanged);
    connect(&m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
}

SendClient::~SendClient()
{
    delete ui;
}


void SendClient::sendFile()
{
    QString address = ui->lineEditIpAddress->text();
    int port = ui->spinBoxPort->text().toInt();

    QHostAddress hostAddress;
    if (!hostAddress.setAddress(address)) {
        QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("目标网络地址错误！"));
        return;
    }

    if (0 == ui->listWidget->count()) {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请选择需要发送的文件！"));
        addFile();
        return;
    }

    m_fileQueue.clear();
    int count = ui->listWidget->count();
    for (int i = 0; i < count; ++i) {
        QString file = ui->listWidget->item(i)->text();
        m_fileQueue.append(file);

        QFileInfo info(file);
        m_totalFileSize += info.size();
    }

    m_socket.connectToHost(address, port);
}

void SendClient::addFile()
{
    QStringList files = QFileDialog::getOpenFileNames(this, QStringLiteral("选择文件"),
                                                QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    if (files.isEmpty()) {
        return;
    }

    ui->listWidget->addItems(files);
}

void SendClient::delFile()
{
    int row = ui->listWidget->currentRow();
    if (row < 0) {
        return;
    }

    delete ui->listWidget->takeItem(row);
}

void SendClient::clearFile()
{
    int count = ui->listWidget->count();
    if (0 == count) {
        return;
    }

    int ret = QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("确定清空文件列表吗？"),
                             QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);
    if (QMessageBox::Ok != ret) {
        return;
    }

    for (int i = 0; i < count; ++i) {
        delete ui->listWidget->takeItem(0);
    }
}

void SendClient::onSocketStateChanged(QAbstractSocket::SocketState state)
{
    switch (state) {
    case QAbstractSocket::UnconnectedState:
        m_file.close();
        qDebug() << m_totalFileSize << " " << m_totalFileBytesWritten;
        qDebug() << __FUNCTION__ << "QAbstractSocket::UnconnectedState";
        break;
    case QAbstractSocket::HostLookupState:
        qDebug() << __FUNCTION__ << "QAbstractSocket::HostLookupState";
        break;
    case QAbstractSocket::ConnectingState:
        qDebug() << __FUNCTION__ << "QAbstractSocket::ConnectingState";
        break;
    case QAbstractSocket::ConnectedState:
        qDebug() << __FUNCTION__ << "QAbstractSocket::ConnectedState";
        m_timer.restart();
        send();
        break;
    case QAbstractSocket::BoundState:
        break;
    case QAbstractSocket::ClosingState:
        qDebug() << __FUNCTION__ << "QAbstractSocket::ClosingState";
        break;
    case QAbstractSocket::ListeningState:
        break;
    default:
        break;
    }
}

void SendClient::onSocketError(QAbstractSocket::SocketError error)
{
    switch (error) {
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << __FUNCTION__ << "QAbstractSocket::ConnectionRefusedError";
        break;
    case QAbstractSocket::RemoteHostClosedError:
        qDebug() << __FUNCTION__ << "QAbstractSocket::RemoteHostClosedError";
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug() << __FUNCTION__ << "QAbstractSocket::HostNotFoundError";
        break;
    case QAbstractSocket::SocketTimeoutError:
        qDebug() << __FUNCTION__ << "QAbstractSocket::SocketTimeoutError";
        break;
    case QAbstractSocket::AddressInUseError:
        qDebug() << __FUNCTION__ << "QAbstractSocket::AddressInUseError";
        break;
    default:
        break;
    }
    QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("%1").arg(m_socket.errorString()));
}

void SendClient::onBytesWritten(const qint64 &bytes)
{
    Q_UNUSED(bytes)

    QByteArray arry = m_file.read(m_blockSize);
    if (arry.isEmpty()) {
        reset();
        return;
    }

    int size = arry.size();
    m_outStream.writeRawData(arry.constData(), size);

    updateProgress(size);
}

void SendClient::send()
{
    m_file.close();
    m_file.setFileName(m_fileQueue.dequeue());

    if (!m_file.open(QIODevice::ReadOnly)) {
        qCritical() << m_file.errorString();
        QMessageBox::critical(this, QStringLiteral("错误"), m_file.errorString());
        return;
    }

    m_currentFileSize = m_file.size();

    //设置当前文件进度显示格式
    ui->currentProgressBar->setFormat(QStringLiteral("%1 : %p%").arg(m_file.fileName()));

    m_outStream.setDevice(&m_socket);
    m_outStream.setVersion(QDataStream::Qt_5_0);

    QFileInfo info(m_file.fileName());
    QString fileName = info.fileName();

    //发送文件大小及文件名
    m_outStream << m_currentFileSize << fileName;

    //开始传输文件
    QByteArray arry = m_file.read(m_blockSize);
    int size = arry.size();
    m_outStream.writeRawData(arry.constData(), size);

    ui->pushButtonSend->setEnabled(false);
    updateProgress(size);
}

void SendClient::reset()
{
    ui->pushButtonSend->setEnabled(true);

    m_currentFileBytesWritten = 0;

    if (m_fileQueue.isEmpty()) {
        m_socket.close();

        qint64 milliseconds = m_timer.elapsed();
        ui->labelTip->setText(QStringLiteral("共耗时：%1 毫秒  平均：%2 MB/s")
                              .arg(QString::number(milliseconds))
                              .arg(QString::number(((m_totalFileSize / (1024.0 * 1024.0)) / (milliseconds / 1000.0)), 'f', 3)));
        m_totalFileSize = 0;
        m_totalFileBytesWritten = 0;
    } else {
        send();
    }
}

void SendClient::updateProgress(const int &size)
{
    m_totalFileBytesWritten += size;
    ui->totalProgressBar->setValue((double(m_totalFileBytesWritten)/m_totalFileSize) * 100);

    m_currentFileBytesWritten += size;
    ui->currentProgressBar->setValue((double(m_currentFileBytesWritten)/m_file.size()) * 100);

    double speed = (double)m_totalFileBytesWritten / m_timer.elapsed();

    //ui->statusBar->showMessage(QStringLiteral("实时速度：%1").arg(QString::number(speed / (1024*1024/1000), 'f', 3) + "MB/s"), 5000);
}
