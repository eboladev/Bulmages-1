/***************************************************************************
 *   Copyright (C) 2002 by Tomeu Borras Riera                              *
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

#ifndef EMPRESA_H
#define EMPRESA_H

#include <QMessageBox>
#include <QString>
#include <QWorkspace>
#include <QWidget>
#include <QSizePolicy>
#include <QDir>
#include <QAssistantClient>
#include <QLibraryInfo>

#include "balanceview.h"
#include "extractoview1.h"
#include "listado347.h"
#include "postgresiface2.h"
#include "listventanas.h"
#include "qworkspace2.h"
#include "listcuentasview1.h"
#include "empresabase.h"


class selectccosteview;
class selectcanalview;
class BalanceTreeView;
class Asiento1View;
class DiarioView;
class BalanceView;
class AsientosView;
class CuentaView;


/// Soporte principal para cada empresa contable.
/** Clase que soporta todas las funciones de la empresa contable. Supuestamente el programa
    podr&aacute; tener m&uacute;ltiples empresas abiertas al mismo tiempo.
    Esta clase es el coraz&oacute;n del programa donde acaban confluyendo todos los
    dem&aacute; objetos.
    Se encarga de instanciar los objetos necesarios, inicializarlos e invocarlos cuando es
    preciso.
    Tambi&eacute;n debe servir como punto de comunicaci&oacute;n entre los diferentes
    objetos de modo que estos s&oacute;lo tengan referencias a esta clase y haciendo el
    flujo del c&oacute;digo m&aacute;s f&aacute;cil de entender.
    \todo Proporcionar la funcionalidad de comunicaci&oacute;n entre las otras clases. */
class empresa : public EmpresaBase {
private:
    QString nombre;
    int ano;
    QString contrasenya;
    BalanceTreeView *balance1;
    DiarioView *diario;
    extractoview1 *extracto;
    BalanceView *balance;
    Asiento1View *introapunts2;
    AsientosView *m_listasientos;
    listcuentasview1 *m_listcuentas;
    unsigned int numdigitos; /// El n&uacute;mero de d&iacute;gitos que tienen las cuentas.
    /// Hay unos selectores generales, que van a ser estos y que indican con que se trabaja,
    /// luego estos selectores generales se podr&aacute;n invocar desde distintas partes.
    selectccosteview *selccostes;
    selectcanalview *selcanales;

public:
    QString nombreDB;

public:
    empresa();
    virtual ~empresa();
    /// Devuelve un puntero a la clase de la base de datos.
    postgresiface2 *bdempresa();
    BalanceView *balanceempresa();
    extractoview1 *extractoempresa();
    DiarioView *diarioempresa();
    BalanceTreeView *balance1empresa();
    Asiento1View *intapuntsempresa();
    Asiento1View *intapuntsempresa2();
    int numdigitosempresa();
    QString nomuserempresa();
    QString nombreempresa();
    int muestracuentas();
    int nuevacuenta();
    int muestraapuntes1();
    int muestraasientos();
    int propiedadempresa();
    int libromayor();
    int librodiario();
    int librobalance();
    int librobalancetree();
    int registroiva();
    int listado347();
    int inicializa1();
    int boton_siguiente();
    int boton_anterior();
    int boton_primero();
    int boton_ultimo();
    int boton_guardar();
    int boton_imprimir();
    int boton_reload();
    int ccostes();
    int canales();
    int tiposIVA();
    int fPago();
    void cierraempresa();
    void abreempresa();
    void regularizaempresa();
    void regularizaempresa(QString, QString);
    void maximiza();
    void Abrirasientos();
    void Ordenarasientos();
    void Filtro();
    int mpatrimoniales();
    int compbalance();
    int amortizaciones();
    void reemplazacuentaenasientos();
    int cambioejercicio();
    void centrocostedefecto();
    void canaldefecto();
    void bloqueoFechas();
    selectccosteview *getselccostes();
    selectcanalview *getselcanales();
    void recalculasaldos();
    /// Gesti&oacute;n de cobros y pagos.
    int cobPag();
    virtual void s_asiento1();
    CuentaView *newcuentaview();
};

#endif

