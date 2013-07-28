/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

/***************************************************************************
      LISTA DE BUGS CONOCIDOS

      - Las cuentas del estilo 47200477 o 47700472 deformaran los resultados
       ya que la busqueda esta hecha con un LIKE que no contempla estos casos.

      - Las fechas incorrectas producen mal funcionamiento pej: 31/06/2003 no
      dara ningun registro porque el dia 31 no existe.
 ***************************************************************************/

#ifndef LISTREGISTROIVAVIEW_H
#define LISTREGISTROIVAVIEW_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>

#include "ui_listregistroivabase.h"
#include "bccompany.h"


class ListRegistroIvaView : public QWidget, public Ui_ListRegistroIvaBase
{
    Q_OBJECT

private:
    /// Opciones del men&uacute; contextual.
    QAction *m_verreg;
    QAction *m_verasiento;

public:
    BcCompany *m_companyact;

public:
    ListRegistroIvaView ( BcCompany *, QString ejerActual = "", QWidget *parent = 0 );
    ~ListRegistroIvaView();
    void inicializa();

public slots:
    virtual void on_mui_tablasoportado_cellDoubleClicked ( int, int );
    virtual void on_mui_tablarepercutido_cellDoubleClicked ( int, int );
    virtual void on_mui_tablarepercutido_pintaMenu ( QMenu *menu );
    virtual void on_mui_tablasoportado_pintaMenu ( QMenu *menu );
    virtual void on_mui_tablasoportado_trataMenu ( QAction *ac );
    virtual void on_mui_tablarepercutido_trataMenu ( QAction *ac );
    virtual void on_mui_actualizar_clicked();
    virtual void boton_print();
    virtual void on_mui_list_editFinished ( int, int ) {}
    ;
};

#endif
