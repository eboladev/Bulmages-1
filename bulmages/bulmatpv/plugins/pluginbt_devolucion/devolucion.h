#ifndef DEVOLUCION_H
#define DEVOLUCION_H

#include <QtWidgets/QTableWidget>

#include "blwidget.h"
#include "ui_devolucionbase.h"
#include "btcompany.h"
#include "btticket.h"


class MyDevButton : public QPushButton
{
    Q_OBJECT

public:
    BtCompany *m_emp;
    MyDevButton ( const QString & text, QWidget * parent = 0, BtCompany *emp = NULL );
    ~MyDevButton();

public slots:
    virtual void on_click ( );
};


class Devolucion : public BlWidget, public Ui_DevolucionBase
{
    Q_OBJECT

public:
    int m_value;
    BtTicket *m_ticket;
    QString m_totalin;

public:
    Devolucion ( BtCompany *emp, QWidget *parent );
    virtual ~Devolucion();
    virtual void pintar();

public slots:
    virtual void on_mui_cancelar_clicked();
    virtual void on_mui_ref_returnPressed();
    virtual void on_mui_devolver_clicked();
    virtual void on_mui_browser_anchorClicked ( const QUrl & );
    virtual void on_mui_vale_clicked();
};

#endif

