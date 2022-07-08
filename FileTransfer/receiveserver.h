#ifndef RECEIVESERVER_H
#define RECEIVESERVER_H

#include <QWidget>
#include <QMainWindow>
#include <QTcpServer>
#include <QUdpSocket>
#include <QFile>

class QTcpSocket;

namespace Ui {
class ReceiveServer;
}

class ReceiveServer : public QWidget
{
    Q_OBJECT

public:
    explicit ReceiveServer(QWidget *parent = 0);
    ~ReceiveServer();

private slots:
    void onNewConnection();
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError error);

private:
    void reset();

private:
    Ui::ReceiveServer *ui;

    QFile m_file;
    qint64 m_fileSize;
    QString m_fileName;

    QDataStream m_inStream;
    QTcpServer m_server;

    qint64 m_fileBytesReceived;

    QTcpSocket *m_pSocket;
};

#endif // RECEIVESERVER_H
