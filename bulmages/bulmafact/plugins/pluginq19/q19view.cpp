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

#include <fstream>

#include "company.h"
#include "configuracion.h"
#include "busquedafecha.h"

#include "funcaux.h"
#include "q19view.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
Q19View::Q19View ( FacturasList *fac, Company *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "Q19View::Q19View", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        m_facturasList = fac;

        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
//        mui_idcliente->setEmpresaBase ( comp );
//        mui_refcobro->setEmpresaBase ( comp );
        mui_idbanco->setEmpresaBase ( comp );
        mui_idbanco->setidbanco ( "0" );

        setDBTableName ( "factura" );
        setDBCampoId ( "idfactura" );

        /*
                setTitleName ( tr ( "Cobro" ) );
         
                addDBCampo ( "idcobro", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate ( "Cobro", "ID cobro" ) );
                addDBCampo ( "idcliente", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate ( "Cobro", "ID cliente" ) );
                addDBCampo ( "previsioncobro", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate ( "Cobro", "Prevision de cobro" ) );
                addDBCampo ( "fechacobro", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate ( "Cobro", "Fecha de cobro" ) );
                addDBCampo ( "fechavenccobro", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate ( "Cobro", "Fecha de vencimiento" ) );
                addDBCampo ( "refcobro", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Cobro", "Referencia del cobro" ) );
                addDBCampo ( "cantcobro", DBCampo::DBnumeric, DBCampo::DBNotNull, QApplication::translate ( "Cobro", "Cantidad" ) );
                addDBCampo ( "comentcobro", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Cobro", "Comentarios" ) );
                addDBCampo ( "idbanco", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate ( "Banco", "Comentarios" ) );
         
                pintar();
                dialogChanges_cargaInicial();
        */
        meteWindow ( windowTitle(), this, FALSE );
        dialogChanges_cargaInicial();

    } catch ( ... ) {
        mensajeInfo ( tr ( "Error al crear el archivo" ) );
    } // end try
    _depura ( "END Q19View::Q19View", 0 );
}


/** No precisa acciones adicionales en el destructor.
*/
Q19View::~Q19View()
{
    _depura ( "Q19View::~Q19View", 0 );
    /// ATENCION: Hacer esto es un error ya que puede machacar procesos dependientes del listado.
    // ((Company *)empresaBase())->refreshCobrosCliente();
    _depura ( "END Q19View::~Q19View", 0 );
}



///
/**
\param out
\param idcobro
\return
**/
QByteArray Q19View::cabeceraPresentador ( QTextStream &out, QString idfactura )
{
    _depura ( "Q19View::cabeceraPresentador", 0 );
    cursor2 *curcobro = empresaBase() ->cargacursor ( "SELECT * FROM factura WHERE idfactura = " + idfactura );

    cursor2 *curbanco = empresaBase() -> cargacursor ( "SELECT * FROM banco WHERE idbanco = " + mui_idbanco->idbanco() );

    /// CABECERA PRESENTADOR
    /// Generamos la cabecera presentador
    QString cab_present = "";
    /// Registro en Euros. Longitud: 2
    cab_present.append ( "51" );
    /// Registro de codigo de dato: 80. Longitud: 2
    cab_present.append ( "80" );

    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    cursor2 *cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='CIF'" );
    QString nif = cur->valor ( "valor" );
    delete cur;
    QString sufijo = "000";
    QString codpresent = nif + sufijo;
    codpresent = codpresent.rightJustified ( 12, '0' );
    if ( codpresent.size() > 12 )  _depura ( "El CIF del ordenante supera la longitud maxima", 2 );
    cab_present.append ( codpresent.toAscii() );

    /// Fecha de emision del archivo
    QString fechaemfich = QDate::currentDate().toString ( "ddMMyy" );
    cab_present.append ( fechaemfich.toAscii() );

    /// Espacio libre Longitud: 6
    cab_present.append ( QString ( 6, ' ' ).toAscii() );

    /// Nombre del cliente Presentador Longitud: 40
    cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
    QString clientepresentador = cur->valor ( "valor" );
    delete cur;
    clientepresentador = clientepresentador.leftJustified ( 40, ' ' );
    clientepresentador = clientepresentador.left ( 40 );
    cab_present.append ( clientepresentador.toAscii() );

    /// Espacio libre Longitud: 20
    cab_present.append ( QString ( 20, ' ' ).toAscii() );

    /// Entidad Receptora del fichero Longitud: 4
    QString ent_recept = curbanco->valor ( "codentidadbanco" ).leftJustified ( 4, '0' );
    if ( ent_recept.size() > 4 ) _depura ( "Entidad bancaria supera longitud maxima", 2 );
    cab_present.append ( ent_recept.toAscii() );

    /// Oficina Receptora del fichero Longitud: 4
    QString ofi_recept = curbanco->valor ( "codagenciabanco" ).leftJustified ( 4, '0' );
    if ( ofi_recept.size() > 4 ) _depura ( "Oficina bancaria supera longitud maxima", 2 );
    cab_present.append ( ofi_recept.toAscii() );

    /// Espacio libre Longitud: 12
    cab_present.append ( QString ( 12, ' ' ).toAscii() );

    /// Espacio libre Longitud: 40
    cab_present.append ( QString ( 40, ' ' ).toAscii() );

    /// Espacio libre Longitud: 14
    cab_present.append ( QString ( 14, ' ' ).toAscii() );
    out << cab_present.toAscii()  << "\n";
    delete curcobro;
    delete curbanco;
    _depura ( "END Q19View::cabeceraPresentador", 0 );
    return cab_present.toAscii();
}


