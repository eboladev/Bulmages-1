/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include <QMessageBox>
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <fstream>

#include "bfcompany.h"
#include "blconfiguration.h"
#include "bldatesearch.h"

#include "blfunctions.h"
#include "q34view.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
Q34View::Q34View ( CarteraPagosList *fac, BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    _depura ( "Q34View::Q34View", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        m_carteraPagosList = fac;

        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        mui_idbanco->setMainCompany ( comp );
        mui_idbanco->setidbanco ( "0" );

        setDbTableName ( "vencimientop" );
        setDbFieldId ( "idvencimientop" );



/// Calculamos el TOTAL y el numero de operaciones
        BlSubForm *sub = m_carteraPagosList->mui_list;

	BlFixed a("0.00");
	int numop = 0;
        /// Reseteamos los valores
        for ( int i = 0; i < sub->rowCount(); i++ ) {
            BlDbSubFormRecord *rec = sub->lineaat ( i );
            rec->refresh();
            QString val = rec->dbValue ( "selector" );
            if ( val == "TRUE" ) {
		if (rec->dbValue("descforma_pago") != "Transferencia") 
			mensajeInfo("Hay Vencimientos que no pueden ser Remesados");
		if (rec->dbValue("estadovencimientop") != "Pendiente") 
			mensajeInfo("Hay vencimientos con estado no Pendiente");
                a = a + BlFixed ( rec->dbValue ( "cantvencimientop" ) );
		numop++;
            } // end if
        } // end for

	mui_total->setText(a.toQString());
	mui_numop->setText(QString::number(numop));

        meteWindow ( windowTitle(), this, FALSE );
        dialogChanges_cargaInicial();

    } catch ( ... ) {
        mensajeInfo ( tr ( "Error al crear el archivo" ) );
    } // end try
    _depura ( "END Q34View::Q34View", 0 );
}


/** No precisa acciones adicionales en el destructor.
*/
Q34View::~Q34View()
{
    _depura ( "Q34View::~Q34View", 0 );
    /// ATENCION: Hacer esto es un error ya que puede machacar procesos dependientes del blformlist.

    _depura ( "END Q34View::~Q34View", 0 );
}



