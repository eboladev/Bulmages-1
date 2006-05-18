/***************************************************************************
                          DiarioView.h  -  description
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

#ifndef DIARIOVIEW12_H
#define DIARIOVIEW12_H

/**
 * \author Tomeu Borrás Riera
 * \class DiarioView DiarioView.h
 * \brief Libro diario
 * 
 * Clase que muestra el libro diario de la empresa entre dos fechas.
 */

#include "qlineedit1.h"
#include "qtable1.h"
#include "postgresiface2.h"
#include "diariosubform.h"
#include "ui_diariobase.h"

class empresa;
class filtrardiarioview;

class DiarioView : public QWidget, public Ui_DiarioBase  {
   Q_OBJECT
private:
/// Clase adyacente a esta que se encarga de toda la parte de filtrado y que ayuda en la composición del query del listado pasandole parámetros.
   filtrardiarioview *filt;
/// Empresa con la que esta trabajando esta clase.
   empresa *m_companyact;
public:
   DiarioView(empresa *, QWidget *parent=0, const char *name=0, int flags=0);
   ~DiarioView();
   void accept();
/// Inicializa la clase con las fechas inicial y final. El ltimo parametro no se usa.  
   void inicializa1(QString, QString, int);
private:
/// Muestra en pantalla los resultados para los datos introducidos.
   void presentar();
   void presenta();
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

/// SLOT que responde a la petición de menu contextual sobre un elemento proporcionado del listado.
   virtual void contextmenu(int , int , const QPoint &);   
/// SLOT que responde al cambio del texto en alguno de los input boxes. Se usa para detectar las pulsaciones de + y de *
   virtual void textChanged(const QString &);
};
#endif
