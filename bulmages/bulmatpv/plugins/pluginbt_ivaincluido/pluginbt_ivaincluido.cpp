/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
 *                                                                         *
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#include <QtWidgets/QDockWidget>

#include "pluginbt_ivaincluido.h"
#include "blfunctions.h"
#include "btcompany.h"
#include "blescprinter.h"

typedef QMap<QString, BlFixed> base;

int BtTicket_agregarLinea_Post ( BtTicket *tick )
{
    BL_FUNC_DEBUG
    
    BlDbRecord *item = (BlDbRecord *) g_plugParams;

    item->addDbField ( "pvpivainclalbaran", BlDbField::DbInt, BlDbField::DbNothing, _( "IVA inc." ) );
    
    
    
    return 0;
}

int BtTicket_insertarArticulo_Post ( BtTicket *tick )
{

    BL_FUNC_DEBUG
    
    int valor = -1;
    static int semaforo = 0;
    
    if ( semaforo == 0 ) {
        semaforo = 1;

        /// Buscamos los parametros en la base de datos.
        QString query = "SELECT pvpivaincarticulo FROM articulo WHERE idarticulo = " + tick->lineaActBtTicket()->dbValue("idarticulo");
        BlDbRecordSet *cur = tick->mainCompany() ->loadQuery ( query );

	if ( !cur->eof() ) {
            tick->lineaActBtTicket()->setDbValue ( "pvpivainclalbaran", cur->value( "pvpivaincarticulo" ) );
        } // end if
        delete cur;

        valor = 0;
        semaforo = 0;
    } // end if
    
    

    return valor;
}


int BtTicket_ponerPrecio_Post ( BtTicket *tick ) {
    BL_FUNC_DEBUG

    /// Cogemos el dato del precio sin IVA porque es el precio marcado en el teclado y
    /// para nosotros corresponde al precio IVA incluido.
    QString precioIvaIncluido = tick->lineaActBtTicket()->dbValue("pvplalbaran");
    

    /// 1) Coge el idarticulo y averigua el tipo de IVA asignado a ese articulo
    BlDbRecordSet* rs = NULL;
    rs = tick->mainCompany()->load ( "SELECT t2.porcentasa_iva FROM articulo AS t1 INNER JOIN tasa_iva AS t2 ON t1.idtipo_iva = t2.idtipo_iva WHERE t1.idarticulo = $1 ORDER BY fechatasa_iva DESC LIMIT 1", tick->lineaActBtTicket()->dbValue("idarticulo") );

    BlFixed tasaIVA = BlFixed("0.00");
    
    if ( !rs->eof() ) {
        tasaIVA = BlFixed(rs->value( "porcentasa_iva" ));
    } // end if
    
    /// 3) Calcula y establece el precio IVA incluido y precio SIN IVA.
    BlFixed precioSinIvaNuevo =  BlFixed(precioIvaIncluido.toLatin1().constData()) / ( BlFixed("1.00") + (tasaIVA / BlFixed("100.00")) );

    tick->lineaActBtTicket()->setDbValue("pvplalbaran",  precioSinIvaNuevo.toQString() );
    tick->lineaActBtTicket()->setDbValue("pvpivainclalbaran", precioIvaIncluido );
    
    return 0;
}


