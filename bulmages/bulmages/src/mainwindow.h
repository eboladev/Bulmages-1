#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QSystemTrayIcon>
#include <QtNetwork/QTcpServer>



namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT
    
public:
    QTcpServer *m_tcpServer;
    QList <QTcpSocket *> m_listaSockets;
    
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSystemTrayIcon *m_trayIcon;
    void createTrayIcon();
    void createServer();
    virtual int send(const QString &, QTcpSocket *sock);
    bool conectaDB();
    void procesaDemand(const QString &, QTcpSocket *sock);


public slots:
    virtual void conection();
    virtual void readyRead();
    virtual void readChannelFinished();
    void activado(QSystemTrayIcon::ActivationReason reason);
    virtual void tiempo();
    virtual void on_mui_queryb_released();
    
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
