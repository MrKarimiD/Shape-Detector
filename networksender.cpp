#include "networksender.h"

NetworkSender::NetworkSender(QString ip, quint16 port, QObject *parent) :
    QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress(ip),port);
}

void NetworkSender::sendData(QByteArray out)
{
    udpSocket->write(out);
}
