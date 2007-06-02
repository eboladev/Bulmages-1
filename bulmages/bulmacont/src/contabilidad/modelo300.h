/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
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

#ifndef MOD300_H
#define MOD300_H

#include <QCheckBox>

#include "ui_modelo300base.h"
#include "modelosps.h"
#include "numerocuenta.h"


/// Modelo 300 del IVA.
/** Clase para generar el modelo 300 de declaraci&oacute; de IVA trimestral en formato
    postscript.
    Utiliza los formularios oficiales obtenidos de la Agencia Tributaria (http://www.aeat.es)
    convertidos de pdf a postscript. Hereda de \ref Modgenps */
class Mod300ps: public QDialog, public Ui_Modelo300Base, public Modgenps {
    Q_OBJECT

public:
    Mod300ps(QWidget *parent);
    ~Mod300ps();
    void accept();
    void generaps();
    void escribe_cuenta_bancaria(int, int);
    void escribe_postscriptdefs(void);
    void rellena_identificacion();
    void rellena_compensacion();
    void rellena_liquidacion();
    void escribe_codigo_postal(QString);
    float baser0;
    float baser16;
    float baser7;
    float baser4;
    float bases0;
    float bases16;
    float bases7;
    float bases4;
    float ivas4;
    float ivas7;
    float ivas16;
    float ivar4;
    float ivar7;
    float ivar16;
    /// Contenido de la casilla 34 (Resultado).
    float cas34;

private:
    QString *nombresccc;
    QString *numerccc;
    /// C&oacute;digo de Cuenta Corriente.
    numerocuenta *ccc;
    void personalButtonPressed();
    /// Si es true, no tenemos ninguna cuenta de banco en nuestra base de datos.
    bool sincuentasbancarias;

protected:
    QString ano;
    QString empresa;
};

#endif

