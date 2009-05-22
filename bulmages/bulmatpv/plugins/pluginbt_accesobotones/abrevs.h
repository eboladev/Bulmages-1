#ifndef ABREVS_H
#define ABREVS_H

#include <QProcess>
#include <QDockWidget>

#include "blwidget.h"
#include "ui_abrevsbase.h"


class BtCompany;


class Abrevs : public BlWidget, public Ui_AbrevsBase
{
    Q_OBJECT

public:
    QProcess *m_proc;
    QDockWidget *m_doc2;
    Abrevs ( BlMainCompany *emp, QWidget *parent );
    virtual ~Abrevs();

public slots:
    virtual void on_mui_teclado_released();
    virtual void on_mui_usuario_released();
    virtual void on_mui_aparcar_released();
    virtual void on_mui_recuperar_released();
    virtual void on_mui_cliente_released();

};

#endif



