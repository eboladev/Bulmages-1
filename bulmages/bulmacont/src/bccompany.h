/***************************************************************************
 *   Copyright (C) 2002 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org Asociacion Iglues -- Contabilidad Linux         *
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

#ifndef BCCOMPANY_H
#define BCCOMPANY_H

#include <QtWidgets/QMessageBox>
#include <QtCore/QString>
#include <QtWidgets/QWidget>
#include <QtWidgets/QSizePolicy>
#include <QtCore/QDir>
#include <QtCore/QLibraryInfo>

#include "blpostgresqlclient.h"
#include "blwindowlistdock.h"
#include "blworkspace.h"
#include "blmaincompany.h"
#include "blsplashscreen.h"
#include "bldefs.h"


class BcCentroCosteSeleccionarView;
class BcCanalSeleccionarView;
/* class BcAsientoView; */
/* class BcDiarioView; */
/* class BcAsientoListView; */
/* class BcCuentaView; */
class BlCountryView;
/*class BcExtractoView;*/
/*class BcPlanContableListView; */
class BcBulmaCont;


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
class BC_EXPORT BcCompany : public BlMainCompany
{
private:
    BcBulmaCont *m_bulmacont;
    QString nombre;
    int ano;
    QString contrasenya;
    /// El n&uacute;mero de d&iacute;gitos que tienen las cuentas.
    unsigned int numDigitos;
    /// Hay unos selectores generales, que van a ser estos y que indican con que se trabaja,
    /// luego estos selectores generales se podr&aacute;n invocar desde distintas partes.
    BcCentroCosteSeleccionarView *selccostes;
    BcCanalSeleccionarView *selcanales;

public:
    QString nombreDB;

public:
    BcCompany ( BcBulmaCont *bcont );
    virtual ~BcCompany();
    /// Devuelve un puntero a la clase de la base de datos.
    BlPostgreSqlClient *bdempresa();

    int numDigitosEmpresa();
    QString nomuserempresa();
    QString nombreempresa();
//    int muestracuentas();
//    int nuevacuenta();
    int propiedadempresa();
//    int listado347();
    int createMainWindows ( BlSplashScreen * );
    int ccostes();
    int canales();
    int tiposIVA();
    void cerrarAsientos();
    int fPago();
    void maximiza();
    int masasPatrimoniales();
    //int compbalance();
    //int amortizaciones();
    void reemplazacuentaenasientos();
    int cambioejercicio();
    void centroCosteDefecto();
    void canalDefecto();
    BcCentroCosteSeleccionarView *getselccostes();
    BcCanalSeleccionarView *getselcanales();
    void recalculasaldos();
    virtual void s_asiento1();
//    BcCuentaView *newcuentaview();
    void guardaConf();
    void cargaConf();
};

#endif

