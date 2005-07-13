/***************************************************************************
                          diarioview1.h  -  description
                             -------------------
    begin                : Thu Jan 9 2003
    copyright            : (C) 2003 by Tomeu Borr� Riera
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

class empresa;
class intapunts3view;
class extractoview1;
class balanceview;
class filtrardiarioview;

/**
 * \author Tomeu Borr� Riera
 * \class diarioview1 diarioview1.h
 * \brief Libro diario
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
/// Base de datos con la que trabaja esta clase.
   postgresiface2 *conexionbase;
/// Clase adyacente a esta que se encarga de toda la parte de filtrado y que ayuda en la composici� del query del listado pasandole par�etros.
   filtrardiarioview *filt;
/// Empresa con la que esta trabajando esta clase.
   empresa *empresaactual;
public:
   diarioview1(empresa *, QWidget *parent=0, const char *name=0, int flags=0);
   ~diarioview1();
   void accept();
/// Inicializa la clase con las fechas inicial y final. El ltimo parametro no se usa.  
   void inicializa1(QString, QString, int);
/// Inicializa la clase indicandole cuales son las clases amigas a esta para que pueda mandarles mensajes. (Esta funci� est�pendiente de ser olvidada y de ser todo pasado a la clase \ref empresa.)   
   void inicializa2(intapunts3view *, extractoview1 *, balanceview *);
private:
/// Muestra en pantalla los resultados para los datos introducidos.
   void presentar();
public slots:
/// SLOT que responde a la pulsaci� del bot� imprimir. Es invocado desde la clase \ref empresa
   virtual void boton_imprimir();
/// SLOT que responde a la pulsaci� del bot� guardar listado. Es invocado desde la clase \ref empresa
   virtual void boton_guardar();
/// SLOT que responde a la pulsaci� del bot� de filtrado. El SLOT muestra la pantalla de filtrado \ref filtrardiarioview .Es invocado desde \ref empresa 
   virtual void boton_filtrar();
private slots:
/// SLOT que responde a la petici� del extracto de una cuenta determinada.
   virtual void boton_extracto1(int);
/// SLOT que responde a la petici� de un balance de sumas y saldos de una cuenta determinada
   virtual void boton_balance1(int);
/// SLOT que responde a la petici� de mostrar un asiento determinado.
   virtual void boton_asiento();

/// SLOT que responde a la petici� de menu contextual sobre un elemento proporcionado del listado.
   virtual void contextmenu(int , int , const QPoint &);   
/// SLOT que responde al cambio del texto en alguno de los input boxes. Se usa para detectar las pulsaciones de + y de *
   virtual void textChanged(const QString &);
};
#endif
