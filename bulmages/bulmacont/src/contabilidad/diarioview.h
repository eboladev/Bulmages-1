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

#ifndef DIARIOVIEW12_H
#define DIARIOVIEW12_H

#include "ui_diariobase.h"
#include "fichabc.h"


class empresa;


/// Libro diario.
/** Clase que muestra el libro diario de la empresa entre dos fechas. */
class DiarioView : public Ficha, public Ui::DiarioBase {
    Q_OBJECT

public:
    empresa *m_companyact;
    DiarioView(empresa *, QWidget *parent = 0, int flags = 0);
    virtual ~DiarioView();
    void accept();
    /// Inicializa la clase con las fechas inicial y final. El &uacute;ltimo par&aacute;metro
    /// no se usa.
    void inicializa1(QString, QString, int);

private:
    /// Muestra en pantalla los resultados para los datos introducidos.
    void presentar();

public slots:
    /// Slot que responde a la pulsaci&oacute;n del bot&oacute;n actualizar en el formulario.
    /// Supuestamente debe repintar el formulario con las nuevas opciones de filtrado
    /// que se hayan elegido.
    virtual void on_mui_actualizar_clicked();
    /// Slot que responde a la pulsaci&oacute;n del bot&oacute;n imprimir en el formulario.
    /// Realiza la impresi&oacute;n del listado que tenemos en pantalla.
    virtual void on_mui_imprimir_clicked();
    /// SLOT que responde a la pulsaci&oacute;n del bot&oacute;n imprimir.
    /// Es invocado desde la clase \ref empresa
    virtual void boton_imprimir();
    /// SLOT que responde a la pulsaci&oacute;n del bot&oacute;n guardar listado.
    /// Es invocado desde la clase \ref empresa
    virtual void boton_guardar();
    /// SLOT que responde a la activaci&oacute;n del bot&oacute;n de configurar listado.
    /// Muestra el configurador de subform3 \ref subform3
    virtual void on_mui_configurar_clicked();
};

#endif

