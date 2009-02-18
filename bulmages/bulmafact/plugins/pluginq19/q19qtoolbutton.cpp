/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

/// Necesarios para importacion de efactura
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomNode>
#include <QString>
#include <QFileDialog>
#include <QMap>
#include <QList>
#include <QWidget>

#include "q19qtoolbutton.h"
#include "blfunctions.h"
#include "blfixed.h"
#include "cobroslist.h"
#include "cobroview.h"
#include "bfcompany.h"
#include "bldb.h"


///
/**
\param cob
\param parent
**/
Q19QToolButton::Q19QToolButton ( CobrosList *cob , QWidget *parent ) : QToolButton ( parent )
{
    _depura ( "Q19QToolButton::Q19QToolButton", 0 );
    m_cobrosList = cob;
    setBoton();
    _depura ( "END Q19QToolButton::Q19QToolButton", 0 );
}


///
/**
**/
Q19QToolButton::~Q19QToolButton()
{
    _depura ( "Q19QToolButton::~Q19QToolButton", 0 );
    _depura ( "END Q19QToolButton::~Q19QToolButton", 0 );
}


///
/**
**/
void Q19QToolButton::setBoton()
{
    _depura ( "Q19QToolButton::setBoton", 0 );
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( "Generar Q19" );
    setToolTip ( "Generar archivo Q19 de los elementos seleccionados" );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( confpr->valor ( CONF_PROGDATA ) + "icons/q19.png"  ) );
    setIconSize ( QSize ( 22, 22 ) );
    _depura ( "END Q19QToolButton::setBoton", 0 );
}


///
/**
\return
**/
void Q19QToolButton::click()
{
    _depura ( "Q19QToolButton::click", 0 );

    QString fileName = QFileDialog::getSaveFileName ( this, _( "Save file" ),
                       "",
                       _( "*.q19" ) );
    QFile file ( fileName );
    if ( !file.open ( QIODevice::WriteOnly | QIODevice::Text ) )
        return;

    QDataStream out ( &file );
//    out.setCodec("CP1252");
    m_companyact = ( BfCompany * ) m_cobrosList->mainCompany();
    BlSubForm *sub = m_cobrosList->mui_list;

    BlFixed total ( "0.00" );
    int j = 0;
    /// Reseteamos los valores
    for ( int i = 0; i < sub->rowCount(); i++ ) {
        BlDbSubFormRecord *rec = sub->lineaat ( i );
        rec->refresh();
        QString val = rec->DBvalue ( "selector" );
        if ( val == "TRUE" ) {
            /// La primera vez se ponen las cabeceras
            if ( j == 0 ) {
                cabeceraPresentador ( out, rec->DBvalue ( "idcobro" ) );
                cabeceraOrdenante ( out, rec->DBvalue ( "idcobro" ) );
            }
            cobroQ19 ( out, rec->DBvalue ( "idcobro" ) );
            j++;
            total = total + BlFixed ( rec->DBvalue ( "cantcobro" ) );
        } // end if
    } // end for
    totalOrdenante ( out, total.toQString().remove ( ',' ).remove ( '.' ), QString::number ( j ), QString::number ( j + 2 ) );
    totalGeneral ( out, total.toQString().remove ( ',' ).remove ( '.' ), QString::number ( j ), QString::number ( j + 4 ) );
    file.close();

    _depura ( "END Q19QToolButton::click", 0 );
}


