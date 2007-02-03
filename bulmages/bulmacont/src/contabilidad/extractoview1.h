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

#ifndef EXTRACTOVIEW1_H
#define EXTRACTOVIEW1_H

#include "ui_extractobase.h"
#include "qtable1.h"
#include "postgresiface2.h"
#include "ficha.h"


class empresa;

/// Esta clase mantiene la generaci&oacute;n de extractos contables.
/** A partir de esta panalla se pueden llevar a cabo todos los temas de punteo y
    casaci&oacute;n.
    Y acceso a filtrado. Tambi&eacute;n desde esta pantalla se puede pedir m&aacute;s
    informaci&oacute;n de una cuenta determinada. */
class extractoview1 : public Ficha, public Ui_ExtractoBase {
    Q_OBJECT

public:
    /// El extracto se realiza sobre un cursor que siempre est&aacute; en memoria.
    cursor2 *m_cursorcta;
    /// Siempre se propaga la empresa a estos objetos.
    empresa *m_companyact;

public:
    extractoview1(empresa *, QWidget *parent = 0, int flags = 0);
    ~extractoview1();
    void inicializa1(QString, QString, QString, QString, int);
    void vaciar();
    void presentar();
    void ajustes();
    void boton_filtrar();
    QString imprimeExtractoCuenta(QString);

public slots:
    virtual void on_mui_guardar_clicked();
    virtual void on_mui_actualizar_clicked() {
        accept();
    };
    virtual void on_mui_imprimir_clicked();
    virtual void accept();
    virtual void boton_siguiente();
    virtual void boton_anterior();
    virtual void boton_fin();
    virtual void boton_inicio();
    virtual void boton_imprimir();
    virtual void boton_guardar();
    virtual void on_mui_borrapunteo_clicked();
    virtual void on_mui_casacion_clicked();
    virtual void on_mui_guardarpunteo_clicked();
    virtual void on_mui_cargarpunteos_clicked();
    virtual void on_mui_configurar_clicked() {
        mui_list->showConfig();
    };
};

#endif

