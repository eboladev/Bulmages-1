/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#ifndef COMPANY_H
#define COMPANY_H

#include <QObject>
#include <QAssistantClient>
#include <QLibraryInfo>

#include "postgresiface2.h"
#include "listventanas.h"
#include "qworkspace2.h"
#include "empresabase.h"
#include "splashscreen.h"

class ArticuloList;
class orderslist;
class FacturaView;
class ArticuloView;
class PagoView;
class PedidoClienteView;
class TipoArticuloList;
class FamiliasView;
class ListTipoIVAView;

class Bulmafact;

/// Clase company (empresa).
/** Clase principal del programa donde se almacenan y gestionan
    todos los datos de la empresa con la que se est&aacute; trabajando.
    Deriva de postgresiface ya que tiene el tratamiento de la base de datos. */
class Company : public EmpresaBase
{
private:
    /// Puntero al mainWindow
    Bulmafact *m_bulmafact;

    /// Puntero a la ventana de lista de articulos. Esta ventan ase abre una vez al inicio del programa.
    ArticuloList *m_articleslist;

public:
    Company ( Bulmafact *bges );
    virtual ~Company();
    void listproviders();
    void listClients();
    void listarticles();
    void *newClienteView();
    void *newProveedorView();
    ArticuloView *newArticuloView();
    void createMainWindows ( Splash * );
    void refreshArticles();
    void refreshClientes();
    void refreshProveedores();
    void s_FPago();
    void s_Familias();
    void s_trabajadores();
    void s_bancos();
    void s_seriesFactura();
    void s_newArticulo();
    void s_newClienteView();
    void s_newProveedorView();
    void s_newListConfiguracionView();
    void s_newTipoArticuloList();
    void s_newfamiliasview();
    void s_almacenes();
    void viewPagosList();
    void s_TipoIVAView();
    void s_TasaIVAView();
    void guardaConf();
    void cargaConf();
    void *newPagoView();
    TipoArticuloList *newTipoArticuloList ( QWidget *parent = 0, bool modoConsulta = FALSE );
    FamiliasView *newfamiliasview ( QWidget *parent = 0, bool modoConsulta = FALSE );
};

#endif