///
/**
\param out
\param idcobro
\return
**/
QByteArray Q19QToolButton::cabeceraPresentador ( QDataStream &out, QString idcobro )
{
    _depura ( "Q19QToolButton::cabeceraPresentador", 0 );
    BlDbRecordSet *curcobro = m_companyact->cargacursor ( "SELECT * FROM cobro NATURAL LEFT JOIN banco WHERE idcobro = " + idcobro );

    /// CABECERA PRESENTADOR
    /// Generamos la cabecera presentador
    QString cab_present = "";
    /// Registro en Euros. Longitud: 2
    cab_present.append ( "51" );
    /// Registro de codigo de dato: 80. Longitud: 2
    cab_present.append ( "80" );

    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    BlDbRecordSet *cur = m_cobrosList->mainCompany() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='CIF'" );
    QString nif = cur->valor ( "valor" );
    delete cur;
    QString sufijo = "000";
    QString codpresent = nif + sufijo;
    codpresent = ascii127 ( codpresent ).rightJustified ( 12, '0' );
    if ( codpresent.size() > 12 )  _depura ( "El CIF del ordenante supera la longitud maxima", 2 );
    cab_present.append ( codpresent.toLatin1() );

    /// Fecha de emision del archivo
    QString fechaemfich = QDate::currentDate().toString ( "ddMMyy" );
    cab_present.append ( fechaemfich.toLatin1() );

    /// Espacio libre Longitud: 6
    cab_present.append ( QString ( 6, ' ' ).toLatin1() );

    /// Nombre del cliente Presentador Longitud: 40
    cur = m_cobrosList->mainCompany() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
    QString clientepresentador = cur->valor ( "valor" );
    delete cur;
    clientepresentador = ascii127 ( clientepresentador ).leftJustified ( 40, ' ' );
    clientepresentador = clientepresentador.left ( 40 );
    cab_present.append ( clientepresentador.toLatin1() );

    /// Espacio libre Longitud: 20
    cab_present.append ( QString ( 20, ' ' ).toLatin1() );

    /// Entidad Receptora del fichero Longitud: 4
    QString ent_recept = ascii127 ( curcobro->valor ( "codentidadbanco" ) ).leftJustified ( 4, '0' );
    if ( ent_recept.size() > 4 ) _depura ( "Entidad bancaria supera longitud maxima", 2 );
    cab_present.append ( ent_recept.toLatin1() );

    /// Oficina Receptora del fichero Longitud: 4
    QString ofi_recept = ascii127 ( curcobro->valor ( "codagenciabanco" ) ).leftJustified ( 4, '0' );
    if ( ofi_recept.size() > 4 ) _depura ( "Oficina bancaria supera longitud maxima", 2 );
    cab_present.append ( ofi_recept.toLatin1() );

    /// Espacio libre Longitud: 12
    cab_present.append ( QString ( 12, ' ' ).toLatin1() );

    /// Espacio libre Longitud: 40
    cab_present.append ( QString ( 40, ' ' ).toLatin1() );

    /// Espacio libre Longitud: 14
    cab_present.append ( QString ( 14, ' ' ).toLatin1() );
    out << cab_present.toLatin1()  << "\n";
    delete curcobro;
    _depura ( "END Q19QToolButton::cabeceraPresentador", 0 );
    return cab_present.toLatin1();
}


