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

#ifndef FAMILIASVIEW_H
#define FAMILIASVIEW_H

#include "ui_familiasbase.h"
#include "fichabf.h"


class Company;


/// Muestra y administra la ventana de familias de art&iacute;culos.
/** */
class FamiliasView : public FichaBf, public Ui_familiasbase
{
    Q_OBJECT

private:
    /// Indica cu&aacute;l es el registro que se est&aacute; modificando. Sirve para saber los
    /// cambios que hay que guardar despu&eacute;s de que se haya modificado.
    QString m_idfamilia;
    /// Indica si es modo consulta o modo edici&oacute;n. (modifica el comportamiento del
    /// doble click sobre la lista).
    bool m_modoConsulta;
    /// Este semaforo indica si se esta en el proceso de pintado.
    bool m_semaforoPintar;

public:
    FamiliasView ( Company *, QWidget *parent = 0, bool modoConsulta = FALSE );
    ~FamiliasView();
    bool trataModificado();
    void mostrarplantilla();
    void setModoConsulta();
    void setModoEdicion();
    QString codigoCompletoFamilia();
    QString idFamilia();
    QString nombreFamilia();
    void pintar ( QTreeWidgetItem * );
    virtual int borrar();
    virtual void on_mui_aceptar_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_imprimir_clicked();
    virtual int guardar();

public slots:
    virtual void pintar();
    virtual void on_m_listFamilias_itemDoubleClicked ( QTreeWidgetItem * );
    virtual void on_m_listFamilias_currentItemChanged ( QTreeWidgetItem *current, QTreeWidgetItem *previous );
    virtual void on_mui_crear_clicked();

// No es necesario en Qt4.4
//    virtual void on_mui_crearRaiz_clicked();


signals:
    void selected ( QString );
};

#endif

