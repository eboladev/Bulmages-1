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

#include <QtWidgets/QMessageBox>
#include <QtGui/QCloseEvent>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <fstream>

#include "q19view.h"
#include "blconfiguration.h"
#include "bldatesearch.h"
#include "blfunctions.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
Q19View::Q19View ( CarteraCobrosList *fac, BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        m_carteraCobrosList = fac;

        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        mui_idbanco->setMainCompany ( comp );
        mui_idbanco->setidbanco ( "0" );

        setDbTableName ( "vencimientoc" );
        setDbFieldId ( "idvencimientoc" );



/// Calculamos el TOTAL y el numero de operaciones
        BlSubForm *sub = m_carteraCobrosList->mui_list;

	BlFixed a("0.00");
	int numop = 0;
        /// Reseteamos los valores
        for ( int i = 0; i < sub->rowCount(); i++ ) {
            BlDbSubFormRecord *rec = sub->lineaat ( i );
            rec->refresh();
            QString val = rec->dbValue ( "selector" );
            if ( val == "true" ) {
		if (rec->dbValue("descforma_pago") != "Transferencia") 
			blMsgInfo("Hay vencimientos que no pueden ser Remesados");
		if (rec->dbValue("estadovencimientoc") != "Pendiente") 
			blMsgInfo("Hay vencimientos con estado no Pendiente");
                a = a + BlFixed ( rec->dbValue ( "cantvencimientoc" ) );
		numop++;
            } // end if
        } // end for

	mui_total->setText(a.toQString());
	mui_numop->setText(QString::number(numop));

        insertWindow ( windowTitle(), this, false );
        dialogChanges_readValues();
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( tr ( "Error al crear el archivo" ) );
    } // end try
    
}


/** No precisa acciones adicionales en el destructor.
*/
Q19View::~Q19View()
{
    BL_FUNC_DEBUG
    /// ATENCION: Hacer esto es un error ya que puede machacar procesos dependientes del listado.

    
}



///
/**
\param out
\param idcobro
\return
**/
QByteArray Q19View::cabeceraPresentador ( QTextStream &out, QString idvencimientoc )
{
    BL_FUNC_DEBUG
    BlDbRecordSet *curcobro = mainCompany() ->loadQuery ( "SELECT * FROM vencimientoc LEFT JOIN factura ON vencimientoc.idfactura = factura.idfactura WHERE vencimientoc.idvencimientoc = " + idvencimientoc );

    BlDbRecordSet *curbanco = mainCompany() -> loadQuery ( "SELECT * FROM banco WHERE idbanco = " + mui_idbanco->idbanco() );

    /// CABECERA PRESENTADOR
    /// Generamos la cabecera presentador
    QString cab_present = "";
    /// Registro en Euros. Longitud: 2
    cab_present.append ( "51" );
    /// Registro de codigo de dato: 80. Longitud: 2
    cab_present.append ( "80" );

    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='CIF'" );
    QString nif = cur->value( "valor" );
    delete cur;
    QString sufijo = "000";
    QString codpresent = nif + sufijo;
    codpresent = codpresent.rightJustified ( 12, '0' );
    if ( codpresent.size() > 12 ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("El CIF del ordenante supera la longitud maxima.") );
    } // end if
    cab_present.append ( codpresent.toLatin1() );

    /// Fecha de emision del archivo
    QString fechaemfich = QDate::currentDate().toString ( "ddMMyy" );
    cab_present.append ( fechaemfich.toLatin1() );

    /// Espacio libre Longitud: 6
    cab_present.append ( QString ( 6, ' ' ).toLatin1() );

    /// Nombre del cliente Presentador Longitud: 40
    cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
    QString clientepresentador = cur->value( "valor" );
    delete cur;
    clientepresentador = clientepresentador.leftJustified ( 40, ' ' );
    clientepresentador = clientepresentador.left ( 40 );
    cab_present.append ( clientepresentador.toLatin1() );

    /// Espacio libre Longitud: 20
    cab_present.append ( QString ( 20, ' ' ).toLatin1() );

    /// Entidad Receptora del fichero Longitud: 4
    QString ent_recept = curbanco->value( "codentidadbanco" ).leftJustified ( 4, '0' );
    if ( ent_recept.size() > 4 ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Entidad bancaria supera longitud maxima.") );
    } // end if
    cab_present.append ( ent_recept.toLatin1() );

    /// Oficina Receptora del fichero Longitud: 4
    QString ofi_recept = curbanco->value( "codagenciabanco" ).leftJustified ( 4, '0' );
    if ( ofi_recept.size() > 4 ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Oficina bancaria supera longitud maxima.") );
    } // end if
    cab_present.append ( ofi_recept.toLatin1() );

    /// Espacio libre Longitud: 12
    cab_present.append ( QString ( 12, ' ' ).toLatin1() );

    /// Espacio libre Longitud: 40
    cab_present.append ( QString ( 40, ' ' ).toLatin1() );

    /// Espacio libre Longitud: 14
    cab_present.append ( QString ( 14, ' ' ).toLatin1() );
    out << cab_present.toLatin1()  << "\n";
    delete curcobro;
    delete curbanco;
    
    return cab_present.toLatin1();
}


