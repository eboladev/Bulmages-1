#ifndef ABREVS_H
#define ABREVS_H

#include <QProcess>
#include <QDockWidget>

#include "blwidget.h"
#include "ui_abrevsbase.h"


class EmpresaTPV;


class Abrevs : public BlWidget, public Ui_AbrevsBase
{
    Q_OBJECT

public:
    QProcess *m_proc;
    QDockWidget *m_doc2;
    Abrevs ( EmpresaBase *emp, QWidget *parent );
    virtual ~Abrevs();

public slots:
    virtual void on_mui_teclado_clicked();
    virtual void on_mui_usuario_clicked();
    virtual void on_mui_aparcar_clicked();
    virtual void on_mui_recuperar_clicked();
    virtual void on_mui_cliente_clicked();

};

#endif



