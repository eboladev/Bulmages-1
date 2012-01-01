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
    BlDbRecordSet *m_cursorasientos;

public:
    BcAsientoList ( BcCompany *, QWidget *parent );
    virtual ~BcAsientoList();
    void cargaasientos();
    void boton_filtrar();
    virtual void muestraasiento ( QString );
    void situarasiento ( QString );
    bool esprimerasiento();
    bool esultimoasiento();
    QString idasientoanterior();
    QString idasientosiguiente();

public slots:
    void boton_inicio();
    void boton_fin();
    void boton_siguiente();
    void boton_anterior();
};


class PLUGINBC_ASIENTO_EXPORT BcAsientoView : public BcAsientoList, public Ui_BcAsientoBase
{
    Q_OBJECT

private:
    void pintaFecha ( QString val );
    void pintaordenasiento ( QString val );
    void pintaclase ( QString val );
    void pintacomentariosasiento ( QString text );
    virtual void calculaypintatotales();
    void prepareSave();

public:
    BcAsientoView ( BcCompany *, QWidget *parent = 0, int flags = 0 );
    ~BcAsientoView();
    void muestraasiento ( QString v );
    void muestraasiento ( int v );
    /// Desabilitamos el removeWindow ya que esta ventana no debe ser sacada ante un close.
    virtual int removeWindow();
    virtual int afterSave();

public:
    void setFecha ( QString val );
    virtual void trataestadoBcAsientoForm();
    void asientoabiertop();
    void asientocerradop();
    void iniciar_asiento_nuevo ( QString nuevoordenasiento = "" );
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_borrar_clicked ( bool );
    virtual void asiento_regularizacion ( QString, QString );
    virtual void asiento_cierre ( QString, QString );
    virtual void asiento_apertura ( QString );

public slots:
    virtual void on_mui_list_editFinish ( int, int );
    virtual void on_mui_abrirasiento_clicked();
    virtual void on_mui_cerrarasiento_clicked();
    virtual void on_mui_guardarasiento_clicked();
    virtual void on_mui_nuevoasiento_clicked();
    virtual void on_mui_duplicar_clicked();
    virtual void on_mui_inteligente_clicked();
    virtual void on_mui_fecha_enterPressed();
    virtual void on_mui_configurar_toggled ( bool checked );
    virtual void on_mui_list_toogledConfig ( bool check );
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
    BcAsientoView *objeto;

public:
    eventos_mui_ordenasiento ( BcAsientoView *ob );
    ~eventos_mui_ordenasiento() {};

protected:
    bool eventFilter ( QObject *obj, QEvent *event );
};

#endif

