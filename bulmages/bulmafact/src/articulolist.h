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


class ArticuloListSubForm : public SubForm2Bf {
    Q_OBJECT
public:
    ArticuloListSubForm(QWidget *parent = 0, const char *name = 0);
    ~ArticuloListSubForm() {}
    ;
};


#include "ui_articleslistbase.h"


class ArticuloList : public QWidget, private Ui_ArticuloListBase, public pgimportfiles {
    Q_OBJECT

public:
    enum edmode
    {
        EditMode = 0, SelectMode = 1
    };

private:
    QString mdb_idarticulo;
    QString mdb_nomarticulo;
    QString mdb_codigocompletoarticulo;
    company *m_companyact;
    edmode m_modo;
    void inicializar();

public:
    ArticuloList(company *, QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmodo = EditMode);
    ~ArticuloList();
    QString formaQuery();
    QString detalleArticulos();
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
    void Imprimir();
    void editArticle(int);
    /// Funciones que se encargan en guardar y cargar la configuracion del listado.
    void guardaconfig();
    void cargaconfig();

    void modoseleccion() {
        m_modo = SelectMode;
    };
    void modoedicion() {
        m_modo = EditMode;
    };
    void hideBusqueda() {
        m_busqueda->hide();
    };
    void showBusqueda() {
        m_busqueda->show();
    };

public slots:
    virtual void on_m_filtro_textChanged(const QString &text) {
	if(text.size() >=3) 
		on_mui_actualizar_clicked();
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
        if (checked)
            mui_list->showConfig();
        else
            mui_list->hideConfig();
    };
    virtual void on_mui_list_toogledConfig(bool check) {
          mui_configurar->setChecked(check);
    };



signals:
    void selected(QString);
};

#endif

