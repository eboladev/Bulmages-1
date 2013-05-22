/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#ifndef FPAGOVIEW_H
#define FPAGOVIEW_H

#include "blfunctions.h"
#include "ui_fpagobase.h"
#include "bfform.h"
#include "blpostgresqlclient.h"
#include "pdefs_pluginbf_formasdepago.h"


class BfCompany;


/// Muestra y administra la ventana de formas de pago.
/** */
class PLUGINBF_FORMASDEPAGO_EXPORT FPagoView : public BfForm, public Ui_FPagoBase
{
    Q_OBJECT

private:
    BlDbRecordSet *m_cursorFPagoView;
    /// Indica cu&aacute;l es el objeto que se est&aacute; mostrando.
    QString mdb_idforma_pago;
    /// Indica si es modo consulta o modo edici&oacute;n. (modifica el comportamiento
    /// del doble click sobre la lista)
    bool m_selectMode;
    QListWidgetItem *m_item;

private:
    /// Se encarga de hacer la carga de la consulta (query) inicial, de mostrar la
    /// lista bien y presentar el elemento que se especifique.
    void pintar();

public:
    FPagoView ( BfCompany * emp, QWidget *parent = 0 );
    ~FPagoView();
    void setSelectMode();
    void setEditMode();
    bool trataModificado();
    QString idFormaPago();
    virtual int save();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_aceptar_clicked();

private slots:
    virtual void on_mui_lista_currentItemChanged ( QListWidgetItem *cur, QListWidgetItem *prev );
    virtual void on_mui_crear_clicked();
};

#endif

