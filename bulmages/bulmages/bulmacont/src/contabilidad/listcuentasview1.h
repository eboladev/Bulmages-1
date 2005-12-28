/********************************************************************
                          listcuentasview.h  -  description
                             -------------------
    begin                : Wed Nov 27 2002
    copyright            : (C) 2002 by Tomeu Borrás Riera
    email                : tborras@conetxia.com
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef LISTCUENTASVIEW1_H
#define LISTCUENTASVIEW1_H



#include "postgresiface2.h"
#include "listcuentasdlg1.h"

#include "pgimportfiles.h"
//Added by qt3to4:
#include <QEvent>

/** @author Tomeu Borrás Riera 
  * \brief Presenta un listado del plan contable.
  *
  * Esta pantalla tiene dos modos de funcionamiento, en uno actua como selector de cuentas y en el otro actua como soporte para la edición del plan contable.
*/

class empresa;

class listcuentasview1 : public listcuentasdlg1 , public pgimportfiles  {
Q_OBJECT

private:
/// La base de datos con la que se trabaja
postgresiface2 *conexionbase;
/// La empresa que ha construido todo el tema.
empresa *empresaactual;
int ccuenta,cdesccuenta;
int cidcuenta, cbloqueada, cnodebe, cnohaber, cregularizacion, cimputacion, ctipocuenta;
/// Indice para la QListView de la columna que indica el debe actual de la cuenta
int cdebe;
/// Indice para la QListView de la columna que indica el haber actual de la cuenta
int chaber;
int cgrupo;
 /// Indica si se abre para busqueda o para edicion.
int modo;      
/// Indica el numero de digitos que tienen por defecto todas las cuentas que se crean.
unsigned int numdigitos;
/// El codigo de la cuenta que se devuelve.
QString mdb_codcuenta;      
/// El identificador de la cuenta que se devuelve
QString mdb_idcuenta;
QString mdb_desccuenta;

public:


QString codcuenta() {return mdb_codcuenta;};
QString idcuenta() {return mdb_idcuenta;};
QString desccuenta() {return mdb_desccuenta;};

public:
   listcuentasview1(empresa *, QWidget *parent=0, const char *name=0, bool modal=true);
   ~listcuentasview1();
   int inicializa();
   void setModoLista() {modo=1;};
   void setModoEdicion(){modo=0;};

private:
   void listpulsada(Q3ListViewItem *);
   void listdblpulsada(Q3ListViewItem *);
   void inicializatabla();

public slots:
   virtual void dbtabla(int, int, int, const QPoint &);
   virtual void editarcuenta();
   virtual void borrarcuenta();
   virtual void nuevacuenta();
   virtual void descripcioncambiada(const QString &);
   virtual void eturn_descripcion();
   virtual bool eventFilter( QObject *, QEvent * );
   virtual void s_PrintCuentas();
	virtual void s_exportar();
	virtual void s_importar();
};
#endif
