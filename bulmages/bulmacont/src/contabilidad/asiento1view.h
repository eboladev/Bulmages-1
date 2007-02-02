/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera and Josep Burcion            *
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

#ifndef ASIENTO1VIEW_H
#define ASIENTO1VIEW_H

#include <QWidget>
#include <QString>
#include <QFrame>

#include "configuracion.h"
#include "busquedafecha.h"
#include "asiento1.h"

#include "ui_asientobase.h"

class empresa;


/// Clase ListAsientos. Se encarga de controlar la ventana de introducci&oacute;n de apuntes.
/** Esta es una de las clases m&aacute;s complejas del programa porque controla toda la
    acci&oacute;n y casi toda la interactuaci&oacute;n del usuario con el programa.
    Pretende conseguir una interficie que resulte muy r&iacute;gida y c&oacute;moda para
    el usuario que introduzca datos. Hereda intapunts3dlg. */
class ListAsientos : public Asiento1 {
Q_OBJECT
private:
    /// Este es el cursor que se usar&aacute; para recorrer la lista de asientos.
    cursor2 *cursorasientos;

public:
    ListAsientos(empresa *, QWidget *parent);
    virtual ~ListAsientos();
    void cargaasientos();
    void boton_inicio();
    void boton_fin();
    void boton_siguiente();
    void boton_anterior();
    void boton_filtrar() {
        _depura("Funcion no implementada", 2);
    };
    virtual void muestraasiento(QString) {
        mensajeInfo("Funcion no implementada.");
    };
    void situarasiento(QString);
    bool esprimerasiento() {
        return cursorasientos->esprimerregistro();
    };
    bool esultimoasiento() {
        return cursorasientos->esultimoregistro();
    };
    QString idasientoanterior();
    QString idasientosiguiente();
};


class Asiento1View : public ListAsientos, public Ui_AsientoBase {
    Q_OBJECT

private:
    void pintafecha(QString val) {
        mui_fecha->setText(val);
    };
    void pintaordenasiento(QString val) {
        m_ordenasiento->setText(val);
    };
    void pintaclase(QString val) {
        mui_claseAsiento->setCurrentIndex(val.toInt());
    };
    void pintacomentariosasiento(QString text) {
        mui_comentariosAsiento->setPlainText(text);
    };
    virtual void calculaypintatotales(QString);
    void prepguardar();

public:
    Asiento1View(empresa *, QWidget *parent = 0, int flags = 0);
    ~Asiento1View();

    void muestraasiento(QString v);
    void muestraasiento(int v) {
        muestraasiento(QString::number(v));
    };

public:
    void setFecha(QString val) {
        mui_fecha->setText(val);
    };
    virtual void trataestadoAsiento1();
    void asientoabiertop();
    void asientocerradop();
    void iniciar_asiento_nuevo();

public slots:
    virtual void on_mui_abrirasiento_clicked() {
        abrir();
    };
    virtual void on_mui_cerrarasiento_clicked() {
        prepguardar();
        cerrar();
    };
    virtual void on_mui_guardarasiento_clicked() {
        prepguardar();
        Asiento1::guardar();
    };

    virtual void on_mui_nuevoasiento_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_duplicar_clicked();
    virtual void on_mui_inteligente_clicked();
    virtual void on_mui_fecha_editingFinished();
    /// SLOT que responde a la pulsaci&oacute;n del bot&oacute;n de IVA.
    /// Crea la clase \ref ivaview y la inicializa con el identificador de borrador para que se
    /// presente con los datos ya introducidos. La clase ivaview hace una inserci&oacute;n
    /// o una modificaci&oacute;n seg&uacute;n exista o no una entrada de IVA para dicho
    /// borrador.
    virtual void on_mui_iva_clicked() {
        _depura("on_mui_iva_clicked", 0);
        mui_list->boton_iva();
        _depura("END on_mui_iva_clicked", 0);
    };
    virtual void s_lineaValueChanged() {
        calculaypintatotales(idasiento());
    };
    /// Al pulsar return sobre el n&uacute;mero de asiento se procede como si fuese una
    /// carga de dicho asiento.
    void eturn_numasiento() {
        boton_cargarasiento();
    };
    virtual void boton_cargarasiento();
};

#endif

