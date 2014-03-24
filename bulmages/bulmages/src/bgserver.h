#ifndef BGSERVER_H
#define BGSERVER_H

#include <QtNetwork/QTcpServer>




class BgServer : public QObject
{
    Q_OBJECT
    
public:
    QTcpServer *m_tcpServer;
    QList <QTcpSocket *> m_listaSockets;
    
    BgServer();
    ~BgServer();
    void createServer();
    virtual int send(const QString &, QTcpSocket *sock);
    bool conectaDB();
    void procesaDemand(const QString &, QTcpSocket *sock);
    void procesaCommand(const QString &, QTcpSocket *sock);
    void readyRead(QTcpSocket *socket);


public slots:
    virtual void conection();
    virtual void readyRead();
    virtual void readChannelFinished();
    virtual void tiempo();
    virtual void on_mui_queryb_released();
    
};

#endif // MAINWINDOW_H
