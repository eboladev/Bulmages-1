//
// C++ Interface: busquedacuenta
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BUSQUEDACUENTA_H
#define BUSQUEDACUENTA_H

//#include "../contabilidad/empresa.h"
#include "ui_busquedacuentabase.h"
#include <qlineedit.h>
#include <qlabel.h>

#include "postgresiface2.h"

class empresa;

/**
@author Tomeu Borras
*/
class BusquedaCuenta : public QWidget, public Ui_BusquedaCuentaBase
{
Q_OBJECT
private:
empresa *empresaactual;
postgresiface2 *conexionbase;
QString mdb_idcuenta;
QString mdb_nomcuenta;
QString mdb_codigocuenta;
int numdigitos;
public:
    BusquedaCuenta(QWidget *parent = 0, const char *name = 0);
    ~BusquedaCuenta();

    void setempresa(empresa *comp);
    QString text()  {return mdb_codigocuenta;};
    virtual void setText(QString val) {setcodigocuenta(val);};

    virtual QString codigocuenta() {return mui_codigocuenta->text();};
    virtual QString idcuenta() {return mdb_idcuenta;};
    virtual QString nomcuenta() {return mdb_nomcuenta;};
    virtual void setidcuenta(QString val);
    virtual void setcodigocuenta(QString val);
	void hideNombre() {mui_nomcuenta->hide();};
	void showNombre() {mui_nomcuenta->show();};
public slots:
	virtual void on_mui_buscar_clicked() {s_searchCuenta();};
	virtual void s_searchCuenta();

	virtual void on_mui_codigocuenta_textChanged(const QString &q) {s_codigocuentatextChanged(q); };
	virtual void s_codigocuentatextChanged(const QString &);

	virtual void on_lostFocus() {s_lostFocus();};
	virtual void s_lostFocus();

	virtual void on_returnPressed() {s_returnPressed();};
	virtual void s_returnPressed() {s_lostFocus();emit returnPressed();};


	virtual void selectAll() {mui_codigocuenta->selectAll();};

	virtual void setFocus() {mui_codigocuenta->setFocus();};
signals:
	void valueChanged(QString );
	void returnPressed();
};

#endif