///
/**
\param out
\param idcobro
\return
**/
QByteArray Q19View::cabeceraOrdenante ( QTextStream &out, QString idvencimientoc )
{
    BL_FUNC_DEBUG
    BlDbRecordSet *curcobro = mainCompany() ->loadQuery ( "SELECT * FROM vencimientoc LEFT JOIN factura ON vencimientoc.idfactura = factura.idfactura  WHERE vencimientoc.idvencimientoc = " + idvencimientoc );

    BlDbRecordSet *curbanco = mainCompany() -> loadQuery ( "SELECT * FROM banco WHERE idbanco = " + mui_idbanco->idbanco() );


    /// GENERAMOS LA CABECERA ORDENANTE
    /// REGISTRO DEL ORDENANTE
    QString cab_orden = "";
    /// Registro en Euros. Longitud: 2
    cab_orden.append ( "53" );
    /// Registro de codigo de dato: 80. Longitud: 2
    cab_orden.append ( "80" );

    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='CIF'" );
    QString nif = cur->value( "valor" );
    delete cur;
    QString sufijo = "000";
    QString codpresent = nif + sufijo;
    codpresent = codpresent.rightJustified ( 12, '0' );
    if ( codpresent.size() > 12 ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("CIF demasiado largo.") );
    } // end if
    cab_orden.append ( codpresent.toLatin1() );

    /// Fecha de emision del archivo
    QString fechaemfich = QDate::currentDate().toString ( "ddMMyy" );
    cab_orden.append ( fechaemfich.toLatin1() );

    /// Fecha de cargo
    QString fechacargo = blNormalizeDate ( curcobro->value( "fechacobro" ) ).toString ( "ddMMyy" );
    cab_orden.append ( fechacargo.toLatin1() );

    /// Nombre del cliente Ordenante Longitud: 40
    cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
    QString clientepresentador = cur->value( "valor" );
    delete cur;
    clientepresentador = clientepresentador.leftJustified ( 40, ' ' );
    if ( clientepresentador.size() > 40 ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Nombre de empresa demasiado largo.") );
    } // end if
    cab_orden.append ( clientepresentador.toLatin1() );

    /// Entidad Receptora del fichero Longitud: 4
    QString ent_recept = curbanco->value( "codentidadbanco" ).leftJustified ( 4, '0' );
    if ( ent_recept.size() > 4 ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Entidad bancaria demasiado larga.") );
    } // end if
    cab_orden.append ( ent_recept.toLatin1() );

    /// Oficina Receptora del fichero Longitud: 4
    QString ofi_recept = curbanco->value( "codagenciabanco" ).leftJustified ( 4, '0' );
    if ( ofi_recept.size() > 4 ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Oficina bancaria demasiado larga.") );
    } // end if
    cab_orden.append ( ofi_recept.toLatin1() );

    /// DC Receptora del fichero Longitud: 2
    QString dc_recept = curbanco->value( "dcbanco" ).leftJustified ( 2, '0' );
    if ( dc_recept.size() > 4 ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Digito de control demasiado largo.") );
    } // end if
    cab_orden.append ( dc_recept.toLatin1() );

    /// Oficina Receptora del fichero Longitud: 10
    QString cta_recept = curbanco->value( "numcuentabanco" ).leftJustified ( 10, '0' );
    if ( cta_recept.size() > 10 ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Cuenta bancaria demasiado larga.") );
    } // end if
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
    delete curbanco;
    
    return cab_orden.toLatin1();
}


