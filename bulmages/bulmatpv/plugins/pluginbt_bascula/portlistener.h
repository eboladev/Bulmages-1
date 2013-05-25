#ifndef PORTLISTENER_H_
#define PORTLISTENER_H_

#include <QtCore/QObject>
#include "qextserialport.h"
#include "btbulmatpv.h"
#include "btcompany.h"
#include "pdefs_pluginbt_bascula.h"

class PLUGINBT_BASCULA_EXPORT PortListener : public QObject
{
Q_OBJECT
public:
    PortListener(const QString & portName, BtCompany *tpv);

private:
    QextSerialPort *port;
    BtCompany *m_company;
    
public:
    void comm();
    
private slots:
    void onReadyRead();
    void onDsrChanged(bool status);


};


#endif /*PORTLISTENER_H_*/