///
/**
\param out
\param idcobro
\return
**/
QByteArray Q34View::cabeceraPresentador ( QTextStream &out, QString idvencimientop )
{
    _depura ( "Q34View::cabeceraPresentador", 0 );

    BlDbRecordSet *curbanco = mainCompany() -> loadQuery ( "SELECT * FROM banco WHERE idbanco = " + mui_idbanco->idbanco() );



    /// CABECERA PRESENTADOR
    /// Generamos la cabecera presentador
    QString cab_present = "";

    QString cab_present1= "";
    QString cab_present2= "";
    QString cab_present3= "";
    QString cab_present4= "";
    QString cab_present5= "";


    /// Registro en Euros. Longitud: 2
    cab_present.append ( "03" );
    /// Registro de codigo de dato: 80. Longitud: 2
    cab_present.append ( "56" );

    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    BlDbRecordSet *cur = m_carteraPagosList->mainCompany() ->loadQuery ( "SELECT * FROM blconfiguration WHERE nombre='CIF'" );
    QString nif = cur->valor ( "valor" );
    delete cur;
    QString sufijo = "000";
    QString codpresent = nif + sufijo;
    codpresent = ascii127 ( codpresent ).rightJustified ( 10, '0' );
    if ( codpresent.size() > 10 )  _depura ( "El CIF del ordenante supera la longitud maxima", 2 );
    cab_present.append ( codpresent.toLatin1() );

    /// Version del cuaderno
    cab_present.append ( "34016" );

    /// Espacio libre Longitud: 7
    cab_present.append ( QString ( 7, ' ' ).toLatin1() );

    /// Como los datos son iguales a los de los otros tipos hacemos la puesta igual.
    cab_present1 = cab_present;
    cab_present2 = cab_present;
    cab_present3 = cab_present;
    cab_present4 = cab_present;
    cab_present5 = cab_present;

    /// Numero de dato
    cab_present.append ( "001" );

    /// Fecha de envio del fichero
    QString fechaemfich = QDate::currentDate().toString ( "ddMMyy" );
    cab_present.append ( fechaemfich.toLatin1() );

    /// Fecha de emision de las ordenes
    cab_present.append ( fechaemfich.toLatin1() );

    /// Entidad Receptora del fichero Longitud: 4
    QString ent_recept = ascii127 ( curbanco->valor ( "codentidadbanco" ) ).leftJustified ( 4, '0' );
    if ( ent_recept.size() > 4 ) _depura ( "Entidad bancaria supera longitud maxima", 2 );
    cab_present.append ( ent_recept.toLatin1() );

    /// Oficina Receptora del fichero Longitud: 4
    QString ofi_recept = ascii127 ( curbanco->valor ( "codagenciabanco" ) ).leftJustified ( 4, '0' );
    if ( ofi_recept.size() > 4 ) _depura ( "Oficina bancaria supera longitud maxima", 2 );
    cab_present.append ( ofi_recept.toLatin1() );

    /// Cuenta Receptora del fichero Longitud: 10
    QString cta_recept = ascii127 ( curbanco->valor ( "numcuentabanco" ) ).leftJustified ( 10, '0' );
    if ( cta_recept.size() > 10 ) _depura ( "Cuenta bancaria supera longitud maxima", 2 );
    cab_present.append ( cta_recept.toLatin1() );

    /// Numero de dato
    cab_present.append ( "0" );

    /// Gastos por cuenta del beneficiario
    cab_present.append ( "2" );

    /// Espacio libre Longitud: 2
    cab_present.append ( QString ( 2, ' ' ).toLatin1() );

    /// DC Receptora del fichero Longitud: 2
    QString dc_recept = ascii127 ( curbanco->valor ( "dcbanco" ) ).leftJustified ( 2, '0' );
    if ( dc_recept.size() > 2 ) _depura ( "Cuenta bancaria supera longitud maxima", 2 );
    cab_present.append ( dc_recept.toLatin1() );

    /// Espacio libre Longitud: 7
    cab_present.append ( QString ( 7, ' ' ).toLatin1() );

    out << cab_present.toLatin1()  << "\n";


     /// PRESENTAMOS LA SEGUNDA LINEA
    /// Numero de dato
    cab_present1.append ( "002" );

    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    cur = m_carteraPagosList->mainCompany() ->loadQuery ( "SELECT * FROM blconfiguration WHERE nombre='NombreEmpresa'" );
    codpresent = cur->valor ( "valor" );
    delete cur;

    codpresent = ascii127 ( codpresent ).rightJustified ( 36, '0' );
    if ( codpresent.size() > 36 )  _depura ( "El CIF del ordenante supera la longitud maxima", 2 );
    cab_present1.append ( codpresent.toLatin1() );

    /// Espacio libre Longitud: 7
    cab_present1.append ( QString ( 7, ' ' ).toLatin1() );

    out << cab_present1.toLatin1()  << "\n";

     /// PRESENTAMOS LA TERCERA LINEA
    /// Numero de dato
    cab_present2.append ( "003" );

    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    cur = m_carteraPagosList->mainCompany() ->loadQuery ( "SELECT * FROM blconfiguration WHERE nombre='DireccionCompleta'" );
    codpresent = cur->valor ( "valor" );
    delete cur;

    codpresent = ascii127 ( codpresent ).rightJustified ( 36, '0' );
    if ( codpresent.size() > 36 )  _depura ( "El CIF del ordenante supera la longitud maxima", 2 );
    cab_present2.append ( codpresent.toLatin1() );

    /// Espacio libre Longitud: 7
    cab_present2.append ( QString ( 7, ' ' ).toLatin1() );

    out << cab_present2.toLatin1()  << "\n";

     /// PRESENTAMOS LA CUARTA LINEA
    /// Numero de dato
    cab_present3.append ( "004" );

    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    cur = m_carteraPagosList->mainCompany() ->loadQuery ( "SELECT * FROM blconfiguration WHERE nombre='Municipio'" );
    codpresent = cur->valor ( "valor" );
    delete cur;

    codpresent = ascii127 ( codpresent ).rightJustified ( 36, '0' );
    if ( codpresent.size() > 36 )  _depura ( "El CIF del ordenante supera la longitud maxima", 2 );
    cab_present3.append ( codpresent.toLatin1() );

    /// Espacio libre Longitud: 7
    cab_present3.append ( QString ( 7, ' ' ).toLatin1() );

    out << cab_present3.toLatin1()  << "\n";

    _depura ( "END Q34QToolButton::cabeceraPresentador", 0 );
    return cab_present.toLatin1();


}


///
/**
\param out
\param idcobro
\return
**/
QByteArray Q34View::cabeceraOrdenante ( QTextStream &out, QString idvencimientop )
{

}


