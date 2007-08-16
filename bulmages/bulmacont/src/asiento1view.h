/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera and Josep Burcion            *
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

#ifndef ASIENTO1VIEW_H
#define ASIENTO1VIEW_H

#include <QWidget>
#include <QString>
#include <QFrame>

#include "configuracion.h"
#include "busquedafecha.h"
#include "asiento1.h"

#include "ui_asientobase.h"


class Empresa;


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
    ListAsientos(Empresa *, QWidget *parent);
    virtual ~ListAsientos();
    void cargaasientos();
    void boton_inicio();
    void boton_fin();
    void boton_siguiente();
    void boton_anterior();
    void boton_filtrar();
    virtual void muestraasiento(QString);
    void situarasiento(QString);
    bool esprimerasiento();
    bool esultimoasiento();
    QString idasientoanterior();
    QString idasientosiguiente();
};


class Asiento1View : public ListAsientos, public Ui_AsientoBase {
    Q_OBJECT

private:
    void pintafecha(QString val);
    void pintaordenasiento(QString val);
    void pintaclase(QString val);
    void pintacomentariosasiento(QString text);
    virtual void calculaypintatotales(QString);
    void prepguardar();

public:
    Asiento1View(Empresa *, QWidget *parent = 0, int flags = 0);
    ~Asiento1View();
    void muestraasiento(QString v);
    void muestraasiento(int v);
    /// Desabilitamos el sacaWindow ya que esta ventana no debe ser sacada ante un close.
    virtual int sacaWindow();

public:
    void setFecha(QString val);
    virtual void trataestadoAsiento1();
    void asientoabiertop();
    void asientocerradop();
    void iniciar_asiento_nuevo(QString nuevoordenasiento = "");
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_borrar_clicked(bool);

public slots:
    virtual void on_mui_abrirasiento_clicked();
    virtual void on_mui_cerrarasiento_clicked();
    virtual void on_mui_guardarasiento_clicked();
    virtual void on_mui_nuevoasiento_clicked();
    virtual void on_mui_duplicar_clicked();
    virtual void on_mui_inteligente_clicked();
    virtual void on_mui_fecha_editingFinished();
    /// SLOT que responde a la pulsaci&oacute;n del bot&oacute;n de IVA.
    /// Crea la clase \ref ivaview y la inicializa con el identificador de borrador para que se
    /// presente con los datos ya introducidos. La clase ivaview hace una inserci&oacute;n
    /// o una modificaci&oacute;n seg&uacute;n exista o no una entrada de IVA para dicho
    /// borrador.
    virtual void on_mui_iva_clicked();
    virtual void s_lineaValueChanged();
    /// Al pulsar return sobre el n&uacute;mero de asiento se procede como si fuese una
    /// carga de dicho asiento.
    void mui_ordenasiento_pulsadoIntro();
    virtual void boton_cargarasiento();
};


/// Captura todos los eventos del SpinBox 'mui_ordenasiento'.
class eventos_mui_ordenasiento : public QObject
{
    Q_OBJECT

private:
    Asiento1View *objeto;

public:
    eventos_mui_ordenasiento(Asiento1View *ob);
    ~eventos_mui_ordenasiento() {};

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif

