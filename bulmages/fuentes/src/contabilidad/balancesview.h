/***************************************************************************
                          balancesview.h  -  description
                             -------------------
    begin                : sáb oct 18 2003
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

#ifndef BALANCESVIEW_H
#define BALANCESVIEW_H

#include <qwidget.h>
#include <balancesdlg.h>

#include "postgresiface2.h"

/**
 * \author Tomeu Borrás Riera
 * \class balancesview balancesview.h
 * \brief Listado de Balances Disponibles
 * 
 * Clase que muestra un listado de todos los balances introducidos en el sistema.
 * Sirve como punto de entrada a la generación de los balances permitiendo la importación / exportación de los mismos, su modificación y su impresión.
 */

class empresa;

class balancesview : public balancesdlg  {
   Q_OBJECT
private:
/// Empresa con la que se trabaja (inicializada con el constructor de clase)
   empresa *empresaactual;
/// Base de datos con la que trabaja toda la clase (Inicializada con el constructor de clase)
   postgresiface2 *conexionbase;
/// Esta variable indica si se abre para busqueda o para edicion.  (edición =0; seleccion = 1 )  
   int m_modo;
/// Indica el nombre del balance seleccionado
   QString m_nomBalance;         
/// Indica el identificador del balance seleccionado.
   QString m_idBalance;
   
public: 
   balancesview(empresa *, QWidget *parent=0, const char *name=0);
   ~balancesview();
   void inicializatabla();
/// Establece el funcionamiento de la ventana como selector (afecta sobre el dobleclick en el listado)   
   void setmodoselector() {m_modo = 1;}
/// Establece el funcionamiento de la ventana como editor (afecta sobre el dobleclick en el listado)   
   void setmodoeditor() {m_modo = 0;}
/// Devuelve el nombre del Balance seleccionado por la clase
   QString nomBalance() {return m_nomBalance;}
/// Devuelve el identificador del Balance que se ha seleccionado   
   QString idBalance() {return m_idBalance;}

private slots:
/// SLOT que responde a la pulsación del botón de nuevo Balance
   virtual void nuevo();
/// SLOT que responde a la pulsación del botón de borrar Balance   
   virtual void borrar();
/// SLOT que responde a la pulsación del botón de imprimir Balance   
   virtual void imprimir();
/// SLOT que responde a la acción de hacer doble click sobre el listado   
   virtual void dbtabla(int, int, int, const QPoint &);
/// SLOT que responde a la pulsación del botón de exportación de balances   
   virtual void boton_exportar();
/// SLOT que responde a la pulsación del botón de importación de balances   
   virtual void boton_importar();
/// SLOT que responde a la pulsación del botón de abrir balance
   virtual void s_abrirBalance();
   
};

#endif