///
/**
\param out
\param idcobro
\return
**/
QByteArray Q19View::cobroQ19 ( QTextStream &out, QString idvencimientoc )
{
    BL_FUNC_DEBUG
    BlDbRecordSet *curcobro = mainCompany() ->loadQuery ( "SELECT * FROM vencimientoc LEFT JOIN factura ON vencimientoc.idfactura = factura.idfactura  WHERE vencimientoc.idvencimientoc = " + idvencimientoc );

//    BlDbRecordSet *curbanco = mainCompany()-> loadQuery( "SELECT * FROM banco WHERE idbanco = " + mui_idbanco->idbanco() );


    /// CABECERA INDIVIDUAL OBLIGATORIO
    QString cab_indob = "";
    /// Registro en Euros. Longitud: 2
    cab_indob.append ( "56" );
    /// Registro de codigo de dato: 80. Longitud: 2
    cab_indob.append ( "80" );
    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='CIF'" );
    QString nif = cur->value( "valor" );
    delete cur;
    QString sufijo = "000";
    QString codpresent = nif + sufijo;
    codpresent = codpresent.rightJustified ( 12, '0' );
    if ( codpresent.size() > 12 ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("CIF cliente demasiado largo.") );
    } // end if
    cab_indob.append ( codpresent.toLatin1() );

    /// Codigo de referencia Longitud: 12
    QString cod_ref = curcobro->value( "idcliente" );
    cod_ref = cod_ref.rightJustified ( 12, '0' );
    cab_indob.append ( cod_ref.toLatin1() );


    /// Nombre del titular de la domiciliacion: 40
    cur = mainCompany() ->loadQuery ( "SELECT * FROM cliente WHERE idcliente= " + curcobro->value( "idcliente" ) );
    QString clientedomiciliacion = cur->value( "nomcliente" );

    clientedomiciliacion = clientedomiciliacion.leftJustified ( 40, ' ' );
    if ( clientedomiciliacion.size() > 40 ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Nombre cliente demasiado largo.") );
    } // end if
    cab_indob.append ( clientedomiciliacion.toLatin1() );


    /// Entidad domiciliacion del fichero Longitud: 4
    QString bancocliente = cur->value( "bancocliente" ).remove ( QChar ( ' ' ) );
    if ( bancocliente.size() != 20 ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Banco del cliente invalido en el cobro.") );
    } // end if
    bancocliente = bancocliente.leftJustified ( 20, ' ' );
    QString ent_recept = bancocliente.left ( 4 );
    cab_indob.append ( ent_recept.toLatin1() );

    /// Oficina domiciliacion del fichero Longitud: 4
    bancocliente = bancocliente.right ( 16 );
    QString ofi_recept = bancocliente.left ( 4 );
    cab_indob.append ( ofi_recept.toLatin1() );

    /// DC domiciliacion del fichero Longitud: 2
    bancocliente = bancocliente.right ( 12 );
    QString dc_recept = bancocliente.left ( 2 );
    cab_indob.append ( dc_recept.toLatin1() );

    /// Oficina domiciliacion del fichero Longitud: 10
    bancocliente = bancocliente.right ( 10 );
    QString cta_recept = bancocliente.left ( 10 );
    cab_indob.append ( cta_recept.toLatin1() );

    delete cur;

    /// Total Importe domiciliacion Longitud: 10
    QString importe = curcobro->value( "cantvencimientoc" ).remove ( '.' ).remove ( ',' );
    importe = importe.rightJustified ( 10, '0' );
    cab_indob.append ( importe.toLatin1() );

    /// Codigo para devoluciones Longitud : 6
    QString coddev = curcobro->value( "idvencimientoc" );
    coddev = coddev.rightJustified ( 6, '0' );
    cab_indob.append ( coddev.toLatin1() );

    /// Codigo de referencia interna Longitud 10
    QString codrefint = curcobro->value( "numfactura" );
    codrefint = codrefint.rightJustified ( 10, ' ' );
    cab_indob.append ( codrefint.toLatin1() );

    /// Primer campo de concepto Longitud: 40
    QString concepto = "FRA " + curcobro->value( "codigoserie_factura" ) +  curcobro->value( "numfactura" ) + " " + curcobro->value( "descfactura" );
    concepto = concepto.leftJustified ( 40, ' ' );
    concepto = concepto.left ( 40 );
    cab_indob.append ( concepto.toLatin1() );

    /// Espacio libre Longitud: 8
    cab_indob.append ( QString ( 8, ' ' ).toLatin1() );
    out << cab_indob.toLatin1() << "\n";
    delete curcobro;
    
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
QByteArray Q19View::totalOrdenante ( QTextStream &out, QString importes, QString ordenantes, QString registros )
{
    BL_FUNC_DEBUG
    /// CABECERA TOTAL ORDENANTE
    QString cab_indob = "";
    /// Registro en Euros. Longitud: 2
    cab_indob.append ( "58" );

    /// Registro de codigo de dato: 80. Longitud: 2
    cab_indob.append ( "80" );

    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='CIF'" );
    QString nif = cur->value( "valor" );
    delete cur;
    QString sufijo = "000";
    QString codpresent = nif + sufijo;
    codpresent = codpresent.rightJustified ( 12, '0' );
    if ( codpresent.size() > 12 ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("CIF demasiado largo.") );
    } // end if
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
QByteArray Q19View::totalGeneral ( QTextStream &out, QString importes, QString domiciliaciones, QString registros )
{
    BL_FUNC_DEBUG
    /// CABECERA INDIVIDUAL OBLIGATORIO
    QString cab_indob = "";
    /// Registro en Euros. Longitud: 2
    cab_indob.append ( "59" );
    /// Registro de codigo de dato: 80. Longitud: 2
    cab_indob.append ( "80" );
    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    /// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='CIF'" );
    QString nif = cur->value( "valor" );
    delete cur;
    QString sufijo = "000";
    QString codpresent = nif + sufijo;
    codpresent = codpresent.rightJustified ( 12, '0' );
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
    
    return cab_indob.toLatin1();
}


