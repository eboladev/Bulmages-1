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

#include "busquedacuentabase.h"
#include <qlineedit.h>
#include <qlabel.h>

#include "postgresiface2.h"
#include "empresa.h"

/**
@author Tomeu Borras
*/
class BusquedaCuenta : public BusquedaCuentaBase
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

    void setempresa(empresa *comp) {
	empresaactual = comp; 
	conexionbase= empresaactual->bdempresa(); 
	numdigitos=empresaactual->numdigitosempresa();
	};

    QString text()  {return mdb_codigocuenta;};
    virtual void setText(QString val) {setcodigocuenta(val);};

    virtual QString codigocuenta() {return m_codigocuenta->text();};
    virtual QString idcuenta() {return mdb_idcuenta;};
    virtual QString nomcuenta() {return mdb_nomcuenta;};
    virtual void setidcuenta(QString val);
    virtual void setcodigocuenta(QString val);
	void hideNombre() {m_nomcuenta->hide();};
	void showNombre() {m_nomcuenta->show();};
public slots:
	virtual void s_searchCuenta();
	virtual void s_codigocuentatextChanged(const QString &);
	virtual void s_lostFocus();
	virtual void s_returnPressed() {s_lostFocus();emit returnPressed();};

	virtual void selectAll() {m_codigocuenta->selectAll();};
	virtual void setFocus() {m_codigocuenta->setFocus();};
signals:
	void valueChanged(QString );
	void returnPressed();
};

#endif
