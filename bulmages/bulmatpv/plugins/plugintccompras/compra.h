#ifndef DEVOLUCION_H
#define DEVOLUCION_H

#include "blwidget.h"
#include <QTableWidget>
#include "ui_comprabase.h"
#include "empresatpv.h"
#include "ticket.h"

class MyDevButton1 : public QPushButton
{
    Q_OBJECT
public:
    EmpresaTPV *m_emp;
    MyDevButton1 ( const QString & text, QWidget * parent = 0, EmpresaTPV *emp = NULL );
    ~MyDevButton1();

public slots:
    virtual void on_click ( );
};

class Compra : public BLWidget, public Ui_CompraBase
{
    Q_OBJECT
public:
    int m_value;
    Ticket *m_ticket;
public:
    Compra ( EmpresaTPV *emp, QWidget *parent );
    virtual ~Compra();
//    virtual void pintar();
/*
    void anadir ( int row );
    void quitar ( int row );
*/
public slots:
    virtual void on_mui_aceptar_released();
    virtual void on_mui_codigoarticulo_returnPressed();
/*
    virtual void on_mui_refLineEdit_returnPressed();
    virtual void on_mui_todoButton_clicked();
    virtual void on_mui_anadirButton_clicked();
    virtual void on_mui_sacarButton_clicked();
    virtual void on_mui_cancelarButton_clicked();

    virtual void on_mui_valeButton_clicked();
    virtual void on_mui_efectivoButton_clicked();
    virtual void refreshDevolver();

/// La parte de las teclas
public slots:
    virtual void ticketTableDoubleclick ( int row, int column );
    virtual void devolverTableDoubleclick ( int row, int column );
*/
};

#endif