///
/**
\param out
\param idcobro
\return
**/
QByteArray Q19QToolButton::cabeceraOrdenante ( QDataStream &out, QString idcobro )
{
    _depura ( "Q19QToolButton::cabeceraOrdenante", 0 );
    BlDbRecordSet *curcobro = m_companyact->cargacursor ( "SELECT * FROM cobro NATURAL LEFT JOIN banco WHERE idcobro = " + idcobro );

    /// GENERAMOS LA CABECERA ORDENANTE
    /// REGISTRO DEL ORDENANTE
    QString cab_orden = "";
    /// Registro en Euros. Longitud: 2
    cab_orden.append ( "53" );
    /// Registro de codigo de dato: 80. Longitud: 2
    cab_orden.append ( "80" );

    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    BlDbRecordSet *cur = m_cobrosList->mainCompany() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='CIF'" );
    QString nif = cur->valor ( "valor" );
    delete cur;
    QString sufijo = "000";
    QString codpresent = nif + sufijo;
    codpresent = ascii127 ( codpresent ).rightJustified ( 12, '0' );
    if ( codpresent.size() > 12 ) _depura ( "CIF demasiado largo", 2 );
    cab_orden.append ( codpresent.toLatin1() );

    /// Fecha de emision del archivo
    QString fechaemfich = QDate::currentDate().toString ( "ddMMyy" );
    cab_orden.append ( fechaemfich.toLatin1() );

    /// Fecha de cargo
    QString fechacargo = normalizafecha ( curcobro->valor ( "fechacobro" ) ).toString ( "ddMMyy" );
    cab_orden.append ( fechacargo.toLatin1() );

    /// Nombre del cliente Ordenante Longitud: 40
    cur = m_cobrosList->mainCompany() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
    QString clientepresentador = cur->valor ( "valor" );
    delete cur;
    clientepresentador = ascii127 ( clientepresentador ).leftJustified ( 40, ' ' );
    if ( clientepresentador.size() > 40 ) _depura ( "Nombre de empresa demasiado largo", 2 );
    cab_orden.append ( clientepresentador.toLatin1() );

    /// Entidad Receptora del fichero Longitud: 4
    QString ent_recept = ascii127 ( curcobro->valor ( "codentidadbanco" ) ).leftJustified ( 4, '0' );
    if ( ent_recept.size() > 4 ) _depura ( "Entidad bancaria demasiado larga", 2 );
    cab_orden.append ( ent_recept.toLatin1() );

    /// Oficina Receptora del fichero Longitud: 4
    QString ofi_recept = ascii127 ( curcobro->valor ( "codagenciabanco" ) ).leftJustified ( 4, '0' );
    if ( ofi_recept.size() > 4 ) _depura ( "Oficina bancaria demasiado larga", 2 );
    cab_orden.append ( ofi_recept.toLatin1() );

    /// DC Receptora del fichero Longitud: 2
    QString dc_recept = ascii127 ( curcobro->valor ( "dcbanco" ) ).leftJustified ( 2, '0' );
    if ( dc_recept.size() > 4 ) _depura ( "Control bancario demasiado larga", 2 );
    cab_orden.append ( dc_recept.toLatin1() );

    /// Oficina Receptora del fichero Longitud: 10
    QString cta_recept = ascii127 ( curcobro->valor ( "numcuentabanco" ) ).leftJustified ( 10, '0' );
    if ( cta_recept.size() > 10 ) _depura ( "Cuenta bancaria demasiado larga", 2 );
    cab_orden.append ( cta_recept.toLatin1() );

    /// Espacio libre Longitud: 8
    cab_orden.append ( QString ( 8, ' ' ) );

    /// Procedimiento de realizacion del adeudo (01 o 02) Longitud: 2
    cab_orden.append ( QString ( "01" ).toLatin1() );

    /// Espacio libre Longitud: 10
    cab_orden.append ( QString ( 10, ' ' ) );

    /// Espacio libre Longitud: 40
    cab_orden.append ( QString ( 40, ' ' ) );

    /// Espacio libre Longitud: 14
    cab_orden.append ( QString ( 14, ' ' ) );
    out << cab_orden.toLatin1()  << "\n";
    delete curcobro;
    _depura ( "END Q19QToolButton::cabeceraOrdenante", 0 );
    return cab_orden.toLatin1();
}


