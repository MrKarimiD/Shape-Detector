#ifndef NETWORKSENDER_H
#define NETWORKSENDER_H

#include <QObject>
#include "QUdpSocket"
#include "QHostAddress"

class NetworkSender : public QObject
{
    Q_OBJECT
public:
    explicit NetworkSender(QObject *parent = 0);
    void configure(QString ip,quint16 port);
    void sendData(std::string out);

private:
    QUdpSocket *udpSocket;
    QHostAddress groupAddress;
    quint16 port;

signals:

public slots:

};

#endif // NETWORKSENDER_H
