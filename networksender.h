#ifndef NETWORKSENDER_H
#define NETWORKSENDER_H

#include <QObject>
#include "QUdpSocket"
#include "QHostAddress"

class NetworkSender : public QObject
{
    Q_OBJECT
public:
    explicit NetworkSender(QString ip,quint16 port,QObject *parent = 0);
    void sendData(QByteArray out);

private:
    QUdpSocket *udpSocket;

signals:

public slots:

};

#endif // NETWORKSENDER_H
