#ifndef DEVOLUCION_H
#define DEVOLUCION_H

#include <QTableWidget>

#include "blwidget.h"
#include "ui_devolucion.h"
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


class Devolucion : public BlWidget, public Ui_DevolucionBase2
{
    Q_OBJECT

public:
    int m_value;
    BtTicket *m_ticket;

public:
    Devolucion ( BtCompany *emp, QWidget *parent );
    virtual ~Devolucion();
    virtual void pintar();
    void anadir ( int row );
    void quitar ( int row );

public slots:
    virtual void on_mui_refLineEdit_returnPressed();
    virtual void on_mui_todoButton_released();
    virtual void on_mui_anadirButton_released();
    virtual void on_mui_sacarButton_released();
    virtual void on_mui_cancelarButton_released();
    virtual void on_mui_valeButton_released();
    virtual void on_mui_efectivoButton_released();
    virtual void refreshDevolver();

/// La parte de las teclas
public slots:
    virtual void ticketTableDoubleclick ( int row, int column );
    virtual void devolverTableDoubleclick ( int row, int column );
};

#endif

