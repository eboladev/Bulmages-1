#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include <QObject>

#include "bulmafact.h"




class mypluginbf : public QObject {
Q_OBJECT
private:
    bulmafact *m_bges;
    company *m_companyact;
public:
    mypluginbf();
    virtual ~mypluginbf();
    virtual void inicializa(bulmafact *);
public slots:
    void elslot();
    void elslot1();
};



#endif


