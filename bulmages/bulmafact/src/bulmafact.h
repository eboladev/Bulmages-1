#ifndef BULMAFACT_H
#define BULMAFACT_H

#include <qmainwindow.h>
#include <qworkspace.h>
#include <qvbox.h>

#include "bulmafactbase.h"
#include "company.h"


class bulmafact: public bulmafactbase
{
    Q_OBJECT
private:
   company m_company;
   QVBox *view_back; 
   QWorkspace *pWorkspace;

public:
    bulmafact();
    ~bulmafact();
    
public slots:
    virtual void listproviders();
    virtual void proveedores();
    virtual void caja();
    virtual void clientes();
    virtual void emitirfactura();
    virtual void recibirfactura();
    virtual void listClients();
    virtual void newClient();
    virtual void listarticles();
    virtual void listorders();
    virtual void listdelivnotes();
    virtual void newBudget();
    virtual void listBudgets();
    void about();
    void aboutQt();
/*
private:

    QPrinter *printer;
    QTextEdit *e;
    QString filename;
*/
};


#endif