///
/**
\param out
\param idcobro
\return
**/
QByteArray Q19QToolButton::cobroQ19 ( QDataStream &out, QString idcobro )
{
    _depura ( "Q19QToolButton::cobroQ19", 0 );
    BlDbRecordSet *curcobro = m_companyact->cargacursor ( "SELECT * FROM cobro NATURAL LEFT JOIN banco WHERE idcobro = " + idcobro );

    /// CABECERA INDIVIDUAL OBLIGATORIO
    QString cab_indob = "";
    /// Registro en Euros. Longitud: 2
    cab_indob.append ( "56" );
    /// Registro de codigo de dato: 80. Longitud: 2
    cab_indob.append ( "80" );
    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    BlDbRecordSet *cur = m_cobrosList->mainCompany() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='CIF'" );
    QString nif = cur->valor ( "valor" );
    delete cur;
    QString sufijo = "000";
    QString codpresent = nif + sufijo;
    codpresent = ascii127 ( codpresent ).rightJustified ( 12, '0' );
    if ( codpresent.size() > 12 ) _depura ( "CIF CLiente demasiado largo", 2 );
    cab_indob.append ( codpresent.toLatin1() );

    /// Codigo de referencia Longitud: 12
    QString cod_ref = curcobro->valor ( "idcliente" );
    cod_ref = ascii127 ( cod_ref ).rightJustified ( 12, '0' );
    cab_indob.append ( cod_ref.toLatin1() );


    /// Nombre del titular de la domiciliacion: 40
    cur = m_companyact->cargacursor ( "SELECT * FROM cliente WHERE idcliente= " + curcobro->valor ( "idcliente" ) );
    QString clientedomiciliacion = cur->valor ( "nomcliente" );

    clientedomiciliacion = ascii127 ( clientedomiciliacion ).leftJustified ( 40, ' ' );
    if ( clientedomiciliacion.size() > 40 ) _depura ( "Nombre cliente demasiado largo", 2 );
    cab_indob.append ( clientedomiciliacion.toLatin1() );


    /// Entidad domiciliacion del fichero Longitud: 4
    QString bancocliente = cur->valor ( "bancocliente" ).remove ( QChar ( ' ' ) );
    if ( bancocliente.size() != 20 )
        _depura ( "Banco de Cliente invalido en el cobro", 2 );
    bancocliente = ascii127 ( bancocliente ).leftJustified ( 20, ' ' );
    QString ent_recept = bancocliente.left ( 4 );
    cab_indob.append ( ent_recept.toLatin1() );

    /// Oficina domiciliacion del fichero Longitud: 4
    bancocliente = bancocliente.left ( 16 );
    QString ofi_recept = bancocliente.left ( 4 );
    cab_indob.append ( ofi_recept.toLatin1() );

    /// DC domiciliacion del fichero Longitud: 2
    bancocliente = bancocliente.left ( 12 );
    QString dc_recept = bancocliente.left ( 2 );
    cab_indob.append ( dc_recept.toLatin1() );

    /// Oficina domiciliacion del fichero Longitud: 10
    bancocliente = bancocliente.left ( 10 );
    QString cta_recept = bancocliente.left ( 10 );
    cab_indob.append ( cta_recept.toLatin1() );

    delete cur;

    /// Total Importe domiciliacion Longitud: 10
    QString importe = curcobro->valor ( "cantcobro" ).remove ( '.' ).remove ( ',' );
    importe = importe.rightJustified ( 10, '0' );
    cab_indob.append ( importe.toLatin1() );

    /// Codigo para devoluciones Longitud : 6
    QString coddev = curcobro->valor ( "idcobro" );
    coddev = coddev.rightJustified ( 6, '0' );
    cab_indob.append ( coddev.toLatin1() );

    /// Codigo de referencia interna Longitud 10
    QString codrefint = curcobro->valor ( "idcobro" );
    codrefint = codrefint.rightJustified ( 10, ' ' );
    cab_indob.append ( codrefint.toLatin1() );

    /// Primer campo de concepto Longitud: 40
    QString concepto = curcobro->valor ( "comentcobro" );
    concepto = ascii127 ( concepto ).leftJustified ( 40, ' ' );
    concepto = concepto.left ( 40 );
    cab_indob.append ( concepto.toLatin1() );

    /// Espacio libre Longitud: 8
    cab_indob.append ( QString ( 8, ' ' ).toLatin1() );
    out << cab_indob.toLatin1() << "\n";
    delete curcobro;
    _depura ( "END Q19QToolButton::cobroQ19", 0 );
    return cab_indob.toLatin1();
}


