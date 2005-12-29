/***************************************************************************
 *   Copyright (C) 2004 by J. M. Torres Rigo                               *
 *   joanmi@bulma.net                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef PROVIDERSLIST_H
#define PROVIDERSLIST_H

#include "providerslistbase.h"
#include <q3frame.h>
#include "pgimportfiles.h"

class company;


class providerslist : public providerslistbase, public pgimportfiles {
 Q_OBJECT
public:
   enum edmode {EditMode=0, SelectMode=1};
private:
   company *companyact;
   edmode m_modo;
   QString m_idprovider;
   QString m_cifprovider;
   QString m_nomprovider;
   
public:
    providerslist(company *, QWidget *parent = 0, const char *name = 0, Qt::WFlags flag = 0, edmode editmode=EditMode);
    ~providerslist();
    void inicializa();
    void modoseleccion() {m_modo=SelectMode;};
    void modoedicion() {m_modo=EditMode;};
    QString idprovider() {return m_idprovider;};
    QString cifprovider() {return m_cifprovider;};
    QString nomprovider() {return m_nomprovider;};
    void hideBotonera() {m_botonera->hide();};
    void showBotonera() {m_botonera->show();};
    void hideBusqueda() {m_busqueda->hide();};
    void showBusqueda() {m_busqueda->show();};
    void hideConfiguracion() {m_configuracion->hide();};
    void showConfiguracion() {m_configuracion->show();};
   /// Funciones que se encarga en guardar y cargar la configuracion del listado.
    void guardaconfig();
    void cargaconfig();
public slots:
    virtual void doubleclicked(int, int, int, const QPoint &);
    virtual void contextMenu(int, int, const QPoint &);
    virtual void newprovider();
    virtual void s_findProvider();
    virtual void s_editProvider();
    virtual void s_removeProvider();
    virtual void s_printProviders();
    virtual void s_refreshProveedores() {inicializa();};
    virtual void s_exportar();
    virtual void s_importar();

    virtual void s_mostrarBusqueda() {
    	if (m_busqueda->isVisible())
		hideBusqueda();
	else
		 showBusqueda();
	};
	
    virtual void s_mostrarConfiguracion() {
    	if (m_configuracion->isVisible())
		hideConfiguracion();
	else
		 showConfiguracion();
	};      

	virtual void s_configurar();
};

#endif
