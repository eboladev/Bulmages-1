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

#ifndef ABREEMPRESAVIEW_H
#define ABREEMPRESAVIEW_H

#include ".ui/abreempresadlg.h"


#include <qwidget.h>
#include <qlineedit.h>
#include <qtimer.h>
#include <qstring.h>
#include <stdio.h>
#include <qlistbox.h>
#include <qlistview.h>
#include <qmessagebox.h>

#include "postgresiface2.h"
#include "configuracion.h"


class BSelector;
/** @author Tomeu Borrás Riera 
  * @brief  Class that shows the list of companyes to select one
*/

class abreempresaview : public abreempresadlg  {
   Q_OBJECT
   
private:

   QString m_empresabd;
   QString m_nombreempresa;
   /// Se usa para tener presente que tipo de elementos queremos que el selector muestre.
   QString m_tipo;
   /// Se usa para devolver el tipo de empresa que se ha seleccionado en el selector.
   QString m_tipoempresa;
   
private:   
    int intentos;
    QString user;
    QString password;
       
public:
   QString nomDB() {return m_empresabd;};
   QString nomEmpresa() {return m_nombreempresa;};
   /// Devuelve el tipo de empresa que se ha seleccionado.
   QString tipoEmpresa() {return m_tipoempresa;};
   abreempresaview(QWidget *parent=0,QString tipo=0, const char *name=0, bool modal=true);
   ~abreempresaview();
private:
   void guardaArchivo();
   void cargaArchivo();

private:
    void insertCompany(QString , QString , QString , QString);
    void listDB();

private slots:
    virtual void closeEvent(QCloseEvent * e);
    virtual void s_botonCancelar() {exit(1);};
        
public slots:
/// This SLOT is activated to select a company and to press enter.
   virtual void accept();
/// This SLOT searches in the databases of postgres all the companyes needed.
   virtual void s_reloadButton();
   
};

#endif
