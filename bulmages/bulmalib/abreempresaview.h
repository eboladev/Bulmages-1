/***************************************************************************
                          abreempresaview.h  -  description
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
 /** \file abreempresaview.h
   * Contiene la declaración de la clase \ref abreempresaview y sus métodos.
   * @author Tomeu Borrás Riera 
   */
#ifndef ABREEMPRESAVIEW_H
#define ABREEMPRESAVIEW_H

#include <qstring.h>
#include "abreempresadlg.h"

/** @brief  Clase que muestra la lista de empresas para seleccionar aquella con la que se desea trabajar.
  *
  * Esta clase muestra el diálogo de seleccionar una empresa y espera a que se haga doble click sobre uno de los
  * elementos que contiene, se haga click en aceptar o se de la acción de cancelar.
  * La ventan siempre se abre en modo modal y detiene la ejecución del programa.<BR>
  * Se puede especificar, si es preciso, un tipo de empresas a mostrar (bulmacont o bulmafact) para facilitar la
  * selección de elementos.
*/
class abreempresaview : public abreempresadlg  {
   Q_OBJECT
   
private:
   /// Se usa para almacenar el nombre de la base de datos seleccionada.
   QString m_empresabd;
   /// Se usa para almacenar el nombre de la empresa seleccionada. 
   QString m_nombreempresa;
   /// Se usa para tener presente que tipo de elementos queremos que el selector muestre.
   QString m_tipo;
   /// Se usa para devolver el tipo de empresa que se ha seleccionado en el selector.
   QString m_tipoempresa;
   /// Almacena el nombre del usuario que desea acceder a la contabilidad.
    QString user;
   /// Almacena el password del usuario que desa acceder a la contabilidad.
    QString password;
       
public:
   /// \brief Inicia las variables m_tipo y m_tipoempresa y carga el archivo para hacer la presentación.
   abreempresaview(QWidget *parent=0,QString tipo=0, const char *name=0, bool modal=true);
   ~abreempresaview();
   /// \brief Devuelve el nombre de la base de datos seleccionada.
   QString nomDB() {return m_empresabd;};
   /// \brief Devuelve el nombre de la empresa seleccionada.
   QString nomEmpresa() {return m_nombreempresa;};
   /// \brief Devuelve el tipo de empresa que se ha seleccionado.
   QString tipoEmpresa() {return m_tipoempresa;};
private:
   /// \brief Guarda en el archivo especificado en \ref LISTEMPRESAS la lista de empresas disponibles.
   void guardaArchivo();
   /// \brief Carga del archivo especificado en \ref LISTEMPRESAS la lista de empresas disponibles.
   void cargaArchivo();

private:
     /// \brief Inserta una compañia en el QList empresas definido en el diálogo
    void insertCompany(QString , QString , QString , QString);

private slots:
    /// \brief Responde al evento de cerrar la venta, en cuyo caso devuelve el control al llamante.
    virtual void closeEvent(QCloseEvent * e);
    /// \brief Al pulsar el botón de cancelar se aborta la ejecución del programa.
    virtual void s_botonCancelar() {exit(1);};
        
public slots:
   /// \brief This SLOT is activated to select a company and to press enter.
   virtual void accept();
   /// \brief This SLOT searches in the databases of postgres all the companyes needed.
   virtual void s_reloadButton();
};

#endif
