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
#include <balance1dlg.h>

#include <qlineedit.h>
#include <qdatetimeedit.h>
#include <qfiledialog.h>
#include <qwidget.h>
#include <qtable.h>
#include <qtoolbutton.h>

#include "intapunts3view.h"
#include "listcuentasview1.h"
#include "diarioview1.h"
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
  
class balance1view : public balance1dlg  {
   Q_OBJECT
private:
/// Empresa con la que trabaja la clase.
  empresa *empresaactual;
/// Base de datos con la que trabaja la empresa
  postgresiface2 *conexionbase;
/// Indica el número de dígitos que tienen por defecto las cuentas.
  int numdigitos;
  
/// Puntero a una clase amiga que se podra acceder de otra forma  
  intapunts3view *introapunts;
/// Puntero a una clase amiga que se podra acceder de otra forma  
  diarioview1 *diario;
/// Puntero a una clase amiga que se podra acceder de otra forma  
  extractoview1 *extracto;

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
  void inicializa2(intapunts3view *, diarioview1 *, extractoview1 *);
private:  
  void cargacostes();
  void presentar();

private slots:
  virtual void return_fechafinal();
  virtual void return_fechainicial();
  virtual void return_codigoinicial();
  virtual void return_codigofinal();
  virtual void boton_buscacuentainicial();
  virtual void boton_buscacuentafinal();
  virtual void boton_fechainicial();
  virtual void boton_fechafinal();
  virtual void contextmenu( QListViewItem *, const QPoint &, int);
  virtual void nivelactivated(int);
  virtual void nivelactivated1(int, QListViewItem *);
  virtual void codigo_textChanged(const QString &);
  virtual void fecha_textChanged(const QString &);  
  virtual void boton_extracto1(int);
  virtual void boton_asiento();
  virtual void boton_diario1(int);
  
public slots:
  virtual void boton_imprimir();
  virtual void accept();

};

#endif
