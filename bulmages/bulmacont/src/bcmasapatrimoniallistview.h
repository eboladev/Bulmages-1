/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#ifndef BCMASAPATRIMONIALLISTVIEW_H
#define BCMASAPATRIMONIALLISTVIEW_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QMessageBox>

#include "blfunctions.h"
#include "ui_bcmasapatrimoniallistbase.h"
#include "blwidget.h"


class BcCompany;


///
/** */
class BC_EXPORT BcMasaPatrimonialListView : public QDialog, public Ui_BcMasaPatrimonialListBase, public BlMainCompanyPointer
{
    Q_OBJECT

public:
    /// Esta variable indica el n&uacute;mero de d&iacute;gitos que tienen las cuentas
    /// de nivel m&aacute;s bajo.
    int numDigitos;
    /// Esta variable indica si se abre para b&uacute;squeda o para edici&oacute;n.
    int modo;
    /// Esta variable indica el nombre de la masa, luego se usar&aacute;.
    QString nomMasa;
    /// Esta variable indica el identificador de masa.
    QString idMasa;

public:
    BcMasaPatrimonialListView ( BcCompany *emp, QWidget *parent = 0 );
    ~BcMasaPatrimonialListView();
    int inicializa();
    void inicializaTabla();
    void setModoSelector();
    void setModoEditor();
    QString getNomMasa();
    QString getIdMasa();

public slots:
    virtual void dbtabla ( int, int, int, const QPoint & );
    virtual void on_mui_nuevo_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_editar_clicked();
    virtual void on_mui_cancelar_clicked();
};

#endif
