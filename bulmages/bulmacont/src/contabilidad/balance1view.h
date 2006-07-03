/***************************************************************************
                          balance1view.h  -  description
                             -------------------
    begin                : lun jun 23 2003
    copyright            : (C) 2003 by Tomeu Borrás Riera
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

#ifndef BALANCE1VIEW_H
#define BALANCE1VIEW_H

#include <qwidget.h>
#include "ui_balancetreebase.h"

#include <qlineedit.h>
#include <q3datetimeedit.h>
#include <q3filedialog.h>
#include <qwidget.h>
#include <q3table.h>
#include <qtoolbutton.h>

#include "listcuentasview1.h"
#include "diarioview.h"
#include "extractoview1.h"
#include "qtable1.h"
#include "qlayout.h"
#include "postgresiface2.h"
#include "configuracion.h"

class intapunts3view;
class diarioview1;
class extractoview1;
class empresa;

/** 
  * @author Tomeu Borrás Riera 
  * \class balance1view balance1view.h
  * \brief Pantalla de presentación del balnace de sumas y saldos (no jerarquica)
  * \todo Hay que eliminar el array de ccostes y sustituirlo por un cursor2
  * \bug El array de ccostes no permite más de 200 centros de coste siendo este un error.
  * \todo Hay que eliminar los punteros de extracto - diario - introaputnes y hacerlo pasar todo por la clase empresa.
  * Clase que sirve para presentar en balance de sumas y saldos  en forma de tabla. Permite presentar el balance entre unos rangos definidos y incluyendo las cuentas de un nivel determinado.
  */
  
class balance1view : public QWidget, public Ui_BalanceTreeBase  {
   Q_OBJECT
private:
/// Empresa con la que trabaja la clase.
  empresa *m_companyact;
/// Indica el número de dígitos que tienen por defecto las cuentas.
  int numdigitos;
  
/// Este array contiene los centros de coste por defecto  
  int ccostes[200];

	int m_ccuenta;
	int m_cdenominacion;
	int m_csaldo_ant;
	int m_cdebe;
	int m_chaber;
	int m_csaldo;
	int m_cdebeej;
	int m_chaberej;
	int m_csaldoej;
	int m_cidcuenta;
	int m_cnivel;
	int m_cpadre;

public:
  balance1view(empresa *, QWidget *parent=0, const char *name=0, int flags=0);
  void inicializa1(QString, QString, QString, QString, int);
  ~balance1view();
private:  
  void cargacostes();
  void presentar();

private slots:
  virtual void contextmenu( Q3ListViewItem *, const QPoint &, int);
  virtual void nivelactivated(int);
  virtual void nivelactivated1(int, Q3ListViewItem *);
  virtual void boton_extracto1(int);
  virtual void boton_asiento();
  virtual void boton_diario1(int);
  
public slots:
  virtual void boton_imprimir();
  virtual void accept();

};

#endif
