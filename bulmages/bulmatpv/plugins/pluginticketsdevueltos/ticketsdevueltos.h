#ifndef TICKETSDEVUELTOS_H
#define TICKETSDEVUELTOS_H

#include "blwidget.h"
#include <QTableWidget>
#include "ui_ticketsdevueltosbase.h"
#include "empresatpv.h"
#include "ticket.h"

class MyDevButtonTD : public QPushButton
{
    Q_OBJECT
public:
    EmpresaTPV *m_emp;
    MyDevButtonTD ( const QString & text, QWidget * parent = 0, EmpresaTPV *emp = NULL );
    ~MyDevButtonTD();

public slots:
    virtual void on_click ( );
};


class TicketsDevueltos : public BLWidget, public Ui_TicketsDevueltosBase
{
    Q_OBJECT
public:
    TicketsDevueltos ( EmpresaTPV *emp, QWidget *parent );
    virtual ~TicketsDevueltos();
public slots:
    virtual void on_mui_cancelar_clicked();
    virtual void on_mui_ref_returnPressed();
};

#endif

