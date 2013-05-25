#ifndef TICKETSDEVUELTOS_H
#define TICKETSDEVUELTOS_H

#include <QtWidgets/QTableWidget>

#include "blwidget.h"
#include "ui_ticketsdevueltosbase.h"
#include "btcompany.h"
#include "btticket.h"

class MyDevButtonTD : public QPushButton
{
    Q_OBJECT

public:
    BtCompany *m_emp;
    MyDevButtonTD ( const QString & text, QWidget * parent = 0, BtCompany *emp = NULL );
    ~MyDevButtonTD();

public slots:
    virtual void on_click ( );
};


class TicketsDevueltos : public BlWidget, public Ui_TicketsDevueltosBase
{
    Q_OBJECT

public:
    TicketsDevueltos ( BtCompany *emp, QWidget *parent );
    virtual ~TicketsDevueltos();

public slots:
    virtual void on_mui_cancelar_clicked();
    virtual void on_mui_ref_returnPressed();
};

#endif

