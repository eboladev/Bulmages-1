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

#ifndef APLINTELIGENTESVIEW_H
#define APLINTELIGENTESVIEW_H

#include <stdlib.h>
#include <stdio.h>

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QToolButton>
#include <QComboBox>
#include <QLabel>
#include <QLayout>
#include <QToolButton>
#include <QDateTime>
#include <QPixmap>

#include <QDomDocument>

#include "listcuentasview1.h"
#include "funcaux.h"
#include "postgresiface2.h"
#include "busquedafecha.h"
#include "busquedacuenta.h"
#include "ui_aplinteligentesbase.h"
#include "ficha.h"


/// Formulario para la introducci&oacute;n de asientos mediante las plantillas.
/** Clase que sirve para introducir asientos en el sistema mediante el uso de plantillas
    predefinidas, o asientos plantilla (\ref ainteligentesview).
 
    A partir de la plantilla se genera el formulario de forma din&aacute;mica. Y una vez
    introducidos los datos se hacen c&aacute;lculos en un bucle resolvedor de
    inc&oaacute;gnitas recursivas. Si no hay bucles infinitos al final se obtienen todos
    los campos necesarios para hacer la introducci&oacute;n de asientos y esta se
    efect&uacute;a.
 
    La clase tiene dos modos de actuar dependiendo de si estamos trabajando con un asiento
    abierto o con uno cerrado. En el primer caso el comportamiento es de todo lo introducido
    al asiento abierto.
    En el segundo caso se genera un asiento nuevo por cada introducci&oacute;n de plantilla.
*/
class aplinteligentesview : public Ficha, public Ui_AplInteligentesBase {
    Q_OBJECT

private:
    empresa *companyact; /// La empresa del programa.
    QDomDocument m_doc;
    QString variablescta[100][3];
    QString variablesfecha[100][3];
    QString variablesnumero[100][3];
    QString variablestexto[100][3];
    QString variablespredefinidas[100][3];
    QString variablesapunte[100][3]; /// Las variables de un apunte.

    int indvariablescta;
    int indvariablesfecha;
    int indvariablesnumero;
    int indvariablestexto;
    int indvariablespredefinidas;
    int indvariablesapunte; /// El &iacute;ndice a las variables de apunte.

    /// Si modo == 0 entonces es modo normal. Si modo == 1 entonces es modo 1 ejecuci&oacute;n.
    /// El modo provoca que la plantilla de asiento inteligente se cierre tras haber pulsado
    /// el bot&oacute;n de aceptar y as&iacute; permite recuperar el control a otros
    /// estamentos. Por defecto el modo es 0.
    int modo;

    QLabel *labelcta[100];
    QLabel *labelfecha[100];
    QLabel *labelnumero[100];
    QLabel *labeltexto[100];
    BusquedaCuenta *varcta[100];
    BusquedaFecha *varfecha[100];
    QLineEdit *varnumero[100];
    QLineEdit *vartexto[100];

    /// Esta variable indica el idainteligente que estamos usando como plantilla.
    int numainteligente;
    /// Esta variable indica en qu&eacute; asiento se va a introducir el asiento inteligente.
    int numasiento;
    /// Esta lista se usa en conjunci&oacute;n con el combo-box para saber los &iacute;ndices
    /// de las plantillas de asientos inteligentes.
    QStringList listasientos;
    int idainteligente;
    unsigned int numdigitos;

public:
    aplinteligentesview(empresa *, QWidget *parent = 0);
    ~aplinteligentesview();
    void inicializa(int);
    void mostrarplantilla();
    void recogevalores();
    void creaasiento();
    void recogevariables(QString, int);
    QString aplicavariable(QString);
    void borrawidgets();
    void selectsiguiente(QObject *);
    void inicializavariables();
    void cifcuenta(int);
    void selectfirst();
    void muestraplantilla(QString);
    void setvalores(QString, QString);
    void setfechaasiento(QString fecha) {
        fechaasiento->setText(fecha);
    }
    /// La funci&oacute;n sirve para especificar el modo de funcionamiento de los asientos
    /// intelgientes. Consultar la variable m&eacute;todo para m&aacute;s detalles.
    void setmodo(int i) {
        modo = i;
    }

public slots:
    void on_mui_aceptar_clicked();
    void return_numero();
    void return_texto();
    void on_mui_comboainteligentes_activated(int index);
    virtual void on_mui_guardar_clicked() {
        guardar();
    }
};

#endif

