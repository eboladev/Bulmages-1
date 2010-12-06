
#include "portlistener.h"
#include <QtDebug>

#include "blfunctions.h"




PortListener::PortListener(const QString & portName, BtCompany *tpv)
{
    qDebug() << "hi there";
    
    m_empresaTPV = tpv;
    
    this->port = new QextSerialPort(portName, QextSerialPort::EventDriven);
    port->setBaudRate(BAUD9600);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);

    if (port->open(QIODevice::ReadWrite) == true) {
        connect(port, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(port, SIGNAL(dsrChanged(bool)), this, SLOT(onDsrChanged(bool)));
        if (!(port->lineStatus() & LS_DSR))
            qDebug() << "warning: device is not turned on";
        qDebug() << "listening for data on" << port->portName();
    }
    else {
        qDebug() << "device failed to open:" << port->errorString();
    }
}

void PortListener::onReadyRead()
{
  
    static QString buffer = "";
    QByteArray bytes;
    int a = port->bytesAvailable();
    bytes.resize(a);
    port->read(bytes.data(), bytes.size());
    qDebug() << "bytes read:" << bytes.size();
    qDebug() << "bytes:" << bytes;
    buffer = buffer + bytes;
    
    buffer.replace(" ", "");
    buffer.replace("\"", "");
    if (buffer.indexOf("kg") > 3) {
      QString cant = buffer.left(buffer.indexOf("kg"));  
      cant.replace("S", "");
      if (BlFixed(cant) > BlFixed("0.01")) {
	cant.replace(".",",");
	m_empresaTPV->ticketActual() ->ponerCantidad (cant );
      } // end if
      buffer = "";
    } // end if
    
}

void PortListener::onDsrChanged(bool status)
{
    if (status)
        qDebug() << "device was turned on";
    else
        qDebug() << "device was turned off";
}
