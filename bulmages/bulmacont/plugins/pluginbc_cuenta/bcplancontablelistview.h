/***************************************************************************
 *   Copyright (C) 2002 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org Asociación Iglues -- Contabilidad Linux         *
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

#ifndef BCPLANCONTABLELISTVIEW_H
#define BCPLANCONTABLELISTVIEW_H

#include <QEvent>

#include "blfunctions.h"
#include "blpostgresqlclient.h"
#include "ui_bcplancontablelistbase.h"
#include "blimportexport.h"
#include "bcform.h"


class BcCompany;


/// Presenta un listado del plan contable.
/** Esta pantalla tiene dos modos de funcionamiento, en uno act&uacute;a como selector de
    cuentas y en el otro act&uacute;a como soporte para la edici&oacute;n del plan contable. */
class BcPlanContableListView : public BcForm, public Ui_BcPlanContableListBase, public BlImportExport
{
    Q_OBJECT

private:
    int ccuenta, cdesccuenta;
    int cidcuenta, cbloqueada, cnodebe, cnohaber, cregularizacion, cimputacion, ctipocuenta;
    /// Indice para la QListView de la columna que indica el debe actual de la cuenta.
    int cdebe;
    /// Indice para la QListView de la columna que indica el haber actual de la cuenta.
    int chaber;
    int cgrupo;
    /// Indica si se abre para busqueda o para edicion.
    edmode m_modo;
    /// Indica el n&uacute;mero de d&iacute;gitos que tienen por defecto todas las cuentas que se crean.
    unsigned int numDigitos;
    /// El c&oacute;digo de la cuenta que se devuelve.
    QString mdb_codcuenta;
    /// El identificador de la cuenta que se devuelve
    QString mdb_idcuenta;
    QString mdb_desccuenta;
    QList<QTreeWidgetItem *> m_itemList;
    int m_itemListPosition;

public:
    void selectMode();
    void editMode();
    QString codcuenta();
    QString idcuenta();
    QString desccuenta();
    void imprimir();

public:
    BcPlanContableListView ( BcCompany *, QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmode = BL_EDIT_MODE );
    ~BcPlanContableListView();
    int inicializa();
    virtual void on_mui_borrar_clicked();

private:
    void inicializaTabla();
    void setSearchButtonsState();

private slots:
    virtual void on_mui_arbolcuentas_itemClicked ( QTreeWidgetItem *, int );
    virtual void on_mui_arbolcuentas_itemDoubleClicked ( QTreeWidgetItem *, int );

public slots:
    virtual void on_mui_tablacuentas_cellDoubleClicked ( int row, int );
    virtual void on_mui_editar_clicked();
    virtual void on_mui_crear_clicked();
    virtual void on_mui_busqueda_textChanged ( const QString & );
    virtual void on_mui_busqueda_editFinished();
    virtual bool eventFilter ( QObject *, QEvent * );
    virtual void on_mui_exportar_clicked();
    virtual void on_mui_importar_clicked();
    virtual void on_mui_actualizar_clicked();
    virtual void on_mui_anterior_clicked();
    virtual void on_mui_siguiente_clicked();

signals:
    void selected ( QString );
};

#endif