///
/**
**/
void Q19View::on_mui_aceptar_clicked()
{
    BL_FUNC_DEBUG
    try {


        QFile file ( mui_archivo->text() );
        if ( !file.open ( QIODevice::WriteOnly | QIODevice::Text ) )
            return;

        QTextStream out ( &file );
        out.setCodec ( "LATIN1" );

        BlSubForm *sub = m_carteraCobrosList->mui_list;

        BlFixed total ( "0.00" );
        int j = 0;
        /// Reseteamos los valores
        for ( int i = 0; i < sub->rowCount(); i++ ) {
            BlDbSubFormRecord *rec = sub->lineaat ( i );
            rec->refresh();
            QString val = rec->dbValue ( "selector" );
            if ( val == "true" ) {
                /// La primera vez se ponen las cabeceras
                if ( j == 0 ) {
                    cabeceraPresentador ( out, rec->dbValue ( "idvencimientoc" ) );
                    cabeceraOrdenante ( out, rec->dbValue ( "idvencimientoc" ) );
                } // end if
                cobroQ19 ( out, rec->dbValue ( "idvencimientoc" ) );
                j++;
                total = total + BlFixed ( rec->dbValue ( "cantvencimientoc" ) );
            } // end if
        } // end for

        blMsgInfo ( "Numero de Operaciones: " + QString::number ( j ) );
        blMsgInfo ( "Total del archivo: " + total.toQString() );

        totalOrdenante ( out, total.toQString().remove ( ',' ).remove ( '.' ), QString::number ( j ), QString::number ( j + 2 ) );
        totalGeneral ( out, total.toQString().remove ( ',' ).remove ( '.' ), QString::number ( j ), QString::number ( j + 4 ) );
        file.close();

        dialogChanges_readValues();
        close();
    } catch ( ... ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Error al guardar.") );
    } // end try
    
}


void Q19View::on_mui_buscararchivo_clicked() {
 QString fileName = QFileDialog::getSaveFileName(this, tr("Guardar archivo"),
                                                 "",
                                                 tr("Remesas Bancarias (*.q19)"));
mui_archivo->setText(fileName);
}