///
/**
\param out
\param idcobro
\return
**/
QByteArray Q19View::cabeceraOrdenante ( QTextStream &out, QString idfactura )
{
    _depura ( "Q19View::cabeceraOrdenante", 0 );
    cursor2 *curcobro = empresaBase() ->cargacursor ( "SELECT * FROM factura WHERE idfactura = " + idfactura );

    cursor2 *curbanco = empresaBase() -> cargacursor ( "SELECT * FROM banco WHERE idbanco = " + mui_idbanco->idbanco() );


    /// GENERAMOS LA CABECERA ORDENANTE
    /// REGISTRO DEL ORDENANTE
    QString cab_orden = "";
    /// Registro en Euros. Longitud: 2
    cab_orden.append ( "53" );
    /// Registro de codigo de dato: 80. Longitud: 2
    cab_orden.append ( "80" );

    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    cursor2 *cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='CIF'" );
    QString nif = cur->valor ( "valor" );
    delete cur;
    QString sufijo = "000";
    QString codpresent = nif + sufijo;
    codpresent = codpresent.rightJustified ( 12, '0' );
    if ( codpresent.size() > 12 ) _depura ( "CIF demasiado largo", 2 );
    cab_orden.append ( codpresent.toAscii() );

    /// Fecha de emision del archivo
    QString fechaemfich = QDate::currentDate().toString ( "ddMMyy" );
    cab_orden.append ( fechaemfich.toAscii() );

    /// Fecha de cargo
    QString fechacargo = normalizafecha ( curcobro->valor ( "fechacobro" ) ).toString ( "ddMMyy" );
    cab_orden.append ( fechacargo.toAscii() );

    /// Nombre del cliente Ordenante Longitud: 40
    cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
    QString clientepresentador = cur->valor ( "valor" );
    delete cur;
    clientepresentador = clientepresentador.leftJustified ( 40, ' ' );
    if ( clientepresentador.size() > 40 ) _depura ( "Nombre de empresa demasiado largo", 2 );
    cab_orden.append ( clientepresentador.toAscii() );

    /// Entidad Receptora del fichero Longitud: 4
    QString ent_recept = curbanco->valor ( "codentidadbanco" ).leftJustified ( 4, '0' );
    if ( ent_recept.size() > 4 ) _depura ( "Entidad bancaria demasiado larga", 2 );
    cab_orden.append ( ent_recept.toAscii() );

    /// Oficina Receptora del fichero Longitud: 4
    QString ofi_recept = curbanco->valor ( "codagenciabanco" ).leftJustified ( 4, '0' );
    if ( ofi_recept.size() > 4 ) _depura ( "Oficina bancaria demasiado larga", 2 );
    cab_orden.append ( ofi_recept.toAscii() );

    /// DC Receptora del fichero Longitud: 2
    QString dc_recept = curbanco->valor ( "dcbanco" ).leftJustified ( 2, '0' );
    if ( dc_recept.size() > 4 ) _depura ( "Control bancario demasiado larga", 2 );
    cab_orden.append ( dc_recept.toAscii() );

    /// Oficina Receptora del fichero Longitud: 10
    QString cta_recept = curbanco->valor ( "numcuentabanco" ).leftJustified ( 10, '0' );
    if ( cta_recept.size() > 10 ) _depura ( "Cuenta bancaria demasiado larga", 2 );
    cab_orden.append ( cta_recept.toAscii() );

    /// Espacio libre Longitud: 8
    cab_orden.append ( QString ( 8, ' ' ) );

    /// Procedimiento de realizacion del adeudo (01 o 02) Longitud: 2
    cab_orden.append ( QString ( "01" ).toAscii() );

    /// Espacio libre Longitud: 10
    cab_orden.append ( QString ( 10, ' ' ) );

    /// Espacio libre Longitud: 40
    cab_orden.append ( QString ( 40, ' ' ) );

    /// Espacio libre Longitud: 14
    cab_orden.append ( QString ( 14, ' ' ) );
    out << cab_orden.toAscii()  << "\n";
    delete curcobro;
    delete curbanco;
    _depura ( "END Q19View::cabeceraOrdenante", 0 );
    return cab_orden.toAscii();
}


