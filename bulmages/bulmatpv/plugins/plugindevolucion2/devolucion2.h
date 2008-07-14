#ifndef DEVOLUCION_H
#define DEVOLUCION_H

#include "blwidget.h"
#include <QTableWidget>
#include "ui_devolucion.h"
#include "empresatpv.h"
#include "ticket.h"

class MyDevButton : public QPushButton
{
    Q_OBJECT
public:
    EmpresaTPV *m_emp;
    MyDevButton ( const QString & text, QWidget * parent = 0, EmpresaTPV *emp = NULL );
    ~MyDevButton();

public slots:
    virtual void on_click ( );
};

class Devolucion : public BLWidget, public Ui_DevolucionBase2
{
    Q_OBJECT
public:
    int m_value;
    Ticket *m_ticket;
public:
    Devolucion ( EmpresaTPV *emp, QWidget *parent );
    virtual ~Devolucion();
    virtual void pintar();
    void anadir ( int row );
    void quitar ( int row );
public slots:
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
};

#endif

