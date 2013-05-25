
#include "portlistener.h"
#include <QtCore/QtDebug>
#include <time.h>
#include "blfunctions.h"
#ifdef Q_OS_WIN32
#include <windows.h> // Para el uso de Sleep
#endif

PortListener::PortListener(const QString & portName, BtCompany *tpv)
{
    qDebug() << "hi there";
    
    m_company = tpv;
    
    port = new QextSerialPort(portName, QextSerialPort::EventDriven);
    port->setBaudRate(BAUD9600);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);

    port->setParity(PAR_EVEN);
    port->setDataBits(DATA_7);
    port->setStopBits(STOP_1);
    
    if (port->open(QIODevice::ReadWrite) == true) {
//        connect(port, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
//        connect(port, SIGNAL(dsrChanged(bool)), this, SLOT(onDsrChanged(bool)));
//	comm();
        if (!(port->lineStatus() & LS_DSR))
            qDebug() << "warning: device is not turned on";
        qDebug() << "listening for data on" << port->portName();
    }
    else {
        qDebug() << "device failed to open:" << port->errorString();
    }
}


void PortListener::comm() {
    char buffer[50];
    buffer[0] = 2;
    
    if (!m_company->ticketActual())
      return;
       
    if (!m_company->ticketActual() ->lineaActBtTicket()) 
      return;
    
    QString pvp = m_company->ticketActual() ->lineaActBtTicket()->dbValue("pvpivainclalbaran");
    pvp.replace(".","");
    pvp.replace(",","");
    pvp = pvp.rightJustified(5, '0');
    
    fprintf(stderr,"Enviamos STX\n");
    port->writeData(buffer,1);	/// Enviamos el STX
    int duerme=0;
    while (!port->readData(buffer, 1) && buffer[0] != 6) {
      duerme ++;
#ifndef Q_OS_WIN32
      usleep(5000);
#else
	  Sleep(5000);
#endif

      if (duerme > 3)
	return;
    } // end while
      /// Esperamos el ACK
    fprintf(stderr,"Recibimos ACK %d\n", buffer[0]);
    buffer[0] = '@';
    buffer[1] = '1';
    buffer[2] = pvp[0].toLatin1();
    buffer[3] = pvp[1].toLatin1();
    buffer[4] = pvp[2].toLatin1();
    buffer[5] = pvp[3].toLatin1();
    buffer[6] = pvp[4].toLatin1();
    buffer[7] = 13; // CR
    buffer[8] = 10; // LF
    buffer[9] = 2;
    fprintf(stderr,"Enviamos Pframe\n");
    port->writeData(buffer,10);	/// Enviamos el Pframe
    duerme=0;
    buffer[0] = 9; // Un valor distinto de 6 para comprobar
    while (!port->readData(buffer, 1) && buffer[0] != 6) {
      duerme ++;
#ifndef Q_OS_WIN32
      usleep(5000);
#else
	  Sleep(5000);
#endif
      if (duerme > 3)
	return;
    } // end while
      /// Esperamos el ACK
    fprintf(stderr,"Recibimos ACK %d\n", buffer[0]);
    buffer[0] = '1';
    buffer[1] = '2';
    buffer[2] = 13;
    buffer[3] = 10;
    buffer[4] = '5';
    fprintf(stderr,"Enviamos Rframe\n");    
    port->writeData(buffer,5);	/// Enviamos el Rframe
    duerme=0;
    buffer[0] = 9; // Un valor distinto de 6 para comprobar
    while (!port->readData(buffer, 1) && buffer[0] != 2) {
      duerme ++;
#ifndef Q_OS_WIN32
      usleep(5000);
#else
	  Sleep(5000);
#endif
      if (duerme > 3)
	return;
    } // end while
      /// Esperamos el ACK
    fprintf(stderr,"Recibimos STX %d\n", buffer[0]);
    buffer[0] = 6;
    port->writeData(buffer,1);	/// Enviamos el ACK
    fprintf(stderr,"Enviamos ACK\n");
    port->flush();
    int recibidos = 0;
    duerme = 0;
    while (recibidos < 19) { /// Esperamos el STX
      int nrecibidos = port->readData(buffer+recibidos, 19);
      if (nrecibidos) {
	fprintf(stderr,"\nbuffer:[%s]\n", buffer);
      } else {
	duerme ++;
#ifndef Q_OS_WIN32
      usleep(5000);
#else
	  Sleep(5000);
#endif
	if (duerme > 3)
	    return;
      } // end if
      recibidos += nrecibidos;
    }// end while
    fprintf(stderr,"\nbuffer:[%s]\n", buffer);
    buffer[5] = 0;
    QString cantidad(buffer);
    cantidad = cantidad.left(2) + "." + cantidad.right(3);
    m_company->ticketActual() ->ponerCantidad (cantidad);
}


void PortListener::onReadyRead()
{
  
    static QString buffer = "";
    QByteArray bytes;
    int a = port->bytesAvailable();
    bytes.resize(a);
    port->read(bytes.data(), bytes.size());
//    qDebug() << "bytes read:" << bytes.size();
//    qDebug() << "bytes:" << bytes;
    buffer = buffer + bytes;
    
    buffer.replace(" ", "");
    buffer.replace("\"", "");
    buffer.replace("\n", "");
    buffer.replace("\r", "");
    if (buffer.indexOf("kg") > 1) {
      qDebug() << "buffer:[" << buffer << "]\n";
      QString cant = buffer.left(buffer.indexOf("kg"));  
      cant.replace("S", "");
      //qDebug() << "cant:[" << cant << "]\n";
      fprintf(stderr,"\ncant:[%s]\n", cant.toLatin1().data());
      QChar *data = cant.data();
      while (!data->isNull()) {
	  //fprintf(stderr,"\n[%d-%c]\n", data->toLatin1(), data->toLatin1());
	  fprintf(stderr,"\n[%d-%c]\n", data->unicode(), data->unicode());
	  ++data;
      } // end while      
      
      
      if (BlFixed(cant) > BlFixed("0.01")) {
	cant.replace(".",",");
	m_company->ticketActual() ->ponerCantidad (cant );
      } // end if
      buffer = "";
      buffer.clear();
//      port->writeData("123",3);
    } // end if
    
}

void PortListener::onDsrChanged(bool status)
{
    if (status)
        qDebug() << "device was turned on";
    else
        qDebug() << "device was turned off";
}
