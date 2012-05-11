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

#ifndef BCASIENTOVIEW_H
#define BCASIENTOVIEW_H

#include <QWidget>
#include <QString>
#include <QFrame>

#include "blconfiguration.h"
#include "bldatesearch.h"
#include "bcasientoform.h"
#include "ui_bcasientobase.h"
#include "pdefs_pluginbc_asiento.h"


class BcCompany;


/// Clase BcAsientoList. Se encarga de controlar la ventana de introducci&oacute;n de apuntes.
/** Esta es una de las clases m&aacute;s complejas del programa porque controla toda la
    acci&oacute;n y casi toda la interactuaci&oacute;n del usuario con el programa.
    Pretende conseguir una interficie que resulte muy r&iacute;gida y c&oacute;moda para
    el usuario que introduzca datos. Hereda intapunts3dlg. */
class PLUGINBC_ASIENTO_EXPORT BcAsientoList : public BcAsientoForm
{
    Q_OBJECT

private:
    /// Este es el cursor que se usar&aacute; para recorrer la lista de asientos.
    BlDbRecordSet *m_cursorAsientos;

public:
    BcAsientoList ( BcCompany *, QWidget *parent );
    virtual ~BcAsientoList();
    void cargaAsientos();
    void botonFiltrar();
    virtual void muestraAsiento ( QString );
    void situarAsiento ( QString );
    bool esPrimerAsiento();
    bool esUltimoAsiento();
    QString idAsientoAnterior();
    QString idAsientoSiguiente();

public slots:
    void botonInicio();
    void botonFin();
    void botonSiguiente();
    void botonAnterior();
    virtual void on_mui_inicio_released();
    virtual void on_mui_fin_released();
    virtual void on_mui_siguiente_released();
    virtual void on_mui_anterior_released();
};


class PLUGINBC_ASIENTO_EXPORT BcAsientoView : public BcAsientoList, public Ui_BcAsientoBase
{
    Q_OBJECT

private:
    void pintaFecha ( QString val );
    void pintaOrdenAsiento ( QString val );
    void pintaClase ( QString val );
    void pintaComentariosAsiento ( QString text );
    virtual void calculaPintaTotales();
    virtual int save();

public:
    BcAsientoView ( BcCompany *, QWidget *parent = 0, int flags = 0 );
    ~BcAsientoView();
    void muestraAsiento ( QString v );
    void muestraAsiento ( int v );
    /// Desabilitamos el removeWindow ya que esta ventana no debe ser sacada ante un close.
    virtual int removeWindow();
    virtual int afterSave();

public:
    void setFecha ( QString val );
    virtual void trataEstadoBcAsientoForm();
    void modoAsientoAbierto();
    void modoAsientoCerrado();
    void iniciar_asiento_nuevo ( QString nuevoordenasiento = "" );
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_borrar_clicked ( bool );
    virtual void asientoRegularizacion ( QString, QString );
    virtual void asientoCierre ( QString, QString );
    virtual void asientoApertura ( QString );

public slots:
    virtual void on_mui_list_editFinish ( int, int );
    virtual void on_mui_abrirAsiento_clicked();
    virtual void on_mui_cerrarAsiento_clicked();
    virtual void on_mui_guardarAsiento_clicked();
    virtual void on_mui_nuevoAsiento_clicked();
    virtual void on_mui_duplicar_clicked();
    virtual void on_mui_inteligente_clicked();
    virtual void on_mui_fecha_enterPressed();
    virtual void on_mui_configurar_toggled ( bool checked );
    virtual void on_mui_list_toogledConfig ( bool check );
    /// Al pulsar return sobre el n&uacute;mero de asiento se procede como si fuese una
    /// carga de dicho asiento.
    void mui_ordenAsiento_pulsadoIntro();
    virtual void boton_cargarasiento();
};


/// Captura todos los eventos del SpinBox 'mui_ordenAsiento'.
class eventos_mui_ordenAsiento : public QObject
{
    Q_OBJECT

private:
    BcAsientoView *objeto;

public:
    eventos_mui_ordenAsiento ( BcAsientoView *ob );
    ~eventos_mui_ordenAsiento() {};

protected:
    bool eventFilter ( QObject *obj, QEvent *event );
};




class SubForm_Asiento : public QObject
{
    Q_OBJECT

public:
    SubForm_Asiento ( BlSubForm * );
    ~SubForm_Asiento();
public slots:
    void mostrarAsiento ();
    virtual void s_pintaMenu ( QMenu * );
    virtual void s_trataMenu ( QAction * );
};



#endif

