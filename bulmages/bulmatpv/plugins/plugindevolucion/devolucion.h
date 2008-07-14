#ifndef DEVOLUCION_H
#define DEVOLUCION_H

#include "blwidget.h"
#include <QTableWidget>
#include "ui_devolucionbase.h"
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

class Devolucion : public BLWidget, public Ui_DevolucionBase
{
    Q_OBJECT
public:
    int m_value;
    Ticket *m_ticket;
    QString m_totalin;
public:
    Devolucion ( EmpresaTPV *emp, QWidget *parent );
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

