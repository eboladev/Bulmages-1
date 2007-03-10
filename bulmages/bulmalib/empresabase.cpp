/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
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

#include "empresabase.h"
#include "funcaux.h"

#include "abreempresaview.h"


void EmpresaBase::setListVentanas(listventanas *doc) {
    m_listventanas = doc;
}


QWorkspace2 *EmpresaBase::pWorkspace() {
    return m_pWorkspace;
}


void EmpresaBase::setWorkspace(QWorkspace2 *qw) {
    m_pWorkspace = qw;
}


void EmpresaBase::setProgressBar(QProgressBar *pb) {
    m_progressbar = pb;
}


int EmpresaBase::meteWindow(QString nom, QObject *obj, bool compdup) {
    return m_listventanas->meteWindow(nom, obj, compdup);
}


int EmpresaBase::seleccionaWindow(QString nom, QObject *obj) {
    return m_listventanas->seleccionaWindow(nom, obj);
}


int EmpresaBase::deSeleccionaWindow() {
    return m_listventanas->deSeleccionaWindow();
}


void EmpresaBase::sacaWindow(QObject *nom) {
    m_listventanas->sacaWindow(nom);
}


/// No precisa de operaciones en su construccion.
EmpresaBase::EmpresaBase() {
    _depura("EmpresaBase::EmpresaBase", 0);
    _depura("END EmpresaBase::EmpresaBase", 0);
}


/// El destructor de la clase EmpresaBase borra toda la memoria almacenada.
EmpresaBase::~EmpresaBase() {
    _depura("EmpresaBase::~EmpresaBase", 0);
    /// cerramos todas las ventanas y las DestructiveClose se borran.
    m_listventanas->vaciar();
    /// \NOTE: El vaciado de las ventanas debe hacerse en la clase hija ya que el destructor de la clase derivada se ejecuta primero y se generaria un segfault.
    _depura("END EmpresaBase::~EmpresaBase", 0);
}


/// Inicializa la base de datos que se pasa, si se pasa una cadena vacia
/// entonces invoca el selector de empresa.
void EmpresaBase::init(QString bd, QString tipo) {
    _depura("EmpresaBase::init", 0);
    if (bd == "") {
        bd = searchCompany(tipo);
    } // end if
    /// Hacemos visible el ProgressBar mientras se habre la base de datos y se cargan
    /// los datos de la en las ventanas.
    m_progressbar->setVisible(TRUE);

    inicializa(bd);
    _depura("END EmpresaBase::init", 0);
}


/// Se utiliza para mostrar un selector de empresas abreempresaview
/// Al usuario debe seleccionar una empresa y el sistema empieza la inicializacion de
/// clases a partir de dicha inicializacion.
QString EmpresaBase::searchCompany(QString tipo) {
    /// El cambio de empresa se realiza desde el selector.
    _depura("EmpresaBase::searchCompany", 0);
    abreempresaview *nuevae = new abreempresaview(0, tipo);
    nuevae->exec();
    _depura("Vamos a cambiar la empresa \n", 0);
    QString bd = nuevae->nomDB();
    _depura("Empresa cambiada a " + bd, 0);
    delete nuevae;
    /// Si no se ha seleccionado ninguna base de datos entonces abortamos.
    if (bd == "") {
        exit(1);
    } // end if
    _depura("END EmpresaBase::searchCompany", 0);
    return bd;
}


/// Cambia el estado de la ventana de Indexador. Visible o no visible.
void EmpresaBase::s_indexadorCambiaEstado(bool visible) {
    _depura("company::s_indexadorCambiaEstado", 0);
    m_listventanas->cambiaVisible(visible);
    _depura("END company::s_indexadorCambiaEstado", 0);
}

