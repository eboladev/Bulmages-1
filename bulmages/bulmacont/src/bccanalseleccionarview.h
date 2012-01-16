/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras                                    *
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

#ifndef BCCANALSELECCIONARVIEW_H
#define BCCANALSELECCIONARVIEW_H

#include "blfunctions.h"
#include "ui_bccanalseleccionarbase.h"
#include "blwidget.h"


class BcCompany;


///
/** */
class BC_EXPORT BcCanalSeleccionarView : public QDialog, public Ui_BcCanalSeleccionarBase, public BlMainCompanyPointer
{
    Q_OBJECT

public:
    BcCanalSeleccionarView ( BcCompany *emp, QWidget *parent = 0 );
    ~BcCanalSeleccionarView();

private:
    int m_colNomCoste;
    int m_colDescCoste;
    int m_colIdCoste;
    int m_colStatusCoste;
    int m_colCheck;
    int numDigitos;
    QTreeWidgetItemIterator *m_iterador;

public:
    void cargaCanales();
    int firstCanal();
    int nextCanal();
    QString cadCanal();
    QString nomCanal();

public slots:
    void on_mui_todo_clicked();
    void on_mui_nada_clicked();
    void on_mui_invertir_clicked();
};

#endif

