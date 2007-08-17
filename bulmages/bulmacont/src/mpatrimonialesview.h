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

#ifndef MPATRIMONIALESVIEW_H
#define MPATRIMONIALESVIEW_H

#include <QWidget>
#include <QStringList>

#include "ui_mpatrimonialesbase.h"
#include "blwidget.h"

class Empresa;

///
/** */
class mpatrimonialesview : public QDialog, public Ui_MPatrimonialesBase, public PEmpresaBase {
    Q_OBJECT

public:
    /// Esta variable indica el n&uacute;mero de d&iacute;gitos que tienen las cuentas
    /// de nivel m&aacute;s bajo.
    int numdigitos;
    /// Esta variable indica si se abre para b&uacute;squeda o para edici&oacute;n.
    int modo;
    /// Esta variable indica el nombre de la masa, luego se usar&aacute;.
    QString nommasa;
    /// Esta variable indica el identificador de masa.
    QString idmasa;

public:
    mpatrimonialesview(Empresa *emp, QWidget *parent = 0);
    ~mpatrimonialesview();
    int inicializa();
    void inicializatabla();
    void setmodoselector();
    void setmodoeditor();
    QString getnommasa();
    QString getidmasa();

public slots:
    virtual void dbtabla(int, int, int, const QPoint &);
    virtual void on_mui_nuevo_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_editar_clicked();
    virtual void on_mui_cancelar_clicked();
};

#endif