///
/**
\param out
\param idcobro
\return
**/
QByteArray Q34View::cobroQ34 ( QTextStream &out, QString idvencimientop )
{
    _depura ( "Q34View::cobroQ34", 0 );


    BlDbRecordSet *curvencimiento = mainCompany() ->loadQuery ( "SELECT * FROM vencimientop LEFT JOIN facturap ON vencimientop.idfacturap = facturap.idfacturap WHERE idvencimientop = " + idvencimientop );


    QString query = "UPDATE vencimientop SET estadovencimientop = 'Emitido', fechaefvencimientop = now() WHERE idvencimientop = " + idvencimientop;
    mainCompany() -> runQuery(query);


    /// CABECERA INDIVIDUAL OBLIGATORIO
    QString cab_indob = "";
    /// Registro en Euros. Longitud: 2
    cab_indob.append ( "06" );
    /// Registro de codigo de dato: 80. Longitud: 2
    cab_indob.append ( "56" );

    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 10
    BlDbRecordSet *cur = m_carteraPagosList->mainCompany() ->loadQuery ( "SELECT * FROM blconfiguration WHERE nombre='CIF'" );
    QString nif = cur->valor ( "valor" );
    delete cur;
    QString sufijo = "000";
    QString codpresent = nif + sufijo;
    codpresent = ascii127 ( codpresent ).rightJustified ( 10, '0' );
    if ( codpresent.size() > 10 )  _depura ( "El CIF del ordenante supera la longitud maxima", 2 );
    cab_indob.append ( codpresent.toLatin1() );


    /// Nombre del titular de la domiciliacion: 12
    BlDbRecordSet *curproveedor = mainCompany() ->loadQuery ( "SELECT * FROM proveedor WHERE idproveedor= " + curvencimiento->valor ( "idproveedor" ) );
    QString clientedomiciliacion = cur->valor ( "cifproveedor" );

    clientedomiciliacion = clientedomiciliacion.leftJustified ( 12, ' ' );
    if ( clientedomiciliacion.size() > 12 ) _depura ( "CIF Proveedor demasiado largo", 2 );
    cab_indob.append ( clientedomiciliacion.toAscii() );



    QString cab2 = cab_indob;

/// Hasta aqui la zona comun. ahora vienen registros especificos.

// el campo Numero de dato
    cab_indob.append ("010");

// el campo importe
    /// Total Importe domiciliacion Longitud: 12
    QString importe = curvencimiento->valor ( "cantvencimientop" ).remove ( '.' ).remove ( ',' );
    importe = importe.rightJustified ( 12, '0' );
    cab_indob.append ( importe.toAscii() );


// el campo numero de banco.
    /// Entidad domiciliacion del fichero Longitud: 4
    QString bancocliente = cur->valor ( "bancoproveedor" ).remove ( QChar ( ' ' ) );
    if ( bancocliente.size() != 20 )
        _depura ( "Banco de Proveedor invalido en el pago", 2 );
    bancocliente = bancocliente.leftJustified ( 20, ' ' );
    QString ent_recept = bancocliente.left ( 4 );
    cab_indob.append ( ent_recept.toAscii() );

    /// Oficina domiciliacion del fichero Longitud: 4
    QString ofi_recept = bancocliente.right ( 16 );
    ofi_recept = bancocliente.left ( 4 );
    cab_indob.append ( ofi_recept.toAscii() );


    /// cuenta domiciliacion del fichero Longitud: 10
    QString cta_recept  = bancocliente.right ( 10 );
    cta_recept = bancocliente.left ( 10 );
    cab_indob.append ( cta_recept.toAscii() );

/// Libre
    cab_indob.append(" ");

/// 1 nomina, 8 pension, 9 otros conceptos
    cab_indob.append("9");

/// Libre
    cab_indob.append("  ");

    /// DC domiciliacion del fichero Longitud: 2
    QString dc_recept  = bancocliente.right ( 12 );
    dc_recept = bancocliente.left ( 2 );
    cab_indob.append ( dc_recept.toAscii() );

    /// Espacio libre Longitud: 7
    cab_indob.append ( QString ( 7, ' ' ).toLatin1() );

    out << cab_indob.toAscii() << "\n";


/// HACEMOS EL SEGUNDO REGISTRO
    cab_indob = cab2;

// el campo Numero de dato
    cab_indob.append ("011");

// el nombre del proveedor
    QString nomdomiciliacion = ascii127(curproveedor->valor ( "nomproveedor" ));

    nomdomiciliacion = nomdomiciliacion.leftJustified ( 36, ' ' );
    nomdomiciliacion = nomdomiciliacion.left(36);
    cab_indob.append ( nomdomiciliacion.toAscii() );

    /// Espacio libre Longitud: 7
    cab_indob.append ( QString ( 7, ' ' ).toLatin1() );

    out << cab_indob.toAscii() << "\n";

    delete curproveedor;
    delete curvencimiento;

    _depura ( "END Q34View::cobroQ34", 0 );
    return cab_indob.toAscii();
}


