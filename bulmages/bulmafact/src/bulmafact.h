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
    
private slots:

    void about();
    void aboutQt();
    void listproviders();
/*
private:

    QPrinter *printer;
    QTextEdit *e;
    QString filename;
*/
};


#endif