int BtCompany_z(BtCompany * emp)
{
    BL_FUNC_DEBUG
    
    QString queryfechas;
    

    
    /// Obtenemos fecha de la ultima Z
    QString query = "SELECT idz, fechaz FROM z ORDER BY idz DESC LIMIT 1";
    BlDbRecordSet *curFechaUltimaZ = emp->loadQuery ( query );

    if ( curFechaUltimaZ->numregistros() == 0) {
        queryfechas = "SELECT DISTINCT fechaalbaran FROM albaran WHERE idz IS NULL ORDER BY fechaalbaran ASC";
    } else {
        queryfechas = "SELECT DISTINCT fechaalbaran FROM albaran WHERE idz IS NULL AND fechaalbaran >= '" + curFechaUltimaZ->value("fechaz") + "' ORDER BY fechaalbaran ASC";
    } // end if

    /// Buscamos las fechas de los tickets que quedan pendientes de hacer Z
    /// Hacemos una Z por cada fecha que exista en los tickets y no tenga Z asociada
    BlDbRecordSet *curfechas = emp->loadQuery ( queryfechas );
    
    while ( !curfechas->eof() ) {
        //blMsgInfo(curfechas->value("fechaalbaran"));
	
        emp->begin();
        query = "INSERT INTO z (idalmacen) VALUES(" + g_confpr->value( CONF_IDALMACEN_DEFECTO ) + ")";
        emp->runQuery ( query );
        
        query = "SELECT max(idz) AS id FROM z";
        BlDbRecordSet *cur = emp->loadQuery ( query );
        QString idz = cur->value( "id" );
        delete cur;
        
        query = "UPDATE albaran SET idz = " + idz + " WHERE idz IS NULL AND ticketalbaran = true AND fechaalbaran = '" + curfechas->value("fechaalbaran") + "'";
        emp->runQuery ( query );
        
	
	
        query = "SELECT count(idz) AS numtickets, sum(totalalbaran) AS total FROM albaran WHERE idz = " + idz;
        cur = emp->loadQuery ( query );
        
	
        QString numtickets = cur->value( "numtickets" );
        QString total = cur->value( "total" );

        if ( total == "" )
            total = "0";
        
        query = "UPDATE z SET totalz = " + total + ", numtickets = " + numtickets + ", fechaz = '" + curfechas->value("fechaalbaran") + "' WHERE idz = " + idz;
        emp->runQuery ( query );
        emp->commit();
        
        delete cur;

	emp->ticketActual() -> generateRML("informe_Z.txt");

	if (!g_confpr->value( CONF_CASHBOX_FILE).isEmpty() && g_confpr->value( CONF_CASHBOX_FILE) != "/dev/null") {
	    QString comando = "cat " + g_confpr->value(CONF_DIR_USER) + "informe_Z.txt" + "  > " + g_confpr->value( CONF_CASHBOX_FILE );
	    system ( comando.toLatin1().data() );
	} else if (g_confpr->value(CONF_CUPS_DEFAULT_PRINTER).isEmpty() || g_confpr->value(CONF_CUPS_DEFAULT_PRINTER).toLower() == "none") {
	    BlDebug::blDebug("Debe establecer el parametro CONF_CUPS_DEFAULT_PRINTER o CONF_CASHBOX_FILE para abrir el cajon " , 2);
	} else {
		blRawPrint ( "informe_Z.txt" );
	} // end if 
	
	
/*	
        QString querycont = "SELECT count(idalbaran) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz = " + idz + " AND ticketalbaran = true AND idforma_pago = " + g_confpr->value( CONF_IDFORMA_PAGO_CONTADO );
        BlDbRecordSet *cur1 = emp->loadQuery ( querycont );
        QString numticketscont = cur1->value( "numtickets" );
        QString totalcont = cur1->value( "total" );
        
        if ( totalcont == "" )
            totalcont = "0";
        
        delete cur1;

        QString queryvisa = "SELECT count(idalbaran) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz = " + idz + " AND ticketalbaran = true AND idforma_pago = " + g_confpr->value(CONF_IDFORMA_PAGO_VISA);

        BlDbRecordSet *cur2 = emp->loadQuery ( queryvisa );
        QString numticketsvisa = cur2->value( "numtickets" );
        QString totalvisa = cur2->value( "total" );

        if ( totalvisa == "" )
            totalvisa = "0";
        
        delete cur2;

        // ========================================

        QFile file (  g_confpr->value(CONF_DIR_USER) + "bulmatpv_z.txt" );
        if ( !file.open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
            BlDebug::blDebug ( "Error en la Impresion de ticket", 2 );
            return -1;
        } // end if
        
        file.write ( QString ( "Informe Z\n" ).toLatin1() );
        file.write ( QString ( "=========\n" ).toLatin1() );
        
        BlDbRecordSet *curemp = emp->loadQuery ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
        if ( !curemp->eof() ) {
            file.write ( curemp->value( "valor" ).toLatin1() );
            file.write ( "\n", 1 );
        } // end if
        delete curemp;
        
        file.write ( QString ( "====================================\n" ).toLatin1() );
        
        cur = emp->loadQuery ( "SELECT * FROM configuracion WHERE nombre='DireccionCompleta'" );
        if ( !cur->eof() ) {
            file.write ( cur->value( "valor" ).toLatin1() );
            file.write ( "\n", 1 );
        } // end if
        delete cur;

        cur = emp->loadQuery ( "SELECT * FROM configuracion WHERE nombre='CodPostal'" );
        if ( !cur->eof() ) {
            file.write ( cur->value( "valor" ).toLatin1() );
        } // end if
        delete cur;

        file.write ( QString ( " " ).toLatin1() );
        
        cur = emp->loadQuery ( "SELECT * FROM configuracion WHERE nombre='Ciudad'" );
        if ( !cur->eof() ) {
            file.write ( cur->value( "valor" ).toLatin1() );
            file.write ( QString ( " " ).toLatin1() );
        } // end if
        delete cur;

        cur = emp->loadQuery ( "SELECT * FROM configuracion WHERE nombre='Provincia'" );
        if ( !cur->eof() ) {
            file.write ( QString ( "(" ).toLatin1() );
            file.write ( cur->value( "valor" ).toLatin1() );
            file.write ( QString ( ")" ).toLatin1() );
            file.write ( "\n", 1 );
        } // end if
        delete cur;

        /// Imprimimos espacios
        file.write ( "\n \n", 3 );

        /// Imprimimos la fecha
        file.write ( QString ( "Fecha: " ).toLatin1() );
        //QDate fecha = QDate::currentDate();
        //QString sfecha = fecha.toString ( "d-M-yyyy" );
        //file.write ( sfecha.toLatin1() );
        file.write ( (curfechas->value("fechaalbaran")).toLatin1() );
        QTime hora = QTime::currentTime();
        QString stime = " " + hora.toString ( "HH:mm" );
        file.write ( stime.toLatin1() );
        file.write ( "\n", 1 );

        /// Imprimimos el almacen
        cur = emp->loadQuery ( "SELECT * FROM almacen WHERE idalmacen=" + g_confpr->value( CONF_IDALMACEN_DEFECTO ) );
        if ( !cur->eof() ) {
            file.write ( QString ( "Almacen: " ).toLatin1() );
            file.write ( cur->value( "nomalmacen" ).toLatin1() );
            file.write ( "\n", 1 );
        } // end if
        delete cur;

        file.write ( "\n", 1 );
        file.write ( "\n", 1 );

        // ============================================

        file.write ( QString ( "=======================\n" ).rightJustified ( 43, ' ' ).toLatin1() );

        QString str = "Num tickets " + numtickets.rightJustified ( 10, ' ' );
        file.write ( str.rightJustified ( 42, ' ' ).toLatin1() );
        file.write ( "\n", 1 );

        str = "Total " + total.rightJustified ( 10, ' ' );
        file.write ( str.rightJustified ( 42, ' ' ).toLatin1() );
        file.write ( "\n", 1 );

        str = "Num tickets Contado" + numticketscont.rightJustified ( 10, ' ' );
        file.write ( str.rightJustified ( 42, ' ' ).toLatin1() );
        file.write ( "\n", 1 );

        str = "Total Contado" + totalcont.rightJustified ( 10, ' ' );
        file.write ( str.rightJustified ( 42, ' ' ).toLatin1() );
        file.write ( "\n", 1 );

        str = "Num tickets Visa" + numticketsvisa.rightJustified ( 10, ' ' );
        file.write ( str.rightJustified ( 42, ' ' ).toLatin1() );
        file.write ( "\n", 1 );

        str = "Total Visa" + totalvisa.rightJustified ( 10, ' ' );
        file.write ( str.rightJustified ( 42, ' ' ).toLatin1() );
        file.write ( "\n", 1 );

        // ============================================
        // ============================================
        
        file.write ( QString ( "=======================\n" ).rightJustified ( 43, ' ' ).toLatin1() );
        file.write ( QString ( "=======================\n" ).rightJustified ( 43, ' ' ).toLatin1() );
        file.write ( QString ( "==== RESUMEN FAMILIAS ======\n" ).rightJustified ( 43, ' ' ).toLatin1() );
        
        // Informes por familias

        /// Imprimimos el almacen
        cur = emp->loadQuery ( "SELECT * FROM familia");
        
        while ( !cur->eof() ) {
            file.write ( QString ( "Familia: " ).toLatin1() );
            file.write (cur->value( "nombrefamilia" ).toLatin1() );
            file.write ( "\n", 1 );

            QString querycont = "SELECT sum(cantlalbaran) AS unidades, sum(pvpivainclalbaran * cantlalbaran)::NUMERIC(12,2) as total FROM lalbaran NATURAL LEFT JOIN articulo WHERE idalbaran IN (SELECT idalbaran FROM albaran WHERE idz="+idz+")  AND idfamilia = " + cur->value("idfamilia");
            BlDbRecordSet *cur1 = emp->loadQuery ( querycont );
            QString numticketscont = cur1->value( "unidades" );
            QString totalcont = cur1->value( "total" );
            
            if ( totalcont == "" )
                totalcont = "0";
            
            delete cur1;

            str = "Und. Vendidas: " + numticketscont.rightJustified ( 10, ' ' );
            file.write ( str.rightJustified ( 42, ' ' ).toLatin1() );
            file.write ( "\n", 1 );
        
            str = "Total:" + totalcont.rightJustified ( 10, ' ' );
            file.write ( str.rightJustified ( 42, ' ' ).toLatin1() );
            file.write ( "\n", 1 );

            file.write ( QString ( "=======================\n" ).rightJustified ( 43, ' ' ).toLatin1() );

            cur-> nextRecord();
        } // end while
        
        delete cur;
        // Fin informes por familias

        /// Imprimimos espacios
        file.write ( "\n \n \n \n", 7 );

        /// Preparamos para un codigo de barras
        /// Especificamos la altura del codigo de barras
        file.write ( "\x1Dh\x40", 3 );

        /// Especificamos que los caracteres vayan debajo del codigo de barras
        file.write ( "\x1DH\x02", 3 );

        /// Establecemos el tipo de codificacion para el codigo de barras
        file.write ( "\x1D", 1 );
        file.write ( "f\x01", 2 );

        /// Ponemos el ancho de la fuente a uno
        file.write ( "\x1D\x77\x01", 3 );
        /// Imprimimos la palabra top con el juego de caracteres 04
        file.write ( "\x1Dk\x04", 3 );
        file.write ( QString ( "ZZZ" ).toLatin1() );
        file.write ( " ", 1 );
        file.write ( idz.toLatin1() );
        file.write ( "\x00", 1 );

        /// Imprimimos espacios
        file.write ( "\n \n \n \n \n", 9 );

        /// El corte de papel.
        file.write ( "\x1D\x56\x01", 3 );
        file.close();

        // ========================================
 */
        curfechas->nextRecord();
 
    } // end while


    

    return -1;
}



int ArticuloListSubForm_ArticuloListSubForm_Post(ArticuloListSubForm *list) {
    BL_FUNC_DEBUG
    list->addSubFormHeader ( "pvpivaincarticulo", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, "PVP Iva Inc." );
    return 0;
}