///
/**
\param out
\param importes
\param ordenantes
\param registros
\return
**/
QByteArray Q34View::totalOrdenante ( QTextStream &out, QString importes, QString ordenantes, QString registros )
{

}


///
/**
\param out
\param importes
\param domiciliaciones
\param registros
\return
**/
QByteArray Q34View::totalGeneral ( QTextStream &out, QString importes, QString domiciliaciones, QString registros )
{
    _depura ( "Q34View::totalGeneral", 0 );
    /// CABECERA INDIVIDUAL OBLIGATORIO
    QString cab_indob = "";
    /// Registro en Euros. Longitud: 2
    cab_indob.append ( "08" );
    /// Registro de codigo de dato: 80. Longitud: 2
    cab_indob.append ( "56" );
    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 10
    BlDbRecordSet *cur = m_carteraPagosList->mainCompany() ->loadQuery ( "SELECT * FROM blconfiguration WHERE nombre='CIF'" );
    QString nif = cur->valor ( "valor" );
    delete cur;
    QString sufijo = "000";
    QString codpresent = nif + sufijo;
    codpresent = ascii127 ( codpresent ).rightJustified ( 10, '0' );
    if ( codpresent.size() > 10 )  _depura ( "El CIF del ordenante supera la longitud maxima", 2 );
    cab_indob.append ( codpresent.toLatin1() );


    /// Espacio libre Longitud: 12
    cab_indob.append ( QString ( 12, ' ' ).toAscii() );


    /// Espacio libre Longitud: 3
    cab_indob.append ( QString ( 3, ' ' ).toAscii() );

    /// Suma de Importes Longitud: 12
    QString importe = importes;
    importe = importe.rightJustified ( 12, '0' );
    cab_indob.append ( importe.toAscii() );

    /// Numero de domiciliaciones : 8
    QString coddev = domiciliaciones;
    coddev = coddev.rightJustified ( 8, '0' );
    cab_indob.append ( coddev.toAscii() );

    /// Numero total registros : 10
    coddev = registros;
    coddev = coddev.rightJustified ( 10, '0' );
    cab_indob.append ( coddev.toAscii() );

    /// Espacio libre Longitud: 6
    cab_indob.append ( QString ( 6, ' ' ).toAscii() );

    /// Espacio libre Longitud: 7
    cab_indob.append ( QString ( 7, ' ' ).toAscii() );


    /// Imprimimos los resultados
    out << cab_indob.toAscii() << "\n";
    _depura ( "END Q34View::totalGeneral", 0 );
    return cab_indob.toAscii();
}


///
/**
**/
void Q34View::on_mui_aceptar_clicked()
{
    _depura ( "Q34View::on_mui_aceptar_clicked", 0 );
    try {


        QFile file ( mui_archivo->text() );
        if ( !file.open ( QIODevice::WriteOnly | QIODevice::Text ) )
            return;

        QTextStream out ( &file );
        out.setCodec ( "LATIN1" );

        BlSubForm *sub = m_carteraPagosList->mui_list;

        BlFixed total ( "0.00" );
        int j = 0;
        /// Reseteamos los valores
        for ( int i = 0; i < sub->rowCount(); i++ ) {
            BlDbSubFormRecord *rec = sub->lineaat ( i );
            rec->refresh();
            QString val = rec->dbValue ( "selector" );
            if ( val == "TRUE" ) {
                /// La primera vez se ponen las cabeceras
                if ( j == 0 ) {
                    cabeceraPresentador ( out, rec->dbValue ( "idvencimientop" ) );
                } // end if
                cobroQ34 ( out, rec->dbValue ( "idvencimientop" ) );
                j++;
                total = total + BlFixed ( rec->dbValue ( "total" ) );
            } // end if
        } // end for

        totalGeneral ( out, total.toQString().remove ( ',' ).remove ( '.' ), QString::number ( j ), QString::number ( j*2 + 5 ) );
        file.close();

        dialogChanges_cargaInicial();
        close();
    } catch ( ... ) {
        _depura ( "Error al guardar la ficha", 2 );
    } // end try
    _depura ( "END Q34View::on_mui_aceptar_clicked", 0 );
}


void Q34View::on_mui_buscararchivo_clicked() {
 QString fileName = QFileDialog::getSaveFileName(this, tr("Guardar archivo"),
                                                 "",
                                                 tr("Remesas Bancarias (*.q34)"));
mui_archivo->setText(fileName);
}
