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

#include "company.h"
#include "pgimportfiles.h"
#include "subform2bf.h"
#include "fichabf.h"


/** Clase que maneja el subformulario de la lista de art&iacute;culos.
    Deriva de SubForm2Bf y agrega la configuracion para tratar especificamente
    con la tabla articulo.
*/
class ArticuloListSubForm : public SubForm2Bf {
    Q_OBJECT

public:
    ArticuloListSubForm(QWidget *parent = 0, const char *name = 0);
    ~ArticuloListSubForm() {}
    ;
};


#include "ui_articleslistbase.h"


/** Clase que maneja la ventana de listado de articulos
    Se encarga del control y manejo de la venta que presenta el listado de articulos.
*/
/// \TODO: Deberia crearse la clase Listado y hacer que esta clase derive de Listado en lugar
/// derivar de Ficha.
class ArticuloList : public Ficha, public Ui_ArticuloListBase, public pgimportfiles {
    Q_OBJECT

private:
    /// El identificador de articulo para el modo seleccion. Indica el articulo seleccionado.
    QString mdb_idarticulo;
    /// El nombre del articulo seleccionado si estamos en modo seleccion.
    QString mdb_nomarticulo;
    /// El codigo completo del articulo seleccionado si estamos en modo seleccion.
    QString mdb_codigocompletoarticulo;
    /// El indicador del modo (Seleccion o Edicion). Esta funcionalidad deberia pasarse a la clase Listado.
    company *m_companyact;

    void inicializar();

public:
    ArticuloList(company *, QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmodo = EditMode);
    virtual ~ArticuloList();
    company *getcompany() {
        return m_companyact;
    };
    QString formaQuery();
    QString idArticle() {
        return mdb_idarticulo;
    };
    QString idarticulo() {
        return mdb_idarticulo;
    };
    QString nomarticulo() {
        return mdb_nomarticulo;
    };
    QString codigocompletoarticulo() {
        return mdb_codigocompletoarticulo;
    };
    void presenta();
    void editArticle(int);
    /// Funciones que se encargan en guardar y cargar la configuracion del listado.
    void guardaconfig();
    void cargaconfig();
    void hideBusqueda() {
        m_busqueda->hide();
    };
    void showBusqueda() {
        m_busqueda->show();
    };

public slots:
    virtual void on_m_filtro_textChanged(const QString &text) {
        if (text.size() >= 3) {
            on_mui_actualizar_clicked();
        } // end if
    };
    virtual void on_mui_list_cellDoubleClicked(int, int);
    virtual void on_mui_list_customContextMenuRequested(const QPoint &);
    virtual void s_imprimir1();
    virtual void on_mui_editar_clicked();
    virtual void on_mui_importar_clicked();
    virtual void on_mui_exportar_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_crear_clicked() {
        m_companyact->s_newArticulo();
    };
    virtual void on_mui_imprimir_clicked() {
        s_imprimir1();
    };
    virtual void on_mui_actualizar_clicked() {
        presenta();
    };
    virtual void on_mui_configurar_toggled(bool checked) {
        if (checked) {
            mui_list->showConfig();
        } else {
            mui_list->hideConfig();
        } // end if
    };
    virtual void on_mui_list_toogledConfig(bool check) {
        mui_configurar->setChecked(check);
    };

signals:
    void selected(QString);
};

#endif

