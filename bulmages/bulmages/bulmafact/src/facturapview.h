//
// C++ Interface: facturap
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FACTURAPVIEW_H
#define FACTURAPVIEW_H

#include "facturapbase.h"
#include "facturap.h"
#include "busquedaproveedor.h"
#include "busquedafecha.h"
#include "busquedaformapago.h"
#include "busquedaalmacen.h"

#include <qlineedit.h>
#include <q3textedit.h>
#include <qlabel.h>
#include <qcheckbox.h>

#include "postgresiface2.h"

/** @author Tomeu Borras */

class FacturaProveedorView : public FacturaProveedorBase, public FacturaProveedor
{
  Q_OBJECT

public:
  FacturaProveedorView(company *, QWidget *parent = 0, const char *name = 0);
  ~FacturaProveedorView();
  void inicialize();

  void	pintaidproveedor(QString id) {m_proveedor->setidproveedor(id);}
  ;
  void	pintaNumFacturaProveedor(QString id) {m_numfacturap->setText(id);};

  void    pintafechafacturap(QString id) {m_fechafacturap->setText(id);};
  void    pintadescfacturap(QString id) {m_descfacturap->setText(id);};

  void	pintaComentFacturaProveedor(QString id) {m_comentfacturap->setText(id);};
  void    pintareffacturap(QString id) {m_reffacturap->setText(id);};


  void    pintaidforma_pago(QString id) {m_forma_pago->setidforma_pago(id);};
  void pintaprocesadafacturap(QString id)
  {
    fprintf(stderr,"pintaprocesadafacturap(%s)\n",id.ascii());
    if (id == "t" || id == "TRUE") m_procesadafacturap->setChecked(TRUE);
    else m_procesadafacturap->setChecked(FALSE);
  };


  void    pintatotales(float base, float iva);

public slots:
virtual void s_comentfacturaptextChanged() { setcomentfacturap(m_comentfacturap->text());};

  virtual void s_almacenvalueChanged(QString val) {setidalmacen(val);};
  virtual void s_numfacturaptextChanged(const QString &val) {setnumfacturap(val);};
  virtual void s_reffacturaptextChanged(const QString &val) {setreffacturap(val);};
  virtual void s_codigoserie_facturaptextChanged(const QString &val) {setcodigoserie_facturap(val);};

  virtual void s_proveedorvalueChanged(QString val) {setidproveedor(val);};
  virtual void s_fechafacturapvalueChanged(QString val) {setfechafacturap(val);};
  virtual void s_descfacturaptextChanged(const QString &val) {setdescfacturap(val);};

  virtual void s_forma_pagovalueChanged(QString val) {setidforma_pago(val);};

  virtual void s_saveFacturaProveedor() {guardaFacturaProveedor();};
  virtual void cargaFacturaProveedor(QString id) {FacturaProveedor::cargaFacturaProveedor(id);setCaption("FacturaProveedor   "+mdb_reffacturap);companyact->meteWindow(caption(),this);};
  virtual void s_deleteFacturaProveedor() {borraFacturaProveedor();};
  virtual void s_printFacturaProveedor(){imprimirFacturaProveedor();};

  /// Este slot se activa cuando hay cambios en los subformularios.
  virtual void s_pintaTotales()
  {
    pintatotales(listalineas->calculabase(), listalineas->calculaiva());
  }// end pintaTotales

  virtual void s_procesadafacturapstateChanged(int i)
  {
    fprintf(stderr,"s_procesadafacturapstateChanged(%d)\n",i);
    if (i) setprocesadafacturap("TRUE");
    else setprocesadafacturap("FALSE");
  };

  virtual void s_nuevoCobro();
};

#endif
