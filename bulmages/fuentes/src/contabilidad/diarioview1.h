/***************************************************************************
                          diarioview1.h  -  description
                             -------------------
    begin                : Thu Jan 9 2003
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

#ifndef DIARIOVIEW1_H
#define DIARIOVIEW1_H

#include "diariodlg1.h"

#include "qlineedit1.h"
#include "qtable1.h"
#include "postgresiface2.h"

#include "filtrardiarioview.h"
#include "intapunts3view.h"
#include "extractoview1.h"
#include "balanceview.h"

class empresa;
class intapunts3view;
class extractoview1;
class balanceview;

/**
 * \author Tomeu Borrás Riera
 * \class diarioview1 diarioview1.h
 * \brief Libro diario
 * \todo Hay que eliminar el ccostes[200] y cambiarlo por un cursor
 * 
 * Clase que muestra el libro diario de la empresa entre dos fechas.
 */

class diarioview1 : public diariodlg1  {
   Q_OBJECT
private:
/// Estas tres variables son las clases sobre las que trabaja. Deberian estar en la empresa.
   intapunts3view *introapunts;
   extractoview1 *extracto;
   balanceview *balance;
/// Esta clase sustituye al listado diseñado con QT Designer
   QTable1 *listado;
/// Base de datos con la que trabaja esta clase.
   postgresiface2 *conexionbase;
/// Tabla auxiliar en la que se apoya el combo box de centros de coste. Esta tabla limita los posibles centros de coste a 200
   int ccostes[200];
/// Clase adyacente a esta que se encarga de toda la parte de filtrado y que ayuda en la composición del query del listado pasandole parámetros.
   filtrardiarioview *filt;
/// Empresa con la que esta trabajando esta clase.
   empresa *empresaactual;
public:
   diarioview1(empresa *, QWidget *parent=0, const char *name=0, int flags=0);
   ~diarioview1();
   void accept();
/// Inicializa la clase con las fechas inicial y final. El último parametro no se usa.  
   void inicializa1(QString, QString, int);
/// Inicializa la clase indicandole cuales son las clases amigas a esta para que pueda mandarles mensajes. (Esta función está pendiente de ser olvidada y de ser todo pasado a la clase \ref empresa.)   
   void inicializa2(intapunts3view *, extractoview1 *, balanceview *);
private:
/// Muestra en pantalla los resultados para los datos introducidos.
   void presentar();
public slots:
/// SLOT que responde a la pulsación del botón imprimir. Es invocado desde la clase \ref empresa
   virtual void boton_imprimir();
/// SLOT que responde a la pulsación del botón guardar listado. Es invocado desde la clase \ref empresa
   virtual void boton_guardar();
/// SLOT que responde a la pulsación del botón de filtrado. El SLOT muestra la pantalla de filtrado \ref filtrardiarioview .Es invocado desde \ref empresa 
   virtual void boton_filtrar();
private slots:
/// SLOT que responde a la petición del extracto de una cuenta determinada.
   virtual void boton_extracto1(int);
/// SLOT que responde a la petición de un balance de sumas y saldos de una cuenta determinada
   virtual void boton_balance1(int);
/// SLOT que responde a la petición de mostrar un asiento determinado.
   virtual void boton_asiento();
/// SLOT que responde a la perdida de foco en la casilla de fecha inicial.  Lo que provoca que se compruebe la integridad de la fecha.
   virtual void return_fechainicial();
/// SLOT que responde a la perdida de foco en la casilla de fecha final.  Lo que provoca que se compruebe la integridad de la fecha.   
   virtual void return_fechafinal();
/// SLOT que responde a la petición de menu contextual sobre un elemento proporcionado del listado.
   virtual void contextmenu(int , int , const QPoint &);   
/// SLOT que responde al cambio del texto en alguno de los input boxes. Se usa para detectar las pulsaciones de + y de *
   virtual void textChanged(const QString &);
/// SLOT que responde a la pulsación del botón de buscar fecha en la fecha inicial.
   virtual void boton_fechainicial();
/// SLOT que responde a la pulsación del botón de buscar fecha en la fecha final.
   virtual void boton_fechafinal();
};
#endif
