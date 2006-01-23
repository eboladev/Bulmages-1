/***************************************************************************
 *   Copyright (C) 2004 by J. M. Estopa Rey                                *
 *   pepma@telefonica.net                                                  *
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
#ifndef ARTICLESLIST_H
#define ARTICLESLIST_H

#include "articleslistbase.h"
#include <q3frame.h>
#include "company.h"
#include "pgimportfiles.h"

class articleslist : public articleslistbase, public pgimportfiles {
    Q_OBJECT
public:
    enum edmode {EditMode=0, SelectMode=1};
private:
    company *companyact;
    edmode m_modo;
    QString m_idArticle;
    QString mdb_nomarticulo;
    QString mdb_codigocompletoarticulo;

public:
    articleslist(company *, QWidget *parent = 0, const char *name = 0, Qt::WFlags flag = 0, edmode editmodo=EditMode);
    void inicializa();
    void presenta();
    void modoseleccion() {
        m_modo=SelectMode;
    };
    void modoedicion() {
        m_modo=EditMode;
    };
    QString idArticle() {
        return m_idArticle;
    };
    QString idarticulo() {
        return m_idArticle;
    };
    QString nomarticulo() {
        return mdb_nomarticulo;
    };
    QString codigocompletoarticulo() {
        return mdb_codigocompletoarticulo;
    };
    ~articleslist();
    void Imprimir();
    QString formaQuery();
    QString detalleArticulos();
    void hideBusqueda() {
        m_busqueda->hide();
    };
    void showBusqueda() {
        m_busqueda->show();
    };
    void hideConfiguracion() {
        m_configuracion->hide();
    };
    void showConfiguracion() {
        m_configuracion->show();
    };
    void editArticle(int);
    /// Funciones que se encarga en guardar y cargar la configuracion del listado.
    void guardaconfig();
    void cargaconfig();
public slots:

    virtual void newArticle() {
        companyact->s_newArticulo();
    };
    virtual void removeArticle();
    virtual void s_imprimir() {
        Imprimir();
    };
    virtual void s_filtrar() {
        presenta();
    };
    virtual void s_configurar();
    virtual void s_imprimir1();
    virtual void s_editArticle(int, int, int, const QPoint &);
    virtual void s_editArticle();

    virtual void s_importar();
    virtual void s_exportar();

    virtual void s_mostrarBusqueda() {
        if (m_busqueda->isVisible())
            hideBusqueda();
        else
            showBusqueda();
    }
    ;
    virtual void s_mostrarConfiguracion() {
        if (m_configuracion->isVisible())
            hideConfiguracion();
        else
            showConfiguracion();
    }
    ;
};

#endif