///
/**
\param out
\param importes
\param ordenantes
\param registros
\return
**/
QByteArray Q19QToolButton::totalOrdenante ( QDataStream &out, QString importes, QString ordenantes, QString registros )
{
    _depura ( "Q19QToolButton::totalOrdenante", 0 );
    /// CABECERA TOTAL ORDENANTE
    QString cab_indob = "";
    /// Registro en Euros. Longitud: 2
    cab_indob.append ( "58" );

    /// Registro de codigo de dato: 80. Longitud: 2
    cab_indob.append ( "80" );

    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    BlDbRecordSet *cur = m_cobrosList->mainCompany() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='CIF'" );
    QString nif = cur->valor ( "valor" );
    delete cur;
    QString sufijo = "000";
    QString codpresent = nif + sufijo;
    codpresent = ascii127 ( codpresent ).rightJustified ( 12, '0' );
    if ( codpresent.size() > 12 ) _depura ( "CIF demasiado largo", 2 );
    cab_indob.append ( codpresent.toLatin1() );


    /// Espacio libre Longitud: 12
    cab_indob.append ( QString ( 12, ' ' ).toLatin1() );


    /// Espacio libre Longitud: 40
    cab_indob.append ( QString ( 40, ' ' ).toLatin1() );

    /// Espacio libre Longitud: 20
    cab_indob.append ( QString ( 20, ' ' ).toLatin1() );

    /// Suma de Importes del Ordenante Longitud: 10
    QString importe = importes;
    importe = importe.rightJustified ( 10, '0' );
    importe = importe.right ( 10 );
    cab_indob.append ( importe.toLatin1() );

    /// Espacio libre Longitud: 6
    cab_indob.append ( QString ( 6, ' ' ).toLatin1() );

    /// Numero de domiciliaciones del ordenante : 10
    QString coddev = ordenantes;
    coddev = coddev.rightJustified ( 10, '0' );
    coddev = coddev.right ( 10 );
    cab_indob.append ( coddev.toLatin1() );

    /// Numero total registros del ordenante : 10
    coddev = registros;
    coddev = coddev.rightJustified ( 10, '0' );
    coddev = coddev.right ( 10 );
    cab_indob.append ( coddev.toLatin1() );

    /// Espacio libre Longitud: 20
    cab_indob.append ( QString ( 20, ' ' ).toLatin1() );

    /// Espacio libre Longitud: 18
    cab_indob.append ( QString ( 18, ' ' ).toLatin1() );


    /// Imprimimos los resultados
    out << cab_indob.toLatin1() << "\n";
    _depura ( "END Q19QToolButton::totalOrdenante", 0 );
    return cab_indob.toLatin1();
}


///
/**
\param out
\param importes
\param domiciliaciones
\param registros
\return
**/
QByteArray Q19QToolButton::totalGeneral ( QDataStream &out, QString importes, QString domiciliaciones, QString registros )
{
    _depura ( "Q19QToolButton::totalGeneral", 0 );
    /// CABECERA INDIVIDUAL OBLIGATORIO
    QString cab_indob = "";
    /// Registro en Euros. Longitud: 2
    cab_indob.append ( "59" );
    /// Registro de codigo de dato: 80. Longitud: 2
    cab_indob.append ( "80" );
    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    BlDbRecordSet *cur = m_cobrosList->mainCompany() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='CIF'" );
    QString nif = cur->valor ( "valor" );
    delete cur;
    QString sufijo = "000";
    QString codpresent = nif + sufijo;
    codpresent = ascii127 ( codpresent ).rightJustified ( 12, '0' );
    codpresent = codpresent.right ( 12 );
    cab_indob.append ( codpresent.toLatin1() );


    /// Espacio libre Longitud: 12
    cab_indob.append ( QString ( 12, ' ' ).toLatin1() );


    /// Espacio libre Longitud: 40
    cab_indob.append ( QString ( 40, ' ' ).toLatin1() );

    /// Numero de Ordenantes Longitud : 4
    /// PAra este algoritmo siempre es un ordenante
    QString coddev = "1";
    coddev = coddev.rightJustified ( 4, '0' );
    cab_indob.append ( coddev.toLatin1() );

    /// Espacio libre Longitud: 16
    cab_indob.append ( QString ( 16, ' ' ).toLatin1() );

    /// Suma de Importes Longitud: 10
    QString importe = importes;
    importe = importe.rightJustified ( 10, '0' );
    cab_indob.append ( importe.toLatin1() );

    /// Espacio libre Longitud: 6
    cab_indob.append ( QString ( 6, ' ' ).toLatin1() );

    /// Numero de domiciliaciones : 10
    coddev = domiciliaciones;
    coddev = coddev.rightJustified ( 10, '0' );
    cab_indob.append ( coddev.toLatin1() );

    /// Numero total registros : 10
    coddev = registros;
    coddev = coddev.rightJustified ( 10, '0' );
    cab_indob.append ( coddev.toLatin1() );

    /// Espacio libre Longitud: 20
    cab_indob.append ( QString ( 20, ' ' ).toLatin1() );

    /// Espacio libre Longitud: 18
    cab_indob.append ( QString ( 18, ' ' ).toLatin1() );


    /// Imprimimos los resultados
    out << cab_indob.toLatin1() << "\n";
    _depura ( "END Q19QToolButton::totalGeneral", 0 );
    return cab_indob.toLatin1();
}