///
/**
\param out
\param idcobro
\return
**/
QByteArray Q19View::cobroQ19 ( QTextStream &out, QString idfactura )
{
    _depura ( "Q19View::cobroQ19", 0 );
    cursor2 *curcobro = empresaBase() ->cargacursor ( "SELECT * FROM factura WHERE idfactura = " + idfactura );

//    cursor2 *curbanco = empresaBase()-> cargacursor( "SELECT * FROM banco WHERE idbanco = " + mui_idbanco->idbanco() );


    /// CABECERA INDIVIDUAL OBLIGATORIO
    QString cab_indob = "";
    /// Registro en Euros. Longitud: 2
    cab_indob.append ( "56" );
    /// Registro de codigo de dato: 80. Longitud: 2
    cab_indob.append ( "80" );
    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    cursor2 *cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='CIF'" );
    QString nif = cur->valor ( "valor" );
    delete cur;
    QString sufijo = "000";
    QString codpresent = nif + sufijo;
    codpresent = codpresent.rightJustified ( 12, '0' );
    if ( codpresent.size() > 12 ) _depura ( "CIF CLiente demasiado largo", 2 );
    cab_indob.append ( codpresent.toAscii() );

    /// Codigo de referencia Longitud: 12
    QString cod_ref = curcobro->valor ( "idcliente" );
    cod_ref = cod_ref.rightJustified ( 12, '0' );
    cab_indob.append ( cod_ref.toAscii() );


    /// Nombre del titular de la domiciliacion: 40
    cur = empresaBase() ->cargacursor ( "SELECT * FROM cliente WHERE idcliente= " + curcobro->valor ( "idcliente" ) );
    QString clientedomiciliacion = cur->valor ( "nomcliente" );

    clientedomiciliacion = clientedomiciliacion.leftJustified ( 40, ' ' );
    if ( clientedomiciliacion.size() > 40 ) _depura ( "Nombre cliente demasiado largo", 2 );
    cab_indob.append ( clientedomiciliacion.toAscii() );


    /// Entidad domiciliacion del fichero Longitud: 4
    QString bancocliente = cur->valor ( "bancocliente" ).remove ( QChar ( ' ' ) );
    if ( bancocliente.size() != 20 )
        _depura ( "Banco de Cliente invalido en el cobro", 2 );
    bancocliente = bancocliente.leftJustified ( 20, ' ' );
    QString ent_recept = bancocliente.left ( 4 );
    cab_indob.append ( ent_recept.toAscii() );

    /// Oficina domiciliacion del fichero Longitud: 4
    bancocliente = bancocliente.right ( 16 );
    QString ofi_recept = bancocliente.left ( 4 );
    cab_indob.append ( ofi_recept.toAscii() );

    /// DC domiciliacion del fichero Longitud: 2
    bancocliente = bancocliente.right ( 12 );
    QString dc_recept = bancocliente.left ( 2 );
    cab_indob.append ( dc_recept.toAscii() );

    /// Oficina domiciliacion del fichero Longitud: 10
    bancocliente = bancocliente.right ( 10 );
    QString cta_recept = bancocliente.left ( 10 );
    cab_indob.append ( cta_recept.toAscii() );

    delete cur;

    /// Total Importe domiciliacion Longitud: 10
    QString importe = curcobro->valor ( "totalfactura" ).remove ( '.' ).remove ( ',' );
    importe = importe.rightJustified ( 10, '0' );
    cab_indob.append ( importe.toAscii() );

    /// Codigo para devoluciones Longitud : 6
    QString coddev = curcobro->valor ( "idfactura" );
    coddev = coddev.rightJustified ( 6, '0' );
    cab_indob.append ( coddev.toAscii() );

    /// Codigo de referencia interna Longitud 10
    QString codrefint = curcobro->valor ( "numfactura" );
    codrefint = codrefint.rightJustified ( 10, ' ' );
    cab_indob.append ( codrefint.toAscii() );

    /// Primer campo de concepto Longitud: 40
    QString concepto = "FRA " + curcobro->valor ( "codigoserie_factura" ) +  curcobro->valor ( "numfactura" ) + " " + curcobro->valor ( "descfactura" );
    concepto = concepto.leftJustified ( 40, ' ' );
    concepto = concepto.left ( 40 );
    cab_indob.append ( concepto.toAscii() );

    /// Espacio libre Longitud: 8
    cab_indob.append ( QString ( 8, ' ' ).toAscii() );
    out << cab_indob.toAscii() << "\n";
    delete curcobro;
    _depura ( "END Q19View::cobroQ19", 0 );
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
QByteArray Q19View::totalOrdenante ( QTextStream &out, QString importes, QString ordenantes, QString registros )
{
    _depura ( "Q19View::totalOrdenante", 0 );
    /// CABECERA TOTAL ORDENANTE
    QString cab_indob = "";
    /// Registro en Euros. Longitud: 2
    cab_indob.append ( "58" );

    /// Registro de codigo de dato: 80. Longitud: 2
    cab_indob.append ( "80" );

    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    cursor2 *cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='CIF'" );
    QString nif = cur->valor ( "valor" );
    delete cur;
    QString sufijo = "000";
    QString codpresent = nif + sufijo;
    codpresent = codpresent.rightJustified ( 12, '0' );
    if ( codpresent.size() > 12 ) _depura ( "CIF demasiado largo", 2 );
    cab_indob.append ( codpresent.toAscii() );


    /// Espacio libre Longitud: 12
    cab_indob.append ( QString ( 12, ' ' ).toAscii() );


    /// Espacio libre Longitud: 40
    cab_indob.append ( QString ( 40, ' ' ).toAscii() );

    /// Espacio libre Longitud: 20
    cab_indob.append ( QString ( 20, ' ' ).toAscii() );

    /// Suma de Importes del Ordenante Longitud: 10
    QString importe = importes;
    importe = importe.rightJustified ( 10, '0' );
    importe = importe.right ( 10 );
    cab_indob.append ( importe.toAscii() );

    /// Espacio libre Longitud: 6
    cab_indob.append ( QString ( 6, ' ' ).toAscii() );

    /// Numero de domiciliaciones del ordenante : 10
    QString coddev = ordenantes;
    coddev = coddev.rightJustified ( 10, '0' );
    coddev = coddev.right ( 10 );
    cab_indob.append ( coddev.toAscii() );

    /// Numero total registros del ordenante : 10
    coddev = registros;
    coddev = coddev.rightJustified ( 10, '0' );
    coddev = coddev.right ( 10 );
    cab_indob.append ( coddev.toAscii() );

    /// Espacio libre Longitud: 20
    cab_indob.append ( QString ( 20, ' ' ).toAscii() );

    /// Espacio libre Longitud: 18
    cab_indob.append ( QString ( 18, ' ' ).toAscii() );


    /// Imprimimos los resultados
    out << cab_indob.toAscii() << "\n";
    _depura ( "END Q19View::totalOrdenante", 0 );
    return cab_indob.toAscii();
}


///
/**
\param out
\param importes
\param domiciliaciones
\param registros
\return
**/
QByteArray Q19View::totalGeneral ( QTextStream &out, QString importes, QString domiciliaciones, QString registros )
{
    _depura ( "Q19View::totalGeneral", 0 );
    /// CABECERA INDIVIDUAL OBLIGATORIO
    QString cab_indob = "";
    /// Registro en Euros. Longitud: 2
    cab_indob.append ( "59" );
    /// Registro de codigo de dato: 80. Longitud: 2
    cab_indob.append ( "80" );
    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    cursor2 *cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='CIF'" );
    QString nif = cur->valor ( "valor" );
    delete cur;
    QString sufijo = "000";
    QString codpresent = nif + sufijo;
    codpresent = codpresent.rightJustified ( 12, '0' );
    codpresent = codpresent.right ( 12 );
    cab_indob.append ( codpresent.toAscii() );


    /// Espacio libre Longitud: 12
    cab_indob.append ( QString ( 12, ' ' ).toAscii() );


    /// Espacio libre Longitud: 40
    cab_indob.append ( QString ( 40, ' ' ).toAscii() );

    /// Numero de Ordenantes Longitud : 4
    /// PAra este algoritmo siempre es un ordenante
    QString coddev = "1";
    coddev = coddev.rightJustified ( 4, '0' );
    cab_indob.append ( coddev.toAscii() );

    /// Espacio libre Longitud: 16
    cab_indob.append ( QString ( 16, ' ' ).toAscii() );

    /// Suma de Importes Longitud: 10
    QString importe = importes;
    importe = importe.rightJustified ( 10, '0' );
    cab_indob.append ( importe.toAscii() );

    /// Espacio libre Longitud: 6
    cab_indob.append ( QString ( 6, ' ' ).toAscii() );

    /// Numero de domiciliaciones : 10
    coddev = domiciliaciones;
    coddev = coddev.rightJustified ( 10, '0' );
    cab_indob.append ( coddev.toAscii() );

    /// Numero total registros : 10
    coddev = registros;
    coddev = coddev.rightJustified ( 10, '0' );
    cab_indob.append ( coddev.toAscii() );

    /// Espacio libre Longitud: 20
    cab_indob.append ( QString ( 20, ' ' ).toAscii() );

    /// Espacio libre Longitud: 18
    cab_indob.append ( QString ( 18, ' ' ).toAscii() );


    /// Imprimimos los resultados
    out << cab_indob.toAscii() << "\n";
    _depura ( "END Q19View::totalGeneral", 0 );
    return cab_indob.toAscii();
}


///
/**
**/
void Q19View::on_mui_aceptar_clicked()
{
    _depura ( "Q19View::on_mui_aceptar_clicked", 0 );
    try {
        mensajeInfo ( "Generando" );

/// ----------------------------------

//    QString fileName = QFileDialog::getSaveFileName ( this, tr ( "Save File" ),
//                       "",
//                       tr ( "*.q19" ) );

        QFile file ( mui_archivo->text() );
        if ( !file.open ( QIODevice::WriteOnly | QIODevice::Text ) )
            return;

        QTextStream out ( &file );
        out.setCodec ( "LATIN1" );

        SubForm3 *sub = m_facturasList->mui_list;

        Fixed total ( "0.00" );
        int j = 0;
        /// Reseteamos los valores
        for ( int i = 0; i < sub->rowCount(); i++ ) {
            SDBRecord *rec = sub->lineaat ( i );
            rec->refresh();
            QString val = rec->DBvalue ( "selector" );
            if ( val == "TRUE" ) {
                /// La primera vez se ponen las cabeceras
                if ( j == 0 ) {
                    cabeceraPresentador ( out, rec->DBvalue ( "idfactura" ) );
                    cabeceraOrdenante ( out, rec->DBvalue ( "idfactura" ) );
                } // end if
                cobroQ19 ( out, rec->DBvalue ( "idfactura" ) );
                j++;
                total = total + Fixed ( rec->DBvalue ( "total" ) );
            } // end if
        } // end for

        mensajeInfo ( "Numero de Operaciones: " + QString::number ( j ) );
        mensajeInfo ( "Total del archivo: " + total.toQString() );
//    buscarBanco();

        totalOrdenante ( out, total.toQString().remove ( ',' ).remove ( '.' ), QString::number ( j ), QString::number ( j + 2 ) );
        totalGeneral ( out, total.toQString().remove ( ',' ).remove ( '.' ), QString::number ( j ), QString::number ( j + 4 ) );
        file.close();


/// -----------------------------------
        dialogChanges_cargaInicial();
        close();
    } catch ( ... ) {
        _depura ( "Error al guardar la ficha", 2 );
    } // end try
    _depura ( "END Q19View::on_mui_aceptar_clicked", 0 );
}



